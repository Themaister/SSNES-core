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

// Depends on regions, and probably not inaccurate anyways. :)
#define CYCLES_PER_SCANLINE (1364)
#define SCANLINES_PER_FRAME (262)
#define CYCLES_PER_FRAME (CYCLES_PER_SCANLINE * SCANLINES_PER_FRAME)

static void cpu_init_registers(void)
{
   REGS.e = true;
   cpu_set_p(0x30);
   REGS.sp.w = 0x100;
   STATUS.pending_irq.reset = true;

   STATUS.cycles = CYCLES_PER_FRAME + 100;
   STATUS.smp_cycles = CYCLES_PER_FRAME + 100;
}

void ssnes_cpu_reset(void)
{
   memset(&ssnes_state, 0, sizeof(ssnes_state)); 
   memset(&ssnes_state.mem.wram, 0x55, sizeof(ssnes_state.mem.wram));
   cpu_init_registers();
}


static void cpu_check_cycles(void)
{
   if (STATUS.ppu.scanline_ready)
   {
      ssnes_ppu_scanline(STATUS.ppu.vcount - 1);
      STATUS.ppu.scanline_ready = false;
   }

   // Check if we have to jump to SMP or whatever.
   if (STATUS.smp_state && STATUS.cycles > STATUS.smp_cycles)
   {
      STATUS.smp_cycles += ssnes_smp_run(STATUS.cycles - STATUS.smp_cycles);
   }
   else if ((int)STATUS.cycles - (int)STATUS.smp_cycles > 100)
      STATUS.smp_cycles += ssnes_smp_run(STATUS.cycles - STATUS.smp_cycles);

   STATUS.smp_state = false;

   // Check HDMA
   if (STATUS.hdma_run && STATUS.ppu.hcount > 0x116 * 4)
   {
      if (STATUS.hdma_run)
      {
         hdma_run();
         STATUS.hdma_run = false;
      }
   }

   // hblank
   PPU.hvbjoy |= isel_gte(STATUS.ppu.hcount, 0x121 * 4, 0x40, 0);
}

static void cpu_finish_cycles(void)
{
   if (STATUS.smp_cycles < CYCLES_PER_FRAME)
      ssnes_smp_run(STATUS.cycles - STATUS.smp_cycles);
}

static inline void cpu_check_vhirq(void)
{
   if (STATUS.pending_irq.irq_fired)
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

   if (REGS.p.i && STATUS.pending_irq.irq) // Mask away the IRQ.
   {
      STATUS.pending_irq.irq = false;
      STATUS.pending_irq.irq_fired = true;
   }
}

static void cpu_check_irq(void)
{
   if (STATUS.ppu.frame_ready)
   {
      ssnes_video_cb(ssnes_ppu_buffer(), 256, 224);
      STATUS.ppu.frame_ready = false;

      // OAM reset (correct? :V) on vblank if not force-blank.
      iup_if(STATUS.regs.oam_addr, ~PPU.inidisp & 0x80, STATUS.regs.oam_addr_buf);

      iup_if(STATUS.pending_irq.nmi, STATUS.regs.nmitimen & 0x80, true);

      STATUS.irq.nmi_flag = 0x80;

      ssnes_poll_cb();
      // Joypad autopoll
      input_autopoll(STATUS.regs.nmitimen & 0x01);

      PPU.stat78 ^= 0x80; // Interlace field.
   }

   cpu_check_vhirq();
}

