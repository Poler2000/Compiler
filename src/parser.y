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
extern unsigned int lines;

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

program:        VAR declarations T_BEGIN commands END { compiler.finish(); }
                | T_BEGIN commands END { compiler.finish(); }
                ;

declarations:   declarations COMMA declare { /*nothing to do*/ }
                | declare { /*nothing to do*/ }
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
                    compiler.assert_array_range(*($1.id), $3.val, $5.val, $1.line);
                    Array* var = new Array(*($1.id), $3.val, $5.val);
                    compiler.get_var_manager().declare(var);
                }
                ;

commands:      commands command { /*nothing to do*/ }
                | command { /*nothing to do*/ }
                ;

command:       identifier ASSIGN expression SEMICOLON {
                }
                | IF condition THEN commands ELSE commands END_IF {; }
                | IF condition THEN commands END_IF {; }
                | WHILE condition DO commands END_WHILE {; }
                | REPEAT commands UNTIL condition SEMICOLON {; }
                | start_for end_for
                | FOR VARIABLE FROM value DOWN_TO value DO commands END_FOR {; }
                | READ identifier SEMICOLON {
                    LValue* val = dynamic_cast<LValue*>($2);
                    compiler.get_code_generator().read(*val);
                 }
                | WRITE value SEMICOLON {
                    compiler.assert_initialized(*$2, $1.line);
                    compiler.get_code_generator().write(*$2);
                 }
                ;

start_for:      FOR VARIABLE FROM value TO value {
                    printf("new variable declaration: %s\n", $2.id->c_str());
                    compiler.assert_variable_not_declared(*($2.id), $2.line);

                    LValue* var = new LValue(*($2.id));
                    compiler.get_var_manager().declare(var);

                    std::string counterLabel = *($2.id) + "-counter";
                    LValue* counter = new LValue(counterLabel);
                    printf("new variable declaration: %s\n", counterLabel.c_str());
                }
                ;

end_for:        commands END_FOR {
                    //compiler.get_var_manager().undeclare(iterator);
                    //compiler.get_var_manager().undeclare(counter);
                }
                ;

expression:    value {
                    compiler.assert_initialized(*$1, lines);
                }
                | value PLUS value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().add(*$1, *$3);
                }
                | value MINUS value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().sub(*$1, *$3);
                }
                | value TIMES value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().mul(*$1, *$3);
                }
                | value DIV value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().div(*$1, *$3);
                }
                | value MOD value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().mod(*$1, *$3);
                }
                ;

condition:     value EQ value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().eq(*$1, *$3);
                }
                | value NEQ value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().neq(*$1, *$3);
                }
                | value LE value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().le(*$1, *$3);
                }
                | value GE value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().ge(*$1, *$3);
                }
                | value LEQ value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().leq(*$1, *$3);
                }
                | value GEQ value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().geq(*$1, *$3);
                }
                ;

value:          NUM  { $$ = new RValue($1.val); }
                | identifier  { /*nothing tod do*/ }
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

void yyerror (const char *s) {fprintf (stderr, "Error: %s in line: %d\n", s, lines);}

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