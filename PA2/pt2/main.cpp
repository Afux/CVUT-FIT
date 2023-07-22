#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */
class Person{
public:
    Person  (const string &name, const string &surname, const string &email, unsigned int salary);
    ~Person (void);
    pair<string,string> FullName;
    string Cemail;
    unsigned int Csalary;
    bool operator==(const Person& rhs) const;
};
bool Person::operator==(const Person& rhs) const {
    return Cemail == rhs.Cemail||(FullName.first == rhs.FullName.first&&FullName.second == rhs.FullName.second);
}
Person::Person(const string &name, const string &surname, const string &email, unsigned int salary) {
    FullName.first=name;
    FullName.second=surname;
    Cemail=email;
    Csalary=salary;
}
Person::~Person(void) {

}

class CPersonalAgenda
{
public:
    CPersonalAgenda  (void);
    ~CPersonalAgenda (void);
    bool add (const string &name, const string &surname, const string &email, unsigned int salary);
    bool del (const string &name, const string &surname);
    bool del (const string &email);
    bool changeName (const string &email,const string &newName,const string &newSurname);
    bool changeEmail (const string &name,const string &surname,const string &newEmail);
    bool setSalary (const string &name,const string &surname,unsigned int salary);
    bool setSalary (const string &email,unsigned int salary);
    unsigned int getSalary (const string &name,const string &surname) const;
    unsigned int getSalary (const string &email) const;
    bool getRank (const string &name, const string &surname,int &rankMin,int &rankMax) const;
    bool getRank (const string &email, int &rankMin,int &rankMax ) const;
    bool getFirst (string &outName, string &outSurname) const;
    bool getNext (const string &name,const string &surname,string &outName,string &outSurname) const;
    void Print();

private:

    void SalaryFounder(int &min, int &max, unsigned int salary) const;
    bool FindDuplicate(const string &email);
    int FindAt(const string &name, const string &surname ) const;
    int FindAt(const string &email) const;
    vector<Person> Persons;
    vector<Person> Emails;
};

CPersonalAgenda::CPersonalAgenda(void) {

}
CPersonalAgenda::~CPersonalAgenda(void) {

}
void CPersonalAgenda:: Print(){
    int size=Emails.size();
    for (int i = 0; i < size; ++i) {
        cout<<Emails[i].FullName.first<<" "<<Emails[i].FullName.second<<" "<<Emails[i].Cemail<<" "<<Emails[i].Csalary<<endl;

    }
    cout<<"PERSOHS............."<<endl;
    size=Persons.size();
    for (int i = 0; i < size; ++i) {
        cout<<Persons[i].FullName.first<<" "<<Persons[i].FullName.second<<" "<<Persons[i].Cemail<<" "<<Persons[i].Csalary<<endl;

    }
    cout<<endl;
}
struct MailCmp{
    bool operator()( const Person & left, const Person & right )const
    {

        return left.Cemail.compare(right.Cemail) < 0;
    }
};
struct AddresCMP{
    bool operator()( const Person & left, const Person & right )const
    {
        if(left.FullName.second.compare(right.FullName.second) == 0){

            return left.FullName.first.compare(right.FullName.first) < 0;
        }
        else
            return left.FullName.second.compare(right.FullName.second) < 0;
    }
};
int CPersonalAgenda::FindAt(const string &name, const string &surname )const {


    auto it_A = lower_bound( Persons.begin(), Persons.end(),Person(name,surname,"0000",00), AddresCMP() );

    int its=it_A - Persons.begin();
    int size=Persons.size();

    for (int i = its; i < size; ++i) {
        if(Persons[i].FullName.first==name&&Persons[i].FullName.second==surname){
            return i;
        }
    }
    return -1;
}

int CPersonalAgenda::FindAt(const string &email) const{
    auto it_A = lower_bound( Emails.begin(), Emails.end(),Person("name","surname",email,00), MailCmp() );
    int its=it_A - Emails.begin();
    int size=Emails.size();
    for (int i = its; i < size; ++i) {
        if(Emails[i].Cemail==email)
            return i;
    }
    return -1;
}

