#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect {
public:
    CRect(double x,
          double y,
          double w,
          double h)
            : m_X(x),
              m_Y(y),
              m_W(w),
              m_H(h) {
    }

    friend ostream &operator<<(ostream &os,
                               const CRect &x) {
        return os << '(' << x.m_X << ',' << x.m_Y << ',' << x.m_W << ',' << x.m_H << ')';
    }

    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};

#endif /* __PROGTEST__ */

class Base{
public:
    Base(int id, const string &title, const CRect &absPos,const string &type,int posX,int posY,int posW,int posH )
            :m_id(id), m_title(title), m_absPos(absPos),m_type(type){
        Prefix="+-";
        m_PanelPrefix="";
        parr=NULL;
        flag_p=0;
        flag_s=0;
        selec=0;
        m_posXL=m_absPos.m_X;
        m_posYL=m_absPos.m_Y;;
        m_posWL=m_absPos.m_W;;
        m_posHL=m_absPos.m_H;;
    };
    void setPosRec(Base* itr){

        for (int i = 0; i < (int)itr->items.size() ; ++i) {
            setIdP(itr->items[i].get(),itr);
            setCoord(itr->items[i].get());
            if(getName(itr->items[i].get())=="Panel"){
                setPosRec(itr->items[i].get());
            }
        }

    }
    virtual void PanelPrefix(int i){
        m_PanelPrefix+="";
        for (int j = 0; j < i ; ++j) {
            m_PanelPrefix+="   ";
        }
    }
    virtual void PanelPrefixx(Base * s,int i) const {
        s->m_PanelPrefix+="";
        for (int j = 0; j < i ; ++j) {
            s->m_PanelPrefix+="   ";
        }
    }
    virtual void print(ostream &os) const = 0;
    string Prefix;
    string m_PanelPrefix;
    virtual int getId(Base* s) const=0;
    virtual void setId(Base* s) const=0;
    virtual void setPanel(Base* s,string pref)const{
        s->m_PanelPrefix=pref;

    }
    virtual void setIdP(Base* s,Base* src){
        s->m_posX=src->m_absPos.m_X;
        s->m_posY=src->m_absPos.m_Y;
        s->m_posW=src->m_absPos.m_W;
        s->m_posH=src->m_absPos.m_H;
    }

    virtual void setCoord(Base* s){
        s->m_absPos.m_X=s->m_posXL*s->m_posW+s->m_posX;
        s->m_absPos.m_Y=s->m_posYL*s->m_posH+s->m_posY;
        s->m_absPos.m_W=s->m_posWL*s->m_posW;
        s->m_absPos.m_H=s->m_posHL*s->m_posH;


    }
    virtual string getName(Base* s)const {
        return s->m_type;
    }
    virtual void setFlag(Base* s,string pref) const {
        s->Prefix=pref;
    }

    virtual shared_ptr<Base>  clone() const=0;

    int m_id;
    int flag_p;
    int flag_s;
    int selec;
    Base *parr;
    vector< shared_ptr<Base>> items;
    double  m_posX;
    double  m_posY;
    double  m_posW;
    double  m_posH;

    double  m_posXL;
    double  m_posYL;
    double  m_posWL;
    double  m_posHL;

protected:

    friend ostream & operator<<(ostream &s, const Base &k){
        k.print(s);
        return s;
    }


    string m_title;
    CRect m_absPos;
    string m_type;

};

class CWindow: public  Base{
public:
    CWindow( int id, const string &title, const CRect &absPos):Base(id,title,absPos,"Window",0,0,0,0){
    };

    CWindow(const CWindow& rhs): Base(rhs.m_id,rhs.m_title,rhs.m_absPos,"Window",0,0,0,0)
    {
        Copy(rhs);
    }
    vector< shared_ptr<Base>> all;

