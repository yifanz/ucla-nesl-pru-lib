#ifndef NESL_PRU_REGS_H
#define NESL_PRU_REGS_H

#include <stdint.h>

volatile register unsigned int __R31, __R30;

static uint32_t *CTRL_REG = (uint32_t*) 0x22000;

#define HWREG(x) (*((volatile unsigned int *)(x)))

// PRU Module Registers
#define PRU_ICSS_CFG 0x26000
#define PRU_ICSS_CFG_SYSCFG 0x04

// Enable OCP so we can access the whole memory map for the
// device from the PRU. Clear bit 4 of SYSCFG register
void init_ocp()
{
    HWREG(PRU_ICSS_CFG + PRU_ICSS_CFG_SYSCFG) &= ~(1 << 4);
}

#endif
