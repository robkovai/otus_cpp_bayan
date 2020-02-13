#include "hashlib.h"

#include <algorithm>
#include <boost/crc.hpp>
#include <boost/uuid/sha1.hpp>

#include "options.h"

Hash HashLib::calcHash(Type type, const char *block) {
    switch (type) {
        case Type::Crc32: return HashLib::calcHashCrc32(block);
        case Type::Sha1: return HashLib::calcHashSha1(block);
    }
}

HashLib::Type HashLib::convertHashType(const std::string &value) const {
    std::string lowerValue = value;
    std::transform(value.begin(), value.end(), lowerValue.begin(), ::tolower);
    if (lowerValue == "sha1")
        return Type::Sha1;
    return Type::Crc32;
}

Hash HashLib::convertHashType(const Type &value) const {
    switch (value) {
        case Type::Crc32: return "crc32";
        case Type::Sha1: return "sha1";
    }
}

Hash HashLib::calcHashCrc32(const char *block) {
    boost::crc_32_type crc32Hash;
    crc32Hash.process_bytes(block, Options::instance()->getBlockSize());
    Hash hash = std::to_string(crc32Hash.checksum());
    return hash;
}

Hash HashLib::calcHashSha1(const char *block) {
    boost::uuids::detail::sha1 sha1Hash;
    sha1Hash.process_bytes(block, Options::instance()->getBlockSize());
    unsigned digest[5] = {0};
    sha1Hash.get_digest(digest);
    Hash hash;
    for (auto value : digest)
        hash += std::to_string(value);
    return hash;
}
