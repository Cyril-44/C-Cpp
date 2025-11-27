import re
import random
import string
import random

def generate_name(length = 32):
    # 可选样式规则
    styles = [
        # lambda: 'o' + ''.join(random.choices(['o', '0'], k=length)),
        lambda: 'l' + ''.join(random.choices(['1', 'l'], k=length)),
    ]
    return random.choice(styles)()


def load_target_types(filename):
    with open(filename, "r", encoding="utf-8") as f:
        return [line.strip() for line in f if line.strip()]

def extract_variables_by_type(code, type_list):
    # 构建动态类型匹配模式
    type_pattern = r'|'.join([re.escape(t) for t in type_list])
    decl_pattern = rf'\b({type_pattern})\b\s+([^;{{}}]+);'  # 避免误包含初始值

    matches = re.findall(decl_pattern, code)
    variables = set()

    for typename, decl in matches:
        parts = re.split(r',', decl)
        for part in parts:
            part = part.strip()
            # 去除数组维度和初始化表达式
            part = re.sub(r'\[.*?\]', '', part)   # 删除数组维度
            part = re.sub(r'=.*$', '', part)      # 删除初始化部分
            var_match = re.match(r'[\*&\s]*(\w+)', part)
            if var_match:
                varname = var_match.group(1)
                if re.fullmatch(r'[a-zA-Z_]\w*', varname):  # 排除数字和非法名
                    variables.add(varname)

    return variables

def obfuscate_variables(code, variables):
    mapping = {var: generate_name() for var in variables}
    for var, obf in mapping.items():
        code = re.sub(rf'\b{re.escape(var)}\b', obf, code)
    return code, mapping

def obfuscate_char_literals(code):
    char_pattern = r"'(\\.|[^\\'])'"
    mapping = {}

    def replace_char(match):
        char = match.group(1)
        if char.startswith('\\'):
            ascii_value = ord(eval(f"'{char}'"))
        else:
            ascii_value = ord(char)
        mapping[match.group(0)] = str(ascii_value)
        return str(ascii_value)

    code = re.sub(char_pattern, replace_char, code)
    return code, mapping

def main():
    # 🚀 加载用户指定类型列表
    target_types = load_target_types("OBF.to.map")

    with open("OBF.in", "r", encoding="utf-8") as f:
        source = f.read()

    var_list = extract_variables_by_type(source, target_types)
    code, var_map = obfuscate_variables(source, var_list)
    code, char_map = obfuscate_char_literals(code)

    with open("OBF.out", "w", encoding="utf-8") as f:
        f.write(code)

    with open("OBF.map", "w", encoding="utf-8") as f:
        for k, v in var_map.items():
            f.write(f"VAR {k} -> {v}\n")
        for k, v in char_map.items():
            f.write(f"CHAR {k} -> {v}\n")

if __name__ == "__main__":
    main()
