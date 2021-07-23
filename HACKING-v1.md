# Hacking Detox v1.x

## Step 1 - Confirm you're using detox v1.x

The instructions are slightly different for version 2.

Run `detox -V` to see the version you're using.  If you're using an early copy
of version 2, please reach out to me via GitHub issues.

## Step 2 - Find Your Config Files

Run `detox -L -v | head`.  You'll see output that looks something like this:

```
sequence name: default (*)
        source file: /etc/detoxrc
        cleaner: utf_8
        cleaner: safe
        cleaner: wipeup
sequence name: iso8859_1
        source file: /etc/detoxrc
        cleaner: iso8859_1
        cleaner: safe
        cleaner: wipeup
```

The `source file:` line tells us where detox's config file is.

Chances are the config file is `/etc/detoxrc` or `/usr/local/etc/detoxrc`.

## Step 3 - Find Your Translation Tables

If your config file is `/etc/detoxrc`, your translation tables should be in
`/usr/share/detox/`.

If your config file is `/usr/local/etc/detoxrc`, your translation tables should
be in `/usr/local/share/detox/`.

## Step 4 - Edit Your Translation Tables

Note the `default` sequence above; this is the sequence that detox runs if you
don't specify the sequence to run on the command line.

The sequence above runs three filters.  The first filter, `utf_8`, takes
multibyte characters and transliterates them to 7-bit ASCII characters.  The
second filter, `safe`, is what translates characters to `-` or `_`.  The third
filter, `wipeup`, will deduplicate cases of `-` and `_`, and sometimes `.`.

If you want to change the way detox cleans up characters, you'll want to edit
the translation table `safe.tbl`, in the location you found.

Edit `safe.tbl` in your favorite editor.  For instance:

```
cd /usr/share/detox
sudo cp safe.tbl original-safe.tbl
sudo vi safe.tbl # or nano, emacs, gedit, whatever
```

## Step 5 - Figuring Out What To Change

The translation tables are simple in concept, but potentially overwhelming at
first.

For instance, this line turns a space into an underscore.

```
0x20            _       # space
```

You can tell that it's operating on a space one of two ways.  I've left
comments next to any entry that's changed by default.

For any entry that isn't changed, the value will be the same as its Latin-1
(ISO 8859-1) equivalent.  For instance:

```
0x36            6
```

Leaves the number 6 alone.

## Step 6 - Making Changes

To make detox change a space into a period, instead of an underscore, find the
line that starts with 0x20 and change it to this:

```
0x20            .       # space
```

To make detox change the number 6 into the word six, find the line that starts
with 0x36 and change it to this:

```
0x36            six     # 6
```

## Step 7 - Testing

Save your changes and open a new terminal.

Use `inline-detox` to test your change.  If you don't have `inline-detox`, you
can just run `detox --inline` and it'll do the same thing.

```
echo "this is the 6th test" | inline-detox
```

The old output would be:
```
this_is_the_6th_test
```

The new output will now be:
```
this.is.the.sixth.test
```

## Conclusion

Hopefully this helps you get started with changing the behavior of detox.  If
not, please reach out via GitHub issues.
