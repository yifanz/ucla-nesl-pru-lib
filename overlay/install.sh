#!/bin/bash

NAME=NESL-PRU
DTBO=$NAME-00A0.dtbo
DTS=$NAME.dts

cd ${0%/*}

echo "Compiling the overlay from .dts to .dtbo"

dtc -O dtb -o $DTBO -b 0 -@ $DTS

if [ $? -eq 0 ]; then
    echo "Installing into /lib/firmware"
    cp $DTBO /lib/firmware
fi

if [ $? -eq 0 ]; then
    echo "Activating PRU overlay"
    echo $NAME > /sys/devices/bone_capemgr.9/slots
fi

if cat /sys/devices/bone_capemgr.9/slots | grep -q cape-universal; then
    echo "Activating cape-universal overlay"
    config-pin overlay cape-universal
fi
