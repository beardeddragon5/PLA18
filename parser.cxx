
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
void parser_t::factor(  ) {
	trace( "Faktor" );

	// je nach nächstem Eingabesymbol in lookahead
	switch( lookahead.type ) {
		case token::KLAUF:	// found '('
			next();
			this->exp();

			if ( lookahead != token::KLZU ) {
				lexan.error( error::NO_FACTOR ); // no factor
			}
			break;

		case token::INTNUM: // found int constant
		case token::REALNUM: // found real constant
			break;

		case token::ID: // found identifier 
			/**
			 * identifier must be declared so
			 * a valid entry in the symbol table must be set
			 */	
			{
				st_entry* found = lookup( lookahead.idname );
				if ( found == nullptr ) {
					// not found so exit with error
					lexan.error( error::IDENTIFIER_NOT_DECLARED );
				}
				symtype_t kind = found->token;
				// check witch kind of token it is
				switch ( kind ) {
					case KONST:
					case INTIDENT:
					case REALIDENT:
						break;
					case PROC:
						// procedures are not allowed
						lexan.error( error::PROCDURE_IN_EXPRESSION_NOT_ALLOWED );
				}		
			}
			break;
		default:
			lexan.error( error::NO_FACTOR );
	}
	next();
	traceend();
}

/**
 * term checks for term defined by EBNF
 * 		TERM ::= FACTOR { '*' FACTOR | '/' FACTOR }*
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 * @return type of term
 */
void parser_t::term( ) {
	trace( "term" );
	factor();

	// TODO: witch type ( implicit typecast ??? )

	// check for multiple factors seperated by '*' or '/'
	while ( lookahead == token::MULT || lookahead == token::DIV ) {
	  	next();
		factor();
	}
	traceend();
}

/**
 * exp checks for EXPRESSION defined by EBNF
 * 		EXPRESSION ::= TERM { '+' TERM | '-' TERM}*
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 * @return type of the expression
 */
void parser_t::exp() {
	trace( "expression" );

	term();
	// TODO: witch type ( implicit typecast ??? )
	// check for multipe term seperated by '+' or '-'
	while ( lookahead == token::PLUS || lookahead == token::MINUS ) {
		next();
		term();
	}
	traceend();
}

/**
 * condition checks for CONDITION defined by EBNF
 * 		CONDITION ::= EXPRESSION  RELOP  EXPRESSION
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 */
void parser_t::condition() {
	trace( "condition" );

	exp();
	// after expression there must be a relop 
	switch( lookahead.type ) {
		case token::EQ:
		case token::NE:
		case token::LT:
		case token::LE:
		case token::GT:
		case token::GE:
			next();
			exp();
			break;

		default: // kein relationaler Operator
			lexan.error( error::EXPECTED_RATIONAL_OPERATOR );
			break;
	}
	traceend();
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
void parser_t::statement() {
	trace( "statement" );
	// TODO implement me 
	traceend();
}

/**
 * procdecl checks for PROCDECL defined by EBNF
 * 		PROCDECL ::= {procedure IDENT ';' BLOCK ';' }*
 * 
 * found keyword procedure
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 */
void parser_t::procdecl() {
	trace( "procdecl" );
	// TODO implement me
	traceend();
}

/**
 * vardecl checks for VARDECL defined by EBNF
 * 		VARDECL ::= var IDENT ' : ' TYP  { ',' IDENT ' : ' TYP} *  ';'
 * 
 * found keyword var
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 */
void parser_t::vardecl( ) {
	trace( "vardecl" );
	// TODO implement me
	traceend();
}

/**
 * constdecl checks for CONSTDECL defined by EBNF
 * 		CONSTDECL ::= const IDENT '=' INTNUMBER {',' IDENT '=' INTNUMBER } * ';'
 * 
 * found keyword const
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 */
void parser_t::constdecl() {
  	trace( "constdecl" );
	// TODO implement me
  	traceend();
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
void parser_t::block( symtable* newsym ) {
	trace( "block" );
	// TODO implement me
	traceend();
}

/**
 * program checks for PROGRAM defined by EBNF
 * 		PROGRAM	::= BLOCK '$'
 * 
 * ne next symbol must be in set in parser and will set the next symbol to parser
 */
void parser_t::program() {
	trace( "programm" );

	// globale Symboltabelle  anlegen (firstsym
	firstsym = create_newsym();

	// Block muss folgen
	block( firstsym );

	//  nach Block muss '$' folgen
	if ( lookahead != token::PROGEND ) {
		lexan.error( error::EXPECTED_PROGEND );
	}
	next();

	// Dateiende erreicht ?
	if ( lookahead != token::DONE ) {
		lexan.error( error::EXPECTED_DONE );
	}
	traceend();
}
