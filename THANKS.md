# Thanks

*If I missed you, please let me know!*

ninedotnine on github added the ability to load config files from
`$XDG_CONFIG_HOME`.

a1346054 on github contributed maintenance improvments.

Thomas Klausner reminded me that `cp -n` does not work on NetBSD.

Sean M. Burke for [Text::Unidecode], which helped with Unicode transliteration
and became a new table, `unidecode.tbl`.  Additionally, Behat's [PHP
transliteration library] put Text::Unidecode in a format I was comfortable
working with.

K Lange's awesome `AC_C_COMPILE_FLAGS` macro helped me write my own, updated
version of the same feature.

Hans-Peter Jansen gave me the impetus I needed to finally fix the char /
unsigned char differences throughout the code.  Also, he provided a list of
security-based compiler flags.

David Tardon fixed a memory overflow bug, which was passed to me by
UsernameRandomlyGenerated.

Mplx let me know that `detox.1` had a typo in the examples.

Daniel Hauck for letting me know that passing `.` as an argument was not
working.

Kanliot and Tyler Adams for reporting on issues with newlines and spaces
causing problems.

Thank you to Vasily Kolobkov, Zenaan Harkness, Quentin Guittard, Joao Eriberto
Mota Filho, and Andrew Berezovskyi for the fix addressing the issue with detox
generating malformed characters during a translation.

Sanjaymsh added the PowerPC architecture to the Travis tests.

Special thanks to Patrick Schoenfeld and Joao Eriberto Mota Filho, for
maintaining the Debian version.  Release v1.2.1 is almost entirely comprised of
their patches.

Many thanks to Miguel Angelo Rozsas for suggesting UTF-8 support and his
valuable input following that suggestion.  This change helped introduce
translation tables into detox.

Ciaran McCreesh put together a package for Gentoo (rock!), submitted a patch
to add a prefix onto install paths (for package builds) and helped fix
parallel builds.

Lou Alfonso for suggesting that the safe filter be controlled through a
table so that it can be tuned easily.

Christoph Wegscheider noticed that the install script was installing
everything 0755, for pointing out that not everyone has lex or yacc, and for
his input on how umlauts should be converted.

Gerg Thor informed me that characters on the PowerPC platform are unsigned
by default and that the libpopt parser was rolling into an infinite loop as
a result.

Jon Amundsen pointed out that the lowercase converting sequence was only
really working on case-insensitive filesystems.

zero_dogg from sourceforge.net suggested adding the ability to prevent
certain files from being translated.

rsnemmen from sourceforge.net pointed out a bug in the way directories are
handled when passed in from the command line.

# Additional Thanks

Eric S. Raymond for his work on "The Art of UNIX Programming"

http://www.bbsinc.com/iso8859.html for their help in building a complete list
of Latin-1 translations.

SourceForge.net for their generous hosting of this and many other projects.

http://en.wikipedia.org/wiki/UTF-8 for its help with explaining UTF-8.

Data Structures Using C - Tenenbaum, Langsam and Augenstein - for their help
with hashes.

Paul Oakenfold for his unbelievable mixes.  Version 1.0.0 was developed with
the help of his Great Wall mix.

[PHP transliteration library]: https://github.com/Behat/Transliterator
[Text::Unidecode]: https://metacpan.org/pod/Text::Unidecode
