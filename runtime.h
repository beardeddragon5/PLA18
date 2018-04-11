
#include <map>
#include <vector>

#ifndef PARSER_H
#include "parser.h"
#endif

#ifndef RUNTIME_H
#define RUNTIME_H
#endif

struct symbol_t {
    enum type_t {
        INT,
        REAL,
        PROC,
        INT_KONST
    } type;

    union {
        int intValue;
        int procIndex;
        double realValue;
    };
};
std::ostream& operator<<( std::ostream& os, const symbol_t& symbol );


struct symtable_t {
    map<string, symbol_t> symbols;
    
    symtable_t* parent;
    vector<symtable_t*> children;

    symbol_t* lookup( string identifier );
};
std::ostream& operator<<( std::ostream& os, const symtable_t& symbol );


struct value_t {
    enum type_t {
        INT, REAL
    } type;
    union {
        double realValue;
        int intValue;
    };
};


struct program_t {
    symtable_t table;
    vector<token_t> tokens;

    int position;
    symtable_t* current;

    vector<value_t> arguments;

    void run(vector<string>&);

    private:
    void constdecl();
    void vardecl();
    void procdecl();
    value_t factor();
    value_t term();
    value_t exp();
    bool condition();
    void statement();
    void program(vector<string>&);
    void block();

    void skip_procedure();
    void skip_block();
    void skip_expression();
    void skip_term();
    void skip_factor();
    void skip_statement();
    void skip_condition();
};

void run_const( program_t& prg );
void run_block( program_t& prg );
void run_program( program_t& prg );