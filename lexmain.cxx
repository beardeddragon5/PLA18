
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

	/* 1. Symbol lesen  */
	lookahead = nextsymbol( lexan );

	/* Symbol ausgeben */
	fout << "		Tokentyp=    " << lookahead;

	 /* Symbole aus Eingabedatei lesen bis Dateiende */
  	while (lookahead != DONE) {
		lookahead = nextsymbol( lexan );
		fout << "		Tokentyp=    " << lookahead;
 	}
	exit(0); 	/* Erfolg */
}
