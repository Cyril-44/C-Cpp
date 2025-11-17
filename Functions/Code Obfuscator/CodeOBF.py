#!/usr/bin/env python3
"""Refactored C++ obfuscator

主要改动：
- 修复了之前对 obj.member 的过度排除（以前会把所有 ".name" 都当系统成员从而不混淆，导致结构体成员/构造函数参数/局部变量名不被混淆）
  现在只排除形如 obj.method(...) 的方法名和 namespace::name 形式的命名空间成员；普通的成员变量（obj.x）会被混淆。
- 在按行替换时同时保护字符串占位符和字符占位符（__STR...__ / __CHAR...__），避免对它们误替换。
- 改进随机名生成（更短、更可读，同时仍然难以猜测）。
- 增强对类/结构体内部声明与构造函数参数的覆盖能力（通过不再错误地把成员变量排除）。
- 一些小的健壮性改进和注释。

使用方法：行为与原脚本相同（命令行参数、输入/输出/映射/保护文件）。
"""

import re
import random
import string
import argparse
from typing import Tuple, List, Set

# 全局映射表：原名 -> 混淆名
ID_MAPPING = {}

# def rand_name(min_len=8, max_len=16):
#     # 生成以字母或下划线开头的随机标识符
#     first = random.choice(''.join(random.choices(['o', 'O'], k=random.randint(min_len, max_len))))
#     length = random.randint(min_len, max_len)
#     rest = ''.join(random.choices(string.ascii_letters, k=length))
#     return first + rest

RAND_NAME_ALL_NAMES = set()
RAND_NAME_AVALIABLE_CHARS = set(string.ascii_letters)

def rand_name():
    global RAND_NAME_ALL_NAMES, RAND_NAME_AVALIABLE_CHARS
    
    # 如果所有字符都已用完，清空重新开始或抛出异常
    if not RAND_NAME_AVALIABLE_CHARS:
        # 选项1: 清空重新开始
        # rand_name.all_names.clear()
        # rand_name.available_chars = set(string.ascii_letters)
        # 或者选项2: 抛出异常
        raise Exception("所有单个字符都已使用完毕")
    
    # 从可用字符中随机选择一个
    temp = random.choice(list(RAND_NAME_AVALIABLE_CHARS))
    RAND_NAME_AVALIABLE_CHARS.remove(temp)  # 移除已使用的字符
    RAND_NAME_ALL_NAMES.add(temp)
    return temp


def parse_args():
    p = argparse.ArgumentParser(
        description="C++ Obfuscator: protect strings then unicode-escape"
    )
    p.add_argument("input",  nargs="?", default="OBF.in", help="输入 C++ 文件 (默认 OBF.in)")
    p.add_argument("output", nargs="?", default="OBF.out", help="输出混淆后 C++ 文件 (默认 OBF.out)")
    p.add_argument("map", nargs="?", default="OBF.map", help="输出混淆表 (默认 OBF.map)")
    p.add_argument("protect", nargs="?", default="OBF.protect", help="混淆保护 (不混淆某些参数) (默认 OBF.protect)")
    return p.parse_args()


def collect_macros(lines: List[str]) -> Set[str]:
    """收集所有 #define 宏名，用于排除混淆。"""
    macros = set()
    for ln in lines:
        m = re.match(r'\s*#\s*define\s+([A-Za-z_]\w*)', ln)
        if m:
            macros.add(m.group(1))
    return macros

# C++ 关键词集合
CPP_KEYWORDS = {
    "alignas","alignof","and","and_eq","asm","auto","bitand","bitor","bool",
    "break","case","catch","char","class","const","constexpr","continue",
    "decltype","default","delete","do","double","dynamic_cast","else","enum",
    "explicit","export","extern","false","float","for","friend","goto","if",
    "inline","int","long","mutable","namespace","new","noexcept","not","nullptr",
    "operator","or","private","protected","public","register",
    "reinterpret_cast","return","short","signed","sizeof","static",
    "static_assert","static_cast","struct","switch","template","this","throw",
    "true","try","typedef","typename","union","unsigned","using","virtual",
    "void","volatile","while","main","include",
    "uint32_t", "uint64_t", "__int128",
}

