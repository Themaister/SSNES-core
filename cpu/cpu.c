#include "state.h"
#include "cpu.h"
#include "op.h"
#include "table.h"
#include <string.h>
#include <stdio.h>

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

static void cpu_check_cycles(void)
{
   if (STATUS.ppu.scanline_ready)
   {
      // ppu_run_scanline(STATUS.ppu.vcount);
      STATUS.ppu.scanline_ready = false;
   }
}

static void cpu_check_irq(void)
{
   if (STATUS.ppu.nmi_ready)
   {
      //system_refresh_frame();
      // Set NMI active here.
      STATUS.ppu.nmi_ready = false;
   }
}

static void print_registers(void)
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
      STATUS.ppu.scanline_ready = true;

      if (STATUS.ppu.vcount == 224)
         STATUS.ppu.nmi_ready = true;
      else if (STATUS.ppu.vcount >= 262)
         STATUS.ppu.vcount = 0;
   }

   return STATUS.cycles;
}

#define CYCLES_PER_FRAME (262 * 1364)

void ssnes_cpu_run_frame(void)
{
   STATUS.cycles = 0;
   STATUS.ppu.vcount = 0;
   STATUS.ppu.hcount = 0;
   unsigned last_cycles = 0;

   while (STATUS.cycles < CYCLES_PER_FRAME)
   {
      if (STATUS.pending_irq.reset)
      {
         REGS.wai = false;
         STATUS.pending_irq.reset = false;
         cpu_op_interrupt_reset_e();
      }
      else if (STATUS.pending_irq.nmi)
      {
         REGS.wai = false;
         STATUS.pending_irq.nmi = false;

         if (REGS.e)
            cpu_op_interrupt_nmi_e();
         else
            cpu_op_interrupt_nmi_n();
      }
      else if (STATUS.pending_irq.irq)
      {
         REGS.wai = false;
         STATUS.pending_irq.irq = false;

         if (REGS.e)
            cpu_op_interrupt_irq_e();
         else
            cpu_op_interrupt_irq_n();
      }

      if (!REGS.wai && !REGS.stp)
      {
         fprintf(stderr, "======================================\n");
         fprintf(stderr, "PC: %04x, V = %3u, H = %4u\n", (unsigned)REGS.pc.w.l, STATUS.ppu.vcount, STATUS.ppu.hcount);
         uint8_t opcode = cpu_read_pc();
         fprintf(stderr, "Opcode: 0x%02x || %s\n", (unsigned)opcode, ssnes_cpu_opcode_names[opcode]);
         ssnes_cpu_op_table[opcode]();
         STATUS.cycles += ssnes_cpu_cycle_table[opcode];

         print_registers();
         fprintf(stderr, "======================================\n");
      }

      last_cycles = update_ppu_cycles(last_cycles);

      cpu_check_cycles();
      cpu_check_irq();
   }
}
