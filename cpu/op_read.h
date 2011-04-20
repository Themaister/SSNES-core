#ifndef __OP_READ_H
#define __OP_READ_H

// C templates ;)
// Here we have all the batshit crazy addressing modes of the 65c816 chip.
// All reading modes have byte or word variants depending on register flags (8/16-bit A, X, Y ...)

// Constant, e.g. lda #$ff
#define CPU_OP_READ_CONST_B_DECL(op) cpu_op_read_const_b_##op
#define CPU_OP_READ_CONST_B(op) \
   static inline void CPU_OP_READ_CONST_B_DECL(op) (void) \
   { \
      cpu_op_##op##_b (cpu_read_pc()); \
   }

#define CPU_OP_READ_CONST_W_DECL(op) cpu_op_read_const_w_##op
#define CPU_OP_READ_CONST_W(op) \
   static inline void CPU_OP_READ_CONST_W_DECL(op) (void) \
   { \
      cpu_op_##op##_w (cpu_readw_pc()); \
   }

// Group 1
CPU_OP_READ_CONST_B(adc);
CPU_OP_READ_CONST_W(adc);
CPU_OP_READ_CONST_B(and);
CPU_OP_READ_CONST_W(and);
CPU_OP_READ_CONST_B(cmp);
CPU_OP_READ_CONST_W(cmp);
CPU_OP_READ_CONST_B(eor);
CPU_OP_READ_CONST_W(eor);
CPU_OP_READ_CONST_B(lda);
CPU_OP_READ_CONST_W(lda);
CPU_OP_READ_CONST_B(ora);
CPU_OP_READ_CONST_W(ora);
CPU_OP_READ_CONST_B(sbc);
CPU_OP_READ_CONST_W(sbc);

// Not sure what this is. Testing bits for comparison?
static inline void cpu_op_read_bit_const_b(void) 
{
   REGS.p.z = ((cpu_read_pc() & REGS.a.b.l) == 0);
}

static inline void cpu_op_read_bit_const_w(void) 
{
   REGS.p.z = ((cpu_readw_pc() & REGS.a.w) == 0);
}


// Absolute addressing, e.g. lda $1337
#define CPU_OP_READ_ADDR_B_DECL(op) cpu_op_read_addr_b_##op
#define CPU_OP_READ_ADDR_B(op) \
   static inline void CPU_OP_READ_ADDR_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc(); \
      cpu_op_##op##_b (cpu_read(addr)); \
   }

#define CPU_OP_READ_ADDR_W_DECL(op) cpu_op_read_addr_w_##op
#define CPU_OP_READ_ADDR_W(op) \
   static inline void CPU_OP_READ_ADDR_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc(); \
      cpu_op_##op##_w (cpu_readw(addr)); \
   }

CPU_OP_READ_ADDR_B(adc);
CPU_OP_READ_ADDR_W(adc);
CPU_OP_READ_ADDR_B(and);
CPU_OP_READ_ADDR_W(and);
CPU_OP_READ_ADDR_B(cmp);
CPU_OP_READ_ADDR_W(cmp);
CPU_OP_READ_ADDR_B(eor);
CPU_OP_READ_ADDR_W(eor);
CPU_OP_READ_ADDR_B(lda);
CPU_OP_READ_ADDR_W(lda);
CPU_OP_READ_ADDR_B(ora);
CPU_OP_READ_ADDR_W(ora);
CPU_OP_READ_ADDR_B(sbc);
CPU_OP_READ_ADDR_W(sbc);


// Absolute address, X indexed, e.g. lda $1337, x
#define CPU_OP_READ_ADDRX_B_DECL(op) cpu_op_read_addrx_b_##op
#define CPU_OP_READ_ADDRX_B(op) \
   static inline void CPU_OP_READ_ADDRX_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc() + REGS.x.w; \
      cpu_op_##op##_b (cpu_read(addr)); \
   }

#define CPU_OP_READ_ADDRX_W_DECL(op) cpu_op_read_addrx_w_##op
#define CPU_OP_READ_ADDRX_W(op) \
   static inline void CPU_OP_READ_ADDRX_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc() + REGS.x.w; \
      cpu_op_##op##_w (cpu_readw(addr)); \
   }

