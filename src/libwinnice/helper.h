#pragma once

#define MYL(s) L ## s

#define APPNAME MYL("winnice")
#define APPVERSION MYL("1.0.0")

#include "libwinnice.h"

extern WNUShowInformation gUFShowOutput;
extern WNUShowInformation gUFShowError;

void ShowOutput(const tchar* pMessage);
void ShowOutput(const tstring& s);
void ShowOutput(const tstringstream& s);



void ShowError(const tchar* pMessage);
void ShowError(const tstring& message);
void ShowError(const tstringstream& message);
void ShowErrorWithLastError(int err);


void ShowHelp(bool more=false);
