
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
    parser_t( lexan_t& lexan, ostream& fsym, ostream& trace ) : 
        lexan( lexan ),
        fsym( fsym ),
        _trace( trace ) {
        this->next();
    }

    lexan_t& lexan;
    token_t lookahead;

    int traceLevel;

    ostream& fsym;
    ostream& _trace;

    void next() {
        lookahead = lexan.nextsymbol();
    }

    void trace( string info ) {
        for ( int i = 0; i < traceLevel; i++ ) {
            _trace << '\t';
        }
        traceLevel++;
        _trace << "Zeile[" << lexan.lineno << "]: " << info << endl;
    }

    void traceend() {
        traceLevel--;
    }
    
    void constdecl();
    void vardecl();
    void procdecl();
    void factor();
    void term();
    void exp();
    void condition();
    void statement();
    void program();
    void block( symtable* neusym );
};
