#include "table.h"
#include "op.h"
#include <stddef.h>

#define ALUOP(op, alu) CPU_OP_READ_##op##_DECL(alu)
#define RMWOP(mode, op) CPU_OP_RMW_##mode##_DECL(op)
#define STOREOP(op, reg) CPU_OP_STORE_##op##_DECL(reg)
#define NOP cpu_op_nop

const cpu_op_t op_table_EMUL[256] = {
   CPU_OP_INTERRUPT_N_DECL(brk),    // 00 
   ALUOP(IDPX_B, ora),              // 01
   NOP,                             // 02 ?!?!
   ALUOP(SR_B, ora),                // 03
   RMWOP(DP_B, tsb),                // 04
   ALUOP(DP_B, ora),                // 05
   RMWOP(DP_B, asl),                // 06
   ALUOP(ILDP_B, ora),              // 07
   cpu_op_php,                      // 08
   ALUOP(CONST_B, ora),             // 09
   CPU_OP_RMW_REG_B_DECL(a, asl),   // 0a
   cpu_op_phd,                      // 0b
   RMWOP(ADDR_B, tsb),              // 0c
   ALUOP(ADDR_B, ora),              // 0d
   RMWOP(ADDR_B, asl),              // 0e
   ALUOP(LONG_B, ora),              // 0f
   CPU_OP_BRANCH_REG_N_DECL(n),     // 10
   ALUOP(IDPY_B, ora),              // 11
   ALUOP(IDP_B, ora),               // 12
   ALUOP(ISRY_B, ora),              // 13
   RMWOP(DP_B, trb),                // 14
   CPU_OP_READ_DPR_B_DECL(ora, x),  // 15
   RMWOP(DPX_B, asl),               // 16
   ALUOP(IDPY_B, ora),              // 17
   CPU_OP_RESET_FLAG_DECL(c),       // 18
   ALUOP(ADDRY_B, ora),             // 19
   CPU_OP_RMW_REG_B_DECL(a, inc),   // 1a
   cpu_op_tcs_e,                    // 1b
   RMWOP(ADDR_B, trb),              // 1c
   ALUOP(ADDRX_B, ora),             // 1d
   RMWOP(ADDRX_B, asl),             // 1e
   ALUOP(LONGX_B, ora),             // 1f
   cpu_op_jsr_addr,                 // 20
   ALUOP(IDPX_B, and),              // 21
   cpu_op_jsr_long_e,               // 22
   ALUOP(SR_B, and),                // 23
   ALUOP(DP_B, bit),                // 24
   ALUOP(DP_B, and),                // 25
   RMWOP(DP_B, rol),                // 26
   ALUOP(ILDP_B, and),              // 27
   cpu_op_plp_e,                    // 28
   ALUOP(CONST_B, and),             // 29
   CPU_OP_RMW_REG_B_DECL(a, rol),   // 2a
   cpu_op_pld,                      // 2b
   ALUOP(ADDR_B, bit),              // 2c
   ALUOP(ADDR_B, and),              // 2d
   RMWOP(ADDR_B, rol),              // 2e
   ALUOP(LONG_B, and),              // 2f
   CPU_OP_BRANCH_REG_DECL(n),       // 30
   ALUOP(IDPY_B, and),              // 31
   ALUOP(IDP_B, and),               // 32
   ALUOP(ISRY_B, and),              // 33
   CPU_OP_READ_DPR_B_DECL(bit, x),  // 34
   CPU_OP_READ_DPR_B_DECL(and, x),  // 35
   RMWOP(DPX_B, rol),               // 36
   ALUOP(ILDPY_B, and),             // 37
   CPU_OP_SET_FLAG_DECL(c),         // 38
   ALUOP(ADDRY_B, and),             // 39
   CPU_OP_RMW_REG_B_DECL(a, dec),   // 3a
   cpu_op_tsc,                      // 3b
   ALUOP(ADDRX_B, bit),             // 3c
   ALUOP(ADDRX_B, and),             // 3d
   RMWOP(ADDRX_B, rol),             // 3e
   ALUOP(LONGX_B, and),             // 3f
   cpu_op_rti_e,                    // 40
   ALUOP(IDPX_B, eor),              // 41
   cpu_op_wdm,                      // 42
   ALUOP(SR_B, eor),                // 43
   cpu_op_mvp,                      // 44
   ALUOP(DP_B, eor),                // 45
   RMWOP(DP_B, lsr),                // 46
   ALUOP(ILDP_B, eor),              // 47
   cpu_op_pha_b,                    // 48
   ALUOP(CONST_B, eor),             // 49
   CPU_OP_RMW_REG_B_DECL(a, lsr),   // 4a
   cpu_op_phk,                      // 4b
   cpu_op_jmp_addr,                 // 4c
   ALUOP(CONST_B, eor),             // 4d
   RMWOP(ADDR_B, lsr),              // 4e
   ALUOP(LONG_B, eor),              // 4f
   CPU_OP_BRANCH_REG_N_DECL(v),     // 50
   ALUOP(IDPY_B, eor),              // 51
   ALUOP(IDP_B, eor),               // 52
   ALUOP(ISRY_B, eor),              // 53
   cpu_op_mvn,                      // 54
   CPU_OP_READ_DPR_B_DECL(eor, x),  // 55
   RMWOP(DPX_B, lsr),               // 56
   ALUOP(ILDPY_B, eor),             // 57
   CPU_OP_RESET_FLAG_DECL(i),       // 58
   ALUOP(ADDRY_B, eor),             // 59
   cpu_op_phy_b,                    // 5a
   cpu_op_tcd,                      // 5b
   cpu_op_jmp_long,                 // 5c
   ALUOP(ADDRX_B, eor),             // 5d
   RMWOP(ADDRX_B, lsr),             // 5e
   ALUOP(LONGX_B, eor),             // 5f
   cpu_op_rts,                      // 60
   ALUOP(IDPX_B, adc),              // 61
   cpu_op_per_e,                    // 62
   ALUOP(SR_B, adc),                // 63
   STOREOP(DP_B, zero),             // 64
   ALUOP(DP_B, adc),                // 65
   RMWOP(DP_B, ror),                // 66
   ALUOP(ILDP_B, adc),              // 67
   cpu_op_pla_b,                    // 68
   ALUOP(CONST_B, adc),             // 69
   CPU_OP_RMW_REG_B_DECL(a, ror),   // 6a
   cpu_op_rtl_e,                    // 6b
   cpu_op_jmp_iaddr,                // 6c
   ALUOP(ADDR_B, adc),              // 6d
   RMWOP(ADDR_B, ror),              // 6e 
   ALUOP(LONG_B, adc),              // 6f
   CPU_OP_BRANCH_REG_DECL(v),       // 70
   ALUOP(IDPY_B, adc),              // 71
   ALUOP(IDP_B, adc),               // 72
   ALUOP(ISRY_B, adc),              // 73
   CPU_OP_STORE_DPR_B_DECL(zero, x),// 74
   CPU_OP_READ_DPR_B_DECL(adc, x),  // 75
   RMWOP(DPX_B, ror),               // 76
   ALUOP(ILDPY_B, adc),             // 77
   CPU_OP_SET_FLAG_DECL(i),         // 78
   ALUOP(ADDRY_B, adc),             // 79
   cpu_op_ply_b,                    // 7a
   cpu_op_tdc,                      // 7b
   cpu_op_jmp_iaddrx,               // 7c
   ALUOP(ADDRX_B, adc),             // 7d
   RMWOP(ADDRX_B, ror),             // 7e
   ALUOP(LONGX_B, adc),             // 7f
   cpu_op_bra,                      // 80
   cpu_op_sta_idpx_b,               // 81
   cpu_op_brl,                      // 82
   cpu_op_sta_sr_b,                 // 83
   STOREOP(DP_B, y),                // 84
   STOREOP(DP_B, a),                // 85
   STOREOP(DP_B, x),                // 86
   cpu_op_sta_ildp_b,               // 87
   CPU_OP_RMW_REG_B_DECL(y, dec),   // 88
   ALUOP(CONST_B, bit),             // 89
   cpu_op_txa_MX,                   // 8a
   cpu_op_phb,                      // 8b
   STOREOP(ADDR_B, y),              // 8c
   STOREOP(ADDR_B, a),              // 8d
   STOREOP(ADDR_B, x),              // 8e
   cpu_op_sta_long_b,               // 8f
   CPU_OP_BRANCH_REG_N_DECL(c),     // 90
   cpu_op_sta_idpy_b,               // 91
   cpu_op_sta_idp_b,                // 92
   cpu_op_sta_isry_b,               // 93
   CPU_OP_STORE_DPR_B_DECL(y, x),   // 94
   CPU_OP_STORE_DPR_B_DECL(a, x),   // 95
   CPU_OP_STORE_DPR_B_DECL(x, y),   // 96
   cpu_op_sta_ildpy_b,              // 97
   cpu_op_tya_MX,                   // 98
   CPU_OP_STORE_ADDR_REGI_B_DECL(a, y),  // 99
   cpu_op_txs_e,                    // 9a
   cpu_op_txy_b,                    // 9b
   STOREOP(ADDR_B, zero),           // 9c
   CPU_OP_STORE_ADDR_REGI_B_DECL(a, x),  // 9d
   CPU_OP_STORE_ADDR_REGI_B_DECL(zero, x),  // 9e
   cpu_op_sta_longx_b,              // 9f
   ALUOP(CONST_B, ldy),             // a0
   ALUOP(IDPX_B, lda),              // a1
   ALUOP(CONST_B, ldx),             // a2
   ALUOP(SR_B, lda),                // a3
   ALUOP(DP_B, ldy),                // a4
   ALUOP(DP_B, lda),                // a5
   ALUOP(DP_B, ldx),                // a6
   ALUOP(ILDP_B, lda),              // a7
   cpu_op_tay_MX,                   // a8
   ALUOP(CONST_B, lda),             // a9
   cpu_op_tax_MX,                   // aa
   cpu_op_plb,                      // ab
   ALUOP(ADDR_B, ldy),              // ac
   ALUOP(ADDR_B, lda),              // ad
   ALUOP(ADDR_B, ldx),              // ae
   ALUOP(LONG_B, lda),              // af
   CPU_OP_BRANCH_REG_DECL(c),       // b0
   ALUOP(IDPY_B, lda),              // b1
   ALUOP(IDP_B, lda),               // b2
   ALUOP(ISRY_B, lda),              // b3
   CPU_OP_READ_DPR_B_DECL(ldy, x),  // b4
   CPU_OP_READ_DPR_B_DECL(lda, x),  // b5
   CPU_OP_READ_DPR_B_DECL(ldx, y),  // b6
   ALUOP(ILDPY_B, lda),             // b7
   CPU_OP_RESET_FLAG_DECL(v),       // b8
   ALUOP(ADDRY_B, lda),             // b9
   cpu_op_tsx_b,                    // ba
   cpu_op_tyx_b,                    // bb
   ALUOP(ADDRX_B, ldy),             // bc
   ALUOP(ADDRX_B, lda),             // bd
   ALUOP(ADDRX_B, ldx),             // be
   ALUOP(LONGX_B, lda),             // bf
   ALUOP(CONST_B, cpy),             // c0
   ALUOP(IDPX_B, cmp),              // c1
   cpu_op_rep,                      // c2
   ALUOP(SR_B, cmp),                // c3
   ALUOP(DP_B, cpy),                // c4
   ALUOP(DP_B, cmp),                // c5
   RMWOP(DP_B, dec),                // c6
   ALUOP(ILDP_B, cmp),              // c7
   CPU_OP_RMW_REG_B_DECL(y, inc),   // c8
   ALUOP(CONST_B, cmp),             // c9
   CPU_OP_RMW_REG_B_DECL(x, dec),   // ca
   cpu_op_wai,                      // cb
   ALUOP(ADDR_B, cpy),              // cc
   ALUOP(ADDR_B, cmp),              // cd
   RMWOP(ADDR_B, dec),              // ce
   ALUOP(LONG_B, cmp),              // cf
   CPU_OP_BRANCH_REG_N_DECL(z),     // d0
   ALUOP(IDPY_B, cmp),              // d1
   ALUOP(IDP_B, cmp),               // d2
   ALUOP(ISRY_B, cmp),              // d3
   cpu_op_pei_e,                    // d4
   CPU_OP_READ_DPR_B_DECL(cmp, x),  // d5
   RMWOP(DPX_B, dec),               // d6
   ALUOP(ILDPY_B, cmp),             // d7
   CPU_OP_RESET_FLAG_DECL(d),       // d8
   ALUOP(ADDRY_B, cmp),             // d9
   cpu_op_phx_b,                    // da
   cpu_op_stp,                      // db
   cpu_op_jmp_iladdr,               // dc
   ALUOP(ADDRX_B, cmp),             // dd
   RMWOP(ADDRX_B, dec),             // de
   ALUOP(LONGX_B, cmp),             // df
   ALUOP(CONST_B, cpx),             // e0
   ALUOP(IDPX_B, sbc),              // e1
   cpu_op_sep,                      // e2
   ALUOP(SR_B, sbc),                // e3
   ALUOP(DP_B, cpx),                // e4
   ALUOP(DP_B, sbc),                // e5
   RMWOP(DP_B, inc),                // e6
   ALUOP(ILDP_B, sbc),              // e7
   CPU_OP_RMW_REG_B_DECL(x, inc),   // e8
   ALUOP(CONST_B, sbc),             // e9
   NOP,                             // ea
   cpu_op_xba,                      // eb
   ALUOP(ADDR_B, cpx),              // ec
   ALUOP(ADDR_B, sbc),              // ed
   RMWOP(ADDR_B, inc),              // ee
   ALUOP(LONG_B, sbc),              // ef
   CPU_OP_BRANCH_REG_DECL(z),       // f0
   ALUOP(IDPY_B, sbc),              // f1
   ALUOP(IDP_B, sbc),               // f2
   ALUOP(ISRY_B, sbc),              // f3
   cpu_op_pea,                      // f4
   CPU_OP_READ_DPR_B_DECL(sbc, x),  // f5
   RMWOP(DPX_B, inc),               // f6
   ALUOP(ILDPY_B, sbc),             // f7
   CPU_OP_SET_FLAG_DECL(d),         // f8
   ALUOP(ADDRY_B, sbc),             // f9
   cpu_op_plx_b,                    // fa
   cpu_op_xce,                      // fb
   cpu_op_jsr_iaddrx_e,             // fc
   ALUOP(ADDRX_B, sbc),             // fd
   RMWOP(ADDRX_B, inc),             // fe
   ALUOP(LONGX_B, sbc)              // ff
};