bool CPersonalAgenda::FindDuplicate(const string &email){

    auto it_A = lower_bound( Emails.begin(), Emails.end(),Person("name","surname",email,00), MailCmp() );
    int its=it_A - Emails.begin();
    int size=Emails.size();
    for (int i = its; i < size; ++i) {
        if(Emails[i].Cemail==email)
            return true;
    }
    return false;

}
void CPersonalAgenda::SalaryFounder(int &min, int &max,unsigned int salary) const {
    int Mymin=0;
    int Mymax=0;
    int size=Persons.size();
    for (int i = 0; i < size ; ++i) {
        if(salary>Persons[i].Csalary)
            Mymin++;
        else if(salary==Persons[i].Csalary)
            Mymax++;
    }
    Mymax+=Mymin-1;
    max=Mymax;
    min=Mymin;
}

bool CPersonalAgenda::add(const string &name, const string &surname, const string &email, unsigned int salary) {


    Person tmp ( name, surname, email,salary );
    bool isEql= false;
    bool isEqlA=false;
    long unsigned int POS=0;
    long unsigned int POS_A=0;

    if(Persons.empty()&&Emails.empty()){

        Persons.push_back(tmp);
        Emails.push_back(tmp);
        return true;
    }

    auto it_N = lower_bound( Persons.begin(), Persons.end(),tmp, AddresCMP() );
    auto it_E = lower_bound( Emails.begin(), Emails.end(),tmp, MailCmp() );

    POS=it_E - Emails.begin();
    POS_A=it_N - Persons.begin();
    //Checking if found company is in array range so we can keep our array sorted
    if((POS>Emails.size()&&POS_A<Persons.size())||(POS_A>Persons.size()&&POS<Emails.size()))
        return false;
    if(POS<Emails.size()||POS_A<Persons.size()){

        if(POS<Emails.size()){
            isEql=(tmp==Emails[POS]);
        }
        if(POS_A<Persons.size()){
            isEqlA=(tmp==Persons[POS_A]);

        }
        //If firm already exists return false
        if((isEql&&POS<Emails.size())||(isEqlA&&POS_A<Persons.size())){
            return false;}


        else{
            //Adding firm in both arrays

            Emails.insert(Emails.begin()+POS,tmp);
            Persons.insert(Persons.begin()+POS_A,tmp);
            return true;
        }

    }
    //Else adding on end od array
    Emails.push_back(tmp);
    Persons.push_back(tmp);
    return true;

}

bool CPersonalAgenda::del(const string &name, const string &surname) {
    int ind= FindAt(name,surname);
    if(ind>=0){
        int indx= FindAt(Persons[ind].Cemail);
        if(indx>=0)
            Emails.erase(Emails.begin()+indx);
        Persons.erase(Persons.begin()+ind);


        return true;
    }
    return false;
}

bool CPersonalAgenda::del(const string &email) {
    int ind= FindAt(email);
    if(ind>=0){
        int indx= FindAt(Emails[ind].FullName.first,Emails[ind].FullName.second);
        if(indx>=0)
            Persons.erase(Persons.begin()+indx);
        Emails.erase(Emails.begin()+ind);

        return true;
    }
    return false;
}

bool CPersonalAgenda::changeName(const string &email, const string &newName, const string &newSurname) {
    int ind=FindAt(email);

    if(ind>=0){

        Person tmp=Emails[ind];
        if(Emails[ind].FullName.first==newName&&Emails[ind].FullName.second==newSurname)
            return false;
        int indx= FindAt(Emails[ind].FullName.first,Emails[ind].FullName.second);
        Emails.erase(Emails.begin()+ind);
        if(indx>=0)
            Persons.erase(Persons.begin()+indx);

        bool flag= add(newName,newSurname,tmp.Cemail,tmp.Csalary);
        if(!flag)
            add(tmp.FullName.first,tmp.FullName.second,tmp.Cemail,tmp.Csalary);
        return flag;
    }
    return false;
}

bool CPersonalAgenda::changeEmail(const string &name, const string &surname, const string &newEmail) {
    if(FindDuplicate(newEmail))
        return false;
    int ind=FindAt(name,surname);
    if(ind>=0){
        if(Persons[ind].Cemail==newEmail)
            return false;
        int indx= FindAt(Persons[ind].Cemail);
        if(indx>=0)
            Emails[indx].Cemail=newEmail;
        Persons[ind].Cemail=newEmail;

        return true;
    }
    return false;
}

bool CPersonalAgenda::setSalary(const string &name, const string &surname, unsigned int salary) {
    int ind= FindAt(name,surname);
    if(ind>=0){
        Persons[ind].Csalary=salary;
        int indx= FindAt( Persons[ind].Cemail);
        if(indx>=0)
            Emails[indx].Csalary=salary;
        return true;
    }
    return false;
}

