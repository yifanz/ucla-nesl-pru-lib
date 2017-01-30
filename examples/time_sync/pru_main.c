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

cycle_t
read_cc(const struct cyclecounter *cc)
{
    return IEP_CNT;
}

int main()
{
    struct rbuffer *send_buf = (struct rbuffer *) (uint32_t) 0x10000;
    init_rbuffer(send_buf);

    struct rbuffer *rec_buf = (struct rbuffer *) (uint32_t) (0x10000 + sizeof(struct rbuffer));

    struct timecounter tc = {0};
    struct cyclecounter cc = {0};

    cc.read = read_cc;
    cc.mask = CLOCKSOURCE_MASK(32);
    cc.mult = 5;
    cc.shift = 0;

    short status = -1;
    uint64_t data = 0;

    WAIT_MS(2000);

    DISABLE_IEP_TMR();
    ENABLE_IEP_TMR();
    IEP_CNT = 0;

    timecounter_init(&tc, &cc, IEP_CNT);

    int i = 30;
    while(i--) {
        if (i % 5 == 0) {
            u64 ts_pru = timecounter_read(&tc);
            assert_pin(P9_27);
            WAIT_US(10);
            deassert_pin(P9_27);

            uint64_t ts_host = 0;

            do {
                data = rbuf_read_uint64(rec_buf, &status);
            } while(status);

            ts_host = data;

            // discard any other messages
            /*
            do {
                data = rbuf_read_uint64(rec_buf, &status);
            } while(!status);
            */

            // send time the pin was asserted
            rbuf_write_uint64(send_buf, ts_host);

            s64 delta = 0;
            if (ts_host > ts_pru) {
                delta = ts_host - ts_pru;
            } else if (ts_host < ts_pru) {
                delta = ts_pru - ts_host;
                delta = -delta;
            }
            timecounter_adjtime(&tc, delta);

            // send back delta
            rbuf_write_uint64(send_buf, delta);
        }

        // send current ts
        rbuf_write_uint64(send_buf, timecounter_read(&tc));
        TRIG_INTC(3); // Trigger interrupt PRUEVENT_0
        WAIT_MS(1000);
    }

    DISABLE_IEP_TMR();

    // Exiting the application - send the interrupt
    TRIG_INTC(4); // Trigger interrupt PRUEVENT_1
    __halt(); // halt the PRU
}
