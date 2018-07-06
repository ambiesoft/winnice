#ifndef LIBWINNICE_GLOBAL_H
#define LIBWINNICE_GLOBAL_H

#if defined(LIBWINNICE_LIBRARY)
#  define LIBWINNICESHARED_EXPORT __declspec( dllexport )
#else
#  define LIBWINNICESHARED_EXPORT __declspec( dllimport )
#endif

#endif // LIBWINNICE_GLOBAL_H
