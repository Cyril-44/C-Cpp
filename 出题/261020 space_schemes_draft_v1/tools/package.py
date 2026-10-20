#!/usr/bin/env python3
"""Create reproducible organizer/player ZIP archives and archive checksums."""
import argparse
import zipfile
from pathlib import Path

from common import ROOT, sha256


def write_archive(target, entries):
    with zipfile.ZipFile(target, 'w', compression=zipfile.ZIP_DEFLATED, compresslevel=6) as archive:
        for source, name in sorted(entries, key=lambda x: x[1]):
            info = zipfile.ZipInfo(name, date_time=(2026, 10, 20, 0, 0, 0))
            info.compress_type = zipfile.ZIP_DEFLATED
            info.external_attr = 0o644 << 16
            archive.writestr(info, source.read_bytes(), compress_type=zipfile.ZIP_DEFLATED, compresslevel=6)
    with zipfile.ZipFile(target) as archive:
        bad = archive.testzip()
        if bad: raise RuntimeError('corrupt ZIP member: ' + bad)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output-dir', type=Path, default=ROOT.parent)
    args = parser.parse_args()
    output_dir = args.output_dir.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)
    organizer = output_dir / 'space_schemes_organizer_draft_v1.zip'
    contestant = output_dir / 'space_schemes_contestant_draft_v1.zip'
    private_entries = []
    for path in ROOT.rglob('*'):
        if not path.is_file(): continue
        relative = path.relative_to(ROOT)
        if any(part in ('build', '__pycache__', '.git') for part in relative.parts): continue
        if path.suffix in ('.exe', '.o', '.obj', '.pyc', '.zip'): continue
        if path.name in ('space_schemes_archives_sha256.txt', 'FILE_SHA256SUMS.txt'): continue
        private_entries.append((path, 'space_schemes_draft_v1/' + relative.as_posix()))
    file_checksums = ROOT / 'FILE_SHA256SUMS.txt'
    file_checksums.write_text('\n'.join(f'{sha256(path)}  {path.relative_to(ROOT).as_posix()}'
                                       for path, _ in sorted(private_entries, key=lambda x: x[1])) + '\n',
                              encoding='utf-8')
    private_entries.append((file_checksums, 'space_schemes_draft_v1/FILE_SHA256SUMS.txt'))
    public_entries = [(ROOT / 'statement.md', 'statement.md'),
                      (ROOT / 'contestant_README.md', 'README.md')]
    public_entries += [(p, p.relative_to(ROOT).as_posix()) for p in (ROOT / 'samples').rglob('*')
                       if p.is_file() and p.suffix in ('.in', '.ans')]
    write_archive(organizer, private_entries)
    write_archive(contestant, public_entries)
    with zipfile.ZipFile(contestant) as archive:
        for name in archive.namelist():
            if name not in ('README.md', 'statement.md') and not name.startswith('samples/'):
                raise AssertionError('unexpected public file: ' + name)
    checksums = output_dir / 'space_schemes_archives_sha256.txt'
    checksums.write_text('\n'.join(f'{sha256(p)}  {p.name}' for p in (organizer, contestant)) + '\n', encoding='ascii')
    print(f'Organizer: {organizer} ({organizer.stat().st_size} bytes)')
    print(f'Contestant: {contestant} ({contestant.stat().st_size} bytes)')
    print(f'Checksums: {checksums}')


if __name__ == '__main__':
    main()
