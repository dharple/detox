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

Linting:

- astyle
- cppcheck
- mandoc
- sparse

Testing:

- check
- lcov
- printf
- sed
- strace
- valgrind

## Rebuild Internals

```bash
make
make internals
```

## Code and Content Formatting

Check code formatting:

```bash
astyle --style=kr --indent-switches --add-braces --pad-oper --pad-header $(ls src/*.[ch] | egrep -v 'config_file_(lex|yacc)')
```

Check formatting on man pages:

```bash
mandoc -T lint man/*.[15]
```

Confirm tests reference correct GitHub issue:

```bash
grep -ri github tests/ | sed -e s'/[^0-9 ]//g' -e s'/ \+/ /g' -e s'/^0\+//'
```

## Testing

### Unit and Regression Testing

```bash
./configure --with-check
make clean
make
make check
```

### Static Analysis

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
( cd tests/unit/ && make check )
make coverage
```

#### Caveats

For some reason, if you pass `-ftest-coverage` to gcc when you're linking
objects into an exectuable, whichever object is first on the command line will
have its `.gcno` file wiped out.  There is a hack in `src/Makefile.am` that
puts `-ftest-coverage` on the `DEFS` variable.

# Release Instructions

## Release

1. Update version at head of `CHANGELOG.md`.
2. Update version link at footer of `CHANGELOG.md`.
3. Update version in `configure.ac`.
4. Run `autoconf` or `make` to rebuild `configure`.
5. Commit and push.

```bash
git diff
git add -A
git commit -m "Release v1.4.0"
git tag
git tag v1.4.0
git push
git push --tags
```

6. Create a new release on GitHub, using the rendered contents of the
   CHANGELOG.
7. Play [Alwa's Legacy].

[Alwa's Legacy]: https://eldenpixels.com/alwas-legacy/
