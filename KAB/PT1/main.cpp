#ifndef __PROGTEST__
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

#endif /* __PROGTEST__ */

unsigned char text[EVP_MAX_MD_SIZE];
char hashFunction[] = "sha512";  // zvolena hashovaci funkce ("sha1", "md5", ...)
EVP_MD_CTX * ctx;  // struktura kontextu
const EVP_MD * type; // typ pouzite hashovaci funkce
unsigned char hashS[EVP_MAX_MD_SIZE]; // char pole pro hash - 64 bytu (max pro sha 512)
unsigned int length;  // vysledna delka hashe
bool first= true;
void RandHash(){

    if(first){

        RAND_bytes(text,64);
    }
    OpenSSL_add_all_digests();

    /* Inicializace OpenSSL hash funkci */

    /* Zjisteni, jaka hashovaci funkce ma byt pouzita */
    type = EVP_get_digestbyname(hashFunction);

    /* Pokud predchozi prirazeni vratilo -1, tak nebyla zadana spravne hashovaci funkce */
    if (!type) {
        printf("Hash %s neexistuje.\n", hashFunction);
    }
    ctx = EVP_MD_CTX_new(); // create context for hashing
    EVP_DigestInit_ex(ctx, type, NULL);// context setup for our hash type
    EVP_DigestUpdate(ctx, text, EVP_MAX_MD_SIZE); // feed the message in
    EVP_DigestFinal_ex(ctx, hashS, &length);// get the hash
    EVP_MD_CTX_free(ctx); // destroy the context

}
string HextoBin(int i) {

    std::stringstream ss;
    ss<< std::hex << i ;
    std::string res ( ss.str() );
    if(res.size()==1){
        char temp=res[0];
        res[0]='0';
        res[1]=temp;
    }
    return res;
}
bool ValidHash(int bits,string & outputMessage, string & outputHash){
    int tmp=0;
    bool valid= false;
    bool firstBit= false;
    outputMessage.clear();
    outputHash.clear();
    for (unsigned int i = 0; i < 64; i++){
        if(tmp>=bits)
            valid= true;
        if(tmp<bits&&firstBit== true){
            return false;
        }
        string hashBin= HextoBin(hashS[i]);
        string messBin= HextoBin(text[i]);
        switch (hashBin[0]) {
            case '0':
                if(!firstBit)
                    tmp+=4;
                break;
            case '1':
                if(!firstBit)
                    tmp+=3;
                firstBit= true;
                break;
            case '2': case '3':
                if(!firstBit)
                    tmp+=2;
                firstBit= true;
                break;
            case '4': case '5': case '6': case '7':
                if(!firstBit)
                    tmp+=1;
                firstBit= true;
                break;
            default:
                firstBit= true;
                break;


        }
        switch (hashBin[1]) {
            case '0':
                if(!firstBit)
                    tmp+=4;
                break;
            case '1':
                if(!firstBit)
                    tmp+=3;
                firstBit= true;
                break;
            case '2': case '3':
                if(!firstBit)
                    tmp+=2;
                firstBit= true;
                break;
            case '4': case '5': case '6': case '7':
                if(!firstBit)
                    tmp+=1;
                firstBit= true;
                break;
            default:
                firstBit= true;
                break;


        }
        outputMessage+=messBin[0];
        outputMessage+=messBin[1];
        outputHash+=hashBin[0];
        outputHash+=hashBin[1];
    }

    return valid;
}

int findHash (int numberZeroBits, string & outputMessage, string & outputHash) {

    if(numberZeroBits<0||numberZeroBits > EVP_MAX_MD_SIZE)
        return 0;
    RandHash();
    first= false;
    do{
        std::copy(std::begin(hashS), std::end(hashS), std::begin(text));
        RandHash();
    }
    while (!ValidHash(numberZeroBits,outputMessage,outputHash));
    return 1;
}

int findHashEx (int numberZeroBits, string & outputMessage, string & outputHash, string_view hashType) {
    /* TODO or use dummy implementation */
    return 1;
}


#ifndef __PROGTEST__
int checkHash(int bits, const string & hash) {
    // DIY
    return 1;
}

int main (void) {
    string hash, message;
    assert(findHash(0, message, hash) == 1);
    /*
     cout<<"1)"<<message<<"-MMM-"<<endl;
     cout<<"1)"<<hash<<"-HHH-"<<endl;
 */
    assert(!message.empty() && !hash.empty() && checkHash(0, hash));
    message.clear();
    hash.clear();
    assert(findHash(1, message, hash) == 1);
    /*
      cout<<"2)"<<message<<"-MMM-"<<endl;
      cout<<"2)"<<hash<<"-HHH-"<<endl;
    */
    assert(!message.empty() && !hash.empty() && checkHash(1, hash));
    message.clear();
    hash.clear();
    assert(findHash(2, message, hash) == 1);
    /*
    cout<<"3)"<<message<<"-MMM-"<<endl;
    cout<<"3)"<<hash<<"-HHH-"<<endl;
    */
    assert(!message.empty() && !hash.empty() && checkHash(2, hash));
    message.clear();
    hash.clear();
    assert(findHash(10, message, hash) == 1);
/*
    cout<<"4)"<<message<<"-MMM-"<<endl;
    cout<<"4)"<<hash<<"-HHH-"<<endl;
*/
    assert(!message.empty() && !hash.empty() && checkHash(3, hash));
    message.clear();
    hash.clear();
    assert(findHash(-1, message, hash) == 0);
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */