#include <sstream>
#include "string_processor.h"

std::string firstWord( std::string const &str )
{
    std::istringstream iss(str);
    std::string res;
    iss >> res;
    return res;
}

int wordCount( std::string const &str )
{
    int count = 0;
    std::istringstream iss(str);
    std::string word;

    while (iss >> word)
    {
        count++;

        int innerCount = 0;
        char prev = ',';
        for (auto it = word.begin(); it != word.end(); ++it)
        {
            if (*it == ',' && prev != ',' && it + 1 != word.end())
                innerCount++;
            prev = *it;
        }
        count += innerCount;
    }
    return count;
}
