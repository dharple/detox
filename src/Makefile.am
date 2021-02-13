# References and reasons for compiler flags:
#
# https://github.com/dharple/detox/issues/31
# https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html
# https://www.keil.com/support/man/docs/armclang_ref/armclang_ref_cjh1548250046139.htm
# https://developers.redhat.com/blog/2020/05/22/stack-clash-mitigation-in-gcc-part-3/
# https://gcc.gnu.org/onlinedocs/gccint/LTO-Overview.html
#

AM_CFLAGS = \
	-DDATADIR=\"$(datadir)\" \
	-DSYSCONFDIR=\"$(sysconfdir)\" \
	-DYY_NO_INPUT \
	-DYY_NO_UNPUT \
	-D_FORTIFY_SOURCE=2 \
	-Wall \
	-Werror=return-type \
	-flto=auto \
	-fstack-clash-protection \
	-fstack-protector-strong

AM_YFLAGS = -d

bin_PROGRAMS = \
	detox \
	inline-detox

detox_SOURCES = \
	clean_string.c \
	config_file_dump.c \
	config_file_lex.l \
	config_file_spoof.c \
	config_file_yacc.y \
	detox.c \
	file.c \
	parse_options.c \
	parse_table.c \
	table.c

inline_detox_SOURCES = $(detox_SOURCES)

