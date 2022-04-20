%code requires{

  #include "../include/ast.hpp"
  #include <cassert>
  #include <vector>
  #include <string>

  extern std::vector<const Node*> root;

  int yylex(void);
  extern void yyset_in(FILE* fd);
  void yyerror(const char *);
}

%define parse.error verbose

%union{const Node *node; std::string *string; int integer; double floater; }

%token T_INT_TYPE T_FLOAT_TYPE T_CHAR_TYPE T_VOID_TYPE T_SHORT_TYPE T_LONG_TYPE T_DOUBLE_TYPE T_SIGNED_TYPE T_UNSIGNED_TYPE
%token T_IF T_ELSE T_FOR T_WHILE T_RETURN T_CONTINUE T_BREAK T_SIZEOF T_DO T_SWITCH T_CASE T_DEFAULT T_STRUCT T_TYPEDEF T_STATIC T_EXTERN T_AUTO T_REGISTER T_ELLIPSIS
%token T_STAR T_QUESTION T_COLON T_INCREMENT T_PLUS T_DECREMENT T_MINUS T_DIVIDE 
%token T_equalequal T_starequal T_divideequal T_plusequal T_minusequal T_andequal T_orequal T_shiftrightequal T_shiftleftequal T_percentequal T_xorequal
%token T_EQUAL T_NOT_EQUAL T_NOT T_AND T_BITWISE_AND T_OR T_BITWISE_OR T_BITWISE_XOR T_MODULO
%token T_LEFT_SHIFT T_RIGHT_SHIFT T_GREATER_THAN_OR_EQUAL T_SMALLER_THAN_OR_EQUAL T_GREATER_THAN T_SMALLER_THAN
%token T_LBRACKET_ROUND T_RBRACKET_ROUND T_LBRACKET_CURLY T_RBRACKET_CURLY T_LBRACKET_SQUARE T_RBRACKET_SQUARE
%token T_SEMICOLON T_COMMA T_ENUM
%token T_NAME T_INT T_FLOAT T_STRING T_CHAR 


%type<node> TRANSLATION_UNIT EXTERNAL_DECLARATION FUNCTION_DEFINITION 
%type<node> DECLARATION_LIST DECLARATION DECLARATION_SPECIFIERS ARGUMENT_LIST PARAMETER_LIST PARAMETER_DECLARATION
%type<node> INIT_DECLARATOR_LIST INIT_DECLARATOR DECLARATOR DIRECT_DECLARATOR 
%type<node> JUMP_STATEMENT COMPOUND_STATEMENT STATEMENT_LIST STATEMENT SELECTION_STATEMENT ITERATION_STATEMENT EXPRESSION_STATEMENT CASE_DEFAULT_STATEMENT_LIST CASE_STATEMENT_LIST COMPOUND_CASE_STATEMENT CASE_STATEMENT DEFAULT_STATEMENT
%type<node> PRIMARY_EXPRESSION POSTFIX_EXPRESSION UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION ENUM_DECLARATION_LIST ENUM_DECLARATION
%type<node> ADDITIVE_EXPRESSION SHIFT_EXPRESSION RELATIONAL_EXPRESSION EQUALITY_EXPRESSION AND_EXPRESSION 
%type<node> EXCLUSIVE_OR_EXPRESSION INCLUSIVE_OR_EXPRESSION LOGICAL_AND_EXPRESSION LOGICAL_OR_EXPRESSION 
%type<node> CONDITIONAL_EXPRESSION ASSIGNMENT_EXPRESSION EXPRESSION CONSTANT_EXPRESSION INITIALIZER INITIALIZER_LIST
%type<string>  T_NAME UNARY_OPERATOR ASSIGNMENT_OPERATOR TYPE_SPECIFIER T_STRING T_CHAR
%type<integer> T_INT
%type<floater> T_FLOAT

%start TRANSLATION_UNIT
%%

TRANSLATION_UNIT
   : EXTERNAL_DECLARATION { root.push_back($1); }
   | TRANSLATION_UNIT EXTERNAL_DECLARATION { root.push_back($2); }
   ;

