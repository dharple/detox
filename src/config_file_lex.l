%{
/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "config_file_yacc.h"
#include "config_file.h"

int config_file_lineno = 1;

%}

%option noyywrap

%%

[\t ]+  { /* ignore whitespace */ }

#.*     { /* ignore comments */ }

\n      { config_file_lineno++; }

builtin            { return BUILTIN; }
filename           { return FILENAME; }
ignore             { return IGNORE; }
iso8859_1          { return ISO8859_1; }
length             { return LENGTH; }
lower              { return LOWER; }
max_length         { return MAX_LENGTH; }
remove_trailing    { return REMOVE_TRAILING; }
safe               { return SAFE; }
sequence           { return SEQUENCE; }
uncgi              { return UNCGI; }
utf_8              { return UTF_8; }
wipeup             { return WIPEUP; }

[{]     { return OPEN; }
[}]     { return CLOSE; }

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

[a-zA-Z][a-zA-Z0-9_]* {
    yylval.string = strdup(yytext);
    return ID;
}

[0-9]+ {
    yylval.nvalue = atoi(yytext);
    return NVALUE;
}

[;]     { return EOL; }

%%
