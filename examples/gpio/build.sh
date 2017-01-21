#!/bin/bash

PRU=0; # which PRU 0 or 1
PRU_SDK=/root/pru_2.0.0B2
PRU_SRC="pru_main.c"
HOST_SRC="host_main.c"
PRU_TARGET=pru.out
HOST_TARGET=host
GEN=gen
SCRIPT_DIR=${0%/*}

cd $SCRIPT_DIR
mkdir -p $GEN

echo "Compiling and linking the PRU firmware"
$PRU_SDK/bin/clpru --silicon_version=3 --keep_asm --c_src_interlist \
    --asm_directory=$GEN --obj_directory=$GEN --temp_directory=$GEN \
    -DPRU=$PRU \
    $PRU_SRC \
    -i$PRU_SDK/include \
    -i$PRU_SDK/lib \
    -i../../include \
    -z AM3359_PRU.cmd \
    -o $GEN/$PRU_TARGET \
    -m $GEN/$PRU_TARGET.map

pushd .
cd $GEN

# The output of the compiler/linker is an ELF executable.
# We cannot directly load the ELF file into the PRU because unlike Linux,
# the PRU does not have a ELF loader.
# In theory, we could parse the ELF file in our host application,
# but that is too much work.
# Instead, we break out the text and data segments and so that we can
# manually map them into PRU memory.
echo "Generating text and data segments (text.bin and data.bin)"
$PRU_SDK/bin/hexpru ../bin.cmd $PRU_TARGET
popd

echo "Compiling the host application"
gcc $HOST_SRC -o $GEN/$HOST_TARGET -lpthread -lprussdrv
