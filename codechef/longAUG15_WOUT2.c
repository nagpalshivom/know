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
void update_bit(lli * bit, lli r, lli val, lli n) {
    for(;r <= n;r += (r & (-r)))
        bit[r] += (val);
}
void range_update_bit(lli * bit1, lli * bit2, lli l, lli r, lli n) {
    update_bit(bit1, l, 1, n);
    update_bit(bit1, r + 1, -1, n);
    update_bit(bit2, l, l - 1, n);
    update_bit(bit2, r + 1, -r, n);
}
lli query_bit(lli * bit, lli r) {
    lli sum = 0;
    for(;r > 0;r -= (r & (-r)))
        sum += bit[r];
    return sum;
}
lli query_bits(lli * bit1, lli * bit2, lli r) {
    return ((long long int)query_bit(bit1, r) * r - query_bit(bit2, r));
}
int main() {
    int t;
    lli * bit1, * bit2, n, * il, * ih, i, h, maxSpaces, emptySpaces;
    si(t);
    while(t--) {
        maxSpaces = emptySpaces = 0;
        slli(n);
        slli(h);
        il = givemem(lli, n);
        ih = givemem(lli, n);
        bit1 = (lli *)calloc(n + 5, sizeof(lli));
        bit2 = (lli *)calloc(n + 5, sizeof(lli));
        fl(i, 0, n) {
            slli(il[i]);
            slli(ih[i]);
        }
        fl(i, 0, n)
            range_update_bit(bit1, bit2, il[i] + 1, ih[i] + 1, n);
        fl(i, 1, (n - h + 2)) {
            emptySpaces = query_bits(bit1, bit2, i + h - 1) - query_bits(bit1, bit2, i - 1);
            if(maxSpaces < emptySpaces)
                maxSpaces = emptySpaces;
        }
        pi((long long int)((long long int)(n * h) - maxSpaces));
        pc('\n');
        free(bit1);
        free(bit2);
        free(il);
        free(ih);
    }
    return 0;
}
