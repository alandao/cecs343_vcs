// cecs343_vcs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include "cecs343_vcs.h"
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>
#include <io.h>
#include <time.h>

using namespace std;

/*
void findFile(int argc, TCHAR *argv[]) {
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	if (argc != 2) {
		_tprintf(TEXT("Usage: %s [target_file]\n"), argv[0]);
		return;
	}

	_tprintf(TEXT("Target file is %s\n"), argv[1]);
	hFind = FindFirstFile(argv[1], &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return;
	}
	else {
		_tprintf(TEXT("The first file found is %s\n"),
			FindFileData.cFileName);
		FindClose(hFind);
	}
}

*/

string Chop(string &str) {
	string res = str;
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

__int64 FileSize64(LPCWSTR szFileName)
{
	struct __stat64 fileStat;
	int err = _wstat64(szFileName, &fileStat);
	if (0 != err) 
		return 0;
	return fileStat.st_size;
}

//gets the files bytes, and modulos it by 256 to get a number from 0 to 255.
//returns -1 if filesize fails.
int CheckSum(LPCWSTR filepath) {
	int filesize = FileSize64(filepath);
	if (filesize == 0)
		return -1;
	else
		return filesize % 256;
}
void TrackFile(LPCWSTR filepath) {
	//copies file from filepath into manifest
	//calls CreateDirectory always
	LPCWSTR newfolder = (wstring(L"manifest\\") + wstring(filepath)).c_str();
	CreateDirectory(filepath, NULL);
	bool loop = true;
	int filecount = 1;
	/*
	while (loop) {
		wstring stemp2 = newfolder + "\\" + filepath + " - " + to_string(filecount));
		//this name is supposed to be a checksum.
		LPCWSTR newfolderLPC = stemp2.c_str();
		if (CopyFile(filepath, newfolder, true)) {
			cout << "File copied";
			filecount = 1;
			loop = false;
		}
		else {
			cout << "File exists";
			filecount++;
		}
	} */

}






void DumpEntry(_finddata_t &data, const char * address ) {
	string createtime(ctime(&data.time_create));
	cout << Chop(createtime) << "\t";
	cout << data.size << "\t";
	if ((data.attrib & _A_SUBDIR) == _A_SUBDIR) {
		cout << "[" << data.name << "]" << endl;
		string temp = address;
		temp.pop_back();
		temp.pop_back();
		temp = temp + data.name + "/**";
		cout << temp << endl;
		if (data.name == "x64") {
			findFiles(temp.c_str());
		}
	}
	else {
		cout << data.name << endl;
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
	cin >> yolo;
	return 0;
}






void DumpEntry(_finddata_t &data, const char * address ) {
	string createtime(ctime(&data.time_create));
	cout << Chop(createtime) << "\t";
	cout << data.size << "\t";
	if ((data.attrib & _A_SUBDIR) == _A_SUBDIR) {
		cout << "[" << data.name << "]" << endl;
		string temp = address;
		temp.pop_back();
		temp.pop_back();
		temp = temp + data.name + "/**";
		cout << temp << endl;
		if (data.name == "x64") {
			findFiles(temp.c_str());
		}
	}
	else {
		cout << data.name << endl;
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
	cin >> yolo;
	return 0;
}

int main()
{
	// g++ create_repo sourcefolder targetfolder
	//source folder has the original files, target will is where it will copied to
	cout << argv[1] << endl;
	if (std::string(argv[1]) == "create_repo") {
		LPCWSTR sourcefolder = (wchar_t*)argv[2];
		LPCWSTR targetfolder = (wchar_t*)argv[3];
		cout << "Copying from: " << sourcefolder << " into: " << targetfolder << endl;
	}
	cout << FileSize64(L"h.txt") << endl;
	cout << CheckSum(L"h.txt") << endl;
	const char* directoryAddress = "c:/users/richard/source/repos/cecs343_vcs/cecs343_vcs/**";
	int result = findFiles(directoryAddress);
	return 0;
}