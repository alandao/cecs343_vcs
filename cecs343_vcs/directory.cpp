#include <string>
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <time.h>
#include <atlbase.h>

#include "directory.h"

// alan: we shouldn't need <iostream> when this module is done since we want findFiles to return 
// a std::vector<std::string>
#include <iostream>



/*the asterisks are required at the end of the address. In between them specify the type of file you want, or leave
them empty to look for all files, including folders.*/

void DumpEntry(_finddata_t &data, const char * address) {
	std::string createtime(ctime(&data.time_create));
	std::cout << Chop(createtime) << "\t";
	std::cout << data.size << "\t";

	//this if statement will execute when a subdirectory has been found
	if ((data.attrib & _A_SUBDIR) == _A_SUBDIR) {
		std::cout << "[" << data.name << "]" << std::endl;
		std::string temp = address;
		temp.pop_back();
		temp.pop_back();
		temp = temp + data.name + "/**";
		std::string folder = data.name;
		std::cout << temp << std::endl;
		if (folder == "x64") {
			//findFiles(temp.c_str());
		}
	}
	else {
		std::cout << data.name << std::endl;
	}
}


int findFiles(LPCWSTR directoryAddress) {
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
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
			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
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

std::string Chop(std::string &str) {
	std::string res = str;
	int len = str.length();
	if (str[len - 1] == '\r') {
		res.replace(len - 1, 1, "");
	}
	len = str.length();
	if (str[len - 1] == '\n') {
		res.replace(len - 1, 1, "");
	}
	return res;
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