bool CPersonalAgenda::setSalary(const string &email, unsigned int salary) {
    int ind= FindAt(email);
    if(ind>=0){
        Emails[ind].Csalary=salary;
        int indx= FindAt(Emails[ind].FullName.first,Emails[ind].FullName.second);
        if(indx>=0)
            Persons[indx].Csalary=salary;
        return true;
    }
    return false;
}

unsigned int CPersonalAgenda::getSalary(const string &name, const string &surname) const {
    int ind= FindAt(name,surname);
    if(ind>=0){
        return Persons[ind].Csalary;

    }
    return 0;
}

unsigned int CPersonalAgenda::getSalary(const string &email) const {

    int ind= FindAt(email);
    if(ind>=0){
        return Emails[ind].Csalary;
    }
    return 0;
}

bool CPersonalAgenda::getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const {
    int ind= FindAt(name,surname);
    if(ind<0)
        return false;
    int salary=Persons[ind].Csalary;
    SalaryFounder(rankMin,rankMax,salary);

    return true;
}

bool CPersonalAgenda::getRank(const string &email, int &rankMin, int &rankMax) const {
    int ind= FindAt(email);
    if(ind<0)
        return false;
    int salary=Emails[ind].Csalary;
    SalaryFounder(rankMin,rankMax,salary);
    return true;
}

bool CPersonalAgenda::getFirst(string &outName, string &outSurname) const {
    if(!Persons.empty()){

        outName=Persons[0].FullName.first;
        outSurname=Persons[0].FullName.second;
        return true;
    }
    return false;
}

bool CPersonalAgenda::getNext(const string &name, const string &surname, string &outName, string &outSurname) const {
    int ind= FindAt(name,surname);
    int size=Persons.size()-1;
    if (ind>=0&&ind<size){

        outName=Persons[ind+1].FullName.first;
        outSurname=Persons[ind+1].FullName.second;
        return true;
    }
    return false;
}



