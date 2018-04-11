
/****************** parsemain.cxx ***************
			Stand Sommersemester 2018
*************************************************/

#ifndef INIT_H
#include "init.h"
#endif

#ifndef PARSER_H
#include "parser.h"
#endif

void help() {
	cout << "Usage: parsemain [-t trace] <inputfile>" << endl;
	cout << "Help:" << endl;
	cout << "\t-t --trace: output the trace to the trace file";
	exit( 1 );
}

// Mainprogramm für Parser

int main(int argc, char** argv) {
	// Compiler  intialisieren
	if ( argc == 1 ) {
		help();
	}

	char* tracefile = "";
	if ( argc > 2 && argc == 4 ) {
		tracefile = argv[ 2 ];
	} else if ( argc != 2 ) {
		help();
	}

	char* inputfile = argv[ argc - 1 ];
	if ( access( inputfile, F_OK ) == -1 ) {
		cerr << "Input file doesn't exist\n";
		exit(1);
	}

	ostream error(nullptr);
	error.rdbuf( cerr.rdbuf() );
	ostream output(nullptr);
	ifstream input( inputfile, ifstream::in );

	ostream trace( nullptr );
	trace.rdbuf( openWriteFile( tracefile ) );

	ostream fsym( nullptr );
	fsym.rdbuf( cout.rdbuf() );

	lexan_t lexan( input, output, error );
	parser_t parser( lexan, fsym, trace );

	// Analyse PROGRAM
	parser.program();
	// Analyse korrekt beendet --> ST ausgeben; Abschlussarbeiten
	printsymtab( parser, firstsym );
	exit(0); 			// Erfolg
}
