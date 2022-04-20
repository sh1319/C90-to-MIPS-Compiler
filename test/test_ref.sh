#!/bin/bash

for folder in compiler_tests/*; do
    echo "Running programs generated as ref... [$folder]"
    for filename in $folder/*.c; do
        test_case=$(echo $filename | cut -d'/' -f 3 | cut -d'.' -f 1)
        test_case_sub=${test_case##*_}
        test_case_driver=${test_case_sub%.*}
        if [[ $test_case_driver != "driver" ]]; then
            if qemu-mips ${folder}/${test_case}_ref; then
                echo "  Testing ${test_case}: [Worked]"
            else
                echo "  Testing ${test_case}: [Error]"
            fi
        fi
    done
done