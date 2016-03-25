// cecs343_vcs.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>

//our libraries
#include "directory.h"
#include "vcs.h"

using namespace std;




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
		// LPCWSTR sourcefolder = (wchar_t*)argv[2];
		//LPCWSTR targetfolder = (wchar_t*)argv[3];
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