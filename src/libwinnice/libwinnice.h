#ifndef LIBWINNICE_H
#define LIBWINNICE_H

#include "libwinnice_global.h"

typedef void(*WNUShowInformation)(const wchar_t* pMessage);

LIBWINNICESHARED_EXPORT int LibWinNiceMain(WNUShowInformation wnuShowInformation);

#endif // LIBWINNICE_H