// testtarget.cpp : Defines the entry point for the console application.
//

#include <Windows.h>

#include <stdio.h>

int gCtrlCCount;
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		++gCtrlCCount;
		printf("Ctrl-C event. %d more event will close the application.\n", 5-gCtrlCCount);
		Beep(750, 300);
		return !(gCtrlCCount >= 5);

		// CTRL-CLOSE: confirm that the user wants to exit. 
	case CTRL_CLOSE_EVENT:
		Beep(600, 200);
		printf("Ctrl-Close event\n\n");
		return(TRUE);

		// Pass other signals to the next handler. 
	case CTRL_BREAK_EVENT:
		Beep(900, 200);
		printf("Ctrl-Break event\n\n");
		return FALSE;

	case CTRL_LOGOFF_EVENT:
		Beep(1000, 200);
		printf("Ctrl-Logoff event\n\n");
		return FALSE;

	case CTRL_SHUTDOWN_EVENT:
		Beep(750, 500);
		printf("Ctrl-Shutdown event\n\n");
		return FALSE;

	default:
		return FALSE;
	}
}

int main()
{
	if (SetConsoleCtrlHandler(CtrlHandler, TRUE))
	{
		printf("The Control Handler is installed.\n");
		printf(" -- Now try pressing Ctrl+C or Ctrl+Break, or\n");
		printf("    try logging off or closing the console...\n");
		printf("    Pressing Ctrl-C five times will close the application.\n");
		printf("(...waiting in a loop for events...)\n");

		while (1) 
		{
			Sleep(1000);
		}
	}
	else
	{
		printf("\nERROR: Could not set control handler");
		return 1;
	}
    return 0;
}

