#ifndef __OP_READ_H
#define __OP_READ_H

// C templates ;)
// Here we have all the batshit crazy addressing modes of the 65c816 chip.
// All reading modes have byte or word variants depending on register flags (8/16-bit A, X, Y ...)

// Constant, e.g. lda #$ff
#define CPU_OP_READ_CONST_DECL(op) cpu_op_read_const_b_##op
#define CPU_OP_READ_CONST_B(op) \
   static inline void CPU_OP_READ_CONST_DECL(op) (void) \
   { \
      op(cpu_read_pc()); \
   }

#define CPU_OP_READ_CONST_W_DECL(op) cpu_op_read_const_w_##op
#define CPU_OP_READ_CONST_W(op) \
   static inline void CPU_OP_READ_CONST_W(op) (void) \
   { \
      op(cpu_readw_pc()); \
   }

// Not sure what this is. Testing bits for comparison?
static inline void cpu_op_read_bit_const_b(void) 
{
   REGS.p.z = ((op_read_pc() & REGS.a.l) == 0);
}

static inline void cpu_op_read_bit_const_w(void) 
{
   REGS.p.z = ((op_readw_pc() & REGS.a.w) == 0);
}


// Absolute addressing, e.g. lda $1337
#define CPU_OP_READ_ADDR_B_DECL(op) cpu_op_read_addr_b_##op
#define CPU_OP_READ_ADDR_B(op) \
   static inline void CPU_OP_READ_ADDR_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc(); \
      op(cpu_read(addr)); \
   }

#define CPU_OP_READ_ADDR_W_DECL(op) cpu_op_read_addr_w_##op
#define CPU_OP_READ_ADDR_W(op) \
   static inline void CPU_OP_READ_ADDR_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc(); \
      op(cpu_readw(addr)); \
   }


// Absolute address, X indexed, e.g. lda $1337, x
#define CPU_OP_READ_ADDRX_B_DECL(op) cpu_op_read_addrx_b_##op
#define CPU_OP_READ_ADDRX_B(op) \
   static inline void CPU_OP_READ_ADDRX_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc() + REGS.x.w; \
      op(cpu_read(addr)); \
   }

#define CPU_OP_READ_ADDRX_W_DECL(op) cpu_op_read_addrx_w_##op
#define CPU_OP_READ_ADDRX_W(op) \
   static inline void CPU_OP_READ_ADDRX_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc() + REGS.x.w; \
      op(cpu_readw(addr)); \
   }


// Absolute address, Y indexed, e.g. lda $1337, y
#define CPU_OP_READ_ADDRY_B_DECL(op) cpu_op_read_addry_b_##op
#define CPU_OP_READ_ADDRY_B(op) \
   static inline void CPU_OP_READ_ADDRY_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc() + REGS.y.w; \
      op(cpu_read(addr)); \
   }

#define CPU_OP_READ_ADDRY_W_DECL(op) cpu_op_read_addry_w_##op
#define CPU_OP_READ_ADDRY_W(op) \
   static inline void CPU_OP_READ_ADDRY_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc() + REGS.y.w; \
      op(cpu_readw(addr)); \
   }


// Long addressing (24-bit address), e.g. lda.l $7e1337
#define CPU_OP_READ_LONG_B_DECL(op) cpu_op_read_long_b_##op
#define CPU_OP_READ_LONG_B(op) \
   static inline void CPU_OP_READ_LONG_B_DECL(op) (void) \
   { \
      op(cpu_readl(cpu_readl_pc())); \
   }

#define CPU_OP_READ_LONG_W_DECL(op) cpu_op_read_long_w_##op
#define CPU_OP_READ_LONG_W(op) \
   static inline void CPU_OP_READ_LONG_W_DECL(op) (void) \
   { \
      op(cpu_readlw(cpu_readl_pc())); \
   }


// Long addressing, X indexed.
#define CPU_OP_READ_LONGX_B_DECL(op) cpu_op_read_longx_b_##op
#define CPU_OP_READ_LONGX_B(op) \
   static inline void CPU_OP_READ_LONGX_B_DECL(op) (void) \
   { \
      op(cpu_readl(cpu_readl_pc() + REGS.x.w)); \
   }

#define CPU_OP_READ_LONGX_W_DECL(op) cpu_op_read_longx_w_##op
#define CPU_OP_READ_LONGX_W(op) \
   static inline void CPU_OP_READ_LONGX_W_DECL(op) (void) \
   { \
      op(cpu_readlw(cpu_readl_pc() + REGS.x.w)); \
   }


// Direct page, e.g. lda $14
#define CPU_OP_READ_DP_B_DECL(op) cpu_op_read_dp_b_##op
#define CPU_OP_READ_DP_B(op) \
   static inline void CPU_OP_READ_DP_B_DECL(op) (void) \
   { \
      op(cpu_read_dp(cpu_read_pc())); \
   }

#define CPU_OP_READ_DP_W_DECL(op) cpu_op_read_dp_w_##op
#define CPU_OP_READ_DP_W(op) \
   static inline void CPU_OP_READ_DP_W_DECL(op) (void) \
   { \
      op(cpu_readw_dp(cpu_read_pc())); \
   }


// Direct page, register indexed, e.g. lda $04, x
#define CPU_OP_READ_DPR_B_DECL(op, reg) cpu_op_read_dpr_b_##op_##reg
#define CPU_OP_READ_DPR_B(op, reg) \
   static inline void CPU_OP_READ_DPR_B(op, reg) (void) \
   { \
      op(cpu_read_dp(cpu_read_pc() + REGS.reg.w)); \
   }

