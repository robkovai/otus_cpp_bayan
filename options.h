#pragma once

#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "hashlib.h"

using FileName = std::string;
using FileSize = std::size_t;
using FileNames = std::vector<FileName>;

class Options {
public:
    static Options* instance();

    bool parse(int argc, char* argv[]);

    std::size_t getBlockSize() const;
    HashLib::Type getHashType() const;
    const FileNames &getFileNames() const;

private:
    static Options *self;

    bool parseMap(boost::program_options::options_description &description, const boost::program_options::variables_map &vm);

    FileNames getAbsoluteFileNames(const FileNames &names) const;
    FileNames getDirectoryFiles(const std::string &directory) const;

    void removeFileNamesDuplicates();

    HashLib hashLib;
    std::size_t blockSize = 1024;
    HashLib::Type hashType = HashLib::Type::Crc32;
    FileNames files;
};
