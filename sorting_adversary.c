
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N 10
#define NFACT 3628800

typedef int Permutation[N];
Permutation perms[NFACT];
int n, m;

void backtrack(int p[], int k, unsigned mask) {
    int i;

    if(k == n) {
        for(i = 0; i < n; i++)
            perms[m][i] = p[i] + 1;
        m += 1;
    }
    else {
        for(i = 0; i < n; i++)
            if(((mask >> i) & 1) == 0) {
                p[k] = i;
                backtrack(p, k + 1, mask | (1 << i));
            }
    }
}

void printPermutations(void) {
    int i, j;
    for(i = 0; i < m; i++) {
        for(j = 0; j < n; j++)
            printf("%d ", perms[i][j]);
        puts("");
    }
}

void init(void) {
    static int p[N];
    backtrack(p, 0, 0);
}

int inverted(int p[], int x, int y) {
    int i;
    int pos_x, pos_y;

    for(i = 0; i < n; i++)
        if(p[i] == x)
            pos_x = i;
        else if(p[i] == y)
            pos_y = i;

    return (pos_x > pos_y);
}

int cached_x[100];
int cached_y[100];
int cached_answer[100];
int cache_size;

int split(int x, int y) {
    int i, j, k;
    int s, t;

    for(i = 0; i < cache_size; i++)
        if(cached_x[i] == x && cached_y[i] == y)
            return cached_answer[i];
        else if(cached_x[i] == y && cached_y[i] == x)
            return (1 ^ cached_answer[i]);

    i = 0, j = m - 1;
    s = t = 0;

    for(i = 0; i < m; i++)
        if(inverted(perms[i], x, y))
            s += 1;
        else
            t += 1;

    k = 0;
    for(i = 0; i < m; i++)
        if(inverted(perms[i], x, y)) {
            if(s > t) {
                for(j = 0; j < n; j++)
                    perms[k][j] = perms[i][j];
                k += 1;
            }
        }
        else if(s <= t) {
            for(j = 0; j < n; j++)
                perms[k][j] = perms[i][j];
            k += 1;
        }

    m = k;

    cached_x[cache_size] = x;
    cached_y[cache_size] = y;
    cached_answer[cache_size] = ((s > t) ? 0 : 1);
    cache_size += 1;

    if(s > t) return 0;
    return 1;
}

int main(void) {

    puts("Size of array (between 1 and 10) ?");

    scanf("%d", &n);
    assert(n >= 0 && n <= N);

    puts("Ask questions of the form i j."
    " Adversary outputs YES if A[i] < A[j] and NO otherwise."
    " Adversary stops when array is sorted."
    " Try to ask as few questions as possible.");

    init();
    while(m != 1) {
        int x, y;
        scanf("%d %d", &x, &y);
        puts(split(x, y) ? "YES" : "NO");
    }

    printf("You asked %d questions.\n", cache_size);
    {
        int i;
        printf("A[%d]", perms[0][0]);
        for(i = 1; i < n; i++)
            printf(" < A[%d]", perms[0][i]);
    }

    return 0;
}