    void Copy (const CWindow & rhs)
    {
        m_id=rhs.m_id;
        m_title=rhs.m_title;
        m_absPos=rhs.m_absPos;
        m_type=rhs.m_type;
        m_posX=rhs.m_posX;
        m_posY=rhs.m_posY;
        m_posH=rhs.m_posH;
        m_posW=rhs.m_posW;
        flag_p=rhs.flag_p;
        flag_s=rhs.flag_s;
        Prefix=rhs.Prefix;
        size_t size=rhs.items.size();
        items.clear();
        for (int i = 0; i <(int) size; ++i) {
                add( * rhs.items[i]);
        }
    }
    CWindow& operator=(const CWindow& rhs){
        if (&rhs==this) return *this;
        Copy (rhs);
        return *this;
    };
    shared_ptr<Base>  clone() const
    {
        shared_ptr<Base> tmp = shared_ptr<Base>( new CWindow(*this));
        return tmp;
    }


    void show(ostream &os) const
    {

        for(auto itr=items.begin();itr!=items.end();itr++)
        {

            if(getName(itr->get())=="ComboBox"||getName(itr->get())=="Panel"){

                if(itr+1!=items.end()){
                    if(getName(itr->get())=="ComboBox")
                        setFlag(itr->get(),"|  +-");
                    else{
                        setPanel(itr->get(),"|  ");

                    }

                }
                else{
                    if(getName(itr->get())=="ComboBox"){
                        setFlag(itr->get(),"   +-");

                    }
                    else{
                        setPanel(itr->get(),"   ");

                    }


                }

                os<<"+- "<<*itr->get();
                if(getName(itr->get())=="ComboBox"){
                    setFlag(itr->get(),"+-");

                }
                else{
                    setPanel(itr->get(),"");

                }


            }
            else
                os<<"+- "<<*itr->get();
           // itr->get()->m_PanelPrefix="";


        }
    }


    virtual int getId(Base* s)const override{
        return  s->m_id;
    }
    virtual void setId(Base* s)const override{
        s->m_posX=m_absPos.m_X;
        s->m_posY=m_absPos.m_Y;
        s->m_posW=m_absPos.m_W;
        s->m_posH=m_absPos.m_H;
    }

    void setPosition(const CRect &absNew){
        m_absPos.m_X=absNew.m_X;
        m_absPos.m_Y=absNew.m_Y;
        m_absPos.m_W=absNew.m_W;
        m_absPos.m_H=absNew.m_H;
        for(auto itr=items.begin();itr!=items.end();itr++)
        {
            setId(itr->get());
            setCoord(itr->get());
            if(getName(itr->get())=="Panel"){
                setPosRec(itr->get());
            }
        }
    }
    void print(ostream &os) const{


        os<<"["<<m_id<<"] "<<m_type<<" \""<<m_title<<"\" "<<m_absPos<<endl;
        show(os);

    }

    CWindow& add(const Base &s){
        auto tmp = s.clone();
        setId(tmp.get());
        setCoord(tmp.get());
        items.push_back(tmp);
        all.push_back(tmp);
        if(tmp.get()->getName(tmp.get())=="Panel"){

            for (int i = 0; i < (int)tmp.get()->items.size() ; ++i) {
                setIdP(tmp.get()->items[i].get(),tmp.get());
                setCoord(tmp.get()->items[i].get());
                if(getName(tmp.get()->items[i].get())=="Panel"){
                    setPosRec(tmp.get());
                }
                all.push_back(tmp.get()->items[i]);
            }
        }

        return *this;
    }
    Base* search(int id){
        for(auto itr=all.begin();itr!=all.end();itr++)
        {   if(getId(itr->get())==id){
                return itr->get();
            }

        }

        return nullptr;
    }

};

class CPanel: public Base{
public:

