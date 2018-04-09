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
			char zahl[BSIZE];
			int b = 0;
			bool isReal = false;

			this->output.put( this->actchar );
			zahl[b++] = this->actchar;
			this->next();
			while ( isdigit( this->actchar ) || ( !isReal && this->actchar == '.' ) ) {
				if ( this->actchar == '.' ) {
					isReal = true;
				}
				zahl[b++] = this->actchar;
				this->output.put( this->actchar );
				this->next();
				if ( b > BSIZE ) {
					this->error( error::NUMBER_TO_BIG );
				}
			}
			zahl[b] = '\0';

			if ( isReal ) {
				double real = strtod( zahl, nullptr );
				if ( errno == ERANGE && real == HUGE_VAL ) {
					this->error( error::NUMBER_TO_BIG ); // To big number
				}  
				return real;
			}
			long val = strtol( zahl, nullptr, 0 );
			if ( errno == ERANGE && ( val == LONG_MIN || val == LONG_MAX ) ) {
				this->error( error::NUMBER_TO_BIG ); // To big number
			}	
			if ( val > INT32_MAX || val < INT32_MIN ) {
				this->error( error::NUMBER_TO_BIG ); // To big number
			}
			return (int) val;
		} else if ( isalpha(this->actchar) ) {
			/* reg. Ausdruck   letter (letter|digit)*  erkennen ==>
			 * solange Buchstaben oder Ziffern folgen --> Identifikator 
			 */
			string ident;

			ident += this->actchar;
			this->output.put( this->actchar );

			while ( isalpha( this->actchar ) ||  isdigit( this->actchar ) ) {
				this->next();
				if ( isalpha( this->actchar ) ||  isdigit( this->actchar ) ) {
					ident += this->actchar;
					this->output.put( this->actchar );
				}
				if ( ident.length() > BSIZE ) {
					this->error( error::IDENTFIER_TO_BIG );
				}
			}

			if ( keywords.count( ident ) == 0 ) {
				return ident;
			}
			return keywords[ ident ];
		} else {
			this->output.put(this->actchar);				/* Zeichen in Ausgabedatei */
			switch( this->actchar) {
				case '=':
					this->input.get(this->actchar);
					return EQ;

				case '<':
					this->input.get( this->actchar);
					switch ( this->actchar ) {
						case '=':
							this->output.put(this->actchar);
							this->next();
							return LE;
						default:
							return LT;
					}

				case '!':
					this->next();
					this->output.put( this->actchar );
					if ( this->actchar != '=' ) {
						this->error( error::EXPECTED_EQUAL );
					}
					this->next();
					return NE;

				case '>':
					this->next();

					switch ( this->actchar ) {
						case '=':
							this->next();
							return GE;
						default:
							return GT;
					}

				case ':':
					this->next();
					switch ( this->actchar ) {
						case '=':
							this->output.put( this->actchar );
							this->next();
							return ASS;
						default:
							return COLON;
					}

				case ',':
					this->next();
					return KOMMA;
				
							case ';':
					this->next();
					return SEMICOLON;

				case '+':
					this->next();
					return PLUS;

				case '-':
					this->next();
					return MINUS;

				case '*':
					this->next();
					return MULT;

				case '/':
					this->next();
					return DIV;

				case '(':
					this->next();
					return KLAUF;

				case ')':
					this->next();
					return KLZU;

				case '$':
					this->next();
					return PROGEND;

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