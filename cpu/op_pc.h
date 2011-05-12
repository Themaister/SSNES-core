#ifndef __OP_PC_H
#define __OP_PC_H

#include <stdint.h>
#include "system/util.h"
#include "memory.h"

#define CPU_OP_BRANCH_REG_DECL(reg) cpu_op_branch_##reg
#define CPU_OP_BRANCH_REG(reg) \
   static inline void CPU_OP_BRANCH_REG_DECL(reg) (void) \
   { \
      int8_t rel = cpu_read_pc(); \
      uint16_t addr = REGS.pc.w.l; \
      REGS.pc.w.l = isel_if(REGS.p.reg, addr + rel, addr); \
   }

#define CPU_OP_BRANCH_REG_N_DECL(reg) cpu_op_branch_n_##reg
#define CPU_OP_BRANCH_REG_N(reg) \
   static inline void CPU_OP_BRANCH_REG_N_DECL(reg) (void) \
   { \
      int8_t rel = cpu_read_pc(); \
      uint16_t addr = REGS.pc.w.l; \
      REGS.pc.w.l = isel_if(REGS.p.reg, addr, addr + rel); \
   }

CPU_OP_BRANCH_REG(n) // bmi
CPU_OP_BRANCH_REG(v) // bvs
CPU_OP_BRANCH_REG(z) // beq
CPU_OP_BRANCH_REG(c) // bcs
CPU_OP_BRANCH_REG_N(n) // bpl
CPU_OP_BRANCH_REG_N(v) // bvc
CPU_OP_BRANCH_REG_N(z) // bne
CPU_OP_BRANCH_REG_N(c) // bcc

static inline void cpu_op_bra(void) 
{
   int8_t rel = cpu_read_pc();
   REGS.pc.w.l += rel;
}

static inline void cpu_op_brl(void) 
{
   int16_t rel = cpu_readw_pc();
   REGS.pc.w.l += rel;
}

static inline void cpu_op_jmp_addr(void) 
{
   REGS.pc.w.l = cpu_readw_pc();
}

static inline void cpu_op_jmp_long(void) 
{
   REGS.pc.l = cpu_readl_pc() & 0xffffff;
}

static inline void cpu_op_jmp_iaddr(void) 
{
   uint16_t addr = cpu_readw_pc();

   word_reg_t jmp_addr;
   jmp_addr.b.l = cpu_readl(addr++);
   jmp_addr.b.h = cpu_readl(addr);
   REGS.pc.w.l = jmp_addr.w;
}

// This seems to read indirect data from program bank, while the others do not? :(
static inline void cpu_op_jmp_iaddrx(void) 
{
   uint16_t addr = cpu_readw_pc() + REGS.x.w;
   long_reg_t src;
   src.w.h = REGS.pc.w.h;
   src.w.l = addr;

   uint16_t jmp_addr = cpu_readlw(src.l);
   REGS.pc.w.l = jmp_addr;
}

static inline void cpu_op_jmp_iladdr(void) 
{
   uint16_t addr = cpu_readw_pc();
   long_reg_t dst;
   dst.b.ll = cpu_readl(addr++);
   dst.b.lh = cpu_readl(addr++);
   dst.w.h = cpu_readl(addr);
   REGS.pc = dst;
}

static inline void cpu_op_jsr_addr(void) 
{
   uint16_t addr = cpu_readw_pc();
   long_reg_t pc = REGS.pc;
   pc.w.l--;
   cpu_stack_push(pc.b.lh);
   cpu_stack_push(pc.b.ll);

   REGS.pc.w.l = addr;
}

static inline void cpu_op_jsr_long_n(void) 
{
   uint32_t addr = cpu_readl_pc();
   long_reg_t pc = REGS.pc;
   pc.w.l--;
   cpu_stack_push(pc.b.hl);
   cpu_stack_push(pc.b.lh);
   cpu_stack_push(pc.b.ll);
   REGS.pc.l = addr;
}

static inline void cpu_op_jsr_long_e(void) 
{
   cpu_op_jsr_long_n();
   REGS.sp.b.h = 0x01;
}

