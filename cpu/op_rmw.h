#ifndef __OP_RMW_H
#define __OP_RMW_H

// Read-modify-write opcodes.

#define CPU_OP_RMW_REG_B_DECL(reg, op) cpu_op_rmw_reg_b_##reg##_##op
#define CPU_OP_RMW_REG_B(reg, operation) \
   static inline void CPU_OP_RMW_REG_B_DECL(reg, operation) (void) \
   { \
      REGS.reg.b.l = cpu_op_##operation##_b (REGS.reg.b.l); \
   }

#define CPU_OP_RMW_REG_W_DECL(reg, op) cpu_op_rmw_reg_w_##reg##_##op
#define CPU_OP_RMW_REG_W(reg, op) \
   static inline void CPU_OP_RMW_REG_W_DECL(reg, op) (void) \
   { \
      REGS.reg.w = cpu_op_##operation##_w (REGS.reg.w); \
   }

CPU_OP_RMW_REG_B(a, inc); // inc A
CPU_OP_RMW_REG_W(a, inc);
CPU_OP_RMW_REG_B(x, inc); // inx
CPU_OP_RMW_REG_W(x, inc);
CPU_OP_RMW_REG_B(y, inc); // iny
CPU_OP_RMW_REG_W(y, inc);

CPU_OP_RMW_REG_B(a, dec); // dec A
CPU_OP_RMW_REG_W(a, dec);
CPU_OP_RMW_REG_B(x, dec); // dex
CPU_OP_RMW_REG_W(x, dec);
CPU_OP_RMW_REG_B(y, dec); // dey
CPU_OP_RMW_REG_W(y, dec);

CPU_OP_RMW_REG_B(a, asl); // asl A
CPU_OP_RMW_REG_W(a, asl);

CPU_OP_RMW_REG_B(a, lsr); // lsr A
CPU_OP_RMW_REG_W(a, lsr);

CPU_OP_RMW_REG_B(a, rol); // rol A
CPU_OP_RMW_REG_W(a, rol);

CPU_OP_RMW_REG_B(a, ror); // ror A
CPU_OP_RMW_REG_W(a, ror);

// Absolute addressing
#define CPU_OP_RMW_ADDR_B_DECL(op) cpu_op_rmw_addr_b_##op
#define CPU_OP_RMW_ADDR_B(op) \
   static inline void CPU_OP_RMW_ADDR_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc(); \
      uint8_t res = op(cpu_read(addr)); \
      cpu_write(res); \
   }

#define CPU_OP_RMW_ADDR_W_DECL(op) cpu_op_rmw_addr_w_##op
#define CPU_OP_RMW_ADDR_W(op) \
   static inline void CPU_OP_RMW_ADDR_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc(); \
      uint16_t res = op(cpu_readw(addr)); \
      cpu_writew(res); \
   }

// Absolute addressing, X indexed.
#define CPU_OP_RMW_ADDRX_B_DECL(op) cpu_op_rmw_addrx_b_##op
#define CPU_OP_RMW_ADDRX_B(op) \
   static inline void CPU_OP_RMW_ADDRX_B_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc(); \
      uint8_t res = op(cpu_read(addr)); \
      cpu_write(res); \
   }

#define CPU_OP_RMW_ADDRX_W_DECL(op) cpu_op_rmw_addrx_w_##op
#define CPU_OP_RMW_ADDRX_W(op) \
   static inline void CPU_OP_RMW_ADDRX_W_DECL(op) (void) \
   { \
      uint16_t addr = cpu_readw_pc() + REGS.x.w; \
      uint16_t res = op(cpu_readw(addr)); \
      cpu_writew(res); \
   }



#endif
