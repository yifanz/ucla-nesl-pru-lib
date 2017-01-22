#ifndef NESL_PRU_REGS_H
#define NESL_PRU_REGS_H

#include <stdint.h>

volatile register unsigned int __R31, __R30;

static uint32_t *CTRL_REG = (uint32_t*) 0x22000;

#endif
