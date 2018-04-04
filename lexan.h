
#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef LEXAN_H
#define LEXAN_H
#endif

/* Definition der Codierung für Tokentypen */

enum tokentype_t {
    INVALID = 0, 
    INTNUM = 2561,      /* Int-Konstante */
    REALNUM,            /* Real-Konstante */
    ID = 257,           /* Identifikator */
    CONST,              /* Schlüsselwort const */
    VAR,                /* Schlüsselwort var */
    PROCEDURE,          /* Procedure */ 
    CALL,               /* call */
    BEGIN,              /* begin */
    END,                /* end */
    IF,                 /* if */
    THEN = 2651,        /* then */
    ELSE,               /* else */
    WHILE = 266,        /* while */
    DO,                 /* do */
    EQ,                 /* = */
    NE,                 /* != */
    LT,                 /* <  */
    LE,                 /* <= */
    GT,                 /* >  */
    GE,                 /* >= */
    ASS,                /* := */
    KOMMA,              /* ,  */
    SEMICOLON,          /* ;  */
    PLUS,               /* +  */
    MINUS,              /* -  */
    MULT,               /* *  */
    DIV,                /* /  */
    KLAUF,              /* (  */
    KLZU,               /* )  */
    PROGEND,            /*  $ */
    COLON,              /*  :  */
    INT,                /* int */
    REAL,               /* real */
    FI = 291,           /* fi */
    DONE = 300          /* Ende-Token */
};

/**
 * token_t holds a single symbol.
 * 
 * If the symbol is of type INTNUM, REALNUM, or ID the additional fields are populated.
 * 
 * INTNUM:      num
 * REALNUM:     real
 * ID:          idname
 */
struct token_t {
    token_t( ) : token_t( INVALID ) { }
    token_t( tokentype_t type );
    token_t( int num ) : type( INTNUM ), num( num ) { };
    token_t( double real ) : type( REALNUM ), realnum( real ) { };
    token_t( string id ) : type( ID ), idname( id ) { };
    token_t( const token_t& symbol );
    ~token_t() {}

    tokentype_t type;       /* type of the token */
    string idname;          /* name of identifier */
	union {
		double realnum;     /* value of double-konstant */
		int num;            /* value of integer-konstant */
	};

    token_t& operator= ( const token_t& symbol );
    bool operator== ( const token_t& b );
    bool operator!= ( const token_t& b );
    bool operator== ( const tokentype_t& type );
    bool operator!= ( const tokentype_t& type );
    
};

/**
 * lexan_t holds information of the current character read from the inputfile and the
 * current line in the inputfile
 */
struct lexan_t {
	int lineno;         /* Zeilennummer */
	char actchar;       /* gelesenes Zeichen */
};

void TRACE( lexan_t& lexan, string type );
void TRACE_END( );

lexan_t* initlexan();                       /* Scanner initialisieren */
token_t nextsymbol(lexan_t&); 				/* liest nächstes Symbol der Eingabe */	
int lookforres( char * );	/* sucht in Tabelle der res. Symbole nach Zeichenkette */

string tokentype_toString( tokentype_t type );
std::ostream& operator<<( std::ostream& os, const token_t& symbol );