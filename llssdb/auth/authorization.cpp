#include "llssdb/auth/authorization.h"
#include <iostream>
#include <openssl/sha.h>
#include <cstring>

bool simpleSHA256(void* input, unsigned long length, unsigned char* md)
{
    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;
    if(!SHA256_Update(&context, (unsigned char*)input, length))
        return false;
    return SHA256_Final(md, &context) != 0;

}

unsigned char * Authorization::Hasher_(std::string login, std::string pass) {
    std::string salt = login;
    int j = 0;
    for (int i = login.size()-1; i >= 0; --i)
        salt[j++] = login[i];  // salt - reversed login - unique

    unsigned char md[SHA256_DIGEST_LENGTH]; // 32 bytes
    const char * str_c = pass.c_str();
    char * copy = new char[strlen(str_c)];
    strcpy(copy, str_c);
    if (!simpleSHA256(copy, pass.size(), md)) {
        std::cerr << "error in hasher" << std::endl;
    }
    delete str_c;
    return md;
}

bool Authorization::RemoveUser(std::string login, std::string pass) {
    if (Users_.count(login) == 0) {
        return false;
    }
    unsigned char *pass_hash = Hasher_(login, pass);
    if (Users_[login].pass_hash == pass_hash) {
        Users_.erase(login);
        return true;
    }
    return false;
}

bool Authorization::IsAuth(std::string login, std::string pass, int table_id) {
    unsigned char *pass_hash = Hasher_(login, pass);
    int u_id = Users_[login].table_id;
    unsigned char *u_hash = Users_[login].pass_hash;
    return u_id == table_id && u_hash == pass_hash;
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

Authorization::Authorization(std::string login) {

}

