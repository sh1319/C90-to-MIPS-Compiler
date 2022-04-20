#!/bin/bash

flex -o src/lexer.yy.cpp src/lexer.flex
g++ src/lexer.yy.cpp -o bin/lexer
./bin/lexer < test/test_lexer.txt