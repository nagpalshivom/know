#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define li long int
#define lli long long int
#define setZ(x) memset(x, 0, sizeof(x))
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
int main() {
    int i = 2, index;
    li t;
    lli n, fib[100], sum[100];
    sli(t);
    fib[0] = 0;
    fib[1] = 1;
    sum[0] = 0;
    sum[1] = 1;
    fl(i, 2, 100) {
        fib[i] = fib[i - 1] + fib[i - 2];
        sum[i] = fib[i] + sum[i - 1];
    }
    while(t--) {
        slli(n);
        fl(i, 1, 100) {
            if(n == sum[i]) {
                pi(i);
                break;
            }
            else if(n > sum[i] && n < sum[i + 1]) {
                pi(i + 1);
                break;
            }
        }
        pc('\n');
    }
    return 0;
}
