#include "memmap.h"
#include "cart.h"
#include "state.h"
#include "macros.h"
#include <assert.h>
#include <string.h>
#include "memory.h"

static enum ssnes_mapper_type find_mapper_type(const uint8_t *data, size_t size)
{
   (void)data;
   (void)size;

   // Dummy
   return SSNES_MAPPER_LOROM;
}

void ssnes_cartridge_load(const uint8_t *data, size_t size)
{
   if (!MEM.cart.rom)
      MEM.cart.rom = ssnes_pool_alloc(1024, 1 << 22);

   memcpy(MEM.cart.rom, data, size < (1 << 22) ? size : (1 << 22));

   ssnes_memmap_init(find_mapper_type(data, size));
}

void ssnes_cartridge_unload(void)
{}
