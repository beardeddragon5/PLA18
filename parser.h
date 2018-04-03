
#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef LEXAN_H
#include "lexan.h"
#endif

#ifndef SYMBOL_H
#include "symbol.h"
#endif

#ifndef PARSER_H
#define PARSER_H
#endif



struct parser_t {
    parser_t( lexan_t& lexan ) : lexan( lexan ) {
        lookahead = nextsymbol( lexan );
    }

    lexan_t& lexan;
    token_t lookahead;

    void next() {
        lookahead = nextsymbol( lexan );
    }
};

static inline void TRACE( parser_t& parser, string type ) {
    TRACE( parser.lexan, type );
}

enum type_t {
    TYPE_REAL,
    TYPE_INT
};

void constdecl( parser_t& parser );					/* Verarbeiten einer Konstantendeklaration */
void vardecl( parser_t& parser ); 					/* Verarbeiten einer VAriablendeklaration */
void procdecl( parser_t& parser ); 					/* Verarbeiten einer Prozedurdeklaration */
type_t factor( parser_t& parser ); 						/* Verarbeiten eines Faktors */
type_t term( parser_t& parser );								/* Verarbeiten eines Terms */
type_t exp( parser_t& parser ); 								/* Verarbeiten eines Ausdrucks */
void condition( parser_t& parser ); 					/* Verarbeiten einer Bedingung */
void statement( parser_t& parser );					/* Verarbeiten Statement */
void program( parser_t& parser ); 					/* Programm übersetzen */
void block( parser_t& parser, symtable* neusym);		/* Bearbeiten eines Blockes */