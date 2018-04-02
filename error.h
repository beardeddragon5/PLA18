
#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef LEXAN_H
#include "lexan.h"
#endif

#ifndef ERROR_H
#define ERROR_H
#endif



void error( lexan_t&, int );								/* Fehlerausgabe */
void errortext( lexan_t&, string );				/* Fehlerausgabe */
void warningtext( lexan_t&, string );			/* Warnung ausgeben */