EXTERNAL_DECLARATION
   : FUNCTION_DEFINITION { $$ = $1; }
   | DECLARATION { $$ = $1; }
   | T_ENUM T_NAME T_LBRACKET_CURLY ENUM_DECLARATION_LIST T_RBRACKET_CURLY T_SEMICOLON { $$ = $4; }
  ;

ENUM_DECLARATION_LIST
  : ENUM_DECLARATION_LIST T_COMMA ENUM_DECLARATION  { $$ = new Enum_Declaration_List($1, $3); }
  | ENUM_DECLARATION                                { $$ = new Enum_Declaration_List(nullptr, $1); }
  ;

ENUM_DECLARATION
  : T_NAME                                       { $$ = new Enum_Declaration(*$1, nullptr); delete $1; }
  | T_NAME T_EQUAL ASSIGNMENT_EXPRESSION  { $$ = new Enum_Declaration(*$1, $3); delete $1; }
  ;

FUNCTION_DEFINITION
   : DECLARATION_SPECIFIERS DECLARATOR COMPOUND_STATEMENT { std::cerr << "FUNCTION_DEFINITION: DECLARATION_SPECIFIERS DECLARATOR COMPOUND_STATEMENT\n";    $$ = new Function_Definition($1, $2, nullptr, $3); }
   | DECLARATION_SPECIFIERS DECLARATOR T_LBRACKET_ROUND DECLARATION_LIST T_RBRACKET_ROUND COMPOUND_STATEMENT { std::cerr << "DECLARATION_SPECIFIERS DECLARATOR DECLARATION_LIST COMPOUND_STATEMENT\n"; $$ = new Function_Definition($1, $2, $4, $6) ; }
   | DECLARATOR DECLARATION_LIST COMPOUND_STATEMENT { std::cerr << "decl decllist comp_st\n"; $$ = new Function_Definition(nullptr, $1, $2, $3) ; }
   | DECLARATOR COMPOUND_STATEMENT { $$ = new Function_Definition(nullptr, $1, nullptr, $2) ; }
   ;

DECLARATION_LIST
   : DECLARATION   { std::cerr << "DECLARATION_LIST: DECLARATION\n"; $$ = new Declaration_List(nullptr,$1); }
   | DECLARATION_LIST DECLARATION  { std::cerr << "DECLARATION_LIST: DECLARATION_LIST DECLARATION\n"; $$ = new Declaration_List($1, $2); }
   ;


DECLARATION
   : DECLARATION_SPECIFIERS T_SEMICOLON { $$ = new Declarator($1, nullptr); }
   | DECLARATION_SPECIFIERS INIT_DECLARATOR_LIST T_SEMICOLON { std::cerr << "DECLARATION: DECLARATION_SPECIFIERS INIT_DECLARATOR_LIST\n"; $$ = new Declaration($1, $2); }

   ;


DECLARATION_SPECIFIERS
   : TYPE_SPECIFIER    { std::cerr << "DECLARATION_SPECIFIERS: TYPE_SPECIFIER\n"; $$ = new Declaration_Specifiers(*$1); }
//   | TYPE_SPECIFIER DECLARATION_SPECIFIERS { $$ = new Declaration_Specifiers($1, $2); }
//   | STORAGE_CLASS_SPECIFIER DECLARATION_SPECIFIERS
//   | STORAGE_CLASS_SPECIFIER
   ;

TYPE_SPECIFIER
   : T_VOID_TYPE  { $$ = new std::string("void"); }
   | T_CHAR_TYPE  { $$ = new std::string("char"); }
   | T_SHORT_TYPE  { $$ = new std::string("short"); }
   | T_INT_TYPE  { std::cerr << "TYPE_SPECIFIER: T_INT_TYPE\n"; $$ = new std::string("int"); }
   | T_LONG_TYPE  { $$ = new std::string("long"); }
   | T_FLOAT_TYPE  { $$ = new std::string("float"); }
   | T_DOUBLE_TYPE  { $$ = new std::string("double"); }
   | T_SIGNED_TYPE  { $$ = new std::string("signed"); }
   | T_UNSIGNED_TYPE  { $$ = new std::string("unsigned"); }
//   | STRUCT_SPECIFIER {$$ = $1}
//   | ENUM_SPECIFIER {$$ = $1}
//   | TYPE_NAME {$$ = $1}
   ;


