# Build Instructions

For general instructions, please see the build instructions in `README.md`.

# Development Instructions

## Tools Used

Basic Development:

- autoconf
- automake
- bash
- bison or yacc
- flex
- gcc
- make
- php
- pkg-config or pkgconf

Linting:

- astyle
- mandoc

Static Analysis:

- cppcheck
- sparse

Testing:

- check
- lcov
- printf
- sed
- strace
- valgrind

Nice to Have:

- draw-title (from my [bin] repo)

## Rebuild Internals

```bash
make
make internals
```

## Code and Content Formatting

Check code formatting:

```bash
astyle --style=kr --indent-switches --add-braces --pad-oper --pad-header $(ls src/*.[ch] | grep -E -v 'config_file_(lex|yacc)')
```

Check formatting on man pages:

```bash
mandoc -T lint man/*.[15]
```

Check formatting on translation tables:

```bash
# if you have draw-title in your path
find table/ -type f -exec draw-title '{}' ';' -exec src/check-table '{}' ';'

# otherwise
find table/ -type f -exec echo ';' -exec src/check-table '{}' ';'
```

Confirm tests reference correct GitHub issue:

```bash
grep -ri github tests/legacy/ | sed -e s'/[^0-9 ]//g' -e s'/ \+/ /g' -e s'/^0\+//'
```

## Debugging

```bash
./configure --enable-debug --with-check
make clean
make
make internals
make check
cat tests/unit/*.log
```

## Testing

### Unit and Regression Testing

```bash
./configure --with-check
make clean
make
make internals
make check
```

### Static Analysis

Neither of these work particularly well, but it's what we have.

```bash
make clean
make
cppcheck src/*.[ch]
sparse src/*.[ch]
```

### Code Coverage

```bash
./configure --with-coverage --with-check
make clean
make
make check
make coverage
```

A directory, `coverage/`, will be created.  Open index.html to see the code
coverage report.  One of the following might work:

```bash
gio open coverage/index.html
# or
firefox  coverage/index.html
# or
chromium coverage/index.html
```

#### Code Coverage - Unit Tests Only

```bash
./configure --with-coverage --with-check
make clean
make
( cd tests/unit/ && make check )
make coverage
```

#### Code Coverage - Legacy Tests Only

```bash
./configure --with-coverage --with-check
make clean
make
( cd tests/legacy/ && make check )
make coverage
```

#### Caveats

For some reason, if you pass `-ftest-coverage` to gcc when you're linking
objects into an executable, whichever object is first on the command line will
have its `.gcno` file wiped out.  There is a hack in `src/Makefile.am` that
puts `-ftest-coverage` on the `DEFS` variable.

---

# Release Instructions

## Pre-Release

1. Review the diff.  If any manpages are in the changeset, confirm that the
   date has been updated.
2. Run the lint checks from earlier in this document, `astyle` and `mandoc`.
   Note that `astyle` sometimes confuses a multiplication operator with a
   pointer, and `mandoc` complains about missing references.
3. Run the translation table lint checker, using `find` and `check-table`, and
   look for any warnings.  The `draw-title` script is from my [bin] repo, and
   completely optional.  I just find it easier to read.
4. Run `make internals`.  Confirm no changes are recorded.

```bash
./configure && make clean && make && make internals
git status -s
```

5. Run `make distcheck`.

```bash
./configure && make clean && make && make distcheck
```

## Release

1. Update version at head of `CHANGELOG.md`.
2. Update version link at footer of `CHANGELOG.md`.
3. Update version in `configure.ac`.
4. Commit and push.

```bash
git diff
git commit -a -m "Release v1.4.0"
git tag v1.4.0
git tag # confirm your tag was created correctly
git push
git push --tags
```

5. Build tarballs.

```bash
TAR_OPTIONS="--owner=0 --group=0 --numeric-owner"
export TAR_OPTIONS
make dist
make dist-bzip2
make dist-zip
```

6. Create a new release on GitHub, using the raw contents of the CHANGELOG, and
   attaching the release files.
7. Create a new release on Sourceforge, using the same items.  In the project,
   click on Files, then the `detox` folder.  Click on Add Folder, add a version
   folder, then click in to it.  Add files to the folder.
8. Update the "latest stable" version in README.md, if appropriate.
9. Play [A Void Hope].

[A Void Hope]: https://eldenpixels.com/a-void-hope/
[bin]: https://github/dharple/bin
