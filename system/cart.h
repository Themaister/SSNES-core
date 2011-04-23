#ifndef __CART_H
#define __CART_H

#include <stdint.h>
#include <stddef.h>

void ssnes_cartridge_load(const uint8_t* data, size_t size);
void ssnes_cartridge_unload(void);

#endif