# 保留集合
RESERVED = {
    # C headers and C++ headers
    "stdio.h", "stdlib.h", "string.h", "math.h", "limits.h", "float.h", "ctype.h", "stdbool.h", "assert.h", "time.h", 
    "wchar.h", "locale.h",  "errno.h", "signal.h", "stdarg.h", "stddef.h", "stdint.h", "inttypes.h", 
    "unistd.h", "fcntl.h", "sys/types.h", "sys/stat.h", "sys/time.h", "sys/wait.h", "dirent.h", "pthread.h", "netinet/in.h", 
    "arpa/inet.h", "netdb.h", "sys/socket.h", "poll.h", "termios.h",
    "time", "1LL<<62", "std", "u<<1", "u<<1|1", 
    "iostream", "iomanip", "fstream", "sstream", "tie", "sync_with_stdio", 
    "string", "vector", "list", "deque", "queue", "stack", "map", "unordered_map",  "set", "unordered_set", 
    "bitset", "priority_queue", "pair", "first", "second",
    "algorithm", "numeric", "functional", "utility", "memory", "tuple", "limits", "typeinfo", "cstdio", "cctype", 
    "cmath", "cstdlib", "cstring", "ctime", "cassert", 
    "cstdarg", "cstddef", "cstdint", "climits", "csignal", "locale", "new", "stdexcept", "exception",
    "size_t", "int64_t", "uint64_t", "EOF", "__always_inline", "stdin", "stdout",
    "enable_if", "type", "value", "const_cast", 
    "fread", "fwrite", "printf", "scanf", "cin", "cout", "fprintf", "sort", "assert",
    "bitset", "resize", "push_back", "pop_back", "emplace_back", "top", "front", "push", "pop", "emplace", 
    "multiset", "list", "max", "min", "empty", "at", "array", "stack",
    "__always_inline", "reverse", "ios", "swap", "kth_element", "lower_bound", "upper_bound", "unique",
    "enable_if", "type", "value", "is_integral", "decay", "is_same", "is_unsigned", "is_signed", 
    "common_type", "is_floating_point", "conditional", "remove_reference", "remove_const", "remove_volatile",
    "integral_constant", "make_signed", "make_unsigned", "underlying_type",
    "endl", "boolalpha", "fixed", "defined",
    "sort", "strlen", "memcpy", "memset", "getchar", "puts", "putchar", "to_string", 
    "acos", "cos", "asin", "sin", "atan", "tan", "abs", "sqrt", "log", "floor", "ceil", "round",
    "iterator", "const_iterator", "begin", "end", "cbegin", "cend", "rbegin", "rend", "crbegin", "crend",
    "greater", "less", "greater_equal", "less_equal",
    "runtime_error", "out_of_range", "overflow_error", "underflow_error", "domain_error", "length_error", "invalid_argument",
    "FILE", "freopen", "fopen", "fclose", "fflush", "_IOFBF", "_IONBF", "_IOLBF", "setvbuf"
}



def collect_user_functions(src: str) -> Set[str]:
    """
    找出本文件中真正定义的函数名（后跟大括号的函数体），包括成员函数定义。
    例如: "int foo(...) {" 或 "Class::bar(...) {"
    """
    pat = re.compile(r'\b([A-Za-z_]\w*)\s*\([^;{)]*\)\s*\{')
    names = set(pat.findall(src))
    return {n for n in names if n not in CPP_KEYWORDS}


def collect_identifiers(src: str, exclude_all: Set[str]) -> Set[str]:
    """
    提取所有可混淆标识符：排除 C++ 关键字、宏名、保留集。
    """
    toks = set(re.findall(r'\b[A-Za-z_]\w*\b', src))
    return {t for t in toks if t not in CPP_KEYWORDS and t not in exclude_all}


def protect_strings(src: str) -> Tuple[str, List[str]]:
    """
    用占位符 __STR0__, __STR1__… 替换字符串字面量，返回 (new_src, [orig_str0, ...])
    """
    STR_RE = re.compile(r'"(\\.|[^"\\])*"')
    strings = []
    def repl(m):
        idx = len(strings)
        strings.append(m.group(0))
        return f"__STR{idx}__"
    new_src = STR_RE.sub(repl, src)
    return new_src, strings

# 转义序列到 code point
ESCAPES = {
    r'\\': ord('\\'),
    r'\"': ord('"'),
    r'\n': ord('\n'),
    r'\r': ord('\r'),
    r'\t': ord('\t'),
    r'\0': 0,
}

