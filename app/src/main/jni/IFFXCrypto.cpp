/*
Copyleft 2016 Gordon Young
gjyoung1974@gmail.com

This class demonstrates Botan's Credit Card Number format preserving Crypto
TODO: need an architecture for Zero knowledge sercret derevation for PBKDF secret

*/

#include <jni.h>
#include <string>

#include "botan_all.h"

using namespace Botan;

/* Luhn checksum is required for FFX */
uint8_t luhn_checksum(uint64_t cc_number) //TODO is this the best LUHN mechanism for CPP?
{
    uint8_t sum = 0;

    bool alt = false;
    while (cc_number) {
        uint8_t digit = cc_number % 10;
        if (alt) {
            digit *= 2;
            if (digit > 9)
                digit -= 9;
        }

        sum += digit;

        cc_number /= 10;
        alt = !alt;
    }

    return (sum % 10);
}

bool luhn_check(uint64_t cc_number) {
    return (luhn_checksum(cc_number) == 0);
}

uint64_t cc_rank(uint64_t cc_number) {
    // Remove Luhn checksum
    return cc_number / 10;
}

uint64_t cc_derank(uint64_t cc_number) {
    for (size_t i = 0; i != 10; ++i) {
        if (luhn_check(cc_number * 10 + i)) {
            return (cc_number * 10 + i);
        }
    }

    return 0;
}

/* a method to decrypt a credit card number */
uint64_t decrypt_cc_number(uint64_t enc_cc,
                           const Botan::secure_vector<uint8_t>& key,
                           const std::vector<uint8_t>& tweak)
{
    const Botan::BigInt n = 1000000000000000;

    const uint64_t cc_ranked = cc_rank(enc_cc);

    const Botan::BigInt c = Botan::FPE::fe1_decrypt(n, cc_ranked, key, tweak);

    if(c.bits() > 50)
        throw Botan::Internal_Error("FPE produced a number too large");

    uint64_t dec_cc = 0;
    for(size_t i = 0; i != 7; ++i)
        dec_cc = (dec_cc << 8) | c.byte_at(6-i);
    return cc_derank(dec_cc);
}



/* a method to encrypt a credit card number */
uint64_t encrypt_cc_number(uint64_t cc_number,
                           const Botan::secure_vector<uint8_t> &key,
                           const std::vector<uint8_t> &tweak) {
    const Botan::BigInt n = 1000000000000000;

    const uint64_t cc_ranked = cc_rank(cc_number);

    const Botan::BigInt c = Botan::FPE::fe1_encrypt(n, cc_ranked, key, tweak);

    if (c.bits() > 50)
        throw Botan::Internal_Error("FPE produced a number too large");

    uint64_t enc_cc = 0;
    for (size_t i = 0; i != 7; ++i)
        enc_cc = (enc_cc << 8) | c.byte_at(6 - i);
    return cc_derank(enc_cc);
}

extern "C"
jstring  Java_com_gyoung_crypto_botan_android_demo_DecryptActivity_DEcryptCCString(JNIEnv * env, jobject jobj, jstring ccid, jstring passwd) {


    jboolean isCopy;
    std::string s_ccid;
    s_ccid = env->GetStringUTFChars(ccid, &isCopy);

    jboolean isCopy2;
    std::string s_passwd;
    s_passwd = env->GetStringUTFChars(passwd, &isCopy2);

    const uint64_t cc_number = std::stoull(s_ccid);
    const std::vector<uint8_t> tweak;

    std::unique_ptr<Botan::PBKDF> pbkdf(Botan::PBKDF::create("PBKDF2(SHA-256)"));

    Botan::secure_vector<uint8_t> key = pbkdf->pbkdf_iterations(32, s_passwd, tweak.data(), tweak.size(), 100000);

    uint64_t ffx =  decrypt_cc_number(cc_number, key, tweak);

    std::ostringstream o;
    o << ffx;

    std::string cc_ffx = o.str();//return the FFX transform of our CC#;

    return env->NewStringUTF(cc_ffx.c_str());
}

extern "C"
jstring  Java_com_gyoung_crypto_botan_android_demo_MainActivity_EncryptCCString(JNIEnv * env, jobject jobj, jstring ccid, jstring passwd) {

    jboolean isCopy;
    std::string s_ccid;
    s_ccid = env->GetStringUTFChars(ccid, &isCopy);

    jboolean isCopy2;
    std::string s_passwd;
    s_passwd = env->GetStringUTFChars(passwd, &isCopy2);

    const uint64_t cc_number = std::stoull(s_ccid);
    const std::vector<uint8_t> tweak;

    std::unique_ptr<Botan::PBKDF> pbkdf(Botan::PBKDF::create("PBKDF2(SHA-256)"));

    Botan::secure_vector<uint8_t> key = pbkdf->pbkdf_iterations(32, s_passwd, tweak.data(), tweak.size(), 100000);

    uint64_t ffx =  encrypt_cc_number(cc_number, key, tweak);

    std::ostringstream o;
    o << ffx;

    std::string cc_ffx = o.str();//return the FFX transform of our CC#;

    return env->NewStringUTF(cc_ffx.c_str());
}



