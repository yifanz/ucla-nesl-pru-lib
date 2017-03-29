/*
 * Copyright (c) Regents of the University of California, 2017. All rights reserved.
 * See LICENSE and ATTRIB in the repository root.
 */

/*
 * Based on code from Derek Molloy for the book "Exploring BeagleBone:
 * Tools and Techniques for Building with Embedded Linux" by John Wiley & Sons, * 2014 ISBN 9781118935125. Please see the file ATTRIB in the repository root * directory for copyright and GNU GPLv3 license information.
 */

#include "nesl_pru_intc.h"
#include "nesl_pru_iep.h"
#include "nesl_pru_wait.h"
#include "nesl_pru_rbuffer.h"
#include "nesl_pru_time.h"
#include "nesl_pru_ticks.h"
#include "nesl_pru_gpio.h"
#include <stdint.h>
#include <stdlib.h>

void
terminate()
{
    TRIG_INTC(4); // Trigger interrupt PRUEVENT_1
    __halt(); // halt the PRU
}

cycle_t
read_cc(const struct cyclecounter *cc)
{
    return IEP_CNT;
}

int main()
{
    uint32_t *shared_mem = (void*) (unsigned long) 0x10000;

    ENABLE_TICKS();

    // IEP is our clock source
    DISABLE_IEP_TMR();
    ENABLE_IEP_TMR();
    IEP_CNT = 0;

    struct pru_time time;

    // Initialize pru_time with IEP as the source
    // slew_cc is optional
    init_pru_time(&time, 5, 0, 32, read_cc, NULL);

    unsigned int i = 1000;
    while(i--) {
        TICKS = 0;
        //u64 ts_pru = 0;
        u64 ts_pru = read_pru_time(&time);
        //s64 delta = adj_pru_time(&time, ts_pru, ts_pru + 100);
        *shared_mem = TICKS;
        shared_mem++;
    }

    DISABLE_IEP_TMR();

    // Exiting the application
    terminate();
}
