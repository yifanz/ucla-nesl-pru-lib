/*
 * Copyright (c) Regents of the University of California, 2017. All rights reserved.
 * See LICENSE and ATTRIB in the repository root.
 */

/*
 * Based on timecounter and clocksource from the Linux kernel v4.1
 * See ATTRIB for GPLv2 licensing information.
 */

#ifndef NESL_PRU_TIME_H
#define NESL_PRU_TIME_H

#include <stdint.h>

#define CLOCKSOURCE_MASK(bits) (cycle_t)((bits) < 64 ? ((1ULL<<(bits))-1) : -1)

typedef uint64_t u64;
typedef uint32_t u32;
typedef u64 cycle_t;

struct cyclecounter
{
    cycle_t (*read)(const struct cyclecounter *cc);
    cycle_t mask;
    u32 mult;
    u32 shift;
};

struct timecounter
{
    const struct cyclecounter *cc;
    cycle_t cycle_last;
    u64 nsec;
    u64 mask;
    u64 frac;
};

void
timecounter_init(struct timecounter *tc,
        const struct cyclecounter *cc,
        u64 start_tstamp)
{
    tc->cc = cc;
    tc->cycle_last = cc->read(cc);
    tc->nsec = start_tstamp;
    tc->mask = (1ULL << cc->shift) - 1;
    tc->frac = 0;
}

static inline u64
cyclecounter_cyc2ns(const struct cyclecounter *cc,
        cycle_t cycles, u64 mask, u64 *frac)
{
    u64 ns = (u64) cycles;

    ns = (ns * cc->mult) + *frac;
    *frac = ns & mask;
    return ns >> cc->shift;
}

static u64
timecounter_read_delta(struct timecounter *tc)
{
    cycle_t cycle_now, cycle_delta;
    u64 ns_offset;

    /* read cycle counter: */
    cycle_now = tc->cc->read(tc->cc);

    /* calculate the delta since the last timecounter_read_delta(): */
    cycle_delta = (cycle_now - tc->cycle_last) & tc->cc->mask;

    /* convert to nanoseconds: */
    ns_offset = cyclecounter_cyc2ns(tc->cc, cycle_delta,
            tc->mask, &tc->frac);

    /* update time stamp of timecounter_read_delta() call: */
    tc->cycle_last = cycle_now;

    return ns_offset;
}

u64
timecounter_read(struct timecounter *tc)
{
    u64 nsec;

    /* increment time by nanoseconds since last call */
    nsec = timecounter_read_delta(tc);
    nsec += tc->nsec;
    tc->nsec = nsec;

    return nsec;
}

#endif
