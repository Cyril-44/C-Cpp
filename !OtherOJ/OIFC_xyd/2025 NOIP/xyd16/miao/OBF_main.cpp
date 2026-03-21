#include <bits/stdc++.h>
class NameGenerator {
public:
  explicit NameGenerator(size_t len = 32, const std::string &prefix = "")
      : length(len), prefix(prefix) {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    uint64_t seed = static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(now).count());
    rng.seed(seed);

    for (char c = 'A'; c <= 'Z'; ++c)
      alphaNum.push_back(c);
    for (char c = 'a'; c <= 'z'; ++c)
      alphaNum.push_back(c);
    for (char c = '0'; c <= '9'; ++c)
      alphaNum.push_back(c);

    for (char c = 'A'; c <= 'Z'; ++c)
      firstChar.push_back(c);
    for (char c = 'a'; c <= 'z'; ++c)
      firstChar.push_back(c);
    firstChar.push_back('_');

    distAll = std::uniform_int_distribution<size_t>(0, alphaNum.size() - 1);
    distFirst = std::uniform_int_distribution<size_t>(0, firstChar.size() - 1);
  }

  std::string generate() {
    std::string s;
    s.reserve(prefix.size() + length);
    s += firstChar[distFirst(rng)];
    for (size_t i = 1; i < length; ++i)
      s += alphaNum[distAll(rng)];
    if (!prefix.empty())
      s = prefix + s;

    size_t tries = 0;
    while (!used.insert(s).second) {
      if (++tries > 16) {
        s += '_' + std::to_string(tries);
        if (used.insert(s).second)
          break;
      }
      std::string body;
      body += firstChar[distFirst(rng)];
      for (size_t i = 1; i < length; ++i)
        body += alphaNum[distAll(rng)];
      s = prefix.empty() ? body : (prefix + body);
    }
    return s;
  }

  void reset() { used.clear(); }

