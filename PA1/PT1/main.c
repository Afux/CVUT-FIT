#include <stdio.h>

double rounding(double s) {

    double nevim = s * 100;
    int also = nevim;
    double nove = also / 100.0;
    return nove;
}

int main() {

    double kredit_urok, debet_urok = 0.0;
    int day = 0;
    int amount = 0;
    double final_amount = 0;
    int x = 100;
    int firstDate=0;
    char c = '%';


    printf("Zadejte kreditni urok [%c]:\n", c);
    if (scanf("%le", &kredit_urok) != 1) {
        printf("Nespravny vstup.\n");
        return 0;
    }

    printf("Zadejte debetni urok [%c]:\n", c);
    if (scanf("%le", &debet_urok) != 1) {
        printf("Nespravny vstup.\n");
        return 0;
    }


    printf("Zadejte transakce:\n");
    kredit_urok /= 100;
    debet_urok /= 100;
    while (x) {
        int last_day = day;

        if (scanf("%d , %d", &day, &amount) != 2) {
            printf("Nespravny vstup.\n");
            return 0;
        }
        if (last_day >= day&&firstDate!=0) {
            printf("Nespravny vstup.\n");
            return 0;
        }

        for (int i = 0; i < (day - last_day); i++) {
            if (final_amount > 0) {
                double tmp = final_amount * kredit_urok;
                final_amount += rounding(tmp);
            }

            if (final_amount < 0) {
                double tmp = final_amount * debet_urok;
                final_amount += rounding(tmp);
            }
        }

        x = amount;
        final_amount += amount;
        firstDate=1;
    }
    printf("Zustatek: %.2f\n", final_amount);

    return 0;
}
