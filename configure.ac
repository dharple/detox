# Process this file with autoconf to produce a configure script.
AC_INIT([detox], [2.0.0-alpha1], [detox.dharple at gmail.com], [], [https://github.com/dharple/detox])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC

AC_CHECK_FUNCS([getopt_long])

AC_CONFIG_HEADER([src/config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
