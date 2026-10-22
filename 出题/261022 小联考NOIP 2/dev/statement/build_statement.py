#!/usr/bin/env python3
"""Assemble dev/*/statement.md into this directory's main.tex.

Run from any directory with:
    python3 dev/statement/build_statement.py

Pandoc is used for Markdown-to-LaTeX conversion. The generated problem block is
bounded by markers in main.tex, so the preamble and cover page stay untouched.
Question text inside that block is sourced from the Markdown files on each run.
"""

from __future__ import annotations

import re
import shutil
import subprocess
import sys
from pathlib import Path


SCRIPT_DIR = Path(__file__).resolve().parent
DEV_DIR = SCRIPT_DIR.parent
MAIN_TEX = SCRIPT_DIR / "main.tex"
START_MARKER = "% BEGIN GENERATED PROBLEMS"
END_MARKER = "% END GENERATED PROBLEMS"
TABLE_WIDTH = r"0.85\textwidth"

# Keep the contest's A-D order even if the filesystem returns entries in a
# different order. Any additional statement.md directories follow alphabetically.
PREFERRED_ORDER = ("ring", "calibration", "lrud", "storage")
LETTER_BY_DIR = {name: chr(ord("A") + i) for i, name in enumerate(PREFERRED_ORDER)}
CHINESE_NUMBERS = {
    "一": 1,
    "二": 2,
    "三": 3,
    "四": 4,
    "五": 5,
    "六": 6,
    "七": 7,
    "八": 8,
    "九": 9,
    "十": 10,
}


def discover_statements() -> list[tuple[str, str, Path]]:
    sources = {
        path.parent.name: path
        for path in DEV_DIR.glob("*/statement.md")
        if path.parent != SCRIPT_DIR
    }
    missing = [name for name in PREFERRED_ORDER if name not in sources]
    if missing:
        raise RuntimeError(
            "缺少题目 Markdown：" + ", ".join(f"dev/{name}/statement.md" for name in missing)
        )

    extra_names = sorted(set(sources) - set(PREFERRED_ORDER))
    ordered_names = [*PREFERRED_ORDER, *extra_names]
    result = []
    for index, name in enumerate(ordered_names):
        letter = LETTER_BY_DIR.get(name, chr(ord("A") + index))
        result.append((letter, name, sources[name]))
    return result


def _sample_number(token: str | None, next_number: int) -> int:
    if not token:
        return next_number
    token = token.lstrip("#").strip()
    if token.isdigit():
        return int(token)
    return CHINESE_NUMBERS.get(token, next_number)


def _parse_sample_heading(title: str, next_number: int) -> tuple[int, str | None] | None:
    title = re.sub(r"\s+", " ", title.strip())
    if title in {"样例解释", "样例说明"}:
        return None

    match = re.fullmatch(
        r"(?:样例输入输出|输入输出样例|样例)\s*(.*?)\s*", title
    )
    if not match:
        return None

    rest = match.group(1).strip()
    suffix_match = re.search(r"(输入|输出|解释)\s*$", rest)
    suffix = suffix_match.group(1) if suffix_match else None
    if suffix_match:
        rest = rest[: suffix_match.start()].strip()
    number_match = re.search(r"#?\s*(\d+|[一二三四五六七八九十])", rest)
    number = _sample_number(number_match.group(1) if number_match else None, next_number)
    return number, suffix


def _normalize_data_heading(title: str) -> str:
    title = re.sub(r"\s+", " ", title.strip())
    if title in {"说明/提示", "数据范围", "数据规模", "数据规模与约定"}:
        return "数据规模与约定"
    if title.startswith(("数据范围", "数据规模")):
        return "数据规模与约定"
    return title


def _count_headings(markdown: str) -> int:
    count = 0
    fence_char: str | None = None
    for line in markdown.splitlines():
        fence_match = re.match(r"^\s*(```+|~~~+)", line)
        if fence_match:
            token = fence_match.group(1)
            if fence_char is None:
                fence_char = token[0]
            elif token[0] == fence_char:
                fence_char = None
            continue
        if fence_char is None and line.startswith("## "):
            count += 1
    return count


