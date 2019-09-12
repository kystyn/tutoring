#include <iostream>
#include "../task4_lib/string_processor.h"

int main( void )
{
    std::string str;
    std::cout << "Input string: ";
    std::getline(std::cin, str, '\n');
    std::cout << "First sequence unitl ' ': " << firstWord(str) << '\n' <<
      "Word count: " << wordCount(str) << "\n";
    return 0;
}