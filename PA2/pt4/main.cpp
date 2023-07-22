#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
#endif /* __PROGTEST__ */

/*
 * CMail
 */
class CMail {
public:
    CMail(const char *from, const char *to, const char *body);

    ~CMail();

    bool operator==(const CMail &x) const;

    friend ostream &operator<<(ostream &os, const CMail &m);

    char *fromS;
    char *toS;
    char *contentS;
private:


};

bool CMail::operator==(const CMail &x) const {


    if (strcmp(this->fromS, x.fromS) == 0 && strcmp(this->toS, x.toS) == 0 && strcmp(this->contentS, x.contentS) == 0)
        return true;


    return false;
}

ostream &operator<<(ostream &os, const CMail &m) {
    os << "From: " << m.fromS << ", To: " << m.toS << ", Body: " << m.contentS;
    return os;
}

CMail::CMail(const char *from, const char *to, const char *body) {
    fromS = new char[strlen(from) + 20];
    strcpy(fromS, from);
    toS = new char[strlen(to) + 20];
    strcpy(toS, to);
    contentS = new char[strlen(body) + 20];
    strcpy(contentS, body);


}

CMail::~CMail() {


    delete[] fromS;
    delete[] toS;
    delete[] contentS;

}

class Person {
public:


    int posO;
    int sizeO;
    int posI;
    int sizeI;
    char *nameS;
    CMail *inbox;
    CMail *outbox;

private:

    // todo
};


/*
 * CMailIterator
 */
class CMailIterator {
public:
    explicit operator bool(void) const;

    bool operator!(void) const;

    const CMail &operator*(void) const;

    CMailIterator &operator++(void);

    ~CMailIterator();

    void Print();

    CMail *Mails;
    int size;
    int pos;
private:

};

bool CMailIterator::operator!(void) const {
    if (pos < size)
        return false;
    return true;
}

const CMail &CMailIterator::operator*(void) const {

    return Mails[pos];
}

CMailIterator &CMailIterator::operator++(void) {
    pos++;

    return *this;
}

CMailIterator::operator bool(void) const {
    if (pos < size)
        return true;
    return false;
}

CMailIterator::~CMailIterator() {
    for (int i = 0; i < size; ++i) {
        delete[] Mails[i].fromS;
        delete[] Mails[i].toS;
        delete[] Mails[i].contentS;

    }
    free(Mails);
}

void CMailIterator::Print() {
    for (int i = 0; i < size; ++i) {


        cout << Mails[i] << endl;

        cout << endl;
    }
}

/*
 * CMailServer
 */
class CMailServer {
public:
    CMailServer(void);

    CMailServer(const CMailServer &src);

    CMailServer &operator=(const CMailServer &src);

    ~CMailServer(void);

    void sendMail(const CMail &m);

    CMailIterator outbox(const char *email) const;

    CMailIterator inbox(const char *email) const;

    void Print();

private:

    int FindPerson(const char *name) const;
    void Myreal();
    Person *persons;
    int pos;
    int size;

