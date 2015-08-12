//The second solution for the problem passes the 2 second barrier easily.
//This solutions gets TLE for the last test case
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define li long int
#define lli long long int
#define setZ(x) memset(x, 0, sizeof(x));
#define givemem(t, n) (t*)malloc(sizeof(t) * (n))
#define si(x) fri(&x)
#define pi(x) writeInt(x)
#define sli(x) fril(&x)
#define slli(x) frill(&x)
#define gc getchar_unlocked
#define pc putchar_unlocked
#define fl(i, val, n) for(i = (val);i < (n);i++)
#define fln(i, val, n) for(i = (val);i > (n);i--)
inline void writeInt(long long int n) {
    long long int N = n, rev, count = 0;
    rev = N;
    if (N == 0) {
        pc('0'); 
        return;
    }
    while ((rev % 10) == 0) {
        count++; 
        rev /= 10;
    } //obtain the count of the number of 0s
    rev = 0;
    while (N != 0) {
        rev = (rev << 3) + (rev << 1) + N % 10; 
        N /= 10;
    }  //store reverse of N in rev
    while (rev != 0) { 
        pc(rev % 10 + '0'); 
        rev /= 10;
    }
    while (count--) {
        pc('0');
    }
}
inline void frs(char * str) {
    register char c = 0;
    register int i = 0;
    while (c < 33)
        c = gc();
    while (c != '\n') {
        str[i] = c;
        c = gc();
        i = i + 1;
    }
    str[i] = '\0';
}
inline void fril(long int * x) {
    register int c = gc();
    *x = 0;
    int neg = 0;
    for(; ((c < 48 || c > 57) && c != '-'); 
            c = gc());
    if(c == '-') {
        neg = 1;
        c = gc();
    }
    for(; c > 47 && c < 58 ; c = gc()) {
        *x = ((*x) << 1) + ((*x) << 3) + c - 48;
    }
    if(neg)
        *x = -(*x);
}
inline void frill(long long int * x) {
    register int c = gc();
    *x = 0;
    int neg = 0;
    for(; ((c < 48 || c > 57) && c != '-'); 
            c = gc());
    if(c == '-') {
        neg = 1;
        c = gc();
    }
    for(; c > 47 && c < 58 ; c = gc()) {
        *x = ((*x) << 1) + ((*x) << 3) + c - 48;
    }
    if(neg)
        *x = -(*x);
}
inline void fri(int * x) 
{
    register int c = gc();
    *x = 0;
    int neg = 0;
    for(; ((c < 48 || c > 57) && c != '-'); 
            c = gc());
    if(c == '-') {
        neg = 1;
        c = gc();
    }
    for(; c > 47 && c < 58 ; c = gc()) {
        *x = ((*x) << 1) + ((*x) << 3) + c - 48;
    }
    if(neg)
        *x = -(*x);
}
//lli segTree[2100000];
//li lazy[2100000];
lli * segTree;
li * lazy;
void update_tree(lli seg, li s, li e, li us, li ue) {
    lli left, right;
    li mid;
    left = (seg << 1) + 1;
    right = left + 1;
    if(lazy[seg]) {
        segTree[seg] += (long long int)(lazy[seg] * (e - s + 1));
        if(s != e) {
            lazy[left] += lazy[seg];
            lazy[right] += lazy[seg];
        }
        lazy[seg] = 0;
    }
    if(us > e || ue < s)
        return;
    if(s >= us && e <= ue) {
        segTree[seg] += (long long int)(e - s + 1);
        if(s != e) {
            lazy[left]++;
            lazy[right]++;
        }
    }
    else {
        mid = (s + e) / 2;
        update_tree(left, s, mid, us, ue);
        update_tree(right, mid + 1, e, us, ue);
        segTree[seg] = segTree[left] + segTree[right];
    }
}
lli query_tree(lli seg, li s, li e, li qs, li qe) {
    li mid;
    lli left, right;
    left = (seg << 1) + 1;
    right = left + 1;
    if(lazy[seg]) {
        segTree[seg] += (long long int)(lazy[seg] * (e - s + 1));
        if(s != e) {
            lazy[left] += lazy[seg];
            lazy[right] += lazy[seg];
        }
        lazy[seg] = 0;
    }
    if(qs > e || qe < s)
        return 0;
    if(s >= qs && e <= qe)
        return segTree[seg];
    else {
        mid = (s + e) / 2;
        return query_tree(left, s, mid, qs, qe) + query_tree(right, mid + 1, e, qs, qe);
    }
}
li ih[1000005], il[1000005];
int main() {
    int t;
    li n, h, i, zn, zh;
    lli maxSpaces, emptySpaces;
    si(t);
    while(t--) {
        maxSpaces = 0;
        //setZ(lazy);
        lazy = (li *)calloc(2098000, sizeof(li));
        segTree = (lli *)calloc(2098000, sizeof(lli));
        //setZ(segTree);
        sli(n);
        sli(h);
        fl(i, 0, n) {
            sli(il[i]);
            sli(ih[i]);
        }
        zn = n - 1;
        zh = h - 1;
        fl(i, 0, n)
            update_tree(0, 0, zn, il[i], ih[i]);
        fl(i, 0, n - zh) {
            emptySpaces = query_tree(0, 0, zn, i, i + zh);
            if(maxSpaces < emptySpaces)
                maxSpaces = emptySpaces;
        }
        pi((long long int)((long long int)n * h - maxSpaces));
        pc('\n');
        free(lazy);
        free(segTree);
    }
    return 0;
}
