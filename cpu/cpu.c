#include "cpu.h"
#include "cpu/op_pc.h"
#include <string.h>
#include <stdio.h>

void cpu_init(void)
{
   cpu_reset();
}

static void cpu_init_registers(void)
{
   REGS.e = true;
   cpu_set_p(0x30);
   REGS.sp.b.h = 0x01;
   CPU.status.pending_irq.reset = true;
}

void cpu_reset(void)
{
   memset(&state, 0, sizeof(state)); 
   cpu_init_registers();
}

static void cpu_check_cycles(void)
{
   // Check when to jump to SMP/PPU, etc.
}

static void cpu_check_irq(void)
{
   // Update IRQ status here.
   //system_refresh_frame();
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

void cpu_run_frame(void)
{
   CPU.status.cycles = 0;
   unsigned cycles_per_frame = 10000;

   while (CPU.status.cycles < cycles_per_frame)
   {
      if (CPU.status.pending_irq.reset)
      {
         REGS.wai = false;
         state.cpu.status.pending_irq.reset = false;
         cpu_op_interrupt_reset_e();
      }
      else if (CPU.status.pending_irq.nmi)
      {
         REGS.wai = false;
         CPU.status.pending_irq.nmi = false;

         if (REGS.e)
            cpu_op_interrupt_nmi_e();
         else
            cpu_op_interrupt_nmi_n();
      }
      else if (CPU.status.pending_irq.irq)
      {
         REGS.wai = false;
         state.cpu.status.pending_irq.irq = false;

         if (REGS.e)
            cpu_op_interrupt_irq_e();
         else
            cpu_op_interrupt_irq_n();
      }

      if (!REGS.wai && !REGS.stp)
      {
         fprintf(stderr, "======================================\n");
         fprintf(stderr, "PC: %04x\n", (unsigned)REGS.pc.w.l);
         uint8_t opcode = cpu_read_pc();
         fprintf(stderr, "Opcode: 0x%02x || %s\n", (unsigned)opcode, opcode_names[opcode]);
         op_table[opcode]();
         CPU.status.cycles += cycle_table[opcode];
         print_registers();
         fprintf(stderr, "======================================\n");
      }

      cpu_check_cycles();
      cpu_check_irq();

      CPU.status.cycles++;
   }
}
