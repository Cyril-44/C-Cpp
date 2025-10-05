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
        cout << "\", ";
    }
    fclose(stdout);
    return 0;
}