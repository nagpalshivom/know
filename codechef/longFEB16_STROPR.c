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
#define MOD 1000000007

void writeInt(long long int n) {
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

void frs(char * str) {
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

void fril(long int * x) {
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

void frill(long long int * x) {
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

void fri(int * x) {
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

lli fact[100003], factM[100003];

lli calcInv(long long int y) {
    lli mod, val = 1;
    mod = MOD - 2;
    while(mod > 0) {
        if(mod % 2 == 1)
            val = (val * y) % MOD;
        y = (y * y) % MOD;
        mod /= 2;
    }
    return val;
}

lli calcNCR(lli n, lli r) {
	return (factM[n - r] * fact[n - r]) % MOD;
}

void preFact() {
	lli i;
	fact[0] = 1;
	for(i = 1;i < 100003;i++)
		fact[i] = (i * fact[i - 1]) % MOD;
	for(i = 1;i < 100003;i++)
		fact[i] = calcInv(fact[i]);
}

void preFactM(lli m, lli x) {
	lli i;
	factM[0] = 1;
	for(i = 1;i <= x;i++)
		factM[i] = (((i + m) % MOD) * factM[i - 1]) % MOD;
}

int main() {
    int t;
    lli m, * a, ans, n, x, i;
    si(t);
    preFact();
    while(t--) {
        ans = 0;
        slli(n);
        slli(x);
        slli(m);
        a = givemem(lli, n);
        fl(i, 0, n) {
            slli(a[i]);
            a[i] = a[i] % MOD;
        }
        m--;
        x--;
        preFactM(m, x);
        for(i = 0;i <= x;i++)
            ans = (ans + (calcNCR(m + i, m) * a[x - i]) % MOD) % MOD;
        pi(ans);
        pc('\n');
        free(a);
    }
    return 0;
}