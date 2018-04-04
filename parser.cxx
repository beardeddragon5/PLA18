
#ifndef PARSER_H
#include "parser.h"
#endif

#ifndef ERROR_H
#include "error.h"
#endif

/**
 * factor checks for factor defined by EBNF
 * 		FACTOR ::= IDENT | NUMBER | ‘(‘ EXPRESSION ‘) ‘
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 * @return type of the factor returned
 */
type_t factor( parser_t& parser ) {
	TRACE( parser, "Faktor" );
	type_t out;

	// je nach nächstem Eingabesymbol in lookahead
	switch( parser.lookahead.type ) {
		case KLAUF:	// found '('
			parser.next();
			out = exp( parser );

			if ( parser.lookahead != KLZU ) {
				error( parser.lexan, 27 ); // no factor
			}
			break;

		case INTNUM: // found int constant
			out = TYPE_INT;
			break;
		case REALNUM: // found real constant
			out = TYPE_REAL;
			break;

		case ID: // found identifier 
			/**
			 * identifier must be declared so
			 * a valid entry in the symbol table must be set
			 */	
			{
				st_entry* found = lookup( parser.lookahead.idname );
				if ( found == nullptr ) {
					// not found so exit with error
					error( parser.lexan, 10);
				}
				symtype_t kind = found->token;
				// check witch kind of token it is
				switch ( kind ) {
					case KONST:
					case INTIDENT:
						out = TYPE_INT;
						break;
					case REALIDENT:
						out = TYPE_REAL;
						break;
					case PROC:
						// procedures are not allowed
						error( parser.lexan, 20);
				}		
			}
			break;
		default:
			error( parser.lexan, 27);
	}
	parser.next();
	TRACE_END();
	return out;
}

/**
 * term checks for term defined by EBNF
 * 		TERM ::= FACTOR { '*' FACTOR | '/' FACTOR }*
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 * @return type of term
 */
type_t term( parser_t& parser ) {
	TRACE( parser, "Term" );

	type_t out = factor( parser );

	// TODO: witch type ( implicit typecast ??? )

	// check for multiple factors seperated by '*' or '/'
	while ( parser.lookahead == MULT || parser.lookahead == DIV ) {
	  	parser.next();
		type_t lefttype = factor( parser );
		if ( lefttype == TYPE_REAL ) {
			out = lefttype;
		}
	}
	TRACE_END();
	return out;
}

/**
 * exp checks for EXPRESSION defined by EBNF
 * 		EXPRESSION ::= TERM { '+' TERM | '-' TERM}*
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 * @return type of the expression
 */
type_t exp( parser_t& parser ) {
	TRACE( parser, "Ausdruck" );

	type_t out = term( parser );
	// TODO: witch type ( implicit typecast ??? )
	// check for multipe term seperated by '+' or '-'
	while (parser.lookahead == PLUS || parser.lookahead == MINUS ) {
		parser.next();
		out = term( parser );
	}
	TRACE_END();
	return out;
}

/**
 * condition checks for CONDITION defined by EBNF
 * 		CONDITION ::= EXPRESSION  RELOP  EXPRESSION
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 */
void condition( parser_t& parser ) {
	TRACE( parser, "Condition");
	type_t left, right;

	left = exp( parser );
	// after expression there must be a relop 
	switch( parser.lookahead.type ) {
		case EQ:
		case NE:
		case LT:
		case LE:
		case GT:
		case GE:
			parser.next();
			right = exp( parser );
			break;

		default: // kein relationaler Operator
			error( parser.lexan, 19);
			break;
	}
	if ( left != right ) {
		errortext( parser.lexan, "Typen der Operanden nicht kompatibel");
	}
	TRACE_END();
}

/**
 * statement checks for STATEMENT defined by EBNF
 * 		STATEMENT ::= IDENT  ':=' EXPRESSION
 *					  | call IDENT
 *					  | begin STATEMENT { ';' STATEMENT }* end
 *					  | if CONDITION then STATEMENT [else STATEMENT ] fi
 *					  | while CONDITION do STATEMENT
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 */
void statement( parser_t& parser ) {
	TRACE( parser, "Statement");
	st_entry *found;		// Zeiger auf ST-Eintrag
	int typ_left, typ_right;

	switch ( parser.lookahead.type ) {
		case ID:
			st_entry* entry = lookup( parser.lookahead.idname );
			if ( entry == nullptr ) {
				error( parser.lexan, 10 ); 
			}    
			switch ( entry->token ) {
				case KONST:
					error( parser.lexan, 43 );
				case INTIDENT:
					typ_left = TYPE_INT;
					break;
				case REALIDENT:
					typ_left = TYPE_REAL;
					break;
				case PROC:
					error( parser.lexan, 20 );
			}

			parser.next();
			if ( parser.lookahead != ASS ) {
				error( parser.lexan, 12 );
			}
			parser.next();
			typ_right = exp( parser );

			if ( typ_left != typ_right ) {
				error( parser.lexan, 36 );
			}
			break;
		case CALL:
			parser.next();

			if ( parser.lookahead != ID ) {
				error( parser.lexan, 4 );
			}

			st_entry* entry = lookup( parser.lookahead.idname );
			if ( entry == nullptr ) {
				error( parser.lexan, 10 ); 
			}
			if ( entry->token != PROC ) {
				error( parser.lexan, 36 );
			}
			parser.next();
			break;
		case BEGIN:
			do {
				parser.next();
				statement( parser );
			} while( parser.lookahead == SEMICOLON );
			
			if ( parser.lookahead != END ) {
				error( parser.lexan, 16 );
			}
			break;
		case IF:
			parser.next();
			condition( parser );

			if ( parser.lookahead != THEN ) {
				error( parser.lexan, 15 );
			}
			parser.next();
			statement( parser );
			
			switch ( parser.lookahead.type ) {
				case FI:
					parser.next();
					break;
				case ELSE:
					parser.next();
					statement( parser );
					break;
				default:
					error( parser.lexan, 39 );
			}
			break;
		case WHILE:
			parser.next();
			condition( parser );
			if ( parser.lookahead != DO ) {
				error( parser.lexan, 17 );
			}
			statement( parser );
			break;
		default:
			error( parser.lexan, 30 );
	}
	TRACE_END();
}

