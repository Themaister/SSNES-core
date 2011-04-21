#ifndef __OP_READ_H
#define __OP_READ_H

// C templates ;)
// Here we have all the batshit crazy addressing modes of the 65c816 chip.
// All reading modes have byte or word variants depending on register flags (8/16-bit A, X, Y ...)
//

// Group 1 instruction. They have access to all addressing modes.
#define DECL_GROUP_ONE(instr) \
   CPU_OP_##instr##_B(adc) \
   CPU_OP_##instr##_W(adc) \
   CPU_OP_##instr##_B(and) \
   CPU_OP_##instr##_W(and) \
   CPU_OP_##instr##_B(cmp) \
   CPU_OP_##instr##_W(cmp) \
   CPU_OP_##instr##_B(eor) \
   CPU_OP_##instr##_W(eor) \
   CPU_OP_##instr##_B(lda) \
   CPU_OP_##instr##_W(lda) \
   CPU_OP_##instr##_B(ora) \
   CPU_OP_##instr##_W(ora) \
   CPU_OP_##instr##_B(sbc) \
   CPU_OP_##instr##_W(sbc)

#define DECL_GROUP_ONE_REG(instr, reg) \
   CPU_OP_##instr##_B(adc, reg) \
   CPU_OP_##instr##_W(adc, reg) \
   CPU_OP_##instr##_B(and, reg) \
   CPU_OP_##instr##_W(and, reg) \
   CPU_OP_##instr##_B(cmp, reg) \
   CPU_OP_##instr##_W(cmp, reg) \
   CPU_OP_##instr##_B(eor, reg) \
   CPU_OP_##instr##_W(eor, reg) \
   CPU_OP_##instr##_B(lda, reg) \
   CPU_OP_##instr##_W(lda, reg) \
   CPU_OP_##instr##_B(ora, reg) \
   CPU_OP_##instr##_W(ora, reg) \
   CPU_OP_##instr##_B(sbc, reg) \
   CPU_OP_##instr##_W(sbc, reg)


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

DECL_GROUP_ONE(READ_CONST)

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

DECL_GROUP_ONE(READ_ADDR)
CPU_OP_READ_ADDR_B(bit)

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

DECL_GROUP_ONE(READ_ADDRX)

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

DECL_GROUP_ONE(READ_ADDRY)

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

DECL_GROUP_ONE(READ_LONG)

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

DECL_GROUP_ONE(READ_LONGX)

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

DECL_GROUP_ONE(READ_DP)
CPU_OP_READ_DP_B(bit)

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

DECL_GROUP_ONE_REG(READ_DPR, x)

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

DECL_GROUP_ONE(READ_IDP)


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

DECL_GROUP_ONE(READ_IDPX)

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

DECL_GROUP_ONE(READ_IDPY)

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

DECL_GROUP_ONE(READ_ILDP)

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

DECL_GROUP_ONE(READ_ILDPY)

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

DECL_GROUP_ONE(READ_SR)

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

DECL_GROUP_ONE(READ_ISRY)


#endif
