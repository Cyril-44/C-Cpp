#include <iostream>
#include <string>
#include <algorithm>
int main() {
    std::string s, rs;
    std::cin >> s;
    rs = s;
    std::reverse(rs.begin(), rs.end());
    std::string ans;
    bool flg = true;
    for (int i = s.size(); i >= 1; i--, flg = !flg)
        if (flg) ans += s.substr(0, i);
        else ans += rs.substr(0, i);
    std::cout << ans << '\n';
    return 0;
}