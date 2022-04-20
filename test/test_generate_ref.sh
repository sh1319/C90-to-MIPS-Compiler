#!/bin/bash


for folder in compiler_tests/*; do
    echo "[In test folder: $folder]"
    for filename in $folder/*.c; do
        test_case=$(echo $filename | cut -d'/' -f 3 | cut -d'.' -f 1)
        test_case_sub=${test_case##*_}
        test_case_driver=${test_case_sub%.*}
        if [[ $test_case_driver != "driver" ]]; then
            echo "Generating reference assembly and program for $test_case"
            #bin/c_compiler -S test_ADD0.c -o test_ADD0.s
            mips-linux-gnu-gcc -S ${folder}/${test_case}.c -o ${folder}/${test_case}_ref.s
            mips-linux-gnu-gcc -mfp32 -o ${folder}/${test_case}_ref.o -c ${folder}/${test_case}_ref.s
            mips-linux-gnu-gcc -mfp32 -static -o ${folder}/${test_case}_ref ${folder}/${test_case}_ref.o ${folder}/${test_case}_driver.c
        fi
    done
done


