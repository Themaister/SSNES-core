#include "cpu.h"
#include "cpu/op_pc.h"
#include <string.h>

void cpu_init(void)
{
   cpu_reset();
}

static void cpu_init_registers(void)
{
   REGS.e = true;
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

void cpu_run_frame(void)
{
   CPU.status.cycles = 0;
   unsigned cycles_per_frame = 1000;

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
         uint8_t opcode = cpu_read_pc();
         op_table[opcode]();
         CPU.status.cycles += cycle_table[opcode];
      }

      cpu_check_cycles();
      cpu_check_irq();

      CPU.status.cycles++;
   }
}
