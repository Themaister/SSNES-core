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
   L  rd.l = cpu_op_readpc(); \
      op(); \
   }

#define CPU_OP_READ_CONST_W_DECL(op) cpu_op_read_const_w_##op
#define CPU_OP_READ_CONST_W(op) \
   static inline void CPU_OP_READ_CONST_W(op) (void) \
   { \
      rd.l = cpu_op_readpc(); \
   L  rd.h = cpu_op_readpc(); \
      op(); \
   }

// Not sure what this is. Testing bits for comparison?
static inline void CPUcore::op_read_bit_const_b(void) 
{
L  rd.l = op_readpc();
   regs.p.z = ((rd.l & regs.a.l) == 0);
}

static inline void CPUcore::op_read_bit_const_w(void) 
{
   rd.l = op_readpc();
L  rd.h = op_readpc();
   regs.p.z = ((rd.w & regs.a.w) == 0);
}

// Absolute addressing.
#define CPU_OP_READ_ADDR_B_DECL(op) cpu_op_read_addr_b_##op
#define CPU_OP_READ_ADDR_B(op) \
   static inline void CPU_OP_READ_ADDR_B_DECL(op) (void) \
   { \
      aa.l = op_readpc(); \
      aa.h = op_readpc(); \
   L  rd.l = op_readdbr(aa.w); \
      op(); \
   }

#define CPU_OP_READ_ADDR_W_DECL(op) cpu_op_read_addr_w_##op
#define CPU_OP_READ_ADDR_W(op) \
   static inline void CPU_OP_READ_ADDR_W_DECL(op) (void) \
   { \
      aa.l = op_readpc(); \
      aa.h = op_readpc(); \
      rd.l = op_readdbr(aa.w + 0); \
   L  rd.h = op_readdbr(aa.w + 1); \
      op(); \
   }

// Absolute address, X indexed.
#define CPU_OP_READ_ADDRX_B_DECL(op) cpu_op_read_addrx_b_##op
#define CPU_OP_READ_ADDRX_B(op) \
   static inline void CPU_OP_READ_ADDRX_B_DECL(op) (void) \
   { \
      aa.l = op_readpc(); \
      aa.h = op_readpc(); \
      op_io_cond4(aa.w, aa.w + regs.x.w); \
   L  rd.l = op_readdbr(aa.w + regs.x.w); \
      op(); \
   }
// ?!? What is op_io_cond4 supposed to do?

#define CPU_OP_READ_ADDRX_W_DECL(op) cpu_op_read_addrx_w_##op
#define CPU_OP_READ_ADDRX_W(op) \
   static inline void CPU_OP_READ_ADDRX_W_DECL(op) (void) \
   { \
      aa.l = op_readpc(); \
      aa.h = op_readpc(); \
      op_io_cond4(aa.w, aa.w + regs.x.w); \
      rd.l = op_readdbr(aa.w + regs.x.w + 0); \
   L  rd.h = op_readdbr(aa.w + regs.x.w + 1); \
      op(); \
   }

// Absolute address, Y indexed.
#define CPU_OP_READ_ADDRY_B_DECL(op) cpu_op_read_addry_b_##op
#define CPU_OP_READ_ADDRY_B(op) \
   static inline void CPU_OP_READ_ADDRY_B_DECL(op) (void) \
   { \
      aa.l = op_readpc(); \
      aa.h = op_readpc(); \
      op_io_cond4(aa.w, aa.w + regs.y.w); \
   L  rd.l = op_readdbr(aa.w + regs.y.w); \
      op(); \
   }

#define CPU_OP_READ_ADDRY_W_DECL(op) cpu_op_read_addry_w_##op
#define CPU_OP_READ_ADDRY_W(op) \
   static inline void CPU_OP_READ_ADDRY_W_DECL(op) (void) \
   { \
      aa.l = op_readpc(); \
      aa.h = op_readpc(); \
      op_io_cond4(aa.w, aa.w + regs.y.w); \
      rd.l = op_readdbr(aa.w + regs.y.w + 0); \
   L  rd.h = op_readdbr(aa.w + regs.y.w + 1); \
      op(); \
   }

// Long addressing (24-bit address).
#define CPU_OP_READ_LONG_B_DECL(op) cpu_op_read_long_b_##op
#define CPU_OP_READ_LONG_B(op) \
   static inline void CPU_OP_READ_LONG_B_DECL(op) (void) \
   { \
      aa.l = op_readpc(); \
      aa.h = op_readpc(); \
      aa.b = op_readpc(); \
   L  rd.l = op_readlong(aa.d); \
      op(); \
   }

#define CPU_OP_READ_LONG_W_DECL(op) cpu_op_read_long_w_##op
#define CPU_OP_READ_LONG_W(op) \
   static inline void CPU_OP_READ_LONG_W_DECL(op) (void) \
   { \
      aa.l = op_readpc(); \
      aa.h = op_readpc(); \
      aa.b = op_readpc(); \
      rd.l = op_readlong(aa.d + 0); \
   L  rd.h = op_readlong(aa.d + 1); \
      op(); \
   }

