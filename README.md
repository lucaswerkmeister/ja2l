# ja2l

JSON array to lines. Expects a JSON array like this on standard input:

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

## Attribution

The `cleanup.h` header file is based on systemd header files,
which are published under the LGPL2.1+.

## License

The content of this repository is released under the AGPL3+
as provided in the LICENSE file that accompanied this code.

By submitting a “pull request” or otherwise contributing to
this repository, you agree to license your contribution under
the license mentioned above.

[jq]: https://stedolan.github.io/jq/
