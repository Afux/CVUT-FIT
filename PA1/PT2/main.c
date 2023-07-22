#include <stdio.h>
#include <math.h>

int isPrime(int d) {

    int flag = 0;
    for (int i = sqrt(d); i > 1; i--) {
        if (d % i == 0) {
            flag = 1;
            break;
        }
    }
    return flag;
}

int Count(int num) {

    int soucet = 0;
    for (int i = sqrt(num); i > 1; i--) {
        if (num % i == 0) {
            if ((num / i) != i) {
                soucet += (num / i) + i;
                continue;
            }
            soucet += i;
        }
    }
    soucet++;
    return isPrime(soucet);
}

int main() {

    int a, b, c = 0;
    char type = 0;
    int x = 0;

    printf("Intervaly:\n");
    while ((x = scanf("%c %d %d", &type, &a, &b)) == 3) {

        if ((b < a) || (a < 1) || ((type != '?') && (type != '#'))) {
            printf("Nespravny vstup.\n");
            return 1;
        }

        for (int i = a; i <= b; i++) {
            if (isPrime(i) == 0)
                continue;
            if (Count(i) == 0) {
                if (type == '?')
                    printf("%d\n", i);
                c++;
            }
        }

        printf("Celkem: %d\n", c);
        c = 0;
        getc(stdin);

    }

    if (x != EOF) {
        printf("Nespravny vstup.\n");
    }

    return 0;
}
