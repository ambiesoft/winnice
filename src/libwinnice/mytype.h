#ifndef MYTYPE_H
#define MYTYPE_H

#include <string>
#include <iostream>

#ifdef _WIN32
typedef wchar_t tchar;
typedef std::wstring tstring;
#define tcout std::wcout
#define MYL(s) L ## s
#else
typedef char tchar;
typedef std::string tstring;
#define tcout std::cout
#define MYL(s) s
#endif



#endif // MYTYPE_H
