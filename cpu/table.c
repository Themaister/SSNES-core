#include "table.h"
#include "op.h"
#include <stddef.h>

#define ALUOP(op, alu) CPU_OP_READ_##op##_DECL(alu)
#define RMWOP(mode, op) CPU_OP_RMW_##mode##_DECL(op)
#define STOREOP(op, reg) CPU_OP_STORE_##op##_DECL(reg)
#define NOP cpu_op_nop

const cpu_op_t op_table_EMUL[256];

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
   CPU_OP_READ_DPR_B_DECL(ldy, x),  // b5
   CPU_OP_READ_DPR_B_DECL(ldy, x),  // b6
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
   ALUOP(CONST_B, cpx),             // e2
   ALUOP(SR_B, sbc),                // e3
   ALUOP(DP_B, cpx),                // e4
   ALUOP(DP_B, sbc),                // e5
   RMWOP(DP_B, inc),                // e6
   ALUOP(ILDP_B, sbc),              // e7
   CPU_OP_RMW_REG_B_DECL(x, inc),   // e8
   ALUOP(CONST_B, sbc),             // e9
   NOP,                             // ea
   cpu_op_xba,                      // eb
   ALUOP(ADDRX_B, cpx),              // ec
   


};

const cpu_op_t op_table_mx[256];
const cpu_op_t op_table_mX[256];
const cpu_op_t op_table_Mx[256];

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

void cpu_init_tables(void)
{
   CPU_OP_RMW_REG_B_DECL(a, inc) ();


}
