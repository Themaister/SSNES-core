#include "memmap.h"
#include "cart.h"
#include <stdlib.h>
#include "state.h"
#include "macros.h"
#include <assert.h>
#include <string.h>

void ssnes_cartridge_load(const uint8_t *data, size_t size)
{
   MEM.cart.rom = calloc(1, 1 << 22);
   assert(MEM.cart.rom);

   memcpy(MEM.cart.rom, data, size < (1 << 22) ? size : (1 << 22));

   ssnes_memmap_init(SSNES_MAPPER_LOROM);
}

void ssnes_cartridge_unload(void)
{
   if (MEM.cart.rom)
   {
      free(MEM.cart.rom);
      MEM.cart.rom = NULL;
   }

   if (MEM.cart.sram)
   {
      free(MEM.cart.sram);
      MEM.cart.sram = NULL;
   }
}
