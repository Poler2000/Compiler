%{
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stack>

#include <rvalue.h>
#include <array.h>
#include <compiler.h>
#include <util.h>

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

command:        identifier ASSIGN expression SEMICOLON {
                    printf("assign\n");
                    compiler.assert_variable_mutable(*($1), $2.line);
                    compiler.get_code_generator().assign(*$1);
                    compiler.initialize_variable(Util::to_mut_lvalue(*$1));
                }
                | start_if start_else end_if {; }
                | start_if end_if {; }
                | start_while while_cond end_while {;}
                | start_repeat end_repeat {;}
                | start_for end_for {;}
                | start_down_to end_down_to {;}
                | READ identifier SEMICOLON {
                    LValue* val = dynamic_cast<LValue*>($2);
                    compiler.get_code_generator().read(*val);
                    printf("Read\n");
                    compiler.initialize_variable(*val);
                    printf("Read\n");

                 }
                | WRITE value SEMICOLON {
                    printf("Write\n");
                    compiler.assert_initialized(*$2, $1.line);
                    compiler.get_code_generator().write(*$2);
                 }
                ;

start_if:        IF condition THEN {
                    printf("start if\n");
                    compiler.get_code_generator().start_if();
                }
                ;

start_else:      commands ELSE {
                    printf("start else\n");
                    compiler.get_code_generator().start_else();
                 }
                 ;

end_if:          commands END_IF {
                    printf("end if\n");
                    compiler.get_code_generator().end_if();
                    printf("end if\n");
                }
                ;

start_for:      FOR VARIABLE FROM value TO value DO {
                    printf("new variable declaration: %s\n", $2.id->c_str());
                    compiler.assert_variable_not_declared(*($2.id), $2.line);

                    LValue* var = new LValue(*($2.id));
                    compiler.get_var_manager().declare(var);
                    compiler.initialize_variable(*var);
                    var->set_mutable(false);

                    auto ptr = std::make_shared<Loop>();

                    std::string counterLabel = *($2.id) + "-counter";
                    LValue* counter = new LValue(counterLabel);
                    compiler.get_var_manager().declare(counter);
                    compiler.initialize_variable(*counter);

                    ptr->iterator = var;
                    ptr->counter = counter;
                    compiler.get_code_generator().start_for(ptr, *$4, *$6);
                    printf("new variable declaration: %s\n", counterLabel.c_str());
                }
                ;

end_for:        commands END_FOR {
                    printf("Hello end for\n");

                    auto ptr = compiler.get_code_generator().end_for();
                    printf("Hello end for 2\n");


                    compiler.get_var_manager().undeclare(ptr->iterator);
                    compiler.get_var_manager().undeclare(ptr->counter);
                }
                ;

start_down_to:  FOR VARIABLE FROM value DOWN_TO value DO {
                    compiler.assert_variable_not_declared(*($2.id), $2.line);

                    LValue* var = new LValue(*($2.id));
                    compiler.get_var_manager().declare(var);
                    compiler.initialize_variable(*var);
                    var->set_mutable(false);

                    auto ptr = std::make_shared<Loop>();

                    std::string counterLabel = *($2.id) + "-counter";
                    LValue* counter = new LValue(counterLabel);
                    compiler.get_var_manager().declare(counter);
                    compiler.initialize_variable(*counter);

                    ptr->iterator = var;
                    ptr->counter = counter;

                    compiler.get_code_generator().start_for(ptr, *$4, *$6);
                    printf("new variable declaration: %s\n", counterLabel.c_str());
                }
                ;

end_down_to:    commands END_FOR {
                    auto ptr = compiler.get_code_generator().end_down_to();

                    compiler.get_var_manager().undeclare(ptr->iterator);
                    compiler.get_var_manager().undeclare(ptr->counter);

                }
                ;

start_repeat:   REPEAT {
                    printf("repeat-until\n");
                    auto ptr = std::make_shared<Loop>();
                    compiler.get_code_generator().start_loop(ptr);
                }
                ;

end_repeat:     commands UNTIL condition SEMICOLON {
                     printf("repeat-until end\n");
                     auto ptr = compiler.get_code_generator().end_repeat();
                     printf("repeat-until end\n");
                }
                ;

start_while:   WHILE {
                    printf("while\n");
                    auto ptr = std::make_shared<Loop>();
                    compiler.get_code_generator().start_loop(ptr);
                }
                ;

while_cond:      condition DO {
                    compiler.get_code_generator().while_cond();
                }
                ;

end_while:     commands END_WHILE {
                     printf("while end\n");
                     auto ptr = compiler.get_code_generator().end_while();
                }
                ;

expression:    value {
                    printf("hello variable\n");
                    compiler.assert_initialized(*$1, lines);

                    compiler.get_code_generator().load(*$1);
                }
                | value PLUS value {
                    printf("plus\n");
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
                    LValue* var = new LValue("div-flag");
                    compiler.get_var_manager().declare(var);
                    compiler.initialize_variable(*var);
                    compiler.get_code_generator().div(*$1, *$3);
                }
                | value MOD value {
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);
                    LValue* var = new LValue("mod-helper");
                    compiler.get_var_manager().declare(var);
                    compiler.initialize_variable(*var);
                    compiler.get_code_generator().mod(*$1, *$3);
                    //compiler.get_code_generator().mod(*$1, *$3, *var);
                }
                ;