const cpu_op_t op_table_MX[256] = {
   CPU_OP_INTERRUPT_N_DECL(brk),    // 00 
   ALUOP(IDPX_B, ora),              // 01
   NOP,                             // 02 ?!?!
   ALUOP(SR_B, ora),                // 03
   RMWOP(DP_B, tsb),                // 04
   ALUOP(DP_B, ora),                // 05
   RMWOP(DP_B, asl),                // 06
   ALUOP(ILDP_B, ora),              // 07
   cpu_op_php,                      // 08
   ALUOP(CONST_B, ora),             // 09
   CPU_OP_RMW_REG_B_DECL(a, asl),   // 0a
   cpu_op_phd,                      // 0b
   RMWOP(ADDR_B, tsb),              // 0c
   ALUOP(ADDR_B, ora),              // 0d
   RMWOP(ADDR_B, asl),              // 0e
   ALUOP(LONG_B, ora),              // 0f
   CPU_OP_BRANCH_REG_N_DECL(n),     // 10
   ALUOP(IDPY_B, ora),              // 11
   ALUOP(IDP_B, ora),               // 12
   ALUOP(ISRY_B, ora),              // 13
   RMWOP(DP_B, trb),                // 14
   CPU_OP_READ_DPR_B_DECL(ora, x),  // 15
   RMWOP(DPX_B, asl),               // 16
   ALUOP(IDPY_B, ora),              // 17
   CPU_OP_RESET_FLAG_DECL(c),       // 18
   ALUOP(ADDRY_B, ora),             // 19
   CPU_OP_RMW_REG_B_DECL(a, inc),   // 1a
   cpu_op_tcs_n,                    // 1b
   RMWOP(ADDR_B, trb),              // 1c
   ALUOP(ADDRX_B, ora),             // 1d
   RMWOP(ADDRX_B, asl),             // 1e
   ALUOP(LONGX_B, ora),             // 1f
   cpu_op_jsr_addr,                 // 20
   ALUOP(IDPX_B, and),              // 21
   cpu_op_jsr_long_n,               // 22
   ALUOP(SR_B, and),                // 23
   ALUOP(DP_B, bit),                // 24
   ALUOP(DP_B, and),                // 25
   RMWOP(DP_B, rol),                // 26
   ALUOP(ILDP_B, and),              // 27
   cpu_op_plp_n,                    // 28
   ALUOP(CONST_B, and),             // 29
   CPU_OP_RMW_REG_B_DECL(a, rol),   // 2a
   cpu_op_pld,                      // 2b
   ALUOP(ADDR_B, bit),              // 2c
   ALUOP(ADDR_B, and),              // 2d
   RMWOP(ADDR_B, rol),              // 2e
   ALUOP(LONG_B, and),              // 2f
   CPU_OP_BRANCH_REG_DECL(n),       // 30
   ALUOP(IDPY_B, and),              // 31
   ALUOP(IDP_B, and),               // 32
   ALUOP(ISRY_B, and),              // 33
   CPU_OP_READ_DPR_B_DECL(bit, x),  // 34
   CPU_OP_READ_DPR_B_DECL(and, x),  // 35
   RMWOP(DPX_B, rol),               // 36
   ALUOP(ILDPY_B, and),             // 37
   CPU_OP_SET_FLAG_DECL(c),         // 38
   ALUOP(ADDRY_B, and),             // 39
   CPU_OP_RMW_REG_B_DECL(a, dec),   // 3a
   cpu_op_tsc,                      // 3b
   ALUOP(ADDRX_B, bit),             // 3c
   ALUOP(ADDRX_B, and),             // 3d
   RMWOP(ADDRX_B, rol),             // 3e
   ALUOP(LONGX_B, and),             // 3f
   cpu_op_rti_n,                    // 40
   ALUOP(IDPX_B, eor),              // 41
   cpu_op_wdm,                      // 42
   ALUOP(SR_B, eor),                // 43
   cpu_op_mvp,                      // 44
   ALUOP(DP_B, eor),                // 45
   RMWOP(DP_B, lsr),                // 46
   ALUOP(ILDP_B, eor),              // 47
   cpu_op_pha_b,                    // 48
   ALUOP(CONST_B, eor),             // 49
   CPU_OP_RMW_REG_B_DECL(a, lsr),   // 4a
   cpu_op_phk,                      // 4b
   cpu_op_jmp_addr,                 // 4c
   ALUOP(CONST_B, eor),             // 4d
   RMWOP(ADDR_B, lsr),              // 4e
   ALUOP(LONG_B, eor),              // 4f
   CPU_OP_BRANCH_REG_N_DECL(v),     // 50
   ALUOP(IDPY_B, eor),              // 51
   ALUOP(IDP_B, eor),               // 52
   ALUOP(ISRY_B, eor),              // 53
   cpu_op_mvn,                      // 54
   CPU_OP_READ_DPR_B_DECL(eor, x),  // 55
   RMWOP(DPX_B, lsr),               // 56
   ALUOP(ILDPY_B, eor),             // 57
   CPU_OP_RESET_FLAG_DECL(i),       // 58
   ALUOP(ADDRY_B, eor),             // 59
   cpu_op_phy_b,                    // 5a
   cpu_op_tcd,                      // 5b
   cpu_op_jmp_long,                 // 5c
   ALUOP(ADDRX_B, eor),             // 5d
   RMWOP(ADDRX_B, lsr),             // 5e
   ALUOP(LONGX_B, eor),             // 5f
   cpu_op_rts,                      // 60
   ALUOP(IDPX_B, adc),              // 61
   cpu_op_per_n,                    // 62
   ALUOP(SR_B, adc),                // 63
   STOREOP(DP_B, zero),             // 64
   ALUOP(DP_B, adc),                // 65
   RMWOP(DP_B, ror),                // 66
   ALUOP(ILDP_B, adc),              // 67
   cpu_op_pla_b,                    // 68
   ALUOP(CONST_B, adc),             // 69
   CPU_OP_RMW_REG_B_DECL(a, ror),   // 6a
   cpu_op_rtl_n,                    // 6b
   cpu_op_jmp_iaddr,                // 6c
   ALUOP(ADDR_B, adc),              // 6d
   RMWOP(ADDR_B, ror),              // 6e 
   ALUOP(LONG_B, adc),              // 6f
   CPU_OP_BRANCH_REG_DECL(v),       // 70
   ALUOP(IDPY_B, adc),              // 71
   ALUOP(IDP_B, adc),               // 72
   ALUOP(ISRY_B, adc),              // 73
   CPU_OP_STORE_DPR_B_DECL(zero, x),// 74
   CPU_OP_READ_DPR_B_DECL(adc, x),  // 75
   RMWOP(DPX_B, ror),               // 76
   ALUOP(ILDPY_B, adc),             // 77
   CPU_OP_SET_FLAG_DECL(i),         // 78
   ALUOP(ADDRY_B, adc),             // 79
   cpu_op_ply_b,                    // 7a
   cpu_op_tdc,                      // 7b
   cpu_op_jmp_iaddrx,               // 7c
   ALUOP(ADDRX_B, adc),             // 7d
   RMWOP(ADDRX_B, ror),             // 7e
   ALUOP(LONGX_B, adc),             // 7f
   cpu_op_bra,                      // 80
   cpu_op_sta_idpx_b,               // 81
   cpu_op_brl,                      // 82
   cpu_op_sta_sr_b,                 // 83
   STOREOP(DP_B, y),                // 84
   STOREOP(DP_B, a),                // 85
   STOREOP(DP_B, x),                // 86
   cpu_op_sta_ildp_b,               // 87
   CPU_OP_RMW_REG_B_DECL(y, dec),   // 88
   ALUOP(CONST_B, bit),             // 89
   cpu_op_txa_MX,                   // 8a
   cpu_op_phb,                      // 8b
   STOREOP(ADDR_B, y),              // 8c
   STOREOP(ADDR_B, a),              // 8d
   STOREOP(ADDR_B, x),              // 8e
   cpu_op_sta_long_b,               // 8f
   CPU_OP_BRANCH_REG_N_DECL(c),     // 90
   cpu_op_sta_idpy_b,               // 91
   cpu_op_sta_idp_b,                // 92
   cpu_op_sta_isry_b,               // 93
   CPU_OP_STORE_DPR_B_DECL(y, x),   // 94
   CPU_OP_STORE_DPR_B_DECL(a, x),   // 95
   CPU_OP_STORE_DPR_B_DECL(x, y),   // 96
   cpu_op_sta_ildpy_b,              // 97
   cpu_op_tya_MX,                   // 98
   CPU_OP_STORE_ADDR_REGI_B_DECL(a, y),  // 99
   cpu_op_txs_n,                    // 9a
   cpu_op_txy_b,                    // 9b
   STOREOP(ADDR_B, zero),           // 9c
   CPU_OP_STORE_ADDR_REGI_B_DECL(a, x),  // 9d
   CPU_OP_STORE_ADDR_REGI_B_DECL(zero, x),  // 9e
   cpu_op_sta_longx_b,              // 9f
   ALUOP(CONST_B, ldy),             // a0
   ALUOP(IDPX_B, lda),              // a1
   ALUOP(CONST_B, ldx),             // a2
   ALUOP(SR_B, lda),                // a3
   ALUOP(DP_B, ldy),                // a4
   ALUOP(DP_B, lda),                // a5
   ALUOP(DP_B, ldx),                // a6
   ALUOP(ILDP_B, lda),              // a7
   cpu_op_tay_MX,                   // a8
   ALUOP(CONST_B, lda),             // a9
   cpu_op_tax_MX,                   // aa
   cpu_op_plb,                      // ab
   ALUOP(ADDR_B, ldy),              // ac
   ALUOP(ADDR_B, lda),              // ad
   ALUOP(ADDR_B, ldx),              // ae
   ALUOP(LONG_B, lda),              // af
   CPU_OP_BRANCH_REG_DECL(c),       // b0
   ALUOP(IDPY_B, lda),              // b1
   ALUOP(IDP_B, lda),               // b2
   ALUOP(ISRY_B, lda),              // b3
   CPU_OP_READ_DPR_B_DECL(ldy, x),  // b4
   CPU_OP_READ_DPR_B_DECL(lda, x),  // b5
   CPU_OP_READ_DPR_B_DECL(ldx, y),  // b6
   ALUOP(ILDPY_B, lda),             // b7
   CPU_OP_RESET_FLAG_DECL(v),       // b8
   ALUOP(ADDRY_B, lda),             // b9
   cpu_op_tsx_b,                    // ba
   cpu_op_tyx_b,                    // bb
   ALUOP(ADDRX_B, ldy),             // bc
   ALUOP(ADDRX_B, lda),             // bd
   ALUOP(ADDRX_B, ldx),             // be
   ALUOP(LONGX_B, lda),             // bf
   ALUOP(CONST_B, cpy),             // c0
   ALUOP(IDPX_B, cmp),              // c1
   cpu_op_rep,                      // c2
   ALUOP(SR_B, cmp),                // c3
   ALUOP(DP_B, cpy),                // c4
   ALUOP(DP_B, cmp),                // c5
   RMWOP(DP_B, dec),                // c6
   ALUOP(ILDP_B, cmp),              // c7
   CPU_OP_RMW_REG_B_DECL(y, inc),   // c8
   ALUOP(CONST_B, cmp),             // c9
   CPU_OP_RMW_REG_B_DECL(x, dec),   // ca
   cpu_op_wai,                      // cb
   ALUOP(ADDR_B, cpy),              // cc
   ALUOP(ADDR_B, cmp),              // cd
   RMWOP(ADDR_B, dec),              // ce
   ALUOP(LONG_B, cmp),              // cf
   CPU_OP_BRANCH_REG_N_DECL(z),     // d0
   ALUOP(IDPY_B, cmp),              // d1
   ALUOP(IDP_B, cmp),               // d2
   ALUOP(ISRY_B, cmp),              // d3
   cpu_op_pei_n,                    // d4
   CPU_OP_READ_DPR_B_DECL(cmp, x),  // d5
   RMWOP(DPX_B, dec),               // d6
   ALUOP(ILDPY_B, cmp),             // d7
   CPU_OP_RESET_FLAG_DECL(d),       // d8
   ALUOP(ADDRY_B, cmp),             // d9
   cpu_op_phx_b,                    // da
   cpu_op_stp,                      // db
   cpu_op_jmp_iladdr,               // dc
   ALUOP(ADDRX_B, cmp),             // dd
   RMWOP(ADDRX_B, dec),             // de
   ALUOP(LONGX_B, cmp),             // df
   ALUOP(CONST_B, cpx),             // e0
   ALUOP(IDPX_B, sbc),              // e1
   cpu_op_sep,                      // e2
   ALUOP(SR_B, sbc),                // e3
   ALUOP(DP_B, cpx),                // e4
   ALUOP(DP_B, sbc),                // e5
   RMWOP(DP_B, inc),                // e6
   ALUOP(ILDP_B, sbc),              // e7
   CPU_OP_RMW_REG_B_DECL(x, inc),   // e8
   ALUOP(CONST_B, sbc),             // e9
   NOP,                             // ea
   cpu_op_xba,                      // eb
   ALUOP(ADDR_B, cpx),              // ec
   ALUOP(ADDR_B, sbc),              // ed
   RMWOP(ADDR_B, inc),              // ee
   ALUOP(LONG_B, sbc),              // ef
   CPU_OP_BRANCH_REG_DECL(z),       // f0
   ALUOP(IDPY_B, sbc),              // f1
   ALUOP(IDP_B, sbc),               // f2
   ALUOP(ISRY_B, sbc),              // f3
   cpu_op_pea,                      // f4
   CPU_OP_READ_DPR_B_DECL(sbc, x),  // f5
   RMWOP(DPX_B, inc),               // f6
   ALUOP(ILDPY_B, sbc),             // f7
   CPU_OP_SET_FLAG_DECL(d),         // f8
   ALUOP(ADDRY_B, sbc),             // f9
   cpu_op_plx_b,                    // fa
   cpu_op_xce,                      // fb
   cpu_op_jsr_iaddrx_n,             // fc
   ALUOP(ADDRX_B, sbc),             // fd
   RMWOP(ADDRX_B, inc),             // fe
   ALUOP(LONGX_B, sbc)              // ff
};

