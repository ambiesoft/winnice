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
void ShowErrorWithLastError(int err, DWORD pid);


void ShowHelp(bool more=false);
