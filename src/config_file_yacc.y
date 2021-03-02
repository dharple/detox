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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "detox_struct.h"
#include "clean_string.h"
#include "config_file.h"

/*
 * I must apologize in advance for the cryptic, global variable names.
 */

static struct detox_sequence_list *cf_sequence_ret, *cf_sequence_current;
static struct detox_sequence_filter *cf_filter_ret, *cf_filter_current;
static struct detox_ignore_entry *cf_ignore_ret, *cf_ignore_current;
static struct clean_string_options *cs_options;
static char *current_name = NULL;
static char *current_filename = NULL;
static options_t *current_options;

void cf_append_sequence_list(void);
void cf_append_sequence_filter(void *ptr, struct clean_string_options *options);
void cf_append_ignore_entry(int token, void *str);

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

%option nounput
%option noinput

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

method: UNCGI EOL    { cf_append_sequence_filter(&clean_uncgi, NULL); }
    |
    LOWER EOL    { cf_append_sequence_filter(&clean_lower, NULL); }
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

iso8859_1: ISO8859_1 { cf_append_sequence_filter(&clean_iso8859_1, NULL); }
    |
    ISO8859_1 OPEN CLOSE { cf_append_sequence_filter(&clean_iso8859_1, NULL); }
    |
    ISO8859_1 OPEN FILENAME string EOL CLOSE {
        cs_options = new_clean_string_options();
        cs_options->filename = $4;

        cf_append_sequence_filter(&clean_iso8859_1, cs_options);
    }
    |
    ISO8859_1 OPEN BUILTIN string EOL CLOSE {
        cs_options = new_clean_string_options();
        cs_options->builtin = $4;

        cf_append_sequence_filter(&clean_iso8859_1, cs_options);
    }
    ;

utf_8: UTF_8 { cf_append_sequence_filter(&clean_utf_8, NULL); }
    |
    UTF_8 OPEN CLOSE { cf_append_sequence_filter(&clean_utf_8, NULL); }
    |
    UTF_8 OPEN FILENAME string EOL CLOSE {
        cs_options = new_clean_string_options();
        cs_options->filename = $4;

        cf_append_sequence_filter(&clean_utf_8, cs_options);
    }
    |
    UTF_8 OPEN BUILTIN string EOL CLOSE {
        cs_options = new_clean_string_options();
        cs_options->builtin = $4;

        cf_append_sequence_filter(&clean_utf_8, cs_options);
    }
    ;

safe: SAFE { cf_append_sequence_filter(&clean_safe, NULL); }
    |
    SAFE OPEN CLOSE { cf_append_sequence_filter(&clean_safe, NULL); }
    |
    SAFE OPEN FILENAME string EOL CLOSE {
        cs_options = new_clean_string_options();
        cs_options->filename = $4;

        cf_append_sequence_filter(&clean_safe, cs_options);
    }
    |
    SAFE OPEN BUILTIN string EOL CLOSE {
        cs_options = new_clean_string_options();
        cs_options->builtin = $4;

        cf_append_sequence_filter(&clean_safe, cs_options);
    }
    ;

wipeup:    WIPEUP {
        cf_append_sequence_filter(&clean_wipeup, NULL);
    }
    |
    WIPEUP OPEN CLOSE {
        cf_append_sequence_filter(&clean_wipeup, NULL);
    }
    |
    WIPEUP OPEN REMOVE_TRAILING EOL CLOSE {
        cs_options = new_clean_string_options();
        cs_options->remove_trailing = 1;

        cf_append_sequence_filter(&clean_wipeup, cs_options);
    }
    ;

max_length: MAX_LENGTH    { cf_append_sequence_filter(&clean_max_length, NULL); }
    |
    MAX_LENGTH OPEN CLOSE { cf_append_sequence_filter(&clean_max_length, NULL); }
    |
    MAX_LENGTH OPEN LENGTH NVALUE EOL CLOSE {
        cs_options = new_clean_string_options();
        cs_options->max_length = (size_t)$4;

        cf_append_sequence_filter(&clean_max_length, cs_options);
    }
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
        cf_append_ignore_entry(FILENAME, $2);
    }
    ;

string: QSTRING         { $$ = $1; }
    |
    ID            { $$ = $1; }
    ;

%%

extern FILE *yyin;
extern FILE *yyout;

struct detox_parse_results *parse_config_file(char *filename, struct detox_parse_results *previous_results, options_t *main_options) {
    struct detox_parse_results *ret = NULL;

    current_filename = filename;
    current_options = main_options;

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
        ret = new_detox_parse_results();
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

    cf_ignore_ret = NULL;
    cf_ignore_current = NULL;

    if (previous_results && previous_results->files_to_ignore) {
        cf_ignore_ret = previous_results->files_to_ignore;
        cf_ignore_current = cf_ignore_ret;
        while (cf_ignore_current->next != NULL) {
            cf_ignore_current = cf_ignore_current->next;
        }
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
    ret->files_to_ignore = cf_ignore_ret;
   
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
    struct detox_sequence_list *work;

    if (current_name == NULL) {
        current_name = strdup("default");
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
        work = new_detox_sequence_list();
        work->name = strdup(current_name);

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

    work->head = cf_filter_ret;
    work->source_filename = strdup(current_filename);
    cf_filter_ret = cf_filter_current = NULL;

}


void cf_append_sequence_filter(void *ptr, struct clean_string_options *options) {
    struct detox_sequence_filter *work;

    work = new_detox_sequence_filter();
    work->cleaner = ptr;
    work->options = options;

    if (cf_filter_ret == NULL) {
        cf_filter_ret = cf_filter_current = work;
    }
    else {
        cf_filter_current->next = work;
        cf_filter_current = work;
    }
}


void cf_append_ignore_entry(int token, void *str) {
    struct detox_ignore_entry *work;

    work = new_detox_ignore_entry();

    switch(token) {
        case FILENAME:
            work->filename = str;
            break;

        default:
            break;
    }

    if (cf_ignore_ret == NULL) {
        cf_ignore_ret = cf_ignore_current = work;
    }
    else {
        cf_ignore_current->next = work;
        cf_ignore_current = work;
    }
}
