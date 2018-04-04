
#ifndef LEXAN_H
#include "lexan.h"
#endif

/**
 * Initalize token_t by tokentype_t and setting
 * default values for special values if tokentype is
 * INTNUM, REALNUM or iD
 */
token_t::token_t( tokentype_t type ) : type( type ), idname( "" ) {
    switch ( type ) {
        default:
        case INTNUM:
            num = 0;
            break;
        case REALNUM:
            realnum = 0.0;
            break;
    }
};

/**
 * Copy Constructor for token_t
 */
token_t::token_t( const token_t& symbol ) {
    *this = symbol;
}

/**
 * Check if token_t is equal to an other token_t
 * if one of the special values
 * INTNUM, REALNUM or ID
 * the token value will be checked too.
 */
bool token_t::operator== ( const token_t& b ) {
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

/**
 * Check if two tokens are not equal
 */
bool token_t::operator!= ( const token_t& b ) {
    return ! ( *this == b );
}

/**
 * Check if token.type is equal
 */
bool token_t::operator== ( const tokentype_t& type ) {
    return this->type == type;
}

/**
 * Check if token.type is equal
 */
bool token_t::operator!= ( const tokentype_t& type ) {
    return this->type != type;
}


/**
 * Basic assignment operator
 */
token_t& token_t::operator= ( const token_t& symbol ) {
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