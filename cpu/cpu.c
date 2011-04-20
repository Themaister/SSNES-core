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
   // Meh. Probably do IRQ updates, PPU swaps, SMP swaps... etc here.
}

static void cpu_check_irq(void)
{
   //system_refresh_frame();
}

void cpu_run_frame(void)
{
   unsigned cycle_count = 0;
   unsigned cycles_per_frame = CPU.status.cycles_per_frame;

   while (cycle_count < cycles_per_frame)
   {
      if (CPU.status.pending_irq.reset)
      {
         state.cpu.status.pending_irq.reset = false;
         cpu_op_interrupt_reset_e();
      }
      else if (CPU.status.pending_irq.nmi)
      {
         CPU.status.pending_irq.nmi = false;

         if (REGS.e)
            cpu_op_interrupt_nmi_e();
         else
            cpu_op_interrupt_nmi_n();
      }
      else if (CPU.status.pending_irq.irq)
      {
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
         cycle_count += cycle_table[opcode];
         // More logic here to have variable opcode duration.
      }

      cpu_check_cycles();
      cpu_check_irq();
   }
}
