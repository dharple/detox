SUBDIRS = src
dist_doc_DATA = README.md CHANGELOG.md

dist_man1_MANS = man/detox.1 man/inline-detox.1
dist_man5_MANS = man/detoxrc.5 man/detox.tbl.5

dist_sysconf_DATA = etc/detoxrc

dist_pkgdata_DATA = \
	table/cp1252.tbl \
	table/iso8859_1.tbl \
	table/safe.tbl \
	table/unicode.tbl \
	table/unidecode.tbl

EXTRA_DIST = man/detox.1.pdf man/inline-detox.1.pdf man/detoxrc.5.pdf man/detox.tbl.5.pdf

internals:
	bin/generate.sh

check:
	if [ -x $(top_srcdir)/tests/test.sh ] ; then $(top_srcdir)/tests/test.sh src/detox ; else src/detox -V ; src/detox -L -v ; fi

valgrind:
	USE_VALGRIND=1 $(top_srcdir)/tests/test.sh src/detox
