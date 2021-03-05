%{
/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "detox_struct.h"

#include "config_file.h"
#include "filelist.h"
#include "filter.h"
#include "sequence.h"
#include "wrapped.h"

/*
 * I must apologize in advance for the cryptic, global variable names.
 */

static sequence_t *cf_sequence_ret, *cf_sequence_current;
static filter_t *cf_filter_ret, *cf_filter_current;
static char *current_name = NULL;
static char *current_filename = NULL;
static filelist_t *files_to_ignore;

void cf_append_sequence_list(void);
void cf_append_filter(int cleaner, char *builtin, char *filename, int max_length, int remove_trailing);
void cf_append_ignore_entry(void *str);

void yyerror (char *s);

int yylex (void);

%}

%union {
    char *string; /* string buffer */
    int   cmd;    /* command value */
    int   nvalue; /* nvalue */
}

%token <cmd> BUILTIN
%token <cmd> CLOSE
%token <cmd> EOL
%token <cmd> FILENAME
%token <cmd> IGNORE
%token <cmd> ISO8859_1
%token <cmd> LENGTH
%token <cmd> LOWER
%token <cmd> MAX_LENGTH
%token <cmd> OPEN
%token <cmd> REMOVE_TRAILING
%token <cmd> SAFE
%token <cmd> SEQUENCE
%token <cmd> UNCGI
%token <cmd> UTF_8
%token <cmd> WIPEUP
%token <nvalue> NVALUE
%token <string> ID
%token <string> QSTRING

%type <string> string

%%

configfile:
    |
    configfile rule
    ;

rule: sequence
    |
    ignore
    ;

sequence: sequence_open method_list sequence_close
    ;

sequence_open: SEQUENCE string OPEN { current_name = $2; }
    ;

sequence_close: CLOSE EOL { cf_append_sequence_list(); }
    ;

method_list: method |
    method_list method
    ;

method: UNCGI EOL    { cf_append_filter(FILTER_UNCGI, NULL, NULL, 0, 0); }
    |
    LOWER EOL    { cf_append_filter(FILTER_LOWER, NULL, NULL, 0, 0); }
    |
    wipeup EOL
    |
    iso8859_1 EOL
    |
    utf_8 EOL
    |
    safe EOL
    |
    max_length EOL
    ;

iso8859_1: ISO8859_1 { cf_append_filter(FILTER_ISO8859_1, NULL, NULL, 0, 0); }
    |
    ISO8859_1 OPEN CLOSE { cf_append_filter(FILTER_ISO8859_1, NULL, NULL, 0, 0); }
    |
    ISO8859_1 OPEN FILENAME string EOL CLOSE { cf_append_filter(FILTER_ISO8859_1, NULL, $4, 0, 0); }
    |
    ISO8859_1 OPEN BUILTIN string EOL CLOSE { cf_append_filter(FILTER_ISO8859_1, $4, NULL, 0, 0); }
    ;

utf_8: UTF_8 { cf_append_filter(FILTER_UTF_8, NULL, NULL, 0, 0); }
    |
    UTF_8 OPEN CLOSE { cf_append_filter(FILTER_UTF_8, NULL, NULL, 0, 0); }
    |
    UTF_8 OPEN FILENAME string EOL CLOSE { cf_append_filter(FILTER_UTF_8, NULL, $4, 0, 0); }
    |
    UTF_8 OPEN BUILTIN string EOL CLOSE { cf_append_filter(FILTER_UTF_8, $4, NULL, 0, 0); }
    ;

safe: SAFE { cf_append_filter(FILTER_SAFE, NULL, NULL, 0, 0); }
    |
    SAFE OPEN CLOSE { cf_append_filter(FILTER_SAFE, NULL, NULL, 0, 0); }
    |
    SAFE OPEN FILENAME string EOL CLOSE { cf_append_filter(FILTER_SAFE, NULL, $4, 0, 0); }
    |
    SAFE OPEN BUILTIN string EOL CLOSE { cf_append_filter(FILTER_SAFE, $4, NULL, 0, 0); }
    ;

wipeup:    WIPEUP { cf_append_filter(FILTER_WIPEUP, NULL, NULL, 0, 0); }
    |
    WIPEUP OPEN CLOSE { cf_append_filter(FILTER_WIPEUP, NULL, NULL, 0, 0); }
    |
    WIPEUP OPEN REMOVE_TRAILING EOL CLOSE { cf_append_filter(FILTER_WIPEUP, NULL, NULL, 0, 1); }
    ;

