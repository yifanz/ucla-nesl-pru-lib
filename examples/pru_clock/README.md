### PRU Clock

```
# Login as the root user
ssh root@<beaglebone IP>

cd ~

# load the QoT device tree and kernel modules
./load_QOT_DT_modules

# Activate the PRUSS and PINS for PRU1
cd ucla-nesl-pru-lib/overlay
./install.sh NESL-PRU
./install.sh NESL-PRU1-PINS

# Build the pru_clock example
cd ../examples/pru_clock
./build.sh

# Run the pru_clock
cd gen
./host

# Now you can run whatever you want on PRU0.
# The PRU1 timestamp is continuously updated
# in the PRU shared memory region.
# By default this will be at 0x11400.
```

You may redefine where in memory the timestamp is located by editing [shared_conf.h](https://github.com/yifanz/ucla-nesl-pru-lib/blob/master/examples/pru_clock/shared_conf.h).
