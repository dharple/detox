/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
    QSTRING = 258,
    ID = 259,
    SEQUENCE = 260,
    IGNORE = 261,
    UNCGI = 262,
    ISO8859_1 = 263,
    UTF_8 = 264,
    SAFE = 265,
    WIPEUP = 266,
    MAX_LENGTH = 267,
    LOWER = 268,
    FILENAME = 269,
    REMOVE_TRAILING = 270,
    LENGTH = 271,
    OPEN = 272,
    CLOSE = 273,
    EOL = 274,
    NVALUE = 275
  };
#endif
/* Tokens.  */
#define QSTRING 258
#define ID 259
#define SEQUENCE 260
#define IGNORE 261
#define UNCGI 262
#define ISO8859_1 263
#define UTF_8 264
#define SAFE 265
#define WIPEUP 266
#define MAX_LENGTH 267
#define LOWER 268
#define FILENAME 269
#define REMOVE_TRAILING 270
#define LENGTH 271
#define OPEN 272
#define CLOSE 273
#define EOL 274
#define NVALUE 275

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 66 "config_file_yacc.y" /* yacc.c:1909  */

    char	*string;	/* string buffer */
    int		cmd;		/* command value */
    struct detox_sequence_entry *seq;	/* sequence */
    int		nvalue;		/* nvalue */

#line 101 "config_file_yacc.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CONFIG_FILE_YACC_H_INCLUDED  */
