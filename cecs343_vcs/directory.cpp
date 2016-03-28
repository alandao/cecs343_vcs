#include <string>
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <time.h>
#include <atlbase.h>
#include <vector>

#include "directory.h"

// alan: we shouldn't need <iostream> when this module is done since we want findFiles to return 
// a std::vector<std::string>


int findFiles(std::wstring directoryAddress, std::vector<std::wstring>& addressVector) {
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	DWORD dwError = 0;

	std::wstring dirAddress(directoryAddress);

	// Find the first file in the directory.

	hFind = FindFirstFile(dirAddress.c_str(), &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
		return dwError;
	}

	// List all the files in the directory with some info about them.
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			std::wstring tempString(dirAddress);
			std::wstring back = ffd.cFileName;
			tempString.pop_back();
			tempString.pop_back();
			tempString += ffd.cFileName;
			//std::wcout << tempString;
			if (back != L".." && back != L".") {
				tempString += L"/**";
				findFiles(tempString, addressVector);
			}

		}
		else
		{
			std::wstring location = directoryAddress;
			location.pop_back();
			location.pop_back();
			location += ffd.cFileName;
			addressVector.push_back(location);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
	}

	FindClose(hFind);
	return dwError;
}

const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d @ %I;%M;%S %p", &tstruct);
	return buf;
}

void DisplayErrorBox(LPTSTR lpszFunction)
{
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

	// Display the error message and clean up

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40)*sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}