const cpu_op_t op_table_mx[256] = {
   CPU_OP_INTERRUPT_N_DECL(brk),    // 00 
   ALUOP(IDPX_W, ora),              // 01
   NOP,                             // 02 ?!?!
   ALUOP(SR_W, ora),                // 03
   RMWOP(DP_W, tsb),                // 04
   ALUOP(DP_W, ora),                // 05
   RMWOP(DP_W, asl),                // 06
   ALUOP(ILDP_W, ora),              // 07
   cpu_op_php,                      // 08
   ALUOP(CONST_W, ora),             // 09
   CPU_OP_RMW_REG_B_DECL(a, asl),   // 0a
   cpu_op_phd,                      // 0b
   RMWOP(ADDR_W, tsb),              // 0c
   ALUOP(ADDR_W, ora),              // 0d
   RMWOP(ADDR_W, asl),              // 0e
   ALUOP(LONG_W, ora),              // 0f
   CPU_OP_BRANCH_REG_N_DECL(n),     // 10
   ALUOP(IDPY_W, ora),              // 11
   ALUOP(IDP_W, ora),               // 12
   ALUOP(ISRY_W, ora),              // 13
   RMWOP(DP_B, trb),                // 14
   CPU_OP_READ_DPR_W_DECL(ora, x),  // 15
   RMWOP(DPX_W, asl),               // 16
   ALUOP(IDPY_W, ora),              // 17
   CPU_OP_RESET_FLAG_DECL(c),       // 18
   ALUOP(ADDRY_W, ora),             // 19
   CPU_OP_RMW_REG_W_DECL(a, inc),   // 1a
   cpu_op_tcs_n,                    // 1b
   RMWOP(ADDR_W, trb),              // 1c
   ALUOP(ADDRX_W, ora),             // 1d
   RMWOP(ADDRX_W, asl),             // 1e
   ALUOP(LONGX_W, ora),             // 1f
   cpu_op_jsr_addr,                 // 20
   ALUOP(IDPX_W, and),              // 21
   cpu_op_jsr_long_n,               // 22
   ALUOP(SR_W, and),                // 23
   ALUOP(DP_W, bit),                // 24
   ALUOP(DP_W, and),                // 25
   RMWOP(DP_W, rol),                // 26
   ALUOP(ILDP_W, and),              // 27
   cpu_op_plp_n,                    // 28
   ALUOP(CONST_B, and),             // 29
   CPU_OP_RMW_REG_B_DECL(a, rol),   // 2a
   cpu_op_pld,                      // 2b
   ALUOP(ADDR_W, bit),              // 2c
   ALUOP(ADDR_W, and),              // 2d
   RMWOP(ADDR_W, rol),              // 2e
   ALUOP(LONG_W, and),              // 2f
   CPU_OP_BRANCH_REG_DECL(n),       // 30
   ALUOP(IDPY_W, and),              // 31
   ALUOP(IDP_W, and),               // 32
   ALUOP(ISRY_W, and),              // 33
   CPU_OP_READ_DPR_W_DECL(bit, x),  // 34
   CPU_OP_READ_DPR_W_DECL(and, x),  // 35
   RMWOP(DPX_W, rol),               // 36
   ALUOP(ILDPY_W, and),             // 37
   CPU_OP_SET_FLAG_DECL(c),         // 38
   ALUOP(ADDRY_W, and),             // 39
   CPU_OP_RMW_REG_W_DECL(a, dec),   // 3a
   cpu_op_tsc,                      // 3b
   ALUOP(ADDRX_W, bit),             // 3c
   ALUOP(ADDRX_W, and),             // 3d
   RMWOP(ADDRX_W, rol),             // 3e
   ALUOP(LONGX_W, and),             // 3f
   cpu_op_rti_n,                    // 40
   ALUOP(IDPX_W, eor),              // 41
   cpu_op_wdm,                      // 42
   ALUOP(SR_B, eor),                // 43
   cpu_op_mvp,                      // 44
   ALUOP(DP_W, eor),                // 45
   RMWOP(DP_W, lsr),                // 46
   ALUOP(ILDP_W, eor),              // 47
   cpu_op_pha_w,                    // 48
   ALUOP(CONST_W, eor),             // 49
   CPU_OP_RMW_REG_B_DECL(a, lsr),   // 4a
   cpu_op_phk,                      // 4b
   cpu_op_jmp_addr,                 // 4c
   ALUOP(CONST_W, eor),             // 4d
   RMWOP(ADDR_W, lsr),              // 4e
   ALUOP(LONG_W, eor),              // 4f
   CPU_OP_BRANCH_REG_N_DECL(v),     // 50
   ALUOP(IDPY_W, eor),              // 51
   ALUOP(IDP_W, eor),               // 52
   ALUOP(ISRY_W, eor),              // 53
   cpu_op_mvn,                      // 54
   CPU_OP_READ_DPR_B_DECL(eor, x),  // 55
   RMWOP(DPX_W, lsr),               // 56
   ALUOP(ILDPY_W, eor),             // 57
   CPU_OP_RESET_FLAG_DECL(i),       // 58
   ALUOP(ADDRY_W, eor),             // 59
   cpu_op_phy_w,                    // 5a
   cpu_op_tcd,                      // 5b
   cpu_op_jmp_long,                 // 5c
   ALUOP(ADDRX_W, eor),             // 5d
   RMWOP(ADDRX_W, lsr),             // 5e
   ALUOP(LONGX_W, eor),             // 5f
   cpu_op_rts,                      // 60
   ALUOP(IDPX_W, adc),              // 61
   cpu_op_per_n,                    // 62
   ALUOP(SR_W, adc),                // 63
   STOREOP(DP_W, zero),             // 64
   ALUOP(DP_W, adc),                // 65
   RMWOP(DP_W, ror),                // 66
   ALUOP(ILDP_W, adc),              // 67
   cpu_op_pla_w,                    // 68
   ALUOP(CONST_W, adc),             // 69
   CPU_OP_RMW_REG_W_DECL(a, ror),   // 6a
   cpu_op_rtl_n,                    // 6b
   cpu_op_jmp_iaddr,                // 6c
   ALUOP(ADDR_W, adc),              // 6d
   RMWOP(ADDR_W, ror),              // 6e 
   ALUOP(LONG_W, adc),              // 6f
   CPU_OP_BRANCH_REG_DECL(v),       // 70
   ALUOP(IDPY_W, adc),              // 71
   ALUOP(IDP_W, adc),               // 72
   ALUOP(ISRY_W, adc),              // 73
   CPU_OP_STORE_DPR_W_DECL(zero, x),// 74
   CPU_OP_READ_DPR_W_DECL(adc, x),  // 75
   RMWOP(DPX_W, ror),               // 76
   ALUOP(ILDPY_W, adc),             // 77
   CPU_OP_SET_FLAG_DECL(i),         // 78
   ALUOP(ADDRY_W, adc),             // 79
   cpu_op_ply_w,                    // 7a
   cpu_op_tdc,                      // 7b
   cpu_op_jmp_iaddrx,               // 7c
   ALUOP(ADDRX_W, adc),             // 7d
   RMWOP(ADDRX_W, ror),             // 7e
   ALUOP(LONGX_W, adc),             // 7f
   cpu_op_bra,                      // 80
   cpu_op_sta_idpx_w,               // 81
   cpu_op_brl,                      // 82
   cpu_op_sta_sr_w,                 // 83
   STOREOP(DP_W, y),                // 84
   STOREOP(DP_W, a),                // 85
   STOREOP(DP_W, x),                // 86
   cpu_op_sta_ildp_w,               // 87
   CPU_OP_RMW_REG_W_DECL(y, dec),   // 88
   ALUOP(CONST_W, bit),             // 89
   cpu_op_txa_mx,                   // 8a
   cpu_op_phb,                      // 8b
   STOREOP(ADDR_W, y),              // 8c
   STOREOP(ADDR_W, a),              // 8d
   STOREOP(ADDR_W, x),              // 8e
   cpu_op_sta_long_w,               // 8f
   CPU_OP_BRANCH_REG_N_DECL(c),     // 90
   cpu_op_sta_idpy_w,               // 91
   cpu_op_sta_idp_w,                // 92
   cpu_op_sta_isry_w,               // 93
   CPU_OP_STORE_DPR_W_DECL(y, x),   // 94
   CPU_OP_STORE_DPR_W_DECL(a, x),   // 95
   CPU_OP_STORE_DPR_W_DECL(x, y),   // 96
   cpu_op_sta_ildpy_w,              // 97
   cpu_op_tya_mx,                   // 98
   CPU_OP_STORE_ADDR_REGI_W_DECL(a, y),  // 99
   cpu_op_txs_n,                    // 9a
   cpu_op_txy_w,                    // 9b
   STOREOP(ADDR_W, zero),           // 9c
   CPU_OP_STORE_ADDR_REGI_W_DECL(a, x),  // 9d
   CPU_OP_STORE_ADDR_REGI_W_DECL(zero, x),  // 9e
   cpu_op_sta_longx_w,              // 9f
   ALUOP(CONST_W, ldy),             // a0
   ALUOP(IDPX_W, lda),              // a1
   ALUOP(CONST_W, ldx),             // a2
   ALUOP(SR_W, lda),                // a3
   ALUOP(DP_W, ldy),                // a4
   ALUOP(DP_W, lda),                // a5
   ALUOP(DP_W, ldx),                // a6
   ALUOP(ILDP_W, lda),              // a7
   cpu_op_tay_mx,                   // a8
   ALUOP(CONST_W, lda),             // a9
   cpu_op_tax_mx,                   // aa
   cpu_op_plb,                      // ab
   ALUOP(ADDR_W, ldy),              // ac
   ALUOP(ADDR_W, lda),              // ad
   ALUOP(ADDR_W, ldx),              // ae
   ALUOP(LONG_W, lda),              // af
   CPU_OP_BRANCH_REG_DECL(c),       // b0
   ALUOP(IDPY_W, lda),              // b1
   ALUOP(IDP_W, lda),               // b2
   ALUOP(ISRY_W, lda),              // b3
   CPU_OP_READ_DPR_W_DECL(ldy, x),  // b4
   CPU_OP_READ_DPR_W_DECL(lda, x),  // b5
   CPU_OP_READ_DPR_W_DECL(ldx, y),  // b6
   ALUOP(ILDPY_W, lda),             // b7
   CPU_OP_RESET_FLAG_DECL(v),       // b8
   ALUOP(ADDRY_W, lda),             // b9
   cpu_op_tsx_w,                    // ba
   cpu_op_tyx_w,                    // bb
   ALUOP(ADDRX_W, ldy),             // bc
   ALUOP(ADDRX_W, lda),             // bd
   ALUOP(ADDRX_W, ldx),             // be
   ALUOP(LONGX_W, lda),             // bf
   ALUOP(CONST_W, cpy),             // c0
   ALUOP(IDPX_W, cmp),              // c1
   cpu_op_rep,                      // c2
   ALUOP(SR_W, cmp),                // c3
   ALUOP(DP_W, cpy),                // c4
   ALUOP(DP_W, cmp),                // c5
   RMWOP(DP_W, dec),                // c6
   ALUOP(ILDP_W, cmp),              // c7
   CPU_OP_RMW_REG_W_DECL(y, inc),   // c8
   ALUOP(CONST_W, cmp),             // c9
   CPU_OP_RMW_REG_W_DECL(x, dec),   // ca
   cpu_op_wai,                      // cb
   ALUOP(ADDR_W, cpy),              // cc
   ALUOP(ADDR_W, cmp),              // cd
   RMWOP(ADDR_W, dec),              // ce
   ALUOP(LONG_W, cmp),              // cf
   CPU_OP_BRANCH_REG_N_DECL(z),     // d0
   ALUOP(IDPY_W, cmp),              // d1
   ALUOP(IDP_W, cmp),               // d2
   ALUOP(ISRY_W, cmp),              // d3
   cpu_op_pei_n,                    // d4
   CPU_OP_READ_DPR_B_DECL(cmp, x),  // d5
   RMWOP(DPX_W, dec),               // d6
   ALUOP(ILDPY_W, cmp),             // d7
   CPU_OP_RESET_FLAG_DECL(d),       // d8
   ALUOP(ADDRY_W, cmp),             // d9
   cpu_op_phx_w,                    // da
   cpu_op_stp,                      // db
   cpu_op_jmp_iladdr,               // dc
   ALUOP(ADDRX_W, cmp),             // dd
   RMWOP(ADDRX_W, dec),             // de
   ALUOP(LONGX_W, cmp),             // df
   ALUOP(CONST_W, cpx),             // e0
   ALUOP(IDPX_W, sbc),              // e1
   cpu_op_sep,                      // e2
   ALUOP(SR_W, sbc),                // e3
   ALUOP(DP_W, cpx),                // e4
   ALUOP(DP_W, sbc),                // e5
   RMWOP(DP_W, inc),                // e6
   ALUOP(ILDP_W, sbc),              // e7
   CPU_OP_RMW_REG_W_DECL(x, inc),   // e8
   ALUOP(CONST_W, sbc),             // e9
   NOP,                             // ea
   cpu_op_xba,                      // eb
   ALUOP(ADDR_W, cpx),              // ec
   ALUOP(ADDR_W, sbc),              // ed
   RMWOP(ADDR_W, inc),              // ee
   ALUOP(LONG_W, sbc),              // ef
   CPU_OP_BRANCH_REG_DECL(z),       // f0
   ALUOP(IDPY_W, sbc),              // f1
   ALUOP(IDP_W, sbc),               // f2
   ALUOP(ISRY_W, sbc),              // f3
   cpu_op_pea,                      // f4
   CPU_OP_READ_DPR_B_DECL(sbc, x),  // f5
   RMWOP(DPX_W, inc),               // f6
   ALUOP(ILDPY_W, sbc),             // f7
   CPU_OP_SET_FLAG_DECL(d),         // f8
   ALUOP(ADDRY_W, sbc),             // f9
   cpu_op_plx_w,                    // fa
   cpu_op_xce,                      // fb
   cpu_op_jsr_iaddrx_n,             // fc
   ALUOP(ADDRX_W, sbc),             // fd
   RMWOP(ADDRX_W, inc),             // fe
   ALUOP(LONGX_W, sbc)              // ff
};

