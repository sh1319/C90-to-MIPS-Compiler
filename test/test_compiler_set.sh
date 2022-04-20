#!/bin/bash


folder=$1
test_case=$2

bin/c_compiler -S ${folder}/${test_case}.c -o ${folder}/${test_case}.s 2>/dev/null
mips-linux-gnu-gcc -mfp32 -o ${folder}/${test_case}.o -c ${folder}/${test_case}.s
mips-linux-gnu-gcc -mfp32 -static -o ${folder}/${test_case} ${folder}/${test_case}.o ${folder}/${test_case}_driver.c
echo "Testing : $test_case"
if qemu-mips ${folder}/${test_case}; then
    echo "                          [PASS]"
else
    echo "                          [ERROR]"
fi



