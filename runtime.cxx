
#ifndef RUNTIME_H
#include "runtime.h"
#endif

#include <climits>
#include <cmath>
#include <sstream>

void program_t::constdecl() {
    do {
        string id = tokens[position].idname;
        position += 2;
        int num = tokens[position].num;
        position++;

        symbol_t symbol = { symbol_t::INT_KONST, num };
        current->symbols[ id ] = symbol;
    } while ( tokens[position++] != token::SEMICOLON );
}

void program_t::vardecl() {
    do {
        string id = tokens[position].idname;
        position += 2;
        symbol_t::type_t type = tokens[position] == token::INT ? symbol_t::INT : symbol_t::REAL;
        position++;

        symbol_t symbol = { type };
        if ( type == symbol_t::INT ) {
            symbol.intValue = 0;
        } else {
            symbol.realValue = 0.0;
        }
        
        current->symbols[ id ] = symbol;
    } while ( tokens[position++] != token::SEMICOLON );
}

void program_t::procdecl() {
    while ( tokens[position] == token::PROCEDURE ) {
        position++;
        string id = tokens[position].idname;
        position++;
        position++;
        symbol_t symbol = { symbol_t::PROC };
        
        symbol.procIndex = position;
        skip_block();
        position++;

        current->symbols[ id ] = symbol;
    }
}

value_t program_t::factor() {
    value_t v;
    switch( tokens[position].type ) {
        case token::ID: {
            string id = tokens[position].idname;
            symbol_t* sym = current->lookup( id );
            position++;
            switch ( sym->type ) {
                case symbol_t::INT:
                case symbol_t::INT_KONST:
                    v.type = value_t::INT;
                    v.intValue = sym->intValue;
                    break;
                case symbol_t::REAL:
                    v.type = value_t::REAL;
                    v.realValue = sym->realValue;
                    break;

            }
            break;
        } 
        case token::INTNUM:
            v.type = value_t::INT;
            v.intValue = tokens[position].num;
            position++;
            break;
        case token::REALNUM:
            v.type = value_t::REAL;
            v.realValue = tokens[position].realnum;
            position++;
            break;
        case token::KLAUF:
            position++;
            v = exp();
            position++;
            break;
    }
    return v;
}

value_t program_t::term() {
    value_t v = factor();
    while ( tokens[position] == token::MULT || tokens[position] == token::DIV ) {
        token::type_t type = tokens[position].type;
        position++;

        value_t t = factor();
        if ( v.type == value_t::INT && t.type == value_t::REAL ) {
            v.type = value_t::REAL;
            v.realValue = (double) v.intValue;
        }
        if ( v.type == value_t::INT ) {
            if ( type == token::MULT ) {
                v.intValue *= t.intValue;
            } else {
                v.intValue /= t.intValue;
            }
        } else {
            if ( type == token::MULT ) {
                v.realValue *= t.realValue;
            } else {
                v.realValue /= t.realValue;
            }
        }
    }
    return v;
}

value_t program_t::exp() {
    value_t v = term();
    while ( tokens[position] == token::PLUS || tokens[position] == token::MINUS ) {
        token::type_t type = tokens[position].type;
        position++;

        value_t t = term();
        if ( v.type == value_t::INT && t.type == value_t::REAL ) {
            v.type = value_t::REAL;
            v.realValue = (double) v.intValue;
        }
        if ( v.type == value_t::INT ) {
            if ( type == token::PLUS ) {
                v.intValue += t.intValue;
            } else {
                v.intValue -= t.intValue;
            }
        } else {
            if ( type == token::PLUS ) {
                v.realValue += t.realValue;
            } else {
                v.realValue -= t.realValue;
            }
        }
    }
    return v;
}

bool program_t::condition() {
    value_t a = exp();
    token::type_t type = tokens[position++].type;
    value_t b = exp();

    if ( a.type != b.type ) {
        if ( a.type == value_t::INT ) {
            a.type = value_t::REAL;
            a.realValue = (double) a.intValue;
        } else {
            b.type = value_t::REAL;
            b.realValue = (double) b.intValue;
        }
    }

    switch( type ) {
        case token::EQ:
            if ( a.type == value_t::INT ) {
                return a.intValue == b.intValue;
            } else {
                return a.realValue == b.realValue;
            }
        case token::NE:
            if ( a.type == value_t::INT ) {
                return a.intValue != b.intValue;
            } else {
                return a.realValue != b.realValue;
            }
        case token::LT:
            if ( a.type == value_t::INT ) {
                return a.intValue < b.intValue;
            } else {
                return a.realValue < b.realValue;
            }
        case token::LE:
            if ( a.type == value_t::INT ) {
                return a.intValue <= b.intValue;
            } else {
                return a.realValue <= b.realValue;
            }
        case token::GT:
            if ( a.type == value_t::INT ) {
                return a.intValue > b.intValue;
            } else {
                return a.realValue > b.realValue;
            }
        case token::GE:
            if ( a.type == value_t::INT ) {
                return a.intValue >= b.intValue;
            } else {
                return a.realValue >= b.realValue;
            }
    }
}

