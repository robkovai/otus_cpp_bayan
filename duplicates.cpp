#include "duplicates.h"

#include <fstream>

namespace fs = boost::filesystem;

const Duplicates &DuplicatesSearch::search() {
    const auto &fileNames = Options::instance()->getFileNames();
    for (const auto &fileName : fileNames)
        this->appendFile(fileName);
    return this->duplicates;
}

void DuplicatesSearch::appendFile(const FileName &fileName) {
    this->files.emplace_back(fileName, this->getFileSize(fileName));
    auto &currentFile = this->files.back();
    for (auto it = this->files.begin(); it != --this->files.end(); ++it) {
        if (this->compareFiles(currentFile, *it)) {
            this->appendDuplicate(currentFile);
            this->appendDuplicate(*it);
        }
    }
}

FileSize DuplicatesSearch::getFileSize(const FileName &fileName) {
    return FileSize(fs::file_size(fs::path(fileName)));
}

bool DuplicatesSearch::compareFiles(File &first, File &second) {
    if (first.size != second.size)
        return false;

    auto firstIt = first.hashes.begin();
    auto secondIt = second.hashes.begin();

    while (true) {
        Hash firstHash = this->getHash(first, firstIt);
        Hash secondHash = this->getHash(second, secondIt);

        if (firstHash.empty() && secondHash.empty())
            return true;

        if (firstHash.empty() || secondHash.empty())
            return false;

        if (!firstHash.empty() && !secondHash.empty())
            if (firstHash != secondHash)
                return false;

        ++firstIt;
        ++secondIt;
    }

    return true;
}

Hash DuplicatesSearch::getHash(File &file, HashIterator &it) {
    if (it == file.hashes.end()) {
        auto hash = this->readNextHash(file);
        it = --file.hashes.end();
        return hash;
    }
    return *it;
}

void DuplicatesSearch::appendDuplicate(const File &file) {
    auto &currentDuplicate = this->duplicates[file.hashes];
    if (std::find(currentDuplicate.begin(), currentDuplicate.end(), file.name) == currentDuplicate.end())
        currentDuplicate.push_back(file.name);
}

Hash DuplicatesSearch::readNextHash(File &file) {
    Hash hash;
    auto block = this->readNextBlock(file);
    if (block) {
        hash = this->hashLib.calcHash(Options::instance()->getHashType(), block);
        file.hashes.push_back(hash);
    }
    return hash;
}

char *DuplicatesSearch::readNextBlock(File &file) {
    auto blockSize = Options::instance()->getBlockSize();
    std::ifstream fileStream;
    fileStream.rdbuf()->pubsetbuf(nullptr, 0);
    fileStream.open(file.name);

    auto seek = file.hashes.size() * blockSize;
    if (seek >= file.size)
        return nullptr;

    fileStream.seekg(std::streamoff(seek));
    char *buffer = new char[blockSize];
    std::memset(buffer, 0, blockSize);
    fileStream.read(buffer, std::streamsize(blockSize));
    fileStream.close();
    return buffer;
}
