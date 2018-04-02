
/**************** parser.cxx  Sommersemester 2018******************/

/*******   **************/

#ifndef PARSER_H
#include "parser.h"
#endif

#ifndef ERROR_H
#include "error.h"
#endif

// int lookahead;					/* lookahead enthält nächsten Eingabetoken */

int exp( parser_t& parser );


/******************  factor  **********************************************/
/* analysiert wird der korrekte Aufbau eines Faktors

Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead
*/
int factor( parser_t& parser ) {
	TRACE( parser, "Faktor" );

  	int kind;
  	st_entry *found;		// Zeiger auf Eintrag in ST
	int factor_typ;

	// je nach nächstem Eingabesymbol in lookahead
	switch( parser.lookahead.type ) {
		case KLAUF:	/* Symbol '(' folgt --> (EXPRESSION) erwartet*/
			parser.next();

			exp( parser );
			if ( parser.lookahead == KLZU )
				// korrekt ; nächstes Symbol lesen --> Ende
				parser.next();
			else
				error( parser.lexan, 27); // kein Faktor
			break;

		case INTNUM:
 			/* Int-Zahl (INTNUMBER) gefunden --> okay */
			parser.next();
			break;

		case REALNUM: 		/* Real-Zahl (REALNUMBER) gefunden --> okay */
			parser.next();
			break;

		case ID:	/* Identifikator (ID) gefunden  */
			/* Suche Identifikator in Symboltabelle ;
				angewandtes Auftreten -->
				Deklaration muss vorhanden sein
				und also Eintrag in ST */

			found = lookup( parser.lookahead.idname );

      		/* nicht gefunden --> Fehler: Id nicht deklariert*/
			if (found == NULL) {
				error( parser.lexan, 10);
			}
			// Id in ST gefunden ; Art prüfen
			kind = found->token;	// Art des ST-Eintrags

			switch(kind) {
				case KONST:	// Konstantenname --> okay
				case INTIDENT:// einfache Variable, Typ int --> okay
				case REALIDENT:// einfache Variable, Typ real --> okay
					break;
				case PROC:	// Name einer Prozedur in
					// Factor nicht erlaubt
					error( parser.lexan, 20); // --> exit
			}
			// nächstes Symbol lesen
			parser.next();
			break;

		default:	// kein korrekter Faktor
			error( parser.lexan, 27);
	}	// endswitch (lookahead)
	TRACE_END();
	return (0);
} 	// end factor


/******************  term ***************************************************/
/* analysiert wird der korrekte Aufbau eines Terms nach folgender Syntax:

			TERM	::=		FACTOR  { '*' FACTOR |  '/' FACTOR }*

Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead
							Typ des Terms ist Funktionswert

*/
int term( parser_t& parser ) {
	TRACE( parser, "Term" );
  	int ret;

	ret = factor( parser );
	// korrekter Factor

	while( parser.lookahead == MULT || parser.lookahead == DIV ) {
	  	// solange * oder / folgt, muss Factor kommen
    	// nächstes Symbol lesen
	  	parser.next();
		ret = factor( parser );
	}
	TRACE_END();
	return(0);
 }

/******************  exp ***************************************************/
/* analysiert wird der korrekte Aufbau eines Ausdrucks nach folgender Syntax:

			EXPRESSION	::=		TERM { '+' TERM |  '-' TERM}*

Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead
							Funktionswert ist Typ des Ausdrucks
*/
int exp( parser_t& parser ) {
	TRACE( parser, "Ausdruck" );
	int typ_left,typ_right;

	typ_left = term( parser );
	// korrekter Term

	while (parser.lookahead == PLUS || parser.lookahead == MINUS ) {
		// solange + oder - folgt, muss Term kommen
    	// nächstes Symbol lesen
		parser.next();
		// Term prüfen
		typ_right = term( parser );
		// nach korrektem Ende wurde nächstes Symbol gelesen
	}
	TRACE_END();
	return (0);
}

/******************  condition ***************************************************/
/* analysiert wird der korrekte Aufbau einer Bedingung nach folgender Syntax:

			CONDITION	::=		EXPRESSION  RELOP  EXPRESSION

Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead
*/
int condition( parser_t& parser ) {
	TRACE( parser, "Condition");
  	int typ_left, typ_right;

	typ_left = exp( parser );
	// korrekter Ausdruck
	// relationaler Operator muss folgen
	switch(parser.lookahead.type) {
		case EQ:
		case NE:
		case LT:
		case LE:
		case GT:
		case GE:// nächstes Symbol lesen
			parser.next();
			// Ausdruck muss folgen
			typ_right = exp( parser );
			break;

		default: // kein relationaler Operator
			error( parser.lexan, 19);
	}
	if (typ_left != typ_right)
		errortext( parser.lexan, "Typen der Operanden nicht kompatibel");

	TRACE_END();
	return(typ_left);
}

