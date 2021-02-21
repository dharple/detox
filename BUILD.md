# Compilation Instructions

## Tools Needed

- automake
- gcc or clang
- make

Compilation:

        autoreconf --install
        ./configure
        make

Installation:

        make install

Installation that overwrites config files and translation tables:

        make force-install

# Development Instructions

## Tools Used

- astyle
- bison
- cppcheck
- flex
- mandoc
- sparse

## Generating Docs

Install `mandoc` to generate fresh PDFs.

## Formatting

Check code:

```
astyle --style=kr --indent-switches --add-braces --pad-oper --pad-header $(ls src/*.[ch] | egrep -v 'config_file_(lex|yacc)')
```

Check docs:

```
mandoc -T lint man/*.[15]
```

Check tests:
```
grep -ri github tests/ | sed -e s'/[^0-9 ]//g' -e s'/ \+/ /g' -e s'/^0\+//'
```

## Testing

1. Run static analysis tools. (sparse, cppcheck)
2. Run `make clean ; make` to rebuild `detox`.
3. Run `tests/test.sh src/detox` to perform basic regression tests.
4. Run custom external regression tests.

# Release Instructions

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

6. Create a new release on GitHub, using the rendered contents of the
   CHANGELOG.
7. Play [Alwa's Legacy].

[Alwa's Legacy]: https://eldenpixels.com/alwas-legacy/
