#ifndef __OP_PC_H
#define __OP_PC_H

#include <stdint.h>

#if 0
template<int bit, int val> 
static inline void cpu_op_branch(void) {
   if((bool)(regs.p & bit) != val) {
      L   REGS.rd.l = cpu_op_readpc();
   } else {
      REGS.rd.l = cpu_op_readpc();
      REGS.aa.w = regs.pc.d + (int8)REGS.rd.l;
      cpu_op_io_cond6(REGS.aa.w);
      L   cpu_op_io();
      regs.pc.w = REGS.aa.w;
   }
}
#endif

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
   REGS.pc.l = cpu_readl_pc();
}

static inline void cpu_op_jmp_iaddr(void) 
{
   uint16_t addr = cpu_readw_pc();

   uint16_t jmp_addr = cpu_readw(addr);
   REGS.pc.w.l = jmp_addr;
}

static inline void cpu_op_jmp_iaddrx(void) 
{
   uint16_t addr = cpu_readw_pc();
   uint16_t jmp_addr = cpu_readl(((uint32_t)REGS.pc.b.hl << 16) | addr) + REGS.x.w;
   REGS.pc.w.l = jmp_addr;
}

static inline void cpu_op_jmp_iladdr(void) 
{
   uint16_t addr = cpu_readw_pc();
   uint32_t jmp_addr = cpu_readl(((uint32_t)REGS.pc.b.hl << 16) | addr) + REGS.x.w;
   REGS.pc.l = jmp_addr;
}

static inline void cpu_op_jsr_addr(void) 
{
   uint16_t addr = cpu_readw_pc();
   cpu_push_stack(REGS.pc.b.ll);
   cpu_push_stack(REGS.pc.b.lh);

   REGS.pc.w.l = addr;
}

static inline void cpu_op_jsr_long_n(void) 
{
   uint32_t addr = cpu_readl_pc();
   cpu_push_stack(REGS.pc.b.hl);
   cpu_push_stack(REGS.pc.b.lh);
   cpu_push_stack(REGS.pc.b.ll);
   REGS.pc.l = addr;
}

static inline void cpu_op_jsr_long_e(void) 
{
   cpu_op_jsr_long_n();
   REGS.sp.h = 0x01;
}

static inline void cpu_op_jsr_iaddrx_n(void) 
{
   uint16_t addr = cpu_readw_pc();
   cpu_push_stack(REGS.pc.b.ll);
   cpu_push_stack(REGS.pc.b.lh);
   uint16_t jmp_addr = cpu_readl((((uint32_t)REGS.pc.b.hl) << 16) | addr);
   REGS.pc.w.l = jmp_addr;
}

static inline void cpu_op_jsr_iaddrx_e(void) 
{
   cpu_op_jsr_iaddrx_n();
   REGS.sp.h = 0x01;
}


static inline void cpu_op_rti_e(void) 
{
   cpu_set_p(cpu_pull_stack() | 0x30);
   REGS.pc.b.ll = cpu_pull_stack();
   REGS.pc.b.lh = cpu_pull_stack();
}

static inline void cpu_op_rti_n(void) 
{
   cpu_set_p(cpu_pull_stack());
   REGS.x.b.h = isel_if(REGS.p.x, 0x00, REGS.x.b.h);
   REGS.y.b.h = isel_if(REGS.p.x, 0x00, REGS.y.b.h);

   REGS.pc.b.ll = cpu_pull_stack();
   REGS.pc.b.lh = cpu_pull_stack();
   REGS.pc.b.hl = cpu_pull_stack();
   update_table();
}

static inline void cpu_op_rts(void) 
{
   REGS.pc.b.ll = cpu_pull_stack();
   REGS.pc.b.lh = cpu_pull_stack();
}

static inline void cpu_op_rtl_n(void) 
{
   REGS.pc.b.ll = cpu_pull_stack();
   REGS.pc.b.lh = cpu_pull_stack();
   REGS.pc.b.hl = cpu_pull_stack();
}

static inline void cpu_op_rtl_e(void) 
{
   cpu_op_rtl_n();
   REGS.sp.b.h = 0x01;
}


#endif
