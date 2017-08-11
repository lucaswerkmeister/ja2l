# ja2l

[![Build Status](https://travis-ci.org/lucaswerkmeister/ja2l.svg?branch=master)](https://travis-ci.org/lucaswerkmeister/ja2l)

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

Functional, but more tests and various other goodies would be nice.

## Build instructions

`make && sudo make install`. Needs glibc.
I use `gcc`, but so far `clang` also seems to work.

The Makefile follows standard GNU conventions;
for instance, packagers can use something like
`make DESTDIR="$pkgdir" prefix=/usr install`.
(Note that standard `$(bindir)`, `$(man1dir)`, etc. directories
are expected to exist,
because the GNU Make manual says to not use the nonstandard `mkdir -p`
and I don’t know how to create the directories without it.)

## Requirements

* glibc (`error`, `getopt_long`)
* Linux (`/proc/sys/fs/pipe-max-size`, `fcntl(F_SETPIPE_SZ)`)
* optional: [dgsh]

The reason lists in parentheses are probably not exhaustive.
(The most likely reason for that is that
I’ll probably forget to update them as I update the program.)

## dgsh support

ja2l can be built with [dgsh] support.
When used in a dgsh pipeline,
it accepts zero or one inputs
(depending on whether or not a file name was specified on the command line)
and scatters the JSON values across any (nonzero) number of outputs.

To build ja2l with dgsh support,
add `-DUSE_DGSH` to the `CFLAGS`
and `-ldgsh` to the `LDLIBS`,
e. g. like this:

```sh
make CFLAGS=-DUSE_DGSH LDLIBS=-ldgsh clean all
```

This can be used to speed up processing of the JSON data with [jq],
similar to this script:

```sh
function countElements {
    jq -r '
      .elements |
      .[]
    ' | awk '
      {
        a[$0]++
      }
      END {
        for (k in a)
          print a[k] "\t" k
      }
    '
}

function summarizeElements {
    awk -F'\t' '
      {
        a[$2] += $1
      }
      END {
          for (k in a)
            if (a[k] >= 1000)
              print a[k] "\t" k
      }
    ' |
    sort -nr
}

ja2l | {{
        countElements &
        countElements &
        countElements &
        countElements &
    }} |
    cat |
    summarizeElements
```

This prints the most common “elements” in the JSON input,
parallelizing the extraction and counting of elements across four `countElements` invocations.
The results of those invocations are then aggregated into a single result list again.
If the processing is CPU-bound,
`jq` is the expensive part,
and you have four processors or processor cores (without counting hyper-threading),
this should speed up processing by about a factor of four.

Note that dgsh must be made aware that `ja2l` supports dgsh.
See the workaround in [dspinellis/dgsh#87] for one way to do this.

## Attribution

The `cleanup.h` header file is based on systemd header files,
which are published under the LGPL2.1+.

The install-related variables in the `Makefile` are copied from the GNU Make manual,
which is published under the FDL1.3+.

## License

The content of this repository is released under the AGPL3+
as provided in the LICENSE file that accompanied this code.

By submitting a “pull request” or otherwise contributing to
this repository, you agree to license your contribution under
the license mentioned above.

[dgsh]: https://www.spinellis.gr/sw/dgsh/
[jq]: https://stedolan.github.io/jq/
[dspinellis/dgsh#87]: https://github.com/dspinellis/dgsh/issues/87
