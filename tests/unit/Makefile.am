if BUILDCHECK

TESTS = \
	test_clean_lower \
	test_clean_safe \
	test_clean_wipeup

check_PROGRAMS = \
	test_clean_lower \
	test_clean_safe \
	test_clean_wipeup

test_clean_lower_CFLAGS = @CHECK_CFLAGS@
test_clean_lower_LDADD = \
	$(top_builddir)/src/clean_string.o \
	@CHECK_LIBS@

test_clean_safe_CFLAGS = @CHECK_CFLAGS@
test_clean_safe_LDADD = \
	$(top_builddir)/src/clean_string.o \
	$(top_builddir)/src/table.o \
	$(top_builddir)/src/builtin_table.o \
	@CHECK_LIBS@

test_clean_wipeup_CFLAGS = @CHECK_CFLAGS@
test_clean_wipeup_LDADD = \
	$(top_builddir)/src/clean_string.o \
	@CHECK_LIBS@

endif # BUILDCHECK
