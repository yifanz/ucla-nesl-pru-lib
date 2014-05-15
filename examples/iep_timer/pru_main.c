/*
 * Copyright (c) Regents of the University of California, 2017. All rights reserved.
 * See LICENSE and ATTRIB in the repository root.
 */

/*
 * Based on code from Derek Molloy for the book "Exploring BeagleBone:
 * Tools and Techniques for Building with Embedded Linux" by John Wiley & Sons,
 * 2014 ISBN 9781118935125. Please see the file ATTRIB in the repository root
 * directory for copyright and GNU GPLv3 license information.
 */

#include "nesl_pru_intc.h"
#include "nesl_pru_iep.h"
#include "nesl_pru_wait.h"
#include <stdint.h>

int main()
{
    __far uint32_t *shared_mem = (void*) (unsigned long) 0x10000;

    DISABLE_IEP_TMR();
    ENABLE_IEP_TMR();

    IEP_CNT = 0;
    *shared_mem = IEP_CNT;
    WAIT_CYCLES(2);
    *(shared_mem+1) = IEP_CNT;
    WAIT_CYCLES(100000);
    *(shared_mem+2) = IEP_CNT;

    DISABLE_IEP_TMR();

    // Exiting the application - send the interrupt
    TRIG_INTC(3); // Trigger interrupt PRUEVENT_0
    __halt(); // halt the PRU
}