static inline void print_registers(void)
{
   dprintf("A: %04x X: %04x Y: %04x ", (unsigned)REGS.a.w, (unsigned)REGS.x.w, (unsigned)REGS.y.w);
   dprintf("S: %04x D: %04x ", (unsigned)REGS.sp.w, (unsigned)REGS.dp);
   dprintf("DB: %02x ", (unsigned)(REGS.db >> 16));
   dputc(REGS.p.n ? 'N' : 'n');
   dputc(REGS.p.v ? 'V' : 'v');
   dputc(REGS.p.m ? 'M' : 'm');
   dputc(REGS.p.x ? 'X' : 'x');
   dputc(REGS.p.d ? 'D' : 'd');
   dputc(REGS.p.i ? 'I' : 'i');
   dputc(REGS.p.z ? 'Z' : 'z');
   dputc(REGS.p.c ? 'C' : 'c');
   dprintf(" V: %03u H: %04u\n", STATUS.ppu.vcount, STATUS.ppu.hcount);
}

static inline unsigned update_ppu_cycles(unsigned last_cycles)
{
   unsigned cycles = STATUS.cycles - last_cycles;
   STATUS.ppu.hcount += cycles;
   if (STATUS.ppu.hcount >= CYCLES_PER_SCANLINE)
   {
      STATUS.ppu.hcount -= CYCLES_PER_SCANLINE;
      STATUS.ppu.vcount++;

      iup_lt(STATUS.ppu.scanline_ready, STATUS.ppu.vcount, 226, true);
      iup_lt(STATUS.hdma_run, STATUS.ppu.vcount, 225, true);
      PPU.vsync = isel_gte(STATUS.ppu.vcount, 225, true, false);
      STATUS.ppu.frame_ready = isel_eq(STATUS.ppu.vcount, 225, true, false);
      iup_eq(PPU.hvbjoy, STATUS.ppu.vcount, 225, 0x81);
      iup_eq(PPU.hvbjoy, STATUS.ppu.vcount, 228, 0x80);
      PPU.hvbjoy &= ~0x40; // Not in hblank anymore.

      // HIRQ
      iup_eq(STATUS.pending_irq.irq_fired, STATUS.regs.nmitimen & 0x30, 0x10, false);
   }

   return STATUS.cycles;
}

static void cpu_start_frame(void)
{
   STATUS.cycles -= CYCLES_PER_FRAME;
   STATUS.smp_cycles -= CYCLES_PER_FRAME;

   STATUS.ppu.vcount = 0;
   STATUS.ppu.hcount = 0;

   STATUS.irq.nmi_flag = 0;
   STATUS.hdma_run = false;

   PPU.hvbjoy = 0;
   STATUS.pending_irq.irq_fired = false;
   PPU.vsync = false;
}

#ifdef SSNES_DEBUG
unsigned ssnes_debug_instruction_count[256];
#endif

void ssnes_cpu_run_frame(void)
{
   cpu_start_frame();
   unsigned last_cycles = STATUS.cycles;
   
   // HDMA
   hdma_frame_init();

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
            dprintf("RESET\n");
            iup_if(REGS.wai_quit, REGS.wai, true);
            STATUS.pending_irq.reset = false;
            cpu_op_interrupt_reset_e();
            REGS.sp.w = 0x1ff;
         }
         else if (STATUS.pending_irq.nmi)
         {
            dprintf("NMI\n");
            iup_if(REGS.wai_quit, REGS.wai, true);
            STATUS.pending_irq.nmi = false;

            if (REGS.e)
               cpu_op_interrupt_nmi_e();
            else
               cpu_op_interrupt_nmi_n();
         }
         else if (STATUS.pending_irq.irq)
         {
            dprintf("IRQ\n");
            STATUS.irq.irq_flag = 0x80;
            iup_if(REGS.wai_quit, REGS.wai, true);
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
            uint8_t opcode = cpu_read_pc();

            dprintf("%06x ", (unsigned)REGS.pc.l - 1);
            dprintf("%-14s", ssnes_cpu_opcode_names[opcode]); 
            print_registers();

            ssnes_cpu_op_table[opcode]();

#ifdef SSNES_DEBUG
            ssnes_debug_instruction_count[opcode]++;
#endif
         }
      }

      last_cycles = update_ppu_cycles(last_cycles);

      cpu_check_cycles();
      cpu_check_irq();
   }

   cpu_finish_cycles();
}