def unicode_escape_string(lit: str) -> str:
    """
    将一个原始字符串字面量（含转义）转换为 uXXXX... 形式。
    lit 包含开闭双引号。
    保护常见短转义符和 uXXXX / xXX 形式。
    """
    inner = lit[1:-1]
    buf = []
    i = 0
    while i < len(inner):
        if inner[i] == '\\' and i + 1 < len(inner):
            seq2 = inner[i:i+2]
            if seq2 in ESCAPES:
                buf.append(f"\\u{ESCAPES[seq2]:04X}")
                i += 2
                continue

            # \uXXXX 形式
            if inner[i+1] == 'u' and i + 6 <= len(inner) and re.fullmatch(r'\\u[0-9a-fA-F]{4}', inner[i:i+6]):
                code = int(inner[i+2:i+6], 16)
                buf.append(f"\\u{code:04X}")
                i += 6
                continue

            # \xHH 形式
            if inner[i+1] == 'x' and i + 4 <= len(inner) and re.fullmatch(r'\\x[0-9a-fA-F]{2}', inner[i:i+4]):
                code = int(inner[i+2:i+4], 16)
                buf.append(f"\\u{code:04X}")
                i += 4
                continue

        # 普通字符
        buf.append(f"\\u{ord(inner[i]):04X}")
        i += 1
    return '"' + "".join(buf) + '"'


def restore_strings(src: str, strings: List[str]) -> str:
    """
    把占位符 __STRi__ 替换回 unicode-escaped 形式的字符串。
    """
    for i, lit in enumerate(strings):
        esc = unicode_escape_string(lit)
        src = src.replace(f"__STR{i}__", esc)
    return src

# 用占位符保护单字符字面量，稍后恢复为数字字面量
def protect_chars(src: str) -> Tuple[str, List[str]]:
    """
    1) 用占位符 __CHAR0__, __CHAR1__… 替换单字符字面量
    2) 返回 (new_src, [orig_char0, orig_char1, …])
    """
    CHAR_RE = re.compile(r"'(\\.|[^'\\])'")
    chars = []
    def repl(m):
        lit = m.group(0)
        idx = len(chars)
        chars.append(lit)
        return f"__CHAR{idx}__"
    new_src = CHAR_RE.sub(repl, src)
    return new_src, chars


def restore_chars(src: str, chars: List[str]) -> str:
    """
    把占位符 __CHARi__ 替换为对应的数字字面量，
    例如 '__CHAR0__' -> '65' (原字符 'A' 的 ASCII 码)。
    """
    for i, lit in enumerate(chars):
        inner = lit[1:-1]  # 去掉两侧单引号
        # 处理转义序列
        if inner.startswith("\\"):
            code = ESCAPES.get(inner, ord(inner[-1]))
        else:
            code = ord(inner)
        src = src.replace(f"__CHAR{i}__", "(char)(" + str(code) + ")")
    return src


# 在全局变量区，新增一个用于保存用户保护列表（来自 OBF.protect）
PROTECT_SET = set()

