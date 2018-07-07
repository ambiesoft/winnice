
#include <Windows.h>
#include <tlhelp32.h>

#include <set>
#include <iostream>
#include <sstream>

#include "../../../lsMisc/SetPrority.h"
#include "../../../lsMisc/CommandLineString.h"
#include "../../../lsMisc/CreateProcessCommon.h"
#include "../../../lsMisc/stlScopedClear.h"
#include "../../../lsMisc/stdwin32/stdwin32.h"

#include "mytype.h"
#include "libwinnice.h"

using namespace Ambiesoft;
using namespace stdwin32;
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


set<DWORD> GetProcessIDFromExecutable(LPCTSTR pExe)
{
	set<DWORD> retVec;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			// TODO: Fullpath and Filename compare properly
			if (lstrcmpi(entry.szExeFile, pExe) == 0)
			{
				retVec.insert(entry.th32ProcessID);
			}
		}
	}
	CloseHandle(snapshot);

	return retVec;
}

void ShowError(const tchar* pMessage)
{
	wcerr << pMessage << endl;
}
void ShowError(const tstring message)
{
	ShowError(message.c_str());
}
template<typename chartype>
basic_string<chartype> stdGetEnv(const basic_string<chartype>& s)
{
	chartype buff[1024]; buff[0] = 0;
	size_t req;
	errno_t err = _wgetenv_s(&req, buff, s.c_str());
	return buff;
}
template<typename stringtype>
bool IsExecutableExtension(stringtype s)
{
	if (s.empty())
		return false;

	stringtype env = stdGetEnv(stringtype(MYL("PATHEXT")));
	vector<stringtype> vExts = stdwin32::split_string(env, MYL(";"));
	if (s[0] != MYL('.'))
		s = MYL('.') + s;

	for (auto&& ext : vExts)
	{
		if (lstrcmpi(ext.c_str(), s.c_str())==0)
			return true;
	}
	return false;
}
tstring ModifyCommand(const tstring& command)
{
	CCommandLineStringBase<tchar> cms(command);
	if (cms.getCount() == 0)
		return command;

	tstring exe = cms.getArg(0);
	tstring ext = stdGetFileExtension(exe);
	if (ext.empty())
		return MYL("cmd /c ") + command;
	if(!IsExecutableExtension(ext))
		return MYL("cmd /c ") + command;

	// extension is executable
	return command;
}

enum TargetType
{
	TARGET_NONE,
	TARGET_FIND_FROM_EXECUTABLE,
	TARGET_NEW_PROCESS,
};
int LibWinNiceMain()
{
	wstring aaa = stdToWstring("aaa");
	string sss = stdToString(L"bbb");
	CCommandLineStringBase<tchar> cms;

	CPUPRIORITY cpuPriority = CPUPRIORITY::CPU_NONE;
	IOPRIORITY ioPriority = IOPRIORITY::IO_NONE;
	MEMORYPRIORITY memPriority = MEMORYPRIORITY::MEMORY_NONE;

	bool showCommand = false;
	bool exitifsetpriorityfailed = false;
	bool detachNewProcess = false;

	TargetType targetType = TARGET_NONE;
	set<DWORD> targetIDs;

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
		else if (option == MYL("--detach-newprocess"))
			detachNewProcess = true;


		else if (option == MYL("--executable"))
		{
			if ((i + 1) == count)
			{
				ShowError(MYL("No argument for --executable"));
				return 1;
			}
			++i;
			tstring exe = cms.getArg(i);
			if (exe.empty())
			{
				ShowError(MYL("Executable is empty"));
				return 1;
			}
			if (targetType != TARGET_NONE && targetType != TARGET_FIND_FROM_EXECUTABLE)
			{
				ShowError(MYL("Both --executable and --new-process could not be specified."));
				return 1;
			}
			targetType = TARGET_FIND_FROM_EXECUTABLE;
			set<DWORD> vs  = GetProcessIDFromExecutable(exe.c_str());
			targetIDs.insert(vs.begin(), vs.end());
		}
		else if (option == MYL("--new-process"))
		{
			if (targetType != TARGET_NONE)
			{
				ShowError(MYL("Both --executable and --new-process could not be specified."));
				return 1;
			}
			targetType = TARGET_NEW_PROCESS;
			break;
		}
		else
		{
			ShowError(MYL("Unknown option:") + option);
			return 1;
		}
	}

	if(targetType==TARGET_FIND_FROM_EXECUTABLE)
	{ 
		if (targetIDs.empty())
		{
			ShowError(MYL("Target process not found"));
			return 1;
		}
		if (cpuPriority == CPU_NONE && ioPriority == IO_NONE && memPriority == MEMORY_NONE)
		{
			ShowError(MYL("No priorities specified"));
			return 1;
		}
		string errorstd;
		for (auto&& dwProcessID : targetIDs)
		{
			if (!Ambiesoft::SetProirity(
				dwProcessID,
				cpuPriority,
				ioPriority,
				memPriority,
				errorstd))
			{
				cout << "Failed to set priority:" << endl;
				cout << errorstd << endl;
				if (exitifsetpriorityfailed)
				{
					return 1;
				}
			}
		}
	}
	else if (targetType == TARGET_NEW_PROCESS)
	{
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
			ModifyCommand(newcommnad).c_str(),
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
				wstringstream wss;
				wss << MYL("Failed to set priority:") << endl;
				wss << stdToWstring(errorstd) << endl;
				ShowError(wss.str());
				if (exitifsetpriorityfailed)
				{
					TerminateProcess(hProcess, -1);
					return 1;
				}
			}
		}
		if (!ResumeThread(hThread))
		{
			ShowError(MYL("Resuming failed"));
			TerminateProcess(hProcess, -1);
			return 1;
		}

		if (detachNewProcess)
		{
			return 0;
		}
		gWaiting = true;
		WaitForSingleObject(hProcess, INFINITE);
		gWaiting = false;

		DWORD dwExitCode = -1;
		if (!GetExitCodeProcess(hProcess, &dwExitCode))
		{
			DWORD dwLastError = GetLastError();
			wstringstream wss;
			wss << MYL("Failed to get exit code (LastError=") << dwLastError << MYL(")") << endl;
			ShowError(wss.str());
			return dwLastError;
		}
		return dwExitCode;
	}

	return 0;
}
