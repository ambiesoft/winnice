// MIT License
// 
// Copyright (c) 2018 Ambiesoft
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// libwnmain.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
 
 

#include <Windows.h>
#include <tlhelp32.h>

#include <set>
#include <iostream>
#include <sstream>
#include <functional>

#include "lsMisc/stdosd/SetPrority.h"
#include "lsMisc/CommandLineString.h"
#include "lsMisc/CreateProcessCommon.h"
#include "lsMisc/GetLastErrorString.h"
#include "lsMisc/tstring.h"
#include "lsMisc/stdosd/stdosd.h"


#include "libwinnice.h"
#include "helper.h"



using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
using namespace std;

// TODO: implement
std::set<std::wstring> gOptions =
{
	L"--helpmore",
	L"-h",
};

wstring op(wstring option)
{
	assert(gOptions.find(option) != gOptions.end());
	return option;
}

// TODO: implement
wstring GetHammingSuggest(wstring option)
{
	return wstring();
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



template<typename chartype>
basic_string<chartype> stdGetEnv(const basic_string<chartype>& s)
{
	chartype buff[1024]; buff[0] = 0;
	size_t req;
    _wgetenv_s(&req, buff, s.c_str());

	return buff;
}
template<typename stringtype>
bool IsExecutableExtension(stringtype s)
{
	if (s.empty())
		return false;

	stringtype env = stdGetEnv(stringtype(MYL("PATHEXT")));
	vector<stringtype> vExts = stdSplitString(env, MYL(";"));
	if (s[0] != MYL('.'))
		s = MYL('.') + s;

	for (auto&& ext : vExts)
	{
		if (lstrcmpi(ext.c_str(), s.c_str()) == 0)
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
	if (!IsExecutableExtension(ext))
		return MYL("cmd /c ") + command;

	// extension is executable
	return command;
}

inline void ShowOutput(const wstring& s)
{
	ShowOutputW(s);
}
inline void ShowOutput(const wstringstream& s)
{
	ShowOutputW(s);
}
inline void ShowError(const wstring& s)
{
	ShowErrorW(s);
}
inline void ShowError(const wstringstream& s)
{
	ShowErrorW(s);
}
wstring GetNextArgOrShowError(size_t& i,
	size_t count, 
	const CCommandLineString& cms,
	const tstring option)
{
	if ((i + 1) == count)
	{
		ShowError(MYL("No argument for ") + option);
		return tstring();
	}
	++i;
	tstring next = cms.getArg(i);
	if (next.empty())
	{
		ShowError(option + MYL(" is empty"));
		return tstring();
	}
	return next;
}

enum TargetType
{
	TARGET_NONE,
	TARGET_FIND_FROM_EXECUTABLE,
	TARGET_NEW_PROCESS,
};


enum ERROR_LEVEL {
	ERROR_LEVEL_NONE,
	ERROR_LEVEL_NORMAL,
	ERROR_LEVEL_DEBUG,
};
int LibWinNiceMainW(
	bool bGui,
	int argc,
	const wchar_t* const* argv,
	WNUShowInformationW wnuShowOutputW,
	WNUShowInformationW wnuShowErrorW)
{
	gUFShowOutputW = wnuShowOutputW;
	gUFShowErrorW = wnuShowErrorW;

	CCommandLineStringBase<tchar> cms(argc,argv);

	Process::CPUPRIORITY cpuPriority = Process::CPUPRIORITY::CPU_NONE;
	Process::IOPRIORITY ioPriority = Process::IOPRIORITY::IO_NONE;
	Process::MEMORYPRIORITY memPriority = Process::MEMORYPRIORITY::MEMORY_NONE;

	bool showCommand = false;
	bool exitifsetpriorityfailed = false;
	bool detachNewProcess = bGui ? true : false;
	bool newProcess = false;

	size_t nSubcommandStartIndex = -1;
	const size_t count = cms.getCount();
	set<DWORD> pidsToProcess;

	ERROR_LEVEL errorLevel = ERROR_LEVEL_NORMAL;

	{
		// ensure these 2 sets are in this scope
		set<wstring> targetExes;
		set<DWORD> targetPIDs;

		if (count <= 1)
		{
			ShowError(L"No arguments");
			return 1;
		}
		for (size_t i = 1; i < count; ++i)
		{
			tstring option = cms.getArg(i);
			if (false)
				;
			else if (option == MYL("--cpu-high"))
				cpuPriority = Process::CPU_HIGH;
			else if (option == MYL("--cpu-abovenormal"))
				cpuPriority = Process::CPU_ABOVENORMAL;
			else if (option == MYL("--cpu-normal"))
				cpuPriority = Process::CPU_NORMAL;
			else if (option == MYL("--cpu-belownormal"))
				cpuPriority = Process::CPU_BELOWNORMAL;
			else if (option == MYL("--cpu-idle"))
				cpuPriority = Process::CPU_IDLE;
			else if (option == MYL("--cpu-default"))
				cpuPriority = Process::CPU_NONE;

			else if (option == MYL("--io-high"))
				ioPriority = Process::IO_NORMAL; // IO_HIGH;
			else if (option == MYL("--io-abovenormal"))
				ioPriority = Process::IO_NORMAL; // IO_ABOVENORMAL;
			else if (option == MYL("--io-normal"))
				ioPriority = Process::IO_NORMAL;
			else if (option == MYL("--io-belownormal"))
				ioPriority = Process::IO_BELOWNORMAL;
			else if (option == MYL("--io-idle"))
				ioPriority = Process::IO_IDLE;
			else if (option == MYL("--io-default"))
				ioPriority = Process::IO_NONE;

			else if (option == MYL("--mem-high"))
				memPriority = Process::MEMORY_HIGH;
			else if (option == MYL("--mem-abovenormal"))
				memPriority = Process::MEMORY_HIGH;
			else if (option == MYL("--mem-normal"))
				memPriority = Process::MEMORY_HIGH;
			else if (option == MYL("--mem-belownormal"))
				memPriority = Process::MEMORY_BELOWNORMAL;
			else if (option == MYL("--mem-idle"))
				memPriority = Process::MEMORY_IDLE;
			else if (option == MYL("--mem-default"))
				memPriority = Process::MEMORY_NONE;

			else if (option == MYL("--all-high")) {
				cpuPriority = Process::CPU_HIGH;
				ioPriority = Process::IO_NORMAL;
				memPriority = Process::MEMORY_HIGH;
			}
			else if (option == MYL("--all-abovenormal")) {
				cpuPriority = Process::CPU_ABOVENORMAL;
				ioPriority = Process::IO_NORMAL;
				memPriority = Process::MEMORY_HIGH;
			}
			else if (option == MYL("--all-normal")) {
				cpuPriority = Process::CPU_NORMAL;
				ioPriority = Process::IO_NORMAL;
				memPriority = Process::MEMORY_HIGH;
			}
			else if (option == MYL("--all-belownormal")) {
				cpuPriority = Process::CPU_BELOWNORMAL;
				ioPriority = Process::IO_BELOWNORMAL;
				memPriority = Process::MEMORY_BELOWNORMAL;
			}
			else if (option == MYL("--all-idle")) {
				cpuPriority = Process::CPU_IDLE;
				ioPriority = Process::IO_IDLE;
				memPriority = Process::MEMORY_IDLE;
			}

			else if (option == MYL("--show-command"))
				showCommand = true;
			else if (option == MYL("--exit-if-setpriority-failed"))
				exitifsetpriorityfailed = true;
			else if (option == MYL("--detach-newprocess"))
				detachNewProcess = true;
			else if (option == MYL("--wait-newprocess"))
				detachNewProcess = false;

			else if (option == MYL("--executable"))
			{
				tstring exe = GetNextArgOrShowError(i, count, cms, option);
				if (exe.empty())
				{
					ShowError(L"No executable specified");
					return 1;
				}

				targetExes.insert(exe);
			}
			else if (option == MYL("--pid"))
			{
				tstring pids = GetNextArgOrShowError(i, count, cms, option);
				if (pids.empty())
					return 1;

				vector<tstring> vPids = stdSplitString(pids, MYL(","));
				if (vPids.empty())
				{
					ShowError(MYL("Pid parsed empty"));
					return 1;
				}

				// not yet set
				assert(targetPIDs.empty());

				// Check pid is a number
				for (auto&& pid : vPids)
				{
					if (!stdIsTdigit(pid))
					{
						ShowError(stdFormat(MYL("pid %s is not a number"), pid.c_str()));
						return 1;
					}

					// TODO: To tochar in osd
					targetPIDs.insert(_wtol(pid.c_str()));
				}
			}
			else if (option == MYL("--new-process"))
			{
				newProcess = true;
			}
			else if (option == MYL("-v"))
			{
				ShowVersionW();
				return 0;
			}
			else if (option == MYL("-h") || 
				option == MYL("/h") ||
				option == MYL("--help"))
			{
				ShowHelpW();
				return 0;
			}
			else if (option == op(L"--helpmore"))
			{
				ShowHelpW(true);
				return 0;
			}
			else if (option == MYL("--error-level"))
			{
				tstring errorLevelString = GetNextArgOrShowError(i, count, cms, option);
				if (errorLevelString.empty())
				{
					ShowError(L"No error level specified");
					return 1;
				}
				if (errorLevelString == L"normal")
				{
					// default
				}
				else if (errorLevelString == L"debug")
				{
					errorLevel = ERROR_LEVEL_DEBUG;
				}
				else
				{
					wstringstream wss;
					wss << L"Unknown error level '" << errorLevelString << "'" << endl;
					ShowError(wss.str());
					return 1;
				}
			}
			else if (option == MYL("--show-nooutput"))
			{
				gShowNoOutput = true;
			}
			else if (option == MYL("--show-noerror"))
			{
				gShowNoError = true;
			}

			else if (option.size() > 1 && option[0] == MYL('-'))
			{
				wstringstream message;
				message << (MYL("Unknown option:") + option) << endl;
				message << GetHammingSuggest(option) << endl;
				ShowError(message.str());
				return 1;
			}
			else
			{
				if (option == MYL("-"))
				{
					// treat next argument as main arg

					if ((i + 1) == count)
					{
						ShowError(MYL("Insufficient argument after '-'"));
						return 1;
					}
					++i;
					option = cms.getArg(i);

					// and fall through
				}
				// --new-process is omittable
				//if (targetType != TARGET_NONE && targetType != TARGET_NEW_PROCESS)
				//{
				//	ShowError(MYL("Both --executable and --new-process could not be specified."));
				//	return 1;
				//}
				//targetType = TARGET_NEW_PROCESS;
				nSubcommandStartIndex = i;
				break;
			}
		}

		// Set pid for processing
		for (auto&& exe : targetExes)
		{
			set<DWORD> vs = GetProcessIDFromExecutable(exe.c_str());
			if (vs.empty() && errorLevel >= ERROR_LEVEL_DEBUG)
			{
				tstringstream tss;
				tss << MYL("\"") << exe << MYL("\"") << MYL(" ") << MYL("not found") << endl;
				ShowError(tss);
			}
			else
			{
				pidsToProcess.insert(vs.begin(), vs.end());
			}
		}
		
		pidsToProcess.insert(targetPIDs.begin(), targetPIDs.end());
	}
		
	bool isProcessed = false;
	if (!pidsToProcess.empty())
	{
		isProcessed = true;
		//if (())
		//{
		//	ShowError(MYL("Target process not found"));
		//	return 1;
		//}
		if (cpuPriority == Process::CPU_NONE && 
			ioPriority == Process::IO_NONE && 
			memPriority == Process::MEMORY_NONE)
		{
			ShowError(MYL("No priorities specified"));
			return 1;
		}

		tstringstream errorMessage;
		int err;
		for (auto&& dwProcessID : pidsToProcess)
		{
            err = DoSetPriority(dwProcessID,
                cpuPriority,ioPriority,memPriority,
                exitifsetpriorityfailed,
                errorMessage);
            if(err != 0 && exitifsetpriorityfailed)
            {
                ShowError(errorMessage);
                return err;
            }
        }
		if (err != 0)
		{
			ShowError(errorMessage);
			return err;
		}
	}

	if(newProcess)
	{
		isProcessed = true;
		tstring newcommnad = cms.subString(nSubcommandStartIndex);
		if (newcommnad.empty())
		{
			tstringstream tss;
			tss << MYL("No command to run") << endl;
			ShowError(tss);
			return 1;
		}

		if (showCommand)
		{
			tstringstream tss;
			tss << newcommnad << endl;
			ShowOutput(tss);
		}

		DWORD dwLastError = 0;
		HANDLE hProcess = NULL;
		DWORD dwProcessID = 0;

		HANDLE hThread = NULL;

		if (!CreateProcessCommon(
			NULL,
			bGui ? newcommnad.c_str() : ModifyCommand(newcommnad).c_str(),
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
			wstringstream wss;
			wss << L"Failed to CreateProcess (LastError=" << dwLastError << L")" << endl;
			ShowError(wss.str());
			return dwLastError;
		}

		unique_ptr<HANDLE, std::function<void(HANDLE*)>>
			hProcess_free(&hProcess, [](HANDLE* pH) { DVERIFY(::CloseHandle(*pH)); });
		unique_ptr<HANDLE, std::function<void(HANDLE*)>>
			hThread_free(&hThread, [](HANDLE* pH) { DVERIFY(::CloseHandle(*pH)); });


		if (!bGui)
		{
			if (!SetConsoleCtrlHandler(CtrlHandler, TRUE))
			{
				DWORD dwLastError = GetLastError();
				wstringstream wss;
				wss << L"SetConsoleCtrlHandler Failed (LastError=" << dwLastError << L")" << endl;
				ShowError(wss.str());
				return dwLastError;
			}
		}

		if (!(cpuPriority == Process::CPU_NONE && 
			ioPriority == Process::IO_NONE && 
			memPriority == Process::MEMORY_NONE))
		{
            tstringstream errorMessage;
            int err = DoSetPriority(dwProcessID,
                          cpuPriority,ioPriority,memPriority,
                          exitifsetpriorityfailed,
                          errorMessage);
			if (err != 0)
			{
                ShowError(errorMessage);
				if (exitifsetpriorityfailed)
				{
					TerminateProcess(hProcess, -1);
                    return err;
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

	if (!isProcessed)
	{
		ShowOutput(MYL("No operation to process"));
	}
	return 0;
}
//int LibWinNiceMainA(
//	bool bGui,
//	WNUShowInformationA wnuShowOutputA,
//	WNUShowInformationA wnuShowErrorA)
//{
//	return 0;
//}
