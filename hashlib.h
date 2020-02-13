#pragma once

#include <string>
#include <vector>

using Hash = std::string;
using Hashes = std::vector<Hash>;
using HashIterator = typename Hashes::iterator;

class HashLib {
public:
    enum class Type {
        Crc32,
        Sha1
    };

    Hash calcHash(Type hashType, const char* block);

    Type convertHashType(const Hash &value) const;
    Hash convertHashType(const Type &value) const;

private:
    Hash calcHashCrc32(const char* block);
    Hash calcHashSha1(const char *block);
};
