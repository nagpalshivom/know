#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <vector>
#define li long int
#define lli long long int
#define szf sizeof
#define givemem(t, n) (t*)malloc(sizeof(t) * (n))
#define sstr(x) scanf("%s", str);
#define si(x) scanf("%d", &x)
#define pi(x) printf("%d", x)
#define sli(x) scanf("%ld", &x)
#define pli(x) printf("%ld", x)
#define slli(x) scanf("%l64d", &x)
#define plli(x) printf("%l64d", x)
#define sc(x) scanf("%c", &x)
#define pc(x) printf("%c", x)
#define newl() printf("\n")
#define fl(i, val, n) for(i = (val);i < (n);i++)
#define fln(i, val, n) for(i = (val);i > (n);i--)
std::vector<int> g[101][101];
int vis[101];
int checkPath(int c, int v1, int v2) {
    int vertex, i;
    std::vector<int> queue;
    queue.push_back(v1);
    while(queue.size() != 0) {
        vertex = queue.front();
        queue.erase(queue.begin());
        if(vis[vertex])
            continue;
        else
            vis[vertex] = 1;
        if(vertex == v2)
            return 1;
        fl(i, 0, g[c][vertex].size())
            queue.push_back(g[c][vertex].at(i));
    }
    return 0;
}
int main() {
    int n, m, a, b, c, q, col[101], i, ans, v2, v1;
    si(n);
    si(m);
    memset(col, 0, szf(col));
    while(m--) {
        si(a);
        si(b);
        si(c);
        g[c][a].push_back(b);
        g[c][b].push_back(a);
        col[c] = 1;
    }
    si(q);
    while(q--) {
        si(v1);
        si(v2);
        ans = 0;
        fl(i, 1, 101) {
            if(col[i]) {
                memset(vis, 0, szf(vis));
                if(checkPath(i, v1, v2))
                    ans++;
            }
        }
        pi(ans);
        newl();
    }
    return 0;
}