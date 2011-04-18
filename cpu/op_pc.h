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
   REGS.rd.l = cpu_op_readpc();
   REGS.aa.w = regs.pc.d + (int8)REGS.rd.l;
   cpu_op_io_cond6(REGS.aa.w);
L  cpu_op_io();
   regs.pc.w = REGS.aa.w;
}

static inline void cpu_op_brl(void) 
{
   REGS.rd.l = cpu_op_readpc();
   REGS.rd.h = cpu_op_readpc();
L  cpu_op_io();
   regs.pc.w = regs.pc.d + (int16_t)REGS.rd.w;
}

static inline void cpu_op_jmp_addr(void) 
{
   REGS.rd.l = cpu_op_readpc();
L  REGS.rd.h = cpu_op_readpc();
   regs.pc.w = REGS.rd.w;
}

static inline void cpu_op_jmp_long(void) 
{
   REGS.rd.l = cpu_op_readpc();
   REGS.rd.h = cpu_op_readpc();
L  REGS.rd.b = cpu_op_readpc();
   regs.pc.d = REGS.rd.d & 0xffffff;
}

static inline void cpu_op_jmp_iaddr(void) 
{
   REGS.aa.l = cpu_op_readpc();
   REGS.aa.h = cpu_op_readpc();
   REGS.rd.l = cpu_op_readaddr(REGS.aa.w + 0);
L  REGS.rd.h = cpu_op_readaddr(REGS.aa.w + 1);
   regs.pc.w = REGS.rd.w;
}

static inline void cpu_op_jmp_iaddrx(void) 
{
   REGS.aa.l = cpu_op_readpc();
   REGS.aa.h = cpu_op_readpc();
   cpu_op_io();
   REGS.rd.l = cpu_op_readpbr(REGS.aa.w + regs.x.w + 0);
L  REGS.rd.h = cpu_op_readpbr(REGS.aa.w + regs.x.w + 1);
   regs.pc.w = REGS.rd.w;
}

static inline void cpu_op_jmp_iladdr(void) 
{
   REGS.aa.l = cpu_op_readpc();
   REGS.aa.h = cpu_op_readpc();
   REGS.rd.l = cpu_op_readaddr(REGS.aa.w + 0);
   REGS.rd.h = cpu_op_readaddr(REGS.aa.w + 1);
L  REGS.rd.b = cpu_op_readaddr(REGS.aa.w + 2);
   regs.pc.d = REGS.rd.d & 0xffffff;
}

static inline void cpu_op_jsr_addr(void) 
{
   REGS.aa.l = cpu_op_readpc();
   REGS.aa.h = cpu_op_readpc();
   cpu_op_io();
   regs.pc.w--;
   op_writestack(regs.pc.h);
L  op_writestack(regs.pc.l);
   regs.pc.w = REGS.aa.w;
}

static inline void cpu_op_jsr_long_e(void) 
{
   REGS.aa.l = cpu_op_readpc();
   REGS.aa.h = cpu_op_readpc();
   op_writestackn(regs.pc.b);
   cpu_op_io();
   REGS.aa.b = cpu_op_readpc();
   regs.pc.w--;
   op_writestackn(regs.pc.h);
L  op_writestackn(regs.pc.l);
   regs.pc.d = REGS.aa.d & 0xffffff;
   regs.s.h = 0x01;
}

static inline void cpu_op_jsr_long_n(void) 
{
   REGS.aa.l = cpu_op_readpc();
   REGS.aa.h = cpu_op_readpc();
   op_writestackn(regs.pc.b);
   cpu_op_io();
   REGS.aa.b = cpu_op_readpc();
   regs.pc.w--;
   op_writestackn(regs.pc.h);
L  op_writestackn(regs.pc.l);
   regs.pc.d = REGS.aa.d & 0xffffff;
}

static inline void cpu_op_jsr_iaddrx_e(void) 
{
   REGS.aa.l = cpu_op_readpc();
   op_writestackn(regs.pc.h);
   op_writestackn(regs.pc.l);
   REGS.aa.h = cpu_op_readpc();
   cpu_op_io();
   REGS.rd.l = cpu_op_readpbr(REGS.aa.w + regs.x.w + 0);
L  REGS.rd.h = cpu_op_readpbr(REGS.aa.w + regs.x.w + 1);
   regs.pc.w = REGS.rd.w;
   regs.s.h = 0x01;
}

static inline void cpu_op_jsr_iaddrx_n(void) 
{
   REGS.aa.l = cpu_op_readpc();
   op_writestackn(regs.pc.h);
   op_writestackn(regs.pc.l);
   REGS.aa.h = cpu_op_readpc();
   cpu_op_io();
   REGS.rd.l = cpu_op_readpbr(REGS.aa.w + regs.x.w + 0);
L  REGS.rd.h = cpu_op_readpbr(REGS.aa.w + regs.x.w + 1);
   regs.pc.w = REGS.rd.w;
}

static inline void cpu_op_rti_e(void) 
{
   cpu_op_io();
   cpu_op_io();
   regs.p = cpu_op_readstack(void) | 0x30;
   REGS.rd.l = cpu_op_readstack();
L  REGS.rd.h = cpu_op_readstack();
   regs.pc.w = REGS.rd.w;
}

static inline void cpu_op_rti_n(void) 
{
   cpu_op_io();
   cpu_op_io();
   regs.p = cpu_op_readstack();
   if (regs.p.x) 
   {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
   }
   REGS.rd.l = cpu_op_readstack();
   REGS.rd.h = cpu_op_readstack();
L  REGS.rd.b = cpu_op_readstack();
   regs.pc.d = REGS.rd.d & 0xffffff;
   update_table();
}

static inline void cpu_op_rts(void) 
{
   cpu_op_io();
   cpu_op_io();
   REGS.rd.l = cpu_op_readstack();
   REGS.rd.h = cpu_op_readstack();
L  cpu_op_io();
   regs.pc.w = ++REGS.rd.w;
}

static inline void cpu_op_rtl_e(void) 
{
   cpu_op_io();
   cpu_op_io();
   REGS.rd.l = cpu_op_readstackn();
   REGS.rd.h = cpu_op_readstackn();
L  REGS.rd.b = cpu_op_readstackn();
   regs.pc.b = REGS.rd.b;
   regs.pc.w = ++REGS.rd.w;
   regs.s.h = 0x01;
}

static inline void cpu_op_rtl_n(void) 
{
   cpu_op_io();
   cpu_op_io();
   REGS.rd.l = cpu_op_readstackn();
   REGS.rd.h = cpu_op_readstackn();
L  REGS.rd.b = cpu_op_readstackn();
   regs.pc.b = REGS.rd.b;
   regs.pc.w = ++REGS.rd.w;
}

#endif
