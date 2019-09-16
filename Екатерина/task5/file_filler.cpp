#include <fstream>
#include <iostream>
#include <ctime>
#include "file_filler.h"

void fillFile( const std::string &fileName )
{
    const int minVal = -16000, maxVal = 16000;
    std::ofstream ofs(fileName);

    if (!ofs)
        throw std::string("Filler: Bad file name ") + fileName;

    std::cout << "Input count of numbers: ";
    int count;
    std::cin >> count;

    for (int i = 0; i < count; i++)
        ofs << minVal + rand() % (maxVal - minVal) << '\n';
    ofs << '\n';
}

void processFile( const std::string &inFileName,
                  const std::string &outFileName )
{
    std::ifstream ifs(inFileName, std::ifstream::binary);
    if (!ifs)
        throw std::string("Processor: Bad file name ") + inFileName;

    int minVal = 16300, val, minNum = 0, i = 0;

    while (ifs >> val)
    {
        if (val < minVal)
        {
            minVal = val;
            minNum = i;
        }
        i++;
    }

    ifs.close();
    ifs.open(inFileName);


    std::ofstream ofs(outFileName);
    if (!ofs)
        throw std::string("Processor: Bad file name ") + outFileName;

    // output up to minimal
    for (int i = 0; i < minNum; i++)
    {
        ifs >> val;
        ofs << val + 100 << '\n';
    }

    //min
    ifs >> val;
    ofs << val << '\n';

    for (; ifs >> val;)
        if (val != minVal)
            ofs << val << '\n';
    ofs << '\n';
}

void runFileFill( void )
{
    try {
        srand(time(nullptr));
        fillFile("file.in");
        processFile("file.in", "file.out");
    } catch (std::string const &s) {
        std::cout << "Error: " << s << "\n";
    }
}