INIT_DECLARATOR_LIST
   : INIT_DECLARATOR   { std::cerr << "INIT_DECLARATOR_LIST: INIT_DECLARATOR\n"; $$ = new Init_Declarator_List(nullptr, $1); }
   | INIT_DECLARATOR_LIST T_COMMA INIT_DECLARATOR { $$ = new Init_Declarator_List($1, $3); }
   ;

INIT_DECLARATOR
   : DECLARATOR T_EQUAL INITIALIZER    { std::cerr << "INIT_DECLARATOR: DECLARATOR T_EQUAL INITIALIZER\n"; $$ = new Declarator($1, $3); }
   | DECLARATOR        { std::cerr << "INIT_DECLARATOR: DECLARATOR\n"; $$ = new Declarator($1,nullptr); }
   ;

INITIALIZER
   : ASSIGNMENT_EXPRESSION { $$ = $1; } 
//   | T_LBRACKET_CURLY INITIALIZER_LIST T_RBRACKET_CURLY 
//   | T_LBRACKET_CURLY INITIALIZER_LIST T_COMMA T_RBRACKET_CURLY 
   ;

INITIALIZER_LIST
   : INITIALIZER { $$ = new Initializer_List(nullptr, $1); }
   | INITIALIZER_LIST T_COMMA INITIALIZER { $$ = new Initializer_List($1, $3); }


DECLARATOR
   : DIRECT_DECLARATOR { std::cerr << "DECLARATOR: DIRECT_DECLARATOR\n"; $$ = $1; }
   | T_STAR T_NAME {$$ = new Variable(*$2, "Pointer", nullptr); delete $2; }
   ;

DIRECT_DECLARATOR
   : T_NAME { std::cerr << "DIRECT_DECLARATOR: T_NAME\n"; $$ = new Variable(*$1, "Normal" ,nullptr); }
//   | T_LBRACKET_ROUND DECLARATOR T_RBRACKET_ROUND { $$ = new Variable( *$2, "(declarator)", nullptr); delete $2; }
   | T_NAME T_LBRACKET_SQUARE CONSTANT_EXPRESSION T_RBRACKET_SQUARE  { $$ = new Variable( *$1, "Array", $3 ); delete $1; }
   | T_NAME T_LBRACKET_SQUARE T_RBRACKET_SQUARE  { $$ = new Variable( *$1, "Array", nullptr ); delete $1; }
   | DIRECT_DECLARATOR T_LBRACKET_ROUND PARAMETER_LIST T_RBRACKET_ROUND  { std::cerr << "DIRECT_DECLARATOR: DIRECT_DECLARATOR T_LBRACKET_ROUND PARAMETER_LIST T_RBRACKET_ROUND" << std::endl; $$ = new Function_Declaration($1, $3); }
//   | DIRECT_DECLARATOR T_LBRACKET_ROUND ARGUMENT_LIST T_RBRACKET_ROUND { $$ = new Function_Call($1, $3); }
   | DIRECT_DECLARATOR T_LBRACKET_ROUND T_RBRACKET_ROUND  { $$ = new Function_Declaration($1, nullptr); }
   ;


PARAMETER_LIST
   : PARAMETER_DECLARATION { $$ = new Parameter_List(nullptr, $1); }
   | PARAMETER_LIST T_COMMA PARAMETER_DECLARATION { $$ = new Parameter_List($1, $3); }
   ;

PARAMETER_DECLARATION
   : DECLARATION_SPECIFIERS DECLARATOR {$$ = new Parameter_Declaration($1, $2); }
   ;

ARGUMENT_LIST 
   : ASSIGNMENT_EXPRESSION  { $$ = new Argument_List(nullptr, $1); }
   | ARGUMENT_LIST T_COMMA ASSIGNMENT_EXPRESSION  { $$ = new Argument_List($1, $3); }
   ;

CASE_DEFAULT_STATEMENT_LIST
  : CASE_DEFAULT_STATEMENT_LIST CASE_STATEMENT    { $$ = new Case_Statement_List($1, $2); }
  | CASE_STATEMENT_LIST DEFAULT_STATEMENT          { $$ = new Case_Statement_List($1, $2); }
  | DEFAULT_STATEMENT                               { $$ = new Case_Statement_List(nullptr, $1); }
  ;

