#!/bin/bash

mkdir -p bin

flex -o src/lexer.yy.cpp src/lexer.flex
bison -v -d src/parser.y -o src/parser.tab.cpp
g++ src/lexer.yy.cpp src/parser.tab.cpp -o bin/parser -std=c++0x
./bin/parser < test/test_parser.txt