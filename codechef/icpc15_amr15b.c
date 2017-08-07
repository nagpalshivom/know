#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#define li long int
#define lli long long int
#define givemem(t, n) (t*)malloc(sizeof(t) * (n))
#define MOD 1000000007

li preGCD[100001];

lli myMult(lli a, lli mod) {

	lli y = a, val = 1;

	while(mod > 0) {

        if(mod % 2 == 1)
            val = (val * y) % MOD;
        
        y = (y * y) % MOD;
        mod /= 2;
    }

    return val;
}

lli calcAns() {

	li i;
	lli ans = 1;

	for(i = 2;i < 100001;i++)
		if(preGCD[i])
			ans = (ans * (myMult(i, preGCD[i]))) % MOD;

	return ans;
}

int main() {

	int t;
	lli n, * e, i, j, stop, ans, cnt;
	scanf("%d", &t);

	while(t--) {

		ans = 0;
		memset(preGCD, 0, sizeof(preGCD));
		cnt = 1;
		scanf("%lld", &n);
		e = givemem(lli, n);

		for(i = 0;i < n;i++)
			scanf("%lld", &e[i]);

		for(i = 0;i < n;i++) {
			stop = (long int)(sqrt(e[i]) + 0.0000001);
			preGCD[e[i]] = (preGCD[e[i]] * 2 + 1) % MOD;

			for(j = 2;j <= stop;j++) {
				if(e[i] % j == 0) {
					preGCD[j] = (preGCD[j] * 2) % MOD;

					if((e[i] / j) != j)
						preGCD[e[i] / j] = (preGCD[e[i] / j] * 2) % MOD;
				}
			}
		}

		printf("%lld\n", calcAns());
		free(e);
	}

    return 0;
}
