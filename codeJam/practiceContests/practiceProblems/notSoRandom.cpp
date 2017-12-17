#include <iostream>
#include <cstdlib>

using namespace std;

int getRandomMachine(int a, int b, int c) {
    int randNo = rand() % 100;
    if(randNo < a)
        return a;
    else if(randNo >= a && randNo < b)
        return b;
    else
        return c;
}

int main() {
    int t, a, b, c, caseNo, machine, kbit, xbit, j;
    long int x, n, k, i;
    double p0, p1, res;
    cin >> t;
    for(caseNo = 1;caseNo <= t;caseNo++) {
        cout << "Case #" << caseNo << ": ";
        cin >> n >> x >> k >> a >> b >> c;

        res = 0.0;

        for(i = 0;i < 32;i++) {
            kbit = k & (1l << i);
            xbit = x & (1l << i);
            p0 = p1 = 1.0;
            for(j = 1;j < n;j++) {
                if(xbit == 1 && kbit == 1)
                    p = p * ((a + b) / 100.0);
                else if(xbit != kbit)
                    p = p * ((b + c) / 100.0);
            }
            res += ((1l << i) * p);
        }
        cout << res;

    }
    return 0;
}
