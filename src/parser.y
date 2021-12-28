%{
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stack>

#include <rvalue.h>
#include <array.h>
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
                    printf("new variable declaration: %s\n", $1.id->c_str());
                    compiler.assert_variable_not_declared(*($1.id), $1.line);
                    LValue* var = new LValue(*($1.id));
                    compiler.get_var_manager().declare(var);
                }
                | VARIABLE LEFT_BRACKET NUM ARRAY_RNG NUM RIGHT_BRACKET {
                    printf("new array declaration: %s\n", $1.id->c_str());
                    compiler.assert_variable_not_declared(*($1.id), $1.line);
                    Array* var = new Array(*($1.id), $3.val, $3.val);
                    compiler.get_var_manager().declare(var);
                }
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
                | identifier  { }
                ;

identifier:     VARIABLE {
                    compiler.assert_variable_declared(*($1.id), $1.line);
                    compiler.assert_type(*($1.id), Value::ValueType::TYPE_VAR, $1.line);

                    LValue* var = compiler.get_var_manager().get(*($1.id)).get();
                    $$ = var;
                }
                | VARIABLE LEFT_BRACKET VARIABLE RIGHT_BRACKET {
                    compiler.assert_variable_declared(*($1.id), $1.line);
                    compiler.assert_variable_declared(*($3.id), $3.line);
                    compiler.assert_initialized(*($3.id), $3.line);

                    compiler.assert_type(*($1.id), Value::ValueType::TYPE_ARRAY, $1.line);
                    compiler.assert_type(*($3.id), Value::ValueType::TYPE_VAR, $3.line);

                   Array* var = dynamic_cast<Array*>(compiler.get_var_manager().get(*($1.id)).get());

                   LValue* index = new LValue(*($3.id));
                   var->set_current(index);

                   $$ = var;
                }
                | VARIABLE LEFT_BRACKET NUM RIGHT_BRACKET {
                    compiler.assert_variable_declared(*($1.id), $1.line);

                    compiler.assert_type(*($1.id), Value::ValueType::TYPE_ARRAY, $1.line);

                   Array* var = dynamic_cast<Array*>(compiler.get_var_manager().get(*($1.id)).get());

                   Value* num = new RValue($3.val);
                   var->set_current(num);

                   $$ = var;
                 }

%%

void yyerror (const char *s) {fprintf (stderr, "Error: %s\n", s);} 

int compile(const char* input, const char* output) {
    yyin = fopen(input, "r");
    int result = yyparse();
    fclose(yyin);

    std::ofstream outFile;

    outFile.open(output);

    auto code = compiler.get_code_generator().generate_asm_code();

    outFile << code;

    return result;
}