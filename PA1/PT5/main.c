#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*
 *Comparing two doubles:
 */
int isEqual(double a, double b) {
    return fabs(a - b) <= 1e-12 * (fabs(a) + fabs(b));
}

/*
 * Structure for planes with their coordinates
 */
typedef struct PLANE {
    char name[200];
    double pointX;
    double pointY;
} plane;

/*
 * Getting distance by given coordinates
 * |AB|=√((b1−a1)ˇ2+(b2−a2)ˇ2)
 */
double Distance(double X1, double X2, double Y1, double Y2) {
    return sqrt(((X2 - X1) * (X2 - X1)) + ((Y2 - Y1) * (Y2 - Y1)));
}

//Getting the lowest distance
double LowestDistance(plane *Arr, int cnt) {
    double distance = Distance(Arr[0].pointX, Arr[1].pointX, Arr[0].pointY, Arr[1].pointY);

    for (int i = 0; i < cnt; i++) {

        for (int j = i + 1; j < cnt; j++) {
            double tmp = Distance(Arr[i].pointX, Arr[j].pointX, Arr[i].pointY, Arr[j].pointY);
            if (tmp < distance) {
                distance = tmp;
            }
        }
    }
    return distance;
}

//How many planes has the lowest distance
int HowManyDist(plane *Arr, int cnt, double lowestDistance) {
    int count = 0;
    for (int i = 0; i < cnt; i++) {
        for (int j = i + 1; j < cnt; j++) {
            if (isEqual(lowestDistance, Distance(Arr[i].pointX, Arr[j].pointX, Arr[i].pointY, Arr[j].pointY))) {
                count++;
            }
        }
    }
    return count;
}

/*
 * Printing function
 * Comparing the lowest distance with all items
 * If it found match, printing pair
 */
void PrintFunc(plane *Arr,double minimum,int cnt){
    printf("Vzdalenost nejblizsich letadel: %lf\n", minimum);
    printf("Nalezenych dvojic: %d\n", HowManyDist(Arr, cnt, minimum));
    for (int i = 0; i < cnt; i++) {
        for (int j = i + 1; j < cnt; j++) {

            if (isEqual(minimum,Distance(Arr[i].pointX, Arr[j].pointX, Arr[i].pointY, Arr[j].pointY)))
            {
                printf("%s - %s\n", Arr[i].name, Arr[j].name);
            }
        }
    }
}

int main() {

    double X = 0;
    double Y = 0;
    char Name[200];
    int cnt = 0;
    int res = 0;
    int max = 10;

    //Allocating memory for a single planes arr
    plane *PlaneS = (plane *) malloc(max * sizeof(plane));

    printf("Pozice letadel:\n");

    //Reading input while scanf is returning true
    while ((res = scanf("%lf , %lf : %s", &X, &Y, Name)) == 3) {
        //If current index equals max, reallocating array to max*2.5
        if (cnt >= max) {
            max*=2.5;
            PlaneS = (plane *) realloc(PlaneS, max * sizeof(plane));
        }
        strcpy(PlaneS[cnt].name, Name);
        PlaneS[cnt].pointX = X;
        PlaneS[cnt].pointY = Y;
        cnt++;

    }

    /*
     * If scanf returns "err" and not EOF, or items in Planes array is less than 2, returning err
     * Deallocating array
     */
    if (res != EOF || cnt < 2) {
        printf("Nespravny vstup.\n");
        free(PlaneS);
        return 1;
    }

    double minimumDist = LowestDistance(PlaneS, cnt);
    PrintFunc(PlaneS,minimumDist,cnt);

    //Deallocating arr
    free(PlaneS);
    return 0;
}
