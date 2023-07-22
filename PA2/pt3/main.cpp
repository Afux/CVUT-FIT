#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX

class CRange {
public:

    CRange(const long long &lo, const long long &hi);

    pair<long long, long long> interval;

private:

};

CRange::CRange(const long long &lo, const long long &hi) {
    if (lo > hi)
        throw logic_error(nullptr);
    interval.first = lo;
    interval.second = hi;
}


class CRangeList {
public:
    CRangeList();

    bool includes(const CRange &interval) const;

    bool includes(const long long &inter) const;

    CRangeList &operator+(const CRange &interval);

    CRangeList &operator-(const CRange &interval);

    CRangeList &operator=(const CRangeList &my);

    CRangeList &operator=(const CRange &my);

    CRangeList &operator+=(const CRange &interval);

    CRangeList &operator+=(const CRangeList &interval);

    CRangeList &operator-=(const CRange &interval);

    CRangeList &operator-=(const CRangeList &interval);

    bool operator==(const CRangeList &rhs) const;

    bool operator!=(const CRangeList &rhs) const;


    friend ostream &operator<<(ostream &os, const CRangeList &one);

private:
    vector<CRange> intervaly;
};

bool overlapFull(CRange one, CRange two, long long &start, long long &end) {


    if (one.interval.first >= two.interval.first && one.interval.second <= two.interval.second) {

        return 1;

    } else
        return 0;

}

bool overlap(CRange uno, CRange dos, long long &start, long long &end) {

    CRange one = uno;
    CRange two = dos;
    if (uno.interval.first > dos.interval.first) {
        one = dos;
        two = uno;
    }

    if (two.interval.first != LONG_LONG_MIN) {


        if (one.interval.first <= two.interval.second && one.interval.second >= two.interval.first - 1) {

            if (one.interval.first - 1 >= two.interval.first)
                start = two.interval.first;
            else
                start = one.interval.first;
            if (one.interval.second >= two.interval.second)
                end = one.interval.second;
            else
                end = two.interval.second;
            return 1;


        } else
            return 0;
    } else {
        if (one.interval.first <= two.interval.second && one.interval.second >= two.interval.first) {
            if (one.interval.first != LONG_LONG_MIN) {
                if (one.interval.first - 1 >= two.interval.first)
                    start = two.interval.first;
                else
                    start = one.interval.first;
            } else {
                if (one.interval.first >= two.interval.first)
                    start = two.interval.first;
                else
                    start = one.interval.first;
            }

            if (one.interval.second >= two.interval.second)
                end = one.interval.second;
            else
                end = two.interval.second;
            return 1;


        } else
            return 0;
    }
}

ostream &operator<<(ostream &os, const CRangeList &one) {
    int size = one.intervaly.size();
    if (one.intervaly.empty()) {
        os << "{}";
        return os;
    }
    os << "{";
    for (int i = 0; i < size; ++i) {
        if (one.intervaly[i].interval.first != one.intervaly[i].interval.second) {
            if (i + 1 < size)
                os << "<" << one.intervaly[i].interval.first << ".." << one.intervaly[i].interval.second << ">,";
            else
                os << "<" << one.intervaly[i].interval.first << ".." << one.intervaly[i].interval.second << ">}";
        } else {
            if (i + 1 < size)
                os << one.intervaly[i].interval.first << ",";
            else
                os << one.intervaly[i].interval.first << "}";
        }

    }

    return os;
}

CRangeList::CRangeList() {

}

bool CRangeList::includes(const CRange &interval) const {
    if (intervaly.empty())
        return false;
    int size = intervaly.size() - 1;

    if (interval.interval.first != LONG_LONG_MIN) {
        if (interval.interval.first - 1 > intervaly[size].interval.second) {
            return false;
        }
    } else {
        if (interval.interval.first > intervaly[size].interval.second) {
            return false;
        }
    }


    long long start = 0;
    long long end = 0;
    CRange tmp = interval;
    for (int i = 0; i < size + 1; ++i) {
        bool flag = false;
        flag = overlapFull(tmp, intervaly[i], start, end);
        if (flag)
            return true;
    }
    return false;
}

bool CRangeList::includes(const long long &inter) const {

    CRange interval(inter, inter);
    return this->includes(interval);

}

