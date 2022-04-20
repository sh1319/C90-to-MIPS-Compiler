%option noyywrap

%{
// Avoid error "error: `fileno' was not declared in this scope"
extern "C" int fileno(FILE *stream);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.hpp"

int line_number = 0;
%}

%%

"int"						{ /*fprintf(stdout, "int\n");*/ return T_INT_TYPE; }
"float"					{ /*fprintf(stdout, "float\n");*/ return T_FLOAT_TYPE; }
"char"          { /*fprintf(stdout, "float\n");*/ return T_CHAR_TYPE; }
"void"					{ /*fprintf(stdout, "void\n");*/ return T_VOID_TYPE; }
"short"					{ /*fprintf(stdout, "short\n");*/ return T_SHORT_TYPE; }
"long"					{ /*fprintf(stdout, "long\n");*/ return T_LONG_TYPE; }
"double"					{ /*fprintf(stdout, "double\n");*/ return T_DOUBLE_TYPE; }
"signed"					{ /*fprintf(stdout, "signed\n");*/ return T_SIGNED_TYPE; }
"unsigned"					{ /*fprintf(stdout, "unsigned\n");*/ return T_UNSIGNED_TYPE; }


"if"					  { /*fprintf(stdout, "if\n");*/ return T_IF; }
"else"					  { /*fprintf(stdout, "else\n");*/ return T_ELSE; }
"while"					{ /*fprintf(stdout, "while\n");*/ return T_WHILE; }
"for"					{ /*fprintf(stdout, "for\n");*/ return T_FOR; }
"return"        { /*fprintf(stdout, "return\n");*/ return T_RETURN;}
"break"        { /*fprintf(stdout, "break\n");*/ return T_BREAK;}
"continue"        { /*fprintf(stdout, "continue\n");*/ return T_CONTINUE;}
"sizeof"        { /*fprintf(stdout, "sizeof\n");*/ return T_SIZEOF; }
"do"					{ /*fprintf(stdout, "do\n");*/ return T_DO; }
"switch"					{ /*fprintf(stdout, "switch\n");*/ return T_SWITCH; }
"case"					{ /*fprintf(stdout, "case\n");*/ return T_CASE; }
"default"					{ /*fprintf(stdout, "default\n");*/ return T_DEFAULT; }
"struct"					{ /*fprintf(stdout, "struct\n");*/ return T_STRUCT; }
"typedef"					{ /*fprintf(stdout, "typedef\n");*/ return T_TYPEDEF; }
"static"					{ /*fprintf(stdout, "static\n");*/ return T_STATIC; }
"extern"					{ /*fprintf(stdout, "extern\n");*/ return T_EXTERN; }
"auto"					{ /*fprintf(stdout, "auto\n");*/ return T_AUTO; }
"register"					{ /*fprintf(stdout, "register\n");*/ return T_REGISTER; }
"enum"      { /*fprintf(stdout, "enum\n");*/ return T_ENUM; }
"..."					{ /*fprintf(stdout, "ellipsis\n");*/ return T_ELLIPSIS; }


