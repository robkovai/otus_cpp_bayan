#pragma once

#include <string>
#include <vector>

#include "options.h"

struct File {
    FileName name;
    FileSize size;
    Hashes hashes;
    File(FileName name, FileSize size): name(name), size(size) {}
};

using Files = std::vector<File>;
using Duplicates = std::map<Hashes, FileNames>;

class DuplicatesSearch {
public:
    const Duplicates &search();

private:
    void appendFile(const FileName &fileName);
    FileSize getFileSize(const FileName &fileName);
    bool compareFiles(File &first, File &second);
    Hash getHash(File &file, HashIterator &it);
    void appendDuplicate(const File &file);
    Hash readNextHash(File &file);
    char* readNextBlock(File &file);

    HashLib hashLib;
    Files files;
    Duplicates duplicates;
};
