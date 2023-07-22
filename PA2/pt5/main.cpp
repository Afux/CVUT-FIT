#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>

using namespace std;

class CDate {
public:
    //---------------------------------------------------------------------------------------------
    CDate(int y, int m, int d) : m_Year(y), m_Month(m), m_Day(d) {}

    //---------------------------------------------------------------------------------------------
    int compare(const CDate &x) const {
        if (m_Year != x.m_Year)
            return m_Year - x.m_Year;
        if (m_Month != x.m_Month)
            return m_Month - x.m_Month;
        return m_Day - x.m_Day;
    }

    //---------------------------------------------------------------------------------------------
    int year(void) const {
        return m_Year;
    }

    //---------------------------------------------------------------------------------------------
    int month(void) const {
        return m_Month;
    }

    //---------------------------------------------------------------------------------------------
    int day(void) const {
        return m_Day;
    }

    //---------------------------------------------------------------------------------------------
    friend ostream &operator<<(ostream &os, const CDate &x) {
        char oldFill = os.fill();
        return os << setfill('0') << setw(4) << x.m_Year << "-" << setw(2) << static_cast<int> ( x.m_Month )
                  << "-" << setw(2) << static_cast<int> ( x.m_Day ) << setfill(oldFill);
    }
    //---------------------------------------------------------------------------------------------
private:
    int16_t m_Year;
    int8_t m_Month;
    int8_t m_Day;
};

#endif /* __PROGTEST__ */

/*
 *Func. that compares two doubles
 */
int isEqual(double a, double b) {
    return abs(a - b) <= 1e-12 * (abs(a) + abs(b));
}

/*
 * Func. that transforms canonic name to normalized
 */
string normalizeName(const string &origName) {
    string tmp;
    int end = origName.length();
    int start = origName.length() - 1;
    /*
     * setting start and end of a string without spaces in the beginning or in the end of a string
     */
    while (origName[start] == ' ') {
        start--;
        end--;
    }
    start = 0;
    while (origName[start] == ' ') {
        start++;
    }

    for (int i = start; i < end;) {

        if (origName[i] == ' ') {

            if (i == 0 || i == end - 1) {
                i++;
                continue;
            }
            while (origName[i + 1] == ' ')
                i++;
        }
        tmp += tolower(origName[i++]);
    }

    return tmp;
}

/*
 * Func. that transforms canonic name to normalized (for sort)
 */
string normalizeNameSort(const string &origName) {
    string tmp = origName;
    transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
    return tmp;
}

/*
 * Class CInvoice represents one Invoice. Variable m_pos represents order when invoice was added and variable m_key represents unique key of one invoice
 */
class CInvoice {
public:
    CInvoice(const CDate &date, const string &seller, const string &buyer, unsigned int amount, double vat);

    CDate date(void) const;

    string buyer(void) const;

    string seller(void) const;

    unsigned int amount(void) const;

    double vat(void) const;

    mutable bool isSent;
    mutable bool isRecieved;
    mutable int m_pos;

    mutable string m_buyer;
    mutable string m_seller;
    string m_key;

    bool operator<(const CInvoice &inv) const;

private:
    CDate m_date;
    unsigned int m_amount;
    double m_vat;

};

/*
 * Constructor of class CInvoice
 * sets all variables and key for std::set compare operator
 */
CInvoice::CInvoice
        (const CDate &date, const string &seller, const string &buyer, unsigned int amount, double vat)
        : m_buyer(buyer), m_seller(seller), m_date(date), m_amount(amount), m_vat(vat) {
    m_key = to_string(m_vat) + to_string(m_amount) + to_string(m_date.month()) +
            to_string(m_date.day()) + to_string(m_date.year()) + normalizeName(m_buyer) + normalizeName(m_seller);
    isSent = false;
    isRecieved = false;
}

/*
 * Getters for class variables
 */
string CInvoice::buyer(void) const {
    return m_buyer;
}

string CInvoice::seller(void) const {
    return m_seller;
}

unsigned int CInvoice::amount(void) const {
    return m_amount;
}

double CInvoice::vat(void) const {
    return m_vat;
}

CDate CInvoice::date(void) const {
    return m_date;
}

/*
 * "<" operator for std::set, compares by key;
 */
bool CInvoice::operator<(const CInvoice &inv) const {
    string rightStr = inv.m_key;
    string leftStr = this->m_key;
    return (leftStr < rightStr);
}

