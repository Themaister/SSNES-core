#ifndef __CPU_BUS_H
#define __CPU_BUS_H

#include <stdint.h>

uint8_t ssnes_bus_read_2000(uint32_t addr);
uint8_t ssnes_bus_read_4000(uint32_t addr);
uint8_t ssnes_bus_read_6000(uint32_t addr);
void ssnes_bus_write_2000(uint32_t addr, uint8_t write);
void ssnes_bus_write_4000(uint32_t addr, uint8_t write);
void ssnes_bus_write_6000(uint32_t addr, uint8_t write);

#endif
