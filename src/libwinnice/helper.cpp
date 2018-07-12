#include "StdAfx.h"
#include <sstream>
#include "libwinnice.h"

#include "helper.h"

using namespace std;

WNUShowInformation gUFShowInformation;

void ShowHelp(bool more)
{
	if (!gUFShowInformation)
		return;
	
	wstringstream wss;
	wss << APPNAME << endl;
	wss << L"  Set process priority" << endl;
	wss << endl;
	wss << L"options" << endl;
	wss << L"  [--cpu-high | --cpu-abovenormal | --cpu-normal | --cpu-belownormal | --cpu-idle | --cpu-default]" << std::endl;
	wss << L"  [--io-high | --io-abovenormal | --io-normal | --io-belownormal | --io-idle | --io-default]" << endl;
	wss << L"  [--mem-high | --mem-abovenormal | --mem-normal | --mem-belownormal | --mem-idle | --mem-default]" << endl;
	wss << L"  [--all-abovenormal | --all-normal | --all-belownormal | --all-idle]" << endl;
	wss << endl;
	wss << L"  [--show-command]" << endl;
	wss << L"  [--exit-if-setpriority-failed]" << endl;
	wss << L"  [--detach-newprocess]" << endl;
	wss << L"  [--executable Executable [--executable Executable]]..." << endl;
	wss << endl;
	wss << L"  [--new-process]" << endl;
	wss << endl;
	wss << L"  [- NotOptionArg]" << endl;
	wss << L"  Args..." << endl;
	if (!more)
	{
		wss << endl;
		wss << L"Run with '--helpmore' for more help" << endl;

		gUFShowInformation(wss.str().c_str());
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

	gUFShowInformation(wss.str().c_str());
}