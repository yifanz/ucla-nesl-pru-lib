#ifndef NESL_PRU_REGS_H
#define NESL_PRU_REGS_H

#include <stdint.h>

volatile register unsigned int __R31, __R30;

static uint32_t *CTRL_REG = (uint32_t*) 0x22000;
static uint32_t *CYCLE_CNT_REG = (uint32_t*) 0x2200C;

#define ENABLE_TICKS() *CTRL_REG |= 1 << 3; *CYCLE_CNT_REG = 0
#define TICKS (*CYCLE_CNT_REG)

#endif
