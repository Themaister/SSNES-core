#ifndef __CPU_OP_MISC_H
#define __CPU_OP_MISC_H

#include <stdio.h>

static inline void cpu_op_rep(void)
{
   uint8_t mask = cpu_read_pc();
   uint8_t old_mask = cpu_get_p();
   cpu_set_p(old_mask & (~mask));

   iup_if(REGS.x.w, REGS.p.x, REGS.x.w & 0xff);
   iup_if(REGS.y.w, REGS.p.x, REGS.y.w & 0xff);
}

static inline void cpu_op_sep(void)
{
   uint8_t mask = cpu_read_pc();
   uint8_t old_mask = cpu_get_p();
   cpu_set_p(old_mask | mask);

   iup_if(REGS.x.w, REGS.p.x, REGS.x.w & 0xff);
   iup_if(REGS.y.w, REGS.p.x, REGS.y.w & 0xff);
}

static inline void cpu_op_nop(void)
{}

static inline void cpu_op_wdm(void)
{}

static inline void cpu_op_xba(void)
{
   word_reg_t a = REGS.a;
   REGS.a.b.h = a.b.l;
   uint8_t lo = a.b.h;

   REGS.p.n = (lo & 0x80);
   REGS.p.z = (lo == 0);
   REGS.a.b.l = lo;
}

static inline void cpu_op_xce(void)
{
   bool carry = REGS.p.c;
   REGS.p.c = REGS.e;
   REGS.e = carry;
   
   if (REGS.e)
   {
      REGS.p.m = true;
      REGS.p.x = true;
      REGS.sp.b.h = 0x01;
   }

   if (REGS.p.x)
   {
      REGS.x.b.h = 0x00;
      REGS.y.b.h = 0x00;
   }

   cpu_update_table();
}

static inline void cpu_op_tcs_e(void)
{
   REGS.sp.b.l = REGS.a.b.l;
}

static inline void cpu_op_tcs_n(void)
{
   REGS.sp.w = REGS.a.w;
}

static inline void cpu_op_tsc(void)
{
   uint16_t trans = REGS.sp.w;
   REGS.a.w = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x8000);
}

static inline void cpu_op_tsx_b(void)
{
   REGS.x.b.l = REGS.sp.b.l;
   REGS.p.n = (REGS.x.b.l & 0x80);
   REGS.p.z = (REGS.x.b.l == 0);
}

static inline void cpu_op_tsx_w(void)
{
   REGS.x.w = REGS.sp.w;
   REGS.p.n = (REGS.x.w & 0x8000);
   REGS.p.z = (REGS.x.w == 0);
}

static inline void cpu_op_tcd(void)
{
   uint32_t trans = REGS.a.w;
   REGS.dp = trans;
   REGS.p.n = trans & 0x8000;
   REGS.p.z = (trans == 0);
}

static inline void cpu_op_tdc(void)
{
   uint16_t trans = REGS.dp;
   REGS.a.w = trans;
   REGS.p.n = trans & 0x8000;
   REGS.p.z = (trans == 0);
}

static inline void cpu_op_txs_e(void)
{
   REGS.sp.b.l = REGS.x.b.l;
}

static inline void cpu_op_txs_n(void)
{
   REGS.sp.w = REGS.x.w;
}

static inline void cpu_op_txa_mx(void)
{
   uint16_t trans = REGS.x.w;
   REGS.a.w = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x8000);
}

static inline void cpu_op_txa_MX(void)
{
   uint8_t trans = REGS.x.b.l;
   REGS.a.b.l = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x8000);
}

static inline void cpu_op_txa_Mx(void)
{
   uint8_t trans = REGS.x.b.l;
   REGS.a.b.l = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x80);
}

static inline void cpu_op_txa_mX(void)
{
   uint8_t trans = REGS.x.b.l;
   REGS.a.w = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x80);
}

static inline void cpu_op_tya_mx(void)
{
   uint16_t trans = REGS.y.w;
   REGS.a.w = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x8000);
}

static inline void cpu_op_tya_MX(void)
{
   uint8_t trans = REGS.y.b.l;
   REGS.a.b.l = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x80);
}

static inline void cpu_op_tya_mX(void)
{
   uint8_t trans = REGS.y.b.l;
   REGS.a.w = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x80);
}

static inline void cpu_op_tya_Mx(void)
{
   uint8_t trans = REGS.y.b.l;
   REGS.a.b.l = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x80);
}

