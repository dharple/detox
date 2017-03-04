SUBDIRS = src
dist_doc_DATA = README.md

dist_man1_MANS = man/*.1
dist_man5_MANS = man/*.5

dist_sysconf_DATA = etc/detoxrc.sample

dist_pkgdata_DATA = table/*

install-data-hook:
	cp -an $(DESTDIR)$(sysconfdir)/detoxrc.sample $(DESTDIR)$(sysconfdir)/detoxrc
