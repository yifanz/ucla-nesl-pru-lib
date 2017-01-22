#ifndef NESL_PRU_INTC_H
#define NESL_PRU_INTC_H

#include <stdint.h>
#include "nesl_pru_regs.h"

#define TRIG_INTC(int_num) (__R31 = (1<<5) | int_num)

#endif
