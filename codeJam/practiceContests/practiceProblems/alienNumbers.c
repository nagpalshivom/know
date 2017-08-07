#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define li long int
#define lli long long int
#define szf sizeof
#define givemem(t, n) (t*)malloc(sizeof(t) * (n))
#define sstr(x) scanf("%s", str);
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
int main() {
    int source_base, target_base, n, i;
    char source_lang[105], target_lang[105], val[105];
    int map[300], j, l;
    long long int num, * source_base_pow;
    memset(map, 0, sizeof(map));
    si(n);
    for(i = 1;i <= n;i++) {
        scanf("%s%s%s", val, source_lang, target_lang);
        source_base = strlen(source_lang);
        target_base = strlen(target_lang);
        for(j = 0;j < source_base;j++)
            map[(int)source_lang[j]] = j;
        num = 0;
        l = strlen(val);
        source_base_pow = (long long int *)malloc(sizeof(long long int) * l);
        source_base_pow[0] = 1;
        for(j = 1;j < l;j++)
            source_base_pow[j] = source_base_pow[j - 1] * source_base;
        for(j = l - 1;j > -1;j--)
            num += (map[val[j]]) * source_base_pow[l - j - 1];
        printf("Case #%d: ", i);
        j = 0;
        while(num) {
            source_lang[j++] = (char)target_lang[num % target_base];
            num = num / target_base;
        }
        j--;
        while(j > -1) {
            printf("%c", source_lang[j]);
            j--;
        }
        printf("\n");
        free(source_base_pow);
    }
    return 0;
}
