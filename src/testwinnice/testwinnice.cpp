// testwinnice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../libwinnice/libwinnice.h"

TEST(winnice, Basic)
{
	DWORD id = GetCurrentProcessId();

	EXPECT_NE(LibWinNiceMainW(false, 0, nullptr, NULL, NULL), 0);

	int argc = 0;

	// First argument is dummy
	const wchar_t* commandLine = L"dummy.exe --all-idle --new-process notepad";

	// Create argc and argv from command line
	std::unique_ptr<LPWSTR, void(*)(LPWSTR*)> pArgv(::CommandLineToArgvW(commandLine, &argc),
		[](LPWSTR* ptr) { ::LocalFree(ptr); });
	int nRetNotepad = LibWinNiceMainW(false, argc, pArgv.get(), NULL, NULL);
	EXPECT_EQ(nRetNotepad, 0);
}