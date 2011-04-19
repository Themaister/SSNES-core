#ifndef __CPU_MEMORY_H
#define __CPU_MEMORY_H

// No MMIO yet.
static inline uint8_t cpu_readl(uint32_t addr)
{
   return MEM.wram[addr];
}

static inline void cpu_writel(uint32_t addr, uint8_t data)
{
   MEM.wram[addr] = data;
}


static inline uint8_t cpu_read(uint16_t addr)
{
   return cpu_readl(REGS.mbk | addr);
}

static inline void cpu_write(uint16_t addr, uint8_t data)
{
   cpu_writel(REGS.mbk | addr, data);
}

static inline uint16_t cpu_readw(uint16_t addr)
{
   uint16_t res = 0;
   res |= cpu_read(addr);
   res |= ((uint16_t)cpu_read(addr + 1)) << 8;
   return res;
}

static inline void cpu_writew(uint16_t addr, uint16_t data)
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
   return cpu_readl(REGS.pc++);
}

static inline uint16_t cpu_readw_pc(void)
{
   uint16_t res = 0;
   res |= cpu_readl(REGS.pc++);
   res |= ((uint16_t)cpu_readl(REGS.pc++)) << 8;
   return res;
}

static inline uint32_t cpu_readl_pc(void)
{
   uint32_t res = 0;
   res |= cpu_readl(REGS.pc++);
   res |= ((uint16_t)cpu_readl(REGS.pc++)) << 8;
   res |= ((uint32_t)cpu_readl(REGS.pc++)) << 16;
   return res;
}

static inline uint8_t cpu_stack_pull(void)
{
   return cpu_readl(++REGS.sp.w);
}

static inline void cpu_stack_push(uint8_t data)
{
   cpu_writel(REGS.sp.w++, data);
}

// Direct page.
static inline uint8_t cpu_read_dp(uint8_t addr)
{
   return cpu_readl(REGS.dp | addr);
}

static inline uint16_t cpu_readw_dp(uint8_t addr)
{
   uint16_t res = 0;
   res |= cpu_read_dp(addr);
   res |= ((uint16_t)cpu_read_dp(addr + 1)) << 8;
   return res;
}

static inline uint32_t cpu_readl_dp(uint8_t addr)
{
   uint32_t res = 0;
   res |= cpu_read_dp(addr);
   res |= ((uint16_t)cpu_read_dp(addr + 1)) << 8;
   res |= ((uint32_t)cpu_read_dp(addr + 2)) << 16;
   return res;
}

static inline void cpu_write_dp(uint8_t addr, uint8_t data)
{
   cpu_writel(REGS.dp | addr, data);
}

static inline void cpu_writew_dp(uint8_t addr, uint16_t data)
{
   cpu_write_dp(addr, data & 0xFF);
   cpu_write_dp(addr + 1, data >> 8);
}

// Stack-relative
static inline uint8_t cpu_read_sp(uint16_t addr)
{
   return cpu_readl(REGS.sp + addr);
}

static inline uint16_t cpu_readw_sp(uint16_t addr)
{
   uint16_t res = 0;
   res |= cpu_read_sp(addr);
   res |= ((uint16_t)cpu_read_sp(addr)) << 8;
   return res;
}

static inline void cpu_write_sp(uint16_t addr, uint8_t data)
{
   cpu_writel(REGS.sp + addr, data);
}

static inline void cpu_writew_sp(uint16_t addr, uint16_t data)
{
   cpu_writel(REGS.sp + addr, data & 0xFF);
   cpu_writel(REGS.sp + addr + 1, data >> 8);
}

#endif