const cpu_op_t op_table_mX[256] = {
   CPU_OP_INTERRUPT_N_DECL(brk),    // 00 
   ALUOP(IDPX_W, ora),              // 01
   NOP,                             // 02 ?!?!
   ALUOP(SR_W, ora),                // 03
   RMWOP(DP_W, tsb),                // 04
   ALUOP(DP_W, ora),                // 05
   RMWOP(DP_W, asl),                // 06
   ALUOP(ILDP_W, ora),              // 07
   cpu_op_php,                      // 08
   ALUOP(CONST_W, ora),             // 09
   CPU_OP_RMW_REG_B_DECL(a, asl),   // 0a
   cpu_op_phd,                      // 0b
   RMWOP(ADDR_W, tsb),              // 0c
   ALUOP(ADDR_W, ora),              // 0d
   RMWOP(ADDR_W, asl),              // 0e
   ALUOP(LONG_W, ora),              // 0f
   CPU_OP_BRANCH_REG_N_DECL(n),     // 10
   ALUOP(IDPY_W, ora),              // 11
   ALUOP(IDP_W, ora),               // 12
   ALUOP(ISRY_W, ora),              // 13
   RMWOP(DP_B, trb),                // 14
   CPU_OP_READ_DPR_W_DECL(ora, x),  // 15
   RMWOP(DPX_W, asl),               // 16
   ALUOP(IDPY_W, ora),              // 17
   CPU_OP_RESET_FLAG_DECL(c),       // 18
   ALUOP(ADDRY_W, ora),             // 19
   CPU_OP_RMW_REG_W_DECL(a, inc),   // 1a
   cpu_op_tcs_n,                    // 1b
   RMWOP(ADDR_W, trb),              // 1c
   ALUOP(ADDRX_W, ora),             // 1d
   RMWOP(ADDRX_W, asl),             // 1e
   ALUOP(LONGX_W, ora),             // 1f
   cpu_op_jsr_addr,                 // 20
   ALUOP(IDPX_W, and),              // 21
   cpu_op_jsr_long_n,               // 22
   ALUOP(SR_W, and),                // 23
   ALUOP(DP_W, bit),                // 24
   ALUOP(DP_W, and),                // 25
   RMWOP(DP_W, rol),                // 26
   ALUOP(ILDP_W, and),              // 27
   cpu_op_plp_n,                    // 28
   ALUOP(CONST_B, and),             // 29
   CPU_OP_RMW_REG_B_DECL(a, rol),   // 2a
   cpu_op_pld,                      // 2b
   ALUOP(ADDR_W, bit),              // 2c
   ALUOP(ADDR_W, and),              // 2d
   RMWOP(ADDR_W, rol),              // 2e
   ALUOP(LONG_W, and),              // 2f
   CPU_OP_BRANCH_REG_DECL(n),       // 30
   ALUOP(IDPY_W, and),              // 31
   ALUOP(IDP_W, and),               // 32
   ALUOP(ISRY_W, and),              // 33
   CPU_OP_READ_DPR_W_DECL(bit, x),  // 34
   CPU_OP_READ_DPR_W_DECL(and, x),  // 35
   RMWOP(DPX_W, rol),               // 36
   ALUOP(ILDPY_W, and),             // 37
   CPU_OP_SET_FLAG_DECL(c),         // 38
   ALUOP(ADDRY_W, and),             // 39
   CPU_OP_RMW_REG_W_DECL(a, dec),   // 3a
   cpu_op_tsc,                      // 3b
   ALUOP(ADDRX_W, bit),             // 3c
   ALUOP(ADDRX_W, and),             // 3d
   RMWOP(ADDRX_W, rol),             // 3e
   ALUOP(LONGX_W, and),             // 3f
   cpu_op_rti_n,                    // 40
   ALUOP(IDPX_W, eor),              // 41
   cpu_op_wdm,                      // 42
   ALUOP(SR_B, eor),                // 43
   cpu_op_mvp,                      // 44
   ALUOP(DP_W, eor),                // 45
   RMWOP(DP_W, lsr),                // 46
   ALUOP(ILDP_W, eor),              // 47
   cpu_op_pha_w,                    // 48
   ALUOP(CONST_W, eor),             // 49
   CPU_OP_RMW_REG_B_DECL(a, lsr),   // 4a
   cpu_op_phk,                      // 4b
   cpu_op_jmp_addr,                 // 4c
   ALUOP(CONST_W, eor),             // 4d
   RMWOP(ADDR_W, lsr),              // 4e
   ALUOP(LONG_W, eor),              // 4f
   CPU_OP_BRANCH_REG_N_DECL(v),     // 50
   ALUOP(IDPY_W, eor),              // 51
   ALUOP(IDP_W, eor),               // 52
   ALUOP(ISRY_W, eor),              // 53
   cpu_op_mvn,                      // 54
   CPU_OP_READ_DPR_B_DECL(eor, x),  // 55
   RMWOP(DPX_W, lsr),               // 56
   ALUOP(ILDPY_W, eor),             // 57
   CPU_OP_RESET_FLAG_DECL(i),       // 58
   ALUOP(ADDRY_W, eor),             // 59
   cpu_op_phy_b,                    // 5a
   cpu_op_tcd,                      // 5b
   cpu_op_jmp_long,                 // 5c
   ALUOP(ADDRX_W, eor),             // 5d
   RMWOP(ADDRX_W, lsr),             // 5e
   ALUOP(LONGX_W, eor),             // 5f
   cpu_op_rts,                      // 60
   ALUOP(IDPX_W, adc),              // 61
   cpu_op_per_n,                    // 62
   ALUOP(SR_W, adc),                // 63
   STOREOP(DP_W, zero),             // 64
   ALUOP(DP_W, adc),                // 65
   RMWOP(DP_W, ror),                // 66
   ALUOP(ILDP_W, adc),              // 67
   cpu_op_pla_w,                    // 68
   ALUOP(CONST_W, adc),             // 69
   CPU_OP_RMW_REG_W_DECL(a, ror),   // 6a
   cpu_op_rtl_n,                    // 6b
   cpu_op_jmp_iaddr,                // 6c
   ALUOP(ADDR_W, adc),              // 6d
   RMWOP(ADDR_W, ror),              // 6e 
   ALUOP(LONG_W, adc),              // 6f
   CPU_OP_BRANCH_REG_DECL(v),       // 70
   ALUOP(IDPY_W, adc),              // 71
   ALUOP(IDP_W, adc),               // 72
   ALUOP(ISRY_W, adc),              // 73
   CPU_OP_STORE_DPR_W_DECL(zero, x),// 74
   CPU_OP_READ_DPR_W_DECL(adc, x),  // 75
   RMWOP(DPX_W, ror),               // 76
   ALUOP(ILDPY_W, adc),             // 77
   CPU_OP_SET_FLAG_DECL(i),         // 78
   ALUOP(ADDRY_W, adc),             // 79
   cpu_op_ply_b,                    // 7a
   cpu_op_tdc,                      // 7b
   cpu_op_jmp_iaddrx,               // 7c
   ALUOP(ADDRX_W, adc),             // 7d
   RMWOP(ADDRX_W, ror),             // 7e
   ALUOP(LONGX_W, adc),             // 7f
   cpu_op_bra,                      // 80
   cpu_op_sta_idpx_w,               // 81
   cpu_op_brl,                      // 82
   cpu_op_sta_sr_w,                 // 83
   STOREOP(DP_B, y),                // 84
   STOREOP(DP_W, a),                // 85
   STOREOP(DP_B, x),                // 86
   cpu_op_sta_ildp_w,               // 87
   CPU_OP_RMW_REG_B_DECL(y, dec),   // 88
   ALUOP(CONST_W, bit),             // 89
   cpu_op_txa_mX,                   // 8a
   cpu_op_phb,                      // 8b
   STOREOP(ADDR_B, y),              // 8c
   STOREOP(ADDR_W, a),              // 8d
   STOREOP(ADDR_B, x),              // 8e
   cpu_op_sta_long_w,               // 8f
   CPU_OP_BRANCH_REG_N_DECL(c),     // 90
   cpu_op_sta_idpy_w,               // 91
   cpu_op_sta_idp_w,                // 92
   cpu_op_sta_isry_w,               // 93
   CPU_OP_STORE_DPR_B_DECL(y, x),   // 94
   CPU_OP_STORE_DPR_W_DECL(a, x),   // 95
   CPU_OP_STORE_DPR_B_DECL(x, y),   // 96
   cpu_op_sta_ildpy_w,              // 97
   cpu_op_tya_mX,                   // 98
   CPU_OP_STORE_ADDR_REGI_W_DECL(a, y),  // 99
   cpu_op_txs_n,                    // 9a
   cpu_op_txy_b,                    // 9b
   STOREOP(ADDR_W, zero),           // 9c
   CPU_OP_STORE_ADDR_REGI_W_DECL(a, x),  // 9d
   CPU_OP_STORE_ADDR_REGI_W_DECL(zero, x),  // 9e
   cpu_op_sta_longx_w,              // 9f
   ALUOP(CONST_B, ldy),             // a0
   ALUOP(IDPX_W, lda),              // a1
   ALUOP(CONST_B, ldx),             // a2
   ALUOP(SR_W, lda),                // a3
   ALUOP(DP_B, ldy),                // a4
   ALUOP(DP_W, lda),                // a5
   ALUOP(DP_B, ldx),                // a6
   ALUOP(ILDP_W, lda),              // a7
   cpu_op_tay_mX,                   // a8
   ALUOP(CONST_W, lda),             // a9
   cpu_op_tax_mX,                   // aa
   cpu_op_plb,                      // ab
   ALUOP(ADDR_B, ldy),              // ac
   ALUOP(ADDR_W, lda),              // ad
   ALUOP(ADDR_B, ldx),              // ae
   ALUOP(LONG_W, lda),              // af
   CPU_OP_BRANCH_REG_DECL(c),       // b0
   ALUOP(IDPY_W, lda),              // b1
   ALUOP(IDP_W, lda),               // b2
   ALUOP(ISRY_W, lda),              // b3
   CPU_OP_READ_DPR_B_DECL(ldy, x),  // b4
   CPU_OP_READ_DPR_W_DECL(lda, x),  // b5
   CPU_OP_READ_DPR_B_DECL(ldx, y),  // b6
   ALUOP(ILDPY_W, lda),             // b7
   CPU_OP_RESET_FLAG_DECL(v),       // b8
   ALUOP(ADDRY_W, lda),             // b9
   cpu_op_tsx_w,                    // ba
   cpu_op_tyx_b,                    // bb
   ALUOP(ADDRX_B, ldy),             // bc
   ALUOP(ADDRX_W, lda),             // bd
   ALUOP(ADDRX_B, ldx),             // be
   ALUOP(LONGX_W, lda),             // bf
   ALUOP(CONST_B, cpy),             // c0
   ALUOP(IDPX_W, cmp),              // c1
   cpu_op_rep,                      // c2
   ALUOP(SR_W, cmp),                // c3
   ALUOP(DP_B, cpy),                // c4
   ALUOP(DP_W, cmp),                // c5
   RMWOP(DP_W, dec),                // c6
   ALUOP(ILDP_W, cmp),              // c7
   CPU_OP_RMW_REG_B_DECL(y, inc),   // c8
   ALUOP(CONST_W, cmp),             // c9
   CPU_OP_RMW_REG_B_DECL(x, dec),   // ca
   cpu_op_wai,                      // cb
   ALUOP(ADDR_B, cpy),              // cc
   ALUOP(ADDR_W, cmp),              // cd
   RMWOP(ADDR_W, dec),              // ce
   ALUOP(LONG_W, cmp),              // cf
   CPU_OP_BRANCH_REG_N_DECL(z),     // d0
   ALUOP(IDPY_W, cmp),              // d1
   ALUOP(IDP_W, cmp),               // d2
   ALUOP(ISRY_W, cmp),              // d3
   cpu_op_pei_n,                    // d4
   CPU_OP_READ_DPR_W_DECL(cmp, x),  // d5
   RMWOP(DPX_W, dec),               // d6
   ALUOP(ILDPY_W, cmp),             // d7
   CPU_OP_RESET_FLAG_DECL(d),       // d8
   ALUOP(ADDRY_W, cmp),             // d9
   cpu_op_phx_b,                    // da
   cpu_op_stp,                      // db
   cpu_op_jmp_iladdr,               // dc
   ALUOP(ADDRX_W, cmp),             // dd
   RMWOP(ADDRX_W, dec),             // de
   ALUOP(LONGX_W, cmp),             // df
   ALUOP(CONST_B, cpx),             // e0
   ALUOP(IDPX_W, sbc),              // e1
   cpu_op_sep,                      // e2
   ALUOP(SR_W, sbc),                // e3
   ALUOP(DP_B, cpx),                // e4
   ALUOP(DP_W, sbc),                // e5
   RMWOP(DP_W, inc),                // e6
   ALUOP(ILDP_W, sbc),              // e7
   CPU_OP_RMW_REG_W_DECL(x, inc),   // e8
   ALUOP(CONST_W, sbc),             // e9
   NOP,                             // ea
   cpu_op_xba,                      // eb
   ALUOP(ADDR_B, cpx),              // ec
   ALUOP(ADDR_W, sbc),              // ed
   RMWOP(ADDR_W, inc),              // ee
   ALUOP(LONG_W, sbc),              // ef
   CPU_OP_BRANCH_REG_DECL(z),       // f0
   ALUOP(IDPY_W, sbc),              // f1
   ALUOP(IDP_W, sbc),               // f2
   ALUOP(ISRY_W, sbc),              // f3
   cpu_op_pea,                      // f4
   CPU_OP_READ_DPR_B_DECL(sbc, x),  // f5
   RMWOP(DPX_W, inc),               // f6
   ALUOP(ILDPY_W, sbc),             // f7
   CPU_OP_SET_FLAG_DECL(d),         // f8
   ALUOP(ADDRY_W, sbc),             // f9
   cpu_op_plx_b,                    // fa
   cpu_op_xce,                      // fb
   cpu_op_jsr_iaddrx_n,             // fc
   ALUOP(ADDRX_W, sbc),             // fd
   RMWOP(ADDRX_W, inc),             // fe
   ALUOP(LONGX_W, sbc)              // ff

};

