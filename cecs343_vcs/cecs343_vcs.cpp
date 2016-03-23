// cecs343_vcs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include "cecs343_vcs.h"


using namespace std;


__int64 FileSize64(const char * szFileName)
{
	struct __stat64 fileStat;
	int err = _stat64(szFileName, &fileStat);
	if (0 != err) return 0;
	return fileStat.st_size;
}

//gets the files bytes, and modulos it by 256 to get a number from 0 to 255.
int CheckSum(string filepath) {
	return 0;
}
void TrackFile(LPCWSTR filepath) {
	//copies file from filepath into manifest
	//calls CreateDirectory always
	LPCWSTR newfolder = (wstring(L"manifest\\") + wstring(filepath)).c_str();
	CreateDirectory(filepath, NULL);
	bool loop = true;
	int filecount = 1;
	while (loop) {
		wstring stemp2 = s2ws(newfolder + "\\" + filepath + " - " + to_string(filecount));
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
	}

}

int main()
{
	return 0;
}