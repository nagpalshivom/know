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
li myabs(li val) {
    return val > 0 ? val : (-val);
}
li max(li a, li b) {
    return a > b ? a : b;
}
li min(li a, li b) {
    return a < b ? a : b;
}
int main() {
    li n, * c, i, f, l, mind, maxd;
    sli(n);
    c = givemem(li, n);
    fl(i, 0, n)
        sli(c[i]);
    f = c[0];
    l = c[n - 1];
    fl(i, 0, n) {
        maxd = max(myabs(l - c[i]), myabs(c[i] - f));
        if(i == 0)
            mind = myabs(c[1] - c[0]);
        else if(i == (n - 1))
            mind = myabs(c[n - 1] - c[n - 2]);
        else
            mind = min(myabs(c[i + 1] - c[i]), myabs(c[i] - c[i - 1]));
        pli(mind);
        pc(' ');
        pli(maxd);
        newl();
    }
    free(c);
    return 0;
}
