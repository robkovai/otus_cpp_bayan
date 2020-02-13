#include "options.h"
#include "duplicates.h"

int main(int argc, char* argv[]) {
    if (!Options::instance()->parse(argc, argv))
        return 0;

    DuplicatesSearch duplicatesSearch;
    const auto duplicates = duplicatesSearch.search();

    for (auto it = duplicates.begin(); it != duplicates.end(); ++it) {
        const auto &fileNames = (*it).second;
        for (const auto &fileName : fileNames)
            std::cout << fileName << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
