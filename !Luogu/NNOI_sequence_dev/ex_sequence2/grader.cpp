#include <bits/stdc++.h>
std::string Alice(std::vector<long long>);
std::vector<long long> Bob(int, std::string);
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<long long> a(n);
    for (int i = 0; i < n; i++) std::cin >> a[i];

    std::string data = Alice(std::move(a));
    std::cout << data << std::endl;
    
    std::vector<long long> ans = Bob(n, std::move(data));
    std::cout << ans.size();
    for (long long i : ans) std::cout << ' ' << i;
    std::cout << std::endl;
    return 0;
}