def normalize_markdown(source: str) -> tuple[str, str, str | None]:
    """Remove the document title and normalize headings/sample subsections.

    Returns (normalized Markdown, title, normalized time/memory line).
    """
    lines = source.splitlines()
    title = ""
    for index, line in enumerate(lines):
        if re.match(r"^#\s+", line):
            title = re.sub(r"^#\s+", "", line).strip()
            del lines[index]
            break
    if not title:
        title = "未命名题目"

    metadata_tex = None
    for index, line in enumerate(lines):
        if not line.strip():
            continue
        match = re.match(
            r"^\s*时间限制：\s*(.*?)\s*(?:[。；;]\s*)?空间限制：\s*(.*?)\s*$",
            line,
        )
        if match:
            time_limit = match.group(1).strip().rstrip("。；;")
            memory_and_note = match.group(2).strip()
            parts = re.split(r"[。；;]\s*", memory_and_note, maxsplit=1)
            memory_limit = parts[0].strip()
            note = parts[1].strip() if len(parts) > 1 else ""
            metadata_tex = (
                rf"\noindent 时间限制：{time_limit}\qquad "
                rf"空间限制：{memory_limit}\par"
            )
            lines[index] = note
        break

    normalized: list[str] = []
    current_sample: int | None = None
    current_sample_section: str | None = None
    next_sample = 1
    output_code_ended = False
    fence_char: str | None = None

    for line in lines:
        fence_match = re.match(r"^\s*(```+|~~~+)", line)
        if fence_match:
            token = fence_match.group(1)
            if fence_char is None:
                fence_char = token[0]
                if current_sample_section == "输出":
                    output_code_ended = False
            elif token[0] == fence_char:
                fence_char = None
                if current_sample_section == "输出":
                    output_code_ended = True
            normalized.append(line)
            continue
        if fence_char is not None:
            normalized.append(line)
            continue

        heading = re.match(r"^(#{1,6})\s+(.+?)\s*#*\s*$", line)
        if heading:
            level, heading_title = len(heading.group(1)), heading.group(2).strip()
            sample = _parse_sample_heading(heading_title, next_sample)
            if sample:
                current_sample, suffix = sample
                next_sample = max(next_sample, current_sample + 1)
                current_sample_section = suffix
                output_code_ended = False
                if suffix:
                    normalized.append(f"## 样例 {current_sample} {suffix}")
                # A sample title is a container in Markdown; the generated TeX
                # headings label the input, output, and optional explanation.
                continue

            if current_sample is not None and heading_title in {
                "样例解释", "样例说明", "解释", "解释（可选）", "解释(可选)"
            }:
                current_sample_section = "解释"
                output_code_ended = False
                normalized.append(f"## 样例 {current_sample} 解释")
                continue

            if current_sample is not None and heading_title in {"输入", "输出"}:
                current_sample_section = heading_title
                output_code_ended = False
                normalized.append(f"## 样例 {current_sample} {heading_title}")
                continue

            current_sample = None
            current_sample_section = None
            output_code_ended = False
            heading_title = _normalize_data_heading(heading_title)
            normalized.append(f"## {heading_title}")
            continue

        if current_sample is not None:
            plain_sample_label = re.fullmatch(r"\s*(输入|输出|解释)\s*[：:]?\s*", line)
            if plain_sample_label:
                current_sample_section = plain_sample_label.group(1)
                output_code_ended = False
                normalized.append(f"## 样例 {current_sample} {current_sample_section}")
                continue

            if output_code_ended and line.strip():
                normalized.append(f"## 样例 {current_sample} 解释")
                current_sample_section = "解释"
                output_code_ended = False

        normalized.append(line)

    return "\n".join(normalized).strip() + "\n", title, metadata_tex


def _heading_slug(title: str, fallback_number: int) -> str:
    known = {
        "题目背景": "background",
        "题目描述": "description",
        "输入格式": "input",
        "输出格式": "output",
        "数据规模与约定": "constraints",
    }
    if title in known:
        return known[title]
    match = re.fullmatch(r"样例 (\d+) (输入|输出|解释)", title)
    if match:
        suffix = {"输入": "input", "输出": "output", "解释": "explanation"}[match.group(2)]
        return f"sample_{match.group(1)}_{suffix}"
    return f"section_{fallback_number}"


