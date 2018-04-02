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

#define NORW   14   		/* Zahl der reservierten Worte */
#define RWSYMLEN  15		/* Max. Länge reservierter Symbole */

#include <climits>
#include <cmath>


// int lineno;					/* Zeilennummer */

// int num ;           /* Wert einer int-Konstanten  */
// double realnum; 		/* Wert einer real-Konstanten*/

// char idname[BSIZE];      /* Name eines Bezeichners ; wird vom Parser weiterverwendet */


// char actchar;       /* gelesenes Zeichen */


/*  Um Bezeichner von reservierten Symbolene unterscheiden zu können,
wird eine Tabelle reservierte Worte verwendet (restable).

Die Tabelle enthält Einträge für jedes Schlüsselwort, bestehend aus
dem Schlüsselwort selbst und dem zugehörigen Tokentyp (Codierung vgl.global.h):

Bei Erkennen eines möglichen Bezeichners wird zuerst die Tabelle
der reservierten Symbole durchsucht (lookforres);
wird ein Schlüsselwort gefunden, liefert lookforres den dem Schlüsselwort
zugeordneten Tokentyp; sonst 0. Bei Ergebnis 0 liegt dann tatsächlich ein
Bezeichner vor.
*/


/* Struktur eines Eintrags in der Tabelle reservierter Symbole */
struct ressw {
  char ressymbol [RWSYMLEN];			/* Symbol */
  int token;							/* zugehöriger Tokentyp */
};

/* Tabelle reservierter Worte */
struct ressw restable [] = {
  {"const", CONST},
	{"var", VAR},
	{"procedure", PROCEDURE},
	{"call", CALL},
	{"begin", BEGIN},
	{"end", END},
	{"if", IF},
	{"then", THEN},
	{"else",ELSE},
	{"while",WHILE},
	{"do", DO},
	{"int", INT},
	{"real", REAL},
	{"fi", FI}
};

/* Suchen nach einem reservierten Symbol */

/* Sucht in Tabelle reservierter Worte nach s und liefert zugehörigen Token
   falls gefunden,
   sonst 0
*/
int lookforres( const char *s ) {
	struct ressw *ptr;
	for ( ptr = restable; ptr < &restable[NORW]; ptr++ ) {
		if (strcmp(ptr->ressymbol, s) == 0) {
			return (ptr->token); /* Symbol gefunden */
		}
	}
	return(0); /* Symbol nicht gefunden */
}

/******************* Initialisieren des Scanners **********************/

/* wird aufgerufen von  initialize () aus init.cxx ;
nimmt Vorbesetzungen der Variablen num, realnum,  idname und lineno vor;
liest das erste Zeichen aus der Eingabe */
lexan_t* initlexan() {
	lexan_t* lex = (lexan_t*) malloc( sizeof(lexan_t) );
	lex->lineno = 1;
	fin.get( lex->actchar );
	return lex;
}

/*
*******************************************************************************
********** Funktion nextsymbol ************************************************
*******************************************************************************

**** zentrale Funktion zum Lesen des nächsten lexikalischen Symbols ***************
*****  identifiziert nächstes lexikalisches Symbol der Eingabe **********

***** WICHTIG!!!!!
		Bei Aufruf von nextsymbol muss  sich das nächste Eingabezeichen bereits in actchar befinden
*******

liefert Codierung des nächsten Symbols (token):
   - Konstante:				   token == INTNUM und Wert der Konstanten in Variable num
									       token == REALNUM und Wert in realnum
   - Bezeichner:				 token == ID  und Zeiger auf Name in idname
	 - Schlüsselwort:			 token == Tokentyp des reservierten Symbols nach Suche in restable
	 - Operatoren,Sonderzeichen :entsprechende Token
**/
symbol_t nextsymbol(lexan_t& lex) {
	int token;
	char lexbuf[BSIZE];		/* lokaler Puffer für Eingabezeichen */
	while( !fin.eof() ) {			/* Eingabe-Dateiende nicht erreicht */
    /* Blank und Tab in Ausgabedatei kopieren und überlesen */
		if ( lex.actchar == ' ' || lex.actchar == '\t' || lex.actchar == '\r' ) {
      		//fout.put(lex.actchar);
			fin.get(lex.actchar);

		/* Newline in Ausgabedatei kopieren, überlesen/entfernen, Zeilennummer erhöhen */
		} else if ( lex.actchar == '\n' ) {
			//fout.put(lex.actchar);
			fin.get(lex.actchar);
			lex.lineno++;

		/***** actchar ist Ziffer --> Konstanten erkennen  *****/
		} else if ( isdigit(lex.actchar) ) {
			char zahl [BSIZE];	 /* Puffer für Ziffern */
			int b = 0;				   /* Zeichenzahl*/

			fout.put( lex.actchar );
			zahl[b++] = lex.actchar;
			fin.get( lex.actchar );
			while ( isdigit( lex.actchar ) || lex.actchar == '.' ) {
				zahl[b++] = lex.actchar;
				fout.put( lex.actchar );
				fin.get( lex.actchar );
			}
			zahl[b] = '\0';
			
			bool isReal = false;
			for ( int i = 0; i < b; i++ ) {
				if ( zahl[i] == '.' ) {
					isReal = true;
					break;
				}
			}

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
		/***** actchar ist Buchstabe -->  Identifikatoren erkennen ****/
		} else if ( isalpha(lex.actchar) ) {
			int b = 0 ;				/* Zeichenzahl */
					/* reg. Ausdruck   letter (letter|digit)*  erkennen ==>
					solange Buchstaben oder Ziffern folgen --> Identifikator */
			string ident;

			ident += lex.actchar;
			fout.put(lex.actchar);

			while( isalpha(lex.actchar) ||  isdigit(lex.actchar)){
				fin.get(lex.actchar); // infinity loop
				if( isalpha(lex.actchar) ||  isdigit(lex.actchar)){
				ident += lex.actchar;
				fout.put(lex.actchar);
				}
			}
			if(lookforres(ident.c_str())){
				return symbol_t((tokentype_t) lookforres(ident.c_str()));
			}else{
				return symbol_t(ident);
			}
			
		/***** Sonderzeichen oder Operatoren erkennen ***************/
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
		case INVALID: return "INVALID"; 
		case INTNUM: return "INTNUM";
		case REALNUM: return "REALNUM";
		case ID: return "ID";
		case CONST: return "CONST";
		case VAR: return "VAR";
		case PROCEDURE: return "PROCEDURE";
		case CALL: return "CALL";
		case BEGIN: return "BEGIN";
		case END: return "END";
		case IF: return "IF";
		case THEN: return "THEN";
		case ELSE: return "ELSE";
		case WHILE: return "WHILE";
		case DO: return "DO";
		case EQ: return "EQ";
		case NE: return "NE";
		case LT: return "LT";
		case LE: return "LE";
		case GT: return "GT";
		case GE: return "GE";
		case ASS: return "ASS";
		case KOMMA: return "KOMMA";
		case SEMICOLON: return "SEMICOLON";
		case PLUS: return "PLUS";
		case MINUS: return "MINUS";
		case MULT: return "MULT";
		case DIV: return "DIV";
		case KLAUF: return "KLAUF";
		case KLZU: return "KLZU";
		case PROGEND: return "PROGEND";
		case COLON: return "COLON";
		case INT: return "INT";
		case REAL: return "REAL";
		case FI: return "FI";
		case DONE: return "DONE";
	}
}

std::ostream& operator<<( std::ostream& os, const symbol_t& symbol ) {
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