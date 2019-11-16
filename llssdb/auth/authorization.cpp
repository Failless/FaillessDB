#include "llssdb/auth/authorization.h"
#include <string>


std::string Authorization::Hasher(std::string login) {
    // return boost::hasher<login>;
    // std::hash<std::string> Hash;
    // return Hash<login>;
    return login;
}

bool Authorization::IsAuth(std::string login) {
//    auto cli_id = Users[login].client_id;
//    if (cli_id) {
//        return true;
//    } else {
//        return false;
//    }
    return false;
}

bool Authorization::Registration(std::string login, std::string pass) {
//    auto cli_id = Users[login].client_id;
//    if (cli_id) {
//        return true;
//    } else {
//        return false;
//    }
    return false;
}

bool Authorization::CheckCollisions(std::string login) {
//    if (Users.count(login)){
//        return false;
//    } else {
//        return true;
//    }
    return false;
}

bool Authorization::SignIn(std::string login, std::string pass) {
    CheckCollisions(login);
    return false;
}

