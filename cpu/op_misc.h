#ifndef __CPU_OP_MISC_H
#define __CPU_OP_MISC_H

static inline void cpu_op_nop(void)
{}

static inline void cpu_op_wdm(void)
{}

static inline void cpu_op_xba(void)
{
   word_reg_t a = REGS.a;

   a.b.l ^= a.b.h;
   a.b.h ^= REGS.a.b.l;
   a.b.l ^= REGS.a.b.h;
   REGS.p.n = (a.b.l & 0x80);
   REGS.p.z = (a.b.l == 0);
   REGS.a = a;
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

static inline void cpu_op_txs_e(void)
{
   REGS.sp.b.l = REGS.x.b.l;
}

static inline void cpu_op_txs_n(void)
{
   REGS.sp.w = REGS.x.w;
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
   cpu_stack_push(REGS.dp >> 16);
   cpu_stack_push((REGS.dp >> 8) & 0xFF);
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
   uint32_t low = 0, hi = 0;
   low = cpu_stack_pull();
   hi = cpu_stack_pull();
   uint32_t res = (low << 8) | (hi << 16);
   REGS.dp = res;

   REGS.p.n = res & 0x800000;
   REGS.p.z = (res == 0);
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

static inline void cpu_op_pea_e(void)
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
   REGS.wai = true;
}

static inline void cpu_op_stp(void)
{
   REGS.wai = true;
   REGS.stp = true;
}


#endif
