#include "table.h"

cpu_op_t *op_table = NULL;

cpu_op_t op_table_EMUL[256];
cpu_op_t op_table_mx[256];
cpu_op_t op_table_mX[256];
cpu_op_t op_table_Mx[256];
cpu_op_t op_table_MX[256];

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

void cpu_init_tables(void)
{


}
