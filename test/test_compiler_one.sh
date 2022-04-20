#!/bin/bash


RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

PASS=0
ERROR=0

for folder in compiler_tests/*; do
    echo "[In test folder: $folder]"
    for filename in $folder/*.c; do
        test_case=$(echo $filename | cut -d'/' -f 3 | cut -d'.' -f 1)
        test_case_sub=${test_case##*_}
        test_case_driver=${test_case_sub%.*}
        if [[ $test_case_driver != "driver" ]]; then
            echo "Testing : $test_case"
            bin/c_compiler -S ${folder}/${test_case}.c -o ${folder}/${test_case}.s 2>/dev/null
            mips-linux-gnu-gcc -mfp32 -o ${folder}/${test_case}.o -c ${folder}/${test_case}.s
            mips-linux-gnu-gcc -mfp32 -static -o ${folder}/${test_case} ${folder}/${test_case}.o ${folder}/${test_case}_driver.c
            if qemu-mips ${folder}/${test_case}; then
                echo -e "                               ${GREEN}[PASS]${NC}"
                PASS=$(( ${PASS}+1 ))
            else
                echo -e "                               ${RED}[ERROR]${NC}"
                ERROR=$(( ${ERROR}+1 ))
            fi
        fi
    done
done

echo "Passed $PASS out of $(( $PASS + $ERROR ))"


