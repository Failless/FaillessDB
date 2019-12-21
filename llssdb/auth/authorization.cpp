#include "llssdb/auth/authorization.h"

#include <iostream>

#include <boost/log/trivial.hpp>
#include <openssl/md5.h>
#include <openssl/sha.h>

#include "llss3p/utils/hasher.h"

namespace failless {
namespace db {
namespace auth {

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

    if (!failless::common::utils::SimpleSHA256(pass, md)) {
        BOOST_LOG_TRIVIAL(error) << "[AUTH]: SHA256 failed";
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
        BOOST_LOG_TRIVIAL(error) << "[AUTH]: No such user '" << login << "' to remove";
        return false;
    }
    unsigned char pass_hash[SHA256_DIGEST_LENGTH];
    Hasher_(login, pass, pass_hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        if (Users_[login].pass_hash[i] != pass_hash[i]) {
            BOOST_LOG_TRIVIAL(error) << "[AUTH]: Password for user '" << login << "' is incorrect";
            return false;
        }
    }
    Users_.erase(login);
    BOOST_LOG_TRIVIAL(debug) << "[AUTH]: Removed user '" << login << "'";
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
        BOOST_LOG_TRIVIAL(error) << "[AUTH]: User '" << login << "' already exists";
        return false;
    }
    // TODO add password validation
    UserInfo User(login);
    unsigned char pass_hash[SHA256_DIGEST_LENGTH]{};
    Hasher_(login, pass, pass_hash);
    memcpy(User.pass_hash, pass_hash, SHA256_DIGEST_LENGTH);
    User.table_id = table_id;
    Users_[login] = User;
    BOOST_LOG_TRIVIAL(debug) << "[AUTH]: New user '" << login << "' registered";
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