#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
double randfrom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}
char *randstring(size_t length) {

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char *randomString = NULL;

    if (length) {
        randomString = (char*)malloc(sizeof(char) * (length +1));

        if (randomString) {
            for (int n = 0;n < length;n++) {
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }

    return randomString;
}
int main() {
    for (int i = 0; i < 1000; ++i) {
        printf("%lf,%lf: %s\n", randfrom(-100.0,4350.0),randfrom(-1.0,220.0), randstring(20));
    }
    for (int i = 0; i < 200; ++i) {
        printf("%lf,%lf: %s\n", randfrom(-100.0,4350.0),randfrom(-1.323,33.22), randstring(199));
    }
    for (int i = 0; i < 200; ++i) {
        printf("%d,%lf: %s\n", rand()%100,randfrom(-1.323,33.22), randstring(112));
    }


    return 0;
}
