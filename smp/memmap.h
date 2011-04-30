#ifndef __SMP_MEMMAP_H
#define __SMP_MEMMAP_H

#include <stdint.h>

typedef void (*smp_write_t)(uint16_t addr, uint8_t data, uint8_t mask);
typedef uint8_t (*smp_read_t)(uint16_t addr);

// 16-byte granularity.

extern smp_write_t ssnes_smp_memmap_write[1 << 12];
extern smp_read_t ssnes_smp_memmap_read[1 << 12];


void ssnes_smp_init_memmap(void);

#endif
