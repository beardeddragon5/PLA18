
#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef LEXAN_H
#include "lexan.h"
#endif

#ifndef INIT_H
#define INIT_H
#endif

#define MAX_SWITCH_LENGTH 10

int indexOfArg( int argc, char** argv, const string small, const string big );
char* safeArg( int argc, char** argv, int index );
std::streambuf* openWriteFile( string file );