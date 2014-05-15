/*
 * Copyright (c) Regents of the University of California, 2017. All rights reserved.
 * See LICENSE and ATTRIB in the repository root.
 */
#ifndef NESL_COMMON_RBUFFER_H
#define NESL_COMMON_RBUFFER_H

#include <stdint.h>

#define RBUF_SIZE 64
#define RBUF_IDX(i) (i % RBUF_SIZE)

struct rbuffer
{
    uint16_t c;
    uint16_t p;
    uint32_t buffer[RBUF_SIZE];
};



#endif
