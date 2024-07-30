#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

struct crypto_config
{
	const char * m_crypto_function;
	std::unique_ptr<uint8_t[]> m_key;
	std::unique_ptr<uint8_t[]> m_IV;
	size_t m_key_len;
	size_t m_IV_len;
};

#endif /* _PROGTEST_ */
bool multicrypt_data(const std::string & in_filename, const std::string & out_filename, crypto_config & config, bool encrypt){
    OpenSSL_add_all_ciphers();
    auto cipher = EVP_get_cipherbyname(config.m_crypto_function);
    if(!cipher){
        return false;
    }
    unsigned char *blockBuff= new unsigned char [EVP_CIPHER_block_size(cipher)];
    unsigned char *blockBuffOUT= new unsigned char[EVP_CIPHER_block_size(cipher)];
    char* headerbuffer = new char[18];


    if(config.m_key== nullptr||config.m_key_len<(long unsigned)EVP_CIPHER_key_length(cipher)){
        if(!encrypt){
            delete[]blockBuffOUT;
            delete[]blockBuff;
            delete[]headerbuffer;
            return false;
        }
        auto key= make_unique<uint8_t[]>(EVP_CIPHER_key_length(cipher));
        RAND_bytes(key.get(),EVP_CIPHER_key_length(cipher));
        config.m_key_len=EVP_CIPHER_key_length(cipher);
        config.m_key=move(key);
    }
    if((EVP_CIPHER_iv_length(cipher)>0&&(config.m_IV== nullptr))||(EVP_CIPHER_iv_length(cipher)>0&&(config.m_IV_len<(long unsigned )EVP_CIPHER_iv_length(cipher)))){
        if(!encrypt){
            delete[]blockBuffOUT;
            delete[]blockBuff;
            delete[]headerbuffer;
            return false;
        }
        auto iv= make_unique<uint8_t[]>(EVP_CIPHER_iv_length(cipher));
        RAND_bytes(iv.get(),EVP_CIPHER_iv_length(cipher));
        config.m_IV_len=EVP_CIPHER_iv_length(cipher);
        config.m_IV=move(iv);
    }
    bool header= true;

    ifstream stream;
    ofstream stream1;
    stream.open(in_filename, ios::in | ios::binary);
    stream1.open(out_filename,ios::out | ios::binary );
    EVP_CIPHER_CTX * ctx; // context structure
    ctx = EVP_CIPHER_CTX_new();
    if ((ctx == NULL)){

        return false;
    }
    int i =encrypt  ? EVP_EncryptInit_ex(ctx, cipher, NULL, config.m_key.get(), config.m_IV.get()): EVP_DecryptInit_ex(ctx, cipher, NULL, config.m_key.get(), config.m_IV.get());
    if(!i){
        delete[]blockBuffOUT;
        delete[]blockBuff;
        delete[]headerbuffer;

        return false;

    }
    if(!stream1.is_open()||!stream.is_open()){
        EVP_CIPHER_CTX_free(ctx);
        delete[]blockBuffOUT;
        delete[]blockBuff;
        delete[]headerbuffer;
        return false;
    }

    if (!stream.good()||!stream1.good()) {

        EVP_CIPHER_CTX_free(ctx);
        delete[]blockBuffOUT;
        delete[]blockBuff;
        delete[]headerbuffer;
        return false;
    }

    int stLength=0;
    stream.seekg(0, ios::end);
    int length = stream.tellg();
    stream.seekg(0);
    int pos=0;
    bool errorFlag= false;
    if(length<18){
        EVP_CIPHER_CTX_free(ctx);
        delete[]blockBuffOUT;
        delete[]blockBuff;
        delete[]headerbuffer;
        return false;
    }
    while (pos<length){
        int readed=0;
        if(header){
            stream.read(headerbuffer, 18);
            if (!stream.good()) {
                errorFlag= true;
                break;
            }
            stream1.write(headerbuffer, 18);
            if (!stream1.good()) {
                errorFlag= true;
                break;
            }
            header= false;
            pos+=18;
        }

        if(EVP_CIPHER_get_block_size(cipher)>(length-pos)){
            stream.read(reinterpret_cast< char*> (blockBuff),length-pos );
            readed=(length-pos);
        }
        else{
            stream.read(reinterpret_cast< char*> (blockBuff),EVP_CIPHER_get_block_size(cipher) );
            readed=EVP_CIPHER_get_block_size(cipher);


        }
        pos+=readed;

        if (!stream.good()) {
            errorFlag= true;

            break;
        }
        int control= (encrypt)?  EVP_EncryptUpdate(ctx, blockBuffOUT, &stLength, blockBuff, readed):EVP_DecryptUpdate(ctx, blockBuffOUT, &stLength, blockBuff, readed);
        if(!control){
            errorFlag= true;
            break;
        }
        stream1.write(reinterpret_cast< char*> (blockBuffOUT), stLength);
        if (!stream1.good()) {
            errorFlag= true;
            break;
        }
    }
    if(errorFlag){
        stream1.close();
        stream.close();
        EVP_CIPHER_CTX_free(ctx);
        delete[]blockBuffOUT;
        delete[]blockBuff;
        delete[]headerbuffer;
        return false;

    }
    int final=(encrypt)? EVP_EncryptFinal(ctx,blockBuffOUT,&stLength): EVP_DecryptFinal(ctx,blockBuffOUT,&stLength);
    stream1.write(reinterpret_cast< char*> (blockBuffOUT), stLength);
    if (!final||!stream1.good()) {
        stream1.close();
        stream.close();
        EVP_CIPHER_CTX_free(ctx);
        delete[]blockBuffOUT;
        delete[]blockBuff;
        delete[]headerbuffer;
        return false;
    }
    stream1.close();
    stream.close();
    EVP_CIPHER_CTX_free(ctx);
    delete[]blockBuffOUT;
    delete[]blockBuff;
    delete[]headerbuffer;
    return true;


}
bool encrypt_data ( const std::string & in_filename, const std::string & out_filename, crypto_config & config )
{
    return multicrypt_data(in_filename,out_filename,config,true);
}