static inline void cpu_op_jsr_iaddrx_n(void) 
{
   uint16_t addr = cpu_readw_pc() + REGS.x.w;
   long_reg_t pc = REGS.pc;
   pc.w.l--;
   cpu_stack_push(pc.b.lh);
   cpu_stack_push(pc.b.ll);
   uint16_t jmp_addr = cpu_readl((pc.l & 0xff0000) | addr++);
   jmp_addr |= (uint16_t)cpu_readl((pc.l & 0xff0000) | addr) << 8;
   REGS.pc.w.l = jmp_addr;
}

static inline void cpu_op_jsr_iaddrx_e(void) 
{
   cpu_op_jsr_iaddrx_n();
   REGS.sp.b.h = 0x01;
}


static inline void cpu_op_rti_e(void) 
{
   cpu_set_p(cpu_stack_pull() | 0x30);
   REGS.pc.b.ll = cpu_stack_pull();
   REGS.pc.b.lh = cpu_stack_pull();
}

static inline void cpu_op_rti_n(void) 
{
   cpu_set_p(cpu_stack_pull());
   REGS.x.b.h = isel_if(REGS.p.x, 0x00, REGS.x.b.h);
   REGS.y.b.h = isel_if(REGS.p.x, 0x00, REGS.y.b.h);

   REGS.pc.b.ll = cpu_stack_pull();
   REGS.pc.b.lh = cpu_stack_pull();
   REGS.pc.b.hl = cpu_stack_pull();
}

static inline void cpu_op_rts(void) 
{
   REGS.pc.b.ll = cpu_stack_pull();
   REGS.pc.b.lh = cpu_stack_pull();
   REGS.pc.w.l++;
}

static inline void cpu_op_rtl_n(void) 
{
   REGS.pc.b.ll = cpu_stack_pull();
   REGS.pc.b.lh = cpu_stack_pull();
   REGS.pc.b.hl = cpu_stack_pull();
   REGS.pc.w.l++;
}

static inline void cpu_op_rtl_e(void) 
{
   cpu_op_rtl_n();
   REGS.sp.b.h = 0x01;
}

// Not really a CPU op, but here we jump to IRQ target.
#define CPU_OP_INTERRUPT_E_DECL(type) cpu_op_interrupt_##type##_e
#define CPU_OP_INTERRUPT_E(type, vector) \
   static inline void CPU_OP_INTERRUPT_E_DECL(type) (void) \
   { \
      cpu_stack_push(REGS.pc.b.lh); \
      cpu_stack_push(REGS.pc.b.ll); \
      cpu_stack_push(cpu_get_p()); \
      uint16_t irq_addr = cpu_readlw(vector); \
      REGS.p.i = true; \
      REGS.p.d = false; \
      REGS.pc.l = irq_addr; \
   }

#define CPU_OP_INTERRUPT_N_DECL(type) cpu_op_interrupt_##type##_n
#define CPU_OP_INTERRUPT_N(type, vector) \
   static inline void CPU_OP_INTERRUPT_N_DECL(type) (void) \
   { \
      cpu_stack_push(REGS.pc.b.hl); \
      cpu_stack_push(REGS.pc.b.lh); \
      cpu_stack_push(REGS.pc.b.ll); \
      cpu_stack_push(cpu_get_p()); \
      uint16_t irq_addr = cpu_readlw(vector); \
      REGS.p.i = true; \
      REGS.p.d = false; \
      REGS.pc.l = irq_addr; \
   }

CPU_OP_INTERRUPT_N(brk, 0xffe6)
CPU_OP_INTERRUPT_N(nmi, 0xffea)
CPU_OP_INTERRUPT_N(irq, 0xffee)

CPU_OP_INTERRUPT_E(cop, 0xfff4)
CPU_OP_INTERRUPT_E(nmi, 0xfffa)
CPU_OP_INTERRUPT_E(reset, 0xfffc) // Execution begins here
CPU_OP_INTERRUPT_E(brk, 0xfffe)
CPU_OP_INTERRUPT_E(irq, 0xfffe)

#endif
