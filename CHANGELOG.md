# CHANGELOG

All notable changes to this project will be documented in this file.

For releases after 1.3.0, the format is based on
[Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project
adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [3.0.0-beta2] - 2024-04-05

### Added

- Re-added fraction slash (0x2044) to the unicode translation table. [#106]

### Fixed

- Fixed an install error due to name collisions on the primary and legacy
  translation tables.

## [3.0.0-beta1] - 2024-03-31

### Changed

- Moved legacy translation tables to `tables/legacy/`. [#112]
- Removed the transliteration attempts in `unicode.tbl`, as well as the default
  value, so most Unicode will just pass through unchanged. [#112]
- The translation tables for CP-1252 and ISO-8859-1 now transcode characters to
  UTF-8. [#112]

### Removed

- Removed `utf_8-legacy` sequence (use `iso8859_1-legacy` instead). [#112]

## [2.0.0] - 2024-03-30

### Added

- Look for detoxrc in `$XDG_CONFIG_HOME`. [#101]

### Changed

- Enabling verbose mode is now done with either `-v` or `--verbose`.
- Renamed `.ts` files to `.template` so they don't appear as TypeScript files.

### Merged

- detox v1.4.5

## [2.0.0-beta2] - 2021-08-14

### Fixed

- `inline-detox` no longer chokes when a stream doesn't end in a newline. [#74]
- Compilation works under msys2. [#80]

### Merged

- detox v1.4.4
- detox v1.4.3
- detox v1.4.2

## [2.0.0-beta1] - 2021-03-05

### Added

- Added a new transliteration table, `unidecode.tbl`, based on
  [Text:Unidecode].  [#47] [#53]
- A new config file statement telling `detox` to use a builtin table.  [#28]
  [#50]

### Changed

- BREAKING CHANGE: Transliteration no longer happens by default.  To emulate
  the old behavior, use `detox -s utf_8`.  [#21]
- PACKAGE MAINTAINERS: The default config file and translation tables no longer
  end with `.sample`.  [#59]
- Builtin config file now matches the default supplied one.  [#42]
- Builtin translation tables are generated from `table/*.tbl`.  [#21] [#29]
- Cleaned up man pages.  PDF versions are automatically generated.  [#22]
- Code uses spaces instead of tabs.  [#44]
- CP-1252 transliteration is now done via a separate table.  [#48]
- Files and directories starting with a period are ignored during recursion.
  [#64]
- The default config file enables `remove_trailing` in the `wipeup` filter.
  [#43]
- The default config file now explicitly uses the builtin statement.  [#28]
  [#50]
- The `max_length` filter no longer acts like the wipeup filter.  Excess
  periods are not reduced within this filter.  [#46]
- The UTF-8 filter no longer behaves like the safe filter.  All characters
  between 0x20 and 0x7E are preserved.  [#40]

### Removed

- Removed obsolete BUGS from man pages.  One caveat has been moved to CAVEATS.
  [#37]
- The deprecated command line option `--remove-trailing` is now removed.  Use
  the sequence `wipeup { remove_trailing; };` instead.  [#24]

### Fixed

- Numerous internal bugs and inconsistencies.  [#21] [#31] [#41]
- The `max_length` filter recognizes files with two extensions.  [#46]
- The `safe` filter converts all ASCII control characters to `_`.  [#21] [#29]
- The `safe` filter ignores characters between 0x80 and 0xFF.  [#21] [#29]
- The `uncgi` filter converts `+` to a space as expected. [#72]
- The `utf_8` filter no longer eats a byte if an invalid Unicode sequence is
  encountered. [#72]

### Security

- Added additional compiler protection flags.  [#31]
- Symlinks that point at directories are no longer followed when `--special`
  and `-r` are specified together. [#23]
- UTF-8 encoded NULL (0x0000) values are converted to `_hidden_null_` to make
  them obvious. [#40]

## [1.4.5] - 2021-08-15

### Fixed

- Autoconf macros have been updated to support 2.70 changes. [#82]

## [1.4.4] - 2021-08-14

### Fixed

- Add explicit large file support via autoconf. [#81]

## [1.4.3] - 2021-07-24

### Fixed

- Fixed build when the `stat` struct is missing `st_blocks`. [#77]

## [1.4.2] - 2021-03-06

### Fixed

- Replaced instances of `cp -an` with `test` and `install` in the Makefile rule
  that copies `yyz.sample` to `yyz`. [#73]
- Fixed `make distcheck`. [#73]

## [1.4.1] - 2021-02-20

### Fixed

- Fixed a memory overflow bug while reading files from the command line, using
  a patch from David Tardon, which was passed on by UsernameRandomlyGenerated.
  [#56] [#sf-patch-3]

## [1.4.0] - 2021-02-11

### Added

- Regression tests for basic functionality, based on old custom scripts.
- Regression tests confirming fixes for previously fixed issues: [#14], [#19].

### Changed

- Removed one check for `.` and `..` when traversing a directory tree. [#12]
- Regenerated config file parser.
- Updated the safe filter to translate new lines, carriage returns, and tabs
  into underscores. [#9] [#11] [#17]

### Fixed

- The examples in `detox.1` no longer say `-c` when they mean `-f`. [#30]
- The command synopsis in `detox.1` and `inline-detox.1` no longer adds a dash
  before the `sequence` and `configfile`. [#30]

## [1.3.3] - 2021-02-03

### Fixed

- Fix version identifier in `detox` binary.

## [1.3.2] - 2021-01-31

### Fixed

- Table based UTF-8 translation no longer mangles characters. [#14]

## [1.3.1] - 2021-01-30

### Fixed

- Merged fix for Debian #861537, written by Vasily Kolobkov, passed on by
  Zenaan Harkness, Quentin Guittard, and Joao Eriberto Mota Filho. This
  addresses an issue with detox generating malformed characters during
  translation. [#14]

## [1.3.0] - 2017-03-04

- Migrated from `configure.in` and `Makefile.in` to the full autoconf suite. [#1]
- Remove `detox_path.h`, in favor of command line defines. [#1]
- Removed `libpopt` support. [#2]
- Fixed the way `inline-detox` is generated. [#6]
- Merged `parse_option_*.[ch]` and `file*.[ch]`. [#1], [#2], [#6]
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

        -f    set config file
        -L    list sequences
        -n    the same as --dry-run
        -s    set sequence

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

[Unreleased]: https://github.com/dharple/detox/compare/v3.0.0-beta2...main
[3.0.0-beta2]: https://github.com/dharple/detox/compare/v3.0.0-beta1...v3.0.0-beta2
[3.0.0-beta1]: https://github.com/dharple/detox/compare/v2.0.0...v3.0.0-beta1
[2.0.0]: https://github.com/dharple/detox/compare/v2.0.0-beta2...v2.0.0
[2.0.0-beta2]: https://github.com/dharple/detox/compare/v2.0.0-beta1...v2.0.0-beta2
[2.0.0-beta1]: https://github.com/dharple/detox/compare/v1.4.1...v2.0.0-beta1
[1.4.5]: https://github.com/dharple/detox/compare/v1.4.4...v1.4.5
[1.4.4]: https://github.com/dharple/detox/compare/v1.4.3...v1.4.4
[1.4.3]: https://github.com/dharple/detox/compare/v1.4.2...v1.4.3
[1.4.2]: https://github.com/dharple/detox/compare/v1.4.1...v1.4.2
[1.4.1]: https://github.com/dharple/detox/compare/v1.4.0...v1.4.1
[1.4.0]: https://github.com/dharple/detox/compare/v1.3.3...v1.4.0
[1.3.3]: https://github.com/dharple/detox/compare/v1.3.2...v1.3.3
[1.3.2]: https://github.com/dharple/detox/compare/v1.3.1...v1.3.2
[1.3.1]: https://github.com/dharple/detox/compare/v1.3.0...v1.3.1
[1.3.0]: https://github.com/dharple/detox/compare/v1.2.1...v1.3.0
[1.2.1]: https://github.com/dharple/detox/compare/v1.2.0...v1.2.1
[1.2.0]: https://github.com/dharple/detox/releases/tag/v1.2.0

[#112]: https://github.com/dharple/detox/issues/112
[#101]: https://github.com/dharple/detox/pull/101
[#82]: https://github.com/dharple/detox/issues/82
[#81]: https://github.com/dharple/detox/issues/81
[#80]: https://github.com/dharple/detox/issues/80
[#77]: https://github.com/dharple/detox/issues/77
[#74]: https://github.com/dharple/detox/issues/74
[#73]: https://github.com/dharple/detox/issues/73
[#72]: https://github.com/dharple/detox/issues/72
[#64]: https://github.com/dharple/detox/issues/64
[#59]: https://github.com/dharple/detox/issues/59
[#56]: https://github.com/dharple/detox/issues/56
[#53]: https://github.com/dharple/detox/issues/53
[#50]: https://github.com/dharple/detox/issues/50
[#48]: https://github.com/dharple/detox/issues/48
[#47]: https://github.com/dharple/detox/issues/47
[#46]: https://github.com/dharple/detox/issues/46
[#44]: https://github.com/dharple/detox/issues/44
[#43]: https://github.com/dharple/detox/issues/43
[#42]: https://github.com/dharple/detox/issues/42
[#41]: https://github.com/dharple/detox/issues/41
[#40]: https://github.com/dharple/detox/issues/40
[#37]: https://github.com/dharple/detox/issues/37
[#31]: https://github.com/dharple/detox/issues/31
[#30]: https://github.com/dharple/detox/issues/30
[#29]: https://github.com/dharple/detox/issues/29
[#28]: https://github.com/dharple/detox/issues/28
[#24]: https://github.com/dharple/detox/issues/24
[#23]: https://github.com/dharple/detox/issues/23
[#22]: https://github.com/dharple/detox/issues/22
[#21]: https://github.com/dharple/detox/issues/21
[#19]: https://github.com/dharple/detox/issues/19
[#17]: https://github.com/dharple/detox/issues/17
[#14]: https://github.com/dharple/detox/issues/14
[#12]: https://github.com/dharple/detox/issues/12
[#11]: https://github.com/dharple/detox/issues/11
[#9]: https://github.com/dharple/detox/issues/9
[#6]: https://github.com/dharple/detox/issues/6
[#2]: https://github.com/dharple/detox/issues/2
[#1]: https://github.com/dharple/detox/issues/1

[#sf-patch-3]: https://sourceforge.net/p/detox/patches/3/

[mikrosimage/detox]: https://github.com/mikrosimage/detox
[Text::Unidecode]: https://metacpan.org/pod/Text::Unidecode
