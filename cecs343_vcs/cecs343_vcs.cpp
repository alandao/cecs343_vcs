// cecs343_vcs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include "cecs343_vcs.h"


using namespace std;


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

int main()
{
	cout << FileSize64(L"h.txt") << endl;
	cout << CheckSum(L"h.txt") << endl;
	return 0;
}