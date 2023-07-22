#ifndef __PROGTEST__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#endif /* __PROGTEST__ */

int LeapYearCount(int y, int m) {
   long long int year = y;
    if (m < 3) {
        year--;
    }
    year = (year / 4)  - (year / 100) + (year / 400)- (year / 4000);
    return year;
}

int LeapYear(int y1) {
    if (y1 % 4000 == 0)
        return 0;
    if (y1 % 400 == 0)
        return 1;
    if (y1 % 100 == 0)
        return 0;
    if (y1 % 4 == 0)
        return 1;
    return 0;
}

int AddDay(int m, int y) {
    int add = 0;
    if ((m == 1) || (m == 3) || (m == 5) || (m == 7) || (m == 8) || (m == 10) || (m == 12)) {
        add = 31;
    } else if ((m == 4) || (m == 6) || (m == 9) || (m == 11)) {
        add = 30;
    } else if (m == 2) {
        if ((LeapYear(y) == 1))
            add = 29;
        else if ((LeapYear(y) == 0))
            add = 28;
    }
    return add;

}

int DaysInMonthChecker(int y, int m, int d) {
    int flag = 0;
    if ((m == 1) || (m == 3) || (m == 5) || (m == 7) || (m == 8) || (m == 10) || (m == 12)) {
        if (d > 31)
            flag++;
    } else if ((m == 4) || (m == 6) || (m == 9) || (m == 11)) {
        if (d > 30)
            flag++;
    } else if (m == 2) {
        if ((LeapYear(y) == 1) && (d > 29))
            flag++;
        else if ((LeapYear(y) == 0) && (d > 28))
            flag++;
    }
    return flag;
}

int ConditionChecker(int y1, int m1, int d1, int h1, int i1, int y2, int m2, int d2, int h2, int i2) {
    int flag = 0;
    if ((y1 < 1600) || (m1 > 12) || (m2 > 12) || (m1 < 1) || (m2 < 1) || (h1 > 23) || (h2 > 23) || (h1 < 0) ||
        (h2 < 0) || (i1 > 59) || (i2 > 59) || (i1 < 0) || (i2 < 0) || (d1 < 1) || (d2 < 1)) {
        flag = 1;
    }
    if ((DaysInMonthChecker(y1, m1, d1) > 0) || (DaysInMonthChecker(y2, m2, d2)) > 0) {

        flag = 1;
    }
    if (y1 > y2) {
        flag = 1;
    }
    if ((y2 == y1) && (m1 > m2)) {

        flag = 1;
    }
    if ((y2 == y1) && (m2 == m1) && (d1 > d2)) {
        flag = 1;
    }
    if ((y2 == y1) && (m2 == m1) && (d1 == d2)&&(h1>h2)) {
        flag = 1;
    }
    if ((y2 == y1) && (m2 == m1) && (d1 == d2)&&(h1==h2)&&(i1>i2)) {
        flag = 1;
    }
    return flag;
}

void DateAdd(int y, int m, int d, int *year, int *month, int *day, char c) {
    int yearS = 0;
    int monthS = 0;
    int dayS = 0;
    if (c == 'a') {
        if (DaysInMonthChecker(y, m, d + 1) != 0) {
            if ((m + 1) > 12) {
                monthS = 1;
                dayS = 1;
                yearS = (y + 1);
            } else {
                monthS = (m + 1);
                dayS = 1;
                yearS = y;
            }
        }
        else
        {
            yearS = y;
            monthS = m;
            dayS = (d + 1);
        }
    } else if (c == 'r') {
        if (d == 1) {
            if (m == 1) {
                monthS = 12;
                yearS = (y - 1);
                dayS = 31;
            } else {
                monthS = (m - 1);
                yearS = y;
                dayS = AddDay(monthS, yearS);
            }
        } else {
            dayS = (d - 1);
            monthS = m;
            yearS = y;
        }

    }
    *year = yearS;
    *month = monthS;
    *day = dayS;
}
//https://en.wikipedia.org/wiki/Zeller%27s_congruence
int Zeller(int year, int month, int day) {
    int q = day;
    int m = month;
    if (month == 1) {
        m = 13;
        year--;
    } else if (month == 2) {
        m = 14;
        year--;
    }

    int k = year % 100;
    int j = year / 100;
    int h = q + ((13 * (m + 1)) / 5) + k + (k / 4) + (j / 4) + (5 * j);
    h = h % 7;
    return h;
}

int WeekCounter(int y1, int m1, int d1, int y2, int m2, int d2) {
    int minusDay = 0;
    if((Zeller(y1, m1, d1)==1)||Zeller(y1, m1, d1)==0){
        minusDay++;
        return minusDay;
    }

    int cnt=Zeller(y1, m1, d1);

    while (cnt!=Zeller(y2, m2, d2)){

       if(cnt==1){
           minusDay++;
           return minusDay;

       }
        cnt++;
        if(cnt%7==0){
            cnt=0;
            //printf("%d\n",cnt);
        }

    }



    return minusDay;
}