CRangeList &CRangeList::operator=(const CRange &my) {
    intervaly.clear();
    intervaly.push_back(my);
    return *this;
}

//OK
CRangeList &CRangeList::operator+=(const CRange &interval) {

    if (intervaly.size() == 0) {
        intervaly.push_back(interval);
        return *this;
    }
    int size = intervaly.size() - 1;
    if (size >= 0) {
        if (interval.interval.first != LONG_LONG_MIN) {
            if (interval.interval.first - 1 > intervaly[size].interval.second) {
                intervaly.push_back(interval);
                return *this;
            }
        } else {
            if (interval.interval.first > intervaly[size].interval.second) {
                intervaly.push_back(interval);
                return *this;
            }
        }

    } else {

        return *this;
    }
    long long start = 0;
    long long end = 0;
    CRange tmp = interval;
    bool first = true;

    int holder = -1;
    for (int i = 0; i < size + 1; ++i) {
        bool flag = false;

        flag = overlap(tmp, intervaly[i], start, end);
        if (flag) {
            if (first) {

                intervaly[i].interval.first = start;
                intervaly[i].interval.second = end;
                holder = i;

                first = false;
            } else {

                intervaly.erase(intervaly.begin() + i);
                size--;
                i--;


                intervaly[holder].interval.first = start;
                intervaly[holder].interval.second = end;

            }
            tmp = intervaly[holder];

        } else {

            if (i < (int) intervaly.size() && tmp.interval.second < intervaly[i].interval.first && holder == -1) {
                intervaly.insert(intervaly.begin() + i, tmp);

                break;
            }
        }


    }
    return *this;
}

//OK
CRangeList &CRangeList::operator+=(const CRangeList &interval) {
    int mysize = interval.intervaly.size();
    if (interval.intervaly.empty()) {
        return *this;
    }
    if (intervaly.empty()) {

        for (int j = 0; j < mysize; ++j) {
            intervaly.push_back(interval.intervaly[j]);
        }
        return *this;
    }
    if (interval.intervaly[0].interval.first != LONG_LONG_MIN) {
        if (interval.intervaly[0].interval.first - 1 > intervaly[intervaly.size() - 1].interval.second) {

            for (int j = 0; j < mysize; ++j) {
                intervaly.push_back(interval.intervaly[j]);
            }
            return *this;
        }
    } else {
        if (interval.intervaly[0].interval.first > intervaly[intervaly.size() - 1].interval.second) {

            for (int j = 0; j < mysize; ++j) {
                intervaly.push_back(interval.intervaly[j]);
            }
            return *this;
        }
    }
    for (int j = 0; j < mysize; ++j) {
        mysize = interval.intervaly.size();
        int size = intervaly.size() - 1;
        if (interval.intervaly[j].interval.first != LONG_LONG_MIN) {
            if (interval.intervaly[j].interval.first - 1 > intervaly[size].interval.second) {
                intervaly.push_back(interval.intervaly[j]);
            }
        } else {
            if (interval.intervaly[j].interval.first > intervaly[size].interval.second) {
                intervaly.push_back(interval.intervaly[j]);
            }
        }
        size = intervaly.size() - 1;
        long long start = 0;
        long long end = 0;
        CRange tmp = interval.intervaly[j];
        bool first = true;
        int holder = -1;

        for (int i = 0; i < size + 1; ++i) {

            bool flag = false;

            flag = overlap(tmp, intervaly[i], start, end);
            if (flag) {
                if (first) {

                    intervaly[i].interval.first = start;
                    intervaly[i].interval.second = end;
                    holder = i;

                    first = false;
                } else {
                    intervaly.erase(intervaly.begin() + i);
                    size--;
                    i--;

                    intervaly[holder].interval.first = start;
                    intervaly[holder].interval.second = end;

                }
                tmp = intervaly[holder];

            } else {
                if (i < (int) intervaly.size() && tmp.interval.second < intervaly[i].interval.first && holder == -1) {
                    intervaly.insert(intervaly.begin() + i, tmp);

                    break;
                }
            }
        }
    }

    return *this;

}

CRangeList &CRangeList::operator=(const CRangeList &my) {
    intervaly = my.intervaly;
    return *this;
}

CRangeList &CRangeList::operator+(const CRange &interval) {
    *this += interval;
    return *this;
}

