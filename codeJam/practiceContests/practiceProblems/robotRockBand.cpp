#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int main() {
  int t, c, n, i, j;
  long int k, id, combinations, xorVal;
  cin >> t;
  for(c = 1;c <= t;c++) {
    cin >> n >> k;
    vector<long int> seatCandidates[4], firstTwoSeatsXor;
    map<long int, int> lastTwoSeatsXorExist;
    for(j = 0;j < 4;j++) {
      for(i = 0;i < n;i++) {
        cin >> id;
        seatCandidates[j].push_back(id);
      }
    }

    cout << "Case #" << c << ": ";
    combinations = 0;

    for(i = 0;i < n;i++) {
      for(j = 0;j < n;j++) {
        firstTwoSeatsXor.push_back(seatCandidates[0][i] ^ seatCandidates[1][j]);
        xorVal = seatCandidates[2][i] ^ seatCandidates[3][j];
        if(lastTwoSeatsXorExist.find(xorVal) == lastTwoSeatsXorExist.end())
          lastTwoSeatsXorExist.insert(pair<long int, int> (xorVal, 0));
        lastTwoSeatsXorExist[xorVal]++;  
      }
    }
    for(vector<long int>::iterator it = firstTwoSeatsXor.begin();it < firstTwoSeatsXor.end();it++) {
      xorVal = *it ^ k;
      if(lastTwoSeatsXorExist.find(xorVal) != lastTwoSeatsXorExist.end())
        combinations += lastTwoSeatsXorExist[xorVal];
    }
    cout << combinations << endl;
  }
  return 0;
}
