%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stack>

#include <rvalue.h>
#include <compiler.h>

extern int yylex();
void yyerror(const char* s);
extern FILE *yyin;

static Compiler compiler;

%}

%code requires
{
    #include <string>
    #include <cstdlib>
    #include <rvalue.h>
    typedef struct Token
    {
        int val;
        unsigned int line;
        std::string* id;
    } Token;
}

%union
{
    Token token;
    Value* value;
}


%start program
%token EQ NEQ LE GE LEQ GEQ
%token PLUS MINUS TIMES DIV MOD
%token ASSIGN
%token FOR FROM TO DO DOWN_TO END_FOR
%token WHILE END_WHILE
%token REPEAT UNTIL
%token READ WRITE
%token VAR T_BEGIN END
%token IF THEN ELSE END_IF
%token SEMICOLON COMMA
%token VARIABLE NUM
%token LEFT_BRACKET ARRAY_RNG RIGHT_BRACKET
%token ERROR

%type <token> EQ NEQ LE GE LEQ GEQ
%type <token> PLUS MINUS TIMES DIV MOD
%type <token> ASSIGN
%type <token> FOR FROM TO DO DOWN_TO END_FOR
%type <token> WHILE END_WHILE
%type <token> REPEAT UNTIL
%type <token> READ WRITE
%type <token> VAR T_BEGIN END
%type <token> IF THEN ELSE END_IF
%type <token> SEMICOLON COMMA
%type <token> VARIABLE NUM
%type <token> LEFT_BRACKET ARRAY_RNG RIGHT_BRACKET
%type <token> ERROR

%type <value> value identifier

%%

program:        VAR declarations T_BEGIN commands END {; }
                | T_BEGIN commands END {; }
                ;

declarations:   declarations COMMA declare {; }
                | declare {; }
                ;

declare:        VARIABLE {
                    compiler.assert_variable_not_declared($1);

                    compiler.get_var_manager().declare($1);
                }
                | VARIABLE LEFT_BRACKET ARRAY_RNG RIGHT_BRACKET {;}
                ;

commands:      commands command {; }
                | command {; }
                ;

command:       identifier ASSIGN expression SEMICOLON {; }
                | IF condition THEN commands ELSE commands END_IF {; }
                | IF condition THEN commands END_IF {; }
                | WHILE condition DO commands END_WHILE {; }
                | REPEAT commands UNTIL condition SEMICOLON {; }
                | FOR VARIABLE FROM value TO value DO commands END_FOR {; }
                | FOR VARIABLE FROM value DOWN_TO value DO commands END_FOR {; }
                | READ identifier SEMICOLON {; }
                | WRITE value SEMICOLON {; }
                ;

expression:    value {; }
                | value PLUS value {; }
                | value MINUS value {; }
                | value TIMES value {; }
                | value DIV value {; }
                | value MOD value {; }
                ;

condition:     value EQ value {; }
                | value NEQ value {; }
                | value LE value {; }
                | value GE value {; }
                | value LEQ value {; }
                | value GEQ value {; }
                ;

value:          NUM  { $$ = new RValue($1.val); }
                | identifier  { ; }
                ;

identifier:     VARIABLE {; }
                | VARIABLE LEFT_BRACKET VARIABLE RIGHT_BRACKET {; }
                | VARIABLE LEFT_BRACKET NUM RIGHT_BRACKET {; }

%%

void yyerror (const char *s) {fprintf (stderr, "Error: %s\n", s);} 

int compile(const char* input, const char* output) {
    yyin = fopen(input, "r");
    int result = yyparse();
    fclose(yyin);

    return result;
}