CPU_OP_READ_ADDRX_B(adc);
CPU_OP_READ_ADDRX_W(adc);
CPU_OP_READ_ADDRX_B(and);
CPU_OP_READ_ADDRX_W(and);
CPU_OP_READ_ADDRX_B(cmp);
CPU_OP_READ_ADDRX_W(cmp);
CPU_OP_READ_ADDRX_B(eor);
CPU_OP_READ_ADDRX_W(eor);
CPU_OP_READ_ADDRX_B(lda);
CPU_OP_READ_ADDRX_W(lda);
CPU_OP_READ_ADDRX_B(ora);
CPU_OP_READ_ADDRX_W(ora);
CPU_OP_READ_ADDRX_B(sbc);
CPU_OP_READ_ADDRX_W(sbc);


// Absolute address, Y indexed, e.g. lda $1337, y
#define CPU_OP_READ_ADDRY_B_DECL(op) cpu_op_read_addry_b_##op
#define CPU_OP_READ_ADDRY_B(op) \
   static inline void CPU_OP_READ_ADDRY_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc() + REGS.y.w; \
      cpu_op_##op##_b (cpu_read(addr)); \
   }

#define CPU_OP_READ_ADDRY_W_DECL(op) cpu_op_read_addry_w_##op
#define CPU_OP_READ_ADDRY_W(op) \
   static inline void CPU_OP_READ_ADDRY_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc() + REGS.y.w; \
      cpu_op_##op##_w (cpu_readw(addr)); \
   }

CPU_OP_READ_ADDRY_B(adc);
CPU_OP_READ_ADDRY_W(adc);
CPU_OP_READ_ADDRY_B(and);
CPU_OP_READ_ADDRY_W(and);
CPU_OP_READ_ADDRY_B(cmp);
CPU_OP_READ_ADDRY_W(cmp);
CPU_OP_READ_ADDRY_B(eor);
CPU_OP_READ_ADDRY_W(eor);
CPU_OP_READ_ADDRY_B(lda);
CPU_OP_READ_ADDRY_W(lda);
CPU_OP_READ_ADDRY_B(ora);
CPU_OP_READ_ADDRY_W(ora);
CPU_OP_READ_ADDRY_B(sbc);
CPU_OP_READ_ADDRY_W(sbc);


// Long addressing (24-bit address), e.g. lda.l $7e1337
#define CPU_OP_READ_LONG_B_DECL(op) cpu_op_read_long_b_##op
#define CPU_OP_READ_LONG_B(op) \
   static inline void CPU_OP_READ_LONG_B_DECL(op) (void) \
   { \
      cpu_op_##op##_b (cpu_readl(cpu_readl_pc())); \
   }

#define CPU_OP_READ_LONG_W_DECL(op) cpu_op_read_long_w_##op
#define CPU_OP_READ_LONG_W(op) \
   static inline void CPU_OP_READ_LONG_W_DECL(op) (void) \
   { \
      cpu_op_##op##_w (cpu_readlw(cpu_readl_pc())); \
   }

CPU_OP_READ_LONG_B(adc);
CPU_OP_READ_LONG_W(adc);
CPU_OP_READ_LONG_B(and);
CPU_OP_READ_LONG_W(and);
CPU_OP_READ_LONG_B(cmp);
CPU_OP_READ_LONG_W(cmp);
CPU_OP_READ_LONG_B(eor);
CPU_OP_READ_LONG_W(eor);
CPU_OP_READ_LONG_B(lda);
CPU_OP_READ_LONG_W(lda);
CPU_OP_READ_LONG_B(ora);
CPU_OP_READ_LONG_W(ora);
CPU_OP_READ_LONG_B(sbc);
CPU_OP_READ_LONG_W(sbc);


// Long addressing, X indexed.
#define CPU_OP_READ_LONGX_B_DECL(op) cpu_op_read_longx_b_##op
#define CPU_OP_READ_LONGX_B(op) \
   static inline void CPU_OP_READ_LONGX_B_DECL(op) (void) \
   { \
      cpu_op_##op##_b (cpu_readl(cpu_readl_pc() + REGS.x.w)); \
   }

#define CPU_OP_READ_LONGX_W_DECL(op) cpu_op_read_longx_w_##op
#define CPU_OP_READ_LONGX_W(op) \
   static inline void CPU_OP_READ_LONGX_W_DECL(op) (void) \
   { \
      cpu_op_##op##_w (cpu_readlw(cpu_readl_pc() + REGS.x.w)); \
   }

