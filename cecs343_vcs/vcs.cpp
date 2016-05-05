#include <Windows.h>
#include <string>

#include "vcs.h"
//

__int64 FileSize64(LPCWSTR szFileName)
{
	struct __stat64 fileStat;
	int err = _wstat64(szFileName, &fileStat);
	if (0 != err)
		return 0;
	return fileStat.st_size;
}


int CheckSum(LPCWSTR filepath) {
	int filesize = FileSize64(filepath);
	if (filesize == 0)
		return -1;
	else
		return filesize % 256;
}


void TrackFile(LPCWSTR filepath, LPCWSTR tgtFolder) {
	//copies file from filepath into manifest
	//calls CreateDirectory always
	LPCWSTR newfolder = (std::wstring(tgtFolder) + std::wstring(L"repo343/") + 
							std::wstring(filepath)).c_str();

	//CreateDirectory(filepath, NULL);
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