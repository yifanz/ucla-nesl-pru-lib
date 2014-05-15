/*
 * Copyright (c) Regents of the University of California, 2017. All rights reserved.
 * See LICENSE and ATTRIB in the repository root.
 */
#ifndef NESL_PRU_IEP_H
#define NESL_PRU_IEP_H

#include <stdint.h>

static uint32_t *IEP_REG = (uint32_t*) 0x2E000;
static uint32_t *IEP_TMR_CNT = (uint32_t*) 0x2E00C;

#define ENABLE_IEP_TMR() (*IEP_REG |= 0x111)
#define DISABLE_IEP_TMR() (*IEP_REG &= 0)
#define IEP_CNT (*IEP_TMR_CNT)

#endif
