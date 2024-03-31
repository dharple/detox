if WITH_CHECK

AM_CFLAGS = \
	@CHECK_CFLAGS@ \
	-DYY_NO_INPUT \
	-DYY_NO_UNPUT \
	-D_FORTIFY_SOURCE=2 \
	-Wall \
	-Werror

AM_LDFLAGS = \
	$(top_builddir)/src/builtin_table.o \
	$(top_builddir)/src/clean_string.o \
	$(top_builddir)/src/clean_utf_8.o \
	$(top_builddir)/src/config_file_lex.o \
	$(top_builddir)/src/config_file.o \
	$(top_builddir)/src/config_file_spoof.o \
	$(top_builddir)/src/config_file_yacc.o \
	$(top_builddir)/src/file.o \
	$(top_builddir)/src/filelist.o \
	$(top_builddir)/src/filter.o \
	$(top_builddir)/src/parse_options.o \
	$(top_builddir)/src/parse_table.o \
	$(top_builddir)/src/sequence.o \
	$(top_builddir)/src/table.o \
	$(top_builddir)/src/wrapped.o \
	@CHECK_LIBS@

TESTS = \
	test_clean_iso8859_1 \
	test_clean_iso8859_1_cp1252 \
	test_clean_lower \
	test_clean_max_length \
	test_clean_safe \
	test_clean_uncgi \
	test_clean_utf_8 \
	test_clean_wipeup \
	test_filelist \
	test_parse_inline \
	test_spoof_config_file \
	test_table_max_length \
	test_table_resize \
	test_wrapped

check_PROGRAMS = $(TESTS)

endif # WITH_CHECK

clean-local:
	rm -f *.gcov *.gcno *.gcda
