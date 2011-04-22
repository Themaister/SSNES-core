#ifndef __OP_WRITE_H
#define __OP_WRITE_H

// Write modes.
// For STA, STX and STY and all the funky addressing modes ... ;)

// Absolute address.
#define CPU_OP_STORE_ADDR_B_DECL(reg) cpu_op_store_addr_b_##reg
#define CPU_OP_STORE_ADDR_B(reg) \
   static inline void CPU_OP_STORE_ADDR_B_DECL(reg) (void) \
   { \
      cpu_write(cpu_readw_pc(), REGS.reg.b.l); \
   }


#define CPU_OP_STORE_ADDR_W_DECL(reg) cpu_op_store_addr_w_##reg
#define CPU_OP_STORE_ADDR_W(reg) \
   static inline void CPU_OP_STORE_ADDR_W_DECL(reg) (void) \
   { \
      cpu_writew(cpu_readw_pc(), REGS.reg.w); \
   }

CPU_OP_STORE_ADDR_B(a); // sta $1337
CPU_OP_STORE_ADDR_W(a);
CPU_OP_STORE_ADDR_B(x); // stx $1337
CPU_OP_STORE_ADDR_W(x);
CPU_OP_STORE_ADDR_B(y); // sty $1337
CPU_OP_STORE_ADDR_W(y);
CPU_OP_STORE_ADDR_B(zero); // stz $1337
CPU_OP_STORE_ADDR_W(zero);

// Absolute address, register indexed.
#define CPU_OP_STORE_ADDR_REGI_B_DECL(reg, regindex) cpu_op_store_addr_regi_b_##reg##_##regindex
#define CPU_OP_STORE_ADDR_REGI_B(reg, regindex) \
   static inline void CPU_OP_STORE_ADDR_REGI_B_DECL(reg, regindex) (void) \
   { \
      cpu_write(cpu_readw_pc() + REGS.regindex.w, REGS.reg.b.l); \
   }

#define CPU_OP_STORE_ADDR_REGI_W_DECL(reg, regindex) cpu_op_store_addr_regi_w_##reg##_##regindex
#define CPU_OP_STORE_ADDR_REGI_W(reg, regindex) \
   static inline void CPU_OP_STORE_ADDR_REGI_W_DECL(reg, regindex) (void) \
   { \
      cpu_writew(cpu_readw_pc() + REGS.regindex.w, REGS.reg.w); \
   }

CPU_OP_STORE_ADDR_REGI_B(a, x); // sta $1337, x
CPU_OP_STORE_ADDR_REGI_W(a, x);
CPU_OP_STORE_ADDR_REGI_B(a, y); // sta $1337, y
CPU_OP_STORE_ADDR_REGI_W(a, y);
CPU_OP_STORE_ADDR_REGI_B(zero, x); // stz $1337, y
CPU_OP_STORE_ADDR_REGI_W(zero, x);


// Long address, register indexed.
#define CPU_OP_STORE_LONG_REGI_B_DECL(reg, regindex) cpu_op_store_long_regi_b_##reg##_##regindex
#define CPU_OP_STORE_LONG_REGI_B(reg, regindex) \
   static inline void CPU_OP_STORE_LONG_REGI_B_DECL(reg, regindex) (void) \
   { \
      cpu_writel(cpu_readl_pc() + REGS.regindex.w, REGS.reg.b.l); \
   }

#define CPU_OP_STORE_LONG_REGI_W_DECL(reg, regindex) cpu_op_store_long_regi_w_##reg##_##regindex
#define CPU_OP_STORE_LONG_REGI_W(reg, regindex) \
   static inline void CPU_OP_STORE_LONG_REGI_W_DECL(reg, regindex) (void) \
   { \
      cpu_writelw(cpu_readl_pc() + REGS.regindex.w, REGS.reg.w); \
   }

CPU_OP_STORE_LONG_REGI_B(a, y); // sta $7e1337, y
CPU_OP_STORE_LONG_REGI_W(a, y);

// Direct page
#define CPU_OP_STORE_DP_B_DECL(reg) cpu_op_store_dp_b_##reg
#define CPU_OP_STORE_DP_B(reg) \
   static inline void CPU_OP_STORE_DP_B_DECL(reg) (void) \
   { \
      cpu_write_dp(cpu_read_pc(), REGS.reg.b.l); \
   }

#define CPU_OP_STORE_DP_W_DECL(reg) cpu_op_store_dp_w_##reg
#define CPU_OP_STORE_DP_W(reg) \
   static inline void CPU_OP_STORE_DP_W_DECL(reg) (void) \
   { \
      cpu_writew_dp(cpu_read_pc(), REGS.reg.w); \
   }

CPU_OP_STORE_DP_B(a); // sta $dp
CPU_OP_STORE_DP_W(a);
CPU_OP_STORE_DP_B(x); // stx $dp
CPU_OP_STORE_DP_W(x);
CPU_OP_STORE_DP_B(y); // sty $dp
CPU_OP_STORE_DP_W(y);
CPU_OP_STORE_DP_B(zero); // stz $dp
CPU_OP_STORE_DP_W(zero);

// Direct page, register index.
#define CPU_OP_STORE_DPR_B_DECL(reg, regindex) cpu_op_store_dpr_b_##reg##_##regindex
#define CPU_OP_STORE_DPR_B(reg, regindex) \
   static inline void CPU_OP_STORE_DPR_B_DECL(reg, regindex) (void) \
   { \
      cpu_write_dp(cpu_read_pc() + REGS.regindex.w, REGS.reg.b.l); \
   }

