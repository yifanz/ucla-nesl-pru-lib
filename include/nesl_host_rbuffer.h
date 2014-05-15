/*
 * Copyright (c) Regents of the University of California, 2017. All rights reserved.
 * See LICENSE and ATTRIB in the repository root.
 */
#ifndef NESL_HOST_RBUFFER_H
#define NESL_HOST_RBUFFER_H

#include <stdint.h>
#include <string.h>
#include "nesl_common_rbuffer.h"

struct rbuffer *
init_rbuffer(uint32_t addr)
{
    struct rbuffer *rbuf = (void*) (unsigned long) addr;
    memset(rbuf, 0, sizeof(struct rbuffer));
    rbuf->c = 0;
    rbuf->p = 1;
}

int
rbuf_write_int32(struct rbuffer *rbuf, uint32_t data)
{
    uint16_t p = rbuf->p;
    if (p != rbuf->c) {
        rbuf->buffer[p] = data;
        rbuf->p = RBUF_IDX(p+1);
        return 0;
    }
    // else buffer is full
    return -1;
}

uint32_t
rbuf_read_int32(struct rbuffer *rbuf, int *status)
{
    uint16_t c = RBUF_IDX(rbuf->c+1);
    if (c != rbuf->p) {
        uint32_t data = rbuf->buffer[c];
        rbuf->c = c;
        if (status != NULL) *status = 0;
        return data;
    }
    // no new data
    if (status != NULL) *status = -1;
    return 0;
}
#endif
