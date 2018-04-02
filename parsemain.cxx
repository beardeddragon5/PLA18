
/****************** parsemain.cxx ***************
			Stand Sommersemester 2018
*************************************************/

#ifndef INIT_H
#include "init.h"
#endif

#ifndef PARSER_H
#include "parser.h"
#endif


// Mainprogramm für Parser

int main(int argc, char** argv) {
	// Compiler  intialisieren
	lexan_t* lexan = initialize(argc, argv);
	parser_t parser( *lexan );

	// Analyse PROGRAM
	program( parser );
	// Analyse korrekt beendet --> ST ausgeben; Abschlussarbeiten
	stop();
	exit(0); 			// Erfolg
}