//OK
bool CRangeList::operator==(const CRangeList &rhs) const {
    if (rhs.intervaly.size() != intervaly.size())
        return false;
    int size = intervaly.size();
    for (int i = 0; i < size; ++i) {
        if (intervaly[i].interval.first != rhs.intervaly[i].interval.first ||
            intervaly[i].interval.second != rhs.intervaly[i].interval.second)
            return false;
    }
    return true;
}

//Ok
bool CRangeList::operator!=(const CRangeList &rhs) const {
    if (rhs.intervaly.size() != intervaly.size())
        return true;
    int size = intervaly.size();
    for (int i = 0; i < size; ++i) {
        if (intervaly[i].interval.first != rhs.intervaly[i].interval.first ||
            intervaly[i].interval.second != rhs.intervaly[i].interval.second)
            return true;
    }
    return false;
}

//Skoro OK osetrit nulu
CRangeList &CRangeList::operator-=(const CRange &interval) {
    if (this->intervaly.size() == 1 && this->intervaly[0].interval.first == interval.interval.first
        && this->intervaly[0].interval.second == interval.interval.second) {

        this->intervaly.clear();
        return *this;
    }
    int size = intervaly.size() - 1;
    if (size >= 0) {
        if (interval.interval.first != LONG_LONG_MIN) {
            if (this->intervaly.empty() || interval.interval.first - 1 > intervaly[size].interval.second) {
                return *this;
            }
        } else {
            if (this->intervaly.empty() || interval.interval.first > intervaly[size].interval.second) {
                return *this;
            }
        }
    } else
        return *this;
    long long start = 0;
    long long end = 0;
    CRange tmp = interval;

    for (int i = 0; i < size + 1; ++i) {
        bool flag = false;
        size = intervaly.size() - 1;

        flag = overlap(tmp, intervaly[i], start, end);
        if (flag) {
            end = intervaly[i].interval.second;
            if(interval.interval.second!=LONG_LONG_MAX)
                start = interval.interval.second + 1;
            else
                start = interval.interval.second;


            if (interval.interval.first != LONG_LONG_MIN) {
                if (interval.interval.first - 1 > intervaly[i].interval.first)
                    intervaly[i].interval.second = interval.interval.first - 1;
                else {
                    intervaly.erase(intervaly.begin() + i);
                    //size--;
                    i--;
                }
            } else {
                if (interval.interval.first > intervaly[i].interval.first)
                    intervaly[i].interval.second = interval.interval.first;
                else {
                    intervaly.erase(intervaly.begin() + i);
                    //size--;
                    i--;
                }
            }
            try {
                intervaly.insert(intervaly.begin() + i + 1, CRange(start, end));


            }
            catch (const std::logic_error &e) {

                tmp = interval;
                continue;
            }
            break;
        }
        size = intervaly.size() - 1;

    }
    return *this;
}

//Skoro OK osetrit nulu
CRangeList &CRangeList::operator-=(const CRangeList &interval) {
    if (*this == interval) {

        this->intervaly.clear();
        return *this;
    }

    if (interval.intervaly.empty() || this->intervaly.empty() ||
        interval.intervaly[0].interval.first - 1 > intervaly[intervaly.size() - 1].interval.second) {
        return *this;
    }
    for (int j = 0; j < (int) interval.intervaly.size(); ++j) {
        int size = intervaly.size() - 1;
        if (interval.intervaly[j].interval.first - 1 > intervaly[size].interval.second) {
            return *this;
        }
        long long start = 0;
        long long end = 0;
        CRange tmp = interval.intervaly[j];

        for (int i = 0; i < size + 1; ++i) {
            bool flag = false;
            size = intervaly.size() - 1;

            flag = overlap(tmp, intervaly[i], start, end);
            if (flag) {
                end = intervaly[i].interval.second;
                if(interval.intervaly[j].interval.second!=LONG_LONG_MAX)
                    start = interval.intervaly[j].interval.second + 1;
                else
                    start = interval.intervaly[j].interval.second;

                if (interval.intervaly[j].interval.first != LONG_LONG_MIN) {
                    if (interval.intervaly[j].interval.first - 1 > intervaly[i].interval.first)
                        intervaly[i].interval.second = interval.intervaly[j].interval.first - 1;
                    else {
                        intervaly.erase(intervaly.begin() + i);

                        i--;
                    }
                } else {
                    if (interval.intervaly[j].interval.first > intervaly[i].interval.first)
                        intervaly[i].interval.second = interval.intervaly[j].interval.first;
                    else {
                        intervaly.erase(intervaly.begin() + i);

                        i--;
                    }
                }


                try {
                    intervaly.insert(intervaly.begin() + i + 1, CRange(start, end));
                }
                catch (const std::logic_error &e) {
                    tmp = interval.intervaly[j];
                    continue;
                }
                break;
            }
            size = intervaly.size() - 1;
        }
    }

    return *this;
}