CPU_OP_READ_LONGX_B(adc);
CPU_OP_READ_LONGX_W(adc);
CPU_OP_READ_LONGX_B(and);
CPU_OP_READ_LONGX_W(and);
CPU_OP_READ_LONGX_B(cmp);
CPU_OP_READ_LONGX_W(cmp);
CPU_OP_READ_LONGX_B(eor);
CPU_OP_READ_LONGX_W(eor);
CPU_OP_READ_LONGX_B(lda);
CPU_OP_READ_LONGX_W(lda);
CPU_OP_READ_LONGX_B(ora);
CPU_OP_READ_LONGX_W(ora);
CPU_OP_READ_LONGX_B(sbc);
CPU_OP_READ_LONGX_W(sbc);


// Direct page, e.g. lda $14
#define CPU_OP_READ_DP_B_DECL(op) cpu_op_read_dp_b_##op
#define CPU_OP_READ_DP_B(op) \
   static inline void CPU_OP_READ_DP_B_DECL(op) (void) \
   { \
      cpu_op_##op##_b (cpu_read_dp(cpu_read_pc())); \
   }

#define CPU_OP_READ_DP_W_DECL(op) cpu_op_read_dp_w_##op
#define CPU_OP_READ_DP_W(op) \
   static inline void CPU_OP_READ_DP_W_DECL(op) (void) \
   { \
      cpu_op_##op##_w (cpu_readw_dp(cpu_read_pc())); \
   }

CPU_OP_READ_DP_B(adc);
CPU_OP_READ_DP_W(adc);
CPU_OP_READ_DP_B(and);
CPU_OP_READ_DP_W(and);
CPU_OP_READ_DP_B(cmp);
CPU_OP_READ_DP_W(cmp);
CPU_OP_READ_DP_B(eor);
CPU_OP_READ_DP_W(eor);
CPU_OP_READ_DP_B(lda);
CPU_OP_READ_DP_W(lda);
CPU_OP_READ_DP_B(ora);
CPU_OP_READ_DP_W(ora);
CPU_OP_READ_DP_B(sbc);
CPU_OP_READ_DP_W(sbc);


// Direct page, register indexed, e.g. lda $04, x
#define CPU_OP_READ_DPR_B_DECL(op, reg) cpu_op_read_dpr_b_##op##_##reg
#define CPU_OP_READ_DPR_B(op, reg) \
   static inline void CPU_OP_READ_DPR_B_DECL(op, reg) (void) \
   { \
      cpu_op_##op##_b (cpu_read_dp(cpu_read_pc() + REGS.reg.w)); \
   }

#define CPU_OP_READ_DPR_W_DECL(op, reg) cpu_op_read_dpr_w_##op##_##reg
#define CPU_OP_READ_DPR_W(op, reg) \
   static inline void CPU_OP_READ_DPR_W_DECL(op, reg) (void) \
   { \
      cpu_op_##op##_w (cpu_readw_dp(cpu_read_pc() + REGS.reg.w)); \
   }

CPU_OP_READ_DPR_B(adc, x);
CPU_OP_READ_DPR_W(adc, x);
CPU_OP_READ_DPR_B(and, x);
CPU_OP_READ_DPR_W(and, x);
CPU_OP_READ_DPR_B(cmp, x);
CPU_OP_READ_DPR_W(cmp, x);
CPU_OP_READ_DPR_B(eor, x);
CPU_OP_READ_DPR_W(eor, x);
CPU_OP_READ_DPR_B(lda, x);
CPU_OP_READ_DPR_W(lda, x);
CPU_OP_READ_DPR_B(ora, x);
CPU_OP_READ_DPR_W(ora, x);
CPU_OP_READ_DPR_B(sbc, x);
CPU_OP_READ_DPR_W(sbc, x);


// Indirect direct page. Effective address is fetched from direct page address. 
// Double pointers ftw? :)
// E.g. lda ($00)
#define CPU_OP_READ_IDP_B_DECL(op) cpu_op_read_idp_b_##op
#define CPU_OP_READ_IDP_B(op) \
   static inline void CPU_OP_READ_IDP_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_read_dp(cpu_read_pc()); \
      cpu_op_##op##_b (cpu_read(addr)); \
   }

#define CPU_OP_READ_IDP_W_DECL(op) cpu_op_read_idp_w_##op
#define CPU_OP_READ_IDP_W(op) \
   static inline void CPU_OP_READ_IDP_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_read_dp(cpu_read_pc()); \
      cpu_op_##op##_w (cpu_readw(addr)); \
   }


// Indirect direct page, X indexed, e.g lda ($00, x)
#define CPU_OP_READ_IDPX_B_DECL(op) cpu_op_read_idpx_b_##op
#define CPU_OP_READ_IDPX_B(op) \
   static inline void CPU_OP_READ_IDPX_B_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc() + REGS.x.w; \
      uint16_t addr = cpu_readw_dp(dp); \
      cpu_op_##op##_b (cpu_read(addr)); \
   }