// Long addressing, X indexed.
#define CPU_OP_READ_LONGX_B_DECL(op) cpu_op_read_longx_b_##op
#define CPU_OP_READ_LONGX_B(op) \
   static inline void CPU_OP_READ_LONGX_B_DECL(op) (void) \
   { \
      aa.l = op_readpc(); \
      aa.h = op_readpc(); \
      aa.b = op_readpc(); \
   L  rd.l = op_readlong(aa.d + regs.x.w); \
      op(); \
   }

#define CPU_OP_READ_LONGX_W_DECL(op) cpu_op_read_longx_w_##op
#define CPU_OP_READ_LONGX_W(op) \
   static inline void CPU_OP_READ_LONGX_W_DECL(op) (void) \
   { \
      aa.l = op_readpc(); \
      aa.h = op_readpc(); \
      aa.b = op_readpc(); \
      rd.l = op_readlong(aa.d + regs.x.w + 0); \
   L  rd.h = op_readlong(aa.d + regs.x.w + 1); \
      op(); \
   }

// Direct page.
#define CPU_OP_READ_DP_B_DECL(op) cpu_op_read_dp_b_##op
#define CPU_OP_READ_DP_B(op) \
   static inline void CPU_OP_READ_DP_B_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
   L  rd.l = op_readdp(dp); \
      op(); \
   }
// ?!? op_io_cond2()

#define CPU_OP_READ_DP_W_DECL(op) cpu_op_read_dp_w_##op
#define CPU_OP_READ_DP_W(op) \
   static inline void CPU_OP_READ_DP_W_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
      rd.l = op_readdp(dp + 0); \
   L  rd.h = op_readdp(dp + 1); \
      op(); \
   }

// Direct page, register indexed?
#if 0
template<void (CPUcore::*op)(), int n> void CPUcore::op_read_dpr_b() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
L rd.l = op_readdp(dp + regs.r[n].w);
  call(op);
}

template<void (CPUcore::*op)(), int n> void CPUcore::op_read_dpr_w() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  rd.l = op_readdp(dp + regs.r[n].w + 0);
L rd.h = op_readdp(dp + regs.r[n].w + 1);
  call(op);
}
#endif

// Indirect direct page. Effective address is fetched from direct page address. 
// Double pointers ftw? :)
#define CPU_OP_READ_IDP_B_DECL(op) cpu_op_read_idp_b_##op
#define CPU_OP_READ_IDP_B(op) \
   static inline void CPU_OP_READ_IDP_B_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
      aa.l = op_readdp(dp + 0); \
      aa.h = op_readdp(dp + 1); \
   L  rd.l = op_readdbr(aa.w); \
      op(); \
   }

#define CPU_OP_READ_IDP_W_DECL(op) cpu_op_read_idp_w_##op
#define CPU_OP_READ_IDP_W(op) \
   static inline void CPU_OP_READ_IDP_W_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
      aa.l = op_readdp(dp + 0); \
      aa.h = op_readdp(dp + 1); \
      rd.l = op_readdbr(aa.w + 0); \
      L rd.h = op_readdbr(aa.w + 1); \
      op(); \
   }

// Indirect direct page, X indexed.
#define CPU_OP_READ_IDPX_B_DECL(op) cpu_op_read_idpx_b_##op
#define CPU_OP_READ_IDPX_B(op) \
   static inline void CPU_OP_READ_IDPX_B_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
      op_io(); \
      aa.l = op_readdp(dp + regs.x.w + 0); \
      aa.h = op_readdp(dp + regs.x.w + 1); \
   L  rd.l = op_readdbr(aa.w); \
      op(); \
   }

#define CPU_OP_READ_IDPX_W_DECL(op) cpu_op_read_idpx_w_##op
#define CPU_OP_READ_IDPX_W(op) \
   static inline void CPU_OP_READ_IDPX_W_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
      op_io(); \
      aa.l = op_readdp(dp + regs.x.w + 0); \
      aa.h = op_readdp(dp + regs.x.w + 1); \
      rd.l = op_readdbr(aa.w + 0); \
   L  rd.h = op_readdbr(aa.w + 1); \
      op(); \
   }

// Indirect direct page, Y indexed.
#define CPU_OP_READ_IDPY_B_DECL(op) cpu_op_read_idpy_b_##op
#define CPU_OP_READ_IDPY_B(op) \
   static inline void CPU_OP_READ_IDPY_B_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
      aa.l = op_readdp(dp + 0); \
      aa.h = op_readdp(dp + 1); \
      op_io_cond4(aa.w, aa.w + regs.y.w); \
   L  rd.l = op_readdbr(aa.w + regs.y.w); \
      op(); \
   }