bool decrypt_data ( const std::string & in_filename, const std::string & out_filename, crypto_config & config )
{
    return multicrypt_data(in_filename,out_filename,config, false);
}


#ifndef __PROGTEST__

bool compare_files ( const char * name1, const char * name2 )
{
    return true;
}

int main ( void )
{
	crypto_config config {nullptr, nullptr, nullptr, 0, 0};

	// ECB mode
	config.m_crypto_function = "AES-128-ECB";
	config.m_key = std::make_unique<uint8_t[]>(16);
 	memset(config.m_key.get(), 0, 16);
	config.m_key_len = 16;


	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config)  &&
            compare_files ("out_file.TGA", "homer-simpson_enc_ecb.TGA") );

   assert( decrypt_data  ("homer-simpson_enc_ecb.TGA", "out_file.TGA", config) &&
           compare_files ("out_file.TGA", "homer-simpson.TGA") );



	assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8_enc_ecb.TGA") );

	assert( decrypt_data  ("UCM8_enc_ecb.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8.TGA") );

	assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_1_enc_ecb.TGA") );

	assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_2_enc_ecb.TGA") );

	assert( decrypt_data ("image_3_enc_ecb.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_3_dec_ecb.TGA") );

	assert( decrypt_data ("image_4_enc_ecb.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_4_dec_ecb.TGA") );

	// CBC mode
	config.m_crypto_function = "AES-128-CBC";
	config.m_IV = std::make_unique<uint8_t[]>(16);
	config.m_IV_len = 16;
	memset(config.m_IV.get(), 0, 16);

	assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8_enc_cbc.TGA") );

	assert( decrypt_data  ("UCM8_enc_cbc.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8.TGA") );

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson_enc_cbc.TGA") );

	assert( decrypt_data  ("homer-simpson_enc_cbc.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson.TGA") );

	assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_5_enc_cbc.TGA") );

	assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_6_enc_cbc.TGA") );

	assert( decrypt_data ("image_7_enc_cbc.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_7_dec_cbc.TGA") );

	assert( decrypt_data ("image_8_enc_cbc.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_8_dec_cbc.TGA") );
    /**/
    return 0;

}

#endif /* _PROGTEST_ */
