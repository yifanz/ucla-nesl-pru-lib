/*
 * Copyright (c) Regents of the University of California, 2017. All rights reserved.
 * See LICENSE and ATTRIB in the repository root.
 */
#ifndef NESL_PRU_RBUFFER_H
#define NESL_PRU_RBUFFER_H

#include <stdint.h>
#include <string.h>

#define RBUF_SIZE 64
#define RBUF_IDX(i) (i % RBUF_SIZE)

/*
 * Generic ring buffer implementation
 * Create two if you want bi-directional communication
 */
struct rbuffer
{
    uint16_t c;
    uint16_t p;
    uint32_t buffer[RBUF_SIZE];
};

void
init_rbuffer(struct rbuffer *rbuf)
{
    memset(rbuf, 0, sizeof(struct rbuffer));
    rbuf->c = 0;
    rbuf->p = 1;
}

short
rbuf_write_uint32(struct rbuffer *rbuf, uint32_t data)
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

short
rbuf_write_uint64(struct rbuffer *rbuf, uint64_t data)
{
    short status = rbuf_write_uint32(rbuf, data);
    if (!status) status = rbuf_write_uint32(rbuf, data >> 32);

    return status;
}

uint32_t
rbuf_read_uint32(struct rbuffer *rbuf, short *status)
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

uint64_t
rbuf_read_uint64(struct rbuffer *rbuf, short *status)
{
    uint64_t data;
    uint32_t lower = rbuf_read_uint32(rbuf, status);
    uint32_t upper;

    if (!*status) {
        upper = rbuf_read_uint32(rbuf, status);
    }

    if (*status) return 0;

    data = upper;
    data = data << 32;
    data |= lower;

    return data;
}

#endif
