#!/bin/bash
rm -rf bin
rm -rf compiler_test_results

mkdir -p bin
mkdir -p compiler_test_results

make clean
make bin/print_ast

for type in array control_flow default float functions integer local_var misc pointer programs string struct types ; do

	TESTS="compiler_tests/${type}/*.c"
	mkdir -p compiler_test_results/${type}

	>&2 echo "Commencing ${type} tests."	
	
	for t in $TESTS; do
		TESTCASE="$t"
		base="$(basename -- $TESTCASE)"
		ex="${base%.*}"
		test_case_sub=${ex##*_}
        if [[ $test_case_sub != "driver" ]]; then
			>&2 echo "$t"
			touch compiler_test_results/${type}/${test_case_sub}_result.txt 
			./bin/print_ast <${t} 1>compiler_test_results/${type}/${test_case_sub}_result.txt 
        fi
	done
done