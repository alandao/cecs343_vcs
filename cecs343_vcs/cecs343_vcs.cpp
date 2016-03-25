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
	const char* src = "src_test/**";
	int result = findFiles("src_test/**");

	//if arg1 is equal to create_repo, create a repo.
	if (arg1.compare("create_repo") == 0) {
		// LPCWSTR sourcefolder = (wchar_t*)argv[2];
		//LPCWSTR targetfolder = (wchar_t*)argv[3];
		//this printing thing is broken. Idea is to convert argv into a string then string into LPC.
		//allows for use of string as needed.
		//cout << "Copying from: " << sourcefolder << " into: " << targetfolder << endl;
	}

	return 0;
}