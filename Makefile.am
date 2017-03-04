SUBDIRS = src
dist_doc_DATA = README.md

dist_man1_MANS = man/*.1
dist_man5_MANS = man/*.5

dist_sysconf_DATA = etc/detoxrc.sample

dist_pkgdata_DATA = table/*

install-data-hook:
	cp -an $(DESTDIR)$(sysconfdir)/detoxrc.sample $(DESTDIR)$(sysconfdir)/detoxrc && \
	cp -an $(DESTDIR)$(pkgdatadir)/iso8859_1.tbl.sample $(DESTDIR)$(pkgdatadir)/iso8859_1.tbl && \
	cp -an $(DESTDIR)$(pkgdatadir)/safe.tbl.sample $(DESTDIR)$(pkgdatadir)/safe.tbl && \
	cp -an $(DESTDIR)$(pkgdatadir)/unicode.tbl.sample $(DESTDIR)$(pkgdatadir)/unicode.tbl

.PHONY: force-install
force-install: install
	cp -a $(DESTDIR)$(sysconfdir)/detoxrc.sample $(DESTDIR)$(sysconfdir)/detoxrc && \
	cp -a $(DESTDIR)$(pkgdatadir)/iso8859_1.tbl.sample $(DESTDIR)$(pkgdatadir)/iso8859_1.tbl && \
	cp -a $(DESTDIR)$(pkgdatadir)/safe.tbl.sample $(DESTDIR)$(pkgdatadir)/safe.tbl && \
	cp -a $(DESTDIR)$(pkgdatadir)/unicode.tbl.sample $(DESTDIR)$(pkgdatadir)/unicode.tbl