    CPanel(int id,const CRect &relPos):Base(id,"",relPos,"Panel",0,0,0,0){};
    CPanel(const CPanel& rhs): Base(rhs.m_id,rhs.m_title,rhs.m_absPos,"Panel",0,0,0,0)
    {
        Copy(rhs);
    }
    void Copy (const CPanel & rhs)
    {
        m_id=rhs.m_id;
        m_title=rhs.m_title;
        m_absPos=rhs.m_absPos;
        m_type=rhs.m_type;
        m_posX=rhs.m_posX;
        m_posY=rhs.m_posY;
        m_posH=rhs.m_posH;
        m_posW=rhs.m_posW;
        m_posXL=rhs.m_posXL;
        m_posYL=rhs.m_posYL;
        m_posWL=rhs.m_posWL;
        m_posHL=rhs.m_posHL;
        flag_p=rhs.flag_p;
        flag_s=rhs.flag_s;
        Prefix=rhs.Prefix;
        size_t size=rhs.items.size();
        items.clear();
        for (int i = 0; i <(int) size; ++i) {
            add( * rhs.items[i]);
        }
    }
    CPanel& operator=(const CPanel& rhs){
        if (&rhs==this) return *this;
        Copy (rhs);
        return *this;
    };
    shared_ptr<Base>  clone() const
    {
        shared_ptr<Base> tmp = shared_ptr<Base>( new CPanel(*this));
        return tmp;
    }
    virtual int getId(Base* s)const override{
        return  s->m_id;
    }
    virtual void setId(Base* s)const override{

        s->m_posX=m_absPos.m_X;
        s->m_posY=m_absPos.m_Y;
        s->m_posW=m_absPos.m_W;
        s->m_posH=m_absPos.m_H;
    }
    void show(ostream &os) const{
        for(auto itr=items.begin();itr!=items.end();itr++)
        {
            if(getName(itr->get())=="Panel") {

                if (itr + 1 != items.end()) {
                    itr->get()->m_PanelPrefix = m_PanelPrefix+"|  " + "";
                }
                else
                    itr->get()->m_PanelPrefix = m_PanelPrefix+"   " +"" ;
            }

            if(getName(itr->get())=="ComboBox"){
                if(itr+1!=items.end()){
                    setFlag(itr->get(),m_PanelPrefix+"|  +-");
                }
                else
                    setFlag(itr->get(),m_PanelPrefix+"   +-");

                os<<m_PanelPrefix<<"+- "<<*itr->get();
                setFlag(itr->get(),"+-");
            }
            else{

                os<<m_PanelPrefix<<"+- "<<*itr->get();

            }

            itr->get()->m_PanelPrefix=m_PanelPrefix;


        }

    }
    void print(ostream &os) const override
    {
        os<<"["<<m_id<<"] "<<m_type<<" "<<m_absPos<<endl;
        show(os);

    }
    int deeper=1;
    CPanel& add(const Base &s){
        auto tmp = s.clone();
        setId(tmp.get());
        setCoord(tmp.get());
        items.push_back(tmp);
        if(tmp.get()->getName(tmp.get())=="Panel") {
            setPosRec(tmp.get());
        }
        return *this;
    }
};

class CButton: public virtual Base{
public:
    virtual void showCombo(ostream &os, int flagPipe, int flagSearch ) const{};


    CButton(int id,const CRect &relPos, const string &name):Base(id,name,relPos,"Button",0,0,0,0){}
    shared_ptr<Base>  clone() const
    {
        shared_ptr<Base> tmp = shared_ptr<Base>( new CButton(*this));
        return tmp;
    }
    virtual int getId(Base* s)const override{
        return  s->m_id;
    }
    virtual void setId(Base* s)const override{}
    void print(ostream &os) const{

        os<<"["<<m_id<<"] "<<m_type<<" \""<<m_title<<"\" "<<m_absPos<<endl;


    }

};

class CInput: public  virtual Base{
public:
    virtual void showCombo(ostream &os, int flagPipe, int flagSearch ) const{};


    virtual void setId(Base* s)const override{
        s->m_posX=99;
    }
    CInput(int id,const CRect &relPos,const string &value):Base(id,value,relPos,"Input",0,0,0,0){};
    void print(ostream &os) const override{
        os<<"["<<m_id<<"] "<<m_type<<" \""<<m_title<<"\" "<<m_absPos<<endl;

    }
    virtual int getId(Base* s)const override{
        return  s->m_id;
    }
    shared_ptr<Base>  clone() const
    {
        shared_ptr<Base> tmp = shared_ptr<Base>( new CInput(*this));
        return tmp;
    }
    string getValue(){
        return m_title;
    }
    void setValue(const string &c){
        m_title=c;
    }
    //string setValue

};