CASE_STATEMENT_LIST
  : CASE_STATEMENT_LIST CASE_STATEMENT              { $$ = new Case_Statement_List($1, $2); }
  | CASE_STATEMENT                                  { $$ = new Case_Statement_List(nullptr, $1); }
  ;

COMPOUND_CASE_STATEMENT
  : T_LBRACKET_CURLY CASE_DEFAULT_STATEMENT_LIST T_RBRACKET_CURLY             { $$ = $2; }
  | T_LBRACKET_CURLY CASE_STATEMENT_LIST T_RBRACKET_CURLY                     { $$ = $2; }
  | T_LBRACKET_CURLY T_RBRACKET_CURLY                                        { $$ = new Case_Statement_List(nullptr, nullptr); }
  ;

CASE_STATEMENT
  : T_CASE EXPRESSION T_COLON STATEMENT_LIST              { $$ = new Case_Statement($2, $4); }
  | T_CASE EXPRESSION T_COLON                             { $$ = new Case_Statement($2, nullptr); }
  ;

DEFAULT_STATEMENT
  : T_DEFAULT T_COLON STATEMENT_LIST                      { $$ = new Default_Statement($3); }
  | T_DEFAULT T_COLON                                     { $$ = new Default_Statement(nullptr); }
  ;

COMPOUND_STATEMENT
   : T_LBRACKET_CURLY T_RBRACKET_CURLY { std::cerr << "COMPOUND_STATEMENT: { }\n"; $$ = new Compound_Statement(nullptr, nullptr); }
   | T_LBRACKET_CURLY STATEMENT_LIST T_RBRACKET_CURLY { std::cerr << "COMPOUND_STATEMENT: { STATEMENT_LIST }\n"; $$ = new Compound_Statement($2, nullptr); }
   | T_LBRACKET_CURLY DECLARATION_LIST T_RBRACKET_CURLY { std::cerr << "COMPOUND_STATEMENT: { DECLARATION_LIST }\n"; $$ = new Compound_Statement(nullptr, $2); }
   | T_LBRACKET_CURLY DECLARATION_LIST STATEMENT_LIST T_RBRACKET_CURLY { std::cerr << "COMPOUND_STATEMENT: { DECLARATION_LIST STATEMENT_LIST }\n"; $$ = new Compound_Statement($3, $2); }
   ;

STATEMENT_LIST
   : STATEMENT { $$ = new Statement_List(nullptr, $1); }
   | STATEMENT_LIST STATEMENT  { $$ = new Statement_List($1, $2); }
   ;

STATEMENT
   :  COMPOUND_STATEMENT { std::cerr << "COMPOUND_STATEMENT\n"; $$ = $1; }
   | EXPRESSION_STATEMENT { $$ = $1; }
   | SELECTION_STATEMENT { $$ = $1; } 
   | ITERATION_STATEMENT { $$ = $1; }
   | JUMP_STATEMENT { $$ = $1; }
   ; 
   
SELECTION_STATEMENT
   : T_IF T_LBRACKET_ROUND EXPRESSION T_RBRACKET_ROUND STATEMENT { std::cerr << "SELECTION_STATEMENT: T_IF T_LBRACKET_ROUND EXPRESSION T_RBRACKET_ROUND STATEMENT\n"; $$ = new If_Statement($3, $5 , nullptr); }
   | T_IF T_LBRACKET_ROUND EXPRESSION T_RBRACKET_ROUND STATEMENT T_ELSE STATEMENT { std::cerr << "SELECTION_STATEMENT: T_IF T_LBRACKET_ROUND EXPRESSION T_RBRACKET_ROUND STATEMENT T_ELSE STATEMENT\n"; $$ = new If_Statement($3, $5, $7); }
   | T_SWITCH T_LBRACKET_ROUND EXPRESSION T_RBRACKET_ROUND COMPOUND_CASE_STATEMENT { $$ = new Switch_Statement($3, $5); }
   ;

ITERATION_STATEMENT
   : T_WHILE T_LBRACKET_ROUND EXPRESSION T_RBRACKET_ROUND STATEMENT { $$ = new While_Statement($3, $5); }
