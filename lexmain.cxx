
/****************** lexmain.cxx *********************/

/* Hauptprogramm zum Test der lexikalischen Analyse

		Stand: Sommersemester 2018

************************************************/

#ifndef INIT_H
#include "init.h"
#endif

void help() {
	cout << "Usage: lexmain <inputfile>" << endl;
	exit( 1 );
}

int main(int argc, char** argv) {
	token_t lookahead;

	if ( argc != 2 ) {
		help();
	}

	string inputfile = argv[ 1 ];

	if ( access( inputfile.c_str(), F_OK ) == -1 ) {
		cerr << "Input file doesn't exist\n";
		exit(1);
	}

	ostream error(nullptr);
	error.rdbuf( cerr.rdbuf() );

	ostream output(nullptr);
	output.rdbuf( cout.rdbuf() );
 
	ifstream input( inputfile, ifstream::in );

	lexan_t lexan( input, output, error );
	do {
		lookahead = lexan.nextsymbol();
		lexan.output << "\t\tTokentyp=" << lookahead << endl;
	} while ( lookahead != token::DONE );

	return 0;
}
