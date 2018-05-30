/****************** error.cxx *************************/
/* enth�lt eine Reihe von Fehlertexten, die �ber ihre
Nummer verwendet werden k�nnen und jederzeit erweitert
werden k�nnen.

Zur Ausgabe der Fehlertexte wird die Funktion

	error ( int nr)

  verwendet, der die Fehlernummer zu �bergeben ist.
Die Funktion error f�hrt ein exit durch.

*****************************************************/

#include <stdlib.h>
#include "error.h"

/* Prototyp der Bison-Fehlermelderoutine */
void yyerror(char *);

/***************** Fehlertexte **********************/

#include <map>
#include <string>

std::map<error_t, std::string> errtext = {
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
	{ PROCEDURE_NOT_ASSINABLE, "procedure is not assinable" },
	{ IDENTIFIER_IS_NOT_CALLABLE, "identifier is not callable"},
	{ UNSUPPORTED_TYPE, "unsupported type"}
};

/***************** Fehlerfunktion **********************/

/* Funktion gibt den der Nummer nr entsprechenden Fehlertext aus */
void error( enum error_t nr) {
	const char* str = errtext[nr].c_str();
	yyerror((char*)str);
	exit(1);
}