/*
 * Class CSortOpt represents sort criteria(keys)
 */
class CSortOpt {
public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;

    CSortOpt(void);

    CSortOpt &addKey(int key, bool ascending = true);

    vector<pair<int, bool>> Keys;
private:

};

CSortOpt::CSortOpt(void) {}

/*
 * Func. that adds sort key into vector of keys
 */
CSortOpt &CSortOpt::addKey(int key, bool ascending) {
    Keys.emplace_back(key, ascending);
    return *this;
}

/*
 * Class CVATRegister represents register of all added companies and their invoices
 */
class CVATRegister {
public:
    CVATRegister(void);

    bool registerCompany(const string &name);

    bool addIssued(const CInvoice &x);

    bool addAccepted(const CInvoice &x);

    bool delIssued(const CInvoice &x);

    bool delAccepted(const CInvoice &x);

    list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) const;

    int m_pos;
private:

    map<string, set<CInvoice>> AllCompanies;//map of all companies and their invoices
    map<string, string> normalizedNames; //map of all normalized names of companies and their original name
};

/*
 * Func. that transform name to normalized name and then checks if company was already registered
 * if company was already registered, returns false
 * else, adds company to "database" of all companies and saves normalized name
 */
bool CVATRegister::registerCompany(const string &name) {
    string tmp = normalizeName(name);

    if (normalizedNames.count(tmp)) {
        return false;
    } else {
        normalizedNames[tmp] = name;
        AllCompanies[tmp];
    }

    return true;
}

/*
 * Func. that adds issued invoice into company "database" of invoices
 */
bool CVATRegister::addIssued(const CInvoice &x) {

    string normSeller = normalizeName(x.seller());
    string normBuyer = normalizeName(x.buyer());
    /*
     * If seller and buyer is same company or one of them is not registered, returns false
     */
    if (normSeller == normBuyer || !normalizedNames.count(normSeller) || !normalizedNames.count(normBuyer)) {
        return false;
    }
    /*
     * If invoice is registered and was already Issued, returns false
     * Else adds invoice with canonic names to Seller and Receiver invoices "database" and mark invoice as sent
     */
    if (AllCompanies.at(normSeller).count(x) &&
        AllCompanies.at(normSeller).find(x)->isSent)
        return false;
    else {
        AllCompanies.at(normSeller).insert(x);
        auto seller = AllCompanies.at(normSeller).find(x);

        seller->m_seller = normalizedNames.at(normSeller);
        seller->m_buyer = normalizedNames.at(normBuyer);
        seller->isSent = true;
        seller->m_pos = m_pos++;

        AllCompanies.at(normBuyer).insert(*seller);
        auto buyer = AllCompanies.at(normBuyer).find(x);
        buyer->isSent = true;

    }


    return true;
}

/*
 * Func. acts similar as addIssued but marks invoice as received
 */
bool CVATRegister::addAccepted(const CInvoice &x) {
    string normSeller = normalizeName(x.seller());
    string normBuyer = normalizeName(x.buyer());

    if (normBuyer == normSeller || !normalizedNames.count(normSeller) || !normalizedNames.count(normBuyer)) {
        return false;
    }

    if (AllCompanies.at(normBuyer).count(x) &&
        AllCompanies.at(normBuyer).find(x)->isRecieved)
        return false;

    else {
        AllCompanies.at(normBuyer).insert(x);
        auto buyer = AllCompanies.at(normBuyer).find(x);

        buyer->m_seller = normalizedNames.at(normSeller);
        buyer->m_buyer = normalizedNames.at(normBuyer);
        buyer->isRecieved = true;
        buyer->m_pos = m_pos++;

        AllCompanies.at(normSeller).insert(*buyer);
        auto seller = AllCompanies.at(normSeller).find(x);
        seller->isRecieved = true;

    }


    return true;
}

/*
 * Func. removes issued invoice from buyer and seller "database" of invoices
 */
