
EXTRA_DIST = \
	*.sh \
	github-issue-* \
	legacy-dir \
	man-page-*

check:
	if [ -x $(top_builddir)/tests/legacy/test.sh ] ; then $(top_builddir)/tests/legacy/test.sh $(top_builddir)/src/detox ; else $(top_builddir)/src/detox -V ; $(top_builddir)/src/detox -L -v ; fi
