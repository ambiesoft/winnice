#include <iostream>

#include "../libwinnice/libwinnice.h"

using namespace std;
void ShowInformation(const wchar_t* pMessage)
{
	wcout << pMessage << endl;
}
int main()
{
    return LibWinNiceMain(ShowInformation);
}