    void Copy(const CMailServer &src) {
        size = src.size;
        pos = src.pos;
        persons = (Person *) malloc(size * sizeof(Person));
        for (int i = 0; i < src.pos; ++i) {

            persons[i].sizeI = src.persons[i].sizeI;
            persons[i].sizeO = src.persons[i].sizeO;

            persons[i].posI = src.persons[i].posI;
            persons[i].posO = src.persons[i].posO;

            persons[i].nameS = (char *) malloc((strlen(src.persons[i].nameS) + 20) * sizeof(char));
            strcpy(persons[i].nameS, src.persons[i].nameS);

            persons[i].inbox = (CMail *) malloc((src.persons[i].posI + 20) * sizeof(CMail));
            persons[i].outbox = (CMail *) malloc((src.persons[i].posO + 20) * sizeof(CMail));

            for (int j = 0; j < src.persons[i].posI; ++j) {
                persons[i].inbox[j].fromS = new char[strlen(src.persons[i].inbox[j].fromS) + 20];
                persons[i].inbox[j].toS = new char[strlen(src.persons[i].inbox[j].toS) + 20];
                persons[i].inbox[j].contentS = new char[strlen(src.persons[i].inbox[j].contentS) + 20];

                strcpy(persons[i].inbox[j].fromS, src.persons[i].inbox[j].fromS);
                strcpy(persons[i].inbox[j].toS, src.persons[i].inbox[j].toS);
                strcpy(persons[i].inbox[j].contentS, src.persons[i].inbox[j].contentS);
            }
            for (int j = 0; j < src.persons[i].posO; ++j) {
                persons[i].outbox[j].fromS = new char[strlen(src.persons[i].outbox[j].fromS) + 20];
                persons[i].outbox[j].toS = new char[strlen(src.persons[i].outbox[j].toS) + 20];
                persons[i].outbox[j].contentS = new char[strlen(src.persons[i].outbox[j].contentS) + 20];

                strcpy(persons[i].outbox[j].fromS, src.persons[i].outbox[j].fromS);
                strcpy(persons[i].outbox[j].toS, src.persons[i].outbox[j].toS);
                strcpy(persons[i].outbox[j].contentS, src.persons[i].outbox[j].contentS);
            }


        }
        for (int i = pos; i < size; ++i) {
            persons[i].sizeI = 20;
            persons[i].sizeO = 20;

            persons[i].posI = 0;
            persons[i].posO = 0;
        }
    }
    void MCopy(Person *&my) {


        for (int i = 0; i < pos; ++i) {

            my[i].sizeI = persons[i].sizeI;
            my[i].sizeO = persons[i].sizeO;

            my[i].posI = persons[i].posI;
            my[i].posO = persons[i].posO;

            my[i].nameS = (char *) malloc((strlen(persons[i].nameS) + 20) * sizeof(char));
            strcpy(my[i].nameS, persons[i].nameS);

            my[i].inbox = (CMail *) malloc((persons[i].posI + 20) * sizeof(CMail));
            my[i].outbox = (CMail *) malloc((persons[i].posO + 20) * sizeof(CMail));

            for (int j = 0; j < persons[i].posI; ++j) {
                my[i].inbox[j].fromS = new char[strlen(persons[i].inbox[j].fromS) + 20];
                my[i].inbox[j].toS = new char[strlen(persons[i].inbox[j].toS) + 20];
                my[i].inbox[j].contentS = new char[strlen(persons[i].inbox[j].contentS) + 20];

                strcpy(my[i].inbox[j].fromS, persons[i].inbox[j].fromS);
                strcpy(my[i].inbox[j].toS, persons[i].inbox[j].toS);
                strcpy(my[i].inbox[j].contentS, persons[i].inbox[j].contentS);
            }
            for (int j = 0; j < persons[i].posO; ++j) {
                my[i].outbox[j].fromS = new char[strlen(persons[i].outbox[j].fromS) + 20];
                my[i].outbox[j].toS = new char[strlen(persons[i].outbox[j].toS) + 20];
                my[i].outbox[j].contentS = new char[strlen(persons[i].outbox[j].contentS) + 20];

                strcpy(my[i].outbox[j].fromS, persons[i].outbox[j].fromS);
                strcpy(my[i].outbox[j].toS, persons[i].outbox[j].toS);
                strcpy(my[i].outbox[j].contentS, persons[i].outbox[j].contentS);
            }


        }
        for (int i = pos; i < size; ++i) {
            my[i].sizeI = 20;
            my[i].sizeO = 20;

            my[i].posI = 0;
            my[i].posO = 0;
        }
    }
    void MCopyt( Person *&my) {


        for (int i = 0; i < pos; ++i) {

            persons[i].sizeI = my[i].sizeI;
            persons[i].sizeO = my[i].sizeO;

            persons[i].posI = my[i].posI;
            persons[i].posO = my[i].posO;

            persons[i].nameS = (char *) malloc((strlen(my[i].nameS) + 20) * sizeof(char));
            strcpy(persons[i].nameS, my[i].nameS);

            persons[i].inbox = (CMail *) malloc((my[i].posI + 20) * sizeof(CMail));
            persons[i].outbox = (CMail *) malloc((my[i].posO + 20) * sizeof(CMail));

            for (int j = 0; j < my[i].posI; ++j) {
                persons[i].inbox[j].fromS = new char[strlen(my[i].inbox[j].fromS) + 20];
                persons[i].inbox[j].toS = new char[strlen(my[i].inbox[j].toS) + 20];
                persons[i].inbox[j].contentS = new char[strlen(my[i].inbox[j].contentS) + 20];

                strcpy(persons[i].inbox[j].fromS, my[i].inbox[j].fromS);
                strcpy(persons[i].inbox[j].toS, my[i].inbox[j].toS);
                strcpy(persons[i].inbox[j].contentS, my[i].inbox[j].contentS);
            }
            for (int j = 0; j < my[i].posO; ++j) {
                persons[i].outbox[j].fromS = new char[strlen(my[i].outbox[j].fromS) + 20];
                persons[i].outbox[j].toS = new char[strlen(my[i].outbox[j].toS) + 20];
                persons[i].outbox[j].contentS = new char[strlen(my[i].outbox[j].contentS) + 20];

                strcpy(persons[i].outbox[j].fromS, my[i].outbox[j].fromS);
                strcpy(persons[i].outbox[j].toS, my[i].outbox[j].toS);
                strcpy(persons[i].outbox[j].contentS, my[i].outbox[j].contentS);
            }


        }
        for (int i = pos; i < size; ++i) {
            persons[i].sizeI = 20;
            persons[i].sizeO = 20;

            persons[i].posI = 0;
            persons[i].posO = 0;
        }
    }
    void MyrealBox(int poss, int sizee, int fromm,int in);
    void CopyMail(CMail *&arr,int indt, const CMail &ma){
        arr[indt].fromS = new char[strlen(ma.fromS) + 20];
        arr[indt].toS = new char[strlen(ma.toS) + 20];
        arr[indt].contentS = new char[strlen(ma.contentS) + 20];

        strcpy(  arr[indt].fromS, ma.fromS);
        strcpy(  arr[indt].toS, ma.toS);
        strcpy( arr[indt].contentS, ma.contentS);
    }
    void DeleteFunc(){
        for (int i = 0; i < pos; ++i) {
            free(persons[i].nameS);
            for (int j = 0; j < persons[i].posI; ++j) {
                delete[]persons[i].inbox[j].fromS;
                delete[]persons[i].inbox[j].toS;
                delete[]persons[i].inbox[j].contentS;


            }

            for (int j = 0; j < persons[i].posO; ++j) {
                delete[]persons[i].outbox[j].fromS;
                delete[]persons[i].outbox[j].toS;
                delete[]persons[i].outbox[j].contentS;

            }
            free(persons[i].inbox);
            free(persons[i].outbox);

        }

        free(persons);
    }

};

