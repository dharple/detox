SUBDIRS = src tests
dist_doc_DATA = \
	BUILD.md \
	CHANGELOG.md \
	LICENSE \
	README.md \
	THANKS.md

dist_man1_MANS = man/detox.1 man/inline-detox.1
dist_man5_MANS = man/detoxrc.5 man/detox.tbl.5

dist_sysconf_DATA = etc/detoxrc

dist_pkgdata_DATA = \
	table/cp1252.tbl \
	table/iso8859_1.tbl \
	table/safe.tbl \
	table/unicode.tbl

legacydir = $(pkgdatadir)/legacy

dist_legacy_DATA = \
	table/legacy/cp1252.tbl \
	table/legacy/iso8859_1.tbl \
	table/legacy/unicode.tbl \
	table/legacy/unidecode.tbl

EXTRA_DIST = man/detox.1.pdf man/inline-detox.1.pdf man/detoxrc.5.pdf man/detox.tbl.5.pdf

internals:
	bin/generate-builtin.sh || true
	bin/generate-legacy-tests.sh || true
	bin/generate-pdf.sh || true
	bin/generate-unit-tests.sh || true

valgrind:
	USE_VALGRIND=1 $(top_srcdir)/tests/test.sh src/detox

#
# code coverage rules
#

if WITH_COVERAGE

coverage:
	lcov --capture --directory src --output-file coverage.info
	genhtml coverage.info --output-directory coverage

coverage-text:
	cd src && make coverage-text

else

coverage:
	echo "code coverage is not enabled; run ./configure --with-coverage"


coverage-text: coverage

endif # WITH_COVERAGE

clean-local:
	rm -rf coverage.info coverage/
