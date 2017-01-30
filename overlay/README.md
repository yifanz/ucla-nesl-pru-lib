Install Overlays:
-----------------

```
# Activates the PRUSS
./install.sh NESL-PRU

# Assigns pins to the pinmux helper
# Use config-pins to assign pins to the PRU (see examples/gpio)
./install.sh NESL-PRU-PINS

# Only needed if you are using the QoT stack and want to do accurate time sync
./install.sh NESL-PRU-QOT

# If you ran install.sh in the past, you can do this:
./load.sh NESL-PRU
./load.sh NESL-PRU-PINS
./load.sh NESL-PRU-QOT
```

From my experience, repeatedly loading and unloading overlays may cause system instability. Often, the configuration does not get applied when you reload an overlay. In that case, try rebooting.
