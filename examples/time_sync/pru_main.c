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
#include <stdint.h>

cycle_t
read_cc(const struct cyclecounter *cc)
{
    return IEP_CNT;
}

int main()
{
    struct rbuffer *rbuf = (struct rbuffer *) (uint32_t) 0x10000;
    init_rbuffer(rbuf);

    struct timecounter tc = {0};
    struct cyclecounter cc = {0};

    cc.read = read_cc;
    cc.mask = CLOCKSOURCE_MASK(32);
    cc.mult = 5;
    cc.shift = 0;

    DISABLE_IEP_TMR();
    ENABLE_IEP_TMR();
    IEP_CNT = 0;

    timecounter_init(&tc, &cc, IEP_CNT);

    int i = 30;
    while(i--) {
        rbuf_write_uint64(rbuf, timecounter_read(&tc));
        TRIG_INTC(3); // Trigger interrupt PRUEVENT_0
        WAIT_MS(1000);
    }

    DISABLE_IEP_TMR();

    // Exiting the application - send the interrupt
    TRIG_INTC(4); // Trigger interrupt PRUEVENT_1
    __halt(); // halt the PRU
}
