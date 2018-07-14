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

#include "../../tstring.h"
#include "../../GetLastErrorString.h"

#include "libwinnice.h"

#include "helper.h"



WNUShowInformation gUFShowOutput;
WNUShowInformation gUFShowError;

#define CHECK_USERFUNC(func) do { if(!func) {return;}} while(false)

using namespace std;
using namespace Ambiesoft;

void ShowOutput(const tchar* pMessage)
{
	CHECK_USERFUNC(gUFShowOutput);
	gUFShowOutput(pMessage);
}
void ShowOutput(const tstring& s)
{
	CHECK_USERFUNC(gUFShowOutput);
	ShowOutput(s.c_str());
}
void ShowOutput(const tstringstream& s)
{
	CHECK_USERFUNC(gUFShowOutput);
	ShowOutput(s.str());
}


void ShowError(const tchar* pMessage)
{
	CHECK_USERFUNC(gUFShowError);
	gUFShowError(pMessage);
}
void ShowError(const tstring& message)
{
	CHECK_USERFUNC(gUFShowError);
	ShowError(message.c_str());
}
void ShowError(const tstringstream& message)
{
	CHECK_USERFUNC(gUFShowError);
	ShowError(message.str());
}
void ShowErrorWithLastError(int err, DWORD pid)
{
	tstring errorMessage = GetLastErrorString(err);
	if (!errorMessage.empty())
		errorMessage = MYL(": \"") + errorMessage + MYL("\"");
	wstringstream wss;
	wss << L"Failed to set priority of process " << pid << " with error" << MYL("(") << err << MYL(")") << errorMessage << endl;
	ShowError(wss.str());
}
void ShowHelp(bool more)
{
	CHECK_USERFUNC(gUFShowOutput);

	wstringstream wss;
	wss << APPNAME << endl;
	wss << L"  Set process priority" << endl;
	wss << endl;
	wss << L"options:" << endl;
	wss << L"  [--cpu-high | --cpu-abovenormal | --cpu-normal | --cpu-belownormal | --cpu-idle | --cpu-default]" << std::endl;
	wss << L"  [--io-high | --io-abovenormal | --io-normal | --io-belownormal | --io-idle | --io-default]" << endl;
	wss << L"  [--mem-high | --mem-abovenormal | --mem-normal | --mem-belownormal | --mem-idle | --mem-default]" << endl;
	wss << L"  [--all-abovenormal | --all-normal | --all-belownormal | --all-idle]" << endl;
	wss << L"  Currenly, '--mem-high', '--mem-abovenormal' and '--mem-normal' is equal to '--mem-high'" << endl;
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

		gUFShowOutput(wss.str().c_str());
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

	gUFShowOutput(wss.str().c_str());
}