CMailServer::CMailServer(void) {
    pos = 0;
    size = 2;


    persons = (Person *) malloc(size * sizeof(Person));
    for (int i = 0; i < size; ++i) {
        persons[i].sizeI = 20;
        persons[i].posI = 0;
        persons[i].sizeO = 20;
        persons[i].posO = 0;
    }


}

CMailServer::~CMailServer(void) {
    DeleteFunc();
}

CMailServer::CMailServer(const CMailServer &src) {
    Copy(src);
}

CMailServer &CMailServer::operator=(const CMailServer &src) {

    if (&src == this)
        return *this;
    DeleteFunc();
    Copy(src);
    return *this;

}


void CMailServer::sendMail(const CMail &m) {
    int from = FindPerson(m.fromS);

    if ((pos) >= size - 1) {

        size *= 2;
        Myreal();
        for (int i = pos; i < size; ++i) {
            persons[i].sizeI = 20;
            persons[i].posI = 0;
            persons[i].sizeO = 20;
            persons[i].posO = 0;
        }
    }


    if (from == -1) {

        persons[pos].nameS = (char *) malloc((strlen(m.fromS) + 20) * sizeof(char));
        strcpy(persons[pos].nameS, m.fromS);
        persons[pos].inbox = (CMail *) malloc(20 * sizeof(CMail));
        persons[pos].outbox = (CMail *) malloc(20 * sizeof(CMail));

        CopyMail(persons[pos].outbox,0,m);
        persons[pos++].posO++;



    } else {
        if (persons[from].posO >= persons[from].sizeO) {
            persons[from].sizeO *= 2;
            MyrealBox(persons[from].posO,persons[from].sizeO,from,1);
        }

        CopyMail(persons[from].outbox,persons[from].posO,m);
        persons[from].posO++;

    }

    int to = FindPerson(m.toS);
    if (to == -1) {

        persons[pos].nameS = (char *) malloc((strlen(m.fromS) + 20) * sizeof(char));
        strcpy(persons[pos].nameS, m.toS);

        persons[pos].inbox = (CMail *) malloc(20 * sizeof(CMail));
        persons[pos].outbox = (CMail *) malloc(20 * sizeof(CMail));
        CopyMail(persons[pos].inbox,0,m);
        persons[pos++].posI++;

    } else {
        if (persons[to].posI >= persons[to].sizeI) {
            persons[to].sizeI *= 2;
            MyrealBox(persons[to].posI,persons[to].sizeI,to,0);
        }
        CopyMail( persons[to].inbox,persons[to].posI,m);
        persons[to].posI++;

    }


}

