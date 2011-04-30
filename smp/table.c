#include "table.h"

#include "op.h"

#define NOP smp_op_nop
#define TCALL(x) SMP_OP_TCALL_DECL(x)
#define SET1(x) SMP_OP_SET_DECL(x)
#define CLR1(x) SMP_OP_CLEAR_DECL(x)
#define BBC(x) SMP_OP_BBC_DECL(x)
#define BBS(x) SMP_OP_BBS_DECL(x)

#define ALU(read, op) SMP_OP_ALU_##read##_DECL(op)
#define ALUXY(reg, read) SMP_OP_ALUXY_CMP_DECL(reg, read)
#define ALUW(op) SMP_OP_ALUW_DECL(op)
#define RMW(type, op) SMP_OP_RMW_##type##_DECL(op)
#define RMWW(op) SMP_OP_RMWW_DP_DECL(op)

#define MOVE(dst, src) SMP_OP_MOVE_##dst##_DECL(src)
#define MOVE_(dst, src) SMP_OP_MOVE_DECL(dst, src)

#define BRANCH(flag) SMP_OP_BRANCH_FLAG_DECL(flag)
#define BRANCH_N(flag) SMP_OP_BRANCH_N_FLAG_DECL(flag)

smp_op_t ssnes_smp_optable[256] = {
   NOP,                    // 0x00
   TCALL(0),               // 0x01
   SET1(0),                // 0x02
   BBS(0),                 // 0x03
   ALU(DP, or),            // 0x04
   ALU(ADDR, or),          // 0x05
   ALU(DPX, or),           // 0x06
   ALU(IDPX, or),          // 0x07
   ALU(IMM, or),           // 0x08
   ALU(DP_DP, or),         // 0x09
   ALU(BIT, or1),          // 0x0a
   RMW(DP, asl),           // 0x0b
   RMW(ADDR, asl),         // 0x0c
   smp_op_push_p,          // 0x0d
   RMW(A, inc),            // 0x0e
   smp_op_brk,             // 0x0f

   BRANCH_N(n),            // 0x10
   TCALL(1),               // 0x11
   CLR1(0),                // 0x12
   BBC(0),                 // 0x13
   ALU(DPX, or),           // 0x14
   ALU(ADDRX, or),         // 0x15
   ALU(ADDRY, or),         // 0x16
   ALU(IDPY, or),          // 0x17
   ALU(DP_IMM, or),        // 0x18
   ALU(DPX_DPY, or),       // 0x19
   RMWW(decw),             // 0x1a
   RMW(DPX, asl),          // 0x1b
   RMW(A, asl),            // 0x1c
   RMW(X, dec),            // 0x1d
   ALUXY(x, addr),         // 0x1e
   smp_op_jmpix,           // 0x1f

   smp_op_clrp,            // 0x20
   TCALL(2),               // 0x21
   SET1(1),                // 0x22
   BBS(1),                 // 0x23
   ALU(DP, and),           // 0x24
   ALU(ADDR, and),         // 0x25
   ALU(DPX, and),          // 0x26
   ALU(IDPX, and),         // 0x27
   ALU(IMM, and),          // 0x28
   ALU(DP_DP, and),        // 0x29
   ALU(BIT, orn1),         // 0x2a
   RMW(DP, rol),           // 0x2b
   RMW(ADDR, rol),         // 0x2c
   smp_op_push_a,          // 0x2d
   smp_op_cbne_dp,         // 0x2e
   smp_op_bra,             // 0x2f

   BRANCH(n),              // 0x30
   TCALL(3),               // 0x31
   CLR1(1),                // 0x32
   BBC(1),                 // 0x33

   RMW(X, inc),            // 0x34

   RMWW(incw),             // 0x35


   TCALL(4),               // 0x41
   SET1(2),                // 0x42
   BBS(2),                 // 0x43
   ALU(DP, eor),           // 0x44
   ALU(ADDR, eor),         // 0x45
   ALU(DPIX, eor),         // 0x46
   ALU(IDPX, eor),         // 0x47
   ALU(IMM, eor),          // 0x48
   ALU(DP_DP, eor),        // 0x49
   ALU(BIT, and1),         // 0x4a
   RMW(DP, lsr),           // 0x4b
   RMW(ADDR, lsr),         // 0x4c
   smp_op_push_x,          // 0x4d

   smp_op_pcall,           // 0x4f

   BRANCH_N(v),            // 0x50
   TCALL(5),               // 0x51
   CLR1(2),                // 0x52
   BBC(2),                 // 0x53
   ALU(DPX, eor),          // 0x54
   ALU(ADDRX, eor),        // 0x55
   ALU(ADDRY, eor),        // 0x56
   ALU(IDPY, eor),         // 0x57
   ALU(DP_IMM, eor),       // 0x58
   ALU(DPX_DPY, eor),      // 0x59
   ALUW(cmpw),             // 0x5a
   RMW(DPX, lsr),          // 0x5b
   RMW(A, lsr),            // 0x5c

   smp_op_jmp,             // 0x5f

   TCALL(6),               // 0x61
   SET1(3),                // 0x62
   BBS(3),                 // 0x63
   ALU(DP, cmp),           // 0x64
   ALU(ADDR, cmp),         // 0x65
   ALU(DPIX, cmp),         // 0x66
   ALU(IDPX, cmp),         // 0x67
   ALU(IMM, cmp),          // 0x68
   ALU(DP_DP, cmp),        // 0x69
   ALU(BIT, andn1),        // 0x6a
   RMW(DP, ror),           // 0x6b
   RMW(ADDR, ror),         // 0x6c
   smp_op_push_y,          // 0x6d
   smp_op_dbnz_dp,         // 0x6e
   smp_op_ret,             // 0x6f

   BRANCH(v),              // 0x70
   TCALL(7),               // 0x71
   CLR1(3),                // 0x72
   BBC(3),                 // 0x73
   ALU(DPX, cmp),          // 0x74
   ALU(ADDRX, cmp),        // 0x75
   ALU(ADDRY, cmp),        // 0x76
   ALU(IDPY, cmp),         // 0x77
   ALU(DP_IMM, cmp),       // 0x78
   ALU(DPX_DPY, cmp),      // 0x79
   RMW(DPX, ror),          // 0x7b
   RMW(A, ror),            // 0x7c

   ALUW(addw),             // 0x7a


   smp_op_ret1,            // 0x7f

   TCALL(8),               // 0x81
   SET1(4),                // 0x82
   BBS(4),                 // 0x83
   ALU(DP, adc),           // 0x84
   ALU(ADDR, adc),         // 0x85
   ALU(DPIX, adc),         // 0x86
   ALU(IDPX, adc),         // 0x87
   ALU(IMM, adc),          // 0x88
   ALU(DP_DP, adc),        // 0x89
   ALU(BIT, eor1),         // 0x8a

   smp_op_pop_p,           // 0x8e


   BRANCH_N(c),            // 0x90
   TCALL(9),               // 0x91
   CLR1(4),                // 0x92
   BBC(4),                 // 0x93
   ALU(DPX, adc),          // 0x94
   ALU(ADDRX, adc),        // 0x95
   ALU(ADDRY, adc),        // 0x96
   ALU(IDPY, adc),         // 0x97
   ALU(DP_IMM, adc),       // 0x98
   ALU(DPX_DPY, adc),      // 0x99
   ALUW(subw),             // 0x9a

   smp_op_ei,              // 0xa0
   TCALL(10),              // 0xa1
   SET1(5),                // 0xa2
   BBS(5),                 // 0xa3
   ALU(DP, sbc),           // 0xa4
   ALU(ADDR, sbc),         // 0xa5
   ALU(DPIX, sbc),         // 0xa6
   ALU(IDPX, sbc),         // 0xa7
   ALU(IMM, sbc),          // 0xa8
   ALU(DP_DP, sbc),        // 0xa9
   smp_op_mov1_c_bit,      // 0xaa
   RMW(DP, inc),           // 0xab
   RMW(ADDR, inc),         // 0xac

   smp_op_pop_a,           // 0xae

   BRANCH(c),              // 0xb0
   TCALL(11),              // 0xb1
   CLR1(5),                // 0xb2
   BBC(5),                 // 0xb3
   ALU(DPX, sbc),          // 0xb4
   ALU(ADDRX, sbc),        // 0xb5
   ALU(ADDRY, sbc),        // 0xb6
   ALU(IDPY, sbc),         // 0xb7
   ALU(DP_IMM, sbc),       // 0xb8
   ALU(DPX_DPY, sbc),      // 0xb9
   smp_op_movw_ya_d,       // 0xba
   RMW(DPX, inc),          // 0xbb
   RMW(A, inc),            // 0xbc

   smp_op_di,              // 0xc0
   TCALL(12),              // 0xc1
   SET1(6),                // 0xc2
   BBS(6),                 // 0xc3

   smp_op_mov1_bit_c,      // 0xca
   smp_op_pop_x,           // 0xce
   smp_op_mul,             // 0xcf

   BRANCH_N(z),            // 0xd0
   TCALL(13),              // 0xd1
   CLR1(6),                // 0xd2
   BBC(6),                 // 0xd3
   MOVE_(dpx, a),          // 0xd4
   MOVE_(addrx, a),        // 0xd5
   MOVE_(addry, a),        // 0xd6
   MOVE_(idpy, a),         // 0xd7
   MOVE_(dp, x),           // 0xd8

   smp_op_movw_d_ya,       // 0xda

   smp_op_clrv,            // 0xe0
   TCALL(14),              // 0xe1
   SET1(7),                // 0xe2
   BBS(7),                 // 0xe3
   MOVE(A, dp),            // 0xe4
   MOVE(A, addr),          // 0xe5
   MOVE(A, dpix),          // 0xe6
   MOVE(A, idpx),          // 0xe7
   MOVE(A, const),         // 0xe8
   MOVE(X, addr),          // 0xe9
   smp_op_rmw_not1,        // 0xea 
   smp_op_notc,            // 0xed
   smp_op_pop_y,           // 0xee
   smp_op_stp,             // 0xef Sleep

   BRANCH(z),              // 0xf0
   TCALL(15),              // 0xf1
   CLR1(7),                // 0xf2
   BBC(7),                 // 0xf3
   MOVE(A, dpx),           // 0xf4
   MOVE(A, addrx),         // 0xf5
   MOVE(A, addry),         // 0xf6
   MOVE(A, idpy),          // 0xf7
   MOVE(X, dp),            // 0xf8
   MOVE(X, dpy),           // 0xf9
   smp_op_move_dp_dp,      // 0xfa
   MOVE(Y, dpx),           // 0xfb
   RMW(Y, inc),            // 0xfc
   MOVE(Y, a),             // 0xfd
   smp_op_dbnz_y,          // 0xfe
   smp_op_stp,             // 0xff
};

uint8_t ssnes_smp_cycle_table[256];
