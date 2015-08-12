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
typedef struct nodeCreate {
    li count, cumCount, ind, elem;
}node;
void mergePart(li * e, li * v, long int l1, long int h1, long int l2, long int h2) {
    long int size = h2 - l1 + 1, i = 0, start = l1;
    li * tempV = (li *)malloc(sizeof(li) * size);
    while(l1 <= h1 && l2 <= h2) {
        if(e[v[l1]] < e[v[l2]])
            tempV[i++] = v[l1++];
        else
            tempV[i++] = v[l2++];
    }
    while(l1 <= h1)
        tempV[i++] = v[l1++];
    while(l2 <= h2)
        tempV[i++] = v[l2++];
    memcpy(&v[start], tempV, sizeof(li) * size);
    free(tempV);
}
void mergeSort(li * e, li * v, long int l, long int h){
    if(l == h)
        return;
    mergeSort(e, v, l, (l + h) / 2);
    mergeSort(e, v, (l + h) / 2 + 1, h);
    mergePart(e, v, l, (l + h) / 2, (l + h) / 2 + 1, h);
}
typedef struct treeNode {
    struct treeNode * left, * right;
    li data;
}tree;
li fillCount(tree ** root, li * e, li ind, li l, li r) {
    if((*root) == NULL) {
        (*root) = (tree *)malloc(sizeof(tree));
        (*root)->left = (*root)->right = NULL;
        (*root)->data = ind;
        return (1 + (r - ind) + (ind - l) + (r - ind) * (ind - l));
    }
    else {
        if(ind > (*root)->data)
            return fillCount(&(*root)->right, e, ind, (*root)->data + 1, r);
        else
            return fillCount(&(*root)->left, e, ind, l, (*root)->data - 1);
    }
}
li getEqCount(node * v, li k, li l, li r) {
    li mid = (r + l) / 2;
    if(l > r)
        return 0;
    else if(v[mid].elem == k)
        return v[mid].count;
    else if(v[mid].elem < k)
        return getEqCount(v, k, l, mid - 1);
    else
        return getEqCount(v, k, mid + 1, r);
}
li getCompCount(node * v, li k, li l, li r, li n, int comp) {
    li mid = (l + r) / 2;
    if(l > r)
        return 0;
    else if(v[mid].elem == k) {
        if(comp == 1)
            return v[mid].cumCount;
        else if(comp == 0 && (mid + 1 <= n))
            return v[mid + 1].cumCount;
        else
            return 0;
    }
    else if(v[mid].elem > k) {
        if(comp) {
            if((mid + 1) <= n) {
                if(v[mid + 1].elem <= k)
                    return v[mid + 1].cumCount;
                else
                    return getCompCount(v, k, mid + 1, r, n, comp);
            }
            else
                return 0;
        }
        else
            return getCompCount(v, k, mid + 1, r, n, comp);
    }
    else {
        if(comp) {
            if((mid - 1) >= 0) {
                if(v[mid - 1].elem > k)
                    return v[mid].cumCount;
                else
                    return getCompCount(v, k, l, mid - 1, n, comp);
            }
            else
                return v[mid].cumCount;
        }
        else {
            if((mid - 1) >= 0) {
                if(v[mid - 1].elem >= k)
                    return v[mid].cumCount;
                else
                    return getCompCount(v, k, l, mid - 1, n, comp);
            }
            else
                return v[mid].cumCount;
        }
    }
}
void freeTree(tree * root) {
    if(root->left != NULL)
        freeTree(root->left);
    if(root->right != NULL)
        freeTree(root->right);
    free(root);
}
int main() {
    li n, m, * e, i, k, elemCount, * sortedIndexes, j;
    node * ind;
    tree * root = NULL;
    char comparator, person;
    sli(n);
    sli(m);
    e = givemem(li, n);
    ind = givemem(node, n);
    sortedIndexes = givemem(li, n);
    fl(i, 0, n) {
        sli(e[i]);
        sortedIndexes[i] = i;
    }
    mergeSort(e, sortedIndexes, 0, n - 1);
    j = 0;
    fln(i, n - 1, -1) {
        elemCount = fillCount(&root, e, sortedIndexes[i], 0, n - 1);
        if(j == 0) {
            ind[j].ind = sortedIndexes[i];
            ind[j].count = elemCount;
            ind[j].elem = e[sortedIndexes[i]];
            j++;
        }
        else {
            if(e[sortedIndexes[i]] == e[ind[j - 1].ind])
                ind[j - 1].count += elemCount;
            else {
                ind[j].ind = sortedIndexes[i];
                ind[j].count = elemCount;
                ind[j].elem = e[sortedIndexes[i]];
                j++;
            }
        }
    }
    ind[j - 1].cumCount = ind[j - 1].count;
    fln(i, j - 2, -1)
        ind[i].cumCount = ind[i + 1].cumCount + ind[i].count;
    fl(i, 0, m) {
        comparator = gc();
        sli(k);
        person = gc();
        gc();
        if(comparator == '=')
            elemCount = getEqCount(ind, k, 0, j - 1);
        else if(comparator == '<')
            elemCount = getCompCount(ind, k, 0, j - 1, j - 1, 0);
        else {
            elemCount = getCompCount(ind, k, 0, j - 1, j - 1, 1);
            elemCount = ind[0].cumCount - elemCount;
        }
        if(elemCount % 2 == 0)
            person = (person == 'D' ? 'C' : 'D');
        pc(person);
    }
    free(e);
    free(ind);
    free(sortedIndexes);
    freeTree(root);
    return 0;
}
