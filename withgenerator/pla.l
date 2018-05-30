/*
	Scanner fuer PLA
	J. Uhrmann, 12.11.2005
	M.Messerer , 2017
*/

/*	---- Definitionen ---- 		*/

%{
	/* Definitionen in C */
	#include "pla.tab.h"	/* Token-Konstanten des Parsers verwenden */
	#include <math.h>	/* fuer atoi */
	#include <stdio.h>
	#include <string.h>	/* fuer strcpy */
	#include "error.h"
	#include <limits.h>
%}


/* Zeilennummern sollen ermittelt und in yylineno gespeichert werden */
%option yylineno

/* Definitionen fuer flex */
DIGIT		[0-9]
LETTER	[A-Za-z]
IDENT		LETTER{LETTER|DIGIT}*

%s PROGRAM
/*	---- Scanner-Regeln ----	*/
%%

"const"	return CONST;
"var"	return VAR;
"procedure"	return PROCEDURE;
"call"	return CALL;
"begin"	return _BEGIN;
"end"	return END;
"if"	return IF;
"then"	return THEN;
"else"	return ELSE;
"while"	return WHILE;
"do"	return DO;
"="	return EQ;
"!="	return NE;
"<"	return LT;
"<="	return LE;
">"	return GT;
">="	return GE;
":="	return ASS;
","	return KOMMA;
";"	return SEMICOLON;
"+"	return PLUS;
"-"	return MINUS;
"*"	return MULT;
"/"	return DIV;
"("	return KLAUF;
")"	return KLZU;
"."	return PUNKT;
":"	return COLON;
"int"	return INT;
"real"	return REAL;
"$"	return PROGEND;
"fi"	return FI;

{DIGIT}+ {
	long val = strtol( yytext, NULL, 0 );
	if ( errno == ERANGE && ( val == LONG_MIN || val == LONG_MAX ) ) {
		error( NUMBER_TO_BIG ); // To big number
	}
	if ( val > INT32_MAX || val < INT32_MIN ) {
		error( NUMBER_TO_BIG ); // To big number
	}
	yylval.num = (int) val;
}

{DIGIT}+"."{DIGIT}* {
	double real = strtod( yytext, NULL );
	if ( errno == ERANGE && real == HUGE_VAL ) {
		error( NUMBER_TO_BIG ); // To big number
	}
	yylval.realnum = real;
}

IDENT {
	strncpy(yylval.idname, yytext, sizeof(yylval.idname));
}

[ \t\r]		/* Whitespaces ignorieren */

\n yylineno++;

.		{	/* alle anderen Zeichen sind Fehler */
			fprintf(stderr, "Zeile %d: ungueltiges Zeichen erkannt: %s\n", yylineno, yytext);
			exit(1);
		}
%%
/*	Ende  ---- */
