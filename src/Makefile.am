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

noinst_PROGRAMS = \
	check-table \
	generate-builtin-table \
	test-table-regression

detox_SOURCES = \
	builtin_table.c \
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

check_table_SOURCES = \
	check_table.c \
	table.c \
	table_dump.c

generate_builtin_table_SOURCES = \
	parse_table.c \
	table.c \
	generate_builtin_table.c

test_table_regression_SOURCES = \
	parse_table.c \
	table.c \
	test_table_regression.c

builtin_table.c: generate-builtin-table builtin_table.c.in ../table/*.sample
	cp builtin_table.c.in builtin_table.c
	./generate-builtin-table ../table/safe.tbl.sample | sed -e's/NEW/safe/' >> builtin_table.c
	./generate-builtin-table ../table/iso8859_1.tbl.sample | sed -e's/NEW/iso8859_1/' >> builtin_table.c
	./generate-builtin-table ../table/unicode.tbl.sample | sed -e's/NEW/unicode/' >> builtin_table.c
