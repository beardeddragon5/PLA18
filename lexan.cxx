/************************ lexan.cxx  Sommersemester 2018 ********************/
/*************** Scanner *******************************/


#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef ERROR_H
#include "error.h"
#endif

#ifndef LEXAN_H
#include "lexan.h"
#endif

#include <climits>
#include <cmath>
#include <map>

using namespace token;

map<string, type_t> keywords = {
	{ "const", 		CONST		},
	{ "var", 		VAR			},
	{ "procedure", 	PROCEDURE	},
	{ "call", 		CALL		},
	{ "begin", 		BEGIN		},
	{ "end", 		END			},
	{ "if", 		IF			},
	{ "then", 		THEN		},
	{ "else", 		ELSE		},
	{ "while", 		WHILE		},
	{ "do", 		DO			},
	{ "int", 		INT			},
	{ "real", 		REAL		},
	{ "fi", 		FI			}
};

/** 
 * Initalize a new lexan_t entity starting at line number 1 and reading the
 * first character from this->input
 */
lexan_t::lexan_t( ifstream& input, ostream& output, ostream& error )
	: lineno( 1 ), input( input ), output( output ), err( error ) {
	this->next();
}

void lexan_t::next() {
	this->input.get( this->actchar );
}

/**
 * nextsymbol returns the next symbol from the inputfile.
 * 
 * IMPORTANT!!!: lexan must always be pointing to the next character after the last found symbol
 * 
 * @param lex the used lexan instance with the current character and linenumber
 * @return the found symbol
 */
token_t lexan_t::nextsymbol() {
	while( !this->input.eof() ) {
		if ( isspace( this->actchar ) && this->actchar != '\n' ) {
			/* ignore whitespaces except for the newline */
			this->input.get(this->actchar);
		} else if ( this->actchar == '\n' ) {
			/* found newline => increment linenumber of lexan_t */
			this->input.get(this->actchar);
			this->lineno++;
			this->output << "line: " << this->lineno << endl;
		} else if ( isdigit(this->actchar) ) {
			/* found number. regex: [0-9]+\.?[0-9]* 	*/

			// TODO implement me

		} else if ( isalpha(this->actchar) ) {
			/* reg. Ausdruck   letter (letter|digit)*  erkennen ==>
			 * solange Buchstaben oder Ziffern folgen --> Identifikator 
			 */
			
			// TODO implement me

		} else {
			this->output.put(this->actchar);				/* Zeichen in Ausgabedatei */
			switch( this->actchar) {
				case '=':
					this->input.get(this->actchar);
					return EQ;

				// TODO implement other symbols

				default:
					this->error( error::INVALID_CHARACTER );
					break;
			} /* end-switch */
		} /* end-else */
	}/* end while */
 	return DONE; 	/* EIngabe -Ende erreicht */
}

namespace token {
	string toString( type_t type ) {
		switch( type ) {
			default:
			case INVALID: 	return "INVALID"; 
			case INTNUM: 	return "INTNUM";
			case REALNUM: 	return "REALNUM";
			case ID: 		return "ID";
			case CONST: 	return "CONST";
			case VAR: 		return "VAR";
			case PROCEDURE: return "PROCEDURE";
			case CALL: 		return "CALL";
			case BEGIN: 	return "BEGIN";
			case END: 		return "END";
			case IF: 		return "IF";
			case THEN: 		return "THEN";
			case ELSE: 		return "ELSE";
			case WHILE: 	return "WHILE";
			case DO: 		return "DO";
			case EQ: 		return "EQ";
			case NE: 		return "NE";
			case LT: 		return "LT";
			case LE: 		return "LE";
			case GT: 		return "GT";
			case GE: 		return "GE";
			case ASS: 		return "ASS";
			case KOMMA: 	return "KOMMA";
			case SEMICOLON: return "SEMICOLON";
			case PLUS: 		return "PLUS";
			case MINUS: 	return "MINUS";
			case MULT: 		return "MULT";
			case DIV: 		return "DIV";
			case KLAUF: 	return "KLAUF";
			case KLZU: 		return "KLZU";
			case PROGEND: 	return "PROGEND";
			case COLON: 	return "COLON";
			case INT: 		return "INT";
			case REAL: 		return "REAL";
			case FI: 		return "FI";
			case DONE: 		return "DONE";
		}
	}
}

std::ostream& operator<<( std::ostream& os, const token_t& symbol ) {
	os << "symbol(" << token::toString(symbol.type);

	switch ( symbol.type ) {
		case INTNUM:
			os << ", num = " << symbol.num << ")";
			break;
		case REALNUM:
			os << ", real = " << symbol.realnum << ")";
			break;
		case ID:
			os << ", name = " << symbol.idname << ")";
			break;
		default:
			os << ")";
			break;
	}
	return os;
}