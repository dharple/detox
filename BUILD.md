# Compilation Instructions

Compilation:

	autoreconf --install
	./configure
	make

Installation:

	make install

Installation that overwrites config files and translation tables:

	make force-install

# Compilation notes

Ensure that the path to lex/yacc (or an equivalent) is in your $PATH
environmental variable.  For instance, under Solaris, you may need to add
/usr/ccs/bin to your path to get detox to compile:

	export PATH=${PATH}:/usr/ccs/bin

---

To get detox working with long options on Darwin or Solaris (or any other OS
without getopt_long in its C library), I've included support for libpopt.
Install libpopt and configure with the following:

	./configure --with-popt

If you've installed libpopt in a directory other than /usr, which is likely,
you'll want to specify the base path to libpopt.  So, if popt.h is in
/usr/local/include, you'll run configure like so:

	./configure --with-popt=/usr/local

# Release Instructions

## Testing

1. Run static analysis tools.
2. Run `make clean ; make` to rebuild `detox`.
3. Run `tests/test.sh src/detox` to perform basic regression tests.
4. Run custom external regression tests.

## Release

1. Update version at head of `CHANGELOG.md`.
2. Update version link at footer of `CHANGELOG.md`.
3. Update version in `configure.ac`.
4. Run `autoconf` or `make` to rebuild `configure`.
5. Commit and push.

        git diff
        git add -A
        git commit -m "Release v1.4.0"
        git tag
        git tag v1.4.0
        git push
        git push --tags

6. Build tarballs.

        TAR_OPTIONS="--owner=0 --group=0 --numeric-owner"
        export TAR_OPTIONS
        make dist
        make dist-bzip2
        make dist-zip

7. Create a new release on GitHub, using the rendered contents of the
   CHANGELOG, and attaching the release files.
8. Create a new release on Sourceforge, using the same items.
9. Update the "latest stable" version in README.md, if appropriate.
10. Play [Alwa's Legacy].

[Alwa's Legacy]: https://eldenpixels.com/alwas-legacy/
