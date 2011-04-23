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
   if (CPU.status.ppu.scanline_ready)
   {
      // ppu_run_scanline(CPU.status.ppu.vcount);
      CPU.status.ppu.scanline_ready = false;
   }
}

static void cpu_check_irq(void)
{
   if (CPU.status.ppu.nmi_ready)
   {
      //system_refresh_frame();
      // Set NMI active here.
      CPU.status.ppu.nmi_ready = false;
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
   unsigned cycles = CPU.status.cycles - last_cycles;
   CPU.status.ppu.hcount += cycles;
   if (CPU.status.ppu.hcount >= 1364)
   {
      CPU.status.ppu.hcount -= 1364;
      CPU.status.ppu.vcount++;
      CPU.status.ppu.scanline_ready = true;

      if (CPU.status.ppu.vcount == 224)
      {
         CPU.status.ppu.nmi_ready = true;
      }
      else if (CPU.status.ppu.vcount >= 262)
         CPU.status.ppu.vcount = 0;
   }

   return CPU.status.cycles;
}

void cpu_run_frame(void)
{
   CPU.status.cycles = 0;
   CPU.status.ppu.vcount = 0;
   CPU.status.ppu.hcount = 0;
   unsigned cycles_per_frame = 10000;
   unsigned last_cycles = 0;

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
         fprintf(stderr, "PC: %04x, V = %3u, H = %4u\n", (unsigned)REGS.pc.w.l, CPU.status.ppu.vcount, CPU.status.ppu.hcount);
         uint8_t opcode = cpu_read_pc();
         fprintf(stderr, "Opcode: 0x%02x || %s\n", (unsigned)opcode, opcode_names[opcode]);
         op_table[opcode]();
         CPU.status.cycles += cycle_table[opcode];

         print_registers();
         fprintf(stderr, "======================================\n");
      }

      last_cycles = update_ppu_cycles(last_cycles);

      cpu_check_cycles();
      cpu_check_irq();
   }
}
