#include <iostream>
#include <string>
#include<bits/stdc++.h>
using namespace std;

class ABBA {

  public:string canObtain(string initial, string target) {
    string POSSIBLE = "Possible";
    string IMPOSSIBLE = "Impossible";
    int initialLength = initial.length(), 
        targetLength = target.length(), 
        iter,
        i;
    char control;

    if(initialLength > targetLength)
      return IMPOSSIBLE;

    iter = targetLength - initialLength;

    for(i = 0;i < iter;i++) {
      targetLength = target.length();
      control = target.at(targetLength - 1);
      target.erase(targetLength - 1, 1);
      if(control == 'B')
        reverse(target.begin(), target.end());
    }

    if(initial.compare(target) == 0)
      return POSSIBLE;
    else
      return IMPOSSIBLE;

  }
};

int main() {
  ABBA abba;
  cout << abba.canObtain("B", "ABBA") << endl;
}


