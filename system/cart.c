#include "cpu/memmap.h"
#include "cart.h"
#include "state.h"
#include "macros.h"
#include <assert.h>
#include <string.h>
#include "memory.h"

enum
{
   CartName    = 0x00,
   Mapper      = 0x15,
   RomType     = 0x16,
   RomSize     = 0x17,
   RamSize     = 0x18,
   CartRegion  = 0x19,
   Company     = 0x1a,
   Version     = 0x1b,
   Complement  = 0x1c,
   Checksum    = 0x1e,
   ResetVector = 0x3c,
};

// Header scoring code from bSNES.
static unsigned score_header(const uint8_t *data, size_t size, unsigned addr)
{
   if (size < addr + 64)
      return 0;

   int score = 0;

   uint16_t resetvector = data[addr + ResetVector] | (data[addr + ResetVector + 1] << 8);
   uint16_t checksum    = data[addr + Checksum   ] | (data[addr + Checksum    + 1] << 8);
   uint16_t complement  = data[addr + Complement ] | (data[addr + Complement  + 1] << 8);

   uint8_t resetop = data[(addr & ~0x7fff) | (resetvector & 0x7fff)];  //first opcode executed upon reset
   uint8_t mapper  = data[addr + Mapper] & ~0x10;                      //mask off irrelevent FastROM-capable bit

   //$00:[000-7fff] contains uninitialized RAM and MMIO.
   //reset vector must point to ROM at $00:[8000-ffff] to be considered valid.
   if (resetvector < 0x8000) return 0;

   //some images duplicate the header in multiple locations, and others have completely
   //invalid header information that cannot be relied upon.
   //below code will analyze the first opcode executed at the specified reset vector to
   //determine the probability that this is the correct header.

   //most likely opcodes
   if (resetop == 0x78  //sei
         || resetop == 0x18  //clc (clc; xce)
         || resetop == 0x38  //sec (sec; xce)
         || resetop == 0x9c  //stz $nnnn (stz $4200)
         || resetop == 0x4c  //jmp $nnnn
         || resetop == 0x5c  //jml $nnnnnn
      ) score += 8;

   //plausible opcodes
   if (resetop == 0xc2  //rep #$nn
         || resetop == 0xe2  //sep #$nn
         || resetop == 0xad  //lda $nnnn
         || resetop == 0xae  //ldx $nnnn
         || resetop == 0xac  //ldy $nnnn
         || resetop == 0xaf  //lda $nnnnnn
         || resetop == 0xa9  //lda #$nn
         || resetop == 0xa2  //ldx #$nn
         || resetop == 0xa0  //ldy #$nn
         || resetop == 0x20  //jsr $nnnn
         || resetop == 0x22  //jsl $nnnnnn
      ) score += 4;

   //implausible opcodes
   if (resetop == 0x40  //rti
         || resetop == 0x60  //rts
         || resetop == 0x6b  //rtl
         || resetop == 0xcd  //cmp $nnnn
         || resetop == 0xec  //cpx $nnnn
         || resetop == 0xcc  //cpy $nnnn
      ) score -= 4;

   //least likely opcodes
   if (resetop == 0x00  //brk #$nn
         || resetop == 0x02  //cop #$nn
         || resetop == 0xdb  //stp
         || resetop == 0x42  //wdm
         || resetop == 0xff  //sbc $nnnnnn,x
      ) score -= 8;

   //at times, both the header and reset vector's first opcode will match ...
   //fallback and rely on info validity in these cases to determine more likely header.

   //a valid checksum is the biggest indicator of a valid header.
   if ((checksum + complement) == 0xffff && (checksum != 0) && (complement != 0)) 
      score += 4;

   if(addr == 0x007fc0 && mapper == 0x20) 
      score += 2;  //0x20 is usually LoROM
   if(addr == 0x00ffc0 && mapper == 0x21) 
      score += 2;  //0x21 is usually HiROM
   if(addr == 0x007fc0 && mapper == 0x22) 
      score += 2;  //0x22 is usually ExLoROM
   if(addr == 0x40ffc0 && mapper == 0x25) 
      score += 2;  //0x25 is usually ExHiROM

   if (data[addr + Company] == 0x33) 
      score += 2;        //0x33 indicates extended header
   if (data[addr + RomType] < 0x08) 
      score++;
   if (data[addr + RomSize] < 0x10) 
      score++;
   if (data[addr + RamSize] < 0x08) 
      score++;
   if (data[addr + CartRegion] < 14) 
      score++;

   if (score < 0) 
      score = 0;
   return score;
}

static enum ssnes_mapper_type find_mapper_type(const uint8_t *data, size_t size)
{
   // One 32KiB bank, has to be LoROM.
   if (size < 0x10000)
      return SSNES_MAPPER_LOROM;

   unsigned score_lo = score_header(data, size, 0x7fc0);
   unsigned score_hi = score_header(data, size, 0xffc0);

   unsigned index = (score_lo > score_hi) ? 0x7fc0 : 0xffc0;

   if (index == 0xffc0)
      return SSNES_MAPPER_HIROM;
   else
      return SSNES_MAPPER_LOROM;
}

void ssnes_cartridge_load(const uint8_t *data, size_t size)
{
   ALLOCATE(MEM.cart.rom, 1024, 1 << 22);
   memcpy(MEM.cart.rom, data, size < (1 << 22) ? size : (1 << 22));
   ssnes_memmap_init(find_mapper_type(data, size));
}

void ssnes_cartridge_unload(void)
{}
