#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <iostream>
#include <unordered_map>
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
#define plli(x) printf("%lld", x)
#define sc(x) scanf("%c", &x)
#define pc(x) printf("%c", x)
#define newl() printf("\n")
#define fl(i, val, n) for(i = (val);i < (n);i++)
#define fln(i, val, n) for(i = (val);i > (n);i--)
int main() {
    li n, k, * s, i, val;
    lli ans, v1, v2, v3;
    std::unordered_map<li, li> m1, m2, m3;
    std::vector<li> elem;
    sli(n);
    sli(k);
    s = givemem(li, n);
    fl(i, 0, n)
        sli(s[i]);
    fl(i, 0, n) {
        val = s[i];
        if(m1.find(val) == m1.end()) {
            elem.push_back(val);
            m1[val] = 1;
        }
        else
            m1[val] = m1[val] + 1;
        if(val % k == 0)
            val = val / k;
        else
            continue;
        if(m2.find(val) == m2.end())
            m2[val] = 1;
        else
            m2[val] = m2[val] + 1;
        if(val % k == 0)
            val = val / k;
        else
            continue;
        if(m3.find(val) == m3.end())
            m3[val] = 1;
        else
            m3[val] = m3[val] + 1;
    }
    fl(i, 0, elem.size()) {
        val = elem[i];
        v1 = m1[val];
        if(m2.find(val) != m2.end())
            v2 = m2[val];
        else
            continue;
        if(m3.find(val) != m3.end())
            v3 = m3[val];
        else
            continue;
        ans += (v1 * v2 * v3);
        printf("%lld %lld %lld\n", v1, v2, v3);
    }
    plli(ans);
    free(s);
    return 0;
}
