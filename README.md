# ja2l

JSON array to lines.
Expects a JSON array like this on standard input:

```json
[
{"foo":"bar"},
{"abc":"xyz"},
{"whatever":0}
]
```

and converts it into a stream of JSON values on standard output:

```json
{"foo":"bar"}
{"abc":"xyz"}
{"whatever":0}
```

This stream is suitable for processing with [jq],
and can also be split up for parallelized processing.

## Project status

Functional, but I plan to add [dgsh] support
for efficient parallel processing of the JSON stream
(slightly more efficient than piping into `dgsh-tee -s`).
Tests, a manpage, etc. would also be nice.

## Build instructions

`make`. Needs glibc.
I use `gcc`, but so far `clang` also seems to work.

There’s no `make install` yet,
though I probably won’t refuse a pull request to add it.

## Requirements

* glibc (`error`, `getopt_long`)
* Linux (`/proc/sys/fs/pipe-max-size`, `fcntl(F_SETPIPE_SZ)`)

The reason lists in parentheses are probably not exhaustive.
(The most likely reason for that is that
I’ll probably forget to update them as I update the program.)

## Attribution

The `cleanup.h` header file is based on systemd header files,
which are published under the LGPL2.1+.

## License

The content of this repository is released under the AGPL3+
as provided in the LICENSE file that accompanied this code.

By submitting a “pull request” or otherwise contributing to
this repository, you agree to license your contribution under
the license mentioned above.

[dgsh]: https://www.spinellis.gr/sw/dgsh/
[jq]: https://stedolan.github.io/jq/
