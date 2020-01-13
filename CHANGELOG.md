# CHANGELOG
All notable changes to this project will be documented in this file.

For releases after 1.3.0, the format is based on
[Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project
adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Fixed
- Merged fix for Debian #861537 from [mikrosimage/detox], written by Quentin
  Guittard. This addresses an issue with detox generating malformed UTF-8 when
  no default set has been set. [#14]

## [1.3.0] - 2017-03-04

- Migrated from `configure.in` and `Makefile.in` to the full autoconf suite. [#1]
- Remove `detox_path.h`, in favor of command line defines. [#1]
- Removed `libpopt` support. [#2]
- Fixed the way `inline-detox` is generated. [#6]
- Merged `parse_option_*.[ch]` and `file*.[ch]`. [#1, #2, #6]
- Added `--inline` as an option to `detox`, to enable inline mode on the main
  binary. [#6]

## [1.2.1] - 2017-02-27

- Migrated documents to Markdown for better presentation on github.
- Applied Debian patch `01-make-upstream-makefiles-parallel-build-safe.patch`,
  written by Patrick Schoenfeld and updated by Joao Eriberto Mota Filho.  This
  adds additional variables to the `Makefile` for safe parallel builds and GCC
  hardening.
- Applied Debian patch `02-fix-wrong-use-of-hyphens-in-manpage.patch`, written
  by Patrick Schoenfeld and updated by Joao Eriberto Mota Filho.  This fixes an
  errant "-" in the manpage, and corrects a spelling mistake.
- Applied Debian patch `03-remove-build-instructions-from-upstream-readme.patch`,
  written by Patrick Schoenfeld, in spirit.  I had already converted the
  `README` to `README.md`, so it did not apply.  I moved the compilation
  instructions into a new file, BUILD.md, instead.
- Applied Debian patch `04-change-default-sequence-to-use-utf8-table.patch`,
  written by Teemu Likonen.  This changes the default character set from
  ISO 8859-1 to UTF-8.
- Applied Debian patch `05-install-missing-file.patch`, written by
  Nelson A. de Oliveira.  This ensures that the `safe.tbl` file gets installed
  during `make install` (`make install-safe-config`).
- Applied Debian patch `06-fix-arguments.patch`, written by
  Joao Eriberto Mota Filho.  This fixes several calls to printf that were
  causing `-Werror=format-security` to fail.
- Removed CVS `$Id$` tags and updated copyright.
- Added `inline-detox.1`, from the Debian package, adapted from `detox.1` by
  Patrick Schoenfeld.
- Updated `configure` script from GNU Autoconf 2.61 to 2.69.
- Updated config file parsers; `flex` goes from 5.33 to 6.0, `bison` goes from
  2.3 to 3.0.4.
- Added a minor work around to stop compiler noise regarding `yylex()`.

## [1.2.0] - 2008-04-12

- Modified the safe filter to use a translation table.
- Modified the safe filter fallback (previous functionality) to operate without
  any special behavior.  The wipeup filter now picks up where the safe filter
  left off.
- Fixed the default permissions on install (files are 644 now).
- Updated `libpopt` support to work on Linux under the PowerPC platform (chars
  are unsigned by default).
- Included the generated `lex` and `yacc` files in the default package.
- Added additional logic to allow files on case insensitive filesystems to have
  their case changed.
- Added the ability to set locale specific translations in the translation
  tables.
- Added German specific translations to the translation tables.
- Added the ability to ignore specific files.  [sourceforge.net tracker
  #1253826]
- Fixed a bug where directories specified on the command line wouldn't get
  translated. [sourceforge.net tracker #1213623]
- Added support for translating large files.  [sourceforge.net tracker
  #1509493]
- Added inline-detox for stream based detoxification.

## 1.1.1 - 2005-03-13

- Modified `Makefile` to support parallel builds.
- Added `${DESTDIR}` to install paths, for Gentoo package builds.
- Modified the install script to not overwrite existing configuration files or
  translation tables.
- Modified the install script to install the config file and translation tables
  as `".sample"` as well as the working version, for all users, but in
  particular, to make patching the `Makefile` easier for the FreeBSD port.

## 1.1.0 - 2005-03-05

- Added lowercase filter.
- Added `libpopt` support to facilitate long options on Darwin or Solaris.
- Fixed some compiler gripes with `lex`/`yacc`.
- Replaced the hardcoded `-ll` in `Makefile.in` with `@LEXLIB@`.

## 1.0.0 - 2004-08-08

- Added a new filter for translating UTF-8 encoded Unicode characters.
- Added handling of configuration files for controlling what sequence filters
  are run in.
- Added handling of loadable translation tables, so the user can control how
  the ISO 8859-1 and Unicode filters operate.
- Added a new filter for trimming based on the max length.
- Added command line options:

		-f	set config file
		-L	list sequences
		-n	the same as --dry-run
		-s	set sequence

- Added handling for an environmental variable `DETOX_SEQUENCE`, which sets the
  default sequence name.
- Translation of some Icelandic characters has changed.  0xd0, 0xde, 0xf0,
  0xfe, the Icelandic characters for "Eth" and "Thorn" have been changed from
  "D", "Y", "o", "y" to "TH" and "th".
- Fixed translation of 0xfc (u), 0xfd (y) and 0xff (y).
- Added `.depend` generation to the `Makefile`.
- Created more man pages (`detoxrc.5` and `detox.tbl.5`).

## 0.9.1 - 2004-07-15

- Added `-d` flag to install
- Broke installation out into a script to handle differences between Solaris
  and BSD/Linux.
- Added function check for `getopt_long`.

## 0.9.0 - 2004-02-16

- Initial release

[Unreleased]: https://github.com/dharple/detox/compare/v1.3.0...develop
[1.3.0]: https://github.com/dharple/detox/compare/v1.2.1...v1.3.0
[1.2.1]: https://github.com/dharple/detox/compare/v1.2.0...v1.2.1
[1.2.0]: https://github.com/dharple/detox/releases/tag/v1.2.0

[#14]: https://github.com/dharple/detox/issues/14
[#6]: https://github.com/dharple/detox/issues/6
[#2]: https://github.com/dharple/detox/issues/2
[#1]: https://github.com/dharple/detox/issues/1

[mikrosimage/detox]: https://github.com/mikrosimage/detox
