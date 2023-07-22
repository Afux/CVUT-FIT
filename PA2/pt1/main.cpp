#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>


#endif /* __PROGTEST__ */

//HELEPR FUNCS
using namespace std;
vector<int32_t>Fibs;

/*
 * FUNCS for UTF->FIB
 */
bool WriteByte(const vector<uint_fast64_t> &kk, const char *out,ofstream &stream) {


    if(stream.is_open()){
        int size = kk.size();
        for (int i = 0; i < size; ++i) {
            stream.write((char*) &kk[i], 1);
            if (!stream.good()) {
                stream.close();
                return false;
            }
        }
    } else
        return false;
    return true;
}
void  GenFibsF(int n, vector<int32_t>&fibs){

    if(fibs.size()==0){
        fibs.push_back(1);
        fibs.push_back(2);
    }
    for (int i = 2; i < n; i++){
        int tmp = fibs[i - 1] + fibs[i - 2];
        fibs.push_back(tmp);
    }
}
void GenFibs(int n, vector<int32_t>&fibs, int &ind){
    int i=0;
    if(fibs.size()==0){
        fibs.push_back(1);
        fibs.push_back(2);
        i=0;
    }
    else
        i=fibs.size()-2;

    int curr=fibs[fibs.size()-1];
    while (curr<=n){
        curr=fibs[i]+fibs[i+1];
        i++;
        fibs.push_back(curr);
    }

    int k=0;
    while (fibs[k]<=n)
        k++;

    ind=k-1;
}
unsigned int MyPow(int index){
    int tmp=1;
    if(index==0)
        return 1;
    for (int i = 0; i < index; ++i) {
        tmp*=2;
    }
    return tmp;
}
int MyFib(int n, const char *out,bool &err,long long int &seq,int &mycnt,vector<uint_fast64_t> &bajts,ofstream &stream){
    if(bajts.size()==4){
        if(!WriteByte(bajts, out,stream)){
            err= true;
            return 0;
        }
        bajts.clear();
    }

    int ind=0;
    int tmpcnt=1;
    GenFibs(n,Fibs,ind);
    int piww=MyPow(ind);
    unsigned long long int pls=MyPow(ind+1);

    for (int i = ind; i >=0 ; i--) {
        if(n>=Fibs[i]){
            pls=pls|piww;
            n-=Fibs[i];
        }
        tmpcnt++;
        piww/=2;
    }

    int size = tmpcnt;
    pls=pls<<mycnt;
    seq=seq|pls;
    mycnt+=size;



    if(mycnt>=8){
        while (mycnt>=8){
            unsigned char h = 0;
            int tmp = 128;

            for (int l = 7; 0 <= l; l--) {
                int push = (seq >> l) & 0x01;
                if (push == 1)
                    h += tmp;
                tmp /= 2;
            }
            seq=seq>>8;
            mycnt-=8;
            bajts.push_back(h);
        }
    }
    return 1;
}
int ReadByte(ifstream &stream, int &i) {
    unsigned char x=0;
    stream.seekg (i);
    if (!stream.good()) {
        return -1;
    }
    stream.read((char*)&x,1);
    if (!stream.good()) {
        return -1;
    }

    int j = 7;
    int byte_counter = 0;
    while ((((x >> j) & 0x01) != 0)) {
        byte_counter++;
        j--;
    }

    if(byte_counter>4||byte_counter==1)
        return -1;

    if (byte_counter != 0) {
        stream.seekg (i);
        if (!stream.good()) {
            return -1;
        }
        char * MYFILE=new char [byte_counter];
        stream.read(MYFILE,byte_counter);
        if (!stream.good()) {
            delete[] MYFILE;
            return -1;
        }
        int index = byte_counter + 1;
        int checker=0;
        unsigned int lol=0;
        int tmpcnt=1;
        for (int k = 0; k < byte_counter; ++k) {
            unsigned char y=0;
            if(k<byte_counter)
                y=MYFILE[k];
            else{
                delete[] MYFILE;
                return -1;
            }



            vector<int>checkForm;
            vector<int>checkPat={1,0};
            if(k!=0){
                for (int l = 7; 6 <= l; l--) {
                    int push = (y >> l) & 0x01;
                    checkForm.push_back(push);

                }
                if(checkForm!=checkPat){
                    delete[] MYFILE;
                    return -1;
                }
                checker++;

            }
            y = y << index;

            unsigned int tmp=0;
            if(k==0){
                lol=y;
                switch (byte_counter) {
                    case 2:
                        lol=lol<<3;
                        break;
                    case 3:
                        lol=lol<<8;

                        break;
                    case 4:
                        lol=lol<<13;

                        break;
                }
                tmpcnt++;
            }
            else{
                if(tmpcnt==2){
                    switch (byte_counter) {
                        case 2:
                            tmp=y>>2;
                            break;
                        case 3:
                            tmp=y<<4;

                            break;
                        case 4:
                            tmp=y<<10;

                            break;
                    }
                    tmpcnt++;
                }
                else if(tmpcnt==3){
                    switch (byte_counter) {
                        case 3:
                            tmp=y>>2;

                            break;
                        case 4:
                            tmp=y<<4;

                            break;
                    }
                    tmpcnt++;
                }
                else if(tmpcnt==4){
                    tmp=y>>2;
                    tmpcnt++;
                }
            }
            lol=lol|tmp;
            index = 2;
        }

        if(checker!=byte_counter-1){
            delete[] MYFILE;
            return -1;
        }
        delete[] MYFILE;
        i += byte_counter;
        return lol;
    }

    i++;
    int ret = static_cast<int>(x);
    return ret;
}
bool utf8ToFibonacci(const char *inFile, const char *outFile) {
   // remove(outFile);

    long long int seq=0;
    int mycnt=0;
    vector<uint_fast64_t>bajts;
    bool Err_flag= false;

    ifstream stream;
    ofstream stream1;
    stream1.open(outFile, ios::out | ios::binary);
    stream.open(inFile, ios::in | ios::binary);
    if(!stream1.is_open()){
        stream.close();
        return false;
    }
    if (stream.is_open()){
        stream.seekg(0, ios::end);
        long int size = stream.tellg();
        if (!stream.good()) {
            stream1.close();
            stream.close();
            return false;
        }
        for (int i = 0; i < size;) {
            int tmp = ReadByte(stream, i);
            if(tmp==-1||tmp>1114111||Err_flag){
                stream1.close();
                stream.close();
                return false;
            }
            MyFib(tmp+1,outFile,Err_flag,seq,mycnt,bajts,stream1);
        }
        if(seq!=0){
            unsigned char ret=seq;
            bajts.push_back(ret);
        }
        if(bajts.size()>0){
            if(!WriteByte(bajts,outFile,stream1)){
                stream.close();
                return false;
            }

        }
        stream1.close();
        stream.close();
    }
    else{
        return false;
    }

    return true;
}