#define CPU_OP_READ_IDPY_W_DECL(op) cpu_op_read_idpy_w_##op
#define CPU_OP_READ_IDPY_W(op) \
   static inline void CPU_OP_READ_IDPY_W_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
      aa.l = op_readdp(dp + 0); \
      aa.h = op_readdp(dp + 1); \
      op_io_cond4(aa.w, aa.w + regs.y.w); \
      rd.l = op_readdbr(aa.w + regs.y.w + 0); \
   L  rd.h = op_readdbr(aa.w + regs.y.w + 1); \
      op(); \
   }

// Indirect long addressed direct page. (This is getting pretty crazy now!)
#define CPU_OP_READ_ILDP_B_DECL(op) cpu_op_read_ildp_b_##op
#define CPU_OP_READ_ILDP_B(op) \
   static inline void CPU_OP_READ_ILDP_B_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
      aa.l = op_readdp(dp + 0); \
      aa.h = op_readdp(dp + 1); \
      aa.b = op_readdp(dp + 2); \
   L  rd.l = op_readlong(aa.d); \
      op(); \
   }

#define CPU_OP_READ_ILDP_W_DECL(op) cpu_op_read_ildp_w_##op
#define CPU_OP_READ_ILDP_W(op) \
   static inline void CPU_OP_READ_ILDP_W_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
      aa.l = op_readdp(dp + 0); \
      aa.h = op_readdp(dp + 1); \
      aa.b = op_readdp(dp + 2); \
      rd.l = op_readlong(aa.d + 0); \
   L  rd.h = op_readlong(aa.d + 1); \
      op(); \
   }

// Indirect long addressed direct page, Y indexed. (It's not over yet! :D)
#define CPU_OP_READ_ILDPY_B_DECL(op) cpu_op_read_ildpy_b_##op
#define CPU_OP_READ_ILDPY_B(op) \
   static inline void CPU_OP_READ_ILDPY_B_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
      aa.l = op_readdp(dp + 0); \
      aa.h = op_readdp(dp + 1); \
      aa.b = op_readdp(dp + 2); \
   L  rd.l = op_readlong(aa.d + regs.y.w); \
      op(); \
   }

#define CPU_OP_READ_ILDPY_W_DECL(op) cpu_op_read_ildpy_w_##op
#define CPU_OP_READ_ILDPY_W(op) \
   static inline void CPU_OP_READ_ILDPY_W_DECL(op) (void) \
   { \
      dp = op_readpc(); \
      op_io_cond2(); \
      aa.l = op_readdp(dp + 0); \
      aa.h = op_readdp(dp + 1); \
      aa.b = op_readdp(dp + 2); \
      rd.l = op_readlong(aa.d + regs.y.w + 0); \
   L  rd.h = op_readlong(aa.d + regs.y.w + 1); \
      op(); \
   }

// Stack-relative
#define CPU_OP_READ_SR_B_DECL(op) cpu_op_read_sr_b_##op
#define CPU_OP_READ_SR_B(op) \
   static inline void CPU_OP_READ_SR_B_DECL(op) (void) \
   { \
      sp = op_readpc(); \
      op_io(); \
   L  rd.l = op_readsp(sp); \
      op(); \
   }

#define CPU_OP_READ_SR_W_DECL(op) cpu_op_read_sr_w_##op
#define CPU_OP_READ_SR_W(op) \
   static inline void CPU_OP_READ_SR_W_DECL(op) (void) \
   { \
      sp = op_readpc(); \
      op_io(); \
      rd.l = op_readsp(sp + 0); \
   L  rd.h = op_readsp(sp + 1); \
      op(); \
   }

// Indirect stack relative, Y indexed (Here be mighty dragons :D)
#define CPU_OP_READ_ISRY_B_DECL(op) cpu_op_read_isry_b_##op
#define CPU_OP_READ_ISRY_B(op) \
   static inline void CPU_OP_READ_ISRY_B_DECL(op) (void) \
   { \
      sp = op_readpc(); \
      op_io(); \
      aa.l = op_readsp(sp + 0); \
      aa.h = op_readsp(sp + 1); \
      op_io(); \
   L  rd.l = op_readdbr(aa.w + regs.y.w); \
      op(); \
   }

#define CPU_OP_READ_ISRY_W_DECL(op) cpu_op_read_isry_w_##op
#define CPU_OP_READ_ISRY_W(op) \
   static inline void CPU_OP_READ_ISRY_W_DECL(op) (void) \
   { \
      sp = op_readpc(); \
      op_io(); \
      aa.l = op_readsp(sp + 0); \
      aa.h = op_readsp(sp + 1); \
      op_io(); \
      rd.l = op_readdbr(aa.w + regs.y.w + 0); \
      L rd.h = op_readdbr(aa.w + regs.y.w + 1); \
      op(); \
   }

#endif