private:
  size_t length;
  std::string prefix;
  std::mt19937_64 rng;
  std::vector<char> alphaNum;
  std::vector<char> firstChar;
  std::unordered_set<std::string> used;
  std::uniform_int_distribution<size_t> distAll;
  std::uniform_int_distribution<size_t> distFirst;
};
namespace Define_Obfuscation {
/*
Modular obfuscator with single entry:
string obf(const string &input)

Behavior:
- Collects and moves all leading preprocessor lines (line-start '#') to the top.
- Tokenizes the body using a state machine: preserves strings, chars, comments,
NL.
- Removes ordinary spaces but inserts explicit space tokens where necessary
(ident/num adjacency).
- Splits tokens into segments and generates nested #define macros per segment.
- Returns the complete obfuscated source text as a single string.

Notes:
- Configuration constants (SEG_SIZE, CHUNK_SIZE, PREFIX) are local and can be
adjusted.
- This function returns only the obfuscated .c/.cpp content; if you want JSON
mapping or per-segment files, the code can be extended.
*/
// obfuscate_mod_define_top.cpp
// Requires C++17
using std::pair;
using std::string;
using std::unordered_set;
using std::vector;

pair<vector<string>, vector<string>>
splitCppTokensWithDirectives(const string &s) {
  static const unordered_set<string> two = {
      "==", "!=", "<=", ">=", "&&", "||", "++", "--", "+=", "-=", "*=",
      "/=", "%=", "&=", "|=", "^=", "<<", ">>", "->", "::", ".*", "->*"};
  static const unordered_set<char> one = {
      '+', '-', '*', '/', '%', '&', '|', '^', '~', '!', '=', '<', '>',
      '(', ')', '[', ']', '{', '}', ',', ';', '.', ';', ':', '?'};

  vector<string> tokens;
  vector<string> directives;
  string cur;
  auto flush = [&]() {
    if (!cur.empty()) {
      tokens.push_back(cur);
      cur.clear();
    }
  };

  enum State {
    Normal,
    InStr,
    InChar,
    LineCmt,
    BlockCmt,
    InDirective
  } st = Normal;
  for (size_t i = 0; i < s.size(); ++i) {
    char c = s[i];

    // 状态：字符串字面量
    if (st == InStr) {
      cur.push_back(c);
      if (c == '"' && s[i - 1] != '\\') {
        st = Normal;
        flush();
      }
      continue;
    }
    // 状态：字符字面量
    if (st == InChar) {
      cur.push_back(c);
      if (c == '\'' && s[i - 1] != '\\') {
        st = Normal;
        flush();
      }
      continue;
    }
    // 状态：行注释
    if (st == LineCmt) {
      if (c == '\n')
        st = Normal;
      continue;
    }
    // 状态：块注释
    if (st == BlockCmt) {
      if (c == '*' && i + 1 < s.size() && s[i + 1] == '/') {
        st = Normal;
        ++i;
      }
      continue;
    }
    // 状态：预处理指令，收集整行（含换行之前所有字符）
    if (st == InDirective) {
      // 收集直到未被反斜线续行或者行尾
      size_t start = i;
      bool cont = false;
      for (; i < s.size(); ++i) {
        if (s[i] == '\\') {
          // 如果是行续行，跳过下一字符并继续
          if (i + 1 < s.size() && s[i + 1] == '\n') {
            cont = true;
            ++i;
            continue;
          }
          if (i + 1 == s.size())
            break;
        }
        if (s[i] == '\n')
          break;
      }
      size_t end = (i < s.size() ? i : s.size() - 1);
      // 包含起始 '#' 前的任何前导空白
      string dir = s.substr(start, end - start + 1);
      // 去除末尾的换行（保留行内内容）
      if (!dir.empty() && dir.back() == '\n')
        dir.pop_back();
      directives.push_back(dir);
      st = Normal;
      continue;
    }

    // Normal 状态处理
    // 识别进入字符串/字符/注释
    if (c == '"') {
      flush();
      cur.push_back(c);
      st = InStr;
      continue;
    }
    if (c == '\'') {
      flush();
      cur.push_back(c);
      st = InChar;
      continue;
    }
    if (c == '/' && i + 1 < s.size() && s[i + 1] == '/') {
      st = LineCmt;
      ++i;
      continue;
    }
    if (c == '/' && i + 1 < s.size() && s[i + 1] == '*') {
      st = BlockCmt;
      ++i;
      continue;
    }

    // 识别预处理指令：行首（允许前导空白）遇到 '#'
    if (c == '#') {
      // 确保这是行首（前面只有空白或在字符串开始）
      bool onlySpaceBefore = true;
      if (i > 0) {
        size_t j = i;
        while (j > 0) {
          --j;
          char p = s[j];
          if (p == '\n')
            break;
          if (!std::isspace(static_cast<unsigned char>(p))) {
            onlySpaceBefore = false;
            break;
          }
        }
      }
      if (onlySpaceBefore) {
        flush();
        // 把当前 '#' 的位置传给 InDirective 处理（它期望 i 为起点）
        st = InDirective;
        // 将循环索引回退一个位置，因为 InDirective 分支从 i 开始读取
        // 实际上当前循环会执行 ++i, 所以需要 --i 保持位置不变，下一轮进入
        // InDirective
        --i;
        continue;
      }
    }

    if (std::isspace(static_cast<unsigned char>(c))) {
      flush();
      continue;
    }

    // 尝试两字符运算符（最长优先）
    if (i + 1 < s.size()) {
      string t;
      t.reserve(2);
      t.push_back(c);
      t.push_back(s[i + 1]);
      if (two.find(t) != two.end()) {
        flush();
        tokens.push_back(t);
        ++i;
        continue;
      }
    }
    // 单字符运算符
    if (one.find(c) != one.end()) {
      flush();
      tokens.emplace_back(1, c);
      continue;
    }

    // 一般标识符/数字/其他字符
    cur.push_back(c);
  }
  flush();
  return {tokens, directives};
}

// 返回：edges（parent->children 列表，按创建顺序），以及 root 名称，
// 同时也返回叶子名列表以便打印 leaf_val 行（叶子名与 leaves 索引对应）
pair<pair<vector<pair<string, vector<string>>>, string>, vector<string>>
build_k_ary_tree_with_wrapped_leaves(const vector<string> &leaves,
                                     NameGenerator &gen, int K) {
  if (K <= 0)
    throw std::invalid_argument("K must be >= 1");

  int n = static_cast<int>(leaves.size());
  vector<pair<string, vector<string>>> edges;
  vector<string> leaf_names;
  leaf_names.reserve(n);

  // 1) 为每个叶子生成一个 gen 名称（包装层）
  for (int i = 0; i < n; ++i) {
    leaf_names.push_back(gen.generate());
  }

  // 2) 当前层名列表初始化为叶子名（按输入顺序）
  vector<string> cur = leaf_names;

  // 如果没有叶子，生成单个 root 名称并返回
  if (cur.empty()) {
    string lone = gen.generate();
    return {{edges, lone}, leaf_names};
  }

  // 3) 非递归向上分组构造，每组最多 K 个孩子
  while (cur.size() > 1) {
    vector<string> next;
    next.reserve((cur.size() + K - 1) / K);
    for (size_t i = 0; i < cur.size(); i += K) {
      size_t end = std::min(cur.size(), i + K);
      string parent = gen.generate();
      vector<string> children;
      children.reserve(end - i);
      for (size_t j = i; j < end; ++j)
        children.push_back(cur[j]);
      edges.emplace_back(parent, std::move(children));
      next.push_back(parent);
    }
    cur.swap(next);
  }

  // cur[0] 即为根名
  string root = cur.front();
  return {{edges, root}, leaf_names};
}

// ----------------- print_structure_wrapped (重写，新增 ofstream& outfs)
// -----------------
void print_structure_wrapped(const vector<pair<string, vector<string>>> &edges,
                             const string &root,
                             const vector<string> &leaf_names,
                             const vector<string> &leaves,
                             std::ofstream &outfs) {
  // 叶子行：GEN_LEAF_NAME leaf_val
  vector<size_t> arr(leaf_names.size());
  std::iota(arr.begin(), arr.end(), 0);
  std::mt19937_64 rng(time(nullptr));
  std::shuffle(arr.begin(), arr.end(), rng);
  for (size_t i = 0; i < leaf_names.size(); ++i) {
    outfs << "#define " << leaf_names[arr[i]] << ' ' << leaves[arr[i]] << '\n';
  }
  // parent -> children 行（按创建顺序）
  for (const auto &e : edges) {
    outfs << "#define " << e.first;
    for (const auto &c : e.second)
      outfs << ' ' << c;
    outfs << '\n';
  }
  // 最后一行：root followed by its immediate children if available
  const vector<string> *root_children = nullptr;
  for (const auto &e : edges) {
    if (e.first == root) {
      root_children = &e.second;
      break;
    }
  }
  if (!root_children) {
    outfs << "#define " << root << '\n';
  } else {
    outfs << "#define " << root;
    for (const auto &c : *root_children)
      outfs << ' ' << c;
    outfs << '\n';
    outfs << root << '\n';
  }
}
// ----------------- process 总函数 -----------------
void obf(const string &src, std::ofstream &outfs, NameGenerator &gen, int K) {
  // 1) 拆分（这里把拆分结果作为叶子值的简化示例）
  auto [tokens, def] = splitCppTokensWithDirectives(src);
  for (const auto &s : def)
    outfs << s << '\n';
  // 如果你希望把整行/语句作为叶子，请先把 src 分割成行或逻辑单元；此处示例用
  // tokens 作为叶子值 2) 构造树（叶子先用 gen 包装）
  auto built = build_k_ary_tree_with_wrapped_leaves(tokens, gen, K);
  const auto &edges = built.first.first;
  const auto &root = built.first.second;
  const auto &leaf_names = built.second;
  // 3) 打印到 outfs
  print_structure_wrapped(edges, root, leaf_names, tokens, outfs);
}

} // namespace Define_Obfuscation
std::string readFile(const std::string &path) {
    std::ifstream in(path);
    std::ostringstream ss;
    ss << in.rdbuf();   // 把整个文件缓冲区写入到 stringstream
    return ss.str();    // 转成 string
}
int main() {
    NameGenerator gen;
    std::ofstream os("OBF.cpp");
    Define_Obfuscation::obf(readFile("miao_wtz.cpp"), os, gen, 20);
    os.close();
}