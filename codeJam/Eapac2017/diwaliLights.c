#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define li long int
#define lli long long int
#define szf sizeof
#define givemem(t, n) (t*)malloc(sizeof(t) * (n))
#define sstr(x) scanf("%s", x);
#define si(x) scanf("%d", &x)
#define pi(x) printf("%d", x)
#define sli(x) scanf("%ld", &x)
#define pli(x) printf("%ld", x)
#define slli(x) scanf("%l64d", &x)
#define plli(x) printf("%l64d", x)
#define sc(x) scanf("%c", &x)
#define pc(x) printf("%c", x)
#define newl() printf("\n")
#define fl(i, val, n) for(i = (val);i < (n);i++)
#define fln(i, val, n) for(i = (val);i > (n);i--)

lli calculateBulbsTillN(lli strLen, lli n, lli * locB) {
  lli bulbsTillN = (n / strLen) * locB[strLen] + locB[n % strLen];
  return bulbsTillN;
}

int main() {
  lli t, start, end, locB[105], i, strLen, b_loc, noOfBlueBulbs, bCnt, caseNo = 1;
  char s[105];
  slli(t);
  while(t--) {
    b_loc = noOfBlueBulbs = bCnt = 0;
    sstr(s);
    slli(start);
    slli(end);
    strLen = strlen(s);
    locB[0] = 0;
    fl(i, 1, strLen + 1) {
      if(s[i - 1] == 'B')
        locB[i] = ++bCnt;
      else
        locB[i] = bCnt;
    }
    noOfBlueBulbs = (calculateBulbsTillN(strLen, end, locB) - calculateBulbsTillN(strLen, start - 1, locB));
    printf("Case #%llu: %llu\n", caseNo++, noOfBlueBulbs);
  }
  return 0;
}
