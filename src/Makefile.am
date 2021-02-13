AM_CFLAGS = -g -O2 -Wall -DYY_NO_INPUT -DYY_NO_UNPUT -DDATADIR=\"$(datadir)\" -DSYSCONFDIR=\"$(sysconfdir)\"
AM_YFLAGS = -d

bin_PROGRAMS = detox inline-detox
detox_SOURCES = detox.c file.c clean_string.c table.c parse_table.c config_file_yacc.y config_file_lex.l config_file_spoof.c config_file_dump.c parse_options.c
inline_detox_SOURCES = $(detox_SOURCES)

