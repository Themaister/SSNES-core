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

   fprintf(stderr, "Loaded memory pool: %lu bytes.\n", (unsigned long)size);
}

void* ssnes_pool_alloc(unsigned align, size_t size)
{
   uintptr_t aligned = align - 1;
   fprintf(stderr, "Allocing from pool: ptr: %p, pool: %p\n", (void*)ptr, pool);
   ptr += aligned;
   ptr &= ~(aligned);

   void *ret = (void*)ptr;
   ptr += size;

   fprintf(stderr, "After alloc: ret: %p, ptr: %p\n", ret, (void*)ptr);
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
