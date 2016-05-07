#include <string>
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <time.h>
#include <atlbase.h>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>

#include "directory.h"

// alan: we shouldn't need <iostream> when this module is done since we want findFiles to return 
// a std::vector<std::string>

int numberOfFilesinDirectory(std::wstring directoryAddress) {
	std::vector<std::wstring> filepaths;
	int result = findFiles(directoryAddress.c_str(), filepaths);
	return filepaths.size();
}

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

/*
Parameters: Send it two FULL addresses of manifests. /repo343/manifest/blah.txt
Return: Will return to you the address of their most recent common ancestor.
	NOTE: It will return just the name of the file. If you want the full address, add "directoryAddress" to "return left"
Works recursively by comparing the string names. It compares time in string format, and calls the parent of most recent file.
	Will traverse upwards in a binary tree fashion.
	Worst case: n+n
	Best case: 1
*/
std::wstring findAncestor(std::wstring left, std::wstring right) {
	std::wstring directoryAddress = L"repo343/manifest/";
	std::wstring hold;
	std::wifstream fileRead;
	unsigned found = left.find_last_of(L"/\\");		
	left = left.substr(found + 1);
	found = right.find_last_of(L"/\\");
	right = right.substr(found + 1);


	if (left.substr(0, 10).compare(right.substr(0, 10)) == 0) { //same date
		if (left.substr(12).compare(right.substr(12)) == 0) //same time
			return left;
		else if (left.substr(12).compare(right.substr(12)) == 1) { //left is more recent; call it's parent
			fileRead.open(directoryAddress + left);
			std::getline(fileRead, hold);
			std::getline(fileRead, hold);
			return findAncestor(directoryAddress + hold.substr(8), right);
		}
		else {	//right is more recent; call it's parent
			fileRead.open(directoryAddress + right);
			std::getline(fileRead, hold);
			std::getline(fileRead, hold);
			return findAncestor(left, directoryAddress + hold.substr(8));
		}
	}

	else if (left.substr(0, 10).compare(right.substr(0, 10)) == 1) { //left is more recent; call it's parent
		fileRead.open(directoryAddress + left);
		std::getline(fileRead, hold);
		std::getline(fileRead, hold);
		return findAncestor(directoryAddress + hold.substr(8), right);
	}

	else {
		fileRead.open(directoryAddress + right);
		std::getline(fileRead, hold);
		std::getline(fileRead, hold);
		return findAncestor(left, directoryAddress + hold.substr(8));
	}


}

int copyStructure(std::wstring directoryAddress, std::wstring target) {
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
	std::vector<std::wstring> directoryAddressses;
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{


			std::wstring back = ffd.cFileName;
			if (back != L".." && back != L".") {

				//alan: ugly hack below so we dont make folders for files.
				std::wstring newDirectory = target + L"/";
				CreateDirectory(newDirectory.c_str(), NULL);
				newDirectory = target + L"/" + back;

				std::wstring tempAddress(directoryAddress);
				tempAddress.pop_back();
				tempAddress.pop_back();

				int result = copyStructure(tempAddress + L"/" + back + L"/**", newDirectory);
			}

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

bool sortOnDate(const Files& fA, const Files& fB) {
	FILETIME ftA, ftB;
	ftA = fA.tm;
	ftB = fB.tm;
	return CompareFileTime(&ftA, &ftB)<0;
}

std::wstring fullFilePath(const std::wstring& relativePath) {
	wchar_t full[_MAX_PATH];
	if (_wfullpath(full, relativePath.c_str(), _MAX_PATH) != NULL)
		return std::wstring(full);
	else
		return std::wstring(L"");
}

std::wstring lineFromFile(const std::wstring& filename, int n) {
	std::wifstream in(filename.c_str());

	std::wstring s;
	
	//skip N lines
	for (int i = 1; i < n; i++)
		std::getline(in, s);

	std::getline(in, s);
	return s;
}

std::vector<std::wstring> split(const std::wstring &s, std::wstring delim) {
	std::vector<std::wstring> elems;
	std::wstring temp = s;
	wchar_t * wcs = &temp[0];
	wchar_t * pwc;
	wchar_t * buffer;
	pwc = wcstok_s(wcs, delim.c_str(), &buffer);
	while (pwc) {
		elems.push_back(pwc);
		pwc = wcstok_s(nullptr, delim.c_str(), &buffer);
	}
	return elems;
}