#ifndef LIBWNMAIN_GLOBAL_H
#define LIBWNMAIN_GLOBAL_H

#if defined(LIBWINNICE_LIBRARY)
#  define LIBWNMAINSHARED_EXPORT __declspec( dllexport )
#else
#  define LIBWNMAINSHARED_EXPORT __declspec( dllimport )
#endif

#endif // LIBWNMAIN_GLOBAL_H
