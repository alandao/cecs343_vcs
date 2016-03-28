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
	std::vector<std::wstring> addresses; //Richard: This vector contains the addresses of all the files in a given repo. 
	//const LPCWSTR src = L"src_test/**"; Richard: LPCWSTR is no longer needed.
	std::wstring src = L"src_test/**";
	int result = findFiles(src, addresses);
	for (std::vector<std::wstring>::iterator it = addresses.begin(); it != addresses.end(); ++it) {
		std::wcout << *it << std::endl;
	}

	std::cin >> arg1[1];


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