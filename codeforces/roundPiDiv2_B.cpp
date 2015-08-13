#include <stdio.h>
#include <iostream>
#include <string.h>
#include <malloc.h>
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
int main() {
    int n, vis[1000001], i, p = 0, maxp = 0;
    li * r;
    char c;
    si(n);
    r = givemem(li, n);
    memset(vis, 0 ,szf(vis));
    fl(i, 0, n) {
        std::cin >> c >> r[i];
        if(c == '-')
            r[i] = -r[i];
    }
    fl(i, 0, n) {
        if(r[i] > 0) {
            vis[r[i]] = 1;
            p++;
            maxp = (maxp < p ? p : maxp);
        }
        else {
            if(vis[-r[i]]) {
                p--;
                vis[-r[i]] = 0;
            }
            else
                maxp++;
        }
    }
    pi(maxp);
    free(r);
    return 0;
}
