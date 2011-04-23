#ifndef __CPU_MEMMAP_H
#define __CPU_MEMMAP_H

#include <stdint.h>

typedef uint8_t (*memmap_read_t)(uint32_t addr);
typedef void (*memmap_write_t)(uint32_t addr, uint8_t data);

extern memmap_read_t ssnes_memmap_read_table[256 * 8];
extern memmap_write_t ssnes_memmap_write_table[256 * 8];

enum ssnes_mapper_type
{
   SSNES_MAPPER_LOROM,
   SSNES_MAPPER_HIROM
};

void ssnes_memmap_init(enum ssnes_mapper_type type);

#endif
