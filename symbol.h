#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef LEXAN_H
#include "lexan.h"
#endif

#ifndef SYMBOL_H
#define SYMBOL_H
#endif

/* Definition der Art der Symboltabellen-Einträge  */
enum symtype_t : int {
    KONST = 310,        /* Konstanten-Eintrag */
    INTIDENT = 320,     /* Identifikator vom Typ int  */
    REALIDENT = 330,    /* Identifikator vom Typ real */
    PROC = 400          /* Procedure */
};

/* Symboltabelle  */
struct symtable ;

/* Aufbau eines Symboltabellen-Eintrags */
struct st_entry {
	symtype_t token ;					/* Art des Eintrags (KONST/INTIDENT/REALIDENT/PROC)  */
 	char *name;					/* Zeiger auf Namen */
 	int wertaddr;				/* Wert bei Konstanten (KONST);
									   		 relative Speicheradresse (offset) bei Variablen */
 	symtable * subsym;  /* Zeiger auf Teil-Symboltabelle bei Art PROC */
};

/* Aufbau der (Teil- )Symboltabelle */

struct symtable {
	symtable * precsym;				/* Zeiger auf Übergeordnete Symboltabelle; bei oberster NULL */
	int level;								/* Schachtelungstiefe  */
	int anzahl;								/* Anzahl der Symboltabelleneinträge */
	st_entry eintrag[SYMMAX];	/* Feld für Einträge */
};

extern symtable *actsym,		/* Zeiger auf aktuelle Symboltabelle */
		*firstsym;							/* Zeiger auf oberste (globale) Symboltabelle */

symtable * create_newsym() ;		/* Neue ST erzeugen */
st_entry * lookup( string s);		/* Namen in ganzer Symboltabelle suchen */
st_entry * lookup_in_actsym ( string s);	/* Namen in aktueller Symboltabelle suchen */
st_entry * insert( lexan_t& lexan, symtype_t tok, string name, int wertaddr = 0 );   			/* Neuen Eintrag in actsym erzeugen */

// void generate(int);					/* Codeerzeugung */
void printsymtab(symtable *);		/* Ausgabe der Symboltabelle */