#include <fstream>
#include <iostream>
#include "pressure.h"

int checkPressure( std::string const &fileName )
{
    std::ifstream file(fileName, std::ios::binary);

    if (!file)
        throw std::string("Bad file name ") + fileName;

    long long sumSystolic = 0, sumDiastolic = 0;
    int sys, diast;
    int count = 0;

    while (file >> sys)
    {
        sumSystolic += sys;
        if (!(file >> diast))
            throw std::string("Bad file format");
        sumDiastolic += diast;
        count++;
    }

    const int referenceSys = 130, referenceDiast = 80;

    int res = 0;
    if (sumSystolic / float(count) > referenceSys)
        res |= (1 << int(measurement_res::HIGH_SYS));
    if (sumDiastolic / float(count) > referenceDiast)
        res |= (1 << int(measurement_res::HIGH_DIAST));

    return res;
}

void runPressure( void )
{
    std::string fileName;
    std::cout << "Input file name: ";
    std::cin >> fileName;

    try
    {
        auto res = checkPressure(fileName);

        if (res == 0)
        {
            std::cout << "Nice pressure\n";
            return;
        }

        if (res & (1 << int(measurement_res::HIGH_SYS)))
          std::cout << "Too high systolic pressure\n";
        if (res & (1 << int(measurement_res::HIGH_DIAST)))
          std::cout << "Too high dyastolic pressure\n";
    } catch (std::exception const &e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }
}
