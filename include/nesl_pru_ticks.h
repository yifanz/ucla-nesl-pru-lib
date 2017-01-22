#ifndef NESL_PRU_TICKS_H
#define NESL_PRU_TICKS_H

#include <stdint.h>
#include "nesl_pru_regs.h"

static uint32_t *CYCLE_CNT_REG = (uint32_t*) 0x2200C;

#define ENABLE_TICKS() *CTRL_REG |= 1 << 3; *CYCLE_CNT_REG = 0
#define TICKS (*CYCLE_CNT_REG)

#endif