void program_t::statement() {
    switch( tokens[position].type ) {
        case token::ID: {
            string id = tokens[position++].idname;
            position++;
            symbol_t* var = current->lookup( id );
            value_t value = exp();

            if ( var->type == symbol_t::INT ) {
                if ( value.type == value_t::INT ) {
                    var->intValue = value.intValue;
                } else {
                    var->intValue = (int) value.realValue;
                }
            } else {
                if ( value.type == value_t::INT ) {
                    var->realValue = value.intValue;
                } else {
                    var->realValue = value.realValue;
                }
            }

            if ( id == ASCII_OUT ) {
                char c = (char) var->intValue;
                cout << c;
            } else if ( id == INT_OUT ) {
                cout << var->intValue;
            } else if ( id == REAL_OUT ) {
                cout << var->realValue;
            }
            break;
        }
        case token::CALL: {
            position++;
            string id = tokens[position].idname;
            position++;
            
            if ( id == ARG_PROC ) {
                symbol_t* var = current->lookup( ARG_IDX );
                value_t value = arguments[ var->intValue ];
                symbol_t* out = current->lookup( ARG_OUT );
                out->type = value.type == value_t::INT ? symbol_t::INT : symbol_t::REAL;
                if ( value.type == value_t::INT ) {
                    out->intValue = value.intValue;
                } else {
                    out->realValue = value.realValue;
                }
                break;
            }

            symbol_t* var = current->lookup( id );
            int currentPos = position;
            position = var->procIndex;
            
            symtable_t* neu = new symtable_t;
            neu->parent = current;
            current->children.push_back( neu );
            current = neu;
            
            block();

            current = current->parent;
            position = currentPos;
            break;
        }
        case token::BEGIN:
            do {
                position++;
                statement();
            } while ( tokens[position] == token::SEMICOLON );
            position++;
            break;
        case token::IF: {
            position++;
            bool val = condition();
            position++;
            if ( val ) {
                statement();
            } else {
                skip_statement();
            }
            if ( tokens[position] == token::ELSE ) {
                position++;
                if ( !val ) {
                    statement();
                } else {
                    skip_statement();
                }
            }
            position++;
            break;
        }
        case token::WHILE: {
            int conpos = position;
            position++;
            bool val = condition();
            position++;
            if ( val ) {
                statement();
                position = conpos;
                statement(); // revalueate condition
            } else {
                skip_statement();
            }
            break;
        }
        default:
            break;
    }
}


void program_t::program(vector<string>& args) {
    current = new symtable_t;
    current->symbols[ASCII_OUT] = { symbol_t::INT, { .intValue = 0} };
    current->symbols[INT_OUT] = { symbol_t::INT, { .intValue = 0} };
    current->symbols[REAL_OUT] = { symbol_t::REAL, { .intValue = 0} };
    current->symbols[ARG_COUNT] = { symbol_t::INT_KONST, { .intValue = (int) args.size()} };
    current->symbols[ARG_IDX] = { symbol_t::INT, { .intValue = 0} };
    current->symbols[ARG_PROC] = { symbol_t::PROC, { .procIndex = 0} };
    current->symbols[ARG_OUT] = { symbol_t::INT, { .intValue = 0} };

    for ( int i = 0; i < args.size(); i++ ) {
        if ( args[i].find('.') == string::npos ) {
            long val = strtol( args[i].c_str(), nullptr, 0 );
			if ( errno == ERANGE && ( val == LONG_MIN || val == LONG_MAX ) ) {
				cerr << "integer argument to big: " << args[i];
			}	
			if ( val > INT32_MAX || val < INT32_MIN ) {
				cerr << "integer argument to big: " << args[i];
			}
            
            arguments.push_back( { .type = value_t::INT, { .intValue = (int) val } } );
        } else {
            double real = strtod( args[i].c_str(), nullptr );
            if ( errno == ERANGE && real == HUGE_VAL ) {
                cerr << "real argument to big: " << args[i];
            }
            arguments.push_back( { .type = value_t::REAL, { .realValue = real } } );
        }
    }
    block();
}

