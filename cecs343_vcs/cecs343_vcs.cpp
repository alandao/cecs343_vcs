//The command line application.

// USAGE: cecs343_vcs create_repo sourcefolder targetfolder
//	This will create a repo at the target folder.

#include <string>
#include <iostream>

//our libraries
#include "directory.h"
#include "vcs.h"

int main(int argc, char *argv[], char *envp[])
{
	//initialize the three arguments.
	std::string arg1(argv[1]); 
	std::string arg2(argv[2]);
	std::string arg3(argv[3]);

	std::cout << arg1 << std::endl;

	//alan: this works.
	const LPCWSTR src = L"src_test/**";
	int list = findFiles(src);

	//folders must exist before copying files over to that folder.
	//const LPCWSTR test = L"target_folder/test/one";
	//bool result = CopyFile(L"src_test/a.txt", L"target_test/a.txt/158.txt", true);

	//if arg1 is equal to create_repo, create a repo.
	if (arg1.compare("create_repo") == 0) {
		std::wstring targetfolder = std::wstring(arg3.begin(), arg3.end()) + L"/";
		std::wstring sourcefolder = std::wstring(arg2.begin(), arg2.end()) + L"/";
		//this printing thing is broken. Idea is to convert argv into a string then string into LPC.
		//allows for use of string as needed.
		std::wcout << "Copying from: " << sourcefolder << " into: " << targetfolder << std::endl;
		std::wstring action = TrackFile(L"src_test/test/c.txt", targetfolder.c_str());
	}

	return 0;
}