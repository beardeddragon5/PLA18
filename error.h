
#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef ERROR_H
#define ERROR_H
#endif

namespace error {
    enum error_t : int {
        EXPECTED_EQUAL,
        EXPECTED_ASS,
        EXPECTED_RATIONAL_OPERATOR,
        EXPECTED_ID,
        EXPECTED_END,
        EXPECTED_THEN,
        EXPECTED_IF,
        EXPECTED_DO,
        EXPECTED_SEMICOLON,
        EXPECTED_VAR,
        EXPECTED_COLON,
        EXPECTED_TYPE,
        EXPECTED_KOMMA,
        EXPECTED_CONST,
        EXPECTED_EQ,
        EXPECTED_INTNUM,
        EXPECTED_PROGEND,
        EXPECTED_DONE,

        INVALID_CHARACTER,

        SYMBOL_TABLE_FULL,
        LEXEM_FIELD_FULL,

        IDENTIFIER_NOT_DECLARED,
        IDENTIFIER_ALREADY_DECLARED,

        NO_FACTOR,
        NO_STATEMENT,

        IDENTFIER_TO_BIG,
        NUMBER_TO_BIG,

        PROCDURE_IN_EXPRESSION_NOT_ALLOWED,
        CONST_READONLY,
        PROCEDURE_NOT_ASSINABLE,

        IDENTIFIER_IS_NOT_CALLABLE
    };
}
