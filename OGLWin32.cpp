// Patch version

#include "OGLWin32.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <strsafe.h>
#include <iostream>

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "OGLApplication.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

void ErrorExit(LPTSTR lpszFunction) {
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

int APIENTRY wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	int exitcode = 0;

	// Allocate a console window so that messages can be redirected to stdout
	if (!AllocConsole()) {
		ErrorExit(TEXT("AllocConsole"));
	}

	FILE* pf_out;
	freopen_s(&pf_out, "CONOUT$", "w", stdout);

	// Create the application instance
	OGLApplication* myapp = OGLApplication::createApplication(hInstance);

	exitcode = myapp->run();

	myapp->destroyApplication();

	fclose(pf_out);

	// Free the console window
	if (!FreeConsole())
	{
		ErrorExit(TEXT("FreeConsole"));
	}

	return exitcode;
}