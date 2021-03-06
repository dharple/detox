SUBDIRS = src
dist_doc_DATA = README.md

dist_man1_MANS = man/detox.1 man/inline-detox.1
dist_man5_MANS = man/detox.tbl.5 man/detoxrc.5

dist_sysconf_DATA = etc/detoxrc.sample

dist_pkgdata_DATA = table/iso8859_1.tbl.sample table/safe.tbl.sample table/unicode.tbl.sample

install-data-hook:
	test -f $(DESTDIR)$(sysconfdir)/detoxrc       || $(INSTALL_DATA) $(DESTDIR)$(sysconfdir)/detoxrc.sample       $(DESTDIR)$(sysconfdir)/detoxrc
	test -f $(DESTDIR)$(pkgdatadir)/iso8859_1.tbl || $(INSTALL_DATA) $(DESTDIR)$(pkgdatadir)/iso8859_1.tbl.sample $(DESTDIR)$(pkgdatadir)/iso8859_1.tbl
	test -f $(DESTDIR)$(pkgdatadir)/safe.tbl      || $(INSTALL_DATA) $(DESTDIR)$(pkgdatadir)/safe.tbl.sample      $(DESTDIR)$(pkgdatadir)/safe.tbl
	test -f $(DESTDIR)$(pkgdatadir)/unicode.tbl   || $(INSTALL_DATA) $(DESTDIR)$(pkgdatadir)/unicode.tbl.sample   $(DESTDIR)$(pkgdatadir)/unicode.tbl

.PHONY: force-install
force-install: install
	$(INSTALL_DATA) $(DESTDIR)$(sysconfdir)/detoxrc.sample       $(DESTDIR)$(sysconfdir)/detoxrc
	$(INSTALL_DATA) $(DESTDIR)$(pkgdatadir)/iso8859_1.tbl.sample $(DESTDIR)$(pkgdatadir)/iso8859_1.tbl
	$(INSTALL_DATA) $(DESTDIR)$(pkgdatadir)/safe.tbl.sample      $(DESTDIR)$(pkgdatadir)/safe.tbl
	$(INSTALL_DATA) $(DESTDIR)$(pkgdatadir)/unicode.tbl.sample   $(DESTDIR)$(pkgdatadir)/unicode.tbl

uninstall-hook:
	rm $(DESTDIR)$(sysconfdir)/detoxrc
	rm $(DESTDIR)$(pkgdatadir)/iso8859_1.tbl
	rm $(DESTDIR)$(pkgdatadir)/safe.tbl
	rm $(DESTDIR)$(pkgdatadir)/unicode.tbl
