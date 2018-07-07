#include <iostream>

#include "../../../lsMisc/SetPrority.h"
#include "../../../lsMisc/CommandLineString.h"
#include "../../../lsMisc/CreateProcessCommon.h"
#include "../../../lsMisc/stlScopedClear.h"


#include "mytype.h"
#include "libwinnice.h"

using namespace Ambiesoft;
using namespace std;


Libwinnice::Libwinnice()
{
}

bool gWaiting;
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		if (gWaiting)
		{
			printf("winnice ignores Ctrl-C event.\n");
			return TRUE;
		}
	}

	return FALSE;
}

int LibWinNiceMain()
{
	CCommandLineStringBase<tchar> cms;

	CPUPRIORITY cpuPriority = CPUPRIORITY::CPU_NONE;
	IOPRIORITY ioPriority = IOPRIORITY::IO_NONE;
	MEMORYPRIORITY memPriority = MEMORYPRIORITY::MEMORY_NONE;

	bool showCommand = false;
	bool exitifsetpriorityfailed = false;
	size_t i = 1;
	size_t count = cms.getCount();
	for (; i < count; ++i)
	{
		tstring option = cms.getArg(i);
		if (false)
			;
		else if (option == MYL("--cpu-high"))
			cpuPriority = CPU_HIGH;
		else if (option == MYL("--cpu-abovenormal"))
			cpuPriority = CPU_ABOVENORMAL;
		else if (option == MYL("--cpu-normal"))
			cpuPriority = CPU_NORMAL;
		else if (option == MYL("--cpu-belownormal"))
			cpuPriority = CPU_BELOWNORMAL;
		else if (option == MYL("--cpu-idle"))
			cpuPriority = CPU_IDLE;

		else if (option == MYL("--io-high"))
			ioPriority = IO_HIGH;
		else if (option == MYL("--io-abovenormal"))
			ioPriority = IO_ABOVENORMAL;
		else if (option == MYL("--io-normal"))
			ioPriority = IO_NORMAL;
		else if (option == MYL("--io-belownormal"))
			ioPriority = IO_BELOWNORMAL;
		else if (option == MYL("--io-idle"))
			ioPriority = IO_IDLE;

		else if (option == MYL("--mem-high"))
			memPriority = MEMORY_HIGH;
		else if (option == MYL("--mem-abovenormal"))
			memPriority = MEMORY_ABOVENORMAL;
		else if (option == MYL("--mem-normal"))
			memPriority = MEMORY_NORMAL;
		else if (option == MYL("--mem-belownormal"))
			memPriority = MEMORY_BELOWNORMAL;
		else if (option == MYL("--mem-idle"))
			memPriority = MEMORY_IDLE;

		else if (option == MYL("--all-high")) {
			cpuPriority = CPU_HIGH;
			ioPriority = IO_HIGH;
			memPriority = MEMORY_HIGH;
		}
		else if (option == MYL("--all-abovenormal")) {
			cpuPriority = CPU_ABOVENORMAL;
			ioPriority = IO_ABOVENORMAL;
			memPriority = MEMORY_ABOVENORMAL;
		}
		else if (option == MYL("--all-normal")) {
			cpuPriority = CPU_NORMAL;
			ioPriority = IO_NORMAL;
			memPriority = MEMORY_NORMAL;
		}
		else if (option == MYL("--all-belownormal")) {
			cpuPriority = CPU_BELOWNORMAL;
			ioPriority = IO_BELOWNORMAL;
			memPriority = MEMORY_BELOWNORMAL;
		}
		else if (option == MYL("--all-idle")) {
			cpuPriority = CPU_IDLE;
			ioPriority = IO_IDLE;
			memPriority = MEMORY_IDLE;
		}


		else if (option == MYL("--show-command"))
			showCommand = true;
		else if (option == MYL("--exit-if-setpriority-failed"))
			exitifsetpriorityfailed = true;
		else
			break;
	}

	tstring newcommnad = cms.subString(i);
	if (newcommnad.empty())
	{
		cout << "No command to run" << endl;
		return 1;
	}

	if (showCommand)
	{
		tcout << newcommnad << endl;
	}

	DWORD dwLastError = 0;
	HANDLE hProcess = NULL;
	DWORD dwProcessID = 0;

	HANDLE hThread = NULL;

	if (!CreateProcessCommon(
		NULL,
		(MYL("cmd /c ") + newcommnad).c_str(),
		FALSE,
		&dwLastError,
		WaitProcess_None,
		INFINITE,
		&hProcess,
		&dwProcessID,
		&hThread,
		NULL,
		TRUE))
	{
		cout << "Failed to CreateProcess (LastError=" << dwLastError << ")" << endl;
		return dwLastError;
	}
	STLSOFT_SCOPEDFREE_HANDLE(hProcess);
	STLSOFT_SCOPEDFREE_HANDLE(hThread);

	if (!SetConsoleCtrlHandler(CtrlHandler, TRUE))
	{
		DWORD dwLastError = GetLastError();
		cout << "SetConsoleCtrlHandler Failed (LastError=" << dwLastError << ")" << endl;
		return dwLastError;
	}

	if (!(cpuPriority == CPU_NONE && ioPriority == IO_NONE && memPriority == MEMORY_NONE))
	{
		string errorstd;
		if (!Ambiesoft::SetProirity(
			dwProcessID,
			cpuPriority,
			ioPriority,
			memPriority,
			errorstd))
		{
			cout << "Failed to set priority:" << endl;
			cout << "  " << errorstd << endl;
			if (exitifsetpriorityfailed)
			{
				TerminateProcess(hProcess, -1);
				return 1;
			}
		}
	}
	if (!ResumeThread(hThread))
	{
		cout << "Resuming failed" << endl;
		TerminateProcess(hProcess, -1);
		return 1;
	}
	gWaiting = true;
	WaitForSingleObject(hProcess, INFINITE);
	gWaiting = false;

	DWORD dwExitCode = -1;
	if (!GetExitCodeProcess(hProcess, &dwExitCode))
	{
		DWORD dwLastError = GetLastError();
		tcout << MYL("Failed to get exit code (LastError=") << dwLastError << MYL(")") << endl;
		return dwLastError;
	}
	return dwExitCode;
}