/****************** statement ***************************************************/
/* analysiert wird der korrekte Aufbau eines Statements nach folgender Syntax:

			STATEMENT 	::=		IDENT  ':=' EXPRESSION
							|	call IDENT
							|	begin STATEMENT { ';' STATEMENT }* end
							|	if CONDITION then STATEMENT [else STATEMENT ] fi
							|	while CONDITION do STATEMENT



Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/
void statement( parser_t& parser ) {
	TRACE( parser, "Statement");
	st_entry *found;		// Zeiger auf ST-Eintrag
	int typ_left, typ_right;

	// überprüfung des aktuellen lex. Symbols
	// TODO
	TRACE_END();
}



/****************** procdecl ***************************************************/
/* analysiert wird der korrekte Aufbau einer Prozedurdeklaration
nach folgender Syntax:

			PROCDECL 	::=		{procedure IDENT ';' BLOCK ';' }*



Schnittstelle:
	bei Aufruf :			erkannt wurde das Schlüsselwort procedure
							nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/
void procdecl( parser_t& parser ) {
	TRACE( parser, "Procdeklaration");
	st_entry* neu, *found;          // Zeiger auf ST-Eintrag

	symtable* neusym;		// Zeiger auf Symboltabelle

	// TODO
	TRACE_END();
}


/****************** vardecl ***************************************************/
/* analysiert wird der korrekte Aufbau einer Variablendeklaration
nach folgender Syntax:

			VARDECL 	::=		  var IDENT ' : ' TYP  { ',' IDENT ' : ' TYP} *  ';'


Schnittstelle:
	bei Aufruf :			erkannt wurde das Schlüsselwort var
							nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/
void vardecl( parser_t& parser ) {
	TRACE( parser, "Variablendeklaration");
  	st_entry* neu, *found;
	
	// nach var muss Identifikator folgen
	// TODO
  	TRACE_END();
}

/****************** constdecl ***************************************************/
/* analysiert wird der korrekte Aufbau einer Variablendeklaration
nach folgender Syntax:

			CONSTDECL 	::=	 const IDENT '=' INTNUMBER {',' IDENT '=' INTNUMBER } * ';'



Schnittstelle:
	bei Aufruf :			erkannt wurde das Schlüsselwort const
							nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/
void constdecl( parser_t& parser ) {
  	TRACE( parser, "Konstantendeklaration");
	st_entry *neu, *found;
	// auf const muss IDENT folgen
  	// TODO
  	TRACE_END();
}




/****************** block ***************************************************/

/* analysiert wird der korrekte Aufbau eines Blockes nach folgender Syntax:


		BLOCK		::= 	[ CONSTDECL ]
							[ VARDECL ]
							  PROCDECL
							STATEMENT



Der Parameter neusym ist ein Zeiger auf die Symboltabelle des zu
analysierenden Blockes
===>
		Äussersten (globalen)  Block:		firstsym
		bei Prozeduren:		Zeiger auf neu angelegte ST für Prozedur

Zu Beginn muss der globale Zeiger actsym auf die neue ST gesetzt werden
Vor Verlassen muss actsym wieder auf den vorigen Wert gesetzt werden


Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead


*/
/* symtable * neusym :	Zeiger auf neue ST */
void block( parser_t& parser, symtable * neusym) {
	TRACE( parser, "Block");

	// actsym auf neue Symboltabelle setzen

  	// TODO

	// bei Blockende : Symboltabelle zurücksetzen
	// actsym = Zeiger auf vorherige Symboltabelle
  	// TODO
	TRACE_END();
}


/****************** program    ***************************************************/
/* analysiert wird der korrekte Aufbau eines Programmes nach folgender Syntax:

			PROGRAM		::=		BLOCK	'$'

Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/
void program( parser_t& parser ) {
	TRACE( parser, "Programm" );

	// globale Symboltabelle  anlegen (firstsym
	firstsym = create_newsym();

	// Block muss folgen
	block( parser, firstsym );

	//  nach Block muss '$' folgen
	if (parser.lookahead == PROGEND)
		// nächstes Symbol lesen
		parser.next();
	else
		// korrektes Programmende fehlt
		error( parser.lexan, 31 );

	// Dateiende erreicht ?
	if (parser.lookahead != DONE)
		error( parser.lexan, 33 ); // noch Symbole in Eingabedatei nach RPOGRAM
	TRACE_END();
}