CMailIterator CMailServer::outbox(const char *email) const {
    int ret = FindPerson(email);
    CMailIterator tmp{};
    if (ret != -1) {

        tmp.size = persons[ret].posO;
        tmp.pos = 0;
        tmp.Mails = (CMail *) malloc((tmp.size + 5) * sizeof(CMail));
        for (int i = 0; i < tmp.size; ++i) {
            tmp.Mails[i].fromS = new char[strlen(persons[ret].outbox[i].fromS) + 20];
            tmp.Mails[i].toS = new char[strlen(persons[ret].outbox[i].toS) + 20];
            tmp.Mails[i].contentS = new char[strlen(persons[ret].outbox[i].contentS) + 20];

            strcpy(tmp.Mails[i].fromS, persons[ret].outbox[i].fromS);
            strcpy(tmp.Mails[i].toS, persons[ret].outbox[i].toS);
            strcpy(tmp.Mails[i].contentS, persons[ret].outbox[i].contentS);

        }

    } else {
        tmp.size = 0;
        tmp.pos = 0;
    }

    return tmp;
}

CMailIterator CMailServer::inbox(const char *email) const {
    int ret = FindPerson(email);
    CMailIterator tmp{};
    if (ret != -1) {

        tmp.size = persons[ret].posI;
        tmp.pos = 0;
        tmp.Mails = (CMail *) malloc((tmp.size + 5) * sizeof(CMail));
        for (int i = 0; i < tmp.size; ++i) {
            tmp.Mails[i].fromS = new char[strlen(persons[ret].inbox[i].fromS) + 20];
            tmp.Mails[i].toS = new char[strlen(persons[ret].inbox[i].toS) + 20];
            tmp.Mails[i].contentS = new char[strlen(persons[ret].inbox[i].contentS) + 20];

            strcpy(tmp.Mails[i].fromS, persons[ret].inbox[i].fromS);
            strcpy(tmp.Mails[i].toS, persons[ret].inbox[i].toS);
            strcpy(tmp.Mails[i].contentS, persons[ret].inbox[i].contentS);

        }

    } else {
        tmp.size = 0;
        tmp.pos = 0;
    }

    return tmp;
}

int CMailServer::FindPerson(const char *name) const {
    for (int i = 0; i < pos; ++i) {
        if (strcmp(persons[i].nameS, name) == 0)
            return i;
    }
    return -1;
}

void CMailServer::Print() {

    for (int i = 0; i < pos; ++i) {

        cout << "Schranka: " << persons[i].nameS << endl;
        cout << "Inbox: " << endl;


        for (int j = 0; j < persons[i].posI; ++j) {

            cout << persons[i].inbox[j] << endl;
        }
        cout << "Outbox: " << endl;
        for (int j = 0; j < persons[i].posO; ++j) {

            cout << persons[i].outbox[j] << endl;
        }
        cout << endl;
    }
}

