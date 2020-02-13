#include "options.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

namespace Constant {
    const std::string OptionHelp        = "help";
    const std::string OptionBlockSize   = "blocksize";
    const std::string OptionHashType    = "hashtype";
    const std::string OptionFiles       = "files";
    const std::string OptionDirectory   = "directory";
}

Options* Options::self = nullptr;

Options* Options::instance() {
    if (!Options::self)
        Options::self = new Options();
    return Options::self;
}

bool Options::parse(int argc, char *argv[]) {
    po::options_description description("Bayan options");
    description.add_options()
            ((Constant::OptionHelp + ",h").data(), "Show help")
            ((Constant::OptionBlockSize + ",b").data(), po::value<std::size_t>()->default_value(this->blockSize), "Block size (bytes)")
            ((Constant::OptionHashType + ",t").data(), po::value<std::string>()->default_value(this->hashLib.convertHashType(this->hashType)), "Hash type (crc32, sha1)")
            ((Constant::OptionFiles + ",f").data(), po::value<FileNames>()->multitoken()->zero_tokens()->composing(), "Files to search duplicates")
            ((Constant::OptionDirectory + ",d").data(), po::value<std::string>(), "File search directory");
    po::variables_map vm;
    po::parsed_options parsed = po::command_line_parser(argc, argv).options(description).allow_unregistered().run();
    store(parsed, vm);
    notify(vm);
    return this->parseMap(description, vm);
}

bool Options::parseMap(po::options_description &description, const po::variables_map &vm) {
    if (vm.count(Constant::OptionHelp)) {
        std::cout << description << std::endl;
        return false;
    }

    if (vm.count(Constant::OptionBlockSize))
        this->blockSize = vm[Constant::OptionBlockSize].as<std::size_t>();

    if (vm.count(Constant::OptionHashType))
        this->hashType = this->hashLib.convertHashType(vm[Constant::OptionHashType].as<std::string>());

    if (vm.count(Constant::OptionFiles))
        this->files = this->getAbsoluteFileNames(vm[Constant::OptionFiles].as<FileNames>());

    if (vm.count(Constant::OptionDirectory)) {
        auto directoryFiles = this->getDirectoryFiles(vm[Constant::OptionDirectory].as<std::string>());
        this->files.insert(this->files.end(), directoryFiles.begin(), directoryFiles.end());
    }

    this->removeFileNamesDuplicates();

    if (this->files.size() < 2) {
        std::cout << description << std::endl;
        return false;
    }
    return true;
}

std::size_t Options::getBlockSize() const {
    return this->blockSize;
}

HashLib::Type Options::getHashType() const {
    return this->hashType;
}

const FileNames &Options::getFileNames() const {
    return this->files;
}

FileNames Options::getAbsoluteFileNames(const FileNames &names) const {
    FileNames fileNames;
    for (const auto &name : names) {
        auto path = fs::absolute(name);
        if (fs::exists(path))
            fileNames.push_back(path.string());
    }
    return fileNames;
}

FileNames Options::getDirectoryFiles(const std::string &directory) const {
    FileNames fileNames;
    auto path = fs::absolute(directory);
    if (fs::exists(path)) {
        fs::recursive_directory_iterator it(path), end;
        while (it != end) {
            if (fs::is_regular_file(it->path()))
                fileNames.push_back(it->path().string());
            try {
                ++it;
            } catch (const fs::filesystem_error &) {
                it.no_push();
            }
        }
    }
    return fileNames;
}

void Options::removeFileNamesDuplicates() {
    std::sort(this->files.begin(), this->files.end());
    this->files.erase(std::unique(this->files.begin(), this->files.end()), this->files.end());
}
