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
void mergePart(li * v, long int l1, long int h1, long int l2, long int h2) {
    long int size = h2 - l1 + 1, i = 0, start = l1;
    li * tempV = (li *)malloc(sizeof(li) * size);
    while(l1 <= h1 && l2 <= h2) {
        if(v[l1] < v[l2])
            tempV[i++] = v[l1++];
        else
            tempV[i++] = v[l2++];
    }
    while(l1 <= h1)
        tempV[i++] = v[l1++];
    while(l2 <= h2)
        tempV[i++] = v[l2++];
    memcpy(&v[start], tempV, sizeof(li) * size);
    free(tempV);
}
void mergeSort(li * v, long int l, long int h){
    if(l == h)
        return;
    mergeSort(v, l, (l + h) / 2);
    mergeSort(v, (l + h) / 2 + 1, h);
    mergePart(v, l, (l + h) / 2, (l + h) / 2 + 1, h);
}
int main() {
    li n, k, a, m, * p, i, pval, s, e, du[2 * 100005], * revp, cheatCnt;
    sli(n);
    sli(k);
    sli(a);
    sli(m);
    memset(du, 0, szf(du));
    p = givemem(li, m);
    revp = givemem(li, m);
    fl(i, 0, m) {
        sli(p[i]);
        revp[m - i - 1] = p[i];
    }
    mergeSort(p, 0, m - 1);
    s = 1;
    cheatCnt = 0;
    fl(i, 0, m) {
        pval = p[i];
        du[pval] = 0;
        e = pval - 1;
        if(e >= s)
            du[s] = e;
        while(e > s) {
            du[e] = s;
            e--;
        }
        cheatCnt += ((pval - s + 1) / (a + 1));
        s = pval + 1;
    }
    fl(i, 0, m) {
        if(cheatCnt > k)
            break;
        pval = revp[i];
        if(du[pval - 1])
            du[pval] = du[pval - 1]; // here we are combining two separate partitions .. if they combine we have to find the increase in number 
        else// of ships p1end - p1start + p2end - p2start = prev ships
            du[pval] = pval;// p2end - p1start = new count
        if(du[pval + 1])//we have to increase the cheatCnt by (new cnt - prev ships)
            du[pval + 1] = du[pval];//next loop will check whether cheat cnt tells that required number of ships are now possible and we can say yes
        cheatCnt += 
    }
    free(p);
    free(revp);
    return 0;
}
