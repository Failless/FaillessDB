#include "llssdb/auth/registration.h"

bool Registration::Register(std::string login, std::string pass) { return false; }

bool Registration::CheckCollisions_() {
  login_ = {};
  return false;
}

bool Registration::CreateDir_() { return false; }

bool Registration::UpdateServerDB_() { return false; }

Registration::Registration(std::string directory) {}
