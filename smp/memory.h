#ifndef __SMP_MEMORY_H
#define __SMP_MEMORY_H

static inline uint8_t smp_read_pc(void)
{
   return MEM.apuram[SMP.pc++];
}


#endif
