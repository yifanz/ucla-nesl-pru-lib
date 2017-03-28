### QoT and PRU Time Synchronization Accuracy

#### Pins
* __P8_46__: PRU output for time sync.
* __P8_45__: PRU input for reporting timestamp.
* __P8_9__: QoT input capture for time sync.
* __P8_10__: QoT input capture for reporting timestamp.

__P8_46__ is connected to __P8_9__ and this connection is used for time synchronization between the PRU and main processor. 
Asserting __P8_45__ causes the PRU to report its current timestamp. Similarily asserting __P8_10__, reports the current timestamp from the main processor.
__P8_45__ and __P8_10__ are connected to a PWM output from an external microcontroller which will periodically assert both pins simultaneously to produce timestamps for comparing the synchronization accuracy.

The PRU initiates time synchronization by asserting __P8_46__ and records its own timestamp at that instant (__T1__).
Since __P8_46__ is connected to __P8_9__, this will produce an input capture timestamp (__T2__) on the main processor.
__T2__ is delivered to the PRU by a user-space program and the PRU will subtract __T1__ and __T2__ to calculate the offset and adjust its own clock.

#### Results

8 test runs were performed, each lasting approximately 8 minutes.
For each run, time synchronization between the PRU and the main procesor occurs every *n* seconds.
We tested different periods of time synchronization by setting *n* from 1 second to 32 seconds.
However, in sync-once.csv, we only allow synchronization to occur once at the beginning.
Once the PRU and main processor are synchronized, we assert __P8_45__ and __P8_10__ 
every 500 milliseconds and record the timestamps from the PRU and main processor.
We use the difference between the reported timestamps as a measure for the accuracy of synchronization.

The csv files contain the timestamps from each run. The __left column is the QoT timestamp__ and the __right column is the PRU timestamp__.
All values are in nanoseconds. 

* [sync-1-sec.csv](https://github.com/yifanz/ucla-nesl-pru-lib/blob/master/examples/measure_time_sync/sync-1-sec.csv) (mean: 4973.59 ns stdev: 1125.55 ns)
* [sync-2-sec.csv](https://github.com/yifanz/ucla-nesl-pru-lib/blob/master/examples/measure_time_sync/sync-2-sec.csv) (mean: 5052.19 ns stdev: 1115.58 ns)
* [sync-4-sec.csv](https://github.com/yifanz/ucla-nesl-pru-lib/blob/master/examples/measure_time_sync/sync-4-sec.csv) (mean: 5077.75 ns stdev: 1037.35 ns)
* [sync-8-sec.csv](https://github.com/yifanz/ucla-nesl-pru-lib/blob/master/examples/measure_time_sync/sync-8-sec.csv) (mean: 5198.96 ns stdev: 1227.92 ns)
* [sync-16-sec.csv](https://github.com/yifanz/ucla-nesl-pru-lib/blob/master/examples/measure_time_sync/sync-16-sec.csv) (mean: 4945.89 ns stdev: 1718.47 ns)
* [sync-32-sec.csv](https://github.com/yifanz/ucla-nesl-pru-lib/blob/master/examples/measure_time_sync/sync-32-sec.csv) (mean: 5071.01 ns stdev: 1110.71 ns)
* [sync-once.csv](https://github.com/yifanz/ucla-nesl-pru-lib/blob/master/examples/measure_time_sync/sync-once.csv) (mean: 5032.47 ns stdev: 1023.99 ns)

In all cases, it appears that the PRU is always ~5 microseconds late.