const cpu_op_t op_table_Mx[256] = {
   CPU_OP_INTERRUPT_N_DECL(brk),    // 00 
   ALUOP(IDPX_B, ora),              // 01
   NOP,                             // 02 ?!?!
   ALUOP(SR_B, ora),                // 03
   RMWOP(DP_B, tsb),                // 04
   ALUOP(DP_B, ora),                // 05
   RMWOP(DP_B, asl),                // 06
   ALUOP(ILDP_B, ora),              // 07
   cpu_op_php,                      // 08
   ALUOP(CONST_B, ora),             // 09
   CPU_OP_RMW_REG_B_DECL(a, asl),   // 0a
   cpu_op_phd,                      // 0b
   RMWOP(ADDR_B, tsb),              // 0c
   ALUOP(ADDR_B, ora),              // 0d
   RMWOP(ADDR_B, asl),              // 0e
   ALUOP(LONG_B, ora),              // 0f
   CPU_OP_BRANCH_REG_N_DECL(n),     // 10
   ALUOP(IDPY_B, ora),              // 11
   ALUOP(IDP_B, ora),               // 12
   ALUOP(ISRY_B, ora),              // 13
   RMWOP(DP_B, trb),                // 14
   CPU_OP_READ_DPR_B_DECL(ora, x),  // 15
   RMWOP(DPX_B, asl),               // 16
   ALUOP(IDPY_B, ora),              // 17
   CPU_OP_RESET_FLAG_DECL(c),       // 18
   ALUOP(ADDRY_B, ora),             // 19
   CPU_OP_RMW_REG_B_DECL(a, inc),   // 1a
   cpu_op_tcs_n,                    // 1b
   RMWOP(ADDR_B, trb),              // 1c
   ALUOP(ADDRX_B, ora),             // 1d
   RMWOP(ADDRX_B, asl),             // 1e
   ALUOP(LONGX_B, ora),             // 1f
   cpu_op_jsr_addr,                 // 20
   ALUOP(IDPX_B, and),              // 21
   cpu_op_jsr_long_n,               // 22
   ALUOP(SR_B, and),                // 23
   ALUOP(DP_B, bit),                // 24
   ALUOP(DP_B, and),                // 25
   RMWOP(DP_B, rol),                // 26
   ALUOP(ILDP_B, and),              // 27
   cpu_op_plp_n,                    // 28
   ALUOP(CONST_B, and),             // 29
   CPU_OP_RMW_REG_B_DECL(a, rol),   // 2a
   cpu_op_pld,                      // 2b
   ALUOP(ADDR_B, bit),              // 2c
   ALUOP(ADDR_B, and),              // 2d
   RMWOP(ADDR_B, rol),              // 2e
   ALUOP(LONG_B, and),              // 2f
   CPU_OP_BRANCH_REG_DECL(n),       // 30
   ALUOP(IDPY_B, and),              // 31
   ALUOP(IDP_B, and),               // 32
   ALUOP(ISRY_B, and),              // 33
   CPU_OP_READ_DPR_B_DECL(bit, x),  // 34
   CPU_OP_READ_DPR_B_DECL(and, x),  // 35
   RMWOP(DPX_B, rol),               // 36
   ALUOP(ILDPY_B, and),             // 37
   CPU_OP_SET_FLAG_DECL(c),         // 38
   ALUOP(ADDRY_B, and),             // 39
   CPU_OP_RMW_REG_B_DECL(a, dec),   // 3a
   cpu_op_tsc,                      // 3b
   ALUOP(ADDRX_B, bit),             // 3c
   ALUOP(ADDRX_B, and),             // 3d
   RMWOP(ADDRX_B, rol),             // 3e
   ALUOP(LONGX_B, and),             // 3f
   cpu_op_rti_n,                    // 40
   ALUOP(IDPX_B, eor),              // 41
   cpu_op_wdm,                      // 42
   ALUOP(SR_B, eor),                // 43
   cpu_op_mvp,                      // 44
   ALUOP(DP_B, eor),                // 45
   RMWOP(DP_B, lsr),                // 46
   ALUOP(ILDP_B, eor),              // 47
   cpu_op_pha_b,                    // 48
   ALUOP(CONST_B, eor),             // 49
   CPU_OP_RMW_REG_B_DECL(a, lsr),   // 4a
   cpu_op_phk,                      // 4b
   cpu_op_jmp_addr,                 // 4c
   ALUOP(CONST_B, eor),             // 4d
   RMWOP(ADDR_B, lsr),              // 4e
   ALUOP(LONG_B, eor),              // 4f
   CPU_OP_BRANCH_REG_N_DECL(v),     // 50
   ALUOP(IDPY_B, eor),              // 51
   ALUOP(IDP_B, eor),               // 52
   ALUOP(ISRY_B, eor),              // 53
   cpu_op_mvn,                      // 54
   CPU_OP_READ_DPR_B_DECL(eor, x),  // 55
   RMWOP(DPX_B, lsr),               // 56
   ALUOP(ILDPY_B, eor),             // 57
   CPU_OP_RESET_FLAG_DECL(i),       // 58
   ALUOP(ADDRY_B, eor),             // 59
   cpu_op_phy_w,                    // 5a
   cpu_op_tcd,                      // 5b
   cpu_op_jmp_long,                 // 5c
   ALUOP(ADDRX_B, eor),             // 5d
   RMWOP(ADDRX_B, lsr),             // 5e
   ALUOP(LONGX_B, eor),             // 5f
   cpu_op_rts,                      // 60
   ALUOP(IDPX_B, adc),              // 61
   cpu_op_per_n,                    // 62
   ALUOP(SR_B, adc),                // 63
   STOREOP(DP_B, zero),             // 64
   ALUOP(DP_B, adc),                // 65
   RMWOP(DP_B, ror),                // 66
   ALUOP(ILDP_B, adc),              // 67
   cpu_op_pla_b,                    // 68
   ALUOP(CONST_B, adc),             // 69
   CPU_OP_RMW_REG_B_DECL(a, ror),   // 6a
   cpu_op_rtl_n,                    // 6b
   cpu_op_jmp_iaddr,                // 6c
   ALUOP(ADDR_B, adc),              // 6d
   RMWOP(ADDR_B, ror),              // 6e 
   ALUOP(LONG_B, adc),              // 6f
   CPU_OP_BRANCH_REG_DECL(v),       // 70
   ALUOP(IDPY_B, adc),              // 71
   ALUOP(IDP_B, adc),               // 72
   ALUOP(ISRY_B, adc),              // 73
   CPU_OP_STORE_DPR_B_DECL(zero, x),// 74
   CPU_OP_READ_DPR_B_DECL(adc, x),  // 75
   RMWOP(DPX_B, ror),               // 76
   ALUOP(ILDPY_B, adc),             // 77
   CPU_OP_SET_FLAG_DECL(i),         // 78
   ALUOP(ADDRY_B, adc),             // 79
   cpu_op_ply_w,                    // 7a
   cpu_op_tdc,                      // 7b
   cpu_op_jmp_iaddrx,               // 7c
   ALUOP(ADDRX_B, adc),             // 7d
   RMWOP(ADDRX_B, ror),             // 7e
   ALUOP(LONGX_B, adc),             // 7f
   cpu_op_bra,                      // 80
   cpu_op_sta_idpx_b,               // 81
   cpu_op_brl,                      // 82
   cpu_op_sta_sr_b,                 // 83
   STOREOP(DP_W, y),                // 84
   STOREOP(DP_B, a),                // 85
   STOREOP(DP_W, x),                // 86
   cpu_op_sta_ildp_b,               // 87
   CPU_OP_RMW_REG_W_DECL(y, dec),   // 88
   ALUOP(CONST_B, bit),             // 89
   cpu_op_txa_Mx,                   // 8a
   cpu_op_phb,                      // 8b
   STOREOP(ADDR_W, y),              // 8c
   STOREOP(ADDR_B, a),              // 8d
   STOREOP(ADDR_W, x),              // 8e
   cpu_op_sta_long_b,               // 8f
   CPU_OP_BRANCH_REG_N_DECL(c),     // 90
   cpu_op_sta_idpy_b,               // 91
   cpu_op_sta_idp_b,                // 92
   cpu_op_sta_isry_b,               // 93
   CPU_OP_STORE_DPR_W_DECL(y, x),   // 94
   CPU_OP_STORE_DPR_B_DECL(a, x),   // 95
   CPU_OP_STORE_DPR_W_DECL(x, y),   // 96
   cpu_op_sta_ildpy_b,              // 97
   cpu_op_tya_Mx,                   // 98
   CPU_OP_STORE_ADDR_REGI_B_DECL(a, y),  // 99
   cpu_op_txs_n,                    // 9a
   cpu_op_txy_w,                    // 9b
   STOREOP(ADDR_B, zero),           // 9c
   CPU_OP_STORE_ADDR_REGI_B_DECL(a, x),  // 9d
   CPU_OP_STORE_ADDR_REGI_W_DECL(zero, x),  // 9e
   cpu_op_sta_longx_b,              // 9f
   ALUOP(CONST_W, ldy),             // a0
   ALUOP(IDPX_B, lda),              // a1
   ALUOP(CONST_W, ldx),             // a2
   ALUOP(SR_B, lda),                // a3
   ALUOP(DP_W, ldy),                // a4
   ALUOP(DP_B, lda),                // a5
   ALUOP(DP_W, ldx),                // a6
   ALUOP(ILDP_B, lda),              // a7
   cpu_op_tay_Mx,                   // a8
   ALUOP(CONST_B, lda),             // a9
   cpu_op_tax_Mx,                   // aa
   cpu_op_plb,                      // ab
   ALUOP(ADDR_W, ldy),              // ac
   ALUOP(ADDR_B, lda),              // ad
   ALUOP(ADDR_W, ldx),              // ae
   ALUOP(LONG_B, lda),              // af
   CPU_OP_BRANCH_REG_DECL(c),       // b0
   ALUOP(IDPY_B, lda),              // b1
   ALUOP(IDP_B, lda),               // b2
   ALUOP(ISRY_B, lda),              // b3
   CPU_OP_READ_DPR_W_DECL(ldy, x),  // b4
   CPU_OP_READ_DPR_B_DECL(lda, x),  // b5
   CPU_OP_READ_DPR_W_DECL(ldx, y),  // b6
   ALUOP(ILDPY_B, lda),             // b7
   CPU_OP_RESET_FLAG_DECL(v),       // b8
   ALUOP(ADDRY_B, lda),             // b9
   cpu_op_tsx_b,                    // ba
   cpu_op_tyx_w,                    // bb
   ALUOP(ADDRX_W, ldy),             // bc
   ALUOP(ADDRX_B, lda),             // bd
   ALUOP(ADDRX_W, ldx),             // be
   ALUOP(LONGX_B, lda),             // bf
   ALUOP(CONST_W, cpy),             // c0
   ALUOP(IDPX_B, cmp),              // c1
   cpu_op_rep,                      // c2
   ALUOP(SR_B, cmp),                // c3
   ALUOP(DP_W, cpy),                // c4
   ALUOP(DP_B, cmp),                // c5
   RMWOP(DP_B, dec),                // c6
   ALUOP(ILDP_B, cmp),              // c7
   CPU_OP_RMW_REG_W_DECL(y, inc),   // c8
   ALUOP(CONST_B, cmp),             // c9
   CPU_OP_RMW_REG_W_DECL(x, dec),   // ca
   cpu_op_wai,                      // cb
   ALUOP(ADDR_W, cpy),              // cc
   ALUOP(ADDR_B, cmp),              // cd
   RMWOP(ADDR_B, dec),              // ce
   ALUOP(LONG_B, cmp),              // cf
   CPU_OP_BRANCH_REG_N_DECL(z),     // d0
   ALUOP(IDPY_B, cmp),              // d1
   ALUOP(IDP_B, cmp),               // d2
   ALUOP(ISRY_B, cmp),              // d3
   cpu_op_pei_n,                    // d4
   CPU_OP_READ_DPR_B_DECL(cmp, x),  // d5
   RMWOP(DPX_B, dec),               // d6
   ALUOP(ILDPY_B, cmp),             // d7
   CPU_OP_RESET_FLAG_DECL(d),       // d8
   ALUOP(ADDRY_B, cmp),             // d9
   cpu_op_phx_w,                    // da
   cpu_op_stp,                      // db
   cpu_op_jmp_iladdr,               // dc
   ALUOP(ADDRX_B, cmp),             // dd
   RMWOP(ADDRX_B, dec),             // de
   ALUOP(LONGX_B, cmp),             // df
   ALUOP(CONST_W, cpx),             // e0
   ALUOP(IDPX_B, sbc),              // e1
   cpu_op_sep,                      // e2
   ALUOP(SR_B, sbc),                // e3
   ALUOP(DP_W, cpx),                // e4
   ALUOP(DP_B, sbc),                // e5
   RMWOP(DP_B, inc),                // e6
   ALUOP(ILDP_B, sbc),              // e7
   CPU_OP_RMW_REG_W_DECL(x, inc),   // e8
   ALUOP(CONST_B, sbc),             // e9
   NOP,                             // ea
   cpu_op_xba,                      // eb
   ALUOP(ADDR_W, cpx),              // ec
   ALUOP(ADDR_B, sbc),              // ed
   RMWOP(ADDR_B, inc),              // ee
   ALUOP(LONG_B, sbc),              // ef
   CPU_OP_BRANCH_REG_DECL(z),       // f0
   ALUOP(IDPY_B, sbc),              // f1
   ALUOP(IDP_B, sbc),               // f2
   ALUOP(ISRY_B, sbc),              // f3
   cpu_op_pea,                      // f4
   CPU_OP_READ_DPR_B_DECL(sbc, x),  // f5
   RMWOP(DPX_B, inc),               // f6
   ALUOP(ILDPY_B, sbc),             // f7
   CPU_OP_SET_FLAG_DECL(d),         // f8
   ALUOP(ADDRY_B, sbc),             // f9
   cpu_op_plx_w,                    // fa
   cpu_op_xce,                      // fb
   cpu_op_jsr_iaddrx_n,             // fc
   ALUOP(ADDRX_B, sbc),             // fd
   RMWOP(ADDRX_B, inc),             // fe
   ALUOP(LONGX_B, sbc)              // ff
};

