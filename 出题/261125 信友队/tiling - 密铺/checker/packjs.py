#!/usr/bin/env python3
"""Combine a QuickJS-compiled module and the local QuickJS amalgam as C."""

import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


HERE = Path(__file__).resolve().parent
DEFAULT_QJSC = HERE / "qjsc-linux-x86_64"
DEFAULT_QJAM = HERE / "quickjs-amalgam"


def die(message, code=1):
    print(f"packjs: {message}", file=sys.stderr)
    raise SystemExit(code)


def get_qjsc():
    value = os.environ.get("QJSC_PATH")
    candidate = Path(value).expanduser() if value else DEFAULT_QJSC

    if candidate.is_file():
        return str(candidate.resolve())

    if value:
        found = shutil.which(value)
        if found:
            return found
        die(f"cannot find qjsc from QJSC_PATH={value!r}")

    found = shutil.which("qjsc")
    if found:
        return found
    die(f"qjsc not found; expected {DEFAULT_QJSC} or set QJSC_PATH")


def get_qjam_dir():
    value = os.environ.get("QJAM_PATH")
    path = Path(value).expanduser() if value else DEFAULT_QJAM
    if path.is_file() and path.name == "quickjs-amalgam.c":
        directory = path.parent
    else:
        directory = path
    directory = directory.resolve()

    required = ("quickjs-amalgam.c", "quickjs-libc.h", "quickjs.h")
    missing = [name for name in required if not (directory / name).is_file()]
    if missing:
        die(f"QuickJS amalgam files missing from {directory}: {', '.join(missing)}")
    return directory


def compile_js(qjsc, source_path, generated_c):
    command = [
        qjsc,
        "-e",       # Generate main() and bytecode.
        "-m",       # Compile as an ES module.
        "-s", "-s",  # Strip source and debug information.
        "-o", str(generated_c),
        source_path.name,
    ]

    print(f"[1/3] compiling {source_path.name} with qjsc", file=sys.stderr)
    try:
        result = subprocess.run(
            command,
            cwd=str(source_path.parent),
            stdout=sys.stderr,
            check=False,
        )
    except OSError as exc:
        die(f"cannot execute qjsc: {exc}")

    if result.returncode != 0:
        die(f"qjsc failed with exit code {result.returncode}")
    if not generated_c.is_file():
        die("qjsc did not create the expected C file")

    try:
        return generated_c.read_text(encoding="utf-8")
    except OSError as exc:
        die(f"cannot read qjsc output: {exc}")


def merge_c(amalgam_path, generated):
    print("[2/3] merging the QuickJS amalgam", file=sys.stderr)
    try:
        amalgam = amalgam_path.read_text(encoding="utf-8")
    except OSError as exc:
        die(f"cannot read QuickJS amalgam: {exc}")

    # The amalgam already includes quickjs-libc.h.
    pattern = re.compile(
        r'(?m)^[ \t]*#[ \t]*include[ \t]*'
        r'"quickjs-libc\.h"[ \t]*(?:\r?\n|$)'
    )
    generated, count = pattern.subn("", generated)
    if count == 0:
        die(
            'qjsc output has no #include "quickjs-libc.h"; '
            "check that qjsc and the amalgam are compatible"
        )

    return (
        "#define QJS_BUILD_LIBC 1\n"
        "/* Single-file QuickJS-NG application. */\n\n"
        + amalgam.rstrip()
        + "\n\n"
        + generated.lstrip()
    )


def write_output(text, output_path):
    output_path = output_path.expanduser().resolve()
    output_path.parent.mkdir(parents=True, exist_ok=True)
    temporary_path = output_path.with_name(output_path.name + ".tmp")
    try:
        temporary_path.write_text(text, encoding="utf-8")
        os.replace(temporary_path, output_path)
    except OSError as exc:
        try:
            temporary_path.unlink()
        except OSError:
            pass
        die(f"cannot write output file {output_path}: {exc}")
    print(f"[3/3] wrote {output_path}", file=sys.stderr)


def process_stdin(source_text):
    qjsc = get_qjsc()
    qjam_dir = get_qjam_dir()
    amalgam_path = qjam_dir / "quickjs-amalgam.c"

    with tempfile.TemporaryDirectory(prefix="packjs-") as temp_dir:
        temp = Path(temp_dir)
        source_path = temp / "stdin.mjs"
        generated_c = temp / "generated.c"
        source_path.write_text(source_text, encoding="utf-8")
        generated = compile_js(qjsc, source_path, generated_c)
        combined = merge_c(amalgam_path, generated)

    sys.stdout.write(combined)
    sys.stdout.flush()


def process_file(input_path, output_path):
    input_path = Path(input_path).expanduser().resolve()
    output_path = Path(output_path).expanduser().resolve()
    if not input_path.is_file():
        die(f"input file does not exist: {input_path}", 2)
    if input_path == output_path:
        die("input and output paths must be different", 2)

    try:
        source_text = input_path.read_text(encoding="utf-8")
    except OSError as exc:
        die(f"cannot read input file {input_path}: {exc}", 2)

    qjsc = get_qjsc()
    qjam_dir = get_qjam_dir()
    amalgam_path = qjam_dir / "quickjs-amalgam.c"

    with tempfile.TemporaryDirectory(prefix="packjs-") as temp_dir:
        generated_c = Path(temp_dir) / "generated.c"
        generated = compile_js(qjsc, input_path, generated_c)
        combined = merge_c(amalgam_path, generated)

    write_output(combined, output_path)


def main():
    if len(sys.argv) == 1:
        try:
            source_text = sys.stdin.read()
        except OSError as exc:
            die(f"cannot read standard input: {exc}", 2)
        process_stdin(source_text)
        return 0

    if len(sys.argv) == 3:
        process_file(sys.argv[1], sys.argv[2])
        return 0

    die(
        f"usage: {Path(sys.argv[0]).name} [<input.mjs> <output.c>]\n"
        "       with no arguments, read .mjs from stdin and write .c to stdout",
        2,
    )


if __name__ == "__main__":
    raise SystemExit(main())
