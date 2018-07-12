#pragma once

#include "mytype.h"

#define APPNAME MYL("winnice")
#define APPVERSION MYL("1.0.0")

#include "libwinnice.h"

extern WNUShowInformation gUFShowInformation;
void ShowHelp(bool more=false);
