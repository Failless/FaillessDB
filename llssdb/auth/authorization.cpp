#include "llssdb/auth/authorization.h"

#include <iostream>

#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

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
void Authorization::Hasher_(const std::string &login, std::string pass, unsigned char *md) {
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
    AddingUser_(User);
    BOOST_LOG_TRIVIAL(debug) << "[AUTH]: New user '" << login << "' registered";
    return true;
}

/**
 * this func is for writing new user in user file
 * (in case if db will crash)
 */
bool Authorization::AddingUser_(const UserInfo &User) {
    std::ofstream filer(kUsersPath, std::ios::app);
    if (!filer.is_open()) {
        BOOST_LOG_TRIVIAL(error) << "[AUTH]: Can't add user, file '" << kUsersPath
                                 << "' can't be opened";
        return false;
    }
    std::cout << "login: " << User.login << std::endl;
    filer << "login = " << User.login << std::endl;
    filer << "is_conn = " << User.is_conn << std::endl;
    filer << "table_id = " << User.table_id << std::endl;
    filer << "pass_hash = ";
    for (int j = 0; j < 32; ++j) {
        filer << User.pass_hash[j];
    }
    filer << std::endl;
    filer << std::endl;
    filer << std::endl;
    filer.close();
    return true;
}

/**
 * checking if such user (login) exists.
 * @param login
 * @return true - if exists (means conflict), false - if not.
 */
bool Authorization::CheckCollisions_(const std::string &login) { return Users_.count(login) != 0; }

Authorization::Authorization(std::string login) {}

/*
 * this func loads users from file in tmp
 */
Authorization::Authorization() {
    std::string config_path = kUsersPath;
    if (!boost::filesystem::exists(config_path)) {
        std::cerr << "No such file";
        std::ofstream user_file(config_path);
        user_file.close();
        return;
    }
    std::ifstream cFile(config_path);
    if (!cFile.is_open()) {
        std::cerr << "could not open file with users" << std::endl;
    }

    std::string line{};
    std::string user = "";
    UserInfo UI;
    int i = 0;
    std::string login = "";
    while (getline(cFile, line)) {
        if (i == 4) {
            Users_[login] = UI;
            i = 0;
        }
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
        auto delimiterPos = line.find("=");
        std::string name = line.substr(0, delimiterPos);
        auto value = line.substr(delimiterPos + 1);

        if (name == "login" && i == 0) {
            ++i;
            UI.login = value;
            login = value;
        } else if (name == "is_conn" && i == 1) {
            ++i;
            if (std::stoi(value) == 1) {
                UI.is_conn = true;
            } else {
                UI.is_conn = false;
            }
        } else if (name == "table_id" && i == 2) {
            ++i;
            UI.table_id = std::stoi(value);
        } else if (name == "pass_hash" && i == 3) {
            ++i;
            for (int j = 0; j < 32; ++j) {
                UI.pass_hash[j] = value[j];
            }
        }
    }
}

}  // namespace auth
}  // namespace db
}  // namespace failless
