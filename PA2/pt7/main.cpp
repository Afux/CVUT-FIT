#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

template<typename M_>
class CContest {
public:
    // default constructor
    // destructor
    CContest& addMatch ( const string &cont1, const string &cont2,M_ result ){
        if (resultsS.count({cont1,cont2})||resultsS.count({cont2,cont1}))
            throw logic_error("");
        else{
            resultsS.insert({{cont1,cont2},result});
        }

        return *this;
    }
    bool isOrdered (  function<int(M_)> func ) const;
    list<string> results (function<int(M_)> func) const;

private:
   map<pair<string,string>,M_> resultsS;
};

template<typename M_>
bool CContest<M_>::isOrdered(function<int(M_)> func) const {
    map<string,pair<int,vector<int*>>> myList;
    for (auto itr=resultsS.begin(); itr!= resultsS.end() ; ++itr) {
        if(!myList.count(itr->first.first)){
            vector<int*> p;
            myList[itr->first.first]= make_pair(0,p);
        }

        if(!myList.count(itr->first.second)){
            vector<int*> p;
            myList[itr->first.second]= make_pair(0,p);
        }

        if(func(itr->second)==0){
            return false;

        }
       else if(func(itr->second)>0){
            myList[itr->first.first].first++;
            myList[itr->first.second].second.push_back(& myList[itr->first.first].first);
        }

        else if(func(itr->second)<0){
            myList[itr->first.second].first++;
            myList[itr->first.first].second.push_back(&myList[itr->first.second].first);
        }
    }

    string index;

    while (!myList.empty()){
        int tempcnt=0;

        for (auto & itr : myList) {
            if(itr.second.first==0){
                tempcnt++;
                index=itr.first;
            }
            if(tempcnt>1){
                return false;

            }

        }
        if(tempcnt<1){
            return false;

        }
        for (auto & itr : myList[index].second) {
            *itr-=1;
        }
        myList.erase(index);

    }

    return true;
}

template<typename M_>
list<string> CContest<M_>::results(function<int(M_)> func) const {
    map<string,pair<int,vector<int*>>> myList;
    list<string> ret;
    for (auto itr=resultsS.begin(); itr!= resultsS.end() ; ++itr) {
        if(!myList.count(itr->first.first)){
            vector<int*> p;
            myList[itr->first.first]= make_pair(0,p);
        }

        if(!myList.count(itr->first.second)){
            vector<int*> p;
            myList[itr->first.second]= make_pair(0,p);
        }

        if(func(itr->second)==0){
            throw logic_error("");

        }
        else if(func(itr->second)>0){
            myList[itr->first.first].first++;
            myList[itr->first.second].second.push_back(& myList[itr->first.first].first);
        }

        else if(func(itr->second)<0){
            myList[itr->first.second].first++;
            myList[itr->first.first].second.push_back(&myList[itr->first.second].first);
        }
    }

    string index;

    while (!myList.empty()){
        int tempcnt=0;

        for (auto & itr : myList) {
            if(itr.second.first==0){
                tempcnt++;
                index=itr.first;
            }
            if(tempcnt>1){
                throw logic_error("");


            }

        }
        if(tempcnt<1){
            throw logic_error("");


        }
        for (auto & itr : myList[index].second) {
            *itr-=1;
        }
        ret.push_back(index);
        myList.erase(index);

    }

   if(ret.empty())
       throw logic_error("");
    list<string> temp;
    auto i = ret.end();
    while (i!=ret.begin()){
        --i;
      //  cout<<*i<<endl;
        temp.push_back(*i);
    }
    return temp;
}


#ifndef __PROGTEST__

struct CMatch {
public:
    CMatch(int a,
           int b)
            : m_A(a),
              m_B(b) {
    }

    int m_A;
    int m_B;
};

class HigherScoreThreshold {
public:
    HigherScoreThreshold(int diffAtLeast)
            : m_DiffAtLeast(diffAtLeast) {
    }

    int operator()(const CMatch &x) const {
        return (x.m_A > x.m_B + m_DiffAtLeast) - (x.m_B > x.m_A + m_DiffAtLeast);
    }

private:
    int m_DiffAtLeast;
};

int HigherScore(const CMatch &x) {
    return (x.m_A > x.m_B) - (x.m_B > x.m_A);
}

int main(void) {
    CContest<CMatch> x;

    x.addMatch("C++", "Pascal", CMatch(10, 3))
            .addMatch("C++", "Java", CMatch(8, 1))
            .addMatch("Pascal", "Basic", CMatch(40, 0))
            .addMatch("Java", "PHP", CMatch(6, 2))
            .addMatch("Java", "Pascal", CMatch(7, 3))
            .addMatch("PHP", "Basic", CMatch(10, 0));

    assert (!x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    x.addMatch("PHP", "Pascal", CMatch(3, 6));

    assert (x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        assert ((res == list<string>{"C++", "Java", "Pascal", "PHP", "Basic"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }


    assert (!x.isOrdered(HigherScoreThreshold(3)));
    try {
        list<string> res = x.results(HigherScoreThreshold(3));
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    assert (x.isOrdered([](const CMatch &x) {
        return (x.m_A < x.m_B) - (x.m_B < x.m_A);
    }));
    try {
        list<string> res = x.results([](const CMatch &x) {
            return (x.m_A < x.m_B) - (x.m_B < x.m_A);
        });
        assert ((res == list<string>{"Basic", "PHP", "Pascal", "Java", "C++"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }

    CContest<bool> y;

    y.addMatch("Python", "PHP", true)
            .addMatch("PHP", "Perl", true)
            .addMatch("Perl", "Bash", true)
            .addMatch("Bash", "JavaScript", true)
            .addMatch("JavaScript", "VBScript", true);

    assert (y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert ((res == list<string>{"Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }

    y.addMatch("PHP", "JavaScript", false);
    assert (!y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("PHP", "JavaScript", false);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("JavaScript", "PHP", true);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
