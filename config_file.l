%{
/*
 * Copyright (c) 2004-2006, Doug Harple.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of author nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * $Id: config_file.l,v 1.15 2006/07/03 16:45:54 purgedhalo Exp $
 * 
 */

#include "config.h"

#include <string.h>
#include <stdlib.h>

#include "config_file_y.h"

int config_file_lineno = 1;

%}

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


