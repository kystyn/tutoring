#pragma once

#ifdef STRING_LIB
#define STRINGLIB_API __declspec(dllexport)
#else
#define STRINGLIB_API __declspec(dllimport)
#endif

#include <string>

STRINGLIB_API std::string firstWord( std::string const &str );
STRINGLIB_API int wordCount( std::string const &str );
