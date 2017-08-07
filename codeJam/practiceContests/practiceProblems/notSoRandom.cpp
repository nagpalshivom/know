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
  int t, a, b, c, caseNo, machine;
  long int x, n, k, i;
  cin >> t;
  for(caseNo = 1;caseNo <= t;caseNo++) {
    cout << "Case #" << caseNo << ": ";
    cin >> n >> x >> k >> a >> b >> c;
    for(i = 0;i < n;i++) {
      machine = getRandomMachine(a, b, c);
      if(machine == a)
        x = x & k;
      else if(machine == b)
        x = x | k;
      else
        x = x ^ k;
    }
    cout << x << endl;
  }
  return 0;
}
