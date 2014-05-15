#include "nesl_pru_regs.h"
#include "nesl_pru_wait.h"
#include <stdint.h>

int main()
{
    __far uint32_t *shared_mem = (void*) (unsigned long) 0x10000;

    ENABLE_TICKS();
    TICKS = 0;
    
    *shared_mem = TICKS;
    WAIT_CYCLES(2);
    *(shared_mem+1) = TICKS;
    WAIT_CYCLES(100000);
    *(shared_mem+2) = TICKS;

    // Exiting the application - send the interrupt
    __R31 = 35; // PRUEVENT_0 on PRU0_R31_VEC_VALID
    __halt(); // halt the PRU
}
