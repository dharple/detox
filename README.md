# Notice

I have renamed the `master` branch to `main`.  If you have a copy of detox
checked out, you can update your code to point at the new branch using
[these steps](https://gist.github.com/dharple/79b51d1c2fc0fea64fb84659581a6dc9).
Alternatively, you can clone a fresh copy of the repo.

# Overview

`detox` is a program that renames files to make them easier to work with under
Unix and related operating systems.  Spaces and various other unsafe
characters (such as "$") get replaced with "_".  ISO 8859-1 (Latin-1)
characters can be transliterated to ASCII, as can UTF-8 characters.
More details are contained in the `detox.1` man page.

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

# Build

To build a stable copy of `detox` from source, run:

```
git clone -b 1.x https://github.com/dharple/detox.git
cd detox
autoreconf --install
./configure
make
```

# Contact

Doug Harple <detox.dharple@gmail.com>
