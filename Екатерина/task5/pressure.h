#pragma once

#include <string>

// binary shifts
enum class measurement_res
{
    HIGH_SYS = 0,
    HIGH_DIAST = 1
};

int checkPressure( std::string const &fileName );
void runPressure( void );
