#include "system/memory.h"
#include "ppu/ppu.h"
#include "system/state.h"
#include "system/macros.h"
#include <stdint.h>

void ssnes_ppu_init(void)
{
   ALLOCATE(PPU.buffer, 1024, 512 * 256 * sizeof(uint16_t));
}

void ssnes_ppu_deinit(void)
{}
