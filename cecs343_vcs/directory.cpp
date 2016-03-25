
#include <string>
#include <Windows.h>
#include <strsafe.h>
#include <time.h>

#include "directory.h"

//we shouldn't need this when this module is done.
#include <iostream>



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
			findFiles(temp.c_str());
		}
	}
	else {
		std::cout << data.name << std::endl;
	}
}


int findFiles(const char* directoryAddress) {
	int yolo;
	_finddata_t data;
	int ff = _findfirst(directoryAddress, &data);
	if (ff != -1) {
		int res = 0;
		while (res != -1) {
			DumpEntry(data, directoryAddress);
			res = _findnext(ff, &data);
		}
		_findclose(ff);
	}
	std::cin >> yolo;
	return 0;
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

void DisplayErrorBox(LPTSTR lpszFunction) {
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