const uint8_t cycle_table_EMUL[256];
const uint8_t cycle_table_mx[256];
const uint8_t cycle_table_mX[256];
const uint8_t cycle_table_Mx[256];
const uint8_t cycle_table_MX[256];

const cpu_op_t *op_table = op_table_EMUL;
const uint8_t *cycle_table = cycle_table_EMUL;

const cpu_op_t* op_table_index[] = {
   op_table_mx,
   op_table_mX,
   op_table_Mx,
   op_table_MX,
   op_table_EMUL,
   op_table_EMUL,
   op_table_EMUL,
   op_table_EMUL
};

const uint8_t* cycle_table_index[] = {
   cycle_table_mx,
   cycle_table_mX,
   cycle_table_Mx,
   cycle_table_MX,
   cycle_table_EMUL,
   cycle_table_EMUL,
   cycle_table_EMUL,
   cycle_table_EMUL
};

const char* opcode_names[] = {
   "brk",         "ora (idpx)",     "cop",         "ora (sr)",             // 0x00
   "tsb (dp)",    "ora (dp)",       "asl (dp)",    "ora (ildp)",           // 0x04
   "php",         "ora (imm)",      "asl A",       "phd",                  // 0x08
   "tsb (addr)",  "ora (addr)",     "asl (addr)",  "ora (long)",           // 0x0c

   "bpl",         "ora (idpy)",     "ora (idp)",   "ora (isry)",           // 0x10
   "trb (dp)",    "ora (dpx)",      "asl (dpx)",   "ora (ildpy)",          // 0x14
   "clc",         "ora (addry)",    "inc A",       "tcs",                  // 0x18
   "trb (addr)",  "ora (addrx)",    "asl (addrx)", "ora (longx)",          // 0x1c

   "jsr (addr)",  "and (idpx)",     "jsl",          "and (sr)",             // 0x20
   "bit (dp)",    "and (dp)",       "rol (dp)",    "and (ildp)",           // 0x24
   "plp",         "and (imm)",      "rol A",       "pld",                  // 0x28
   "bit (addr)",  "and (addr)",     "rol (addr)",  "and (long)",           // 0x2c

   "bmi",         "and (idpy)",     "and (idp)",   "and (isry)",           // 0x30
   "bit (dpx)",   "and (dpx)",      "rol (dpx)",   "and (ildpy)",          // 0x34
   "sec",         "and (addry)",    "dec",         "tsc",                  // 0x38
   "bit (addrx)", "and (addrx)",    "rol (addrx)", "and (longx)",          // 0x3c

   "rti",         "eor (idpx)",     "wdm",         "eor (idpx)",           // 0x40
   "mvp",         "eor (dp)",       "lsr (dp)",    "eor (ildp)",           // 0x44
   "pha",         "eor (imm)",      "lsr A",       "phk",                  // 0x48
   "jmp (addr)",  "eor (addr)",     "lsr (addr)",  "eor (long)",           // 0x4c

   "bvc",         "eor (idpy)",     "eor (idp)",   "eor (isry)",           // 0x50
   "mvn",         "eor (dpx)",      "lsr (dpx)",   "eor (ildpy)",          // 0x54
   "cli",         "eor (addry)",    "phy",         "tcd",                  // 0x58
   "jml",         "eor (addrx)",    "lsr (addrx)", "eor (longx)",          // 0x5c

   "rts",         "adc (idpx)",     "per",         "adc (sr)",             // 0x60
   "stz (dp)",    "adc (dp)",       "ror (dp)",    "adc (ildp)",           // 0x64
   "pla",         "adc (imm)",      "ror A",       "rtl",                  // 0x68
   "jmp (iaddr)", "adc (addr)",     "ror (addr)",  "adc (long)",           // 0x6c

   "bvs",         "adc (idpy)",     "adc (idp)",   "adc (isry)",           // 0x70
   "stz (dpx)",   "adc (dpx)",      "ror (dpx)",   "adc (ildpy)",          // 0x74
   "sei",         "adc (addry)",    "ply",         "tdc",                  // 0x78
   "jmp (ilong)", "adc (addrx)",    "ror (addrx)", "adc (longx)",          // 0x7c

   "bra",         "sta (idpx)",     "brl",         "sta (sr)",             // 0x80
   "sty (dp)",    "sta (dp)",       "stx (dp)",    "sta (ildp)",           // 0x84
   "dey",         "bit (imm)",      "txa",         "phb",                  // 0x88
   "sty (addr)",  "sta (addr)",     "stx (addr)",  "sta (long)",           // 0x8c

   "bcc",         "sta (idpy)",     "sta (idp)",   "sta (isry)",           // 0x90
   "sty (dpx)",   "sta (dpx)",      "stx (dpy)",   "sta (ildpy)",          // 0x94
   "tya",         "sta (addry)",    "txs",         "txy",                  // 0x98
   "stz (addr)",  "sta (addrx)",    "stz (addrx)", "sta (longx)",          // 0x9c

   "ldy (imm)",   "lda (idpx)",     "ldx (imm)",   "lda (sr)",             // 0xa0
   "ldy (dp)",    "lda (dp)",       "lda (dp)",    "lda (ildp)",           // 0xa4
   "tay",         "lda (imm)",      "tax",         "plb",                  // 0xa8
   "ldy (addr)",  "lda (addr)",     "ldx (addr)",  "lda (long)",           // 0xac

   "bcs",         "lda (idpy)",     "lda (idp)",   "lda (isry)",           // 0xb0
   "ldy (dpx)",   "lda (dpx)",      "ldx (dpy)",   "lda (ildpy)",          // 0xb4
   "clv",         "lda (addry)",    "tsx",         "tyx",                  // 0xb8
   "ldy (addrx)", "lda (addrx)",    "ldx (addry)", "lda (longx)",          // 0xbc

   "cpy (imm)",   "cmp (idpx)",     "rep",         "cmp (sr)",             // 0xc0
   "cpy (dp)",    "cmp (dp)",       "dec (dp)",    "cmp (ildp)",           // 0xc4
   "iny",         "cmp (imm)",      "dex",         "wai",                  // 0xc8
   "cpy (addr)",  "cmp (addr)",     "dec (addr)",  "cmp (long)",           // 0xcc

   "bne",         "cmp (idpy)",     "cmp (idp)",   "cmp (isry)",           // 0xd0
   "pei",         "cmp (dpx)",      "dec (dpx)",   "cmp (ildpy)",          // 0xd4
   "cld",         "cmp (addry)",    "phx",         "stp",                  // 0xd8
   "jmp (ilong)", "cmp (addrx)",    "dec (addrx)", "cmp (longx)",          // 0xdc

   "cpx (imm)",   "sbc (idpx)",     "sep",         "sbc (sr)",             // 0xe0
   "cpx (dp)",    "sbc (dp)",       "inc (dp)",    "sbc (ildp)",           // 0xe4
   "inx",         "sbc (imm)",      "nop",         "xba",                  // 0xe8
   "cpx (addr)",  "sbc (addr)",     "inc (addr)",  "sbc (long)",           // 0xec

   "beq",         "sbc (idpy)",     "sbc (idp)",   "sbc (isry)",           // 0xf0
   "pea",         "sbc (dpx)",      "inc (dpx)",   "sbc (ildpy)",          // 0xf4
   "sed",         "sbc (addry)",    "plx",         "xce",                  // 0xf8
   "jsr (iaddr)", "sbc (addrx)",    "inc (addrx)", "sbc (longx)",          // 0xfc





};
