#ifndef __SMP_TABLE_H
#define __SMP_TABLE_H

#include "system/state.h"
#include "system/macros.h"
#include <stdint.h>

typedef void (*smp_op_t)(void);
extern smp_op_t ssnes_smp_optable[256];
extern uint8_t ssnes_smp_cycle_table[256];

#endif
