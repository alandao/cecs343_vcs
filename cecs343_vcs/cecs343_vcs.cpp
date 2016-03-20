// cecs343_vcs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "cecs343_vcs.h"
#include <string>
#include <iostream>
using namespace std;

/*Will receive the desired path of new directory as LPCWTSTR

Returns true if succesffuly created; false otherwise
*/
bool CreateFolder(string path) {
	std::wstring stemp = s2ws(path);
	LPCWSTR LPCpath = stemp.c_str();
	return (CreateDirectory(LPCpath, NULL));
}

void TrackFile(string filepath) {
	//copies file from filepath into manifest
	//calls createfolder always
	//places files into folders, copyfile is 
	std::wstring stemp1 = s2ws(filepath);
	LPCWSTR filepathLPC = stemp1.c_str();
	string newfolder = "manifest\\" + filepath;
	CreateFolder(newfolder);
	bool loop = true;
	int filecount = 1;
	while (loop) {
		std::wstring stemp2 = s2ws(newfolder + "\\" + filepath + " - " + to_string(filecount));
		//this name is supposed to be a checksum.
		LPCWSTR newfolderLPC = stemp2.c_str();
		if (CopyFile(filepathLPC, newfolderLPC, true)) {
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

std::wstring s2ws(const std::string& s) {
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

int main()
{
	string top_directory = "manifest\\directory";
	string manifest = "manifest";
	bool test = CreateFolder(manifest);
	CreateFolder(top_directory);
	string filetest = "h.txt";
	TrackFile(filetest);
	if (test) {
		cout << "Directory succesfully created";
	}

	else if (ERROR_ALREADY_EXISTS == GetLastError()) {
		cout << "Already exists";
	}
	else {
		cout << "Failed for some other reason";
	}
}