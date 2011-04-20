#ifndef __OP_RMW_H
#define __OP_RMW_H

// Read-modify-write opcodes.
// Some group 2 instructions.

// Direct register access. Accumulator addressing, but also x and y for inc/dec.
#define CPU_OP_RMW_REG_B_DECL(reg, op) cpu_op_rmw_reg_##reg##_b_##op
#define CPU_OP_RMW_REG_B(reg, op) \
   static inline void CPU_OP_RMW_REG_B_DECL(reg, op) (void) \
   { \
      REGS.reg.b.l = cpu_op_##op##_b (REGS.reg.b.l); \
   }

#define CPU_OP_RMW_REG_W_DECL(reg, op) cpu_op_rmw_reg_##reg##_w_##op
#define CPU_OP_RMW_REG_W(reg, op) \
   static inline void CPU_OP_RMW_REG_W_DECL(reg, op) (void) \
   { \
      REGS.reg.w = cpu_op_##op##_w (REGS.reg.w); \
   }

CPU_OP_RMW_REG_B(x, inc); // inx
CPU_OP_RMW_REG_W(x, inc);
CPU_OP_RMW_REG_B(y, inc); // iny
CPU_OP_RMW_REG_W(y, inc);
CPU_OP_RMW_REG_B(x, dec); // dex
CPU_OP_RMW_REG_W(x, dec);
CPU_OP_RMW_REG_B(y, dec); // dey
CPU_OP_RMW_REG_W(y, dec);

CPU_OP_RMW_REG_B(a, inc); // inc A
CPU_OP_RMW_REG_W(a, inc);
CPU_OP_RMW_REG_B(a, dec); // dec A
CPU_OP_RMW_REG_W(a, dec);
CPU_OP_RMW_REG_B(a, asl); // asl A
CPU_OP_RMW_REG_W(a, asl);
CPU_OP_RMW_REG_B(a, lsr); // lsr A
CPU_OP_RMW_REG_W(a, lsr);
CPU_OP_RMW_REG_B(a, rol); // rol A
CPU_OP_RMW_REG_W(a, rol);
CPU_OP_RMW_REG_B(a, ror); // ror A
CPU_OP_RMW_REG_W(a, ror);


#define DECL_GROUP_TWO(instr) \
   CPU_OP_RMW_##instr##_B(asl) \
   CPU_OP_RMW_##instr##_W(asl) \
   CPU_OP_RMW_##instr##_B(dec) \
   CPU_OP_RMW_##instr##_W(dec) \
   CPU_OP_RMW_##instr##_B(inc) \
   CPU_OP_RMW_##instr##_W(inc) \
   CPU_OP_RMW_##instr##_B(lsr) \
   CPU_OP_RMW_##instr##_W(lsr) \
   CPU_OP_RMW_##instr##_B(rol) \
   CPU_OP_RMW_##instr##_W(rol) \
   CPU_OP_RMW_##instr##_B(ror)

// Absolute addressing
#define CPU_OP_RMW_ADDR_B_DECL(op) cpu_op_rmw_addr_b_##op
#define CPU_OP_RMW_ADDR_B(op) \
   static inline void CPU_OP_RMW_ADDR_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc(); \
      uint8_t res = cpu_op_##op##_b (cpu_read(addr)); \
      cpu_write(addr, res); \
   }

#define CPU_OP_RMW_ADDR_W_DECL(op) cpu_op_rmw_addr_w_##op
#define CPU_OP_RMW_ADDR_W(op) \
   static inline void CPU_OP_RMW_ADDR_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc(); \
      uint16_t res = cpu_op_##op##_w (cpu_readw(addr)); \
      cpu_writew(addr, res); \
   }

DECL_GROUP_TWO(ADDR)
CPU_OP_RMW_ADDR_B(tsb)
CPU_OP_RMW_ADDR_W(tsb)

// Absolute addressing, X indexed.
#define CPU_OP_RMW_ADDRX_B_DECL(op) cpu_op_rmw_addrx_b_##op
#define CPU_OP_RMW_ADDRX_B(op) \
   static inline void CPU_OP_RMW_ADDRX_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc(); \
      uint8_t res = cpu_op_##op##_b (cpu_read(addr)); \
      cpu_write(addr, res); \
   }


#define CPU_OP_RMW_ADDRX_W_DECL(op) cpu_op_rmw_addrx_w_##op
#define CPU_OP_RMW_ADDRX_W(op) \
   static inline void CPU_OP_RMW_ADDRX_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc() + REGS.x.w; \
      uint16_t res = cpu_op_##op##_w (cpu_readw(addr)); \
      cpu_writew(addr, res); \
   }

DECL_GROUP_TWO(ADDRX)

// Direct page
#define CPU_OP_RMW_DP_B_DECL(op) cpu_op_rmw_dp_b_##op
#define CPU_OP_RMW_DP_B(op) \
   static inline void CPU_OP_RMW_DP_B_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint8_t res = cpu_op_##op##_b (cpu_read_dp(dp)); \
      cpu_write_dp(dp, res); \
   }


#define CPU_OP_RMW_DP_W_DECL(op) cpu_op_rmw_dp_w_##op
#define CPU_OP_RMW_DP_W(op) \
   static inline void CPU_OP_RMW_DP_W_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc(); \
      uint16_t res = cpu_op_##op##_w (cpu_readw_dp(dp)); \
      cpu_writew_dp(dp, res); \
   }

DECL_GROUP_TWO(DP)
CPU_OP_RMW_DP_B(tsb)
CPU_OP_RMW_DP_W(tsb)

// Direct page, X indexed
#define CPU_OP_RMW_DPX_B_DECL(op) cpu_op_rmw_dpx_b_##op
#define CPU_OP_RMW_DPX_B(op) \
   static inline void CPU_OP_RMW_DPX_B_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc() + REGS.x.w; \
      uint8_t res = cpu_op_##op##_b (cpu_read_dp(dp)); \
      cpu_write_dp(dp, res); \
   }

#define CPU_OP_RMW_DPX_W_DECL(op) cpu_op_rmw_dpx_w_##op
#define CPU_OP_RMW_DPX_W(op) \
   static inline void CPU_OP_RMW_DPX_W_DECL(op) (void) \
   { \
      uint8_t dp = cpu_read_pc() + REGS.x.w; \
      uint16_t res = cpu_op_##op##_w (cpu_readw_dp(dp)); \
      cpu_writew_dp(dp, res); \
   }

DECL_GROUP_TWO(DPX)

#endif
