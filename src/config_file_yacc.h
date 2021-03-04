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

#ifndef YY_YY_CONFIG_FILE_YACC_H_INCLUDED
# define YY_YY_CONFIG_FILE_YACC_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BUILTIN = 258,
    CLOSE = 259,
    EOL = 260,
    FILENAME = 261,
    IGNORE = 262,
    ISO8859_1 = 263,
    LENGTH = 264,
    LOWER = 265,
    MAX_LENGTH = 266,
    OPEN = 267,
    REMOVE_TRAILING = 268,
    SAFE = 269,
    SEQUENCE = 270,
    UNCGI = 271,
    UTF_8 = 272,
    WIPEUP = 273,
    NVALUE = 274,
    ID = 275,
    QSTRING = 276
  };
#endif
/* Tokens.  */
#define BUILTIN 258
#define CLOSE 259
#define EOL 260
#define FILENAME 261
#define IGNORE 262
#define ISO8859_1 263
#define LENGTH 264
#define LOWER 265
#define MAX_LENGTH 266
#define OPEN 267
#define REMOVE_TRAILING 268
#define SAFE 269
#define SEQUENCE 270
#define UNCGI 271
#define UTF_8 272
#define WIPEUP 273
#define NVALUE 274
#define ID 275
#define QSTRING 276

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 47 "config_file_yacc.y"

    char *string; /* string buffer */
    int   cmd;    /* command value */
    int   nvalue; /* nvalue */

#line 105 "config_file_yacc.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CONFIG_FILE_YACC_H_INCLUDED  */
