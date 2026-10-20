#!/usr/bin/env python3
"""Build reproducible organizer/player archives with SHA-256 checksums."""
import argparse
import json
from pathlib import Path
import zipfile
from common import ROOT, sha256


def archive(target, entries):
    with zipfile.ZipFile(target, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=6) as z:
        for source, name in sorted(entries, key=lambda pair: pair[1]):
            info = zipfile.ZipInfo(name, date_time=(2026, 10, 21, 0, 0, 0))
            info.compress_type = zipfile.ZIP_DEFLATED
            info.external_attr = 0o644 << 16
            z.writestr(info, source.read_bytes(), compress_type=zipfile.ZIP_DEFLATED,
                       compresslevel=6)
    with zipfile.ZipFile(target) as z:
        if z.testzip():
            raise AssertionError("Damaged archive")


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--output-dir", type=Path, default=ROOT.parent)
    args = ap.parse_args()
    args.output_dir.mkdir(parents=True, exist_ok=True)
    private = []
    for path in ROOT.rglob("*"):
        if not path.is_file():
            continue
        rel = path.relative_to(ROOT)
        if any(part in ("build", "work", "__pycache__", ".git") for part in rel.parts):
            continue
        if path.suffix in (".exe", ".o", ".obj", ".pyc", ".zip") or path.name == "FILE_SHA256SUMS.txt":
            continue
        private.append((path, "flexible_storage_v2/" + rel.as_posix()))
    checksums = ROOT / "FILE_SHA256SUMS.txt"
    checksums.write_text("".join(f"{sha256(p)}  {p.relative_to(ROOT).as_posix()}\n"
                                for p, _ in sorted(private, key=lambda x: x[1])),
                         encoding="ascii")
    private.append((checksums, "flexible_storage_v2/FILE_SHA256SUMS.txt"))
    public = [(ROOT / "statement.md", "statement.md"),
              (ROOT / "contestant_README.md", "README.md")]
    public += [(p, p.relative_to(ROOT).as_posix()) for p in (ROOT / "samples").rglob("*")
               if p.is_file() and p.suffix in (".in", ".ans")]
    manifest = json.loads((ROOT / "manifest.json").read_text(encoding="utf-8"))
    for case in manifest["cases"]:
        if case["public"]:
            for kind in ("input", "answer"):
                p = ROOT / case[kind]
                public.append((p, "samples/large/" + p.name))
    organizer = args.output_dir / "flexible_storage_organizer_v2.zip"
    contestant = args.output_dir / "flexible_storage_contestant_v2.zip"
    archive(organizer, private)
    archive(contestant, public)
    with zipfile.ZipFile(contestant) as z:
        assert all(name in ("statement.md", "README.md") or name.startswith("samples/")
                   for name in z.namelist())
    hashes = args.output_dir / "flexible_storage_archives_sha256.txt"
    hashes.write_text("".join(f"{sha256(p)}  {p.name}\n" for p in (organizer, contestant)),
                      encoding="ascii")
    for p in (organizer, contestant):
        print(f"{p}: {p.stat().st_size} bytes")


if __name__ == "__main__":
    main()
