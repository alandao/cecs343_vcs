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


//This method will copy the entire folder structure from a directory to a target folder.
int copyStructure(std::wstring directoryAddress, std::wstring target) {
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	DWORD dwError = 0;

	std::wstring dirAddress(directoryAddress);

	// Find the first file in the directory.

	hFind = FindFirstFile(dirAddress.c_str(), &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
		return dwError;
	}

	// List all the files in the directory with some info about them.
	std::vector<std::wstring> directoryAddressses;
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{


			std:: wstring back = ffd.cFileName;
			if (back != L".." && back != L".") {

				std::wstring newDirectory = target + L"/" + back;
				CreateDirectory(newDirectory.c_str(), NULL);
				std::wstring tempAddress(directoryAddress);
				tempAddress.pop_back();
				tempAddress.pop_back();

				int result = copyStructure(tempAddress + L"/" + back + L"/**", newDirectory); 
			}

		}

	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
	}

	FindClose(hFind);
	return dwError;
}



int main(int argc, char *argv[], char *envp[])
{
	//initialize the three arguments.
	std::string arg1 = argv[1]; 
	std::string arg2 = argv[2];
	std::string arg3 = argv[3];
	std::string arg4 = argv[4];

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
	else if (arg1.compare("co") == 0) {
		std::wstring sourceFolder = std::wstring(arg3.begin(), arg3.end()) +L"/";
		std::wstring v = std::wstring(arg2.begin(), arg2.end()) + L"/";
		std::wstring targetFolder = std::wstring(arg4.begin(), arg4.end());
		int version = std::stoi(v);
		
		/* USED FOR MANUAL INPUT. 
		std::wstring targetFolder;
		std::wcout << "Enter a target folder and version" << std::endl;
		std::wcin >> targetFolder;
		int version;
		std::cin >> version;
		*/

		//finding that version of the ptree
		std::vector<std::wstring> manifestFiles;
		std::wstring manifestLoc = std::wstring(sourceFolder) + std::wstring(L"repo343/manifest/**");
		std::vector<Files> manifestFileDate;	
		int result = findFiles(manifestLoc.c_str(), manifestFiles);

		Files tempf;
		FILETIME ft;
		HANDLE h;
		//gets the creation date of all the manifest files
		for (std::wstring x : manifestFiles) {
			h = CreateFile(manifestLoc.c_str(), NULL, NULL, NULL, NULL, NULL, NULL);
			GetFileTime(h, &ft, NULL, NULL);
			tempf.filename = x;
			tempf.tm = ft;
			manifestFileDate.push_back(tempf);
			CloseHandle(h);
		}

		//sorts by date
		std::sort(manifestFileDate.begin(), manifestFileDate.end(), sortOnDate);
		//gets desired manifest
		std::wstring manifest = manifestFileDate.at(version - 1).filename;
		unsigned found = manifest.find_last_of(L"/\\");
		std::wstring shortManifest = manifest.substr(found + 1);
		std::wcout << shortManifest;

		//creating a new directory at the target folder and copying the folder structure
		CreateDirectory(targetFolder.c_str(), NULL);
		result = copyStructure(sourceFolder + L"**", targetFolder);

		//Creating a new manifest
		std::wofstream manifestFile;
		std::string date(currentDateTime());
		std::wstring manifestName = std::wstring(date.begin(), date.end());
		manifestFile.open(targetFolder + L"/repo343/manifest/" + manifestName + L".txt");
		manifestFile << "Copied from: " << shortManifest;


		//utilizing contents of manifest and adding them to a vector
		std::wstring line;
		std::wifstream myfile(manifest);
		std::vector <std::wstring> fileAddresses;
		std::wofstream newFile;
		if (myfile.is_open())
		{
			getline(myfile, line);//first line is redundant for this process
			while (getline(myfile, line))
			{
				fileAddresses.push_back(line);
			}
			myfile.close();
		}
		newFile.close();


		//reading contents of manifests file locations and adding them to target folder

		for (std::wstring x : fileAddresses) {
			unsigned found = x.find_first_of(L"/");
			std::wstring tempAddress = x.substr(found, x.length()+1);//address of files without the source folder name
			found = x.find_first_of(L" ");
			std::wstring pureAddress = x.substr(found + 1, x.length() + 1);//full address of files without "Created" in the front
			myfile.open (pureAddress);
			newFile.open(targetFolder + tempAddress, std::ofstream::out);
			manifestFile << std::endl << targetFolder + tempAddress;
			while (getline(myfile, line)) {

				newFile << line;
				if (myfile.peek() != EOF)
					newFile << std::endl;

			}
			myfile.close();
			newFile.close();
		}
	}
	
	return 0;
}