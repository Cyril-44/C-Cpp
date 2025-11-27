#include <bits/stdc++.h>
std::string Alice(std::vector<int>);
std::vector<int> Bob(int, std::string);
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int t, n;
    std::cin >> t;
    while (t--) {
        std::cin >> n;
        std::vector<int> a(n);
        for (int i = 0; i < n; i++) std::cin >> a[i];

        std::string data = Alice(std::move(a));
        std::cout << data << std::endl;
        
        std::vector<int> ans = Bob(n, std::move(data));
        std::cout << ans.size();
        for (int i : ans) std::cout << ' ' << i;
        std::cout << std::endl;
    }
    return 0;
}