#include "nesl_pru_intc.h"
#include "nesl_pru_gpio.h"
#include "nesl_pru_wait.h"

int main()
{
    // Set pin to high
    assert_pin(P9_27);

    // Blink pin P9_27 every half of a second
    // Stop when P9_28 is set high
    while(!read_pin(P9_28)){
        toggle_pin(P9_27);
        WAIT_MS(500);
    }

    // Set pin to low before we exit
    deassert_pin(P9_27);

    // Exiting the application - send the interrupt
    TRIG_INTC(3); // PRUEVENT_0 on PRU0_R31_VEC_VALID
    __halt(); // halt the PRU
}
