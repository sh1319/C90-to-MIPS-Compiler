  
CPPFLAGS += -std=c++0x -W -Wall -g -fpermissive -Wno-unused-parameter -I inc

bin/c_compiler : src/lexer.yy.o src/parser.tab.o src/compiler.o
	$(shell mkdir -p bin)
	g++ ${CPPFLAGS} src/lexer.yy.cpp src/parser.tab.cpp src/compiler.cpp -o bin/c_compiler

bin/print_ast : src/lexer.yy.o src/parser.tab.o src/ast_print.o
	g++ ${CPPFLAGS} src/lexer.yy.cpp src/parser.tab.cpp src/ast_print.cpp -o bin/print_ast

bin/parser : src/lexer.yy.o src/parser.tab.o
	g++ ${CPPFLAGS} src/lexer.yy.cpp src/parser.tab.cpp -o bin/parser

src/parser.tab.cpp src/parser.tab.hpp : src/parser.y
	bison -v -d $< -o src/parser.tab.cpp

bin/lexer : src/lexer.yy.o ../include/lookup_table.hpp
	g++ ${CPPFLAGS} src/lexer.yy.cpp -o bin/lexer

src/lexer.yy.cpp : src/lexer.flex src/parser.tab.hpp      
	flex -o $@ $<

clean :
	@echo Cleaning bin/ ...
	rm -f bin/*
	@echo Cleaning src/ ...
	rm -f src/*.tab.cpp
	rm -f src/*.tab.hpp
	rm -f src/*.output
	rm -f src/*.yy.cpp
	rm -f src/*.o