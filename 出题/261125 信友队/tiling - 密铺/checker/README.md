# QuickJS `.mjs` to single-file C packer

`packjs.py` compiles a QuickJS JavaScript module with `qjsc`, then combines its
generated C source with the local QuickJS amalgam. The resulting `.c` contains
the program and QuickJS runtime in one source file. Progress and diagnostics go
to stderr; in standard-stream mode stdout contains only C source.

## Parameters and input/output

There are two invocation forms.

### Standard input and standard output

With no positional arguments, `packjs.py` reads the `.mjs` source from stdin
and writes the generated `.c` source to stdout:

```sh
cat checker.mjs | python3 packjs.py > checker.c
```

Because stdin has no source directory, this form is intended for modules that
do not import relative files. QuickJS system modules such as `std` are fine.

### Input and output file names

With exactly two positional arguments, the first is the `.mjs` input file and
the second is the `.c` output file:

```sh
python3 packjs.py checker.mjs checker.c
```

The paths may be relative or absolute. Relative paths are interpreted from the
current working directory. The input and output paths must be different.

Any other number of arguments prints usage to stderr and exits with status 2.

## Environment variables

| Variable | Meaning |
| --- | --- |
| `QJSC_PATH` | Optional path to the `qjsc` executable, or a command name available through `PATH`. If unset, the packer uses `qjsc-linux-x86_64` beside `packjs.py`, then tries `qjsc` in `PATH`. |
| `QJAM_PATH` | Optional path to the QuickJS amalgam directory, or directly to `quickjs-amalgam.c`. If unset, the packer uses `quickjs-amalgam/` beside `packjs.py`. |

These defaults are resolved relative to `packjs.py`, not the current working
directory.

## Dependencies

- Python 3.8 or newer. No third-party Python packages are needed.
- A `qjsc` executable compatible with the QuickJS amalgam. The bundled
  `qjsc-linux-x86_64` is for x86-64 Linux; use `QJSC_PATH` for another build.
- The amalgam files `quickjs-amalgam.c`, `quickjs-libc.h`, and `quickjs.h`.
  They are in `quickjs-amalgam/` by default.
- To compile the generated C file into a program, a C compiler and the usual
  QuickJS link libraries are needed. On Linux, a typical command is:

  ```sh
  cc -O2 checker.c -lm -ldl -lpthread -o checker
  ```
