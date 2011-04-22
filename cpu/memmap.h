#ifndef __CPU_MEMMAP_H
#define __CPU_MEMMAP_H

#include <stdint.h>

typedef uint8_t (*memmap_read_t)(uint32_t addr);
typedef void (*memmap_write_t)(uint32_t addr, uint8_t data);

extern memmap_read_t memmap_read_table[256 * 8];
extern memmap_write_t memmap_write_table[256 * 8];

enum snes_mapper_type
{
   SNES_MAPPER_LOROM,
   SNES_MAPPER_HIROM
};

void memmap_init(enum snes_mapper_type type);

#endif
