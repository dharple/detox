# Compilation instructions

Compilation:

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

---

	aclocal
	autoconf
	automake --add-missing --copy
	./configure
	make
