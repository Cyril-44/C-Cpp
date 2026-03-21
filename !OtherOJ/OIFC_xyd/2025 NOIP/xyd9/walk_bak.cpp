
void dfs1(int u, int pre, int dep) {
    if (dep > furdis)
        furdis = dep, furpnt = u;
    for (int i = head[u], v; i; i = e[i].nxt) {
        if ((v = e[i].to) == pre)
            continue;
        dfs1(v, u, dep + 1);
    }
}
std::vector<int> sons[N];
int dfs2(int u, int pre) {
    std::vector<std::pair<int, int>> son;
    int mx = 0;
    for (int i = head[u], v; i; i = e[i].nxt) {
        if ((v = e[i].to) == pre)
            continue;
        int tmp = dfs2(v, u);
        umax(mx, tmp + 1);
        son.emplace_back(tmp, v);
    }
    std::sort(son.begin(), son.end());
    sons[u].clear();
    sons[u].reserve(son.size());
    for (const auto& [d, v] : son)
        sons[u].push_back(v);
    return mx;
}
int ts;
int mn[N], mx[N], seq[N << 1], cnt;
void dfs3(int u) {
    int tmp = ++cnt;
    mn[tmp] = ++ts;
    seq[ts] = u;
    if (!sons[u].empty()) {
        for (int v : sons[u])
            dfs3(v);
        mx[tmp] = ++ts;
        seq[ts] = u;
    }
    else mx[tmp] = ts;
}