long long int DateDiff(int y1, int m1, int d1, int h1, int i1, int y2, int m2, int d2, int h2, int i2) {
    int year1 = y1;
    int year2 = y2;
    int month1 = m1;
    int month2 = m2;
    int day1 = d1;
    int day2 = d2;

    if((i1!=0)&&(h1!=0)){
        DateAdd(y1, m1, d1, &year1, &month1, &day1, 'a');
    }

    long long int sum1 = year1 * 365 + day1;
    long long int sum2 = year2 * 365 + day2;
    for (int i = 0; i < month1; i++) {
        sum1 += AddDay(i, 2022);
    }
    for (int i = 0; i < month2; i++) {
        sum2 += AddDay(i, 2022);
    }
    sum1 += LeapYearCount(year1, month1);
    sum2 += LeapYearCount(year2, month2);
    long long int dayDiff = sum2 - sum1;
   // printf("Pocet Nedel:%d\n",WeekCounter(year1, month1, day1, year2, month2, day2));
   long long int sundays = ((dayDiff / 7) + WeekCounter(year1, month1, day1, year2, month2, day2));

    dayDiff -= sundays;
    //printf("DAYS:%lld\n",dayDiff);
    //printf("Pocet Nedel:%d\n",sundays);

    return dayDiff;
}

void Boom(int h1, int i1, int h2, int i2, long long int *b1, long long int *b2) {
    int hour1 = h1;
    long long int bum2 = 0;
    long long int bum1 = 0;

    //Dobit do pulnoci
    if (i1 != 0) {
        while ((i1 % 15) != 0) {
            i1++;
        }
        if (i1 == 60) {
            i1 = 0;
            hour1++;
        } else {

            for (int i = i1; i < 60; i += 15) {
                bum2 += (i / 15);

            }
            hour1++;
        }
    }
    if (hour1 != 0) {
        for (int i = hour1; i < 24; i++) {
            bum2 += 10;
            if (i > 12) {
                bum1 += (1 * (i - 12));
            } else {
                bum1 += (1 * i);
            }
        }
    }
    //printf("%lld\n",bum2);

    //Dobit do konce
    for (int i = 0; i <= h2; i++) {
        if (i < h2) {
            bum2 += 10;
        }
        if (i > 12) {
            bum1 += (1 * (i - 12));
        }
        else {
            if (i == 0)
                bum1 += (1 * 12);
            else
                bum1 += (1 * i);
        }
    }
    bum2 += 4;
    if (i2 != 0) {

        for (int i = 15; i <= i2; i += 15) {
            bum2 += (i / 15);
        }
    }
    //printf("%lld\n",bum2);
    *b1 = bum2;
    *b2 = bum1;
}

int bells(int y1, int m1, int d1, int h1, int i1, int y2, int m2, int d2, int h2, int i2, long long int *b1,long long int *b2)
{
    if (ConditionChecker(y1, m1, d1, h1, i1, y2, m2, d2, h2, i2) == 1) {

        return 0;
    }
    int same1=y1+m1+d1;
    int same2=y2+m2+d2;
    //24h odbije zvon..
    int zvon1 = 156;
    int zvon2 = 240;
    int year1 = y1;
    int year2 = y2;
    int month1 = m1;
    int month2 = m2;
    int day1 = d1;
    int day2 = d2;
    int hour1 = h1;
    int hour2 = h2;
    int minute1 = i1;
    int minute2 = i2;

    long long int bum2 = 0;
    long long int bum1 = 0;
    if ((Zeller(y1, m1, d1) == 1) && (Zeller(y2, m2, d2) == 1)&&(same1==same2)) {
        *b1 = 0;
        *b2 = 0;
        return 1;
    }
    if (Zeller(y1, m1, d1) == 1) {
        DateAdd(y1, m1, d1, &year1, &month1, &day1, 'a');
        hour1 = 0;
        minute1 = 0;

    }
    if (Zeller(y2, m2, d2) == 1) {
        DateAdd(y2, m2, d2, &year2, &month2, &day2, 'r');
        hour2 = 23;
        minute2 = 59;
    }

    if ((year1 == year2) && (month1 == month2) && (day1 == day2)) {
        if (hour1 == hour2) {
            if (minute1 != minute2) {

                while(((minute1%15)!=0)){
                    minute1++;
                }
                for (int i = minute1; i < minute2; i += 15) {
                    if (i == 0) {
                        if(hour1>12){
                            bum1=hour1-12;
                        }
                        else{
                            if (hour1 == 0) {
                                bum1 = 12;
                            }
                            else
                                bum1=hour1;
                        }
                        bum2 += 4;
                    } else {
                        bum2 += (i / 15);
                    }

                }
            }
            else {
                if (hour1 == 0) {
                    bum1 = 12;
                } else {
                    if (hour1 > 12) {
                        bum1 = (hour1 - 12);
                    } else {
                        bum1 = hour1;
                    }
                }
                if (minute1 == 0) {
                    bum2 = 4;
                } else {
                    if((bum2%15==0))
                        bum2 = (minute1 / 15);
                    else
                        bum2=0;
                }
            }

        }
        else {

            //Dobit do pulnoci
            if (minute1 != 0) {
                while((minute1%15)!=0){
                    minute1++;
                }
                if(minute1==60){
                    minute1=0;
                    hour1++;
                } else {
                    for (int i = minute1; i < 60; i += 15) {
                        bum2 += (i / 15);
                    }
                    hour1++;
                }
            }

            for (int i = hour1; i <= hour2; i++) {
                if (i < hour2) {
                    bum2 += 10;

                }

                if (i > 12)
                {
                    bum1 += (i - 12);
                }
                else
                    {
                        if (i == 0)
                        {
                            bum1 += 12;
                        }
                        else
                            bum1 += i;
                    }

            }
            bum2 += 4;
            if (minute2 != 0)
            {
                for (int i = 15; i <= minute2; i += 15)
                {
                    bum2 += (i / 15);
                }
            }
        }
    }
    else
    {
        Boom(hour1, minute1, hour2, minute2, &bum2, &bum1);

        long long int days=DateDiff(year1, month1, day1, hour1, minute1, year2, month2, day2, hour2, minute2);
        bum2 += (zvon2 * days);
        bum1 += (zvon1 * days);
    }

    *b1 = bum2;
    *b2 = bum1;
 // printf("B1: %lld B2:%lld\n",bum2,bum1);
    return 1;
}