"*="					{ /*fprintf(stdout, "starequal\n");*/ return T_starequal ;}
"*"            	{ /*fprintf(stdout, "mult or pointer\n");*/ return T_STAR; }
"?"				{ /*fprintf(stdout, "question mark\n");*/ return T_QUESTION; }
":"				{ /*fprintf(stdout, "colon\n");*/ return T_COLON; }
"++"						{ /*fprintf(stdout, "increment\n");*/ return T_INCREMENT; }
"+="						{ /*fprintf(stdout, "plusequal\n");*/ return T_plusequal ;}
"+"             { /*fprintf(stdout, "plus\n");*/ return T_PLUS; }
"--"						{ /*fprintf(stdout, "decrement\n");*/ return T_DECREMENT; }
"-="						{ /*fprintf(stdout, "minusequal\n");*/ return T_minusequal ;}
"-"             { /*fprintf(stdout, "minus\n");*/ return T_MINUS; }
"/="						{ /*fprintf(stdout, "divideequal\n");*/ return T_divideequal ;}
"/"					  	{ /*fprintf(stdout, "divide\n");*/ return T_DIVIDE; }
"=="						{ /*fprintf(stdout, "equalequal\n");*/ return T_equalequal ;}
"="							{ /*fprintf(stdout, "equal\n");*/ return T_EQUAL; }
"!="						{ /*fprintf(stdout, "not equal\n");*/ return T_NOT_EQUAL; }
"!"							{ /*fprintf(stdout, "not\n");*/ return T_NOT; }
"&&" 						{ /*fprintf(stdout, "and\n");*/ return T_AND; }
"&="						{ /*fprintf(stdout, "andequal\n");*/ return T_andequal ;}
"&"							{ /*fprintf(stdout, "bitwise and\n");*/ return T_BITWISE_AND; }
"||"						{ /*fprintf(stdout, "or\n");*/ return T_OR; }
"|="						{ /*fprintf(stdout, "orequal\n");*/ return T_orequal ;}
"|"							{ /*fprintf(stdout, "bitwise or\n");*/ return T_BITWISE_OR; }
"^="						{ /*fprintf(stdout, "xorequal\n");*/ return T_xorequal ;}
"^"	  					{ /*fprintf(stdout, "xor\n");*/ return T_BITWISE_XOR; }
"%="						{ /*fprintf(stdout, "percentequal\n");*/ return T_percentequal ;}
"%" 						{ /*fprintf(stdout, "mod\n");*/ return T_MODULO; }
"<<="						{ /*fprintf(stdout, "shiftrightequal\n");*/ return T_shiftrightequal ;}
"<<"						{ /*fprintf(stdout, "shift right\n");*/ return T_LEFT_SHIFT; }
">>="						{ /*fprintf(stdout, "shiftleftequal\n");*/ return T_shiftleftequal ;}
">>"						{ /*fprintf(stdout, "shift left\n");*/ return T_RIGHT_SHIFT; }
">="						{ /*fprintf(stdout, "greater or equal\n");*/ return T_GREATER_THAN_OR_EQUAL; }
"<="						{ /*fprintf(stdout, "smaller or equal\n");*/ return T_SMALLER_THAN_OR_EQUAL; }
">"							{ /*fprintf(stdout, "greater than\n");*/ return T_GREATER_THAN; }
"<"							{ /*fprintf(stdout, "smaller than\n");*/ return T_SMALLER_THAN; }
"("             { /*fprintf(stdout, "round lbracket\n");*/ return T_LBRACKET_ROUND; }
")"             { /*fprintf(stdout, "round rbracket\n");*/ return T_RBRACKET_ROUND; }
"{"             { /*fprintf(stdout, "curly lbracket\n");*/ return T_LBRACKET_CURLY; }
"}"             { /*fprintf(stdout, "curly rbracket\n");*/ return T_RBRACKET_CURLY; }
"["		{ /*fprintf(stdout, "square rbracket\n");*/ return T_LBRACKET_SQUARE; }
"]"		{ /*fprintf(stdout, "square rbracket\n");*/ return T_RBRACKET_SQUARE; }
";" 						{ /*fprintf(stdout, "semicolon\n");*/ return T_SEMICOLON; }
","             { /*fprintf(stdout, "comma\n");*/ return T_COMMA; }

[_a-zA-Z]{1}([a-zA-Z]|[0-9]|[_])*			{ /*fprintf(stdout, "name\n");*/ yylval.string=new std::string(yytext); return T_NAME; }

[0-9]+                    { /*fprintf(stdout, "integer\n");*/ yylval.integer=atoi(yytext); return T_INT; }
[1-9][0-9]*((u|U|l|L)*)?                    { /*fprintf(stdout, "integer\n");*/ yylval.integer=atoi(yytext); return T_INT; }
0[xX][a-fA-F0-9]+((u|U|l|L)*)?                { /*fprintf(stdout, "integer_hex\n");*/ yylval.integer = (int)std::strtol(yytext, NULL, 0); return T_INT; }
0[0-7]*((u|U|l|L)*)?                    { /*fprintf(stdout, "integer_oct\n");*/ yylval.integer=atoi(yytext); return T_INT; }
0[bB][0-1]*((u|U|l|L)*)?                    { /*fprintf(stdout, "integer_bin\n");*/ std::string str = std::string(yytext); std::string sub = str.substr(2,str.size()-2); char *cstr = const_cast<char *>(sub.c_str()); yylval.integer=std::stoi(cstr,0,2); return T_INT; }
[0-9]+([.][0-9]*)?                    { /*fprintf(stdout, "floater\n");*/ yylval.floater=atof(yytext); return T_FLOAT; }

["][a-zA-Z]+["]                      { /*fprintf(stdout, "string\n");*/ yylval.string=new std::string(yytext); return T_STRING; }
['][a-zA-Z]+[']                      { /*fprintf(stdout, "string\n");*/ yylval.string=new std::string(yytext); return T_CHAR; }


[\n]            {line_number++;}
[ \t\r\b]+			
.               { /*fprintf(stdout, "Invalid token\n");*/ }

%%

void yyerror (char const *s)
{
  fprintf (stderr, "Parse error %s:\n\t at Line Number [ %d ] for token [ %s ]\n", s, line_number, yytext);
}