bool CVATRegister::delIssued(const CInvoice &x) {
    string normSeller = normalizeName(x.seller());
    string normBuyer = normalizeName(x.buyer());
    /*
    * If seller and buyer is same company or one of them is not registered, returns false
    */
    if (normSeller == normBuyer || !normalizedNames.count(normSeller) || !normalizedNames.count(normBuyer)) {
        return false;
    }
    /*
     * If seller company doesn't have this invoice in "database", returns false
     */
    if (!AllCompanies.at(normSeller).count(x))
        return false;

    auto buyer = AllCompanies.at(normBuyer).find(x);
    auto seller = AllCompanies.at(normSeller).find(x);
    /*
     * If seller company didn't issue this invoice, returns false
     */
    if (!seller->isSent)
        return false;
    /*
     * If seller company issued this invoice but buyer didn't receive it, removes invoice from "database"
     * Else marks invoice as not sent
     */
    if (seller->isSent &&
        !buyer->isRecieved) {
        buyer->isSent = false;
        AllCompanies.at(normSeller).erase(x);
        AllCompanies.at(normBuyer).erase(x);
    } else {
        buyer->isSent = false;
        seller->isSent = false;

    }
    return true;


}

/*
 * Func. acts similar as delIssued but removes accepted invoice
 */
bool CVATRegister::delAccepted(const CInvoice &x) {
    string normSeller = normalizeName(x.seller());
    string normBuyer = normalizeName(x.buyer());
    if (normSeller == normBuyer || !normalizedNames.count(normSeller) || !normalizedNames.count(normBuyer)) {
        return false;
    }

    if (!AllCompanies.at(normSeller).count(x))
        return false;
    auto buyer = AllCompanies.at(normBuyer).find(x);
    auto seller = AllCompanies.at(normSeller).find(x);
    if (!buyer->isRecieved)
        return false;

    if (seller->isSent &&
        !buyer->isSent) {
        AllCompanies.at(normSeller).erase(x);
        AllCompanies.at(normBuyer).erase(x);
    } else {
        buyer->isRecieved = false;
        seller->isRecieved = false;

    }
    return true;
}

/*
 * Functor for sort. iterate through all keys(sorting criteria) and sort by them, if there is no key left, sorts by ID (adding order)
 * if values are same, checks for next key. If there is no key left orders by ID
 */
struct CustomSort {
    explicit CustomSort(const CSortOpt &keys) : tempClass(keys) {};

    CSortOpt tempClass;

    bool operator()(const CInvoice &lhs, const CInvoice &rhs) {

        for (auto &key: tempClass.Keys) {
            switch (key.first) {
                case 0:
                    if (lhs.date().compare(rhs.date()) == 0)
                        continue;
                    if (key.second)
                        return lhs.date().compare(rhs.date()) < 0;
                    else
                        return lhs.date().compare(rhs.date()) > 0;

                case 1: {
                    string normLeft = normalizeNameSort(lhs.buyer());
                    string normRight = normalizeNameSort(rhs.buyer());
                    if (normLeft == normRight)
                        continue;
                    if (key.second)
                        return normLeft < normRight;
                    else
                        return normLeft > normRight;
                }

                case 2: {
                    string normLeftS = normalizeNameSort(lhs.seller());
                    string normRightS = normalizeNameSort(rhs.seller());
                    if (normLeftS == normRightS)
                        continue;
                    if (key.second)
                        return normLeftS < normRightS;

                    else
                        return normLeftS > normRightS;
                }

                case 3:
                    if (lhs.amount() == rhs.amount())
                        continue;
                    if (key.second)
                        return lhs.amount() < rhs.amount();
                    else
                        return lhs.amount() > rhs.amount();

                case 4:
                    if (isEqual(lhs.vat(), rhs.vat()))

                        continue;
                    if (key.second)
                        return lhs.vat() < rhs.vat();
                    else
                        return lhs.vat() > rhs.vat();
                default:
                    break;
            }
            return rhs.m_pos > lhs.m_pos;
        }
        return rhs.m_pos > lhs.m_pos;

    }
};

/*
 * Print func
 */
void Print(set<CInvoice> temp) {

    for (auto it = temp.begin(); it != temp.end(); it++) {
        cout << it->date().year() << ":" << it->date().month() << ":"
             << it->date().day() << " " << it->seller() << " , " << it->buyer() << " -- " << it->amount() << " "
             << it->vat() << " ID: " << it->m_pos << endl;
    }
    cout << "--------------" << endl;

}

/*
 * Func. that finds all company invoices and iterates through them, if it finds invoice that is unmatched, it adds this invoice to the list.
 * Returns list with all unmatched invoices
 */
