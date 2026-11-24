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
  cc -O2 checker.c -lm -pthread -ldl -o checker
  ```

  With glibc 2.34 or newer, `-pthread` and `-ldl` can be omitted because
  those libraries' functionality is integrated into `libc`. Keep them when
  targeting older glibc versions; `-lm` is still needed.

### Windows

The generated source sets `_WIN32_WINNT` and `WINVER` to Windows 7 by default,
before including the QuickJS amalgam. This makes MinGW expose the Windows
one-time initialization and condition-variable APIs used by QuickJS-NG. Compile
the `.c` file as C (use `-std=c11`, not `-std=c++11`):

```bat
gcc -O2 -std=c11 -static checker.c -lm -o checker.exe
```

The executable targets Windows 7 or later. If the compiler command line defines
these version macros itself, set both to at least `0x0601`:

```bat
gcc -O2 -std=c11 -D_WIN32_WINNT=0x0601 -DWINVER=0x0601 -static checker.c -lm -o checker.exe
```

MinGW links the required Windows system APIs from `kernel32` automatically.
The compiler installation must include Windows headers and import libraries
that declare and provide `InitOnceExecuteOnce`, `InitializeConditionVariable`,
`WakeConditionVariable`, `WakeAllConditionVariable`, and
`SleepConditionVariableCS`.