#define CPU_OP_STORE_DPR_W_DECL(reg, regindex) cpu_op_store_dpr_w_##reg##_##regindex
#define CPU_OP_STORE_DPR_W(reg, regindex) \
   static inline void CPU_OP_STORE_DPR_W_DECL(reg, regindex) (void) \
   { \
      cpu_write_dp(cpu_read_pc() + REGS.regindex.w, REGS.reg.b.l); \
   }

CPU_OP_STORE_DPR_B(a, x); // sta $dp, x
CPU_OP_STORE_DPR_W(a, x);
CPU_OP_STORE_DPR_B(x, y); // stx $dp, y
CPU_OP_STORE_DPR_W(x, y);
CPU_OP_STORE_DPR_B(y, x); // sty $dp, x
CPU_OP_STORE_DPR_W(y, x);
CPU_OP_STORE_DPR_B(zero, x); // stz $dp, x
CPU_OP_STORE_DPR_W(zero, x);


// Long address.
static inline void cpu_op_sta_long_b(void)
{
   uint32_t addr = cpu_readl_pc();
   cpu_writel(addr, REGS.a.b.l);
}

static inline void cpu_op_sta_long_w(void)
{
   uint32_t addr = cpu_readl_pc();
   cpu_writelw(addr, REGS.a.w);
}

// Long address, X indexed.
static inline void cpu_op_sta_longx_b(void)
{
   uint32_t addr = cpu_readl_pc() + REGS.x.w;
   cpu_writel(addr, REGS.a.b.l);
}

static inline void cpu_op_sta_longx_w(void)
{
   uint32_t addr = cpu_readl_pc() + REGS.x.w;
   cpu_writelw(addr, REGS.a.w);
}

// Indirect direct page.
static inline void cpu_op_sta_idp_b(void)
{
   uint8_t dp = cpu_read_pc();
   uint16_t addr = cpu_readw_dp(dp);
   cpu_write(addr, REGS.a.b.l);
}

static inline void cpu_op_sta_idp_w(void)
{
   uint8_t dp = cpu_read_pc();
   uint32_t addr = cpu_readw_dp(dp);
   cpu_writew(addr, REGS.a.w);
}

// Indirect long direct page.
static inline void cpu_op_sta_ildp_b(void)
{
   uint8_t dp = cpu_read_pc();
   uint16_t addr = cpu_readl_dp(dp);
   cpu_writel(addr, REGS.a.b.l);
}

static inline void cpu_op_sta_ildp_w(void)
{
   uint8_t dp = cpu_read_pc();
   uint32_t addr = cpu_readl_dp(dp);
   cpu_writelw(addr, REGS.a.w);
}

// Register indexed indirect direct page.
// sta (dp, x)
static inline void cpu_op_sta_idpx_b(void)
{
   uint8_t dp = cpu_read_pc();
   uint16_t addr = cpu_readw_dp(dp + REGS.x.w);
   cpu_write(addr, REGS.a.b.l);
}

static inline void cpu_op_sta_idpx_w(void)
{
   uint8_t dp = cpu_read_pc();
   uint16_t addr = cpu_readw_dp(dp + REGS.x.w);
   cpu_writew(addr, REGS.a.w);
}

// sta (dp), y
static inline void cpu_op_sta_idpy_b(void)
{
   uint8_t dp = cpu_read_pc();
   uint16_t addr = cpu_readw_dp(dp) + REGS.y.w;
   cpu_write(addr, REGS.a.b.l);
}

static inline void cpu_op_sta_idpy_w(void)
{
   uint8_t dp = cpu_read_pc();
   uint16_t addr = cpu_readw_dp(dp) + REGS.y.w;
   cpu_writew(addr, REGS.a.w);
}

// Indirect long direct page, Y indexed
// sta [dp], y
static inline void cpu_op_sta_ildpy_b(void)
{
   uint8_t dp = cpu_read_pc();
   uint32_t addr = cpu_readl_dp(dp) + REGS.y.w;
   cpu_writel(addr, REGS.a.b.l);
}

static inline void cpu_op_sta_ildpy_w(void)
{
   uint8_t dp = cpu_read_pc();
   uint32_t addr = cpu_readl_dp(dp) + REGS.y.w;
   cpu_writelw(addr, REGS.a.w);
}

// Stack relative
// sta 13, s
static inline void cpu_op_sta_sr_b(void)
{
   uint8_t sr = cpu_read_pc();
   cpu_write_sp(sr, REGS.a.b.l);
}

static inline void cpu_op_sta_sr_w(void)
{
   uint8_t sr = cpu_read_pc();
   cpu_writew_sp(sr, REGS.a.w);
}

// Indirect stack relative, Y indexed
// sta (13, s), y
static inline void cpu_op_sta_isry_b(void)
{
   uint8_t sr = cpu_read_pc();
   uint16_t addr = cpu_readw_sp(sr) + REGS.y.w;
   cpu_write(addr, REGS.a.b.l);
}

static inline void cpu_op_sta_isry_w(void)
{
   uint8_t sr = cpu_read_pc();
   uint16_t addr = cpu_readw_sp(sr) + REGS.y.w;
   cpu_writew(addr, REGS.a.w);
}

#endif
