#include "llssdb/auth/authorization.h"

// #include <iostream>

//using boost::uuids::detail::md5;

std::string Authorization::toString_(const boost::uuids::detail::md5::digest_type &digest)
{
    const auto charDigest = reinterpret_cast<const char *>(&digest);
    std::string result;
    boost::algorithm::hex(charDigest, charDigest + sizeof(boost::uuids::detail::md5::digest_type
                                                              ), std::back_inserter(result));
    return result;
}

std::string Authorization::Hasher_(std::string login, std::string pass) {
    std::string salt = login;
    int j = 0;
    for (int i = login.size()-1; i >= 0; --i)
        salt[j++] = login[i];  // salt - reversed login - unique
    boost::uuids::detail::md5 hash;
    boost::uuids::detail::md5::digest_type digest;

    hash.process_bytes(pass.data(), pass.size());
    hash.get_digest(digest);
    std::string new_hash = toString_(digest)+salt;

    hash.process_bytes(new_hash.data(), new_hash.size());
    hash.get_digest(digest);
    return toString_(digest);
}

bool Authorization::IsAuth_(std::string login) {
    return Users_[login].is_conn;
}

bool Authorization::Registration(std::string login, std::string pass) {
    if (CheckCollisions_(login)) {
        return false;
    }
    UserInfo *User = new UserInfo;
    User->login = login;
    User->pass_hash = Hasher_(login, pass);
    Users_[login] = *User;
    return true;
}

bool Authorization::CheckCollisions_(std::string login) {
    if (Users_.count(login)){
        return true;  // such login already in structs
    } else {
        return false;
    }
}

bool Authorization::SignIn(std::string login, std::string pass) {
    if (IsAuth_(login)) {
        return false;
    }
    std::string hash = Hasher_(login, pass);
    if (Users_[login].pass_hash == hash) {
        // toDo some signIn logic
        return true;
    }
    return false;
}

Authorization::Authorization(std::string login) {

}

