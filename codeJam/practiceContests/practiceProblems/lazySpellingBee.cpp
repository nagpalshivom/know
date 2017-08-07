#include <iostream>
#include <string>
#define MOD 1000000007

using namespace std;

int main() {
  int t, i, len, c;
  long int combinations;
  string word;
  cin >> t;
  for(c = 1;c <= t;c++) {
    cin >> word;
    cout << "Case #" << c << ": ";
    combinations = 1;
    len = word.length();
    if(len == 1)
      cout << "1" << endl;
    else {
      word.insert(0, 1, word.at(0));
      word.append(1, word.at(word.length() - 1));
      for(i = 1;i <= len;i++) {
        if(word.at(i) != word.at(i - 1) && word.at(i) != word.at(i + 1) && word.at(i - 1) != word.at(i + 1))
          combinations = (combinations * 3) % MOD;
        else if(!(word.at(i) == word.at(i - 1) && word.at(i) == word.at(i + 1)))
          combinations = (combinations * 2) % MOD;
      }
      cout << combinations << endl;
    }
  }
  return 0;
}
