
#!/usr/bin/env python3
import os
import re
import sys
import shutil
import tempfile
import subprocess
from pathlib import Path


def die(message, code=1):
    print(f"pack_qjs: {message}", file=sys.stderr)
    raise SystemExit(code)


def get_qjsc():
    """从 QJSC_PATH 获取 qjsc 可执行文件路径。"""
    value = os.environ.get("QJSC_PATH")

    if not value:
        die("请设置环境变量 QJSC_PATH")

    candidate = Path(value).expanduser()

    if candidate.is_file():
        return str(candidate.resolve())

    found = shutil.which(value)
    if found:
        return found

    die(f"找不到 qjsc 可执行文件：{value}")


def get_qjam_dir():
    """QJAM_PATH 可以指向目录，也可以直接指向 quickjs-amalgam.c。"""
    value = os.environ.get("QJAM_PATH")

    if not value:
        die("请设置环境变量 QJAM_PATH")

    path = Path(value).expanduser()

    if path.is_file() and path.name == "quickjs-amalgam.c":
        directory = path.parent
    else:
        directory = path

    directory = directory.resolve()

    required = [
        "quickjs-amalgam.c",
        "quickjs-libc.h",
        "quickjs.h",
    ]

    for name in required:
        if not (directory / name).is_file():
            die(f"QJAM_PATH 中缺少文件：{directory / name}")

    return directory


def compile_js(qjsc, source, generated_c, temp_dir):
    """调用 qjsc 生成包含 main() 和字节码的 C 文件。"""
    command = [
        qjsc,
        "-e",           # 生成 main() 和字节码
        "-s", "-s",     # 移除源码及调试信息
        "-o", str(generated_c),
        "--",
        source.name,
    ]

    print("[1/3] 正在编译 JavaScript 字节码……", file=sys.stderr)

    try:
        result = subprocess.run(
            command,
            cwd=str(source.parent),
            check=False,
        )
    except OSError as exc:
        die(f"无法执行 qjsc：{exc}")

    if result.returncode != 0:
        die(f"qjsc 编译失败，退出码：{result.returncode}")

    if not generated_c.is_file():
        die("qjsc 没有生成预期的 C 文件")

    return generated_c.read_text(encoding="utf-8")


def merge_c(amalgam_path, generated):
    """合并 QuickJS-NG 引擎和 qjsc 生成的程序。"""
    print("[2/3] 正在合并 QuickJS-NG 引擎……", file=sys.stderr)

    amalgam = amalgam_path.read_text(encoding="utf-8")

    # qjsc -e 生成的 C 文件会引用 quickjs-libc.h。
    # 该头文件的声明已经包含在 quickjs-amalgam.c 中，
    # 因此需要移除这个外部头文件引用。
    pattern = re.compile(
        r'(?m)^[ \t]*#[ \t]*include[ \t]*'
        r'"quickjs-libc\.h"[ \t]*(?:\r?\n|$)'
    )

    generated, count = pattern.subn("", generated)

    if count == 0:
        die(
            'qjsc 输出中未找到 #include "quickjs-libc.h"；'
            "请确认 QJSC_PATH 与 QJAM_PATH 来自兼容的 "
            "QuickJS-NG 版本"
        )

    # QuickJS-NG 官方要求通过 QJS_BUILD_LIBC 启用
    # std、os、bjson 等标准库模块。
    #
    # quickjs-amalgam.c 已经内联了 quickjs.h、
    # quickjs-libc.h 以及对应实现，不要重复拼接头文件。
    combined = (
        "#define QJS_BUILD_LIBC 1\n"
        "/* Single-file QuickJS-NG application. */\n\n"
        + amalgam.rstrip()
        + "\n\n"
        + generated.lstrip()
    )

    return combined


def main():
    if len(sys.argv) != 3:
        print(
            f"用法：{sys.argv[0]} <input.js> <output.c>",
            file=sys.stderr,
        )
        return 2

    source = Path(sys.argv[1]).expanduser().resolve()
    output = Path(sys.argv[2]).expanduser().resolve()

    if not source.is_file():
        die(f"输入 JS 文件不存在：{source}", 2)

    qjsc = get_qjsc()
    qjam_dir = get_qjam_dir()
    amalgam_path = qjam_dir / "quickjs-amalgam.c"

    # 临时目录仅存放 qjsc 生成的中间 C 文件。
    with tempfile.TemporaryDirectory(prefix="pack-qjs-") as temp:
        generated_c = Path(temp) / "generated.c"

        generated = compile_js(
            qjsc,
            source,
            generated_c,
            temp,
        )

        combined = merge_c(amalgam_path, generated)

    # 只在前面的步骤全部成功后写入最终文件。
    output.parent.mkdir(parents=True, exist_ok=True)

    try:
        temp_output = output.with_name(output.name + ".tmp")
        temp_output.write_text(combined, encoding="utf-8")

        os.replace(temp_output, output)
    except OSError as exc:
        die(f"写入输出文件失败：{exc}")

    print("[3/3] 打包完成！", file=sys.stderr)
    print(f"输出文件：{output}", file=sys.stderr)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

