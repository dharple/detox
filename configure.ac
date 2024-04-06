# Process this file with autoconf to produce a configure script.
AC_INIT([detox], [3.0.0-beta2], [detox.dharple at gmail.com], [], [https://github.com/dharple/detox])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC
AC_PROG_LEX([noyywrap])
AC_PROG_YACC

AC_CHECK_FUNCS([getopt_long])
AC_STRUCT_ST_BLOCKS

AC_SYS_LARGEFILE

AC_CHECK_PROGS([MANDOC], [mandoc])
AM_CONDITIONAL([MANDOC_INSTALLED], [test -n "$MANDOC"])

AC_SEARCH_LIBS([ceil], [m])

#
# References and reasons for compiler flags:
#
# https://github.com/klange/prboom/blob/master/autotools/ac_c_compile_flags.m4
# https://github.com/dharple/detox/issues/31
# https://www.keil.com/support/man/docs/armclang_ref/armclang_ref_cjh1548250046139.htm
# https://developers.redhat.com/blog/2020/05/22/stack-clash-mitigation-in-gcc-part-3/
# https://gcc.gnu.org/onlinedocs/gccint/LTO-Overview.html
#

AC_DEFUN([AC_CHECK_CFLAG], [
	HOLD="$CFLAGS"
	AC_MSG_CHECKING(whether compiler supports $1)
	CFLAGS="$HOLD $1 -Werror"
	AC_COMPILE_IFELSE(
		[AC_LANG_PROGRAM(
			[[]],
			[[]]
		)],
		[
			HOLD="$CFLAGS"
			AC_MSG_RESULT(yes)
		],
		[
			AC_MSG_RESULT(no)
		]
	)
	CFLAGS="$HOLD"
])

AC_CHECK_CFLAG([[-flto=auto]])
AC_CHECK_CFLAG([[-fstack-clash-protection]])
AC_CHECK_CFLAG([[-fstack-protector-strong]])

#
# Support for check unit tests
#

#
# source: https://www.gnu.org/savannah-checkouts/gnu/autoconf/manual/autoconf-2.70/html_node/External-Software.html
#

AC_ARG_WITH([check],
	[AS_HELP_STRING([--with-check],
		[enable experimental support for check])],
	[],
	[with_check=no]
)

AS_IF([test "x$with_check" != xno], [
	PKG_CHECK_EXISTS([CHECK], [check >= 0.10.0],
		[
			# the name of this macro is deceiving, it checks to see
			# if the module is available, and configures it

			PKG_CHECK_MODULES([CHECK], [check >= 0.10.0])
			AC_DEFINE([HAVE_LIBCHECK], [1], [Define if you have libcheck])
		],
		[
			AC_MSG_FAILURE([--with-check was given, but test for check failed])
		]
	)
])
AM_CONDITIONAL([WITH_CHECK], [test "x$with_check" != xno])

#
# Support for code coverage
#

AC_ARG_WITH([coverage],
	[AS_HELP_STRING([--with-coverage],
		[enable experimental support for coverage])],
	[],
	[with_coverage=no]
)

AS_IF([test "x$with_coverage" != xno], [
	AC_CHECK_CFLAG([[-fprofile-arcs]])

	# see src/Makefile.am
	# AC_CHECK_CFLAG([[-ftest-coverage]])

	AC_DEFINE([SUPPORT_COVERAGE], [1], [Define if you want to support coverage tests])
])
AM_CONDITIONAL([WITH_COVERAGE], [test "x$with_coverage" != xno])

#
# Support for debugging
#

AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug], [Turn on debugging])],
	[case "${enableval}" in
	  yes) debug=true ;;
	  no)  debug=false ;;
	  *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
	esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])
AM_COND_IF([DEBUG], [
	AC_DEFINE([DEBUG], [1], [Enables verbose debugging in key points])
]);

#
#
#

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	tests/Makefile
	tests/legacy/Makefile
	tests/unit/Makefile
])
AC_OUTPUT
