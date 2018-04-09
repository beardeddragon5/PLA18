/******************************** init.cxx ************************/



/* Initialisierungen für Compiler PLA

		Stand Sommersemester 2018


*/


#ifndef INIT_H
#include "init.h"
#endif

#ifndef SYMBOL_H
#include "symbol.h"
#endif

/***************** Initialisierungen durchführen ******************/

/**
* Searching for the first index of argument.
* @param argc count of arguments
* @param argv array of arguments
* @param small small version of the argument switch
* @param big big version of the argument switch
* @return index of argument or -1
*/
int indexOfArg( int argc, char** argv, const string small, const string big ) {
	for (int i = 0; i < argc; i++ ) {
		if ( strncmp(argv[i], small.c_str(), MAX_SWITCH_LENGTH) == 0 || strncmp(argv[i], big.c_str(), MAX_SWITCH_LENGTH) == 0 ) {
			return i;
		}
	}
	return -1;
}

/**
* Look if index is in array range.
* @param argc count of arguments
* @param argv arguments of the program
* @param index index to check
* @return argument or nullpointer
*/
char* safeArg( int argc, char** argv, int index ) {
	if ( index >= argc ) {
		return nullptr;
	}
	return argv[index];
}

/**
* Opens File for write access and returns buffer
* @param file filename to open
* @return streambuf of the filestream
*/
std::streambuf* openWriteFile( string file ) {
	ofstream* filestream = new ofstream();
	filestream->open( file, ios::out );
	return filestream->rdbuf();
}
