#include "nesl_pru_regs.h"
#include "nesl_pru_wait.h"
#include "nesl_pru_intc.h"
#include "nesl_pru_adc.h"
#include <stdint.h>

int main()
{
    init_ocp();

    init_adc();
    adc_start_sampling();
    wait_for_adc();

    __far uint32_t *shared_mem = (void*) (unsigned long) 0x10000;
    unsigned int data, i, step_id, value;
    unsigned int count = read_adc_count();

    for(i=0; i<count; i++){
        data = read_adc_data();
        step_id = (data & (0x000f0000)) >> 16;
        value = (data & 0xfff);
        shared_mem[i*2] = step_id;
        shared_mem[i*2+1] = value;
    }

    // Exiting the application - send the interrupt
    TRIG_INTC(3); // Trigger interrupt PRUEVENT_0
    __halt(); // halt the PRU
}