void CMailServer::Myreal() {
   // Copy(*this);
    Person *tmp=new Person[size];
    MCopy(tmp);


    for (int i = 0; i < pos; ++i) {
        free(persons[i].nameS);
        for (int j = 0; j < persons[i].posI; ++j) {
            delete[]persons[i].inbox[j].fromS;
            delete[]persons[i].inbox[j].toS;
            delete[]persons[i].inbox[j].contentS;


        }

        for (int j = 0; j < persons[i].posO; ++j) {
            delete[]persons[i].outbox[j].fromS;
            delete[]persons[i].outbox[j].toS;
            delete[]persons[i].outbox[j].contentS;

        }
        free(persons[i].inbox);
        free(persons[i].outbox);

    }

    free(persons);
    persons = (Person *) malloc(size * sizeof(Person));
    MCopyt(tmp);
    for (int i = 0; i < pos; ++i) {
        free(tmp[i].nameS);
        for (int j = 0; j < tmp[i].posI; ++j) {
            delete[]tmp[i].inbox[j].fromS;
            delete[]tmp[i].inbox[j].toS;
            delete[]tmp[i].inbox[j].contentS;


        }

        for (int j = 0; j < tmp[i].posO; ++j) {
            delete[]tmp[i].outbox[j].fromS;
            delete[]tmp[i].outbox[j].toS;
            delete[]tmp[i].outbox[j].contentS;

        }
        free(tmp[i].inbox);
        free(tmp[i].outbox);

    }

    delete[](tmp);




  // persons = (Person *) realloc(persons, size * sizeof(Person));
}

void CMailServer::MyrealBox(int poss, int sizee,int froom,int in) {
    CMail * temp= (CMail*) malloc(sizee*sizeof (CMail));
    if(in==1){
    for (int j = 0; j < persons[froom].posO; ++j) {

        temp[j].fromS = new char[strlen(persons[froom].outbox[j].fromS) + 20];
        temp[j].toS = new char[strlen(persons[froom].outbox[j].toS) + 20];
        temp[j].contentS = new char[strlen(persons[froom].outbox[j].contentS) + 20];

        strcpy(temp[j].fromS, persons[froom].outbox[j].fromS);
        strcpy(temp[j].toS, persons[froom].outbox[j].toS);
        strcpy(temp[j].contentS, persons[froom].outbox[j].contentS);
    }

    for (int j = 0; j < poss; ++j) {
        delete[]persons[froom].outbox[j].fromS;
        delete[]persons[froom].outbox[j].toS;
        delete[]persons[froom].outbox[j].contentS;

    }
    free(persons[froom].outbox);

    persons[froom].outbox= (CMail*) malloc(sizee*sizeof (CMail));
    for (int j = 0; j < poss; ++j) {

        persons[froom].outbox[j].fromS = new char[strlen(temp[j].fromS) + 20];
        persons[froom].outbox[j].toS = new char[strlen(temp[j].toS) + 20];
        persons[froom].outbox[j].contentS = new char[strlen(temp[j].contentS) + 20];

        strcpy( persons[froom].outbox[j].fromS, temp[j].fromS);
        strcpy( persons[froom].outbox[j].toS, temp[j].toS);
        strcpy( persons[froom].outbox[j].contentS, temp[j].contentS);
    }
}
    else{
        for (int j = 0; j < persons[froom].posI; ++j) {

            temp[j].fromS = new char[strlen(persons[froom].inbox[j].fromS) + 20];
            temp[j].toS = new char[strlen(persons[froom].inbox[j].toS) + 20];
            temp[j].contentS = new char[strlen(persons[froom].inbox[j].contentS) + 20];

            strcpy(temp[j].fromS, persons[froom].inbox[j].fromS);
            strcpy(temp[j].toS, persons[froom].inbox[j].toS);
            strcpy(temp[j].contentS, persons[froom].inbox[j].contentS);
        }

        for (int j = 0; j < poss; ++j) {
            delete[]persons[froom].inbox[j].fromS;
            delete[]persons[froom].inbox[j].toS;
            delete[]persons[froom].inbox[j].contentS;

        }
        free(persons[froom].inbox);

        persons[froom].inbox= (CMail*) malloc(sizee*sizeof (CMail));
        for (int j = 0; j < poss; ++j) {

            persons[froom].inbox[j].fromS = new char[strlen(temp[j].fromS) + 20];
            persons[froom].inbox[j].toS = new char[strlen(temp[j].toS) + 20];
            persons[froom].inbox[j].contentS = new char[strlen(temp[j].contentS) + 20];

            strcpy( persons[froom].inbox[j].fromS, temp[j].fromS);
            strcpy( persons[froom].inbox[j].toS, temp[j].toS);
            strcpy( persons[froom].inbox[j].contentS, temp[j].contentS);
        }
    }

    for (int j = 0; j < poss; ++j) {
        delete[]temp[j].fromS;
        delete[]temp[j].toS;
        delete[]temp[j].contentS;

    }
    free(temp);
}



