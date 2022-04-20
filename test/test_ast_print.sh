#!/bin/bash

flex -o src/lexer.yy.cpp src/lexer.flex
bison -v -d src/parser.y -o src/parser.tab.cpp
g++ src/lexer.yy.cpp -o src/lexer.yy.o
g++ src/lexer.yy.o src/parser.tab.cpp src/ast_print.cpp -o bin/parser -std=c++0x
./bin/parser < test/test_parser.txt