condition:     value EQ value {
                    printf("EQ\n");
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().eq(*$1, *$3);
                    printf("EQ end\n");
                }
                | value NEQ value {
                    printf("NEQ\n");
                    compiler.assert_initialized(*$1, $2.line);
                    compiler.assert_initialized(*$3, $2.line);

                    compiler.get_code_generator().neq(*$1, *$3);
                    printf("NEQ end\n");
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
                | identifier  { /*nothing to do*/ }
                ;

identifier:     VARIABLE {
                    printf("hello var\n");
                    compiler.assert_variable_declared(*($1.id), $1.line);
                    compiler.assert_type(*($1.id), Value::ValueType::TYPE_VAR, $1.line);

                    LValue* var = compiler.get_var_manager().get(*($1.id)).get();
                    $$ = var;
                }
                | VARIABLE LEFT_BRACKET VARIABLE RIGHT_BRACKET {
                    printf("hello array\n");
                    compiler.assert_variable_declared(*($1.id), $1.line);
                    compiler.assert_variable_declared(*($3.id), $3.line);
                    compiler.assert_initialized(*($3.id), $3.line);

                    compiler.assert_type(*($1.id), Value::ValueType::TYPE_ARRAY, $1.line);
                    compiler.assert_type(*($3.id), Value::ValueType::TYPE_VAR, $3.line);

                   Array* var = dynamic_cast<Array*>(compiler.get_var_manager().get(*($1.id)).get());
                   Array* array = new Array(*var);
                   LValue* index = compiler.get_var_manager().get(*($3.id)).get();
                   array->set_current(index);

                   $$ = array;
                   printf("goodbye array\n");
                }
                | VARIABLE LEFT_BRACKET NUM RIGHT_BRACKET {
                    compiler.assert_variable_declared(*($1.id), $1.line);

                   compiler.assert_type(*($1.id), Value::ValueType::TYPE_ARRAY, $1.line);
                   printf("in parser: %d\n", $3.val);
                   Array* var = dynamic_cast<Array*>(compiler.get_var_manager().get(*($1.id)).get());
                   Array* array = new Array(*var);
                   Value* num = new RValue($3.val);
                   array->set_current(num);

                   $$ = array;
                 }

%%

void yyerror (const char *s) {fprintf (stderr, "Error: %s in line: %d\n", s, lines);}

int compile(const char* input, const char* output) {
    std::cout << 26 % 7 << '\n';
    std::cout << -26 % 7 << '\n';
    std::cout << 26 % -7 << '\n';
    std::cout << -26 % -7 << '\n';
    yyin = fopen(input, "r");
    int result = yyparse();
    fclose(yyin);
    std::cout << "Hello\n";
    std::ofstream outFile;

    outFile.open(output);

    auto code = compiler.get_code_generator().generate_asm_code();
    for (auto& l : code) {
        outFile << l;
    }

    return result;
}