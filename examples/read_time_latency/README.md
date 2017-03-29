### Measure latency for reading/calculating PRU timestamp

Measure the latency for `read_pru_time()`, `adj_pru_time()` and reading the IEP counter.

PRU is 200 MHz so 1 cycle is 5 ns.
There is no cache or pipeline in the PRU architecture so every instruction (as long as it does not access main processor resources) executes deterministically. 
In this test, it appears that all functions are completely deterministic and they take the same number of cycles to execute everytime.

* `read_pru_time()` = 720 cycles = 3600 ns
* `adj_pru_time()` = 109 cycles = 545 ns
* Reading IEP counter (32-bit) = 23 cycles = 115 ns

`read_pru_time()` is used to get the current PRU timestamp. It includes reading the IEP counter and handles roll over checking.
It takes so many cycles because the PRU registers are natively 32-bit so it is more costly for it to do 64-bit operations in order to maintaining the 64-bit timestamp.
The function also operates on C structures which cannot be held entirely in registers and accessing data memory costs 3-4 times more cycles than register access.

`adj_pru_time()` is a simpler function used to adjust the current PRU timestamp based on it's offset with respect to an external timestamp.
It does not read the IEP counter.
