%{
/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "config.h"

#include <string.h>
#include <stdlib.h>

#include "config_file_yacc.h"

int config_file_lineno = 1;

%}

%option noyywrap

%%

[\t ]+		{ /* ignore whitespace */ }

#.*		{ /* ignore comments */ }

\n		{ config_file_lineno++; }

[Ss][Ee][Qq][Uu][Ee][Nn][Cc][Ee]	{ return SEQUENCE; }

[Ii][Gg][Nn][Oo][Rr][Ee]		{ return IGNORE; }

[Uu][Nn][Cc][Gg][Ii]			{ return UNCGI; }

[Ii][Ss][Oo]8859_1			{ return ISO8859_1; }

[Uu][Tt][Ff]_8				{ return UTF_8; }

[Ss][Aa][Ff][Ee]			{ return SAFE; }

[Ww][Ii][Pp][Ee][Uu][Pp]		{ return WIPEUP; }

[Mm][Aa][Xx]_[Ll][Ee][Nn][Gg][Tt][Hh]	{ return MAX_LENGTH; }

[Ll][Oo][Ww][Ee][Rr]			{ return LOWER; }

[Ll][Ee][Nn][Gg][Tt][Hh]		{ return LENGTH; }

[Ff][Ii][Ll][Ee][Nn][Aa][Mm][Ee]	{ return FILENAME; }

[Rr][Ee][Mm][Oo][Vv][Ee]_[Tt][Rr][Aa][Ii][Ll][Ii][Nn][Gg]	{ return REMOVE_TRAILING; }

[{]		{ return OPEN; }
[}]		{ return CLOSE; }

\"[^\"\n]*[\"\n] { 
		  yylval.string = strdup(yytext+1);
		  if (yylval.string[yyleng-2] != '"') {
		    printf("Unterminated character string\n");
		  }
		  else {
		    yylval.string[yyleng-2] = '\0';
		  }
		  return QSTRING;
		}

[a-zA-Z][a-zA-Z0-9_]*	{ 
		  yylval.string = strdup(yytext); 
		  return ID; 
		}

[0-9]+		{ 
		  yylval.nvalue = atoi(yytext); 
		  return NVALUE; 
		}



[;]		{ return EOL; }

%%


