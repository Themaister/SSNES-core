#ifndef __CPU_MEMORY_H
#define __CPU_MEMORY_H

#include "cpu.h"
#include "system/state.h"
#include "system/macros.h"
#include "system/util.h"
#include "memmap.h"

#include <assert.h>

// Somewhat memory mapped ...
static inline uint8_t cpu_readl(uint32_t addr)
{
   if (addr >= 0x1000000)
   {
      fprintf(stderr, "PC = %x, addr = %x, wram = %x\n", (unsigned)REGS.pc.l, addr, STATUS.regs.wram_addr.l);
      assert(0);
   }

   addr &= 0xffffff;
   uint8_t result = ssnes_memmap_read_table[addr >> 13](addr);
   dprintf("\tRead: $%02x <= $%06x\n", (unsigned)result, (unsigned)addr);
   return result;

   //return ssnes_memmap_read_table[addr >> 13](addr);
}

static inline void cpu_writel(uint32_t addr, uint8_t data)
{
   dprintf("\tWrite: $%02x => $%06x\n", (unsigned)data, (unsigned)addr);
   ssnes_memmap_write_table[addr >> 13](addr, data);
}

// Mostly just to avoid debugging noise ... ;)
static inline uint8_t cpu_readl_ndbg(uint32_t addr)
{
   return ssnes_memmap_read_table[addr >> 13](addr);
}

static inline void cpu_writel_ndbg(uint32_t addr, uint8_t data)
{
   ssnes_memmap_write_table[addr >> 13](addr, data);
}

static inline uint8_t cpu_read(uint32_t addr)
{
   return cpu_readl((REGS.db + addr) & 0xffffff);
}

static inline void cpu_write(uint32_t addr, uint8_t data)
{
   cpu_writel((REGS.db + addr) & 0xffffff, data);
}

static inline uint16_t cpu_readw(uint32_t addr)
{
   uint16_t res = 0;
   res |= cpu_read(addr);
   res |= ((uint16_t)cpu_read(addr + 1)) << 8;
   return res;
}

static inline void cpu_writew(uint32_t addr, uint16_t data)
{
   cpu_write(addr, data & 0xFF);
   cpu_write(addr + 1, data >> 8);
}

static inline uint16_t cpu_readlw(uint32_t addr)
{
   uint16_t res = 0;
   res |= cpu_readl(addr);
   res |= ((uint16_t)cpu_readl(addr + 1)) << 8;
   return res;
}

static inline void cpu_writelw(uint32_t addr, uint16_t data)
{
   cpu_writel(addr, data & 0xFF);
   cpu_writel(addr + 1, data >> 8);
}

static inline uint8_t cpu_read_pc(void)
{
   uint8_t ret = cpu_readl_ndbg(REGS.pc.l);
   REGS.pc.w.l++;
   return ret;
}

static inline uint16_t cpu_readw_pc(void)
{
   long_reg_t pc = REGS.pc;
   uint16_t res = 0;
   res |= cpu_readl_ndbg(pc.l);
   pc.w.l++;
   res |= ((uint16_t)cpu_readl_ndbg(pc.l)) << 8;
   pc.w.l++;
   REGS.pc = pc;
   return res;
}

static inline uint32_t cpu_readl_pc(void)
{
   long_reg_t pc = REGS.pc;
   uint32_t res = 0;
   res |= cpu_readl_ndbg(pc.l);
   pc.w.l++;
   res |= ((uint16_t)cpu_readl_ndbg(pc.l)) << 8;
   pc.w.l++;
   res |= ((uint32_t)cpu_readl_ndbg(pc.l)) << 16;
   pc.w.l++;
   REGS.pc = pc;
   return res;
}

static inline uint8_t cpu_stack_pull(void)
{
   return cpu_readl(++REGS.sp.w);
}

static inline void cpu_stack_push(uint8_t data)
{
   cpu_writel(REGS.sp.w--, data);
}

// Direct page.
static inline uint8_t cpu_read_dp(uint16_t addr)
{
   return cpu_readl((REGS.dp + addr) & 0xffff);
}

static inline uint16_t cpu_readw_dp(uint16_t addr)
{
   uint16_t res = 0;
   res |= cpu_read_dp(addr);
   res |= ((uint16_t)cpu_read_dp(addr + 1)) << 8;
   return res;
}

static inline uint32_t cpu_readl_dp(uint16_t addr)
{
   uint32_t res = 0;
   res |= cpu_read_dp(addr);
   res |= ((uint32_t)cpu_read_dp(addr + 1)) << 8;
   res |= ((uint32_t)cpu_read_dp(addr + 2)) << 16;
   return res;
}

static inline void cpu_write_dp(uint16_t addr, uint8_t data)
{
   cpu_writel((REGS.dp + addr) & 0xffff, data);
}

static inline void cpu_writew_dp(uint16_t addr, uint16_t data)
{
   cpu_write_dp(addr, data & 0xFF);
   cpu_write_dp(addr + 1, data >> 8);
}

// Stack-relative
static inline uint8_t cpu_read_sp(uint16_t addr)
{
   return cpu_readl((REGS.sp.w + addr) & 0xffff);
}

static inline uint16_t cpu_readw_sp(uint16_t addr)
{
   uint16_t res = 0;
   res |= cpu_read_sp(addr);
   res |= ((uint16_t)cpu_read_sp(addr + 1)) << 8;
   return res;
}

static inline void cpu_write_sp(uint16_t addr, uint8_t data)
{
   cpu_writel((REGS.sp.w + addr) & 0xffff, data);
}

static inline void cpu_writew_sp(uint16_t addr, uint16_t data)
{
   cpu_writel((REGS.sp.w + addr) & 0xffff, data & 0xff);
   cpu_writel((REGS.sp.w + addr + 1) & 0xffff, data >> 8);
}

#endif
