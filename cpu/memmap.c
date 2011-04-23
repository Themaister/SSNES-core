#include "memmap.h"
#include "cpu_state.h"
#include "state.h"
#include <stdio.h>
#include <assert.h>

memmap_read_t ssnes_memmap_read_table[256 * 8];
memmap_write_t ssnes_memmap_write_table[256 * 8];


static uint8_t lorom_read(uint32_t addr)
{
   uint8_t ret = MEM.cart.rom[((addr & 0x3f0000) >> 1) | (0x7fff & addr)];
   //fprintf(stderr, "Read 0x%x from addr 0x%x.\n", (unsigned)ret, (unsigned)addr);
   CPU.status.cycles += 8; // SlowROM
   return ret;
}

static uint8_t lorom_read_fastrom(uint32_t addr)
{
   CPU.status.cycles += 6; // FastROM (needs to be 6 or 8 depending ...)
   return MEM.cart.rom[((addr & 0x3f0000) >> 1) | (0x7fff & addr)];
}

// ;)
static void lorom_write(uint32_t addr, uint8_t data)
{
   //fprintf(stderr, "HEY! Attempted to write 0x%x to addr: %x\n", (unsigned)data, (unsigned)addr);
   (void)addr;
   (void)data;
}

// ;)
static void lorom_write_fastrom(uint32_t addr, uint8_t data)
{
   //fprintf(stderr, "HEY! Attempted to write 0x%x to addr: %x\n", (unsigned)data, (unsigned)addr);
   (void)addr;
   (void)data;
}

static uint8_t hirom_read(uint32_t addr)
{
   CPU.status.cycles += 8; // SlowROM
   return MEM.cart.rom[addr & 0x1fffff];
}

static uint8_t hirom_read_fastrom(uint32_t addr)
{
   CPU.status.cycles += 6; // FastROM (needs to be 6 or 8 depending ...)
   return MEM.cart.rom[addr & 0x1fffff];
}

// ;)
static void hirom_write(uint32_t addr, uint8_t data)
{
   //fprintf(stderr, "HEY! Attempted to write 0x%x to addr: %x\n", (unsigned)data, (unsigned)addr);
   (void)addr;
   (void)data;
}

// ;)
static void hirom_write_fastrom(uint32_t addr, uint8_t data)
{
   //fprintf(stderr, "HEY! Attempted to write 0x%x to addr: %x\n", (unsigned)data, (unsigned)addr);
   (void)addr;
   (void)data;
}

static uint8_t lowram_read(uint32_t addr)
{
   uint8_t ret = MEM.wram[addr & 0x1fff];
   //fprintf(stderr, "Read 0x%x from addr 0x%x.\n", (unsigned)ret, (unsigned)addr);
   CPU.status.cycles += 8;
   return ret;
}

static void lowram_write(uint32_t addr, uint8_t data)
{
   //fprintf(stderr, "Wrote 0x%x to 0x%x.\n", (unsigned)data, (unsigned)addr);
   CPU.status.cycles += 8;
   MEM.wram[addr & 0x1fff] = data;
}

static uint8_t wram_read(uint32_t addr)
{
   uint8_t ret = MEM.wram[addr & 0x1ffff];
   CPU.status.cycles += 8;
   //fprintf(stderr, "Reading 0x%x from addr 0x%x.\n", (unsigned)ret, (unsigned)addr);
   return ret;
}

static void wram_write(uint32_t addr, uint8_t data)
{
   CPU.status.cycles += 8;
   MEM.wram[addr & 0x1ffff] = data;
   //fprintf(stderr, "Writing 0x%x to addr 0x%x.\n", (unsigned)data, (unsigned)addr);
}

static void dummy_write(uint32_t addr, uint8_t data)
{
   CPU.status.cycles += 6;
   //fprintf(stderr, "Dummy write of 0x%x to addr 0x%x.\n", (unsigned)data, (unsigned)addr);
}