//   | T_DO STATEMENT T_WHILE T_LBRACKET_ROUND EXPRESSION T_RBRACKET_ROUND T_SEMICOLON
   | T_FOR T_LBRACKET_ROUND EXPRESSION_STATEMENT EXPRESSION_STATEMENT T_RBRACKET_ROUND STATEMENT { $$ = new For_Statement($3, $4, nullptr, $6); }
   | T_FOR T_LBRACKET_ROUND EXPRESSION_STATEMENT EXPRESSION_STATEMENT EXPRESSION T_RBRACKET_ROUND STATEMENT { $$ = new For_Statement($3, $4, $5, $7); }
   ;

EXPRESSION_STATEMENT
   : T_SEMICOLON  { std::cerr << "empty expression\n"; $$ = new Empty_Expression(); }
   | EXPRESSION T_SEMICOLON { std::cerr << "expression st\n"; $$ = $1; }
   ;

JUMP_STATEMENT
   : T_RETURN T_SEMICOLON { $$ = new Return_Statement(nullptr); }
   | T_RETURN EXPRESSION T_SEMICOLON { std::cerr << "JUMP_STATEMENT T_RETURN EXPRESSION T_SEMICOLON\n"; $$ = new Return_Statement($2); }
//   | T_RETURN DIRECT_DECLARATOR T_SEMICOLON { $$ = new Return_Statement($2); }
   | T_CONTINUE T_SEMICOLON { $$ = new Continue_Statement(); }
   | T_BREAK T_SEMICOLON { $$ = new Break_Statement(); }
   ;


/*!!!!!!!!expressions incoming!!!!!!!!!!*/


UNARY_OPERATOR
   : T_BITWISE_AND { $$ = new std::string("&"); }
   | T_STAR { $$ = new std::string("*"); }
   | T_PLUS   { $$ = new std::string("+"); }
   | T_MINUS { $$ = new std::string("-"); }
//   | '~' { $$ = new std::string("~"); }
   | T_NOT  { $$ = new std::string("!"); }
   ;
   
ASSIGNMENT_OPERATOR
   : T_EQUAL  { $$ = new std::string("="); }
   | T_starequal  { $$ = new std::string("*="); }
   | T_divideequal  { $$ = new std::string("/="); }
   | T_percentequal  { $$ = new std::string("%="); }
   | T_plusequal  { $$ = new std::string("+="); }
   | T_minusequal  { $$ = new std::string("-="); }
   | T_shiftrightequal  { $$ = new std::string(">>="); }
   | T_shiftleftequal  { $$ = new std::string("<<="); }
   | T_andequal  { $$ = new std::string("&="); }
   | T_xorequal  { $$ = new std::string("^="); }
   | T_orequal  { $$ = new std::string("|="); }
   ;
   
PRIMARY_EXPRESSION
   : T_NAME { std::cerr << "PRIMARY_EXPRESSION: T_NAME\n"; $$ = new Variable(*$1, "Normal" , nullptr); delete $1; }
   | T_INT { $$ = new Int_Constant($1); }
   | T_FLOAT { $$ = new Float_Constant($1); }
//   | T_STRING { $$ = new Str_Constant($1); }
   | T_CHAR { $$ = new Char_Constant(*$1); delete $1; }
   | T_LBRACKET_ROUND EXPRESSION T_RBRACKET_ROUND { $$ = $2; }
   | T_SIZEOF T_LBRACKET_ROUND T_INT_TYPE T_RBRACKET_ROUND { int i = 4; $$ = new Int_Constant(i); }
   | T_SIZEOF T_LBRACKET_ROUND T_UNSIGNED_TYPE T_RBRACKET_ROUND { int i = 4; $$ = new Int_Constant(i); }
   | T_SIZEOF T_LBRACKET_ROUND T_CHAR_TYPE T_RBRACKET_ROUND { int i = 1; $$ = new Int_Constant(i); }
   ;
   
POSTFIX_EXPRESSION
   : PRIMARY_EXPRESSION { std::cerr << "POSTFIX_EXPRESSION: PRIMARY_EXPRESSION\n"; $$ = $1; }
   | T_NAME T_LBRACKET_SQUARE EXPRESSION T_RBRACKET_SQUARE { $$ = new Variable(*$1,"Array", $3); }
   | POSTFIX_EXPRESSION T_LBRACKET_ROUND T_RBRACKET_ROUND { $$ = new Function_Call($1, nullptr); }
   | POSTFIX_EXPRESSION T_LBRACKET_ROUND ARGUMENT_LIST T_RBRACKET_ROUND { $$ = new Function_Call($1, $3); }
