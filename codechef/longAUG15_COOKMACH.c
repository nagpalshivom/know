#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define li long int
#define lli long long int
#define setmem(x, val, t, n) memset(x, val, sizeof(t) * (n))
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
int isPow2(li n) {
    if(n == 1 || n == 2 || n == 4 || n == 8 || n == 16 || n == 32 || n == 64 || n == 128 || n == 256 || n == 512 || n == 1024 || n == 2048 || n == 4096 || n == 8192 || n == 16384 || n == 32768 || n == 65536 || n == 131072 || n == 262144 || n == 524288 || n == 1048576 || n == 2097152 || n == 4194304 || n == 8388608)
        return 1;
    else return 0;
}
int dist(li a, li b) {
    int cnt = 0;
    if(a > b) {
        while(a != b) {
            a = a / 2;
            cnt++;
        }
    }
    else {
        while(a != b) {
            b = b / 2;
            cnt++;
        }
    }
    return cnt;
}
int main() {
    int t, cnt;
    si(t);
    li a, b;
    while(t--) {
        cnt = 0;
        sli(a);
        sli(b);
        if(a == b)
            pi(0);
        else {
            if(isPow2(a))
                pi(dist(a, b));
            else {
                while(!isPow2(a)) {
                    a = a / 2;
                    cnt++;
                }
                pi(cnt + dist(a, b));
            }
        }
        pc('\n');
    }
    return 0;
}
