#include "memory.h"

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static uint8_t *pool;
static uintptr_t ptr;
static size_t pool_size;

void ssnes_pool_init(size_t size)
{
   pool = calloc(1, size);
   assert(pool);
   pool_size = size;
   ptr = (uintptr_t)pool;
}

void* ssnes_pool_alloc(unsigned align, size_t size)
{
   uintptr_t aligned = align - 1;
   ptr += aligned;
   ptr &= ~(aligned);

   void *ret = (void*)ptr;
   ptr += size;

   assert((ptrdiff_t)ptr - (ptrdiff_t)pool < pool_size);
   return ret;
}

void ssnes_pool_free(void)
{
   if (pool)
   {
      free(pool);
      pool = NULL;
   }
}
