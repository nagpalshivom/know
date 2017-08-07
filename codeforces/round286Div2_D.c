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
    li n, m, * a, * b, i;
    sli(n);
    sli(m);
    a = (li *)malloc(sizeof(li) * m);
    b = (li *)malloc(sizeof(li) * m);
    for(i = 0;i < m;i++) {
        sli(a[i]);
        sli(b[i]);
    }
    free(a);
    free(b);
    return 0;
}
