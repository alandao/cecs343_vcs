//The command line application.

// USAGE: cecs343_vcs create_repo sourcefolder targetfolder
//	This will create a repo at the target folder.

#include <string>
#include <iostream>
#include <vector>

//our libraries
#include "directory.h"
#include "vcs.h"

int main(int argc, char *argv[], char *envp[])
{
	//initialize the three arguments.
	std::string arg1 = argv[1]; 
	std::string arg2 = argv[2];
	std::string arg3 = argv[3];

	//if arg1 is equal to create_repo, create a repo.
	if (arg1.compare("create_repo") == 0) {
		std::wstring targetfolder = std::wstring(arg3.begin(), arg3.end()) + L"/";
		std::wstring sourcefolder = std::wstring(arg2.begin(), arg2.end()) + L"/";

		//Richard: This vector contains the addresses of all the files in a given repo. 
		std::vector<std::wstring> filepaths;
		std::wstring src = sourcefolder + L"**";
		int result = findFiles(src.c_str(), filepaths);
		std::wcout << L"Source Folder: " << sourcefolder << L"\nTarget Folder: " << targetfolder << std::endl;

		for (std::wstring x : filepaths) {
			std::wstring action = TrackFile(x.c_str(), targetfolder.c_str());
		}
		
	}

	return 0;
}