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

#include "StdAfx.h"
#include <sstream>

// #include "../../tstring.h"
#include "../../../lsMisc/GetLastErrorString.h"

#include "libwinnice.h"

#include "helper.h"

using namespace Ambiesoft::stdosd;

WNUShowInformationW gUFShowOutputW;
WNUShowInformationW gUFShowErrorW;

#define CHECK_USERFUNC(func) do { if(!func) {return;}} while(false)

using namespace std;
using namespace Ambiesoft;

void ShowOutputW(const wchar_t* pMessage)
{
	CHECK_USERFUNC(gUFShowOutputW);
	gUFShowOutputW(pMessage);
}
void ShowOutputW(const std::wstring& s)
{
	CHECK_USERFUNC(gUFShowOutputW);
	ShowOutputW(s.c_str());
}
void ShowOutputW(const std::wstringstream& s)
{
	CHECK_USERFUNC(gUFShowOutputW);
	ShowOutputW(s.str());
}


void ShowErrorW(const wchar_t* pMessage)
{
	CHECK_USERFUNC(gUFShowErrorW);
	gUFShowErrorW(pMessage);
}
void ShowErrorW(const std::wstring& message)
{
	CHECK_USERFUNC(gUFShowErrorW);
	ShowErrorW(message.c_str());
}
void ShowErrorW(const std::wstringstream& message)
{
	CHECK_USERFUNC(gUFShowErrorW);
	ShowErrorW(message.str());
}
tstring GetErrorWithLastErrorW(int err, DWORD pid)
{
	wstring errorMessage = GetLastErrorString(err);
	if (!errorMessage.empty())
		errorMessage = MYL(": \"") + errorMessage + MYL("\"");
	wstringstream wss;
	wss << L"Failed to set priority of process " << pid << " with error" << MYL("(") << err << MYL(")") << errorMessage << endl;
    return wss.str();
}

void ShowVersionW()
{
	CHECK_USERFUNC(gUFShowOutputW);

	wstringstream wss;
	wss << APPNAME << MYL(" v") << APPVERSION << endl;
	gUFShowOutputW(wss.str().c_str());
}

void ShowHelpW(bool more)
{
	CHECK_USERFUNC(gUFShowOutputW);

	wstringstream wss;
	wss << APPNAME << endl;
	wss << L"  Set process priority" << endl;
	wss << endl;
	wss << L"options:" << endl;
	wss << L"  [--cpu-high | --cpu-abovenormal | --cpu-normal | --cpu-belownormal | --cpu-idle | --cpu-default]" << std::endl;
	wss << L"  [--io-high | --io-abovenormal | --io-normal | --io-belownormal | --io-idle | --io-default]" << endl;
	wss << L"  [--mem-high | --mem-abovenormal | --mem-normal | --mem-belownormal | --mem-idle | --mem-default]" << endl;
	wss << L"  [--all-abovenormal | --all-normal | --all-belownormal | --all-idle]" << endl;
	wss << L"  Currenly, '--io-high' and '--io-abovenormal' is equal to '--io-normal'" << endl;
	wss << L"  Likewise, '--mem-abovenormal' and '--mem-normal' is equal to '--mem-high'" << endl;
	wss << endl;
	wss << L"  [--show-command]" << endl;
	wss << L"  Show the command for creating a new process" << endl;
	wss << endl;
	wss << L"  [--exit-if-setpriority-failed]" << endl;
	wss << L"  [--detach-newprocess | --wait-newprocess]" << endl;
	wss << L"  Whether or not to wait new process to finish. Default value is different between winnice and winnicew, "
		L"winnice's default value is '--wait-newprocess' and winnicew's one is '--detach-newprocess'" << endl;
	wss << endl;
	wss << L"  [--executable Executable [--executable Executable]]..." << endl;
	wss << L"  Specify executables" << endl;
	wss << endl;
	wss << L"  [--pid PID1[,PID2,PID3,...]]" << endl;
	wss << L"  Specify process IDs" << endl;
	wss << endl;
	wss << L"  [--new-process]" << endl;
	wss << L"  Launch a new process, this option should be put at last of command line. "
		"In this case, All strings after this option will be a command line string to launch a process." << endl;
	wss << L"  If this option is not a last one, The position of the first unaware string will be the start of the command line string to launch a process." << endl;
	wss << endl;
	wss << L"  [- NotOptionArg]" << endl;
	wss << L"  If '-' appears after '--new-process', next argument will be treated as executable" << endl;
	if (!more)
	{
		wss << endl;
		wss << L"Run with '--helpmore' for more help" << endl;

		gUFShowOutputW(wss.str().c_str());
		return;
	}

	wss << endl;
	wss << L"Priority options" << endl;
	wss << L"  Latter argument overrides former one. '--all-idle --cpu-default' will affect only IO and MEM." << endl;
	wss << L"  Above 'Normal' priority for IO and MEM are not supported." << endl;
	wss << endl;
	wss << L"--show-command" << endl;
	wss << L"  show commnad to run" << endl;
	wss << endl;
	wss << L"--exit-if-setpriority-failed" << endl;
	wss << L"  Exit if setting priority failed (not run commnad)." << endl;
	wss << endl;
	wss << L"--detach-newprocess" << endl;
	wss << L"  Exit after starting commnad(not wait it)." << endl;
	wss << endl;
	wss << L"--new-process" << endl;
	wss << L"  (Omitable) create a new process and set priority" << endl;
	wss << endl;

	gUFShowOutputW(wss.str().c_str());
}


int DoSetPriority(DWORD dwProcessID,
        Process::CPUPRIORITY cpuPriority,
	Process::IOPRIORITY ioPriority,
	Process::MEMORYPRIORITY memPriority,
        bool exitifsetpriorityfailed,
        tstringstream& errorMessage)
{
    int lastError = 0;
    int err;

    // CPU
    err = Process::SetProirity(
        dwProcessID,
        cpuPriority,
		Process::IO_NONE,
		Process::MEMORY_NONE);
    if (err != 0)
    {
        lastError = err;
        errorMessage << MYL("CPU:") << GetErrorWithLastErrorW(err, dwProcessID) << endl;
        if (exitifsetpriorityfailed)
        {
            return err;
        }
    }



    // IO
    err = Process::SetProirity(
        dwProcessID,
		Process::CPU_NONE,
        ioPriority,
		Process::MEMORY_NONE);
    if (err != 0)
    {
        lastError = err;
        errorMessage << MYL("IO:") << GetErrorWithLastErrorW(err, dwProcessID) << endl;

        if (exitifsetpriorityfailed)
        {
            return err;
        }
    }



    // MEMORY
    err = Process::SetProirity(
        dwProcessID,
		Process::CPU_NONE,
		Process::IO_NONE,
        memPriority);
    if (err != 0)
    {
        lastError = err;
        errorMessage << MYL("Memory:") << GetErrorWithLastErrorW(err, dwProcessID) << endl;

        if (exitifsetpriorityfailed)
        {
            return err;
        }
    }


    return lastError;
}