//   | POSTFIX_EXPRESSION '.' T_NAME { $$ = new Postfix_Expression($1, ".", $3); }
//   | POSTFIX_EXPRESSION T_STAR T_NAME { $$ = new Postfix_Expression($1, "*", *$3); }
   | POSTFIX_EXPRESSION T_INCREMENT { $$ = new Postfix_Expression($1, "++", "nullptr"); }
   | POSTFIX_EXPRESSION T_DECREMENT { $$ = new Postfix_Expression($1, "--", "nullptr"); }
   | T_SIZEOF T_LBRACKET_ROUND PRIMARY_EXPRESSION T_RBRACKET_ROUND { $$ = new Sizeof_Expression($3); }
   ;

   
UNARY_EXPRESSION
   : POSTFIX_EXPRESSION { std::cerr << "UNARY_EXPRESSION: POSTFIX_EXPRESSION\n"; $$ = $1; }
   | T_INCREMENT UNARY_EXPRESSION { $$ = new Unary_Expression("++", $2); }
   | T_DECREMENT UNARY_EXPRESSION { $$ = new Unary_Expression("--", $2); }
   | UNARY_OPERATOR UNARY_EXPRESSION { std::cerr << "UNARY_EXPRESSION: UNARY_OPERATOR UNARY_EXPRESSION\n"; $$ = new Unary_Expression(*$1, $2);}
//   | T_SIZEOF UNARY_EXPRESSION { $$ = new Unary_Expression("sizeof", $2); }
//   | T_SIZEOF T_LBRACKET_ROUND TYPE_NAME T_RBRACKET_ROUND { $$ = new Unary_Expression("sizeof", $3); }
   ;
   
MULTIPLICATIVE_EXPRESSION
   : UNARY_EXPRESSION { std::cerr << "MULTIPLICATIVE_EXPRESSION: UNARY_EXPRESSION\n"; $$ = $1; }
   | MULTIPLICATIVE_EXPRESSION T_STAR UNARY_EXPRESSION { std::cerr << "MULTIPLICATIVE_EXPRESSION: MULTIPLICATIVE_EXPRESSION T_STAR UNARY_EXPRESSION\n"; $$ = new Multiplicative_Expression($1, "*", $3); }
   | MULTIPLICATIVE_EXPRESSION T_DIVIDE UNARY_EXPRESSION { std::cerr << "MULTIPLICATIVE_EXPRESSION: MULTIPLICATIVE_EXPRESSION T_DIVIDE UNARY_EXPRESSION\n"; $$ = new Multiplicative_Expression($1, "/", $3); }
   | MULTIPLICATIVE_EXPRESSION T_MODULO UNARY_EXPRESSION { std::cerr << "MULTIPLICATIVE_EXPRESSION: MULTIPLICATIVE_EXPRESSION T_MODULO UNARY_EXPRESSION\n"; $$ = new Multiplicative_Expression($1, "%", $3); }
   ;  

ADDITIVE_EXPRESSION
   : MULTIPLICATIVE_EXPRESSION { std::cerr << "ADDITIVE_EXPRESSION: MULTIPLICATIVE_EXPRESSION\n"; $$ = $1; }
   | ADDITIVE_EXPRESSION T_PLUS MULTIPLICATIVE_EXPRESSION { std::cerr << "ADDITIVE_EXPRESSION: ADDITIVE_EXPRESSION T_PLUS MULTIPLICATIVE_EXPRESSION\n"; $$ = new Additive_Expression($1, "+", $3); }
   | ADDITIVE_EXPRESSION T_MINUS MULTIPLICATIVE_EXPRESSION { $$ = new Additive_Expression($1, "-", $3); }
   ;

SHIFT_EXPRESSION
   : ADDITIVE_EXPRESSION { $$ = $1; }
   | SHIFT_EXPRESSION T_LEFT_SHIFT ADDITIVE_EXPRESSION { $$ = new Shift_Expression($1, "<<", $3); }
   | SHIFT_EXPRESSION T_RIGHT_SHIFT ADDITIVE_EXPRESSION { $$ = new Shift_Expression($1, ">>", $3); }
   ;