CRangeList &CRangeList::operator-(const CRange &interval) {
    *this -= interval;
    return *this;
}

CRangeList operator+(const CRange &my, const CRange &interval) {
    CRangeList temp;
    temp += my;
    temp += interval;
    return temp;
}

CRangeList operator-(const CRange &my, const CRange &interval) {
    CRangeList temp;
    temp = my;
    temp -= interval;
    return temp;
}


#ifndef __PROGTEST__

string toString(const CRangeList &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main(void) {


    CRangeList a, b;

    assert (sizeof(CRange) <= 2 * sizeof(long long));
    a = CRange(5, 10);
    a += CRange(25, 100);
    assert (toString(a) == "{<5..10>,<25..100>}");
    a += CRange(-5, 0);
    a += CRange(8, 50);
    assert (toString(a) == "{<-5..0>,<5..100>}");
    a += CRange(101, 105) + CRange(120, 150) + CRange(160, 180) + CRange(190, 210);
    assert (toString(a) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}");
    a += CRange(106, 119) + CRange(152, 158);
    assert (toString(a) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}");
    a += CRange(-3, 170);
    a += CRange(-30, 1000);
    assert (toString(a) == "{<-30..1000>}");
    b = CRange(-500, -300) + CRange(2000, 3000) + CRange(700, 1001);
    a += b;
    assert (toString(a) == "{<-500..-300>,<-30..1001>,<2000..3000>}");
    a -= CRange(-400, -400);
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}");
    a -= CRange(10, 20) + CRange(900, 2500) + CRange(30, 40) + CRange(10000, 20000);
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    try {
        a += CRange(15, 18) + CRange(10, 0) + CRange(35, 38);
        assert ("Exception not thrown" == nullptr);
    }
    catch (const std::logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown" == nullptr);
    }
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    b = a;
    assert (a == b);
    assert (!(a != b));
    b += CRange(2600, 2700);
    assert (toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    assert (a == b);
    assert (!(a != b));
    b += CRange(15, 15);
    assert (toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}");
    assert (!(a == b));
    assert (a != b);
    assert (b.includes(15));
    assert (b.includes(2900));
    assert (b.includes(CRange(15, 15)));
    assert (b.includes(CRange(-350, -350)));
    assert (b.includes(CRange(100, 200)));
    assert (!b.includes(CRange(800, 900)));
    assert (!b.includes(CRange(-1000, -450)));
    assert (!b.includes(CRange(0, 500)));
    a += CRange(-10000, 10000) + CRange(10000000, 1000000000);
    assert (toString(a) == "{<-10000..10000>,<10000000..1000000000>}");
    b += a;
    assert (toString(b) == "{<-10000..10000>,<10000000..1000000000>}");
    b -= a;
    assert (toString(b) == "{}");
    b += CRange(0, 100) + CRange(200, 300) - CRange(150, 250) + CRange(160, 180) - CRange(170, 170);
    assert (toString(b) == "{<0..100>,<160..169>,<171..180>,<251..300>}");
    b -= CRange(10, 90) - CRange(20, 30) - CRange(40, 50) - CRange(60, 90) + CRange(70, 80);
    assert (toString(b) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}");
#ifdef EXTENDED_SYNTAX
    CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
  assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
  ostringstream oss;
  oss << setfill ( '=' ) << hex << left;
  for ( const auto & v : x + CRange ( -100, -100 ) )
    oss << v << endl;
  oss << setw ( 10 ) << 1024;
  assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
#endif /* EXTENDED_SYNTAX */
    return EXIT_SUCCESS;

}

#endif /* __PROGTEST__ */