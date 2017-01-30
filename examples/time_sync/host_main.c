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
#include <sys/time.h>
#include <sys/timex.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#include <linux/ptp_clock.h>

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#include "nesl_pru_rbuffer.h"

#define PTP_PIN_SETFUNC    _IOW(PTP_CLK_MAGIC, 7, struct ptp_pin_desc)
struct ptp_pin_desc {
    /*
     * Hardware specific human readable pin name. This field is
     * set by the kernel during the PTP_PIN_GETFUNC ioctl and is
     * ignored for the PTP_PIN_SETFUNC ioctl.
     */
    char name[64];
    /*
     * Pin index in the range of zero to ptp_clock_caps.n_pins - 1.
     */
    unsigned int index;
    /*
     * Which of the PTP_PF_xxx functions to use on this pin.
     */
    unsigned int func;
    /*
     * The specific channel to use for this function.
     * This corresponds to the 'index' field of the
     * PTP_EXTTS_REQUEST and PTP_PEROUT_REQUEST ioctls.
     */
    unsigned int chan;
    /*
     * Reserved for future use.
     */
    unsigned int rsv[5];
};

volatile int stop = 0;
volatile uint8_t *shared_mem;

void *receive_pru_thread(void *value)
{
    do {
        prussdrv_pru_wait_event(PRU_EVTOUT_0);
        prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
        struct rbuffer *rbuf = (struct rbuffer *) shared_mem;
        short status = 0;
        uint64_t data;
        data = rbuf_read_uint64(rbuf, &status);
        if (!status) {
            uint64_t ms = data;
            ms /= 1000000ull;
            uint64_t us = data;
            us /= 1000ull;
            printf("%llu ns (%llu us, %llu ms)\n", data, us, ms);
        }
    } while (!stop);
}

void *send_pru_thread(void *value)
{
    char *device_m = "/dev/ptp1";               /* PTP device */
    int index_m = 1;                            /* Channel index, '1' corresponds to 'TIMER6' */
    int fd_m;                                   /* device file descriptor */

    int c, cnt;
    struct ptp_clock_caps caps;                 /* Clock capabilities */
    struct ptp_pin_desc desc;                   /* Pin configuration */
    struct ptp_extts_event event;               /* PTP event */
    struct ptp_extts_request extts_request;     /* External timestamp req */

    /* Open the character device */
    fd_m = open(device_m, O_RDWR);
    if (fd_m < 0) {
        fprintf(stderr, "opening device %s: %s\n", device_m, strerror(errno));
        return;
    }
    printf("Device opened %d\n", fd_m);
    memset(&desc, 0, sizeof(desc));
    desc.index = index_m;
    desc.func = 1;              // '1' corresponds to external timestamp
    desc.chan = index_m;
    if (ioctl(fd_m, PTP_PIN_SETFUNC, &desc)) {
        printf("Set pin func failed for %d\n", fd_m);
        return;
    }
    printf("Set pin func successful for %d\n", fd_m);

    // Request timestamps from the pin
    memset(&extts_request, 0, sizeof(extts_request));
    extts_request.index = index_m;
    extts_request.flags = PTP_ENABLE_FEATURE;
    if (ioctl(fd_m, PTP_EXTTS_REQUEST, &extts_request)) {
        printf("Requesting timestamps failed for %d\n", fd_m);
        return;
    }
    printf("Requesting timestamps success for %d\n", fd_m);

    struct rbuffer *rbuf = (struct rbuffer *) (shared_mem + sizeof(struct rbuffer));
    init_rbuffer(rbuf);

    // Keep checking for time stamps
    while (!stop) {
        /* Read events coming in */
        //printf("Trying to read events %d\n", running++);

        cnt = read(fd_m, &event, sizeof(event));
        if (cnt != sizeof(event)) {
            printf("Cannot read event");
            break;
        }
        uint64_t nano_ts = event.t.sec * 1000000000;
        nano_ts += event.t.nsec;
        rbuf_write_uint64(rbuf, nano_ts); // send ts to PRU
        printf("Sync Time - %lld.%09u - %llu ns\n",
                event.t.sec, event.t.nsec, nano_ts);
    }

    /* Disable the pin */
    memset(&desc, 0, sizeof(desc));
    desc.index = index_m;
    desc.func = 0;              // '0' corresponds to no function
    desc.chan = index_m;
    if (ioctl(fd_m, PTP_PIN_SETFUNC, &desc)) {
        printf("Disable pin func failed for %d\n", fd_m);
    }

    /* Close the character device */
    close(fd_m);
}

int main (void)
{
    int n, ret;

    if(getuid()!=0){
        printf("You must run this program as root. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t thread;
    pthread_t thread2;

    /* Initialize structure used by prussdrv_pruintc_intc   */
    /* PRUSS_INTC_INITDATA is found in pruss_intc_mapping.h */
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

    /* Allocate and initialize memory */
    prussdrv_init ();
    ret = prussdrv_open (PRU_EVTOUT_0);
    if(ret){
        printf("Failed to open the PRU-ICSS, have you loaded the overlay?");
        exit(EXIT_FAILURE);
    }
    ret = prussdrv_open (PRU_EVTOUT_1);
    if(ret){
        printf("Failed to open the PRU-ICSS, have you loaded the overlay?");
        exit(EXIT_FAILURE);
    }

    /* Map PRU's INTC */
    prussdrv_pruintc_init(&pruss_intc_initdata);

    // Map PRU's shared memory into user-space
    if (prussdrv_map_prumem(PRUSS0_SHARED_DATARAM, (void **) &shared_mem)) {
        printf("map shared memory failed\n");
        exit(EXIT_FAILURE);
    }

    // 12Kb of shared memory
    memset((void*) shared_mem, 0, 0x3000);

    /* Load the memory data file */
    prussdrv_load_datafile(PRU_NUM, "./data.bin");

    /* Load and execute binary on PRU */
    prussdrv_exec_program (PRU_NUM, "./text.bin");

    /* Thread for handling message from the PRU */
    if (pthread_create(&thread, NULL, &receive_pru_thread, NULL)){
        printf("Failed to create thread!\n");
        exit(EXIT_FAILURE);
    }

    /* Thread for sending messages to the PRU */
    if (pthread_create(&thread2, NULL, &send_pru_thread, NULL)){
        printf("Failed to create thread!\n");
        exit(EXIT_FAILURE);
    }

    /* Wait for event completion from PRU */
    prussdrv_pru_wait_event(PRU_EVTOUT_1);
    stop = 1;

    /* Disable PRU and close memory mappings */
    prussdrv_pru_disable(PRU_NUM);
    prussdrv_exit ();
    return(EXIT_SUCCESS);
}