class CLabel: public virtual Base{
public:
    virtual void showCombo(ostream &os, int flagPipe, int flagSearch ) const{};


    virtual void setId(Base* s)const override{
        s->m_posX=99;
    }
    virtual int getId(Base* s)const override{
        return  s->m_id;
    }
    CLabel(int id,const CRect &relPos,const string &label):Base(id,label,relPos,"Label",0,0,0,0){};
    void print(ostream &os) const{
        os<<"["<<m_id<<"] "<<m_type<<" \""<<m_title<<"\" "<<m_absPos<<endl;


    }
    shared_ptr<Base>  clone() const
    {
        shared_ptr<Base> tmp = shared_ptr<Base>( new CLabel(*this));
        return tmp;
    }
};

class CComboBox: public virtual Base{
public:
    vector<string > subs;

    virtual void setId(Base* s)const override{
        s->m_posX=99;
    }
    CComboBox(int id,const CRect &relPos):Base(id,"ComboBox",relPos,"ComboBox",0,0,0,0){};
    virtual int getId(Base* s)const override{
        return  s->m_id;
    }
    shared_ptr<Base>  clone() const
    {
        shared_ptr<Base> tmp = shared_ptr<Base>( new CComboBox(*this));
        return tmp;
    }
    void show(ostream &os) const{

        for(auto itr=subs.begin();itr!=subs.end();itr++)
        {

            if(*itr==subs[getSelected()]){

                os <<Prefix<<">"<< *itr<<"<"<<endl;

            }
            else{
                os <<Prefix<<" "<< *itr<<endl;

            }
        }
    }
    void print(ostream &os) const override
    {
        os<<"["<<m_id<<"] "<<m_type<<" "<<m_absPos<<endl;
        show(os);

    }

    CComboBox& add(const string &s){
        if(subs.empty())
            setSelected(0);
        subs.push_back(s);
        return *this;

    }
    string m_selected;
    void setSelected(int sel){
        selec=sel;
    }
    int getSelected() const {
        return selec;
    }


};

// output operators

#ifndef __PROGTEST__

