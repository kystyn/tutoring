#ifndef FILE_FILLER_H
#define FILE_FILLER_H

#include <string>

void fillFile( std::string const &fileName );
void processFile( std::string const &inFileName,
                  std::string const &outFileName );
void runFileFill( void );

#endif // FILE_FILLER_H
