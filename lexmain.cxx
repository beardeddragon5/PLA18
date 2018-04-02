
/****************** lexmain.cxx *********************/

/* Hauptprogramm zum Test der lexikalischen Analyse

		Stand: Sommersemester 2018






************************************************/


#ifndef INIT_H
#include "init.h"
#endif

int main(int argc, char** argv) {
	symbol_t lookahead( INVALID );

	/* Initialisierungen der lexikalischen Analyse durchführen */
	lexan_t* lex = initialize(argc, argv);
	lexan_t& lexan = *lex;

	int lineno = -1;

	do {
		if ( lexan.lineno != lineno ) {
			fout << "Line: " << lexan.lineno << endl;
			lineno = lexan.lineno;
		}
		lookahead = nextsymbol( lexan );
		fout << "\t\tTokentyp=" << lookahead << endl;
	} while ( lookahead != DONE );

	exit(0); 	/* Erfolg */
}
