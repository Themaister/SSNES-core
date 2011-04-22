#ifndef __CART_H
#define __CART_H

#include "memmap.h"
#include <stdint.h>
#include <stddef.h>

void snes_cartridge_load(const uint8_t* data, size_t size);
void snes_cartridge_unload(void);

#endif
