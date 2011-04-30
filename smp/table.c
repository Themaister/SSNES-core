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

#define CMP(read1, read2) SMP_OP_ALU_CMP_DECL(read1, read2)

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
   smp_op_rmw_tset,        // 0x0e
   smp_op_brk,             // 0x0f

   BRANCH_N(n),            // 0x10
   TCALL(1),               // 0x11
   CLR1(0),                // 0x12
   BBC(0),                 // 0x13
   ALU(DPIX, or),          // 0x14
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
   ALU(DPIX, and),         // 0x34
   ALU(ADDRX, and),        // 0x35
   ALU(ADDRY, and),        // 0x36
   ALU(IDPY, and),         // 0x37
   ALU(DP_IMM, and),       // 0x38
   ALU(DPX_DPY, and),      // 0x39
   RMWW(incw),             // 0x3a
   RMW(DPX, rol),          // 0x3b
   RMW(A, rol),            // 0x3c
   RMW(X, inc),            // 0x3d
   ALUXY(x, dp),           // 0x3e
   smp_op_call,            // 0x3f

   smp_op_setp,            // 0x40
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
   smp_op_rmw_tclr,        // 0x4e
   smp_op_pcall,           // 0x4f

   BRANCH_N(v),            // 0x50
   TCALL(5),               // 0x51
   CLR1(2),                // 0x52
   BBC(2),                 // 0x53
   ALU(DPIX, eor),         // 0x54
   ALU(ADDRX, eor),        // 0x55
   ALU(ADDRY, eor),        // 0x56
   ALU(IDPY, eor),         // 0x57
   ALU(DP_IMM, eor),       // 0x58
   ALU(DPX_DPY, eor),      // 0x59
   ALUW(cmpw),             // 0x5a
   RMW(DPX, lsr),          // 0x5b
   RMW(A, lsr),            // 0x5c
   MOVE(X, a),             // 0x5d
   ALUXY(y, addr),         // 0x5e
   smp_op_jmp,             // 0x5f

   smp_op_clrc,            // 0x60
   TCALL(6),               // 0x61
   SET1(3),                // 0x62
   BBS(3),                 // 0x63
   CMP(a, dp),             // 0x64
   CMP(a, addr),           // 0x65
   CMP(a, dpx),            // 0x66
   CMP(a, idpx),           // 0x67
   CMP(a, const),          // 0x68
   CMP(dp, dp),            // 0x69
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
   CMP(a, dpx),            // 0x74
   CMP(a, addrx),          // 0x75
   CMP(a, addry),          // 0x76
   CMP(a, idpy),           // 0x77
   CMP(dp, const),         // 0x78
   CMP(dpx, dpy),          // 0x79
   ALUW(addw),             // 0x7a
   RMW(DPX, ror),          // 0x7b
   RMW(A, ror),            // 0x7c
   MOVE(A, x),             // 0x7d
   ALUXY(y, dp),           // 0x7e
   smp_op_ret1,            // 0x7f

   smp_op_setc,            // 0x80
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
   RMW(DP, dec),           // 0x8b
   RMW(ADDR, dec),         // 0x8c
   MOVE(Y, const),         // 0x8d
   smp_op_pop_p,           // 0x8e
   MOVE_(dp, const),       // 0x8f

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
   RMW(DPX, dec),          // 0x9b
   RMW(A, dec),            // 0x9c
   MOVE(X, sp),            // 0x9d
   smp_op_div,             // 0x9e
   smp_op_xcn,             // 0x9f

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
   ALUXY(y, const),        // 0xad
   smp_op_pop_a,           // 0xae
   MOVE_(dpx_inc, a),      // 0xaf

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
   smp_op_move_sp_x,       // 0xbd
   smp_op_das,             // 0xbe
   MOVE(A, dpx_inc),       // 0xbf

   smp_op_di,              // 0xc0
   TCALL(12),              // 0xc1
   SET1(6),                // 0xc2
   BBS(6),                 // 0xc3
   MOVE_(dp, a),           // 0xc4
   MOVE_(addr, a),         // 0xc5
   MOVE_(dpx, a),          // 0xc6
   MOVE_(idpx, a),         // 0xc7
   ALUXY(x, const),        // 0xc8
   MOVE_(addr, x),         // 0xc9
   smp_op_mov1_bit_c,      // 0xca
   MOVE_(dp, y),           // 0xcb
   MOVE_(addr, y),         // 0xcc
   MOVE(X, const),         // 0xcd
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
   MOVE_(dpiy, x),         // 0xd9
   smp_op_movw_d_ya,       // 0xda
   MOVE_(dpix, y),         // 0xdb
   RMW(Y, dec),            // 0xdc
   MOVE(A, y),             // 0xdd
   smp_op_cbne_dpix,       // 0xde
   smp_op_daa,             // 0xdf

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
   MOVE(Y, dp),            // 0xeb
   MOVE(Y, addr),          // 0xec
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

// Dummy values for now.
uint8_t ssnes_smp_cycle_table[256] = {
   50, 50, 50, 50, 50, 50, 50, 50, // 0x00
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0x10
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0x20
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0x30
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0x40
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0x50
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0x60
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0x70
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0x80
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0x90
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0xa0
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0xb0
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0xc0
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0xd0
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0xe0
   50, 50, 50, 50, 50, 50, 50, 50,

   50, 50, 50, 50, 50, 50, 50, 50, // 0xf0
   50, 50, 50, 50, 50, 50, 50, 50
};

