#include <stdio.h>
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
    int n, m, i, j, * c;
    li ** v, max, winc;
    si(n);
    si(m);
    v = givemem(li *, m);
    c = givemem(int, n);
    memset(c, 0, sizeof(int) * n);
    fl(i, 0, m)
        v[i] = givemem(li, n);
    fl(i, 0, m) {
        fl(j, 0, n)
            sli(v[i][j]);
    }
    fl(i, 0, m) {
        max = v[i][0];
        winc = 0;
        fl(j, 1, n) {
            if(v[i][j] > max) {
                max = v[i][j];
                winc = j;
            }
        }
        c[winc]++;
    }
    max = c[0];
    winc = 0;
    fl(i, 0, n) {
        if(max < c[i]) {
            max = c[i];
            winc = i;
        }
    }
    pli(winc + 1);
    fl(i, 0, m)
        free(v[i]);
    free(v);
    free(c);
    return 0;
}
