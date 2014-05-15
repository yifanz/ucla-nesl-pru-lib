#include "nesl_pru_gpio.h"
#include "nesl_pru_wait.h"

unsigned int i;                  // the counter in the time delay
unsigned int delay = 588260;     // the delay (manually determined)

int main()
{
   // Just a test to show how you can use assembly instructions directly
   // subtract 1 from REG1
   __asm__ __volatile__
   (
      " SUB r1, r1, 1 \n"
   );
   __far int *shared_mem = (void*) (unsigned long) 0x10000;
   *shared_mem = 0xfeedface;

   // while the button r31.3 has not been pressed, keep looping
   while(!read_pin(P9_28)){
      toggle_pin(P9_27);
      WAIT_MS(500);
   }

   // Exiting the application - send the interrupt
   __R31 = 35;                      // PRUEVENT_0 on PRU0_R31_VEC_VALID
   __halt();                        // halt the PRU
}
