/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
# define YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 25 "./src/parser.y"

    #include <string>
    #include <cstdlib>
    #include <rvalue.h>
    typedef struct Token
    {
        long long val;
        unsigned int line;
        std::string* id;
    } Token;

#line 60 "./include/parser_tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    EQ = 258,
    NEQ = 259,
    LE = 260,
    GE = 261,
    LEQ = 262,
    GEQ = 263,
    PLUS = 264,
    MINUS = 265,
    TIMES = 266,
    DIV = 267,
    MOD = 268,
    ASSIGN = 269,
    FOR = 270,
    FROM = 271,
    TO = 272,
    DO = 273,
    DOWN_TO = 274,
    END_FOR = 275,
    WHILE = 276,
    END_WHILE = 277,
    REPEAT = 278,
    UNTIL = 279,
    READ = 280,
    WRITE = 281,
    VAR = 282,
    T_BEGIN = 283,
    END = 284,
    IF = 285,
    THEN = 286,
    ELSE = 287,
    END_IF = 288,
    SEMICOLON = 289,
    COMMA = 290,
    VARIABLE = 291,
    NUM = 292,
    LEFT_BRACKET = 293,
    ARRAY_RNG = 294,
    RIGHT_BRACKET = 295,
    ERROR = 296
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 38 "./src/parser.y"

    Token token;
    Value* value;

#line 118 "./include/parser_tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED  */
