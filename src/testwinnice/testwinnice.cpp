// testwinnice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../libwinnice/libwinnice.h"

TEST(winnice, Basic)
{
	DWORD id = GetCurrentProcessId();

	EXPECT_NE(LibWinNiceMainW(false, 0, nullptr, NULL, NULL), 0);

}