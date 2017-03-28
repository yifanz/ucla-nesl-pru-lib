### Code Comparison

PASM PWM example
```
```

Cyclops high-level language PWM example (https://github.com/yifanz/CSM213A).
```
// PWM with 1 second period and pulse width of 0.5 seconds.
while(P8_45 == 0) {
    // Each tick is 5 nanoseconds.
    tick = 0;
    
    // Output High
    P8_46 = 1;
    
    // Wait for 0.5 seconds (2 * 10^8 cycles)
    while (tick < 0xBEBC200) {}
    
    tick = 0;
    
    // Output Low
    P8_46 = 0;
    
    // Wait for 0.5 seconds (2 * 10^8 cycles)
    while (tick < 0xBEBC200) {}
}
```
