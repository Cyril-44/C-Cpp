import re

def replace_char_patterns(file_path):
    try:
        # 读取文件内容
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
        
        # 定义正则表达式模式，匹配 (char)([num]) 格式
        pattern = r'\(char\)\((\d+)\)'
        
        # 定义特殊字符的映射
        escape_chars = {
            10: '\\n',   # 换行符
            9: '\\t',    # 制表符
            13: '\\r',   # 回车符
            34: '\\"',   # 双引号
            39: "\\'",   # 单引号
            92: '\\\\'   # 反斜杠
        }
        
        # 定义替换函数
        def replace_match(match):
            num = int(match.group(1))
            
            # 检查是否为特殊转义字符
            if num in escape_chars:
                return f"'{escape_chars[num]}'"
            else:
                # 检查是否为可打印字符
                if 32 <= num <= 126:  # 可打印ASCII字符范围
                    char = chr(num)
                    # 如果是单引号或反斜杠，需要转义
                    if char == "'" or char == '\\':
                        return f"'\\{char}'"
                    else:
                        return f"'{char}'"
                else:
                    # 不可打印字符显示为数字形式
                    return f"'\\x{num:02x}'"
        
        # 执行替换
        new_content = re.sub(pattern, replace_match, content)
        
        # 写回文件
        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(new_content)
        
        print(f"成功处理文件: {file_path}")
        
    except FileNotFoundError:
        print(f"错误: 找不到文件 {file_path}")
    except Exception as e:
        print(f"处理文件时出错: {e}")

# 使用示例
if __name__ == "__main__":
    replace_char_patterns("OBF.out")
