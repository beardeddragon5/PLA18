
/****************** lexmain.cxx *********************/

/* Hauptprogramm zum Test der lexikalischen Analyse

		Stand: Sommersemester 2018

************************************************/

#ifndef INIT_H
#include "init.h"
#endif

int main(int argc, char** argv) {
	token_t lookahead;

	/* Initialisierungen der lexikalischen Analyse durchführen */
	lexan_t* lex = initialize(argc, argv);
	lexan_t& lexan = *lex;

	do {
		lookahead = nextsymbol( lexan );
		fout << "\t\tTokentyp=" << lookahead << endl;
	} while ( lookahead != DONE );

	return 0;
}
