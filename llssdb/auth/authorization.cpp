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
    if (CheckCollisions(login)) {
        return false;
    }
    UserInfo *User = new UserInfo;
    User->login = login;
    User->pass_hash = Hasher(login);
    // User->client_id; User->dir; toDO ???
    Users[login] = *User;
    return true;
}

bool Authorization::CheckCollisions(std::string login) {
    if (Users.count(login)){
        return true;  // such login already in structs
    } else {
        return false;
    }
}

bool Authorization::SignIn(std::string login, std::string pass) {
    CheckCollisions(login);
    return false;
}

Authorization::Authorization(std::string directory) {

}

