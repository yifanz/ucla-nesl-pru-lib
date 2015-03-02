/*
 * Copyright (c) Regents of the University of California, 2017. All rights reserved.
 * See LICENSE and ATTRIB in the repository root.
 */

/*
 * Based on helloworld_capture.c, a simple C example showing how to register
 * to listen for capture events by Fatima Anwar and Andrew Symington.
 */

/*
 * Based on code from Derek Molloy for the book "Exploring BeagleBone:
 * Tools and Techniques for Building with Embedded Linux" by John Wiley & Sons,
 * 2014 ISBN 9781118935125. Please see the file ATTRIB in the repository root
 * directory for copyright and GNU GPLv3 license information.
 */

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "host_qot.h"

int main (void)
{
    int err;

    if(getuid()!=0){
        printf("You must run this program as root. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    // Setup QoT
    if (init_qot("/dev/ptp1", 0)) {
        printf("Initialize QoT time sync failed\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        int channel;
        uint64_t nano_ts = qot_read_event_ts(&err, &channel);
    }

    // Teardown QoT
    if (deinit_qot()) {
        printf("Deinitialize QoT time sync failed\n");
    }

    return(EXIT_SUCCESS);
}