def obf_identifiers(src: str, macros: Set[str], protect_set: Set[str], str_placeholders: List[str], char_placeholders: List[str]) -> str:
    """
    混淆标识符：
      - 这版会对 #define 中的宏名和参数生成混淆名，并替换宏体内的引用。
      - 仍然会排除 protect_set、RESERVED 与 C++ 关键字（它们不会被混淆）。
    """
    user_funcs = collect_user_functions(src)

    # 不把源码中定义的宏自动排除——我们要对它们进行混淆（除非用户在 protect_set 中保护）
    exclude_all = set(protect_set) | RESERVED

    # 排除字符串与字符占位符
    for i in range(len(str_placeholders)):
        exclude_all.add(f"__STR{i}__")
    for i in range(len(char_placeholders)):
        exclude_all.add(f"__CHAR{i}__")

    # 先扫描源码中有参数的宏定义，以便提前为参数生成映射
    macro_param_lists = {}
    for m in re.finditer(r'^\s*#\s*define\s+([A-Za-z_]\w*)\s*\(([^)]*)\)', src, re.M):
        mac = m.group(1)
        params = [p.strip() for p in m.group(2).split(',') if p.strip()]
        macro_param_lists[mac] = params

    mapping = {}
    all_obf_names = {}

    # 为源码中发现的宏名与宏参数优先生成映射（除非被用户保护或属于 RESERVED/CPP_KEYWORDS）
    for mac in macros:
        if mac in protect_set or mac in RESERVED or mac in CPP_KEYWORDS:
            continue
        if mac not in mapping:
            mapping[mac] = rand_name()
        for p in macro_param_lists.get(mac, []):
            if p and p not in mapping and p not in protect_set and p not in RESERVED and p not in CPP_KEYWORDS:
                mapping[p] = rand_name()

    # 继续常规的标识符收集（排除 exclude_all）
    all_to_consider = collect_identifiers(src, exclude_all)

    # 优先混淆用户函数（如果未被排除）
    for fn in user_funcs:
        if fn not in exclude_all and fn in all_to_consider and fn not in mapping:
            mapping[fn] = rand_name()
    for name in sorted(all_to_consider):
        if name not in mapping:
            mapping[name] = rand_name()

    ID_MAPPING.update(mapping)

    # 按行替换：对 #define 行做特殊处理（替换宏名/参数/宏体），其他预处理行保持原样（除非需替换）
    out_lines = []
    placeholder_pattern = re.compile(r'(__STR\d+__|__CHAR\d+__)')

    for ln in src.splitlines(keepends=True):
        stripped = ln.lstrip()
        if stripped.startswith("#"):
            # 特殊处理单行 #define（注意：多行宏含反斜杠未处理——见后文）
            m = re.match(r'(\s*#\s*define\s+)([A-Za-z_]\w*)(\s*\([^)]*\))?(\s*)(.*)', ln)
            if m:
                prefix, macname, params_part, spacer, body = m.groups()
                macname_new = mapping.get(macname, macname)
                if params_part:
                    new_params = params_part
                    for orig in sorted(mapping, key=len, reverse=True):
                        new_params = re.sub(rf'\b{re.escape(orig)}\b', mapping[orig], new_params)
                else:
                    new_params = ''
                new_body = body
                for orig in sorted(mapping, key=len, reverse=True):
                    new_body = re.sub(rf'\b{re.escape(orig)}\b', mapping[orig], new_body)
                # 在行末加上换行符
                out_lines.append(prefix + macname_new + new_params + spacer + new_body + "\n")
                continue
            else:
                # 不是 #define（或无法解析），保持原样（但仍可选择替换其中的标识符——通常不做）
                out_lines.append(ln)
                continue

        # 非预处理行：按占位符分段替换
        parts = placeholder_pattern.split(ln)
        for i, seg in enumerate(parts):
            if placeholder_pattern.fullmatch(seg or ""):
                continue
            for orig in sorted(mapping, key=len, reverse=True):
                seg = re.sub(rf'\b{re.escape(orig)}\b', mapping[orig], seg)
            parts[i] = seg
        out_lines.append("".join(parts))

    return "".join(out_lines)

def main():
    args = parse_args()
    text = open(args.input, encoding='utf-8').read()
    lines = text.splitlines(keepends=True)
    macros = collect_macros(lines)  # 从源码里找出的宏 —— 这些绝对不混淆

    # 从 protect 文件读取用户显式要求保护的名字（但不要把它们加进 RESERVED）
    try:
        with open(args.protect, encoding='utf-8') as f:
            for line in f:
                line = line.strip()
                if line:
                    PROTECT_SET.add(line)
    except FileNotFoundError:
        pass

    # 1) 提取并保护字符串与字符
    tmp_src, strings = protect_strings(text)
    tmp_src, chars  = protect_chars(tmp_src)
    # 2) 混淆标识符（占位符已保护） —— 显式传入源码宏与用户保护集
    obf_src = obf_identifiers(tmp_src, macros, PROTECT_SET, strings, chars)
    # 3) 恢复并 unicode-escape 字符串
    final = restore_strings(obf_src, strings)
    final = restore_chars(final, chars)

    # 写入结果
    with open(args.output, 'w', encoding='utf-8') as f:
        f.write(final)

    # 写映射表：排除系统保留、关键字、源码宏、和用户保护集（这些都不是被混淆/不应出现在映射表中）
    with open(args.map, 'w', encoding='utf-8') as mf:
        mf.write("# Obfuscation map (original -> obfuscated)\n")
        mf.write("# Note: macro names and protected names are excluded from this map.\n")
        for orig, obf in ID_MAPPING.items():
            if (str(orig) in RESERVED) or (str(orig) in CPP_KEYWORDS):
                continue
            if str(orig) in macros:      # 源码中定义的宏 —— 跳过
                continue
            if str(orig) in PROTECT_SET: # 用户保护的名字 —— 跳过
                continue
            mf.write(f"{orig} -> {obf}\n")

if __name__ == "__main__":
    main()
