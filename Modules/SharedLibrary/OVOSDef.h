#ifndef OVOSDEF_H
#define OVOSDEF_H

#include <cstring>
#include <climits>
#ifdef WIN32
	#include <windows.h>
	#include <string.h>	//memset
	#define bzero(addr, num) memset(addr, 0, num)
	#define PATH_MAX _MAX_PATH
	#pragma warning(disable:4996)
	#define strcasecmp stricmp
#endif

#endif
