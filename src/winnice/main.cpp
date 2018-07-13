#include <iostream>

#include "../libwinnice/libwinnice.h"

using namespace std;
void showOutput(const wchar_t* pMessage)
{
	wcout << pMessage;
}
void showError(const wchar_t* pMessage)
{
	wcerr << pMessage;
}
int main()
{
	return LibWinNiceMain(showOutput, showError);
}