def _convert_tables(latex: str) -> tuple[str, int]:
    # Pandoc 3 wraps longtable in a local caption-type group. The surrounding
    # contest template does not use captions, so remove that wrapper first.
    latex = re.sub(
        r"\{\s*\\def\\LTcaptype\{none\}\s*%[^\n]*\n(?=\\begin\{longtable\})",
        "",
        latex,
    )
    latex = re.sub(r"(\\end\{longtable\})\s*\}", r"\1", latex)

    table_re = re.compile(
        r"\\begin\{longtable\}\[\]\{@\{\}([lcr]+)@\{\}\}"
        r"([\s\S]*?)\\end\{longtable\}"
    )

    def replace_table(match: re.Match[str]) -> str:
        alignments, body = match.group(1), match.group(2)
        columns = len(alignments)
        if columns == 1:
            layout = "|X|"
        else:
            layout = "|" + "|".join(["c"] * (columns - 1) + ["X"]) + "|"

        body = body.replace(r"\noalign{}", "")
        body = re.sub(r"\\(?:toprule|midrule|bottomrule)", "", body)
        body = body.replace(r"\endhead", "")
        body = body.replace(r"\endlastfoot", "")
        body = body.replace(r"\tabularnewline", r"\\")
        # Pandoc's \toprule is removed with the other booktabs rules; restore
        # the table's top border using the template's \hline style.
        body = r"\hline" + "\n" + body.lstrip()

        def add_row_rule(row: re.Match[str]) -> str:
            return row.group(1).rstrip() + r"\\\hline"

        body = re.sub(r"(?m)^([^\n]*?)(\\\\)\s*$", add_row_rule, body)
        return (
            rf"\begin{{centeredtable}}\begin{{tabularx}}{{{TABLE_WIDTH}}}{{{layout}}}"
            + body
            + r"\end{tabularx}\end{centeredtable}"
        )

    converted, count = table_re.subn(replace_table, latex)
    if r"\begin{longtable}" in converted:
        raise RuntimeError("发现未适配的 Markdown 表格；请检查 Pandoc 输出格式。")
    return converted, count


