/****************** error.cxx *************************/
/* enthält eine Reihe von Fehlertexten, die über ihre
Nummer verwendet werden können und jederzeit erweitert
werden können.

Zur Ausgabe der Fehlertexte wird die Funktion

	error ( int nr)

verwendet, der die Fehlernummer zu übergeben ist.
Die Funktion error fährt ein exit durch.

Alternativ kann die Funktion

	errortext (char *)

verwendet werden, die den angegebenen Text ausgibt


	Stand: Sommersemester 2018

*****************************************************/

#ifndef LEXAN_H
#include "lexan.h"
#endif

#ifndef ERROR_H
#include "error.h"
#endif

/***************** Fehlertexte **********************/


#include <map>

using namespace error;
map<error_t, string> errtext = {
	{ EXPECTED_EQUAL, "expected '=' character" },
	{ EXPECTED_ASS, "expected ':='" },
	{ EXPECTED_RATIONAL_OPERATOR, "expected comparision operator" },
	{ EXPECTED_ID, "expected identifier" },
	{ EXPECTED_END, "expected 'end'" },
	{ EXPECTED_THEN, "expected 'then'"},
	{ EXPECTED_IF, "expected 'if'"},
	{ EXPECTED_DO, "expected 'do'"},
	{ EXPECTED_SEMICOLON, "expected ';'"},
	{ EXPECTED_VAR, "expected 'var'"},
	{ EXPECTED_COLON, "expected ':'"},
	{ EXPECTED_TYPE, "expected 'int' or 'real'"},
	{ EXPECTED_KOMMA, "expected ','"},
	{ EXPECTED_CONST, "expected 'const'"},
	{ EXPECTED_EQ, "expected '='"},
	{ EXPECTED_INTNUM, "expected integer value"},
	{ EXPECTED_PROGEND, "expected '$'"},
	{ EXPECTED_PROGEND, "expected eof"},
	{ INVALID_CHARACTER, "invalid character" },
	{ SYMBOL_TABLE_FULL, "symbol table is full" },
	{ LEXEM_FIELD_FULL, "lexem field is full" },
	{ IDENTIFIER_NOT_DECLARED, "identifier not declared" },
	{ IDENTIFIER_ALREADY_DECLARED, "identifier allready declared" },
	{ NO_FACTOR, "no factor specified by ebnf" },
	{ NO_STATEMENT, "no statement specified by ebnf" },
	{ IDENTFIER_TO_BIG, "identifier to big" },
	{ NUMBER_TO_BIG, "number to big" },
	{ PROCDURE_IN_EXPRESSION_NOT_ALLOWED, "procedure in expression not allowed" },
	{ CONST_READONLY, "const is readonly" },
	{ PROCEDURE_NOT_ASSINABLE, "procedure is not assinable" }
};

/***************** Fehlerfunktion **********************/

/* Funktion gibt den der Nummer nr entsprechenden Fehlertext aus */
void lexan_t::error( error::error_t nr ) {
	this->err << "Zeile" << this->lineno << errtext[nr] << endl;
	exit(1);
}

/* Funktion gibt den angegebenen  Fehlertext aus */
void lexan_t::error( string text ) {
	this->err << "Zeile" << this->lineno << text << endl;
	exit(1);
}

/* Funktion gibt den angegebenen  Warnungstext aus */
void lexan_t::warning( string text ) {
	this->err << "Zeile" << this->lineno << text << endl;
}
