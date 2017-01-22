#include "nesl_pru_intc.h"
#include "nesl_pru_iep.h"
#include "nesl_pru_wait.h"
#include <stdint.h>

int main()
{
    __far uint32_t *shared_mem = (void*) (unsigned long) 0x10000;

    DISABLE_IEP_TMR();
    ENABLE_IEP_TMR();

    IEP_CNT = 0;
    *shared_mem = IEP_CNT;
    WAIT_CYCLES(2);
    *(shared_mem+1) = IEP_CNT;
    WAIT_CYCLES(100000);
    *(shared_mem+2) = IEP_CNT;

    DISABLE_IEP_TMR();

    // Exiting the application - send the interrupt
    TRIG_INTC(3); // Trigger interrupt PRUEVENT_0
    __halt(); // halt the PRU
}
