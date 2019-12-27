#include <openssl/sha.h>
#include "llss3p/utils/hasher.h"

bool failless::common::utils::SimpleSHA256(const std::string &input, unsigned char *md) {
    if (input.empty()) return false;
    SHA256(reinterpret_cast<const uint8_t *>(input.data()), input.size(), md);
    return true;
}