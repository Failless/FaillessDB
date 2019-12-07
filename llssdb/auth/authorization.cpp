#include <iostream>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include "llssdb/auth/authorization.h"

namespace failless {
namespace db {
namespace auth {

bool SimpleSHA256(const std::string &input, unsigned char *md) {
    if (input.empty()) return false;
    SHA256(reinterpret_cast<const uint8_t *>(input.data()), input.size(), md);
    return true;
}

/**
 * private function for hashing with sha256 and salt
 * @param login
 * @param pass
 * @param md <- pointer to hash array. Hasher change it.
 */
void Authorization::Hasher_(const std::string &login, std::string pass,
                                      unsigned char *md) {
    std::string salt(pass);
    for (int i = static_cast<int>(login.length()) - 1, j = 0; i >= 0; --i) {
        salt[j++] = login[i];  // salt - reversed login - unique
    }

    pass += salt;  // правильно было бы хешировать, а потом солить и снова хешировать,
    // но дажеint если солить таким способом, как у нас - нельзя получить доступ к аккаунтам с
    // одинаковыми паролями, взломав лишь один, т.к. итоговые хэши все равно различны

    if (!SimpleSHA256(pass, md)) {
        throw std::logic_error("sha256 failed");
    }
}

/**
 * remove user from Users_ map with login and pass
 * @param login
 * @param pass
 * @return true - if all right, false - if incorrect password or no such user
 */
bool Authorization::RemoveUser(const std::string &login, const std::string &pass) {
    if (!Users_.count(login)) {
        std::cerr << "No such user '" << login << "' to remove" << std::endl;
        return false;
    }
    unsigned char pass_hash[SHA256_DIGEST_LENGTH];
    Hasher_(login, pass, pass_hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        if (Users_[login].pass_hash[i] != pass_hash[i]) {
            std::cerr << "Password for user '" << login << "' is incorrect" << std::endl;
            return false;
        }
    }
    Users_.erase(login);
    return true;
}

/**
 * verify if this pass fits this login by checking Users_ map
 * @param login
 * @param pass
 * @param table_id
 * @return true  - if all right, false - if password is incorrect
 */
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

/**
 * Register new user by adding info about him to Users_ map
 * @param login
 * @param pass
 * @param table_id
 * @return true - if all right, false - if such user exists
 */
bool Authorization::Registration(const std::string &login, const std::string &pass, int table_id) {
    if (CheckCollisions_(login)) {
        std::cerr << "User '" << login << "' already exists" << std::endl;
        return false;
    }
    // TODO add password validation
    UserInfo User(login);
    unsigned char pass_hash[SHA256_DIGEST_LENGTH]{};
    Hasher_(login, pass, pass_hash);
    memcpy(User.pass_hash, pass_hash, SHA256_DIGEST_LENGTH);
    User.table_id = table_id;
    Users_[login] = User;
    return true;
}

/**
 * checking if such user (login) exists.
 * @param login
 * @return true - if exists (means conflict), false - if not.
 */
bool Authorization::CheckCollisions_(const std::string &login) { return Users_.count(login) != 0; }

Authorization::Authorization(std::string login) {}

}  // namespace auth
}  // namespace db
}  // namespace failless