list<CInvoice> CVATRegister::unmatched(const string &company, const CSortOpt &sortBy) const {
    list<CInvoice> temp;
    string normName = normalizeName(company);
    if (!AllCompanies.count(normName))
        return temp;
    auto mySet = AllCompanies.at(normName);
    for (auto it = mySet.begin(); it != mySet.end(); it++) {

        if ((it->isSent && !it->isRecieved) || (!it->isSent && it->isRecieved))
            temp.push_back(*it);
    }
    //Print(mySet);

    temp.sort(CustomSort(sortBy));
    //  Print(mySet);

    return temp;
}

CVATRegister::CVATRegister(void) { m_pos = 0; }


#ifndef __PROGTEST__

/*
 * Func. checks if all items in two list are same. Returns true if all items are same, else returns false
 *
 */
bool equalLists(const list<CInvoice> &a, const list<CInvoice> &b) {
    if (a.empty()) {
        return true;
    }
    if (a.size() != b.size()) {
        return false;

    }
    for (unsigned int i = 0; i < a.size(); ++i) {
        CInvoice frst = a.front();
        CInvoice scnd = b.front();
        if (frst.buyer() != scnd.buyer() || frst.seller() != scnd.seller() || frst.date().compare(scnd.date()) != 0 ||
            frst.amount() != scnd.amount() ||
            !isEqual(frst.vat(), scnd.vat()))
            return false;
    }

    return true;
}

int main(void) {
    CVATRegister r;
    assert (r.registerCompany("first Company"));
    assert (r.registerCompany("Second     Company"));
    assert (r.registerCompany("ThirdCompany, Ltd."));
    assert (r.registerCompany("Third Company, Ltd."));
    assert (!r.registerCompany("Third Company, Ltd."));
    assert (!r.registerCompany(" Third  Company,  Ltd.  "));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 20)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 2), "FirSt Company", "Second Company ", 200, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "Second Company ", "First Company", 300, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34)));
    assert (!r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
    assert (!r.addIssued(CInvoice(CDate(2000, 1, 4), "First Company", "First   Company", 200, 30)));
    assert (!r.addIssued(CInvoice(CDate(2000, 1, 4), "Another Company", "First   Company", 200, 30)));
    // r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) );

    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER,
                                                                                       false).addKey(CSortOpt::BY_DATE,
                                                                                                     false)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000)
                               }));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_DATE, true).addKey(CSortOpt::BY_SELLER, true).addKey(
                                           CSortOpt::BY_BUYER, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000)
                               }));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_VAT, true).addKey(CSortOpt::BY_AMOUNT, true).addKey(
                                           CSortOpt::BY_DATE, true).addKey(CSortOpt::BY_SELLER, true).addKey(
                                           CSortOpt::BY_BUYER, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000)
                               }));
    assert (equalLists(r.unmatched("First Company", CSortOpt()),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000)
                               }));
    assert (equalLists(r.unmatched("second company", CSortOpt().addKey(CSortOpt::BY_DATE, false)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Third Company, Ltd.", "Second     Company", 400,
                                                34.000000)
                               }));
    assert (equalLists(r.unmatched("last company", CSortOpt().addKey(CSortOpt::BY_VAT, true)),
                       list<CInvoice>
                               {
                               }));
    assert (r.addAccepted(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company ", 200, 30)));
    assert (r.addAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert (r.addAccepted(CInvoice(CDate(2000, 1, 1), "Second company ", "First Company", 300, 32)));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                           CSortOpt::BY_DATE, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                32.000000)
                               }));
    assert (!r.delIssued(CInvoice(CDate(2001, 1, 1), "First Company", "Second Company ", 200, 30)));
    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "A First Company", "Second Company ", 200, 30)));
    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Hand", 200, 30)));
    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 1200, 30)));
    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 200, 130)));
    assert (r.delIssued(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company", 200, 30)));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                           CSortOpt::BY_DATE, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                32.000000)
                               }));
    assert (r.delAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                           CSortOpt::BY_DATE, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                32.000000)
                               }));
    assert (r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert (equalLists(r.unmatched("First Company",
                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
                                           CSortOpt::BY_DATE, true)),
                       list<CInvoice>
                               {
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                20.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                30.000000),
                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
                                                32.000000)
                               }));
    /**/
    return EXIT_SUCCESS;

}

#endif /* __PROGTEST__ */