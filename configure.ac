# Process this file with autoconf to produce a configure script.
AC_INIT([detox], [2.0.0-alpha1], [detox.dharple at gmail.com], [], [https://github.com/dharple/detox])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC

AC_CHECK_FUNCS([getopt_long])

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
	CFLAGS="$HOLD $1"
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
#
#

AC_CONFIG_HEADER([src/config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
