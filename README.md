# Overview

`detox` is a program that renames files to make them easier to work with under
Linux and other Unix-like operating systems.  Spaces and various other unsafe
characters (such as "`$`") get replaced with "`_`".  The upper portion of
ISO-8859-1 (Latin-1) can be transcoded to UTF-8, as can CP-1252.

More details are contained in the `detox.1` [man page].

---

# Notice for Package Maintainers

Version 2 and up have a new dependency for package builds: `pkg-config` or
`pkgconf`.

You may also need to add a dependency on `libtool`.  Please let me know if you
do, by creating an [issue].  I'll update the docs accordingly.

Also, the default config file and translation tables are no longer prefixed
with `.sample`.

Please open an [issue] for any build problems encountered.  Thanks!

# Notice for Everyone

As of version 3, `detox` will no longer try to transliterate all of Unicode
into the ASCII character space.  The focus will be on truly problematic
characters.

Older releases and version-specific branches are still available if you need
that functionality.  During this transition, the old tables are also available
in `table/legacy/`

---

# Runtime Notes

The most important option to learn is `-n`, aka `--dry-run`.  This will let you
run `detox` without actually changing any files, so that you can get an idea
of what `detox` is all about.

The simplest way to run `detox` is to just run it on a directory containing
files that need work:

```
detox xfer_files/
```

You can also just to specify the filename:

```
detox my\ bad\ file.txt
```

You can also specify recursion (this works best on directories):

```
detox -r /music/transferred_from_elsewhere/
```

# Building Detox

Many distributions include a copy of detox with their packages.  If you wish to
build it from scratch, you'll need the following tools:

- autoconf
- automake
- bison / yacc / byacc
- flex / lex
- gcc / clang
- make
- pkg-config / pkgconf

## Prerequisites

To install the needed packages on Debian, Ubuntu, Mint, and other Debian
derivatives, run:

```
sudo apt install autoconf automake bison flex gcc make pkg-config
```

To install the needed packages on macOS, run:
```
brew install autoconf automake bison flex gcc make pkg-config
```

On FreeBSD, run:
```
sudo pkg install autoconf automake gcc pkgconf wget
```

On NetBSD, run:
```
sudo pkgin install autoconf automake mozilla-rootcerts pkgconf wget
sudo mozilla-rootcerts install
```

On MSYS2, run:
```
pacman -S --needed base-devel gcc git mingw-w64-x86_64-toolchain
```

## Install Stable from GitHub Package

To build a stable copy of `detox`, run:

```
wget https://github.com/dharple/detox/releases/download/v2.0.0/detox-2.0.0.tar.gz
tar xzvf detox-2.0.0.tar.gz
cd detox-2.0.0
./configure
make
make install
```

If that fails during the configure or make steps, run:
```
autoreconf --install
./configure
make
make install
```

## Install Stable from Repository

To build a stable copy of `detox` from source, install the package `git`, then
run:

```
git clone -b 1.x https://github.com/dharple/detox.git
cd detox
autoreconf --install
./configure
make
make install
```

## Install Development from Repository

To build a development copy of `detox` from source, install the package `git`,
then run:

```
git clone -b main https://github.com/dharple/detox.git
cd detox
autoreconf --install
./configure
make
make install
```

## Uninstall

To remove a copy of `detox` that was installed via these methods, from the same
directory that you ran `make install`, run:

```
make uninstall
```

# Contact

For support, to report a defect, or to request a new feature, please use the
[GitHub Issues system].

[GitHub Issues system]: https://github.com/dharple/detox/issues/
[issue]: https://github.com/dharple/detox/issues/
[man page]: https://raw.githubusercontent.com/dharple/detox/main/man/detox.1.pdf