static inline void cpu_op_tay_mx(void)
{
   uint16_t trans = REGS.a.w;
   REGS.y.w = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x8000);
}

static inline void cpu_op_tay_MX(void)
{
   uint8_t trans = REGS.a.b.l;
   REGS.y.b.l = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x80);
}

static inline void cpu_op_tay_Mx(void)
{
   uint16_t trans = REGS.a.w;
   REGS.y.w = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x8000);
}

static inline void cpu_op_tay_mX(void)
{
   uint8_t trans = REGS.a.b.l;
   REGS.y.b.l = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x80);
}

static inline void cpu_op_tax_mx(void)
{
   uint16_t trans = REGS.a.w;
   REGS.x.w = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x8000);
}

static inline void cpu_op_tax_MX(void)
{
   uint8_t trans = REGS.a.b.l;
   REGS.x.b.l = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x80);
}

static inline void cpu_op_tax_Mx(void)
{
   uint16_t trans = REGS.a.w;
   REGS.x.w = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x8000);
}

static inline void cpu_op_tax_mX(void)
{
   uint8_t trans = REGS.a.b.l;
   REGS.x.b.l = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x80);
}

static inline void cpu_op_tyx_b(void)
{
   uint8_t trans = REGS.y.b.l;
   REGS.x.b.l = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x80);
}

static inline void cpu_op_tyx_w(void)
{
   uint16_t trans = REGS.y.w;
   REGS.x.w = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x8000);
}

static inline void cpu_op_txy_b(void)
{
   uint8_t trans = REGS.x.b.l;
   REGS.y.b.l = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x80);
}

static inline void cpu_op_txy_w(void)
{
   uint16_t trans = REGS.x.w;
   REGS.y.w = trans;
   REGS.p.z = (trans == 0);
   REGS.p.n = (trans & 0x8000);
}

static inline void cpu_op_pha_b(void)
{
   cpu_stack_push(REGS.a.b.l);
}

static inline void cpu_op_pha_w(void)
{
   cpu_stack_push(REGS.a.b.h);
   cpu_stack_push(REGS.a.b.l);
}

static inline void cpu_op_phx_b(void)
{
   cpu_stack_push(REGS.a.b.l);
}

static inline void cpu_op_phx_w(void)
{
   cpu_stack_push(REGS.x.b.h);
   cpu_stack_push(REGS.x.b.l);
}

static inline void cpu_op_phy_b(void)
{
   cpu_stack_push(REGS.y.b.l);
}

static inline void cpu_op_phy_w(void)
{
   cpu_stack_push(REGS.y.b.h);
   cpu_stack_push(REGS.y.b.l);
}

static inline void cpu_op_phb(void)
{
   cpu_stack_push(REGS.db >> 16);
}

static inline void cpu_op_phd(void)
{
   cpu_stack_push(REGS.dp >> 8);
   cpu_stack_push(REGS.dp & 0xFF);
}

static inline void cpu_op_phk(void)
{
   cpu_stack_push(REGS.pc.b.hl);
}

static inline void cpu_op_php(void)
{
   cpu_stack_push(cpu_get_p());
}

static inline void cpu_op_pla_b(void)
{
   uint8_t val = cpu_stack_pull();
   REGS.a.b.l = val;
   REGS.p.z = (val == 0);
   REGS.p.n = val & 0x80;
}

static inline void cpu_op_pla_w(void)
{
   word_reg_t val;
   val.b.l = cpu_stack_pull();
   val.b.h = cpu_stack_pull();

   REGS.a = val;
   REGS.p.z = (val.w == 0);
   REGS.p.n = val.w & 0x8000;
}

static inline void cpu_op_plx_b(void)
{
   uint8_t val = cpu_stack_pull();
   REGS.x.b.l = val;
   REGS.p.z = (val == 0);
   REGS.p.n = val & 0x80;
}

static inline void cpu_op_plx_w(void)
{
   word_reg_t val;
   val.b.l = cpu_stack_pull();
   val.b.h = cpu_stack_pull();

   REGS.x = val;
   REGS.p.z = (val.w == 0);
   REGS.p.n = val.w & 0x8000;
}

static inline void cpu_op_ply_b(void)
{
   uint8_t val = cpu_stack_pull();
   REGS.y.b.l = val;
   REGS.p.z = (val == 0);
   REGS.p.n = val & 0x80;
}

