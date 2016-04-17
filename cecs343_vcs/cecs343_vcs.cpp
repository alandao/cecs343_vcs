//The command line application.

// USAGE: cecs343_vcs create_repo sourcefolder targetfolder
//	This will create a repo at the target folder.

#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <time.h>
#include <algorithm>
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

		//make a new file called manifest where appropriate, cat actions done to the manifest
		//Filename is timestamp, contents are actions done. (added/removed/moved/edited files)
		std::wstring manifestLoc = std::wstring(targetfolder) + std::wstring(L"repo343/manifest");
		CreateDirectory(targetfolder.c_str(), NULL);
		CreateDirectory((targetfolder + L"repo343/").c_str(), NULL);
		CreateDirectory(manifestLoc.c_str(), NULL);

		//filename is the time of changes made.
		std::string nowDate(currentDateTime());
		std::wstring manifestName = std::wstring(nowDate.begin(), nowDate.end());
		std::wofstream outputFile(manifestLoc + std::wstring(L"/") + manifestName + std::wstring(L".txt"));
		outputFile << L"I was born." << std::endl;

		//Richard: This vector contains the addresses of all the files in a given repo. 
		std::vector<std::wstring> filepaths;
		std::wstring src = sourcefolder + L"**";
		int result = findFiles(src.c_str(), filepaths);
		std::wcout << L"Source Folder: " << sourcefolder << L"\nTarget Folder: " << targetfolder << std::endl;

		for (std::wstring x : filepaths) {
			std::wstring action = TrackFile(x.c_str(), targetfolder.c_str());
			outputFile << action << std::endl;
		}
		outputFile.close();
	}
	//to check in
	else if (arg1.compare("check_in") == 0) {
		std::wstring targetfolder = std::wstring(arg3.begin(), arg3.end()) + L"/";
		std::wstring sourcefolder = std::wstring(arg2.begin(), arg2.end()) + L"/";
		//goes to where all the manifests are
		std::vector<std::wstring> manifestFiles;
		std::wstring tgtmanifestLoc = std::wstring(targetfolder) + std::wstring(L"repo343/manifest/**");
		int result = findFiles(tgtmanifestLoc.c_str(), manifestFiles);

		std::vector<Files> manifestFileDate;
		Files tempf;
		FILETIME ft;
		HANDLE h;
		//gets the creation date of all the manifest files
		for (std::wstring x : manifestFiles) {
			h = CreateFile(tgtmanifestLoc.c_str(), NULL, NULL, NULL, NULL, NULL, NULL);
			GetFileTime(h, &ft, NULL, NULL);
			tempf.filename = x;
			tempf.tm = ft;
			manifestFileDate.push_back(tempf);
			CloseHandle(h);
		}
		//sorts by date, oldest to youngest
		std::sort(manifestFileDate.begin(), manifestFileDate.end(), sortOnDate);
		//gets the youngest file's name
		std::wstring latestManifest = manifestFileDate.back().filename;
		unsigned found = latestManifest.find_last_of(L"/\\");
		std::wstring lastManifestName = latestManifest.substr(found + 1);

		//this is basically create_repo
		std::wstring manifestLoc = std::wstring(targetfolder) + std::wstring(L"repo343/manifest");
		CreateDirectory(targetfolder.c_str(), NULL);
		CreateDirectory((targetfolder + L"repo343/").c_str(), NULL);
		CreateDirectory(manifestLoc.c_str(), NULL);

		//filename is the time of changes made.
		std::string nowDate(currentDateTime());
		std::wstring manifestName = std::wstring(nowDate.begin(), nowDate.end());
		std::wofstream outputFile(manifestLoc + std::wstring(L"/") + manifestName + std::wstring(L".txt"));
		outputFile << L"Previous manifest: " << lastManifestName << std::endl;

		//Richard: This vector contains the addresses of all the files in a given repo. 
		std::vector<std::wstring> filepaths;
		std::wstring src = sourcefolder + L"**";
		int result2 = findFiles(src.c_str(), filepaths);
		std::wcout << L"Source Folder: " << sourcefolder << L"\nTarget Folder: " << targetfolder << std::endl;

		for (std::wstring x : filepaths) {
			std::wstring action = TrackFile(x.c_str(), targetfolder.c_str());
			outputFile << action << std::endl;
		}
		outputFile.close();

	}
	//co is to checkout
	/*else if (arg1.compare("co") == 0) {
		std::wstring sourcefolder = std::wstring(arg3.begin(), arg3.end()) + L"/";
		std::wstring version = std::wstring(arg2.begin(), arg2.end()) + L"/";
		std::wstring targetfolder = std::wstring(arg4.begin(), arg4.end());

	}
	*/
	return 0;
}