#
# References and reasons for compiler flags:
#
# https://github.com/dharple/detox/issues/31
# https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html
# https://access.redhat.com/blogs/766093/posts/1976213
#

AM_CFLAGS = \
	-DDATADIR=\"$(datadir)\" \
	-DSYSCONFDIR=\"$(sysconfdir)\" \
	-DYY_NO_INPUT \
	-DYY_NO_UNPUT \
	-D_FORTIFY_SOURCE=2 \
	-Wall \
	-Werror

#
#
#

AM_YFLAGS = -d

bin_PROGRAMS = \
	detox \
	inline-detox

detox_SOURCES = \
	config_file_yacc.y \
	config_file_lex.l \
	clean_string.c \
	config_file_dump.c \
	config_file_spoof.c \
	detox.c \
	file.c \
	parse_options.c \
	parse_table.c \
	table.c

inline_detox_SOURCES = $(detox_SOURCES)

