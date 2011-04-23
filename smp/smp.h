#ifndef __SMP_H
#define __SMP_H

struct smp_state
{
   char foo;
};

void ssnes_smp_init(void);
void ssnes_smp_deinit(void);

#endif
