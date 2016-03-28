//The command line application.

// USAGE: cecs343_vcs create_repo sourcefolder targetfolder
//	This will create a repo at the target folder.

#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
//our libraries
#include "directory.h"
#include "vcs.h"

const std::string currentDateTime() {
	//Time returned in YYYY-MM-DD @ HH;MM;SS format
	//Windows is dumb and won't let me use colons (:). Thanks Bill Gates.
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d @ %I;%M;%S %p", &tstruct);
	return buf;
}

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
		//make a new file called manifest where appropriate, cat actions done to the manifest
		//Filename is timestamp, contents are actions done. (added/removed/moved/edited files)
		std::wstring manifestLoc = std::wstring(targetfolder) + std::wstring(L"repo343/manifest");
		CreateDirectory(manifestLoc.c_str(), NULL);
		//filename is the time of changes made.
		std::string nowDate(currentDateTime());
		std::wstring manifestName = std::wstring(nowDate.begin(), nowDate.end());
		std::ofstream outputFile(manifestLoc + std::wstring(L"/") + manifestName + std::wstring(L".txt"));
		outputFile << "I was born." << std::endl;
		outputFile << nowDate << std::endl;
		outputFile.close();
	}

	return 0;
}
