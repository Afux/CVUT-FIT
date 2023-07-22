#include <stdio.h>
#include <stdlib.h>

#define MAX 2000
#define MAXSUM 1000000

int PrintFunc(int Pole[MAX], int cnt, int (*SUMS)[MAXSUM]) {
    long long int Counter = 0;
    for (int i = 0; i < cnt - 1; i++) {
        for (int j = i; j < cnt; j++) {
            int sum = 0;
            for (int k = i; k <= j; k++) {
                if (i != j) {
                    sum += Pole[k];
                }
            }
            if (sum != 0) {
                (*SUMS)[Counter] = sum;
                Counter++;
            }

        }
    }
    return Counter;
}

int main() {
    int Pole[MAX];
    int Sums[MAXSUM];
    int g = 0;
    int j = 0;
    int cnt = 0;
    int res=0;
    printf("Posloupnost:\n");
    while (((res = scanf("%d", &g)) == 1) && j < MAX) {
        Pole[j] = g;
        j++;
    }
    if ((res != EOF)||j==0) {
        printf("Nespravny vstup.\n");
        return 1;
    }


    int Count = PrintFunc(Pole, j, &Sums);

    for (int i = 0; i < Count; i++) {

        for (int k = i + 1; k < Count; k++) {
            if (Sums[i] == Sums[k]) {
                cnt++;
            }
        }
    }

    printf("Pocet dvojic: %d\n", cnt);
    return 0;
}
