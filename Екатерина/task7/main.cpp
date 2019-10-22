#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

float findPercent( std::string const &dir )
{
    fs::path p(dir);
    if (!fs::exists(p))
        throw "Path does not exist!";

    uintmax_t
            readableSize = 0,
            commonSize = 0;

    for (auto f : fs::directory_iterator(p))
    {
        if (fs::status(f).type() == fs::file_type::regular &&
           (fs::status(f).permissions() & fs::perms::owner_write) == fs::perms::none)
            readableSize += fs::file_size(f);
        commonSize += fs::file_size(f);
    }

    return float(readableSize) / commonSize;
}

int main()
{
    std::string path;
    std::cout << "Please enter relative path: ";
    std::cin >> path;
    try {
        float prc = findPercent(path);
        std::cout << "Percent of readable files is: " << prc << std::endl;
    } catch (const char *p) {
        std::cout << "Error: " << p << std::endl;
    }
    return 0;
}