max_length: MAX_LENGTH    { cf_append_filter(FILTER_MAX_LENGTH, NULL, NULL, 0, 0); }
    |
    MAX_LENGTH OPEN CLOSE { cf_append_filter(FILTER_MAX_LENGTH, NULL, NULL, 0, 0); }
    |
    MAX_LENGTH OPEN LENGTH NVALUE EOL CLOSE { cf_append_filter(FILTER_MAX_LENGTH, NULL, NULL, $4, 0); }
    ;

ignore: ignore_open ignore_list ignore_close
    ;

ignore_open: IGNORE OPEN
    ;

ignore_close: CLOSE EOL
    ;

ignore_list: ignore_filename |
    ignore_list ignore_filename
    ;

ignore_filename: FILENAME string EOL {
        cf_append_ignore_entry($2);
    }
    ;

string: QSTRING         { $$ = $1; }
    |
    ID            { $$ = $1; }
    ;

%%

extern FILE *yyin;
extern FILE *yyout;

config_file_t *parse_config_file(char *filename, config_file_t *previous_results, options_t *main_options) {
    config_file_t *ret = NULL;

    current_filename = filename;

    /*
     * XXX - Should we be closing the default yyin/yyout?  If so, should we
     * be setting them to NULL at the end of this function?
     */

    yyin = fopen(filename, "r");
    if (yyin == NULL) {
        return previous_results;
    }
    yyout = fopen("/dev/null", "w");

    /*
     * Initialize the return variable
     */

    if (previous_results) {
        ret = previous_results;
    }
    else {
        ret = config_file_init();
    }

    /*
     * Initialize the sequence list
     */

    cf_sequence_ret = NULL;
    cf_sequence_current = NULL;

    if (previous_results && previous_results->sequences) {
        cf_sequence_ret = previous_results->sequences;
        cf_sequence_current = cf_sequence_ret;
        while (cf_sequence_current->next != NULL) {
            cf_sequence_current = cf_sequence_current->next;
        }
    }

    /*
     * Initialize the ignore list
     */

    if (previous_results && filelist_count(previous_results->files_to_ignore) > 0) {
        files_to_ignore = previous_results->files_to_ignore;
    } else {
        files_to_ignore = filelist_init();
    }

    /*
     * Reset the sequence entry holding vars
     */

    cf_filter_ret = NULL;
    cf_filter_current = NULL;

    do {
        yyparse();
    }
    while (!feof(yyin));

    fclose(yyin);
    fclose(yyout);

    /*
     * Populate returns
     */

    ret->sequences = cf_sequence_ret;
    ret->files_to_ignore = files_to_ignore;
   
    return ret;
}

void yyerror(char *s) {
    /*
     * XXX - Is extern valid here?  Does it do what I'm expecting?
     */
    extern char *yytext;

    fprintf(stderr, "detox: error parsing config file %s: %s\n", current_filename, s);
    fprintf(stderr, "\tline %d", config_file_lineno);
    if (yytext != NULL) {
        fprintf(stderr, ": %s", yytext);
    }
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}


void cf_append_sequence_list(void) {
    sequence_t *work;

    if (current_name == NULL) {
        current_name = wrapped_strdup("default");
    }

    work = NULL;

    if (cf_sequence_ret != NULL) {
        work = cf_sequence_ret;

        while (work != NULL) {
            if (strcmp(work->name, current_name) == 0) {
                break;
            }

            work = work->next;
        }

    }

    if (work != NULL) {
        /*
         * XXX - Free Old Tree
         */
    }
    else {
        work = sequence_init(current_name);

        /*
         * Append to the tree first.  If we don't, we could create a
         * circular reference.
         */
        if (cf_sequence_ret == NULL) {
            cf_sequence_ret = cf_sequence_current = work;
        }
        else {
            cf_sequence_current->next = work;
            cf_sequence_current = work;
        }

    }

    work->filters = cf_filter_ret;
    work->source_filename = wrapped_strdup(current_filename);
    cf_filter_ret = cf_filter_current = NULL;

}


void cf_append_filter(int cleaner, char *builtin, char *filename, int max_length, int remove_trailing)
{
    filter_t *work;

    work = filter_init(cleaner);
    work->builtin = (builtin == NULL) ? NULL : wrapped_strdup(builtin);
    work->filename = (filename == NULL) ? NULL : wrapped_strdup(filename);
    work->max_length = (size_t) max_length;
    work->remove_trailing = remove_trailing;

    if (cf_filter_ret == NULL) {
        cf_filter_ret = cf_filter_current = work;
    }
    else {
        cf_filter_current->next = work;
        cf_filter_current = work;
    }
}


void cf_append_ignore_entry(void *str) {
    filelist_put(files_to_ignore, str);
}
