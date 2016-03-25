// cecs343_vcs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include "cecs343_vcs.h"
#include <tchar.h>
#include <stdio.h>

//our libraries
#include "directory.h"

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

/*
ex:
filepath = "a.txt" - > Windows Explorer(TARGETFOLDER\repo343\a.txt\<checksum>.txt)
		 = "folder/anotherfolder/
*/
void TrackFile(LPCWSTR filepath, LPCWSTR tgtFolder) {
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


/*the asterisks are required at the end of the address. In between them specify the type of file you want, or leave 
them empty to look for all files, including folders.*/

int main(int argc, char *argv[], char *envp[])
{
	// g++ create_repo sourcefolder targetfolder
	//source folder has the original files, target will is where it will copied to

	//initialize the three arguments.
	string arg1(argv[1]);
	string arg2(argv[2]);
	string arg3(argv[3]);

	cout << arg1 << endl;
	//if arg1 is equal to create_repo
	if (arg1.compare("create_repo") == 0) {
		LPCWSTR sourcefolder = (wchar_t*)argv[2];
		LPCWSTR targetfolder = (wchar_t*)argv[3];
		//this printing thing is broken. Idea is to convert argv into a string then string into LPC.
		//allows for use of string as needed.
		//cout << "Copying from: " << sourcefolder << " into: " << targetfolder << endl;
	}
	cout << FileSize64(L"h.txt") << endl;
	cout << CheckSum(L"h.txt") << endl;
	const char* directoryAddress = "c:/users/richard/source/repos/cecs343_vcs/cecs343_vcs/**";
	int result = findFiles(directoryAddress);
	return 0;
}