#!/bin/bash

make clean
make bin/print_ast

./bin/print_ast <test.txt