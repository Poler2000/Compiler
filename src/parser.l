%option noyywrap 

%{

#define YY_NO_UNPUT

#include <stdio.h>
#include <iostream>
#include <stack>

#include "parser_tab.h"
void yyerror (const char *s);
int yylex();

static int lex_token(int token);
static int lex_number(int token);
static int lex_variable(int token);

unsigned int lines = 1;
std::stack<Token> tokens;

#define lex_degug(token) \
    ({ \
        printf("token: %d\n", token); \
    })

#define lex_degug_num(num) \
    ({ \
        printf("num: %d\n", num); \
    })

#define lex_degug_var(var) \
    ({ \
        printf("var: %s\n", var); \
    })
%}

WHITE_SPACE     [ \t]

%x COMMENT

%%

{WHITE_SPACE}+  ;


\(              { /*printf("Comment begin\n");*/ BEGIN(COMMENT); }
<COMMENT>\)     { /*printf("Comment end\n");*/ BEGIN(INITIAL); }
<COMMENT>\n     { lines++; }
<COMMENT>[^\)\(\n)] { ; }


\n              { lines++; }
\,              { return lex_token(COMMA); }
\;              { return lex_token(SEMICOLON); }


PLUS            { return lex_token(PLUS); }
MINUS           { return lex_token(MINUS); }
TIMES           { return lex_token(TIMES); }
DIV             { return lex_token(DIV); }
MOD             { return lex_token(MOD); }


EQ              { return lex_token(EQ); }
NEQ             { return lex_token(NEQ); }
LE              { return lex_token(LE); }
GE              { return lex_token(GE); }
LEQ             { return lex_token(LEQ); }
GEQ             { return lex_token(GEQ); }


ASSIGN          { return lex_token(ASSIGN); }


FOR             { return lex_token(FOR); }
FROM            { return lex_token(FROM); } 
TO              { return lex_token(TO); }
DO              { return lex_token(DO); }
DOWNTO          { return lex_token(DOWN_TO); } 
ENDFOR          { return lex_token(END_FOR); }


WHILE           { return lex_token(WHILE); } 
ENDWHILE        { return lex_token(END_WHILE); }


REPEAT          { return lex_token(REPEAT); }
UNTIL           { return lex_token(UNTIL); }


READ            { return lex_token(READ); } 
WRITE           { return lex_token(WRITE); }


VAR             { return lex_token(VAR); } 
"BEGIN"         { return lex_token(T_BEGIN); }
END             { return lex_token(END); }


IF              { return lex_token(IF); }
THEN            { return lex_token(THEN); }
ELSE            { return lex_token(ELSE); }
ENDIF           { return lex_token(END_IF); }

\[              { return lex_token(LEFT_BRACKET); }
\:              { return lex_token(ARRAY_RNG); }
\]              { return lex_token(RIGHT_BRACKET); }


[a-z]+          { return lex_variable(VARIABLE); }
\-?[0-9]+       { return lex_number(NUM); }
.               { return lex_token(ERROR); }

%%

static int lex_token(int token) {
    //lex_degug(token);
    yylval.token.val = 0;
    yylval.token.line = lines;
    yylval.token.id = nullptr;
    tokens.push(yylval.token);

    return token;
}

static int lex_number(int token) {
    yylval.token.val = std::stoll(yytext);
    yylval.token.line = lines;
    yylval.token.id = nullptr;
    //lex_degug_num(yylval.token.val);
    tokens.push(yylval.token);

    return token;
}

static int lex_variable(int token) {
    yylval.token.val = 0;
    yylval.token.line = lines;
    yylval.token.id = new std::string(yytext);
    //lex_degug_var(yytext);
    tokens.push(yylval.token);

    return token;
}