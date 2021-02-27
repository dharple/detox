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

AM_LFLAGS = -i

EXTRA_DIST = *.h

bin_PROGRAMS = \
	detox \
	inline-detox

noinst_PROGRAMS = \
	check-table \
	generate-builtin-table

detox_SOURCES = \
	builtin_table.c \
	config_file.c \
	config_file_yacc.y \
	config_file_lex.l \
	clean_string.c \
	config_file_dump.c \
	config_file_spoof.c \
	detox.c \
	detox_struct.c \
	file.c \
	parse_options.c \
	parse_table.c \
	sequence.c \
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

#
# code coverage rules
#

if WITH_COVERAGE

coverage-text:
	gcov *.c

else

coverage-text:
	echo "code coverage is not enabled; run ./configure --with-coverage"

endif # WITH_COVERAGE

#
#
#

clean-local:
	rm -f *.i *.s
	rm -f *.gcov *.gcno *.gcda
	rm -f test-table-regression
