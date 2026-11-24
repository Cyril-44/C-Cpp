#!/usr/bin/env python3
"""Build statement.tex and statement.pdf from statement.md and main.tex.

The generated problem text replaces only the marked section in main.tex, so
its contest overview page and document styling remain intact.

Run from any directory with:
    python3 /path/to/build_statement.py

Requires Pandoc, latexmk, and XeLaTeX.
"""

from __future__ import annotations

import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


SCRIPT_DIR = Path(__file__).resolve().parent
SOURCE = SCRIPT_DIR / "statement.md"
TEMPLATE = SCRIPT_DIR / "main.tex"
TEX_OUTPUT = SCRIPT_DIR / "statement.tex"
PDF_OUTPUT = SCRIPT_DIR / "statement.pdf"
START_MARKER = "% BEGIN GENERATED PROBLEMS"
END_MARKER = "% END GENERATED PROBLEMS"


def _run(
    command: list[str], *, cwd: Path | None = None
) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        command,
        text=True,
        check=True,
        capture_output=True,
        cwd=cwd,
    )


def _merge_caret_cells(table_body: str, column_count: int) -> str:
    trailing_newline = table_body.endswith("\n")
    lines = table_body.splitlines()
    row_records: list[tuple[int, list[str]]] = []
    for line_index, line in enumerate(lines):
        stripped = line.rstrip()
        if not stripped.endswith(r"\\"):
            continue
        row_text = stripped[: -len(r"\\")].rstrip()
        cells = re.split(r"(?<!\\)\s*&\s*", row_text)
        row_records.append((line_index, cells))

    if not any(cell.strip() == r"\^{}" for _, cells in row_records for cell in cells):
        return table_body

    for row_index, (_, cells) in enumerate(row_records):
        for column_index, cell in enumerate(cells):
            if cell.strip() != r"\^{}":
                continue
            anchor_index = row_index - 1
            while anchor_index >= 0:
                if column_index >= len(row_records[anchor_index][1]):
                    raise RuntimeError("表格中的 ^ 与上方单元格列数不一致。")
                if row_records[anchor_index][1][column_index].strip() != r"\^{}":
                    break
                anchor_index -= 1
            if anchor_index < 0:
                raise RuntimeError("表格中的 ^ 没有可合并的上方单元格。")
            if not row_records[anchor_index][1][column_index].strip():
                raise RuntimeError("表格中的 ^ 上方单元格为空，无法合并。")

    for anchor_index, (_, cells) in enumerate(row_records):
        for column_index, cell in enumerate(cells):
            if cell.strip() == r"\^{}":
                continue
            span = 1
            while (
                anchor_index + span < len(row_records)
                and column_index < len(row_records[anchor_index + span][1])
                and row_records[anchor_index + span][1][column_index].strip() == r"\^{}"
            ):
                span += 1
            if span > 1:
                left_rule = "|" if column_index > 0 else ""
                right_rule = "|" if column_index + 1 < column_count else ""
                centered_cell = (
                    rf"\multirow{{{span}}}{{*}}{{{cell}}}"
                )
                cells[column_index] = (
                    rf"\multicolumn{{1}}{{{left_rule}c{right_rule}}}"
                    + "{"
                    + centered_cell
                    + "}"
                )

    for _, cells in row_records:
        for column_index, cell in enumerate(cells):
            if cell.strip() == r"\^{}":
                cells[column_index] = ""

    for line_index, cells in row_records:
        lines[line_index] = " & ".join(cells) + r" \\"
    result = "\n".join(lines)
    return result + ("\n" if trailing_newline else "")


def _add_row_rules(table_body: str, column_count: int) -> str:
    lines = table_body.splitlines()
    data_started = False
    data_rows: list[tuple[int, list[str]]] = []
    for line_index, line in enumerate(lines):
        if line.strip() == r"\endlastfoot":
            data_started = True
            continue
        stripped = line.rstrip()
        if data_started and stripped.endswith(r"\\"):
            row_text = stripped[: -len(r"\\")].rstrip()
            data_rows.append((line_index, re.split(r"(?<!\\)\s*&\s*", row_text)))

    active_until: dict[int, int] = {}
    for row_index, (line_index, cells) in enumerate(data_rows[:-1]):
        for column_index, cell in enumerate(cells):
            multirow = re.search(r"\\multirow\{(\d+)\}", cell)
            if multirow:
                active_until[column_index] = row_index + int(multirow.group(1)) - 1

        skipped_columns = {
            column_index
            for column_index, last_row in active_until.items()
            if last_row > row_index
        }
        if not skipped_columns:
            rule = r"\hline"
        else:
            visible = [
                column_index + 1
                for column_index in range(column_count)
                if column_index not in skipped_columns
            ]
            segments: list[tuple[int, int]] = []
            for column in visible:
                if segments and segments[-1][1] + 1 == column:
                    segments[-1] = (segments[-1][0], column)
                else:
                    segments.append((column, column))
            rule = "".join(rf"\cline{{{start}-{end}}}" for start, end in segments)

        lines[line_index] = lines[line_index].rstrip() + " " + rule

    return "\n".join(lines) + ("\n" if table_body.endswith("\n") else "")


