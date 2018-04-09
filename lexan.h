
#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef ERROR_H
#include "error.h"
#endif

#ifndef LEXAN_H
#define LEXAN_H
#endif


/* Definition der Codierung für Tokentypen */

namespace token {
    enum type_t {
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

    string toString( type_t type );
}

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
    token_t( ) : token_t( token::INVALID ) { }
    token_t( token::type_t type );
    token_t( int num ) : type( token::INTNUM ), num( num ) { };
    token_t( double real ) : type( token::REALNUM ), realnum( real ) { };
    token_t( string id ) : type( token::ID ), idname( id ) { };
    token_t( const token_t& symbol );
    ~token_t() {}

    token::type_t type;       /* type of the token */
    string idname;          /* name of identifier */
	union {
		double realnum;     /* value of double-konstant */
		int num;            /* value of integer-konstant */
	};

    token_t& operator= ( const token_t& symbol );
    bool operator== ( const token_t& b );
    bool operator!= ( const token_t& b );
    bool operator== ( const token::type_t& type );
    bool operator!= ( const token::type_t& type );
};
std::ostream& operator<<( std::ostream& os, const token_t& symbol );


/**
 * lexan_t holds information of the current character read from the inputfile and the
 * current line in the inputfile
 */
struct lexan_t {
    lexan_t( ifstream& input, ostream& output, ostream& error );

	int lineno;         /* Zeilennummer */
	char actchar;       /* gelesenes Zeichen */

    ifstream& input;
    ostream& output;
    ostream& err;

    token_t nextsymbol();
    void next();

    void error( error::error_t );
    void error( string );
    void warning( string );
};
