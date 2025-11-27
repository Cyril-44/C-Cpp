#include <bits/stdc++.h>
using namespace std;

int main() {
    freopen("Converter.in", "r", stdin);
    freopen("Converter.out", "w", stdout);
    string s;
    while (getline(cin, s)) {
        putchar('\"');
        for (char c : s) {
            if (c == '"' || c == '\\') putchar('\\');
            putchar(c);
        }
        putchar('"');
        putchar(',');
        putchar(' ');
    }
    
    fclose(stdout);
    
    // 将Converter.out的内容复制到剪切板
    if (system("command -v xclip > /dev/null 2>&1") == 0) {
        system("cat Converter.out | xclip -selection clipboard");
        clog << "\nConverter.out内容已复制到剪切板 (使用xclip)" << endl;
    }
    else if (system("command -v xsel > /dev/null 2>&1") == 0) {
        system("cat Converter.out | xsel --clipboard --input");
        clog << "\nConverter.out内容已复制到剪切板 (使用xsel)" << endl;
    }
    else {
        clog << "\n警告: 未找到剪切板工具 (请安装xclip或xsel)" << endl;
        clog << "Converter.out内容未自动复制到剪切板" << endl;
    }
    
    return 0;
}
