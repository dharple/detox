BUILT_SOURCES = config_file.h
AM_YFLAGS = -d

AM_CPPFLAGS = -DDATADIR=\"$(datadir)\" -DSYSCONFDIR=\"$(sysconfdir)\"

bin_PROGRAMS = detox inline-detox
detox_SOURCES = detox.c file.c clean_string.c table.c parse_table.c config_file_yacc.y config_file_lex.l config_file_spoof.c config_file_dump.c parse_options.c
inline_detox_SOURCES = $(detox_SOURCES)