#ifndef __PROGTEST__

bool matchOutput(const CMail &m, const char *str) {
    ostringstream oss;
    oss << m;
    return oss.str() == str;
}

int main(void) {

    char from[100], to[100], body[1024];


    assert (CMail("john", "peter", "progtest deadline") == CMail("john", "peter", "progtest deadline"));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("john", "progtest deadline", "peter")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("peter", "john", "progtest deadline")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("peter", "progtest deadline", "john")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "john", "peter")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "peter", "john")));

    CMailServer s0;
    s0.sendMail(CMail("john", "peter", "some important mail"));
    strncpy(from, "john", sizeof(from));
    strncpy(to, "thomas", sizeof(to));
    strncpy(body, "another important mail", sizeof(body));
    s0.sendMail(CMail(from, to, body));
    strncpy(from, "john", sizeof(from));
    strncpy(to, "alice", sizeof(to));
    strncpy(body, "deadline notice", sizeof(body));
    s0.sendMail(CMail(from, to, body));
    s0.sendMail(CMail("alice", "john", "deadline confirmation"));
    s0.sendMail(CMail("peter", "alice", "PR bullshit"));

    CMailIterator i0 = s0.inbox("alice");


    assert (i0 && *i0 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i0, "From: john, To: alice, Body: deadline notice"));

    assert (++i0 && *i0 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i0, "From: peter, To: alice, Body: PR bullshit"));
    assert (!++i0);

    CMailIterator i1 = s0.inbox("john");
    assert (i1 && *i1 == CMail("alice", "john", "deadline confirmation"));
    assert (matchOutput(*i1, "From: alice, To: john, Body: deadline confirmation"));
    assert (!++i1);

    CMailIterator i2 = s0.outbox("john");
    assert (i2 && *i2 == CMail("john", "peter", "some important mail"));
    assert (matchOutput(*i2, "From: john, To: peter, Body: some important mail"));
    assert (++i2 && *i2 == CMail("john", "thomas", "another important mail"));
    assert (matchOutput(*i2, "From: john, To: thomas, Body: another important mail"));
    assert (++i2 && *i2 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i2, "From: john, To: alice, Body: deadline notice"));
    assert (!++i2);

    CMailIterator i3 = s0.outbox("thomas");
    assert (!i3);

    CMailIterator i4 = s0.outbox("steve");
    assert (!i4);

    CMailIterator i5 = s0.outbox("thomas");
    s0.sendMail(CMail("thomas", "boss", "daily report"));
    assert (!i5);

    CMailIterator i6 = s0.outbox("thomas");
    assert (i6 && *i6 == CMail("thomas", "boss", "daily report"));
    assert (matchOutput(*i6, "From: thomas, To: boss, Body: daily report"));
    assert (!++i6);

    CMailIterator i7 = s0.inbox("alice");
    s0.sendMail(CMail("thomas", "alice", "meeting details"));
    assert (i7 && *i7 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i7, "From: john, To: alice, Body: deadline notice"));
    assert (++i7 && *i7 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i7, "From: peter, To: alice, Body: PR bullshit"));
    assert (!++i7);

    CMailIterator i8 = s0.inbox("alice");
    assert (i8 && *i8 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i8, "From: john, To: alice, Body: deadline notice"));
    assert (++i8 && *i8 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i8, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i8 && *i8 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i8, "From: thomas, To: alice, Body: meeting details"));
    assert (!++i8);


    CMailServer s1(s0);

    s0.sendMail(CMail("joe", "alice", "delivery details"));


    s1.sendMail(CMail("sam", "alice", "order confirmation"));

    CMailIterator i9 = s0.inbox("alice");

    assert (i9 && *i9 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i9, "From: john, To: alice, Body: deadline notice"));

    assert (++i9 && *i9 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i9, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i9 && *i9 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i9, "From: thomas, To: alice, Body: meeting details"));
    assert (++i9 && *i9 == CMail("joe", "alice", "delivery details"));
    assert (matchOutput(*i9, "From: joe, To: alice, Body: delivery details"));
    assert (!++i9);

    CMailIterator i10 = s1.inbox("alice");
    assert (i10 && *i10 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i10, "From: john, To: alice, Body: deadline notice"));
    assert (++i10 && *i10 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i10, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i10 && *i10 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i10, "From: thomas, To: alice, Body: meeting details"));
    assert (++i10 && *i10 == CMail("sam", "alice", "order confirmation"));
    assert (matchOutput(*i10, "From: sam, To: alice, Body: order confirmation"));
    assert (!++i10);

    CMailServer s2;
    s2.sendMail(CMail("alice", "alice", "mailbox test"));
    CMailIterator i11 = s2.inbox("alice");
    assert (i11 && *i11 == CMail("alice", "alice", "mailbox test"));
    assert (matchOutput(*i11, "From: alice, To: alice, Body: mailbox test"));
    assert (!++i11);

    s2 = s0;
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s2.sendMail(CMail("paul", "alice", "invalid invoice"));
    CMailIterator i12 = s0.inbox("alice");
    assert (i12 && *i12 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i12, "From: john, To: alice, Body: deadline notice"));
    assert (++i12 && *i12 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i12, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i12 && *i12 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i12, "From: thomas, To: alice, Body: meeting details"));
    assert (++i12 && *i12 == CMail("joe", "alice", "delivery details"));
    assert (matchOutput(*i12, "From: joe, To: alice, Body: delivery details"));
    assert (++i12 && *i12 == CMail("steve", "alice", "newsletter"));
    assert (matchOutput(*i12, "From: steve, To: alice, Body: newsletter"));
    assert (!++i12);

    CMailIterator i13 = s2.inbox("alice");
    assert (i13 && *i13 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i13, "From: john, To: alice, Body: deadline notice"));
    assert (++i13 && *i13 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i13, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i13 && *i13 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i13, "From: thomas, To: alice, Body: meeting details"));
    assert (++i13 && *i13 == CMail("joe", "alice", "delivery details"));
    assert (matchOutput(*i13, "From: joe, To: alice, Body: delivery details"));
    assert (++i13 && *i13 == CMail("paul", "alice", "invalid invoice"));
    assert (matchOutput(*i13, "From: paul, To: alice, Body: invalid invoice"));
    assert (!++i13);
    s2.sendMail(CMail("john","john","Lorem ipsum dolor sit amet, "
                                    "consectetuer adipiscing elit. Sed ut perspiciatis unde "
                                    "omnis iste natus error sit voluptatem accusantium doloremque laudantium, "
                                    "totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Integer rutrum, orci vestibulum ullamcorper ultricies, lacus quam ultricies odio, vitae placerat pede sem sit amet enim. Etiam posuere lacus quis dolor. Nullam sit amet magna in magna gravida vehicula. Etiam egestas wisi a erat. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Maecenas libero. Integer tempor. Aenean vel massa quis mauris vehicula lacinia. Nam libero tempore, cum soluta nobis est eligendi optio cumque nihil impedit quo minus id quod maxime placeat facere possimus, omnis voluptas assumenda est, omnis dolor repellendus. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur "
                                    "ridiculus mus. Maecenas fermentum, sem in pharetra pellentesque, velit turpis volutpat ante, in pharetra metus odio a lectus. Duis viverra diam non justo."));

    CMailServer s3(s2);
    s3=s2;
    CMailIterator i14=s3.outbox("john");
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s0.sendMail(CMail("steve", "alice", "newsletter"));

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */