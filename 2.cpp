#include <bits/stdc++.h>

const int N = 1 << 15 + 3;

bool vis[N];
double d[N];
std::vector<std::pair<int, double>> g[N];


int main(int argc, char *argv[]) {
    int n = 1;
    n = std::stoi(argv[1]);
    // fprintf(stderr, "%d\n", n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    // build graph
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            auto w = dis(gen);
            g[i].push_back({j, w});
            g[j].push_back({i, w});
        }
    }
    // Dijkstra
    int cnt = 0;
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> q;
    std::fill(d, d + n, 1e10);
    q.push({0.0, 0});
    while (!q.empty()) {
        auto t = q.top();
        q.pop();
        auto u = t.second;
        if (vis[u]) continue;
        d[u] = t.first;
        // fprintf(stderr, "%d %lf\n", u, d[u]);
        vis[u] = 1;
        for (auto e: g[u]) {
            auto v = e.first;
            auto w = e.second;
            if (d[u] + w < d[v]) {
                d[v] = d[u] + w;
                q.push({d[v], v});
                cnt++;
            }
        }
    }
    printf("%d\n", cnt);
    return 0;
}
