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
        long long val;
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
                    compiler.assert_variable_not_declared(*($1.id), $1.line);
                    LValue* var = new LValue(*($1.id));
                    compiler.get_var_manager().declare(var);
                    delete $1.id;
                }
                | VARIABLE LEFT_BRACKET NUM ARRAY_RNG NUM RIGHT_BRACKET {
                    compiler.assert_variable_not_declared(*($1.id), $1.line);
                    compiler.assert_array_range(*($1.id), $3.val, $5.val, $1.line);
                    Array* var = new Array(*($1.id), $3.val, $5.val);
                    compiler.get_var_manager().declare(var);
                    delete $1.id;
                }
                ;

commands:      commands command { /*nothing to do*/ }
                | command { /*nothing to do*/ }
                ;

command:        identifier ASSIGN expression SEMICOLON {
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
                    compiler.initialize_variable(*val);
                 }
                | WRITE value SEMICOLON {
                    compiler.assert_initialized(*$2, $1.line);
                    compiler.get_code_generator().write(*$2);
                 }
                ;

start_if:        IF condition THEN {
                    compiler.get_code_generator().start_if();
                }
                ;

start_else:      commands ELSE {
                    compiler.get_code_generator().start_else();
                 }
                 ;

end_if:          commands END_IF {
                    compiler.get_code_generator().end_if();
                }
                ;

start_for:      FOR VARIABLE FROM value TO value DO {
                    compiler.assert_variable_not_declared(*($2.id), $2.line);

                    LValue* var = new LValue(*($2.id));
                    compiler.get_var_manager().declare(var);
                    compiler.initialize_variable(*var);
                    var->inc_priority(100);
                    var->set_mutable(false);

                    auto ptr = std::make_shared<Loop>();

                    std::string counterLabel = *($2.id) + "-counter";
                    LValue* counter = new LValue(counterLabel);
                    compiler.get_var_manager().declare(counter);
                    compiler.initialize_variable(*counter);

                    ptr->iterator = var;
                    ptr->counter = counter;
                    compiler.get_code_generator().start_for(ptr, *$4, *$6);
                    delete $2.id;
                }
                ;

end_for:        commands END_FOR {

                    auto ptr = compiler.get_code_generator().end_for();


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
                    delete $2.id;
                }
                ;

end_down_to:    commands END_FOR {
                    auto ptr = compiler.get_code_generator().end_down_to();

                    compiler.get_var_manager().undeclare(ptr->iterator);
                    compiler.get_var_manager().undeclare(ptr->counter);

                }
                ;

start_repeat:   REPEAT {
                    auto ptr = std::make_shared<Loop>();
                    compiler.get_code_generator().start_loop(ptr);
                }
                ;

end_repeat:     commands UNTIL condition SEMICOLON {
                     auto ptr = compiler.get_code_generator().end_repeat();
                }
                ;

start_while:   WHILE {
                    auto ptr = std::make_shared<Loop>();
                    compiler.get_code_generator().start_loop(ptr);
                }
                ;

while_cond:      condition DO {
                    compiler.get_code_generator().while_cond();
                }
                ;

end_while:     commands END_WHILE {
                     auto ptr = compiler.get_code_generator().end_while();
                }
                ;

expression:    value {
                    compiler.assert_initialized(*$1, lines);

                    compiler.get_code_generator().load(*$1);
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
                | identifier  { $1->inc_priority(); }
                ;

identifier:     VARIABLE {
                    compiler.assert_variable_declared(*($1.id), $1.line);
                    compiler.assert_type(*($1.id), Value::ValueType::TYPE_VAR, $1.line);

                    LValue* var = compiler.get_var_manager().get(*($1.id)).get();
                    $$ = var;
                    delete $1.id;
                }
                | VARIABLE LEFT_BRACKET VARIABLE RIGHT_BRACKET {
                    compiler.assert_variable_declared(*($1.id), $1.line);
                    compiler.assert_variable_declared(*($3.id), $3.line);
                    compiler.assert_initialized(*($3.id), $3.line);

                    compiler.assert_type(*($1.id), Value::ValueType::TYPE_ARRAY, $1.line);
                    compiler.assert_type(*($3.id), Value::ValueType::TYPE_VAR, $3.line);

                   Array* var = dynamic_cast<Array*>(compiler.get_var_manager().get(*($1.id)).get());
                   Array* array = new Array(*var);
                   LValue* index = compiler.get_var_manager().get(*($3.id)).get();
                   array->set_current(index);
                   delete $1.id;
                   delete $3.id;

                   $$ = array;
                }
                | VARIABLE LEFT_BRACKET NUM RIGHT_BRACKET {
                    compiler.assert_variable_declared(*($1.id), $1.line);

                    compiler.assert_type(*($1.id), Value::ValueType::TYPE_ARRAY, $1.line);
                    Array* var = dynamic_cast<Array*>(compiler.get_var_manager().get(*($1.id)).get());
                    Array* array = new Array(*var);
                    Value* num = new RValue($3.val);
                    array->set_current(num);
                    delete $1.id;
                    $$ = array;
                 }

%%

void yyerror (const char *s) { fprintf (stderr, "Error: %s in line: %d\n", s, lines); }

int compile(const char* input, const char* output) {
    yyin = fopen(input, "r");
    int result = yyparse();
    fclose(yyin);
    std::ofstream outFile;

    outFile.open(output);

    auto code = compiler.get_code_generator().generate_asm_code();
    for (auto& l : code) {
        outFile << l;
    }

    return result;
}