static inline void cpu_op_ply_w(void)
{
   word_reg_t val;
   val.b.l = cpu_stack_pull();
   val.b.h = cpu_stack_pull();

   REGS.y = val;
   REGS.p.z = (val.w == 0);
   REGS.p.n = val.w & 0x8000;
}

static inline void cpu_op_plb(void)
{
   uint32_t val = cpu_stack_pull();
   REGS.db = val << 16;

   REGS.p.n = val & 0x80;
   REGS.p.z = (val == 0);
}

static inline void cpu_op_pld(void)
{
   word_reg_t w;
   w.b.l = cpu_stack_pull();
   w.b.h = cpu_stack_pull();
   REGS.dp = w.w;

   REGS.p.n = w.w & 0x8000;
   REGS.p.z = (w.w == 0);
}

static inline void cpu_op_plp_e(void)
{
   cpu_set_p(cpu_stack_pull() | 0x30);
   REGS.x.b.h = isel_if(REGS.p.x, 0x00, REGS.x.b.h);
   REGS.y.b.h = isel_if(REGS.p.x, 0x00, REGS.y.b.h);
}

static inline void cpu_op_plp_n(void)
{
   cpu_set_p(cpu_stack_pull());
   REGS.x.b.h = isel_if(REGS.p.x, 0x00, REGS.x.b.h);
   REGS.y.b.h = isel_if(REGS.p.x, 0x00, REGS.y.b.h);
}

static inline void cpu_op_pea(void)
{
   word_reg_t addr; 
   addr.w = cpu_readw_pc();
   cpu_stack_push(addr.b.h);
   cpu_stack_push(addr.b.l);
}

static inline void cpu_op_pei_n(void)
{
   uint8_t dp = cpu_read_pc();
   word_reg_t addr;
   addr.w = cpu_readw_dp(dp);
   cpu_stack_push(addr.b.h);
   cpu_stack_push(addr.b.l);
}

static inline void cpu_op_pei_e(void)
{
   cpu_op_pei_n();
   REGS.sp.b.h = 0x01;
}

static inline void cpu_op_per_n(void)
{
   uint16_t val = cpu_readw_pc();
   long_reg_t rel = REGS.pc;
   rel.w.l += val;
   cpu_stack_push(rel.b.lh);
   cpu_stack_push(rel.b.ll);
}

static inline void cpu_op_per_e(void)
{
   cpu_op_per_n();
   REGS.sp.b.h = 0x01;
}

static inline void cpu_op_wai(void)
{
   if (!REGS.wai_quit)
   {
      REGS.wai = true;
      REGS.pc.l--;
   }
   else
   {
      REGS.wai = false;
      REGS.wai_quit = false;
   }
}

static inline void cpu_op_stp(void)
{
   REGS.pc.l--;
}

static inline void cpu_op_mvn(void)
{
   uint32_t db = ((uint32_t)cpu_read_pc()) << 16;
   uint32_t sb = ((uint32_t)cpu_read_pc()) << 16;

   cpu_writel(db | REGS.y.w++, cpu_readl(sb | REGS.x.w++));
   REGS.pc.w.l = isel_if(REGS.a.w--, REGS.pc.w.l - 3, REGS.pc.w.l);
}

static inline void cpu_op_mvp(void)
{
   uint32_t db = ((uint32_t)cpu_read_pc()) << 16;
   uint32_t sb = ((uint32_t)cpu_read_pc()) << 16;

   cpu_writel(db | REGS.y.w--, cpu_readl(sb | REGS.x.w--));
   REGS.pc.w.l = isel_if(REGS.a.w--, REGS.pc.w.l - 3, REGS.pc.w.l);
}

#define CPU_OP_RESET_FLAG_DECL(flag) cpu_op_reset_flag_##flag
#define CPU_OP_RESET_FLAG(flag) \
   static inline void CPU_OP_RESET_FLAG_DECL(flag) (void) \
   { \
      REGS.p.flag = false; \
   }

#define CPU_OP_SET_FLAG_DECL(flag) cpu_op_set_flag_##flag
#define CPU_OP_SET_FLAG(flag) \
   static inline void CPU_OP_SET_FLAG_DECL(flag) (void) \
   { \
      REGS.p.flag = true; \
   }

CPU_OP_SET_FLAG(c)
CPU_OP_RESET_FLAG(c)
CPU_OP_SET_FLAG(i)
CPU_OP_RESET_FLAG(i)
CPU_OP_RESET_FLAG(v)
CPU_OP_SET_FLAG(d)
CPU_OP_RESET_FLAG(d)


#endif