/**
 * procdecl checks for PROCDECL defined by EBNF
 * 		PROCDECL ::= {procedure IDENT ';' BLOCK ';' }*
 * 
 * found keyword procedure
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 */
void procdecl( parser_t& parser ) {
	TRACE( parser, "Procdeklaration");
	while ( parser.lookahead == PROCEDURE ) {
		parser.next();

		if ( parser.lookahead.type != ID ) {
			error( parser.lexan, 4 );
		}

		if ( lookup_in_actsym( parser.lookahead.idname ) != nullptr ) {
			error( parser.lexan, 34 );
		}

		st_entry* neu = insert( parser.lexan, PROC, parser.lookahead.idname );
		
		parser.next();

		if ( parser.lookahead != SEMICOLON ) {
			error( parser.lexan, 16 );
		}
		parser.next();

		symtable* neusym = create_newsym();
		block( parser, neusym );

		if ( parser.lookahead != SEMICOLON ) {
			error( parser.lexan, 5 );
		}
		parser.next();
	}
	TRACE_END();
}

/**
 * vardecl checks for VARDECL defined by EBNF
 * 		VARDECL ::= var IDENT ' : ' TYP  { ',' IDENT ' : ' TYP} *  ';'
 * 
 * found keyword var
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 */
void vardecl( parser_t& parser ) {
	TRACE( parser, "Variablendeklaration");
  	st_entry* neu, *found;
	
	if(parser.lookahead != VAR){
		//var erwartet
		error(parser.lexan , 40);
	}
	parser.next();

	if(parser.lookahead.type != ID){
		error(parser.lexan , 4);
	}else{
		//doppel declaration
		if(lookup_in_actsym(parser.lookahead.idname)){
			error(parser.lexan , 42);
		}
	}
	parser.next();

	if(parser.lookahead != COLON){
		error(parser.lexan , 41);
	}
	parser.next();

	if(parser.lookahead.type != INT && parser.lookahead.type != REAL){
		error(parser.lexan , 36);
	}
	parser.next();

	while(parser.lookahead != SEMICOLON){

		if(parser.lookahead != KOMMA){
			error(parser.lexan , 5);
		}
		parser.next();

		if(parser.lookahead.type != ID){
			error(parser.lexan , 4);
		}
		parser.next();

		if(parser.lookahead != COLON){
			error(parser.lexan , 41);
		}
		parser.next();

		if(parser.lookahead.type != INT && parser.lookahead.type != REAL){
			error(parser.lexan , 36);
		}
		parser.next();

	}

  	TRACE_END();
}

/**
 * constdecl checks for CONSTDECL defined by EBNF
 * 		CONSTDECL ::= const IDENT '=' INTNUMBER {',' IDENT '=' INTNUMBER } * ';'
 * 
 * found keyword const
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 */
void constdecl( parser_t& parser ) {
  	TRACE( parser, "Konstantendeklaration");
	st_entry *neu, *found;


	if(parser.lookahead != CONST){
		//var erwartet
		error(parser.lexan , 43);
	}
	parser.next();

	if(parser.lookahead.type != ID){
		error(parser.lexan , 4);
	}else{
		//doppel declaration
		if(lookup_in_actsym(parser.lookahead.idname)){
			error(parser.lexan , 42);
		}
	}
	parser.next();

	if(parser.lookahead != EQ){
		error(parser.lexan , 43);
	}
	parser.next();

	if(parser.lookahead.type != INT){
		error(parser.lexan , 36);
	}
	parser.next();

	while(parser.lookahead != SEMICOLON){

		if(parser.lookahead != KOMMA){
			error(parser.lexan , 5);
		}
		parser.next();

		if(parser.lookahead.type != ID){
			error(parser.lexan , 4);
		}
		parser.next();

		if(parser.lookahead != EQ){
			error(parser.lexan , 43);
		}
		parser.next();

		if(parser.lookahead.type != INT){
			error(parser.lexan , 36);
		}
		parser.next();

	}


  	TRACE_END();
}

/**
 * block checks for BLOCK defined by EBNF
 * 		BLOCK ::= [ CONSTDECL ] [ VARDECL ] PROCDECL STATEMENT
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 * 
 * sets the current active symbolic table to newsym and resets it before execting the block
 * 
 * @param newsym 
 *   is the new symbolic table. This can ether be the global table or the local table of a procedur
 * 
 */
void block( parser_t& parser, symtable* newsym ) {
	TRACE( parser, "Block");

	// actsym auf neue Symboltabelle setzen

  	// TODO

	// bei Blockende : Symboltabelle zurücksetzen
	// actsym = Zeiger auf vorherige Symboltabelle
  	// TODO
	TRACE_END();
}

/**
 * program checks for PROGRAM defined by EBNF
 * 		PROGRAM	::= BLOCK '$'
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
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