def add_table_rules(latex: str) -> str:
    table_pattern = re.compile(
        r"(?P<begin>\\begin\{longtable\}\[\]\{)"
        r"(?P<columns>@\{\}[lcr]+@\{\})\}"
        r"(?P<body>[\s\S]*?)(?P<end>\\end\{longtable\})"
    )

    def convert_table(match: re.Match[str]) -> str:
        spec_match = re.fullmatch(r"@\{\}([lcr]+)@\{\}", match.group("columns"))
        if not spec_match:
            raise RuntimeError(
                f"不支持的 Markdown 表格列格式：{match.group('columns')}"
            )
        columns = spec_match.group(1)
        bordered_spec = "|".join(columns)
        body = _merge_caret_cells(match.group("body"), len(columns))
        body = _add_row_rules(body, len(columns))
        return (
            match.group("begin")
            + bordered_spec
            + "}"
            + body
            + match.group("end")
        )

    return table_pattern.sub(convert_table, latex)


def render_markdown() -> str:
    result = _run(
        [
            "pandoc",
            "--from=markdown+pipe_tables",
            "--to=latex",
            "--wrap=none",
            "--syntax-highlighting=none",
            str(SOURCE),
        ],
        cwd=SCRIPT_DIR,
    )
    latex = result.stdout

    problem_count = 0

    def replace_problem_title(match: re.Match[str]) -> str:
        nonlocal problem_count
        problem_count += 1
        page_break = "" if problem_count == 1 else "\\newpage\n"
        return page_break + rf"\problemtitle{{{match.group(1)}}}{{{match.group(2)}}}"

    latex, _ = re.subn(
        r"(?m)^\\section\{([^\n]*)\}\\label\{([^{}]*)\}$",
        replace_problem_title,
        latex,
    )
    latex, _ = re.subn(
        r"(?m)^\\subsection\{([^\n]*)\}\\label\{([^{}]*)\}$",
        lambda match: rf"\problemheading{{{match.group(1)}}}{{{match.group(2)}}}",
        latex,
    )
    if problem_count == 0:
        raise RuntimeError("statement.md 中没有一级标题，无法识别题目。")
    if re.search(r"(?m)^\\(?:section|subsection)\{", latex):
        raise RuntimeError("Markdown 标题转换不完整，请检查 Pandoc 输出。")
    return add_table_rules(latex).strip() + "\n"


def replace_generated_block(template: str, body: str) -> str:
    if template.count(START_MARKER) != 1 or template.count(END_MARKER) != 1:
        raise RuntimeError("main.tex 中的生成区标记缺失或重复。")
    start = template.index(START_MARKER) + len(START_MARKER)
    end = template.index(END_MARKER)
    if end < start:
        raise RuntimeError("main.tex 中的生成区标记顺序错误。")

    if r"\begin{longtable}" in body and not re.search(
        r"\\usepackage(?:\[[^\]]*\])?\{[^}]*\blongtable\b[^}]*\}", template
    ):
        document_start = template.find(r"\begin{document}")
        if document_start == -1:
            raise RuntimeError("main.tex 中找不到 \\begin{document}。")
        template = (
            template[:document_start]
            + "\\usepackage{longtable}\n"
            + template[document_start:]
        )
        start = template.index(START_MARKER) + len(START_MARKER)
        end = template.index(END_MARKER)

    return template[:start] + "\n" + body + template[end:]


def build() -> None:
    missing = [
        name for name in ("pandoc", "latexmk", "xelatex") if not shutil.which(name)
    ]
    if missing:
        raise RuntimeError("找不到构建工具：" + ", ".join(missing))
    for required in (SOURCE, TEMPLATE):
        if not required.is_file():
            raise RuntimeError(f"找不到构建输入文件：{required}")

    body = render_markdown()
    tex = replace_generated_block(TEMPLATE.read_text(encoding="utf-8"), body)
    temporary_tex = TEX_OUTPUT.with_suffix(".tex.tmp")
    temporary_tex.write_text(tex, encoding="utf-8")
    temporary_tex.replace(TEX_OUTPUT)

    # Compile in a temporary output directory so auxiliary files do not clutter
    # the source directory. Keep the source directory as cwd for relative files.
    with tempfile.TemporaryDirectory(prefix=".statement-build-", dir=SCRIPT_DIR) as build_dir:
        _run(
            [
                "latexmk",
                "-xelatex",
                "-interaction=nonstopmode",
                "-halt-on-error",
                "-file-line-error",
                f"-outdir={build_dir}",
                "-jobname=statement",
                TEX_OUTPUT.name,
            ],
            cwd=SCRIPT_DIR,
        )
        built_pdf = Path(build_dir) / "statement.pdf"
        if not built_pdf.is_file():
            raise RuntimeError("latexmk 未生成 statement.pdf")
        built_pdf.replace(PDF_OUTPUT)


def main() -> int:
    try:
        build()
    except (OSError, RuntimeError, subprocess.CalledProcessError) as error:
        print(f"构建失败：{error}", file=sys.stderr)
        if isinstance(error, subprocess.CalledProcessError):
            if error.stdout:
                print(error.stdout, file=sys.stderr)
            if error.stderr:
                print(error.stderr, file=sys.stderr)
        return 1

    print(f"已保留 {TEMPLATE} 的首页并生成 {TEX_OUTPUT}")
    print(f"已生成 {PDF_OUTPUT}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