template<typename _T>
string toString(const _T &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main(void) {
    assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CButton ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
    assert ( sizeof ( CInput ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
    assert ( sizeof ( CLabel ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
    CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
    a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
    a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
    a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
    a . add ( CPanel ( 12, CRect ( 0.1, 0.3, 0.8, 0.7 ) ) . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) ) );
    assert ( toString ( a ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
             "+- [12] Panel (70,154,480,336)\n"
             "   +- [20] ComboBox (118,254.8,384,33.6)\n"
             "      +->Karate<\n"
             "      +- Judo\n"
             "      +- Box\n"
             "      +- Progtest\n" );
    CWindow b = a;
    assert ( toString ( *b . search ( 20 ) ) ==
             "[20] ComboBox (118,254.8,384,33.6)\n"
             "+->Karate<\n"
             "+- Judo\n"
             "+- Box\n"
             "+- Progtest\n" );
    assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
    dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
    assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
    dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
    CPanel & p = dynamic_cast<CPanel &> ( *b . search ( 12 ) );
    p . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
    assert ( toString ( b ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
             "+- [12] Panel (70,154,480,336)\n"
             "   +- [20] ComboBox (118,254.8,384,33.6)\n"
             "   |  +- Karate\n"
             "   |  +- Judo\n"
             "   |  +- Box\n"
             "   |  +->Progtest<\n"
             "   +- [21] ComboBox (118,322,384,33.6)\n"
             "      +->PA2<\n"
             "      +- OSY\n"
             "      +- Both\n" );
    assert ( toString ( a ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
             "+- [12] Panel (70,154,480,336)\n"
             "   +- [20] ComboBox (118,254.8,384,33.6)\n"
             "      +->Karate<\n"
             "      +- Judo\n"
             "      +- Box\n"
             "      +- Progtest\n" );
    assert ( toString ( p ) ==
             "[12] Panel (70,154,480,336)\n"
             "+- [20] ComboBox (118,254.8,384,33.6)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (118,322,384,33.6)\n"
             "   +->PA2<\n"
             "   +- OSY\n"
             "   +- Both\n" );
    b . setPosition ( CRect ( 20, 30, 640, 520 ) );
    assert ( toString ( b ) ==
             "[0] Window \"Sample window\" (20,30,640,520)\n"
             "+- [1] Button \"Ok\" (84,446,192,52)\n"
             "+- [2] Button \"Cancel\" (404,446,192,52)\n"
             "+- [10] Label \"Username:\" (84,82,128,52)\n"
             "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
             "+- [12] Panel (84,186,512,364)\n"
             "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
             "   |  +- Karate\n"
             "   |  +- Judo\n"
             "   |  +- Box\n"
             "   |  +->Progtest<\n"
             "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
             "      +->PA2<\n"
             "      +- OSY\n"
             "      +- Both\n" );
    p . add ( p );

    assert ( toString ( p ) ==
             "[12] Panel (84,186,512,364)\n"
             "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
             "|  +->PA2<\n"
             "|  +- OSY\n"
             "|  +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "   |  +- Karate\n"
             "   |  +- Judo\n"
             "   |  +- Box\n"
             "   |  +->Progtest<\n"
             "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "      +->PA2<\n"
             "      +- OSY\n"
             "      +- Both\n" );
    p . add ( p );
    assert ( toString ( p ) ==
             "[12] Panel (84,186,512,364)\n"
             "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
             "|  +->PA2<\n"
             "|  +- OSY\n"
             "|  +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "|  |  +- Karate\n"
             "|  |  +- Judo\n"
             "|  |  +- Box\n"
             "|  |  +->Progtest<\n"
             "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "|     +->PA2<\n"
             "|     +- OSY\n"
             "|     +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "   |  +- Karate\n"
             "   |  +- Judo\n"
             "   |  +- Box\n"
             "   |  +->Progtest<\n"
             "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "   |  +->PA2<\n"
             "   |  +- OSY\n"
             "   |  +- Both\n"
             "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
             "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
             "      |  +- Karate\n"
             "      |  +- Judo\n"
             "      |  +- Box\n"
             "      |  +->Progtest<\n"
             "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
             "         +->PA2<\n"
             "         +- OSY\n"
             "         +- Both\n" );
    p . add ( p );
   // cout<<p<<endl;

    assert ( toString ( p ) ==
             "[12] Panel (84,186,512,364)\n"
             "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
             "|  +->PA2<\n"
             "|  +- OSY\n"
             "|  +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "|  |  +- Karate\n"
             "|  |  +- Judo\n"
             "|  |  +- Box\n"
             "|  |  +->Progtest<\n"
             "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "|     +->PA2<\n"
             "|     +- OSY\n"
             "|     +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "|  |  +- Karate\n"
             "|  |  +- Judo\n"
             "|  |  +- Box\n"
             "|  |  +->Progtest<\n"
             "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "|  |  +->PA2<\n"
             "|  |  +- OSY\n"
             "|  |  +- Both\n"
             "|  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
             "|     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
             "|     |  +- Karate\n"
             "|     |  +- Judo\n"
             "|     |  +- Box\n"
             "|     |  +->Progtest<\n"
             "|     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
             "|        +->PA2<\n"
             "|        +- OSY\n"
             "|        +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "   |  +- Karate\n"
             "   |  +- Judo\n"
             "   |  +- Box\n"
             "   |  +->Progtest<\n"
             "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "   |  +->PA2<\n"
             "   |  +- OSY\n"
             "   |  +- Both\n"
             "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
             "   |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
             "   |  |  +- Karate\n"
             "   |  |  +- Judo\n"
             "   |  |  +- Box\n"
             "   |  |  +->Progtest<\n"
             "   |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
             "   |     +->PA2<\n"
             "   |     +- OSY\n"
             "   |     +- Both\n"
             "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
             "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
             "      |  +- Karate\n"
             "      |  +- Judo\n"
             "      |  +- Box\n"
             "      |  +->Progtest<\n"
             "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
             "      |  +->PA2<\n"
             "      |  +- OSY\n"
             "      |  +- Both\n"
             "      +- [12] Panel (208.928,425.148,262.144,124.852)\n"
             "         +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
             "         |  +- Karate\n"
             "         |  +- Judo\n"
             "         |  +- Box\n"
             "         |  +->Progtest<\n"
             "         +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
             "            +->PA2<\n"
             "            +- OSY\n"
             "            +- Both\n" );

     assert ( toString ( b ) ==
              "[0] Window \"Sample window\" (20,30,640,520)\n"
              "+- [1] Button \"Ok\" (84,446,192,52)\n"
              "+- [2] Button \"Cancel\" (404,446,192,52)\n"
              "+- [10] Label \"Username:\" (84,82,128,52)\n"
              "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
              "+- [12] Panel (84,186,512,364)\n"
              "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
              "   |  +- Karate\n"
              "   |  +- Judo\n"
              "   |  +- Box\n"
              "   |  +->Progtest<\n"
              "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
              "   |  +->PA2<\n"
              "   |  +- OSY\n"
              "   |  +- Both\n"
              "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
              "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
              "   |  |  +- Karate\n"
              "   |  |  +- Judo\n"
              "   |  |  +- Box\n"
              "   |  |  +->Progtest<\n"
              "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
              "   |     +->PA2<\n"
              "   |     +- OSY\n"
              "   |     +- Both\n"
              "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
              "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
              "   |  |  +- Karate\n"
              "   |  |  +- Judo\n"
              "   |  |  +- Box\n"
              "   |  |  +->Progtest<\n"
              "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
              "   |  |  +->PA2<\n"
              "   |  |  +- OSY\n"
              "   |  |  +- Both\n"
              "   |  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
              "   |     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
              "   |     |  +- Karate\n"
              "   |     |  +- Judo\n"
              "   |     |  +- Box\n"
              "   |     |  +->Progtest<\n"
              "   |     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
              "   |        +->PA2<\n"
              "   |        +- OSY\n"
              "   |        +- Both\n"
              "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
              "      +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
              "      |  +- Karate\n"
              "      |  +- Judo\n"
              "      |  +- Box\n"
              "      |  +->Progtest<\n"
              "      +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
              "      |  +->PA2<\n"
              "      |  +- OSY\n"
              "      |  +- Both\n"
              "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
              "      |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
              "      |  |  +- Karate\n"
              "      |  |  +- Judo\n"
              "      |  |  +- Box\n"
              "      |  |  +->Progtest<\n"
              "      |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
              "      |     +->PA2<\n"
              "      |     +- OSY\n"
              "      |     +- Both\n"
              "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
              "         +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
              "         |  +- Karate\n"
              "         |  +- Judo\n"
              "         |  +- Box\n"
              "         |  +->Progtest<\n"
              "         +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
              "         |  +->PA2<\n"
              "         |  +- OSY\n"
              "         |  +- Both\n"
              "         +- [12] Panel (208.928,425.148,262.144,124.852)\n"
              "            +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
              "            |  +- Karate\n"
              "            |  +- Judo\n"
              "            |  +- Box\n"
              "            |  +->Progtest<\n"
              "            +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
              "               +->PA2<\n"
              "               +- OSY\n"
              "               +- Both\n" );
     assert ( toString ( a ) ==
              "[0] Window \"Sample window\" (10,10,600,480)\n"
              "+- [1] Button \"Ok\" (70,394,180,48)\n"
              "+- [2] Button \"Cancel\" (370,394,180,48)\n"
              "+- [10] Label \"Username:\" (70,58,120,48)\n"
              "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
              "+- [12] Panel (70,154,480,336)\n"
              "   +- [20] ComboBox (118,254.8,384,33.6)\n"
              "      +->Karate<\n"
              "      +- Judo\n"
              "      +- Box\n"
              "      +- Progtest\n" );
    /* */
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */