#ifndef __SSNES_MEMORY_H
#define __SSNES_MEMORY_H

#include <stddef.h>

void ssnes_pool_init(size_t size);
void* ssnes_pool_alloc(unsigned align, size_t size);
void ssnes_pool_free(void);

#endif
