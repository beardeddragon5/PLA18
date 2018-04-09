/**************** global.h  - SS 2018    ************/

#ifndef GLOBAL_H
#define GLOBAL_H
#endif

#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string.h>

using namespace std;

#define TRUE			1
#define FALSE			0
#define BSIZE			512			/* Puffergrösse */
#define NONE		 	-1		/* Vorbesetzung für num */
#define EOS			 	'\0'		/* End of String */

#define NESTMAX		10			/* Max. Schachtelungstiefe von Blöckeen */
#define STRMAX		999			/* Länge des Stringfeldes */
#define SYMMAX		20			/* Größe der Teil-Symboltabelle */