RELATIONAL_EXPRESSION
   : SHIFT_EXPRESSION { $$ = $1; }
   | RELATIONAL_EXPRESSION T_SMALLER_THAN SHIFT_EXPRESSION { $$ = new Relational_Expression($1, "<", $3); }
   | RELATIONAL_EXPRESSION T_GREATER_THAN SHIFT_EXPRESSION { $$ = new Relational_Expression($1, ">", $3); }
   | RELATIONAL_EXPRESSION T_SMALLER_THAN_OR_EQUAL SHIFT_EXPRESSION { $$ = new Relational_Expression($1, "<=", $3); }
   | RELATIONAL_EXPRESSION T_GREATER_THAN_OR_EQUAL SHIFT_EXPRESSION { $$ = new Relational_Expression($1, ">=", $3); }
   ;

EQUALITY_EXPRESSION
   : RELATIONAL_EXPRESSION { $$ = $1; }
   | EQUALITY_EXPRESSION T_equalequal RELATIONAL_EXPRESSION { $$ = new Equality_Expression($1, "==", $3); }
   | EQUALITY_EXPRESSION T_NOT_EQUAL RELATIONAL_EXPRESSION { $$ = new Equality_Expression($1, "!=", $3); }
   ;

AND_EXPRESSION
   : EQUALITY_EXPRESSION { $$ = $1; }
   | AND_EXPRESSION T_BITWISE_AND EQUALITY_EXPRESSION { $$ = new And_Expression($1, $3); }
   ;

EXCLUSIVE_OR_EXPRESSION
   : AND_EXPRESSION { $$ = $1; }
   | EXCLUSIVE_OR_EXPRESSION T_BITWISE_XOR AND_EXPRESSION { $$ = new Exclusive_Or_Expression($1, $3); }
   ;

INCLUSIVE_OR_EXPRESSION
   : EXCLUSIVE_OR_EXPRESSION { $$ = $1; }
   | INCLUSIVE_OR_EXPRESSION T_BITWISE_OR EXCLUSIVE_OR_EXPRESSION { $$ = new Inclusive_Or_Expression($1, $3); }
   ;

LOGICAL_AND_EXPRESSION
   : INCLUSIVE_OR_EXPRESSION { $$ = $1; }
   | LOGICAL_AND_EXPRESSION T_AND INCLUSIVE_OR_EXPRESSION { $$ = new Logical_And_Expression($1, $3); }
   ;

LOGICAL_OR_EXPRESSION
   : LOGICAL_AND_EXPRESSION { $$ = $1; }
   | LOGICAL_OR_EXPRESSION T_OR LOGICAL_AND_EXPRESSION  { $$ = new Logical_Or_Expression($1, $3); }
   ;

CONDITIONAL_EXPRESSION
   : LOGICAL_OR_EXPRESSION { $$ = $1; }
   | LOGICAL_OR_EXPRESSION T_QUESTION EXPRESSION T_COLON CONDITIONAL_EXPRESSION { $$ = new Conditional_Expression($1, $3, $5); }
   ;

ASSIGNMENT_EXPRESSION
   : CONDITIONAL_EXPRESSION { $$ = $1; }
   | UNARY_EXPRESSION ASSIGNMENT_OPERATOR ASSIGNMENT_EXPRESSION { std::cerr << "ASSIGNMENT_EXPRESSION: UNARY_EXPRESSION ASSIGNMENT_OPERATOR ASSIGNMENT_EXPRESSION\n"; $$ = new Assignment_Expression($1, *$2, $3); }
   ;
   
EXPRESSION
   : ASSIGNMENT_EXPRESSION { std::cerr << "EXPRESSION: ASSIGNMENT_EXPRESSION\n"; $$ = $1; }
//   | EXPRESSION T_COMMA ASSIGNMENT_EXPRESSION { $$ = new Expression($1, $3); }
   ;

CONSTANT_EXPRESSION
   : CONDITIONAL_EXPRESSION { $$ = $1; }
   ;

/*!!!!!!!!expressions ending!!!!!!!!!!*/
   
%%    
std::vector<const Node*> root;

std::vector<const Node*> parse_AST() {
  yyparse();
  return root;
}