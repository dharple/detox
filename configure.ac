# Process this file with autoconf to produce a configure script.
AC_INIT([detox], [1.4.5], [detox.dharple at gmail.com], [], [https://github.com/dharple/detox])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC
AC_PROG_LEX([noyywrap])
AC_PROG_YACC

AC_CHECK_FUNCS([getopt_long])
AC_STRUCT_ST_BLOCKS

AC_SYS_LARGEFILE

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
