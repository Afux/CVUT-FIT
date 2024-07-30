#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/pem.h>

using namespace std;

#endif /* __PROGTEST__ */



int deleteFile(string_view outFile){

    ifstream f(outFile.data());
    if(f.good()){
        remove(outFile.data());
    }
    return 0;
}
bool seal( string_view inFile, string_view outFile, string_view publicKeyFile, string_view symmetricCipher )
{
    if(inFile.empty()||outFile.empty()||publicKeyFile.empty()||symmetricCipher.empty())
        return false;
    OpenSSL_add_all_ciphers();
    EVP_CIPHER_CTX * ctx;
    ctx = EVP_CIPHER_CTX_new();
    EVP_PKEY * pubkey;
    FILE *p_key;
    p_key=fopen(publicKeyFile.data(), "r");
    auto cipher = EVP_get_cipherbyname(symmetricCipher.data());
    int my_ekl=0;
    int stLength=0;
    if(!cipher||ctx==NULL||!p_key){
        if(p_key)
            fclose(p_key);
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    std::unique_ptr< unsigned char[]> blockBuff;
    std::unique_ptr< unsigned char[]> blockBuffOUT;
    blockBuff= make_unique<unsigned char []>(EVP_CIPHER_block_size(cipher));
    blockBuffOUT=make_unique<unsigned char []>(EVP_CIPHER_block_size(cipher));

    pubkey = PEM_read_PUBKEY(p_key, NULL, NULL, NULL); //No password protection of the key itself
    if(!pubkey){
        fclose(p_key);
        EVP_CIPHER_CTX_free(ctx);

        EVP_PKEY_free(pubkey);
        return false;
    }
    std::unique_ptr< unsigned char[]> cipheredKey = make_unique<unsigned char []>(EVP_PKEY_size(pubkey));
    std::unique_ptr< unsigned char[]> IV=  make_unique<unsigned char []>(EVP_CIPHER_iv_length(cipher));

    if(!EVP_SealInit(ctx, cipher, reinterpret_cast<unsigned char **>(&cipheredKey), &my_ekl, IV.get(), &pubkey, 1)){
        fclose(p_key);
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        return false;
    }

    ifstream stream;
    ofstream stream1;
    stream.open(inFile.data(), ios::in | ios::binary);
    stream1.open(outFile.data(),ios::out | ios::binary );

    if(!stream1.is_open()||!stream.is_open()||!stream.good()||!stream1.good()){



        deleteFile(outFile.data());
        fclose(p_key);
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(pubkey);

        return false;
    }


    stream.seekg(0, ios::end);
    int length = stream.tellg();
    stream.seekg(0);
    int pos=0;
    bool errorFlag= false;
    int id=EVP_CIPHER_get_nid(cipher);

    stream1.write(reinterpret_cast< char*> (&id), sizeof(id));
    if (!stream1.good()) {

        deleteFile(outFile.data());
        fclose(p_key);
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(pubkey);

        return false;
    }
    stream1.write(reinterpret_cast< char*>(&my_ekl), 4);
    if (!stream1.good()) {

        deleteFile(outFile.data());
        fclose(p_key);
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        return false;
    }
    stream1.write(reinterpret_cast< char*>(cipheredKey.get()), my_ekl);
    if (!stream1.good()) {
        deleteFile(outFile.data());
        fclose(p_key);
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        return false;
    }
    if(EVP_CIPHER_iv_length(cipher)>0){
        stream1.write(reinterpret_cast< char*>(IV.get()), EVP_CIPHER_iv_length(cipher));
        if (!stream1.good()) {


            deleteFile(outFile.data());
            fclose(p_key);
            EVP_CIPHER_CTX_free(ctx);
            EVP_PKEY_free(pubkey);
            return false;
        }
    }
    while (pos<length){

        int readed=0;
        if(EVP_CIPHER_get_block_size(cipher)>(length-pos)){
            stream.read(reinterpret_cast< char*> (blockBuff.get()),length-pos );
            readed=(length-pos);
        }
        else{
            stream.read(reinterpret_cast< char*> (blockBuff.get()),EVP_CIPHER_get_block_size(cipher) );
            readed=EVP_CIPHER_get_block_size(cipher);
        }
        pos+=readed;

        if (!stream.good()) {
            errorFlag= true;
            break;
        }
        int control=EVP_SealUpdate(ctx, blockBuffOUT.get(), &stLength, blockBuff.get(), readed);
        if(!control){
            errorFlag= true;
            break;
        }
        stream1.write(reinterpret_cast< char*> (blockBuffOUT.get()), stLength);
        if (!stream1.good()) {
            errorFlag= true;
            break;
        }
    }
    if(errorFlag){
        stream1.close();
        stream.close();


        deleteFile(outFile.data());
        fclose(p_key);
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(pubkey);

        return false;

    }
    int final=EVP_SealFinal(ctx,blockBuffOUT.get(),&stLength);
    stream1.write(reinterpret_cast< char*> (blockBuffOUT.get()), stLength);
    if (!final||!stream1.good()) {
        stream1.close();
        stream.close();


        deleteFile(outFile.data());
        fclose(p_key);
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(pubkey);

        return false;
    }
    stream1.close();
    stream.close();
    EVP_CIPHER_CTX_free(ctx);
    EVP_PKEY_free(pubkey);
    fclose(p_key);
    return true;
}
bool open( string_view inFile, string_view outFile, string_view privateKeyFile )
{
    if(inFile.empty()||outFile.empty()||privateKeyFile.empty())
        return false;
    OpenSSL_add_all_ciphers();
    EVP_CIPHER_CTX * ctx; // context structure
    int cipherID= 0;
    int EKlen=0;
    int IVlen=0;
    ctx = EVP_CIPHER_CTX_new();
    const EVP_CIPHER *  cipher;
    std::unique_ptr< unsigned char[]> blockBuff;
    std::unique_ptr< unsigned char[]> blockBuffOUT;
    std::unique_ptr< unsigned char[]> sharedKey;
    std::unique_ptr< unsigned char[]> IV;


    EVP_PKEY * pubkey;

    FILE *p_key=fopen(privateKeyFile.data(), "r");
    if(ctx==NULL||!p_key){
        EVP_CIPHER_CTX_free(ctx);
        if(p_key)
            fclose(p_key);
        return false;
    }
    pubkey = PEM_read_PrivateKey(p_key, NULL, NULL, NULL); //No password protection of the key itself
    if(!pubkey){
        EVP_PKEY_free(pubkey);
        EVP_CIPHER_CTX_free(ctx);
        fclose(p_key);
        return false;
    }



    ifstream stream;
    ofstream stream1;
    stream.open(inFile.data(), ios::in | ios::binary);
    stream1.open(outFile.data(),ios::out | ios::binary );
    if(!stream1.is_open()||!stream.is_open()||!stream.good()||!stream1.good()){
        deleteFile(outFile.data());
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        fclose(p_key);
        return false;
    }


    int stLength=0;
    stream.seekg(0, ios::end);
    int length = stream.tellg();
    stream.seekg(0);
    int pos=0;
    bool errorFlag= false;
    bool header= true;
    while (pos<length){

        int readed=0;
        if(header){
            stream.read(reinterpret_cast<char*>(&cipherID), 4);
            if (!stream.good()) {
                errorFlag= true;
                break;
            }
            cipher= EVP_get_cipherbynid(cipherID);
            stream.read(reinterpret_cast<char*>(&EKlen), 4 );
            if (!stream.good()||!cipher||EKlen<=0) {
                errorFlag= true;
                break;
            }
            sharedKey= make_unique<unsigned char[]>(EKlen);
            stream.read(reinterpret_cast< char*>( sharedKey.get()), EKlen);
            if (!stream.good()) {
                errorFlag= true;
                break;
            }
            if(EVP_CIPHER_iv_length(cipher)>0){
                IVlen=EVP_CIPHER_iv_length(cipher);
                IV= make_unique<unsigned char[]>(IVlen);
                stream.read(reinterpret_cast< char*>( IV.get()), IVlen);

            }
            if(!stream.good()||!EVP_OpenInit(ctx, cipher, sharedKey.get(),EKlen, IV.get(), pubkey)){
                errorFlag= true;
                break;
            }

            blockBuffOUT= make_unique<unsigned char []>(EVP_CIPHER_block_size(cipher));
            blockBuff= make_unique<unsigned char []>(EVP_CIPHER_block_size(cipher));
            header= false;
            pos+=(8+EKlen+IVlen);

        }

        if(EVP_CIPHER_block_size(cipher)>(length-pos)){
            stream.read(reinterpret_cast< char*> (blockBuff.get()),length-pos );
            readed=(length-pos);

        }
        else{
            stream.read(reinterpret_cast< char*> (blockBuff.get()),EVP_CIPHER_block_size(cipher) );
            readed=EVP_CIPHER_block_size(cipher);
        }
        pos+=readed;
        if (!stream.good()) {
            errorFlag= true;
            break;
        }

        if(!EVP_OpenUpdate(ctx, blockBuffOUT.get(), &stLength, blockBuff.get(), readed)){
            errorFlag= true;
            break;
        }
        stream1.write(reinterpret_cast< char*> (blockBuffOUT.get()), stLength);

        if (!stream1.good()) {
            errorFlag= true;
            break;
        }
    }

    if(errorFlag){

        stream1.close();
        stream.close();
        deleteFile(outFile.data());
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        fclose(p_key);
        return false;

    }

    int final=EVP_OpenFinal(ctx,blockBuffOUT.get(),&stLength);
    stream1.write(reinterpret_cast< char*> (blockBuffOUT.get()), stLength);

    if (!final||!stream1.good()) {
        stream1.close();
        stream.close();
        deleteFile(outFile.data());
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        fclose(p_key);
        return false;
    }
    stream1.close();
    stream.close();
    EVP_CIPHER_CTX_free(ctx);
    EVP_PKEY_free(pubkey);
    fclose(p_key);
    return true;
}



#ifndef __PROGTEST__

int main ( void )
{

    assert( seal("fileToEncrypt", "sealed.bin", "PublicKey.pem", "aes-128-cbc") );
    assert( open("sealed.bin", "openedFileToEncrypt", "PrivateKey.pem") );
    assert( open("sealed_sample.bin", "opened_sample.txt", "PrivateKey.pem") );

    return 0;
}

#endif /* __PROGTEST__ */
