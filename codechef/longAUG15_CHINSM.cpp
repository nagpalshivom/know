//due to weak test cases O(n^2) solution gives me 90/100 points
#include <iostream>
#include <algorithm>
#include <vector>
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
lli findMax(lli * a, lli n) {
    lli i, max = a[1];
    fl(i, 1, n + 1) {
        if(max < a[i])
            max = a[i];
    }
    return max;
}
lli findInd(std::vector<lli> & indexes, lli i, lli s, lli e) {
    lli j;
    if(s == e) {
        if(indexes[s] < i)
            return indexes[s];
        else
            return 0;
    }
    fl(j, s, e) {
        if((indexes[j] < i) && (indexes[j + 1] >= i))
            return indexes[j];
    }
    if(indexes[e] < i)
        return indexes[e];
    else
        return 0;
}
lli nsum(lli n) {
    return (n * (n + 1)) / 2;
}
int main() {
    lli n, k, * a, i, max, y, s, e, j, ans, * start, ind;
    std::vector<lli> m[100001];
    slli(n);
    slli(k);
    start = givemem(lli, n + 1);
    memset(start, 0, sizeof(lli) * (n + 1));
    a = givemem(lli, n + 1);
    fl(i, 1, n + 1) {
        slli(a[i]);
        m[a[i]].push_back(i);
    }
    fl(i, 1, 100001)
        std::sort(m[i].begin(), m[i].end());
    max = findMax(a, n);
    fl(i, 2, n + 1) {
        if(k >= a[i])
            continue;
        y = k;
        while(y <= max) {
            if(m[y].size() != 0) {
                ind = findInd(m[y], i, 0, m[y].size() - 1);
                if(ind > start[i])
                    start[i] = ind;
            }
            y += a[i];
        }
    }
    k = n;
    ans = 0;
    fln(i, n, 0) {
        if(start[i]) {
            e = i;
            s = start[i];
            fln(j, e - 1, s) {
                if(start[j] > s) {
                    s = start[j];
                    e = j;
                }
            }
            if(k >= e)
                ans += (nsum(k) - nsum(e - 1) - s * (k - e + 1));
            i = e;
            k = e - 1;
        }
    }
    ans += (nsum(k));
    pi(ans);
    free(start);
    free(a);
    return 0;
}
