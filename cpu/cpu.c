#include "system/state.h"
#include "cpu.h"
#include "op.h"
#include "table.h"
#include <string.h>
#include <stdio.h>
#include "dma.h"
#include "libsnes/interface.h"
#include "input.h"
#include "smp/smp.h"

void ssnes_cpu_init(void)
{
   ssnes_cpu_reset();
}

void ssnes_cpu_deinit(void)
{}

static void cpu_init_registers(void)
{
   REGS.e = true;
   cpu_set_p(0x30);
   REGS.sp.b.h = 0x01;
   STATUS.pending_irq.reset = true;
}

void ssnes_cpu_reset(void)
{
   memset(&ssnes_state, 0, sizeof(ssnes_state)); 
   cpu_init_registers();
}

// Depends.
#define CYCLES_PER_FRAME (262 * 1364)

static void cpu_check_cycles(void)
{
   if (STATUS.ppu.scanline_ready)
   {
      ssnes_ppu_scanline(STATUS.ppu.vcount - 1);
      STATUS.ppu.scanline_ready = false;
   }

   // Check if we have to jump to SMP/DSP or whatever.
   if (STATUS.smp_state && STATUS.cycles > STATUS.smp_cycles)
   {
      STATUS.smp_cycles += ssnes_smp_run(STATUS.cycles - STATUS.smp_cycles);
      STATUS.smp_state = false;
   }
   else if ((int)STATUS.cycles - (int)STATUS.smp_cycles > 500)
      STATUS.smp_cycles += ssnes_smp_run(STATUS.cycles - STATUS.smp_cycles);
}

static void cpu_finish_cycles(void)
{
   if (STATUS.smp_cycles < CYCLES_PER_FRAME)
      ssnes_smp_run(STATUS.cycles - STATUS.smp_cycles);
}

static inline void cpu_check_vhirq(void)
{
   if (REGS.p.i || STATUS.pending_irq.irq_fired) // IRQ disable or already fired ...
      return;

   switch (STATUS.regs.nmitimen & 0x30)
   {
      case 0x20: // VIRQ
         iup_gte(STATUS.pending_irq.irq, STATUS.cycles, STATUS.irq.vtrig, true);
         break;

      case 0x10: // HIRQ
         iup_gte(STATUS.pending_irq.irq, STATUS.ppu.hcount, STATUS.irq.htrig, true);
         break;

      case 0x30: // V/H IRQ
         iup_gte(STATUS.pending_irq.irq, STATUS.cycles, STATUS.irq.vhtrig, true);
         break;
   }
}

static void cpu_check_irq(void)
{
   if (STATUS.ppu.frame_ready)
   {
      ssnes_video_cb(PPU.buffer, 256, 224);
      STATUS.ppu.frame_ready = false;

      iup_if(STATUS.pending_irq.nmi, STATUS.regs.nmitimen & 0x80, true);

      ssnes_poll_cb();
      // Joypad autopoll
      if (STATUS.regs.nmitimen & 0x01)
         input_autopoll();
   }

   cpu_check_vhirq();
}

static inline void print_registers(void)
{
   fprintf(stderr, "\tA: %04x  X: %04x  Y: %04x  ", (unsigned)REGS.a.w, (unsigned)REGS.x.w, (unsigned)REGS.y.w);
   fprintf(stderr, "DP: %04x  SP: %04x  ", (unsigned)(REGS.dp >> 8), (unsigned)REGS.sp.w);
   fprintf(stderr, "DB: %02x  PB: %02x  ", (unsigned)(REGS.db >> 16), (unsigned)REGS.pc.w.h);
   fputc(REGS.p.n ? 'N' : 'n', stderr);
   fputc(REGS.p.v ? 'V' : 'v', stderr);
   fputc(REGS.p.m ? 'M' : 'm', stderr);
   fputc(REGS.p.x ? 'X' : 'x', stderr);
   fputc(REGS.p.d ? 'D' : 'd', stderr);
   fputc(REGS.p.i ? 'I' : 'i', stderr);
   fputc(REGS.p.z ? 'Z' : 'z', stderr);
   fputc(REGS.p.c ? 'C' : 'c', stderr);
   fputc('\n', stderr);
}

static inline unsigned update_ppu_cycles(unsigned last_cycles)
{
   unsigned cycles = STATUS.cycles - last_cycles;
   STATUS.ppu.hcount += cycles;
   if (STATUS.ppu.hcount >= 1364)
   {
      STATUS.ppu.hcount -= 1364;
      STATUS.ppu.vcount++;

      iup_lt(STATUS.ppu.scanline_ready, STATUS.ppu.vcount, 226, true);
      PPU.vsync = isel_gte(STATUS.ppu.vcount, 225, true, false);
      STATUS.ppu.frame_ready = isel_eq(STATUS.ppu.vcount, 225, true, false);
      iup_eq(PPU.hvbjoy, STATUS.ppu.vcount, 225, 0x81);
      iup_eq(PPU.hvbjoy, STATUS.ppu.vcount, 228, 0x80);

      // HIRQ
      iup_eq(STATUS.pending_irq.irq_fired, STATUS.regs.nmitimen & 0x30, 0x10, false);
   }

   return STATUS.cycles;
}


void ssnes_cpu_run_frame(void)
{
   STATUS.cycles = 0;
   STATUS.smp_cycles = 0;
   STATUS.ppu.vcount = 0;
   STATUS.ppu.hcount = 0;
   PPU.hvbjoy = 0;
   STATUS.pending_irq.irq_fired = false;
   PPU.vsync = false;
   unsigned last_cycles = 0;

   while (STATUS.cycles < CYCLES_PER_FRAME)
   {
      if (STATUS.dma_enable)
      {
         cpu_run_dma();
      }
      else
      {
         if (STATUS.pending_irq.reset)
         {
            REGS.wai_quit = true;
            STATUS.pending_irq.reset = false;
            cpu_op_interrupt_reset_e();
         }
         else if (STATUS.pending_irq.nmi)
         {
            REGS.wai_quit = true;
            STATUS.pending_irq.nmi = false;

            if (REGS.e)
               cpu_op_interrupt_nmi_e();
            else
               cpu_op_interrupt_nmi_n();
         }
         else if (STATUS.pending_irq.irq)
         {
            REGS.wai_quit = true;
            STATUS.pending_irq.irq = false;
            STATUS.pending_irq.irq_fired = true;

            if (REGS.e)
               cpu_op_interrupt_irq_e();
            else
               cpu_op_interrupt_irq_n();
         }

         if (STATUS.dma_enable)
         {
            cpu_run_dma();
         }
         else
         {
            //fprintf(stderr, "======================================\n");
            //fprintf(stderr, "PC: %04x, V = %3u, H = %4u\n", (unsigned)REGS.pc.w.l, STATUS.ppu.vcount, STATUS.ppu.hcount);
            uint8_t opcode = cpu_read_pc();
            //fprintf(stderr, "Opcode: 0x%02x || %s\n", (unsigned)opcode, ssnes_cpu_opcode_names[opcode]);

            ssnes_cpu_op_table[opcode]();

            STATUS.cycles += ssnes_cpu_cycle_table[opcode];

            //print_registers();
            //fprintf(stderr, "======================================\n");
         }
      }

      last_cycles = update_ppu_cycles(last_cycles);

      cpu_check_cycles();
      cpu_check_irq();
   }

   cpu_finish_cycles();
}