const char* ssnes_smp_opname[256] = {
   "nop", "tcall 0", "set1 d.0", "bbs d.0, r", "or a, d", "or a, !a", "or a, (x)", "or a, [d+x]", // 0x00
   "or a, #i", "or dd, ds", "or1 c, m.b", "asl d", "asl !a", "push psw", "tset1 !a", "brk", // 0x08

   "bpl r", "tcall 1", "clr1 d.0", "bbc d.0, r", "or a, d+x", "or a, !a+x", "or a, !a+y", "or a, [d]+y", // 0x10
   "or d, #i", "or (x), (y)", "decw d", "asl d+x", "asl a", "dec x", "cmp x, !a", "jmp [!a+x]", // 0x18

   "clrp", "tcall 2", "set1 d.1", "bbs d.1, r", "and a, d", "and a, !a", "and a, (x)", "and a, [d+x]", // 0x20
   "and a, #i", "and dd, ds", "or1 c, /m.b", "rol d", "rol !a", "push a", "cbne d, r", "bra r", // 0x28

   "bmi r", "tcall 3", "clr1 d.1", "bbc d.1, r", "and a, d+x", "and a, !a+x", "and a, !a+y", "and a, [d]+y", // 0x30
   "and d, #i", "and (x), (y)", "incw d", "rol d+x", "rol a", "inc x", "cmp x, d", "call !a", // 0x38

   "setp", "tcall 4", "set1 d.2", "bbs d.2, r", "eor a, d", "eor a, !a", "eor a, (x)", "eor a, [d+x]", // 0x40
   "eor a, #i", "eor dd, ds", "and1 c, m.b", "lsr d", "lsr !a", "push x", "tclr1 !a", "pcall u", // 0x48

   "bvc r", "tcall 5", "clr1 d.2", "bbc d.2, r", "eor a, d+x", "eor a, !a+x", "eor a, !a+y", "eor a, [d]+y", // 0x50
   "eor d, #i", "eor (x), (y)", "cmpw ya, d", "lsr d+x", "lsr a", "mov x, a", "cmp y, !a", "jmp !a", // 0x58

   "clrc", "tcall 6", "set1 d.3", "bbs d.3, r", "cmp a, d", "cmp a, !a", "cmp a, (x)", "cmp a, [d+x]", // 0x60
   "cmp a, #i", "cmp dd, ds", "and1 c, /m.b", "ror d", "ror !a", "push y", "dbnz d, r", "ret", // 0x6F

   "bvs r", "tcall 7", "clr1 d.3", "bbc d.3, r", "cmp a, d+x", "cmp a, !a+x", "cmp a, !a+y", "cmp a, [d]+y", // 0x70
   "cmp d, #i", "cmp (x), (y)", "addw ya, d", "ror d+x", "ror a", "mov a, x", "cmp y, d", "ret1", // 0x77

   "setc", "tcall 8", "set1 d.4", "bbs d.4, r", "adc a, d", "adc a, !a", "adc a, (x)", "adc a, [d+x]", // 0x80
   "adc a, #i", "adc dd, ds", "eor1 c, m.b", "dec d", "dec !a", "mov y, #i", "pop psw", "mov d, #i", // 0x88

   "bcc r", "tcall 9", "clr1 d.4", "bbc d.4, r", "adc a, d+x", "adc a, !a+x", "adc a, !a+y", "adc a, [d]+y", // 0x90
   "adc d, #i", "adc (x), (y)", "subw ya, d", "dec d+x", "dec a", "mov x, sp", "div ya, x", "xcn a", // 0x98

   "ei", "tcall 10", "set1 d.5", "bbs d.5, r", "sbc a, d", "sbc a, !a", "sbc a, (x)", "sbc a, [d+x]", // 0xa0
   "sbc a, #i", "sbc dd, ds", "mov1 c, m.b", "inc d", "inc !a", "cmp y, #i", "pop a", "mov (x)+, a", // 0xa8

   "bcs r", "tcall 11", "clr1 d.5", "bbc d.5, r", "sbc a, d+x", "sbc a, !a+x", "sbc a, !a+y", "sbc a, [d]+y", // 0xb0
   "sbc d, #i", "sbc (x), (y)", "movw ya, d", "inc d+x", "inc a", "mov sp, x", "das a", "mov a, (x)+", // 0xb8

   "di", "tcall 12", "set1 d.6", "bbs d.6, r", "mov d, a", "mov !a, a", "mov (x), a", "mov [d+x], a", // 0xc0
   "cmp x, #i", "mov !a, x", "mov1 m.b, c", "mov d, y", "mov !a, y", "mov x, #i", "pop x", "mul ya", // 0xc8

   "bne r", "tcall 13", "clr1 d.6", "bbc d.6, r", "mov d+x, a", "mov !a+x, a", "mov !a+y, a", "mov [d]+y, a", // 0xd0
   "mov d, x", "mov d+y, x", "movw d, ya", "mov d+x, y", "dec y", "mov a, y", "cbne d+x, r", "daa a", // 0xd8

   "clrv", "tcall 14", "set1 d.7", "bbs d.7, r", "mov a, d", "mov a, !a", "mov a, (x)", "mov a, [d+x]", // 0xe0
   "mov a, #i", "mov x, !a", "not1 m.b", "mov y, d", "mov y, !a", "notc", "pop y", "sleep", // 0xe8

   "beq r", "tcall 15", "clr1 d.7", "bbc d.7, r", "mov a, d+x", "mov a, !a+x", "mov a, !a+y", "mov a, [d]+y", // 0xf0
   "mov x, d", "mov x, d+y", "mov dd, ds", "mov y, d+x", "inc y", "mov y, a", "dbnz y, r", "stop" // 0xf8
};