def render_problem(letter: str, directory: str, source_path: Path) -> tuple[str, int, int]:
    markdown, title, metadata_tex = normalize_markdown(source_path.read_text(encoding="utf-8"))
    if not re.search(rf"\({re.escape(directory)}\)\s*$", title):
        title += f"({directory})"

    pandoc = shutil.which("pandoc")
    if not pandoc:
        raise RuntimeError("找不到 pandoc。请先安装 Pandoc，再运行此脚本。")
    version = subprocess.run(
        [pandoc, "--version"], text=True, check=True, capture_output=True
    ).stdout.splitlines()[0]
    major_match = re.search(r"\bpandoc\s+(\d+)", version)
    highlighting_option = (
        "--syntax-highlighting=none"
        if major_match and int(major_match.group(1)) >= 3
        else "--no-highlight"
    )
    result = subprocess.run(
        [
            pandoc,
            highlighting_option,
            "-f",
            "markdown+pipe_tables",
            "-t",
            "latex",
            "--wrap=none",
        ],
        input=markdown,
        text=True,
        check=True,
        capture_output=True,
    )
    latex, table_count = _convert_tables(result.stdout)

    label_counts: dict[str, int] = {}
    heading_index = 0
    # Pandoc emits a hypertarget wrapper when converting a file, but can emit
    # a bare subsection when Markdown is supplied on stdin. Accept both forms.
    heading_re = re.compile(
        r"\\hypertarget\{[^}]*\}\{%\s*\\subsection\{(?P<wrapped>[^{}\n]*)\}"
        r"\\label\{[^}]*\}\}"
        r"|\\subsection\{(?P<plain>[^{}\n]*)\}\\label\{[^}]*\}"
    )

    def replace_heading(match: re.Match[str]) -> str:
        nonlocal heading_index
        heading_index += 1
        heading_title = match.group("wrapped") or match.group("plain")
        base = _heading_slug(heading_title, heading_index)
        label_counts[base] = label_counts.get(base, 0) + 1
        suffix = f"_{label_counts[base]}" if label_counts[base] > 1 else ""
        return f"\\problemheading{{{heading_title}}}{{prob{letter}_{base}{suffix}}}"

    latex, heading_count = heading_re.subn(replace_heading, latex)
    expected_headings = _count_headings(markdown)
    if heading_count != expected_headings:
        raise RuntimeError(
            f"{source_path}: 识别到 {heading_count}/{expected_headings} 个小标题，"
            "请检查 Markdown 标题层级。"
        )
    if re.search(r"\\hypertarget\{|\\subsection\{|\\subsubsection\{|\\section\{", latex):
        raise RuntimeError(f"{source_path}: 仍有未转换的标题。")

    sample_block_re = re.compile(
        r"\\problemheading\{(样例 \d+ (?:输入|输出))\}\{([^{}]*)\}\s*"
        r"(\\begin\{verbatim\}[\s\S]*?\\end\{verbatim\})"
    )

    def wrap_sample_block(match: re.Match[str]) -> str:
        heading_title, label, code = match.group(1), match.group(2), match.group(3)
        return (
            "\\par\\medskip\\noindent\\begin{minipage}{\\linewidth}\n"
            f"\\problemheading{{{heading_title}}}{{{label}}}\n\n"
            f"{code}\n"
            "\\end{minipage}\\par\\medskip"
        )

    latex = sample_block_re.sub(wrap_sample_block, latex)

    prefix = [f"\\problemtitle{{{title}}}{{prob{letter}_title}}"]
    if metadata_tex:
        prefix.append(metadata_tex)
    body = "\n\n".join(prefix + [latex.strip()]).strip()
    return body, heading_count, table_count


def replace_generated_block(template: str, body: str) -> str:
    if START_MARKER in template or END_MARKER in template:
        if template.count(START_MARKER) != 1 or template.count(END_MARKER) != 1:
            raise RuntimeError("main.tex 中的生成区标记缺失或重复。")
        start = template.index(START_MARKER) + len(START_MARKER)
        end = template.index(END_MARKER)
        if end < start:
            raise RuntimeError("main.tex 中的生成区标记顺序错误。")
        return template[:start] + "\n" + body + "\n" + template[end:]

    start_match = re.search(r"(?m)^[ \t]*\\problemtitle\{", template)
    end_match = re.search(r"(?m)^[ \t]*\\end\{document\}", template)
    if not start_match or not end_match or start_match.start() >= end_match.start():
        raise RuntimeError("无法在 main.tex 中定位题目正文起点或文档结尾。")
    return (
        template[: start_match.start()]
        + START_MARKER
        + "\n"
        + body
        + "\n"
        + END_MARKER
        + "\n"
        + template[end_match.start() :]
    )


def main() -> int:
    try:
        problems = discover_statements()
        rendered = []
        details = []
        for letter, directory, source_path in problems:
            body, heading_count, table_count = render_problem(letter, directory, source_path)
            rendered.append(body)
            details.append((letter, directory, heading_count, table_count))

        combined_body = "\n\\newpage\n".join(rendered)
        template = MAIN_TEX.read_text(encoding="utf-8")
        output = replace_generated_block(template, combined_body)
        temporary = MAIN_TEX.with_suffix(".tex.tmp")
        temporary.write_text(output, encoding="utf-8")
        temporary.replace(MAIN_TEX)
    except (OSError, RuntimeError, subprocess.CalledProcessError) as error:
        print(f"构建失败：{error}", file=sys.stderr)
        if isinstance(error, subprocess.CalledProcessError) and error.stderr:
            print(error.stderr, file=sys.stderr)
        return 1

    print(f"已更新 {MAIN_TEX}")
    for letter, directory, heading_count, table_count in details:
        print(
            f"  {letter}: dev/{directory}/statement.md "
            f"({heading_count} 个小标题，{table_count} 张表格)"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
