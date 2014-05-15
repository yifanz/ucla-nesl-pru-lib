/*
 * Copyright (c) Regents of the University of California, 2017. All rights reserved.
 * See LICENSE and ATTRIB in the repository root.
 */
#ifndef NESL_PRU_WAIT_H
#define NESL_PRU_WAIT_H

#define WAIT_MS(ms) __delay_cycles(ms / 5 * 1000000);
#define WAIT_US(us) __delay_cycles(us / 5 * 1000);
#define WAIT_CYCLES(cycles) __delay_cycles(cycles);

#endif
