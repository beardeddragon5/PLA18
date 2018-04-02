
#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef LEXAN_H
#define LEXAN_H
#endif

/* Definition der Codierung für Tokentypen */

enum tokentype_t {
    INVALID = -1, 
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

struct symbol_t {
    symbol_t( ) : symbol_t( INVALID ) { }
    symbol_t( tokentype_t type ): type( type ) {
        switch ( type ) {
            default:
            case INTNUM:
                num = 0;
                break;
            case REALNUM:
                realnum = 0.0;
                break;
            case ID:
                idname = "";
                break;
        }
    };
    symbol_t( int num ) : type( INTNUM ), num( num ) { };
    symbol_t( double real ) : type( REALNUM ), realnum( real ) { };
    symbol_t( string id ) : type( ID ), idname( id ) { };
    symbol_t( const symbol_t& symbol ) {
        *this = symbol;
    }
    ~symbol_t() {}

    tokentype_t type;       /* type of the token */
    string idname;      /* name of identifier */
	union {
		double realnum;     /* value of double-konstant */
		int num;            /* value of integer-konstant */
	};

    symbol_t& operator=(const symbol_t& symbol) {
        this->type = symbol.type;
        switch ( this->type ) {
            case INTNUM:
                this->num = symbol.num;
                break;
            case REALNUM:
                this->realnum = symbol.realnum;
                break;
            case ID:
                this->idname = symbol.idname;
                break;
            default:
                break;
        }
        return *this;
    }

    bool operator==(const symbol_t& b) {
        if ( this->type != b.type ) {
            return false;
        }
        switch ( this->type ) {
            case INTNUM:
                return this->num == b.num;
            case REALNUM:
                return this->realnum == b.realnum;
            case ID:
                return this->idname == b.idname;
            default:
                return true;
        }
    }

    bool operator!=(const symbol_t& b) {
        return ! ( *this == b );
    }

};



struct lexan_t {
	int lineno;         /* Zeilennummer */
	char actchar;       /* gelesenes Zeichen */
};

static int traceLevel = 0;
static inline void TRACE( lexan_t& lexan, string type ) {
    if ( tracesw == 1 ) {
        for ( int i = 0; i < traceLevel; i++ ) {
            trace << '\t';
        }
        traceLevel++;
        trace << "Zeile: " << lexan.lineno << " " << type << endl;
    }
}

static inline void TRACE_END() {
    traceLevel--;
}

lexan_t* initlexan();                       /* Scanner initialisieren */
symbol_t nextsymbol(lexan_t&); 				/* liest nächstes Symbol der Eingabe */	
int lookforres( char * );	/* sucht in Tabelle der res. Symbole nach Zeichenkette */

string tokentype_toString( tokentype_t type );
std::ostream& operator<<( std::ostream& os, const symbol_t& symbol );