
EXTRA_DIST = \
	character-helper.sh \
	github-issue-0011 \
	github-issue-0012 \
	github-issue-0014 \
	github-issue-0019 \
	github-issue-0020 \
	github-issue-0021 \
	github-issue-0023 \
	github-issue-0028 \
	github-issue-0037 \
	github-issue-0043 \
	github-issue-0050-invalid \
	github-issue-0056 \
	github-issue-0062 \
	github-issue-0064 \
	legacy-dir \
	man-page-example \
	man-page-sequence-with-language \
	test-functions.sh \
	test.sh

check:
	if [ -x $(top_builddir)/tests/legacy/test.sh ] ; then $(top_builddir)/tests/legacy/test.sh $(top_builddir)/src/detox ; else $(top_builddir)/src/detox -V ; $(top_builddir)/src/detox -L -v ; fi
