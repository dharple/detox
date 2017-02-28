# Thanks

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
everything 755, for pointing out that not everyone has lex or yacc, and for
his input on how umlauts should be converted.

Gerg Thor informed me that characters on the PowerPC platform are unsigned
by default and that the libpopt parser was rolling into an infinite loop as
a result.

Jon Amundsen pointed out that the lowercase converting sequence was only
really working on case-insensitive filesystems.

zero_dogg from sourceforget.net suggested adding the ability to prevent
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