static uint8_t dummy_read(uint32_t addr)
{
   CPU.status.cycles += 6;
   //fprintf(stderr, "Dummy read from addr 0x%x.\n", (unsigned)addr);
   return 0;
}

// Sets up whole memory map.
static void init_memmap(
      memmap_read_t rom_read, memmap_write_t rom_write, 
      memmap_read_t rom_read_fast, memmap_write_t rom_write_fast)
{
   // Banks 0x00 - 0x3f || 0x80 - 0xbf
   for (unsigned i = 0; i <= 0x3f; i++)
   {
      // $0000 - $1fff - Scratch RAM
      ssnes_memmap_read_table[i << 3] = lowram_read;
      ssnes_memmap_read_table[(i + 0x80) << 3] = lowram_read;
      ssnes_memmap_write_table[i << 3] = lowram_write;
      ssnes_memmap_write_table[(i + 0x80) << 3] = lowram_write;

      // $2000 - $7fff - Registers and shit, but we don't care atm.
      for (unsigned j = 1; j < 4; j++)
      {
         ssnes_memmap_read_table[(i << 3) + j] = dummy_read;
         ssnes_memmap_read_table[((i + 0x80) << 3) + j] = dummy_read;
         ssnes_memmap_write_table[(i << 3) + j] = dummy_write;
         ssnes_memmap_write_table[((i + 0x80) << 3) + j] = dummy_write;
      }

      // $8000 - $ffff - ROM
      for (unsigned j = 4; j < 8; j++)
      {
         ssnes_memmap_read_table[(i << 3) + j] = rom_read;
         ssnes_memmap_read_table[((i + 0x80) << 3) + j] = rom_read_fast;
         ssnes_memmap_write_table[(i << 3) + j] = rom_write;
         ssnes_memmap_write_table[((i + 0x80) << 3) + j] = rom_write_fast;
      }
   }

   for (unsigned i = 0x40; i <= 0x7d; i++)
   {
      for (unsigned j = 0; j < 8; j++)
      {
         ssnes_memmap_read_table[(i << 3) + j] = rom_read;
         ssnes_memmap_read_table[((i + 0x80) << 3) + j] = rom_read_fast;
         ssnes_memmap_write_table[(i << 3) + j] = rom_write;
         ssnes_memmap_write_table[((i + 0x80) << 3) + j] = rom_write_fast;
      }
   }

   for (unsigned i = 0x7e; i <= 0x7f; i++)
   {
      for (unsigned j = 0; j < 8; j++)
      {
         ssnes_memmap_read_table[(i << 3) + j] = wram_read;
         ssnes_memmap_write_table[(i << 3) + j] = wram_write;
      }
   }

   for (unsigned i = 0xfe; i <= 0xff; i++)
   {
      for (unsigned j = 0; j < 8; j++)
      {
         ssnes_memmap_read_table[(i << 3) + j] = rom_read_fast;
         ssnes_memmap_write_table[(i << 3) + j] = rom_write_fast;
      }
   }
}

void ssnes_memmap_init(enum ssnes_mapper_type type)
{
   memmap_read_t rom_read = NULL;
   memmap_write_t rom_write = NULL;
   memmap_read_t rom_read_fast = NULL;
   memmap_write_t rom_write_fast = NULL;

   switch (type)
   {
      case SSNES_MAPPER_LOROM:
         rom_read = lorom_read;
         rom_write = lorom_write;
         rom_read_fast = lorom_read_fastrom;
         rom_write_fast = lorom_write_fastrom;
         break;

      case SSNES_MAPPER_HIROM:
         rom_read = hirom_read;
         rom_write = hirom_write;
         rom_read_fast = hirom_read_fastrom;
         rom_write_fast = hirom_write_fastrom;
         break;

      default:
         //fprintf(stderr, "No valid MMAP?!?!\n");
         break;
   }

   assert(rom_read);
   assert(rom_write);
   assert(rom_read_fast);
   assert(rom_write_fast);

   init_memmap(rom_read, rom_write, rom_read_fast, rom_write_fast);
}
