#ifndef _DEFINES_H
#define _DEFINES_H

#include <fstream>
#include <iostream>
#include <pthread.h>
#include <assert.h>		//not use it in server
#include <string>
#include <map>


#define	TITLEMAXLEN 80			//
#define	URLMAXLEN 256			//
#define	DATEMAXLEN 32			//
#define	MEDIAMAXLEN 32			//

#define SYSTEMERR "System Command and Not Download"
#define TEMPLETERR "Templet Error"
#define NOTASKERR "No TitleTask Error"
#define NOTEXTERR "No TextTask Error"			//
#define WRITEERR "Write Error"			//
#define TEMPLETGETTEXTERR	"No text get error Check templet"		//
#define TIMEOUTERR "Time Out Error"			//
#define STATERR "Stat is -1 Error"
#define CONNECTERR "Connect is error"
#define OPENERR "Fopen File Error"			//
#define SENDFLG  1
#define NOSENDFLG 0

typedef int64_t INT64;

#endif

