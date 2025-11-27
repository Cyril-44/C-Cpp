import re

def is_space(ch):
    return ch in (' ', '\t', '\n', '\r', '\v', '\f')

def is_identifier_part(ch):
    return ch.isalnum() or ch in ['$', '_']

def safe_index_of(s, ch, start):
    while start < len(s) and not is_space(s[start]):
        if s[start] == ch:
            return start
        start += 1
    return -1

def check(a, b):
    return (is_identifier_part(a) and is_identifier_part(b)) or \
           ((a in ['+', '-']) and a == b) or \
           (a == '/' and b == '*')

def process_trigraph(s):
    trigraphs = {
        '=': '#',
        '/': '\\',
        '\'': '^',
        '(': '[',
        ')': ']',
        '<': '{',
        '>': '}',
        '!': '|',
        '-': '~'
    }
    for ch, replacement in trigraphs.items():
        s = s.replace(f"??{ch}", replacement)
    return s

def process_line_break(s):
    lines = s.splitlines()
    ret = ''
    for line in lines:
        j = len(line) - 1
        while j >= 0 and is_space(line[j]):
            j -= 1
        if j >= 0 and line[j] == '\\':
            ret += line[:j]
        else:
            ret += line + '\n'
    return ret.rstrip('\n')

def process_multiline_comment_and_replace_mark(s):
    ret = ''
    i = 0
    esc = False
    in_string = False
    string_begin = ''
    lst = 0
    while i < len(s):
        if in_string:
            if not esc and s[i] == string_begin:
                in_string = False
        elif s[i] in ['\'', '"']:
            in_string = True
            string_begin = s[i]
        elif i + 1 < len(s):
            if s[i] == '/' and s[i + 1] == '*':
                ret += s[lst:i] + ' '
                end = s.find('*/', i + 2)
                if end == -1:
                    break
                i = lst = end + 2
                continue
            elif s[i] == '%' and s[i + 1] == ':':
                ret += s[lst:i] + '#'
                i += 2
                lst = i
                continue
        esc = s[i] == '\\'
        i += 1
    ret += s[lst:]
    return ret

def compress_single(s):
    s = process_trigraph(s)
    s = process_line_break(s)
    s = process_multiline_comment_and_replace_mark(s)
    lines = s.splitlines()
    ret = ''
    force_newline = True
    last = ''
    for line in lines:
        if not line.strip():
            continue
        i = 0
        tokens = []
        while i < len(line):
            while i < len(line) and is_space(line[i]):
                i += 1
            if i >= len(line):
                break
            start = i
            in_string = False
            string_begin = ''
            esc = False
            while i < len(line) and (in_string or not is_space(line[i])):
                if in_string:
                    if not esc and line[i] == string_begin:
                        in_string = False
                elif line[i] in ['"', "'"]:
                    in_string = True
                    string_begin = line[i]
                elif line[i] == '/' and i + 1 < len(line) and line[i + 1] == '/':
                    break
                esc = line[i] == '\\'
                i += 1
            tokens.append(line[start:i])
            if i + 1 < len(line) and line[i] == '/' and line[i + 1] == '/':
                tokens.append(line[i:])
                break
        if last and tokens:
            if force_newline:
                ret += '\n'
            elif tokens[0] and check(last, tokens[0][0]):
                ret += ' '
        j = 0
        if tokens[0].startswith('#'):
            if not force_newline:
                ret += '\n'
            force_newline = True
            ret += tokens[j]
            j += 1
            while j < len(tokens) and not tokens[j].startswith('('):
                ret += ' ' + tokens[j]
                j += 1
        else:
            force_newline = False
        while j < len(tokens):
            if j > 0 and tokens[j - 1] and tokens[j] and check(tokens[j - 1][-1], tokens[j][0]):
                ret += ' '
            ret += tokens[j]
            j += 1
        last = tokens[-1][-1] if tokens else last
    return ret

def compress(s):
    pattern_start = 'R"('
    pattern_end = ')"'
    result = ''
    i = 0
    while i < len(s):
        start = s.find(pattern_start, i)
        if start == -1:
            result += compress_single(s[i:])
            break
        result += compress_single(s[i:start])
        end = s.find(pattern_end, start + len(pattern_start))
        if end == -1:
            result += s[start:]
            break
        result += s[start:end + len(pattern_end)]
        i = end + len(pattern_end)
    return compress_single(result)

def main():
    with open('Compress.in', 'r', encoding='utf-8') as f:
        input_data = f.read()
    output_data = compress(input_data)
    with open('Compress.out', 'w', encoding='utf-8') as f:
        f.write(output_data)

if __name__ == '__main__':
    main()
