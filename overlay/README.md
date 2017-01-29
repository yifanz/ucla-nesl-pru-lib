Install Overlays:
-----------------

```
# Activates the PRUSS
./install.sh NESL-PRU

# Assigns pins to the pinmux helper
# Use config-pins to assign pins to the PRU (see examples/gpio)
./install.sh NESL-PRU-PINS

# Only if you want to do time sync
./install.sh NESL-PRU-QOT
```

From my experience, repeatedly loading and unloading overlays may cause system instability. Often, the configuration does not get applied when you reload an overlay. In that case, try rebooting.