#define CPU_OP_READ_DPR_W_DECL(op, reg) cpu_op_read_dpr_w_##op_##reg
#define CPU_OP_READ_DPR_W(op, reg) \
   static inline void CPU_OP_READ_DPR_W(op, reg) (void) \
   { \
      op(cpu_readw_dp(cpu_read_pc() + REGS.reg.w)); \
   }


// Indirect direct page. Effective address is fetched from direct page address. 
// Double pointers ftw? :)
// E.g. lda ($00)
#define CPU_OP_READ_IDP_B_DECL(op) cpu_op_read_idp_b_##op
#define CPU_OP_READ_IDP_B(op) \
   static inline void CPU_OP_READ_IDP_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_read_dp(cpu_read_pc()); \
      op(cpu_read(addr)); \
   }

#define CPU_OP_READ_IDP_W_DECL(op) cpu_op_read_idp_w_##op
#define CPU_OP_READ_IDP_W(op) \
   static inline void CPU_OP_READ_IDP_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_read_dp(cpu_read_pc()); \
      op(cpu_readw(addr)); \
   }


// Indirect direct page, X indexed, e.g lda ($00, x)
#define CPU_OP_READ_IDPX_B_DECL(op) cpu_op_read_idpx_b_##op
#define CPU_OP_READ_IDPX_B(op) \
   static inline void CPU_OP_READ_IDPX_B_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc() + REGS.x.w; \
      uint16_t addr = cpu_readw_dp(dp); \
      op(cpu_read(addr)); \
   }

#define CPU_OP_READ_IDPX_W_DECL(op) cpu_op_read_idpx_w_##op
#define CPU_OP_READ_IDPX_W(op) \
   static inline void CPU_OP_READ_IDPX_W_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc() + REGS.x.w; \
      uint16_t addr = cpu_readw_dp(dp); \
      op(cpu_readw(addr)); \
   }


// Indirect direct page, Y indexed, e.g. lda ($00), y
#define CPU_OP_READ_IDPY_B_DECL(op) cpu_op_read_idpy_b_##op
#define CPU_OP_READ_IDPY_B(op) \
   static inline void CPU_OP_READ_IDPY_B_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint16_t addr = cpu_readw_dp(dp); \
      op(cpu_read(addr + REGS.y.w)); \
   }

#define CPU_OP_READ_IDPY_W_DECL(op) cpu_op_read_idpy_w_##op
#define CPU_OP_READ_IDPY_W(op) \
   static inline void CPU_OP_READ_IDPY_W_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint16_t addr = cpu_readw_dp(dp); \
      op(cpu_readw(addr + REGS.y.w)); \
   }


// Indirect long addressed direct page. (This is getting pretty crazy now!)
// E.g. lda [$00]
#define CPU_OP_READ_ILDP_B_DECL(op) cpu_op_read_ildp_b_##op
#define CPU_OP_READ_ILDP_B(op) \
   static inline void CPU_OP_READ_ILDP_B_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint32_t addr = cpu_readl_dp(dp); \
      op(cpu_readl(addr)); \
   }

#define CPU_OP_READ_ILDP_W_DECL(op) cpu_op_read_ildp_w_##op
#define CPU_OP_READ_ILDP_W(op) \
   static inline void CPU_OP_READ_ILDP_W_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint32_t addr = cpu_readl_dp(dp); \
      op(cpu_readlw(addr)); \
   }


// Indirect long addressed direct page, Y indexed. (It's not over yet! :D)
// E.g. lda [$00], y
#define CPU_OP_READ_ILDPY_B_DECL(op) cpu_op_read_ildpy_b_##op
#define CPU_OP_READ_ILDPY_B(op) \
   static inline void CPU_OP_READ_ILDPY_B_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint32_t addr = cpu_readl_dp(dp); \
      op(cpu_readl(addr + REGS.y.w)); \
   }

#define CPU_OP_READ_ILDPY_W_DECL(op) cpu_op_read_ildpy_w_##op
#define CPU_OP_READ_ILDPY_W(op) \
   static inline void CPU_OP_READ_ILDPY_W_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint32_t addr = cpu_readl_dp(dp); \
      op(cpu_readlw(addr + REGS.y.w)); \
   }


// Stack-relative.
// E.g. lda 1, s
#define CPU_OP_READ_SR_B_DECL(op) cpu_op_read_sr_b_##op
#define CPU_OP_READ_SR_B(op) \
   static inline void CPU_OP_READ_SR_B_DECL(op) (void) \
   { \
      op(cpu_read_sp(cpu_read_pc())); \
   }

#define CPU_OP_READ_SR_W_DECL(op) cpu_op_read_sr_w_##op
#define CPU_OP_READ_SR_W(op) \
   static inline void CPU_OP_READ_SR_W_DECL(op) (void) \
   { \
      op(cpu_readw_sp(cpu_read_pc())); \
   }


// Indirect stack relative, Y indexed (Here be mighty dragons :D)
// E.g. lda (1, s), y
#define CPU_OP_READ_ISRY_B_DECL(op) cpu_op_read_isry_b_##op
#define CPU_OP_READ_ISRY_B(op) \
   static inline void CPU_OP_READ_ISRY_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_sp(cpu_read_pc()) + REGS.y.w; \
      op(cpu_read(addr)); \
   }

#define CPU_OP_READ_ISRY_W_DECL(op) cpu_op_read_isry_w_##op
#define CPU_OP_READ_ISRY_W(op) \
   static inline void CPU_OP_READ_ISRY_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_sp(cpu_read_pc()) + REGS.y.w; \
      op(cpu_readw(addr)); \
   }

#endif
