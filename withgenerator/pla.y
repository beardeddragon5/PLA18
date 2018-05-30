/*
	Definition der Grammatik von PLA fuer Bison inkl. einfacher
	Symboltabellenverwaltung.

	Letzte �nderung SS 2017  msr

*/



/*	---- Deklarationen ----			*/
%{
	/* Deklarationen in C */
	#include <stdio.h>
	#include "global.h"
	#include "symbol.h"
	#include "error.h"

	/* Variable fuer den jeweils aktuell gerade "interessanten"
	   Symboltabelleneintrag
	*/
	struct st_entry * symentry;

	/* Fehlermeldungen sollen etwas aussagekraeftiger als "syntax error" sein */
	#define YYERROR_VERBOSE

	/* Bison erwartet, dass eine Routine zur Ausgabe von Fehlermeldungen
	   implementiert wird...
	*/
	void yyerror(char * s)
	{
		printf("Line %d: %s\n", yylineno, s);
	}
%}

	/* Deklarationen fuer Bison */

/* Die folgende Deklaration legt fest, dass der Datentyp von Terminalen und
   Nichtterminalen eine Zahl (num/realnum) oder ein String (idname) sein kann.

   Dies wird fuer die Erkennung von Zahlkonstanten und Variablennamen
   benutzt.
*/
%union {
	int num;
	double realnum;
	char idname[300];
}

/*
	Token

   Die Nummern und die Kurzschreibweisen in Anfuehrungszeichen sind
   optional.
   Die Nummern werden hier verwendet, um die Ausgabe analog zur Aufgabe 1
   zu halten.
   Die Namen in Anfuehrungszeichen dienen der besseren Lesbarkeit der Grammatik,
   sie haben NICHTS mit dem Scanner zu tun!

   Hinweis:
   	Normalerweise werden bei Bison Kleinbuchstaben fuer nichtterminale
		Symbole und die Grossschreibweise fuer terminale Symbole verwendet,
		z.B.:	expr: expr PLUS expr
*/
%token <num> INTNUMBER	2561			/* Int-Konstante */
%token <realnum> REALNUMBER  2562		/* Real-Konstante */
%token <idname> IDENT  	257			/* identifikator */
%token CONST 		258	"const"
%token VAR  		259	"var"
%token PROCEDURE	260	"procedure"
%token CALL 		261	"call"
%token _BEGIN 		262	"begin"
%token END   		263	"end"
%token IF  			264	"if"
%token THEN 		2651"then"
%token ELSE			2652 "else"
%token WHILE 		266	"while"
%token DO 			267	"do"
%token EQ 			268	"="
%token NE			269	"!="
%token LT			270	"<"
%token LE			271	"<="
%token GT			272	">"
%token GE			273	">="
%token ASS			274	":="
%token KOMMA	   	275	","
%token SEMICOLON	276	";"
%token PLUS			277	"+"
%token MINUS		278	"-"
%token MULT			279	"*"
%token DIV			280	"/"
%token KLAUF		281	"("
%token KLZU			282	")"
%token PUNKT		283	"."
%token COLON		284	":"
%token INT			285	"int"
%token REAL			286	"real"
%token BOOL			287	"boolean"
%token WAHR			288	"true"
%token FALSCH		289	"false"
%token PROGEND 		290 "$"
%token FI 		291  "fi"
/*
	Das Nichtterminal TYP hat einen numerischen Wert, naemlich
	INTIDENT, REALIDENT
*/

%type <num> typ



/* Nichtterminales Startsymbol */
%start program

%%
/*	---- Grammatik und Aktionen ----	*/
program: {

			/* als erstes die Symboltabelle initialisieren */
			actsym = create_newsym();
		} block "$";

block: constdecl vardecl prodecl statment


constdecl: "const" constassings ";" | ;
constass: IDENT "=" INTNUMBER {
	symentry= lookup($1);
	if (symentry != NULL) {
		error(IDENTIFIER_ALREADY_DECLARED);
	}
	insert(KONST, $1, $3);
};
constassings: constass | constassings "," constass;


vardecl: "var" varassings ";" | ;
varass: IDENT ":" typ {
	symentry= lookup($1);
	if (symentry != NULL) {
		error(IDENTIFIER_ALREADY_DECLARED);
	}

	insert($3, $1, 0);
};
varassings: varass | varassings "," varass;



prodecl: prodethird | ;
prodeclass: "procedure" IDENT {
		symentry = insert(PROC, $2, 0);
		actsym = symentry->subsym;
	} ";" block {
		actsym = actsym->precsym;
		} ";" {
	};
prodethird: prodethird prodeclass | prodeclass;


statment: IDENT ":=" expression {
					symentry= lookup($1);
					if (symentry == NULL) {
						error(IDENTIFIER_NOT_DECLARED);
					}
					if (symentry->type == KONST) {
						error(CONST_READONLY);
					}
					if (symentry->type == PROC) {
						error(PROCEDURE_NOT_ASSINABLE);
					}
				}
				|"call" IDENT {
					symentry= lookup($2);
					if (symentry == NULL) {
						error(IDENTIFIER_NOT_DECLARED);
					}
					if (symentry->type != PROC) {
						error(IDENTIFIER_IS_NOT_CALLABLE);
					}
				}
				|"begin" recstatment "end"
				|"if" condition "then" statment constatment"fi"
				|"while" condition "do" statment;

recstatment: recstatment ";" statment | statment ;
constatment: "else" statment | ;

condition:	expression relop expression;

relop:	"=" | "!=" | "<" | "<=" | ">" | ">=";

expression:	expression "+" term |
						expression "-" term |
						term;

term:		term "*" factor |
				term "/" factor |
				factor;

factor:	IDENT {
			symentry= lookup($1);
			if (symentry == NULL) {
				error(IDENTIFIER_NOT_DECLARED);
			}
			if (symentry->type == PROC) {
				error(PROCDURE_IN_EXPRESSION_NOT_ALLOWED);
			}
		} | INTNUMBER | REALNUMBER | "(" expression ")";

typ:	"int" { $$ = INTIDENT;} | "real" {$$ = REALIDENT; };

%%
/*	---- abschliessender C-Code ----	*/