#ifndef __PROGTEST__
int main ( void )
{

    string outName, outSurname;
    int lo, hi;

    CPersonalAgenda b1;


    assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
    assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
    assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );

    assert ( b1 . getFirst ( outName, outSurname ) && outName == "John" && outSurname == "Miller" );
    assert ( b1 . getNext ( "John", "Miller", outName, outSurname )&& outName == "John" && outSurname == "Smith" );
    assert ( b1 . getNext ( "John", "Smith", outName, outSurname ) && outName == "Peter" && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );

    assert (b1 . setSalary ( "john", 32000 ) );
    assert (b1 . getSalary ( "john" ) ==  32000 );
    assert (b1 . getSalary ( "John", "Smith" ) ==  32000 );
    assert (b1 . getRank ( "John", "Smith", lo, hi ) && lo == 1 && hi == 1 );
    assert ( b1 . getRank ( "john", lo, hi ) && lo == 1 && hi == 1 );
    assert ( b1 . getRank ( "peter", lo, hi ) && lo == 0 && hi == 0 );
    assert ( b1 . getRank ( "johnm", lo, hi ) && lo == 2 && hi == 2 );
    assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
    assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
    assert ( b1 . getSalary ( "john" ) ==  35000 );
    assert ( b1 . getRank ( "John", "Smith", lo, hi ) && lo == 1 && hi == 2 );
    assert ( b1 . getRank ( "john", lo, hi ) && lo == 1 && hi == 2 );
    assert ( b1 . getRank ( "peter", lo, hi ) && lo == 0 && hi == 0 );
    assert ( b1 . getRank ( "johnm", lo, hi ) && lo == 1 && hi == 2 );
    assert ( b1 . changeName ( "peter", "James", "Bond" ) );
    assert ( b1 . getSalary ( "peter" ) ==  23000 );
    assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
    assert ( b1 . getFirst ( outName, outSurname ) && outName == "James" && outSurname == "Bond" );
    assert ( b1 . getNext ( "James", "Bond", outName, outSurname ) && outName == "John" && outSurname == "Miller" );
    assert ( b1 . getNext ( "John", "Miller", outName, outSurname ) && outName == "John" && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
    assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );

    assert ( b1 . getSalary ( "james" ) ==  23000 );
    assert ( b1 . getSalary ( "peter" ) ==  0 );
    assert ( b1 . del ( "james" ) );
    assert ( b1 . getRank ( "john", lo, hi ) && lo == 0 && hi == 1 );
    assert ( b1 . del ( "John", "Miller" ) );
    assert ( b1 . getRank ( "john", lo, hi ) && lo == 0 && hi == 0 );
    assert ( b1 . getFirst ( outName, outSurname ) && outName == "John" && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . del ( "john" ) );
    assert ( ! b1 . getFirst ( outName, outSurname ) );
    assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
    assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
    assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );


    CPersonalAgenda b2;
    assert ( ! b2 . getFirst ( outName, outSurname ) );
    assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
    assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
    assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );


    assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
    assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
    assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
    assert ( ! b2 . setSalary ( "joe", 90000 ) );

    assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
    assert ( b2 . getSalary ( "joe" ) ==  0 );
    assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
    assert ( ! b2 . getRank ( "joe", lo, hi ) );
    assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . del ( "Joe", "Black" ) );
    assert ( ! b2 . del ( "joe" ) );

    assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
    assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );



    assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
    assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
    assert ( b2 . del ( "Peter", "Smith" ) );

    assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
    assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
    assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
    assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
    assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
    assert ( ! b2 . setSalary ( "peter", 37000 ) );
    assert ( b2 . getSalary ( "peter" ) ==  0 );
    assert ( ! b2 . getRank ( "peter", lo, hi ) );
    assert ( ! b2 . del ( "Peter", "Smith" ) );
    assert ( ! b2 . del ( "peter" ) );
    assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( b2 . getSalary ( "peter" ) ==  40000 );

    CPersonalAgenda b3;
    assert ( b3 . add ( "A", "A", "sfs22", 125915 ) );

    assert ( b3 . add ( "a", "aaa", "ss", 30000 ) );
    assert ( b3 . add ( "~", "~~~", "asda", 30000 ) );


    assert ( b3 . add ( "~~~", "~", "sfs", 30000 ) );

    assert ( b3 . add ( "Depmahmhhara", "aJkjczffaejrbb", "oo", 9 ) );
    assert ( b3 . add ( "Depmahmhhar", "aJkjczffaejrbba", "oo2", 1 ) );
    assert ( b3 . add ( "n9LE1OZQoUcQaeMbJK#lUiQDQfcd#E","jmBWSrxF9Lo?qoe5sNEp1lAEPdLTgK", "kpXLbE4y?0!p0pD5jXHOcxZC1DHAjN", 33 ) );
    assert ( b3 . add ( "#u4W75qZtoOuDCL9wVV5wZqkqgOXIX","zIgrrblJ2G0F0s9zqsj!nF0DReJs1f", "foPnH8qUFfosNdKJC32N2ehPHZVfeK", 33 ) );
    assert ( b3 . add ( "ml01zwXPdpXrJxxi6Z?XA21HHxvpDC","1RNQGb!sTdJDwf2TpXIqI#hyGQY26q", "rW76AC8BUOEqg3x8JPSg5pdbQMSBBN", 33 ) );
    assert ( b3 . add ( "TUwNNYdKnZlTdtJCoge9o?nsa34Utu","u!bQPQEUoRGBxLX7bach9qimKkeD4y", "Xlz03dGuXWatmy49u7#wc8OkhoumRn", 33 ) );
    assert ( b3 . add ( "MEyatqeZL3INjX?bTu8QS!MulVIH9o","UIShLbyP0?HyLRiMU16RHMRi825Fx2", "TgxBo8CMMrMhPk0ZYHQSlkt4uoWmTi", 33 ) );
    assert ( b3 . add ( "bBoB2Cyrd?JPhn29aNDQuQ2NIj3txJ","Bz?P22hnhknS2u5Tq7vVKQzBs9Kjp8", "UQwSulIByROMwF8qlolG?XlzmEvYPM", 33 ) );
    assert ( b3 . add ( "VwrgeOrOcSuStSkq8xGi6F8e4hKp7m","3SLiYRWds0XMGdrSvof2x?usenBPCx", "3tcDD1ho6BbQcJVvrfKw79wqqAGTe8", 33 ) );
    assert ( b3 . add ( "d8qgAV9I92?aHbJrw2w8lt4HLj#e2b","c89udzd!7b37bz8NSsCdpQwhm8sjbi", "lddjyiIBhEC!zGywgfQKi6nHaAEsLF", 33 ) );
    assert ( b3 . add ( "CYJH87PFxY!1XAukWCpAaytp5vRwPq","3fbzO?vrGUeFJ255!OxrdzRwOJRt7w", "LXBOmeNHxgpCN0rHUsieLmDpKhYrAU", 33 ) );
    assert ( b3 . add ( "N?FdMRhonFwE7?tnFbHOhg2NvzUjSj","3sjxvXdDbr#xV4wcg3fNEoTv1cXKlC", "TdV4BfPEIQVFcGyAKErRfX50iU25tc", 33 ) );
    assert ( b3 . add ( "wcgg6JmKaUpYpstPU3haHoYzb8i60C","#lEfszOE#RnmCEGVhqNqqjEdIH?TC0", "kA!QFrcjVb0#qpCW!JbOZsXsxs1war", 33 ) );
    assert ( b3 . add ( "jkTk0nB5wl6jjjAN8Bj9c#qZrOhh!h","yiugvjuWbQ##2igpVbT4aV3qIh4Rq5", "YPdhXyrejuXgrMoA39DJ2DuSU41NIf", 33 ) );
    assert ( b3 . add ( "GvWJDI9WNfdzmx?CX3znCqTW?AP1ck","8KFTi8peT2mXoyjo26gqiIH1tFq8vv", "hr5N#dKmjrdvdrVmHLhO1rlxgH4yEn", 33 ) );
    assert ( b3 . add ( "TMEOoDRYQ2eTlha8vHIEixWwW43Opu","196uKhXq8Cf!jsjloGSY?3iWm7NeI4", "yy0uUzDEPA9XChcLsse#ee2o2diChS", 33 ) );
    assert ( b3 . add ( "uIdj2XJurnXp#mwcXOu3NA#DRZGZrO","HNlMWczuwQIg7FsqHe44#Gr798TFX!", "irMt5wvrST9n23Thgpo!glGAfEx09j", 33 ) );
    assert ( b3 . add ( "0gCBzvXWMFDJSszABIQRHY2cn9HLXF","UMMkaaF0ZespP#JcA!KdRg3GleDSPp", "my10I33cQPj86Y8C0vBzAfIqMTweAa", 33 ) );
    assert ( b3 . add ( "tMzjC9!sbS9kN28KrY7Umw2UPDA?H0","!2CA?6w!nxRjJu?F6rs1aEk2mZsO01", "CZQ6mQZKQ!#uiGQiaKBsyDZKt?zNMm", 33 ) );
    assert ( b3 . add ( "Cdaf9oYVYBWXX6tDbtaEOA8A#qyy5!","KufMCb0nZOQKyCER5Fa5!Ou9duoDSg", "CsBH45LShztX9ToAyh8Ab9dw5gQgJx", 33 ) );
    assert ( b3 . add ( "oaPRIHJimQJFCFlT8L25a31doTj61S","reT6WqAsAO?9iCDwky7!q#5gag1mcP", "6tUOoF4RZtsXrBpVXziTBBStHTBw7D", 33 ) );
    assert ( b3 . add ( "a0YWCaovTbRa08BeQnEZ95pQnWyOhs","fig36K5i5LlJM?HcdksJ?qDn6Q!srg", "MyqSrkrjumXFXyFrCKCUiC!LS8qTpK", 33 ) );
    assert ( b3 . add ( "118fJmr0yL!ifXGMb8kFSs#T6POjx3","VlSRsq5JdqjdAq26334ayLuvrn!8zw", "0hHF00XVy1?I6zYVuQNnQ?YavfnuaO", 33 ) );
    assert ( b3 . add ( "S0YnvNcfvC8u!1V0JcDksjkdjGky0m","aGa0VxAZEYoAhqo2d06G#nPhrYNDkD", "Qm?SbTcDHIoX8vammd!iK9vofOaVfm", 33 ) );
    assert ( b3 . add ( "lXAjCB4E4A!ik#FljUoi40gocl3e8#","qgWQrlgkR#KShVPN80uj8l#dzbprfm", "qvt!aMkgZ3fwJngl2b!jn9wmaVnrcs", 33 ) );
    assert ( b3 . add ( "DuPYuPyGXmx2I6eRg6Ufe7eAgeltxp","N2JpO34bzQnWIXRMDXIl5Oh9cocpJz", "EmocB46v5u?hdG7UixESIwtQuv6wME", 33 ) );
    assert ( b3 . add ( "XfQbih5dC0xC9A93jgoPYY!eDvCwSb","3CjGDrPyuenRHjgFapNq4yb5EFo638", "#Txip3z4qV?DzHOHbOWO4P!8KFCYBs", 33 ) );
    assert ( b3 . add ( "TAbfIq#7jpPjSdQuKR#vu4!v1J1qwp","IePJkk1jgazXjg02CzIAU3tTnhsbxO", "s7T9G5iudowC?IL0zam7C8ZVPa49do", 33 ) );
    assert ( b3 . add ( "KwlsuRmDaqTxUU5sHrvWnXRaIvcBrh","S4D3j0HwqHO?4vSYQpe??sXRuuayWt", "FBk?ruYYSdutbmQU!t?et?yePSARfj", 33 ) );
    assert ( b3 . add ( "?KMiI3EvQly?GAmkjnDjrXhR3YyqDD","ADdbNL5h9KsuKZWY97!Bfsnm#daxwD", "1Y84ZIERRAr#X3YIPWFQkK8AX7DZsZ", 33 ) );
    assert ( b3 . add ( "piKm!xWFdB8wySnlp47UHgsEGeB!3V","0h3Av4Xey2Fto5aBdqs!#1grsOxTNp", "DByt3TmPYLEq6Uj7iokCojswDL#0tN", 33 ) );
    assert ( b3 . add ( "dXdDf6kuI87bn0WwU6N6y1dQkGqjvM","YAy14FUbZr?VtlJeIs#imx?rbkXtvh", "5hIq8BWSCK?AtrL4xijxsvUsOXCye0", 33 ) );
    assert ( b3 . add ( "I97fp2GaJ#KIx31#Wmi6JCqqX5bosh","b0d85u0Auwx54WXVUH74CDvSVfKYu5", "5vU?rNsgaNCAHwjte52?WrDf!nmL!G", 33 ) );
    assert ( b3 . add ( "D44l4i0ko2ZSpvbzQhrGhdZKkY0wxZ","30SWaK60WjQK1775tK!NdjQ3V0QIja", "ub3aX3MQTxZy7Qs3IMAJmGS2yBRb90", 33 ) );
    assert ( b3 . add ( "ct47vOX7uFri3nYjdtXF5??K0waGA#","tCqmyL3kHkQ0sGceRhzBMrzL4o94Uw", "1d0gplThyprbeMHgSoneP2ycCpqyia", 33 ) );
    assert ( b3 . add ( "X!dMfu01CldVmkuVqb9GiMvGQXX8k5","#94cJ!xw01H5LVe8Dv9C3ednMT#yQj", "rOgkQPjca?5H6Ep#zUuvlkCoxd9wCP", 33 ) );
    assert ( b3 . add ( "GTqO38sc?sc3Z8ud8VZpg?CIo1Lnla","2TVfwOaOSa6U4SSnXN9L4eLtM0imav", "p2b!9xN9?u!6c3LWfxwd8phHJVwT#y", 33 ) );
    assert ( b3 . add ( "bmneljD1hBigum?78gttlqJtZhdi0a","G3mV7A6xqd0AljNmfKsA6DQEYDN2NA", "2jse4mE0LU3AjbJWpQwHdraVX0lxP1", 33 ) );
    assert ( b3 . add ( "XH#cL3qeQ30Iq9M1V1EewJxyrhlFE3","tqxqu8gM!ZCZuSV6IFVbLgK8E3eRvI", "HQ046h1cT21kQk4zez61CF7!AA5Fep", 33 ) );
    assert ( b3 . add ( "cO63FZ!w3TlR522Xq8ljWOOSPefHJl","XNZQDuEDQvl1bdS5282bhLPVqrZxZx", "JL?vqDPU6ueglgm3ab13c8DRQW?FgZ", 33 ) );
    assert ( b3 . add ( "4Px2aNvRwq!BxkJJcLM3BRY7xEQv?W","h2BGUBgchFsg8RsGpxe2nIGbC5HfoH", "1vyr4fS!j0EE8ziq5AN!p2HX5?PBd5", 33 ) );
    assert ( b3 . add ( "86qwlhD5gNUNgS!o87OK85BCRwCwZF","qXAGgNPJGWlpwr7xI4tlBog5SXqhic", "OBZc75PKCkvPAT7ufCmzNPPTJvGZEQ", 33 ) );
    assert (  b3.getSalary ( "A", "A" )==125915);

    // b3.Print();
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