#ifndef __PROGTEST__

int main(int argc, char *argv[]) {

    long long int b1, b2;

    assert ( bells ( 2022, 10,  1, 13, 15,
                     2022, 10,  1, 18, 45, &b1, &b2 ) == 1
             && b1 == 56
             && b2 == 20 );
    assert ( bells ( 2022, 10,  3, 13, 15,
                     2022, 10,  4, 11, 20, &b1, &b2 ) == 1
             && b1 == 221
             && b2 == 143 );
    assert ( bells ( 2022, 10,  1, 13, 15,
                     2022, 10,  2, 11, 20, &b1, &b2 ) == 1
             && b1 == 106
             && b2 == 65 );
    assert ( bells ( 2022, 10,  2, 13, 15,
                     2022, 10,  3, 11, 20, &b1, &b2 ) == 1
             && b1 == 115
             && b2 == 78 );
    assert ( bells ( 2022, 10,  1, 13, 15,
                     2022, 10,  3, 11, 20, &b1, &b2 ) == 1
             && b1 == 221
             && b2 == 143 );
    assert ( bells ( 2022,  1,  1, 13, 15,
                     2022, 10,  5, 11, 20, &b1, &b2 ) == 1
             && b1 == 56861
             && b2 == 36959 );
    assert ( bells ( 2019,  1,  1, 13, 15,
                     2019, 10,  5, 11, 20, &b1, &b2 ) == 1
             && b1 == 57101
             && b2 == 37115 );
    assert ( bells ( 2024,  1,  1, 13, 15,
                     2024, 10,  5, 11, 20, &b1, &b2 ) == 1
             && b1 == 57341
             && b2 == 37271 );
    assert ( bells ( 1900,  1,  1, 13, 15,
                     1900, 10,  5, 11, 20, &b1, &b2 ) == 1
             && b1 == 57101
             && b2 == 37115 );
    assert ( bells ( 2022, 10,  1,  0,  0,
                     2022, 10,  1, 12,  0, &b1, &b2 ) == 1
             && b1 == 124
             && b2 == 90 );
    assert ( bells ( 2022, 10,  1,  0, 15,
                     2022, 10,  1,  0, 25, &b1, &b2 ) == 1
             && b1 == 1
             && b2 == 0 );
    assert ( bells ( 2022, 10,  1, 12,  0,
                     2022, 10,  1, 12,  0, &b1, &b2 ) == 1
             && b1 == 4
             && b2 == 12 );
    assert ( bells ( 2022, 11,  1, 12,  0,
                     2022, 10,  1, 12,  0, &b1, &b2 ) == 0
             && b1 == 4
             && b2 == 12 );
    assert ( bells ( 2022, 10, 32, 12,  0,
                     2022, 11, 10, 12,  0, &b1, &b2 ) == 0
             && b1 == 4
             && b2 == 12 );
    assert ( bells ( 2100,  2, 29, 12,  0,
                     2100,  2, 29, 12,  0, &b1, &b2 ) == 0
             && b1 == 4
             && b2 == 12 );
    assert ( bells ( 2000,  2, 29, 12,  0,
                     2000,  2, 29, 12,  0, &b1, &b2 ) == 1
             && b1 == 4
             && b2 == 12 );
    assert ( bells ( 2004,  2, 29, 12,  0,
                     2004,  2, 29, 12,  0, &b1, &b2 ) == 1
             && b1 == 0
             && b2 == 0 );

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
