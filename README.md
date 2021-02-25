# Notice

I have renamed the `master` branch to `main`.  If you have a copy of detox
checked out, you can update your code to point at the new branch using
[these steps](https://gist.github.com/dharple/79b51d1c2fc0fea64fb84659581a6dc9).
Alternatively, you can clone a fresh copy of the repo.

Also, if you have a current copy of `detox` checked out, please note that I
have committed the output from `autoreconf --install`, and you no longer need
to take that step to build `detox`.

If you encounter a problem running `git pull`, you can run `make
maintainer-clean`, which will delete all of the files added by `autoreconf
--install` (as well as some built files in `src/`).  If that doesn't work, I
recommend pulling a fresh copy of the repo.

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

To build a stable copy of `detox`, run:

```
wget https://github.com/dharple/detox/releases/download/v1.4.1/detox-1.4.1.tar.gz
tar xzvf detox-1.4.1.tar.gz
cd detox-1.4.1
./configure
make
make install
```

To build a stable copy of `detox` from source, run:

```
git clone -b 1.x https://github.com/dharple/detox.git
cd detox
autoreconf --install
./configure
make
make install
```

To build the latest copy of `detox` from source, follow the instructions in
`BUILD.md`.

To remove a copy of `detox` that was installed via these methods, from the same
directory that you ran `make install`, run:

```
make uninstall
```

# Contact

Doug Harple <detox.dharple@gmail.com>
