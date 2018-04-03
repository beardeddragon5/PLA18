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

/**
 * keyword_t is a struct mapping a keyword name used in pla18
 * to a tokentype used by the compiler
 */
struct keyword_t {
  string name;			
  tokentype_t token;							
};

/* Tabelle reservierter Worte */
struct keyword_t keywords [] = {
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
 * lookforKeyword looks if given key is a reserved keyword in pla.
 * 
 * This is used to check if a read string is a identifier or a keyword.
 * 
 * @param key string to look for
 * @return token of reserved keyword or INVALID 
 */
tokentype_t lookforKeyword( string key ) {
	int length = sizeof(keywords) / sizeof(keyword_t); 
	for ( int i = 0; i < length; i++ ) {
		if ( keywords[i].name == key ) {
			return keywords[i].token;
		}
	}
	return INVALID;
}

/** 
 * Initalize a new lexan_t entity starting at line number 1 and reading the
 * first character from fin
 */
lexan_t* initlexan() {
	lexan_t* lex = (lexan_t*) malloc( sizeof(lexan_t) );
	lex->lineno = 1;
	fin.get( lex->actchar );
	return lex;
}

/**
 * nextsymbol returns the next symbol from the inputfile.
 * 
 * IMPORTANT!!!: lexan must always be pointing to the next character after the last found symbol
 * 
 * @param lex the used lexan instance with the current character and linenumber
 * @return the found symbol
 */
token_t nextsymbol(lexan_t& lex) {
	while( !fin.eof() ) {
		if ( isspace( lex.actchar ) && lex.actchar != '\n' ) {
			/* ignore whitespaces except for the newline */
			fin.get(lex.actchar);
		} else if ( lex.actchar == '\n' ) {
			/* found newline => increment linenumber of lexan_t */
			fin.get(lex.actchar);
			lex.lineno++;
			fout << "line: " << lex.lineno << endl;
		} else if ( isdigit(lex.actchar) ) {
			/* found number. regex: [0-9]+\.?[0-9]* 	*/
			char zahl[BSIZE];
			int b = 0;
			bool isReal = false;

			fout.put( lex.actchar );
			zahl[b++] = lex.actchar;
			fin.get( lex.actchar );
			while ( isdigit( lex.actchar ) || ( !isReal && lex.actchar == '.' ) ) {
				if ( lex.actchar == '.' ) {
					isReal = true;
				}
				zahl[b++] = lex.actchar;
				fout.put( lex.actchar );
				fin.get( lex.actchar );
				if ( b > BSIZE ) {
					error( lex, 24 );
				}
			}
			zahl[b] = '\0';

			if ( isReal ) {
				double real = strtod( zahl, nullptr );
				if ( errno == ERANGE && real == HUGE_VAL ) {
					error(lex, 24); // To big number
				}  
				return real;
			}
			long val = strtol( zahl, nullptr, 0 );
			if ( errno == ERANGE && ( val == LONG_MIN || val == LONG_MAX ) ) {
				error(lex, 24); // To big number
			}	
			if ( val > INT32_MAX || val < INT32_MIN ) {
				error(lex, 24); // To big number
			}
			return (int) val;
		} else if ( isalpha(lex.actchar) ) {
			/* reg. Ausdruck   letter (letter|digit)*  erkennen ==>
			 * solange Buchstaben oder Ziffern folgen --> Identifikator 
			 */
			string ident;

			ident += lex.actchar;
			fout.put( lex.actchar );

			while ( isalpha( lex.actchar ) ||  isdigit( lex.actchar ) ) {
				fin.get( lex.actchar );
				if ( isalpha( lex.actchar ) ||  isdigit( lex.actchar ) ) {
					ident += lex.actchar;
					fout.put( lex.actchar );
				}
				if ( ident.length() > BSIZE ) {
					error( lex, 32 );
				}
			}

			tokentype_t type = lookforKeyword( ident );
			if ( type != INVALID ) {
				return type;
			}
			return ident;
		} else {
			fout.put(lex.actchar);				/* Zeichen in Ausgabedatei */
			switch( lex.actchar) {
				case '=':
					fin.get(lex.actchar);
					return EQ;

				case '<':
					fin.get(lex.actchar);
					switch ( lex.actchar ) {
						case '=':
							fout.put(lex.actchar);
							fin.get( lex.actchar );
							return LE;
						default:
							return LT;
					}

				case '!':
					fin.get( lex.actchar );
					fout.put( lex.actchar );
					if ( lex.actchar != '=' ) {
						error( lex, 32 );
					}
					fin.get( lex.actchar );
					return NE;

				case '>':
					fin.get( lex.actchar );

					switch ( lex.actchar ) {
						case '=':
							fin.get( lex.actchar );
							return GE;
						default:
							return GT;
					}

				case ':':
					fin.get( lex.actchar );
					switch ( lex.actchar ) {
						case '=':
							fout.put( lex.actchar );
							fin.get( lex.actchar );
							return ASS;
						default:
							return COLON;
					}

				case ',':
					fin.get( lex.actchar );
					return KOMMA;
				
							case ';':
					fin.get( lex.actchar );
					return SEMICOLON;

				case '+':
					fin.get( lex.actchar );
					return PLUS;

				case '-':
					fin.get( lex.actchar );
					return MINUS;

				case '*':
					fin.get( lex.actchar );
					return MULT;

				case '/':
					fin.get( lex.actchar );
					return DIV;

				case '(':
					fin.get( lex.actchar );
					return KLAUF;

				case ')':
					fin.get( lex.actchar );
					return KLZU;

				case '$':
					fin.get( lex.actchar );
					return PROGEND;

				default:
					error(lex, 32);
					break;
			} /* end-switch */
		} /* end-else */
	}/* end while */
 	return DONE; 	/* EIngabe -Ende erreicht */
}

string tokentype_toString( tokentype_t type ) {
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

std::ostream& operator<<( std::ostream& os, const token_t& symbol ) {
	os << "symbol(" << tokentype_toString(symbol.type);

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