void program_t::skip_procedure() {
    position += 3;
    skip_block();
    position++;
}

void program_t::skip_block() {
    if ( tokens[position] == token::CONST ) {
        for (; tokens[position] != token::SEMICOLON; position++ );
        position++;
    }
    if ( tokens[position] == token::VAR ) {
        for (; tokens[position] != token::SEMICOLON; position++ );
        position++;
    }
    while ( tokens[position] == token::PROCEDURE ) {
        skip_procedure();
    }
    skip_statement();
}

void program_t::skip_expression() {
    skip_term();
    while ( tokens[position] == token::PLUS || tokens[position] == token::MINUS ) {
        position++;
        skip_term();
    }
}

void program_t::skip_term() {
    skip_factor();
    while ( tokens[position] == token::MULT || tokens[position] == token::DIV ) {
        position++;
        skip_factor();
    }
}

void program_t::skip_factor() {
    switch ( tokens[position].type ) {
        case token::ID:
        case token::INTNUM:
        case token::REALNUM:
            position++;
            break;
        case token::KLAUF:
            position++;
            skip_expression();
            position++;
            break;
    }
}

void program_t::skip_statement() {
    switch( tokens[position].type ) {
        case token::ID:
            position += 2;
            skip_expression();
            break;
        case token::CALL:
            position += 2;
            break;
        case token::BEGIN:
            do {
                position++;
                skip_statement();
            } while ( tokens[position] == token::SEMICOLON );
            position++;
            break;
        case token::IF:
            position++;
            skip_condition();
            position++;
            skip_statement();
            if ( tokens[position] == token::ELSE ) {
                position++;
                skip_statement();
            }
            position++;
            break;
        case token::WHILE:
            position++;
            skip_condition();
            position++;
            skip_statement();
            break;
    }
}

void program_t::skip_condition() {
    skip_expression();
    position++;
    skip_expression();
}

void program_t::block() {
    if ( tokens[position] == token::CONST ) {
        position++;
        constdecl();
    }
    if ( tokens[position] == token::VAR ) {
        position++;
        vardecl();
    }
    procdecl();
    statement();
}

void program_t::run(vector<string>& args) {
    program(args);
}


void help() {
	cout << "Usage: runtime <inputfile>" << endl;
	exit( 1 );
}

int main(int argc, char** argv) {
    if ( argc == 1 ) {
		help();
	}

	string inputfile = argv[ 1 ];

	if ( access( inputfile.c_str(), F_OK ) == -1 ) {
		cerr << "Input file doesn't exist\n";
		exit(1);
	}

    ostream error(nullptr);
	error.rdbuf( cerr.rdbuf() );
	ostream output(nullptr);
	ifstream input( inputfile, ifstream::in );

	lexan_t lexan( input, output, error );
    program_t program;

    token_t lookahead;
    do {
        lookahead = lexan.nextsymbol();
        program.tokens.push_back( lookahead );
    } while ( lookahead != token::DONE );

    vector<string> arguments;
    for ( int i = 2; i < argc; i++ ) {
        arguments.push_back( argv[i] );
    }
    program.run(arguments);

    return 0;
}

symbol_t* symtable_t::lookup( string identifier ) {
    if ( symbols.count( identifier ) == 1 ) {
        return &symbols[ identifier ];
    }
    if ( parent == nullptr ) {
        cerr << "could not find symbol in symtable" << endl;
        exit( 1 );
    }
    return parent->lookup( identifier );
}

std::ostream& operator<<( std::ostream& os, const symbol_t& symbol ) {
	os << "symbol(";

	switch ( symbol.type ) {
		case symbol_t::INT:
			os <<  "INT, value = " << symbol.intValue << ")";
			break;
		case symbol_t::REAL:
			os << "REAL, value = " << symbol.realValue << ")";
			break;
		case symbol_t::PROC:
			os << "PROC, value = " << symbol.procIndex << ")";
			break;
        case symbol_t::INT_KONST:
			os << "INT_KONST, value = " << symbol.intValue << ")";
			break;
		default:
			os << ")";
			break;
	}
	return os;
}

std::ostream& operator<<( std::ostream& os, const symtable_t& symbol ) {
    os << "symtable( map = ";
    
    for (auto const& x : symbol.symbols) {
        os << x.first  // string (key)
                  << ':' 
                  << x.second // string's value 
                  << std::endl ;
    }

    os << ", parent = ";

    if ( symbol.parent == nullptr ) {
        os << "null";
    } else {
        os << *symbol.parent;
    }
	return os;
}
