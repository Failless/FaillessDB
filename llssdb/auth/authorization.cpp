#include <boost/algorithm/hex.hpp>
#include <iostream>
// #include <cstring>
#include <boost/lexical_cast.hpp>
#include "llssdb/auth/authorization.h"
#include <openssl/md5.h>
#include <openssl/sha.h>


bool SimpleSHA256(const std::string &input, unsigned char *md) {
    if (input.empty()) return false;
    SHA256(reinterpret_cast<const uint8_t *>(input.data()), input.size(), md);
    return true;
}

void Authorization::Hasher_(const std::string &login, std::string pass,
                                      unsigned char *md) {
    std::string salt(pass);
    for (int i = login.length() - 1, j = 0; i >= 0; i--) {
        salt[j++] = login[i];  // salt - reversed login - unique
    }

    pass += salt;  // правильно было бы хешировать, а потом солить и снова хешировать,
    // но даже если солить таким способом, как у нас - нельзя получить доступ к аккаунтам с
    // одинаковыми паролями, взломав лишь один, т.к. итоговые хэши все равно различны

    if (!SimpleSHA256(pass, md)) {
        throw std::logic_error("sha256 failed");
    }
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
    //    Hasher_(login, pass, pass_hash);
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
    unsigned char pass_hash[SHA256_DIGEST_LENGTH]{};
    Hasher_(login, pass, pass_hash);
    memcpy(User.pass_hash, pass_hash, SHA256_DIGEST_LENGTH);
    Users_[login] = User;
    return true;
}

bool Authorization::CheckCollisions_(const std::string &login) { return Users_.count(login) != 0; }

Authorization::Authorization(std::string login) {}