/*
 * FUNCS for FIB->UTF
 */
int HowManyBytes(int ret, vector<unsigned char > &bites) {

    if (ret < 128)
        return 1;
    else if (ret > 127 && ret < 2048) {
        int tmpRETS=ret;
        unsigned char bt=192;
        bt=bt|(tmpRETS>>(11-5));
        bites.push_back(bt);
        bt=128;

        unsigned char secb=0;
        int poww=128;
        for (int l = 7; 0 <= l; l--) {
            int push = ((tmpRETS<<2) >> l) & 0x01;
            if(push==0)
                secb= secb|0;
            else
                secb= secb|poww;
            poww/=2;
        }
        bt=bt|(secb>>2);
        bites.push_back(bt);
        return 2;
    }
    else if (ret > 2047 && ret < 65536) {
        int tmpRETS=ret;
        unsigned char bt=224;
        bt=bt|(tmpRETS>>(16-4));
        bites.push_back(bt);
        bt=128;

        unsigned char secb=0;
        int poww=32;
        for (int l = 11; 6 <= l; l--) {
            int push = ((tmpRETS) >> l) & 0x01;

            if(push==0)
                secb= secb|0;
            else
                secb= secb|poww;
            poww/=2;
        }

        bt=bt|(secb);
        bites.push_back(bt);

        bt=128;
        secb=0;
        poww=32;
        for (int l = 5; 0 <= l; l--) {
            int push = ((tmpRETS) >> l) & 0x01;

            if(push==0)
                secb= secb|0;
            else
                secb= secb|poww;
            poww/=2;
        }
        bt=bt|(secb);
        bites.push_back(bt);
        return 3;
    }
    else if (ret > 65535 && ret < 1114112 ) {
        int tmpRETS=ret;
        unsigned char bt=240;
        bt=bt|(tmpRETS>>(21-3));
        bites.push_back(bt);


        bt=128;
        unsigned char secb=0;
        int poww=32;
        for (int l = 17; 12 <= l; l--) {
            int push = ((tmpRETS) >> l) & 0x01;

            if(push==0)
                secb= secb|0;
            else
                secb= secb|poww;
            poww/=2;
        }

        bt=bt|(secb);
        bites.push_back(bt);


        bt=128;
        secb=0;
        poww=32;
        for (int l = 11; 6 <= l; l--) {
            int push = ((tmpRETS) >> l) & 0x01;

            if(push==0)
                secb= secb|0;
            else
                secb= secb|poww;
            poww/=2;
        }

        bt=bt|(secb);
        bites.push_back(bt);

        bt=128;
        secb=0;
        poww=32;
        for (int l = 5; 0 <= l; l--) {
            int push = ((tmpRETS) >> l) & 0x01;

            if(push==0)
                secb= secb|0;
            else
                secb= secb|poww;
            poww/=2;
        }
        bt=bt|(secb);
        bites.push_back(bt);
        return 4;
    }
    return 5;
}
//Ok
bool WriteByteFib(const vector<int> &BAJT, const char *out,ofstream& stream  ) {



    if (stream.is_open()) {
        int size=BAJT.size();
        for(int j=0; j<size;j++) {
            vector<unsigned char > tmpBytes;
            int bytes = HowManyBytes(BAJT[j], tmpBytes);
            switch (bytes) {
                case 1:
                    stream.write((char *) &BAJT[j], 1);
                    if(!stream.good()){
                        stream.close();
                        return false;
                    }
                    break;
                case 2:

                    for (int i = 0; i < 2; ++i) {
                        stream.write((char *) &tmpBytes[i], 1);
                        if(!stream.good()){
                            stream.close();
                            return false;
                        }
                    }
                    break;
                case 3:
                    for (int i = 0; i < 3; ++i) {
                        stream.write((char *) &tmpBytes[i], 1);
                        if(!stream.good()){
                            stream.close();
                            return false;
                        }
                    }
                    break;
                case 4:
                    for (int i = 0; i < 4; ++i) {
                        stream.write((char *) &tmpBytes[i], 1);
                        if(!stream.good()){
                            stream.close();
                            return false;
                        }
                    }
                    break;
                default:
                    stream.close();
                    return false;
            }
        }

    }
    else{
        return false;
    }


    return true;
}
//OK
int ConvertFib(const unsigned int &rs,const int &size) {

    vector<int32_t> Myf;
    GenFibsF(size+3,Myf);
    int ret2=0;
    int multiply = size - 1;
    for (int i = 0; i < size; i++, multiply--) {

        ret2+=(((rs>>i)&0x01)*Myf[multiply]);
    }
    return ret2 - 1;
}
//OK
int ReadByteFIB( int n,  const char *outFile, bool &flag,unsigned int &chbajt,int &prevCh,int &byteSize,vector<int> &BAJTS,const unsigned char &ytmp,ofstream & stream) {
    if(BAJTS.size()==4){
        if( !WriteByteFib(BAJTS, outFile,stream)){
            flag= true;
            return 0;
        }
        BAJTS.clear();
    }


    unsigned char y=ytmp;


    unsigned char x=0;
    int pow=1;
    for (int l = 7; 0 <= l; l--) {
        int push = (y >> l) & 0x01;
        if(push==1)
            x=x|pow;
        pow*=2;
    }
    int currCh=0;
    for (int i = 0; i < 8; ++i) {
        currCh=((x>>(7-i))&0x01);
        if (currCh == 1 && prevCh == 1) {

            int ret = ConvertFib(chbajt,byteSize);
            BAJTS.push_back(ret);

            byteSize=0;
            chbajt=0;
            prevCh=0;
            continue;
        }
        chbajt=chbajt<<1;
        chbajt=chbajt|((x>>(7-i))&0x01);
        byteSize++;
        prevCh=currCh;
    }

    return 1;
}
bool fibonacciToUtf8(const char *inFile, const char *outFile) {
    //remove(outFile);
    vector<int> BAJTS;

    int byteSize=0;
    unsigned int charbajt=0;
    bool ErrFlag = false;
    int prevCh=0;


    ifstream stream;
    stream.open(inFile, ios::in | ios::binary);
    ofstream stream1;
    stream1.open(outFile,ios::out | ios::binary );
    if(!stream1.is_open()){

        stream.close();
        return false;
    }
    if(stream.is_open())
    {
        stream.seekg(0, ios::end);
        long int size = stream.tellg();
        if (!stream.good()) {
            stream.close();
            return false;
        }


        char *MYFILE=new char[1];
        for (int i = 0; i < size; i++) {

            if(ErrFlag)
                break;
            stream.seekg(i);
            if (!stream.good()) {
                delete[]MYFILE;
                stream.close();
                return false;
            }
            unsigned char y;
            if(size-i>4){


                  delete[]MYFILE;

                MYFILE=new char[4];
                stream.read(MYFILE,4);
                if (!stream.good()) {
                    delete[]MYFILE;
                    stream.close();
                    return false;
                }
                for (int j = 0; j  <4 ; ++j) {
                    ReadByteFIB( i++, outFile, ErrFlag,charbajt,prevCh,byteSize,BAJTS,MYFILE[j],stream1);
                }
                i-=1;

            }
            else{
                stream.read((char *) &y, 1);
                if (!stream.good()) {
                    delete[]MYFILE;
                    stream.close();
                    return false;
                }
                ReadByteFIB(i, outFile, ErrFlag,charbajt,prevCh,byteSize,BAJTS,y,stream1);
            }
        }

            delete[]MYFILE;
        if(charbajt!=0){
            ErrFlag= true;
        }

        if(BAJTS.size()>0){
            if(! WriteByteFib(BAJTS,outFile,stream1))
                ErrFlag= true;
        }

        if (ErrFlag) {
            stream.close();
            return false;
        }
        stream.close();
    } else
        return false;

    return true;
}
#ifndef __PROGTEST__
bool identicalFiles(const char *file1, const char *file2) {
    bool flag = false;
    ifstream stream1(file1, ios::in | ios::binary);
    ifstream stream2(file2, ios::in | ios::binary);
    if (!stream1.good()) {
        return false;
    }
    if (!stream2.good()) {
        return false;
    }
    stream1.seekg(0, ios::end);
    stream2.seekg(0, ios::end);
    long int file_size1 = stream1.tellg();
    long int file_size2 = stream2.tellg();
    if (file_size1 != file_size2) {
        stream1.close();
        if (!stream1.good()) {
            return false;
        }
        stream2.close();
        if (!stream2.good()) {
            return false;
        }
        return false;
    }


    for (int i = 0; i < file_size1; i++) {
        stream1.seekg(i);
        unsigned char x;
        stream1.read((char *) &x, 1);
        stream2.seekg(i);
        unsigned char y;
        stream2.read((char *) &y, 1);
        if (x != y) {
            flag = true;
            break;
        }


    }

    stream1.close();
    if (!stream1.good()) {

        return false;
    }
    stream2.close();
    if (!stream2.good()) {
        return false;
    }
    if (!flag)
        return true;
    else
        return false;
}
int main(int argc, char *argv[]) {

    assert (fibonacciToUtf8("example/src_6.fib", "output.utf8")
            && identicalFiles("output.utf8", "example/dst_6.utf8"));
    assert (fibonacciToUtf8("example/src_7.fib", "output.utf8")
            && identicalFiles("output.utf8", "example/dst_7.utf8"));
    assert (fibonacciToUtf8("example/src_8.fib", "output.utf8")
            && identicalFiles("output.utf8", "example/dst_8.utf8"));
    assert (fibonacciToUtf8("example/src_9.fib", "output.utf8")
            && identicalFiles("output.utf8", "example/dst_9.utf8"));
    assert (fibonacciToUtf8("example/src_10.fib", "output.utf8")
            && identicalFiles("output.utf8", "example/dst_10.utf8"));
    assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );

    assert (utf8ToFibonacci("example/src_0.utf8", "output.fib"));

    assert( fibonacciToUtf8("example/src_6.fib", "output.utf8"));

    assert (utf8ToFibonacci("example/src_0.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_0.fib"));

    assert (utf8ToFibonacci("example/src_1.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_1.fib"));
    assert (utf8ToFibonacci("example/src_2.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_2.fib"));
    assert (utf8ToFibonacci("example/src_3.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_3.fib"));
    assert (utf8ToFibonacci("example/src_4.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_4.fib"));



    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */