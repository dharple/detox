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
	$(top_builddir)/src/detox_struct.o \
	$(top_builddir)/src/table.o \
	@CHECK_LIBS@

TESTS = \
	test_clean_lower \
	test_clean_max_length \
	test_clean_safe \
	test_clean_wipeup

check_PROGRAMS = $(TESTS)

endif # WITH_CHECK

clean-local:
	rm -f *.gcov *.gcno *.gcda
