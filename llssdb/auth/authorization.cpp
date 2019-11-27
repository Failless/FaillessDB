#include <iostream>
#include <boost/algorithm/hex.hpp>
// #include <cstring>
#include "llssdb/auth/authorization.h"


bool SimpleSHA256(void *input, unsigned long length, unsigned char *md) {
    SHA256_CTX context;
    if (!SHA256_Init(&context)) {
        std::cerr << "Can't initialize hash (wrong context format)" << std::endl;
        return false;
    }
    if (!SHA256_Update(&context, (unsigned char *) input, length)) {
        std::cerr << "Can't update context to password" << std::endl;
        return false;
    }
    return SHA256_Final(md, &context) != 0;

}

unsigned char *
Authorization::Hasher_(const std::string &login, std::string pass, unsigned char *md) {
    std::string salt;
    for (int i = login.size() - 1, j = 0; i >= 0; --i) {
        salt[j++] = login[i];  // salt - reversed login - unique
    }

    pass += salt;  // правильно было бы хешировать, а потом солить и снова хешировать,
    // но даже если солить таким способом, как у нас - нельзя получить доступ к аккаунтам с одинаковыми паролями,
    // взломав лишь один, т.к. итоговые хэши все равно различны

    // auto *md = new unsigned char[SHA256_DIGEST_LENGTH]; // 32 bytes

//     const char * str_c = pass.c_str();
//     char * copy = new char[strlen(str_c)];
//     strcpy(copy, str_c);

    if (!SimpleSHA256(&pass, pass.size(), md)) {
        std::cerr << "error in hasher" << std::endl;
    }

    // delete str_c;
    return md;
}

bool Authorization::RemoveUser(const std::string &login, const std::string &pass) {
    if (!Users_.count(login)) {
        return false;
    }
    unsigned char pass_hash[SHA256_DIGEST_LENGTH];
    Hasher_(login, pass, pass_hash);
    if (Users_[login].pass_hash == pass_hash) {
        Users_.erase(login);
        return true;
    }
    return false;
}

bool Authorization::IsAuth(const std::string &login, const std::string &pass, int table_id) {
    unsigned char pass_hash[SHA256_DIGEST_LENGTH];
    Hasher_(login, pass, pass_hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        if (Users_[login].pass_hash[i] != pass_hash[i]) {
            return false;
        }
    }
    return Users_[login].table_id == table_id;
}

bool Authorization::Registration(const std::string &login, const std::string &pass) {
    if (CheckCollisions_(login)) {
        return false;
    }
    UserInfo User(login);
    unsigned char pass_hash[SHA256_DIGEST_LENGTH];
    memmove(User.pass_hash, Hasher_(login, pass, pass_hash), SHA256_DIGEST_LENGTH);
    // is it a correct way to move hash array from Hasher to User.pass_hash?
    Users_[login] = User;
    return true;
}

bool Authorization::CheckCollisions_(const std::string &login) {
    if (Users_.count(login)) {
        return true;
    } else {
        return false;
    }
}

Authorization::Authorization(std::string login) {

}
