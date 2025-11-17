import json

def generate_markdown(data):
    title = data["titleZh"] or "题目"
    description = data["statementZh"]
    input_format = data["inputFormatZh"]
    output_format = data["outputFormatZh"]
    constraints = data["constraintZh"]
    explanation = data.get("explanationZh", "")
    examples = data.get("examples", [])

    md = f"# 📘 题目：{title}\n\n"
    md += f"## 🧾 题目描述\n{description}\n\n"
    md += f"## 📥 输入格式\n{input_format}\n\n"
    md += f"## 📤 输出格式\n{output_format}\n\n"
    md += f"## 📌 约束条件\n{constraints}\n\n"

    if explanation:
        md += f"## 📚 题解说明\n{explanation}\n\n"

    if examples:
        md += "## 🧪 样例\n"
        for i, ex in enumerate(examples, 1):
            md += f"### 样例 {i}\n"
            md += f"**输入：**\n```\n{ex['input']}\n```\n"
            md += f"**输出：**\n```\n{ex['output']}\n```\n\n"

    return md

# 示例：从 JSON 文件或字符串中读取数据
with open("response.json", "r", encoding="utf-8") as f:
    response = json.load(f)

markdown = generate_markdown(response["data"])
print(markdown)
