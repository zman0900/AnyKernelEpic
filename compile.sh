#!/bin/bash

TOOLCHAIN=/mnt/data/gcc-linaro-arm-linux-gnueabihf-2012.09-20120921_linux/bin
TOOLCHAIN_PREFIX=arm-linux-gnueabihf-

$TOOLCHAIN/$TOOLCHAIN_PREFIX'gcc' -static -O3 -o zip/replace_kernel replace_kernel.c
$TOOLCHAIN/$TOOLCHAIN_PREFIX'strip' -s zip/replace_kernel

