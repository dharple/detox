/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_CONFIG_FILE_YACC_H_INCLUDED
# define YY_YY_CONFIG_FILE_YACC_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    BUILTIN = 258,                 /* BUILTIN  */
    CLOSE = 259,                   /* CLOSE  */
    EOL = 260,                     /* EOL  */
    FILENAME = 261,                /* FILENAME  */
    IGNORE = 262,                  /* IGNORE  */
    ISO8859_1 = 263,               /* ISO8859_1  */
    LENGTH = 264,                  /* LENGTH  */
    LOWER = 265,                   /* LOWER  */
    MAX_LENGTH = 266,              /* MAX_LENGTH  */
    OPEN = 267,                    /* OPEN  */
    REMOVE_TRAILING = 268,         /* REMOVE_TRAILING  */
    SAFE = 269,                    /* SAFE  */
    SEQUENCE = 270,                /* SEQUENCE  */
    UNCGI = 271,                   /* UNCGI  */
    UTF_8 = 272,                   /* UTF_8  */
    WIPEUP = 273,                  /* WIPEUP  */
    NVALUE = 274,                  /* NVALUE  */
    ID = 275,                      /* ID  */
    QSTRING = 276                  /* QSTRING  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
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
#line 43 "config_file_yacc.y"

    char *string; /* string buffer */
    int   cmd;    /* command value */
    int   nvalue; /* nvalue */

#line 115 "config_file_yacc.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_CONFIG_FILE_YACC_H_INCLUDED  */
