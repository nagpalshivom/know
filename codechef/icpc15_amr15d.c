#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
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
void mergePart(long int * v, long int l1, long int h1, long int l2, long int h2) {
    long int size = h2 - l1 + 1, i = 0, start = l1;
    long int * tempV = (long int *)malloc(sizeof(long int) * size);
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
    memcpy(&v[start], tempV, sizeof(long int) * size);
    free(tempV);
}
void mergeSort(long int * v, long int l, long int h){
    if(l == h)
        return;
    mergeSort(v, l, (l + h) / 2);
    mergeSort(v, (l + h) / 2 + 1, h);
    mergePart(v, l, (l + h) / 2, (l + h) / 2 + 1, h);
}
int main() {
	long int n, * g, i, q, k, v;
	long long int * preSum;
	scanf("%ld", &n);
	g = givemem(li, n);
	preSum = givemem(lli, n);
	for(i = 0;i < n;i++)
		scanf("%ld", &g[i]);
	mergeSort(g, 0, n - 1);
	preSum[0] = g[0];
	for(i = 1;i < n;i++)
		preSum[i] = g[i] + preSum[i - 1];
	scanf("%ld", &q);
	for(i = 0;i < q;i++) {
		scanf("%ld", &k);
		if(k)
			v = (int)ceil((double)n / (double)(k + 1));
		else
			v = n;
		printf("%lld\n", preSum[v - 1]);
	}
	free(g);
	free(preSum);
    return 0;
}
