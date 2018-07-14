#ifndef LIBWINNICE_H
#define LIBWINNICE_H

#include "libwinnice_global.h"

typedef void(*WNUShowInformation)(const wchar_t* pMessage);

LIBWINNICESHARED_EXPORT int LibWinNiceMain(
	bool bGui,
	WNUShowInformation wnuShowOutput,
	WNUShowInformation wnuShowError);

#endif // LIBWINNICE_H