#define CPU_OP_READ_IDPX_W_DECL(op) cpu_op_read_idpx_w_##op
#define CPU_OP_READ_IDPX_W(op) \
   static inline void CPU_OP_READ_IDPX_W_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc() + REGS.x.w; \
      uint16_t addr = cpu_readw_dp(dp); \
      cpu_op_##op##_w (cpu_readw(addr)); \
   }


// Indirect direct page, Y indexed, e.g. lda ($00), y
#define CPU_OP_READ_IDPY_B_DECL(op) cpu_op_read_idpy_b_##op
#define CPU_OP_READ_IDPY_B(op) \
   static inline void CPU_OP_READ_IDPY_B_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint16_t addr = cpu_readw_dp(dp); \
      cpu_op_##op##_b (cpu_read(addr + REGS.y.w)); \
   }

#define CPU_OP_READ_IDPY_W_DECL(op) cpu_op_read_idpy_w_##op
#define CPU_OP_READ_IDPY_W(op) \
   static inline void CPU_OP_READ_IDPY_W_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint16_t addr = cpu_readw_dp(dp); \
      cpu_op_##op##_w (cpu_readw(addr + REGS.y.w)); \
   }


// Indirect long addressed direct page. (This is getting pretty crazy now!)
// E.g. lda [$00]
#define CPU_OP_READ_ILDP_B_DECL(op) cpu_op_read_ildp_b_##op
#define CPU_OP_READ_ILDP_B(op) \
   static inline void CPU_OP_READ_ILDP_B_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint32_t addr = cpu_readl_dp(dp); \
      cpu_op_##op##_b (cpu_readl(addr)); \
   }

#define CPU_OP_READ_ILDP_W_DECL(op) cpu_op_read_ildp_w_##op
#define CPU_OP_READ_ILDP_W(op) \
   static inline void CPU_OP_READ_ILDP_W_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint32_t addr = cpu_readl_dp(dp); \
      cpu_op_##op##_w (cpu_readlw(addr)); \
   }


// Indirect long addressed direct page, Y indexed. (It's not over yet! :D)
// E.g. lda [$00], y
#define CPU_OP_READ_ILDPY_B_DECL(op) cpu_op_read_ildpy_b_##op
#define CPU_OP_READ_ILDPY_B(op) \
   static inline void CPU_OP_READ_ILDPY_B_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint32_t addr = cpu_readl_dp(dp); \
      cpu_op_##op##_b (cpu_readl(addr + REGS.y.w)); \
   }

#define CPU_OP_READ_ILDPY_W_DECL(op) cpu_op_read_ildpy_w_##op
#define CPU_OP_READ_ILDPY_W(op) \
   static inline void CPU_OP_READ_ILDPY_W_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint32_t addr = cpu_readl_dp(dp); \
      cpu_op_##op##_w (cpu_readlw(addr + REGS.y.w)); \
   }


// Stack-relative.
// E.g. lda 1, s
#define CPU_OP_READ_SR_B_DECL(op) cpu_op_read_sr_b_##op
#define CPU_OP_READ_SR_B(op) \
   static inline void CPU_OP_READ_SR_B_DECL(op) (void) \
   { \
      cpu_op_##op##_b (cpu_read_sp(cpu_read_pc())); \
   }

#define CPU_OP_READ_SR_W_DECL(op) cpu_op_read_sr_w_##op
#define CPU_OP_READ_SR_W(op) \
   static inline void CPU_OP_READ_SR_W_DECL(op) (void) \
   { \
      cpu_op_##op##_w (cpu_readw_sp(cpu_read_pc())); \
   }


// Indirect stack relative, Y indexed (Here be mighty dragons :D)
// E.g. lda (1, s), y
#define CPU_OP_READ_ISRY_B_DECL(op) cpu_op_read_isry_b_##op
#define CPU_OP_READ_ISRY_B(op) \
   static inline void CPU_OP_READ_ISRY_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_sp(cpu_read_pc()) + REGS.y.w; \
      cpu_op_##op##_b (cpu_read(addr)); \
   }

#define CPU_OP_READ_ISRY_W_DECL(op) cpu_op_read_isry_w_##op
#define CPU_OP_READ_ISRY_W(op) \
   static inline void CPU_OP_READ_ISRY_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_sp(cpu_read_pc()) + REGS.y.w; \
      cpu_op_##op##_w (cpu_readw(addr)); \
   }

#endif
