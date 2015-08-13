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
    li n, m, ans = 0, cnt, ind, i;
    int l, r;
    char str[300000], f = 't', c;
    sli(n);
    sli(m);
    scanf("%s", str);
    cnt = 0;
    fl(i, 0, n) {
        if(f == 't' && str[i] == '.') {
            cnt = 1;
            f = 'f';
        }
        else {
            if(str[i] == '.')
                cnt++;
            else {
                if(cnt) {
                    ans += (cnt - 1);
                    cnt = 0;
                }
                f = 't';
            }
        }
    }
    if(cnt)
        ans += (cnt - 1);
    fl(i, 0, m) {
        scanf("%ld%c%c", &ind, &c, &c);
        ind--;
        if((ind - 1) >= 0 && str[ind - 1] == '.')
            l = 1;
        else
            l = 0;
        if((ind + 1) < n && str[ind + 1] == '.')
            r = 1;
        else
            r = 0;
        if(c == '.' && str[ind] != '.') {
            if(l && r)
                ans += 2;
            else if(l || r)
                ans += 1;
        }
        else if(c != '.' && str[ind] == '.'){
            if(l && r)
                ans -= 2;
            else if(l || r)
                ans -= 1;
        }
        str[ind] = c;
        pli(ans);
        pc('\n');
    }
    return 0;
}
