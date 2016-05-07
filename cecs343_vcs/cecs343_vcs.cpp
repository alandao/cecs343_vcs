//The command line application.

// USAGE: cecs343_vcs create_repo sourcefolder targetfolder
//	This will create a repo at the target folder.

//test commit

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
		std::wstring sourcefolder = std::wstring(arg2.begin(), arg2.end());

		//make a new file called manifest where appropriate, cat actions done to the manifest
		//Filename is timestamp, contents are actions done. (added/removed/moved/edited files)
		CreateDirectory(targetfolder.c_str(), NULL);
		CreateDirectory((targetfolder + L"repo343/").c_str(), NULL);
		CreateDirectory((targetfolder + L"repo343/manifest").c_str(), NULL);

		//filename is the time of changes made.
		std::string nowDate(currentDateTime());
		std::wstring manifestName = std::wstring(nowDate.begin(), nowDate.end());
		std::wofstream outputFile(targetfolder + L"repo343/manifest" + std::wstring(L"/") + manifestName + std::wstring(L".txt"));
		outputFile << L"@" + fullFilePath(sourcefolder) << std::endl << L"Parent: None" << std::endl;

		//Richard: This vector contains the addresses of all the files in a given repo. 
		std::vector<std::wstring> filepaths;
		std::wstring src = sourcefolder + L"/**";
		int result = findFiles(src.c_str(), filepaths);

		for (std::wstring x : filepaths) {
			std::wstring action = TrackFile(x.c_str(), targetfolder.c_str());
			outputFile << action << std::endl;
		}
		outputFile.close();
	}
	//to check in
	else if (arg1.compare("check_in") == 0) {
		std::wstring sourcefolder = std::wstring(arg2.begin(), arg2.end());
		std::wstring targetfolder = std::wstring(arg3.begin(), arg3.end()) + L"/";

		//goes to where all the manifests are
		std::vector<std::wstring> manifestFilepaths;
		std::wstring tgtmanifestLoc = std::wstring(targetfolder) + std::wstring(L"repo343/manifest/**");
		int result = findFiles(tgtmanifestLoc.c_str(), manifestFilepaths);

		std::vector<Files> manifestFiles;
		Files tempf;
		FILETIME ft;
		HANDLE h;
		//gets the creation date of all the manifest files
		for (std::wstring x : manifestFilepaths) {
			h = CreateFile(x.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
			if (h == INVALID_HANDLE_VALUE) {
				std::wcout << "Could not find manifest!" << std::endl;
			}
			GetFileTime(h, &ft, NULL, NULL);
			tempf.filename = x;
			tempf.tm = ft;
			manifestFiles.push_back(tempf);
			CloseHandle(h);
		}

		
		//sorts by date, oldest to youngest
		std::sort(manifestFiles.begin(), manifestFiles.end(), sortOnDate);

		//gets the youngest manifest's name whose src folder matches our src folder
		//filter out all manifestFiles who aren't from the same src folder

		auto newEnd = std::remove_if(manifestFiles.begin(), manifestFiles.end(), [&sourcefolder](const Files x) {
			return ((L"@" + fullFilePath(sourcefolder)).compare(lineFromFile(x.filename, 1)) != 0);
		});
		manifestFiles.erase(newEnd, manifestFiles.end());

		std::wstring latestManifest = manifestFiles.back().filename;
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
		outputFile << L"@" + fullFilePath(sourcefolder) << std::endl << L"Parent: " + lastManifestName << std::endl;

		//Richard: This vector contains the addresses of all the files in a given repo. 
		std::vector<std::wstring> filepaths;
		std::wstring src = sourcefolder + L"/**";
		int result2 = findFiles(src.c_str(), filepaths);

		for (std::wstring x : filepaths) {
			std::wstring action = TrackFile(x.c_str(), targetfolder.c_str());
			outputFile << action << std::endl;
		}
		outputFile.close();

	}
	//to checkout

	else if (arg1.compare("check_out") == 0) {
		std::string arg4 = argv[4];
		std::wstring sourceFolder = std::wstring(arg3.begin(), arg3.end()) +L"/";
		std::wstring v = std::wstring(arg2.begin(), arg2.end());
		std::wstring targetFolder = std::wstring(arg4.begin(), arg4.end());
		int version = std::stoi(v);

		//finding that version of the ptree
		std::vector<std::wstring> manifestFiles;
		std::wstring manifestAddresses = std::wstring(sourceFolder) + std::wstring(L"repo343/manifest/**");
		int result = findFiles(manifestAddresses.c_str(), manifestFiles);

		//sorting all the manifests
		std::vector<Files> manifestFileDate;
		Files tempf;
		FILETIME ft;
		HANDLE h;
		//gets the creation date of all the manifest files
		for (std::wstring x : manifestFiles) {
			h = CreateFile(x.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
			if (h == INVALID_HANDLE_VALUE) {
				std::wcout << "Could not find manifest!" << std::endl;
			}
			GetFileTime(h, &ft, NULL, NULL);
			tempf.filename = x;
			tempf.tm = ft;
			manifestFileDate.push_back(tempf);
			CloseHandle(h);
		}
		//sorts by date, oldest to youngest



		std::sort(manifestFileDate.begin(), manifestFileDate.end(), sortOnDate);

		for (Files x : manifestFileDate)
			std::wcout << x.filename <<std::endl;
		//gets the youngest file's name
	

		//gets desired manifest. full address and just the name
		std::wcout << manifestFileDate.at(version - 1).filename;
		std::wstring fullManifest = manifestFileDate.at(version - 1).filename;
		std::size_t found = fullManifest.find_last_of(L"/\\");
		std::wstring shortManifest = fullManifest.substr(found + 1);

		std::wstring firstLineManifest = lineFromFile(fullManifest, 1);
		std::wstring projectTreeName = split(firstLineManifest, L"\\").back();

		std::wstring src = sourceFolder + L"repo343/" + projectTreeName;

		//creating a new directory at the target folder and copying the folder structure
		CreateDirectory(targetFolder.c_str(), NULL);
		result = copyStructure(src + L"/**", targetFolder);

		//Creating a new manifest
		std::wofstream manifestFile;
		std::string date(currentDateTime());
		std::wstring manifestName = std::wstring(date.begin(), date.end());
		manifestFile.open(sourceFolder + L"/repo343/manifest/" + manifestName + L".txt");
		manifestFile << L"@" + fullFilePath(targetFolder) << std::endl << L"Parent: " + shortManifest << std::endl;


		//utilizing contents of manifest and adding them to a vector
		std::wstring line;
		std::wifstream myfile(fullManifest);
		std::vector <std::wstring> fileAddresses;//will contain addresses of all files specified in the manifest
		std::wofstream newFile;
		if (myfile.is_open())
		{
			getline(myfile, line);//first line is redundant for this process.
			getline(myfile, line);//second line is reundant since it's "parent: asdf"
			while (getline(myfile, line))
			{
				fileAddresses.push_back(line);
				manifestFile << line << std::endl;
			}
			myfile.close();
		}
		newFile.close();

		//reading contents of manifests file locations and adding them to user specified folder

		for (std::wstring x : fileAddresses) {
			std::vector<std::wstring> tokens = split(x, L"/");
			//delete first 3 elements because it is REPO/REPO343/SRCTREE
			tokens.erase(tokens.begin());
			tokens.erase(tokens.begin());
			tokens.erase(tokens.begin());
			//delete last element since it is artifact ID
			tokens.pop_back();
			std::wstring filePathIncludingTargetFolder = targetFolder;
			for (auto x : tokens) {
				filePathIncludingTargetFolder = filePathIncludingTargetFolder + L"/" + x;
			}
			if (!CopyFileW(x.c_str(), filePathIncludingTargetFolder.c_str(), false)) {
				std::cout << "Could not copy file!.\n" << std::endl;
			}
		}

	}
	else if (arg1.compare("merge") == 0) {
		//TODO:
		//Copy files over the way they should be done

		//Register command in main as : cecs343_vcs merge REPO MANIFEST1 TARGETFOLDER
		std::string arg4 = argv[4];
		std::wstring repoFolder = std::wstring(arg2.begin(), arg2.end());
		std::wstring manifestName = std::wstring(arg3.begin(), arg3.end());
		std::wstring targetFolder = std::wstring(arg4.begin(), arg4.end());
		std::vector<std::wstring> rManifestLines;
		std::vector<std::wstring> tManifestLines;

		std::wstring manifestAddress = std::wstring(repoFolder) + std::wstring(L"/repo343/manifest/") + std::wstring(manifestName);

		std::wstring strHold;
		std::wifstream rManifest, tManifest;

		//Start Manifest Find
		//goes to where all the manifests are
		std::vector<std::wstring> manifestFiles;
		std::wstring tgtmanifestLoc = std::wstring(targetFolder) + std::wstring(L"/repo343/manifest/**");
		int result = findFiles(tgtmanifestLoc.c_str(), manifestFiles);

		std::vector<Files> manifestFileDate;
		Files tempf;
		FILETIME ft;
		HANDLE h;
		//gets the creation date of all the manifest files
		for (std::wstring x : manifestFiles) {
			h = CreateFile(x.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
			if (h == INVALID_HANDLE_VALUE) {
				std::wcout << "Could not find manifest!" << std::endl;
			}
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
		//End Manifest Find

		//Joel's shit below this
		rManifest.open(manifestAddress);
		//get all lines to the end
		while (!rManifest.eof()) {
			std::getline(rManifest, strHold, L'\n');
			std::wcout << strHold << std::endl;
			rManifestLines.push_back(strHold);
		}
		rManifest.close();
		tManifest.open(latestManifest);
		while (!tManifest.eof()) {
			std::getline(tManifest, strHold, L'\n');
			std::wcout << strHold << std::endl;
			tManifestLines.push_back(strHold);
		}
		tManifest.close();
		std::vector<std::wstring> rDelimStr;
		std::vector<std::wstring> tDelimStr;
		
		std::vector<std::wstring>::iterator it;
		std::vector<std::wstring>::iterator it2;

		std::wstring newLoc;

		//Iterate through comparing the left (rManifest) to the right (tManifest).
		//uses a while loop, incrementing only when we have differences.
		//We artifically move through the vector by removing similarities at 0, any differences get left behind as we increment past it.
		int i = 0;
		while(i < rManifestLines.size()) {
			it = find(tManifestLines.begin(), tManifestLines.end(), rManifestLines.at(i));
			//Things were the same
			if (it != tManifestLines.end()) {
				//CopyFile(rManifestLines.at(i).c_str(), , true); Technically do nothing
				rManifestLines.erase(rManifestLines.begin());
				tManifestLines.erase(it);
			}
			else {
				//Things were different
				rDelimStr = split(rManifestLines.at(i));
				//find the filepath - end() index inside tManifest
				//if it exists, compare the last ending portion and then stick em inside a folder after a rename
				//current code status: can find the second to last file path portion, but not the whole thing minus the end
				std::wstring temp = *(rDelimStr.begin(), rDelimStr.end() - 3)+ L'/' + *(rDelimStr.begin(), rDelimStr.end() - 2);
				it2 = find(tManifestLines.begin(), tManifestLines.end() - 2, temp);
				//File path is the same
				if (it2 != tManifestLines.end()) {
					std::wcout << "Something" << std::endl;
					tDelimStr = split(*it2);
					//the file artifact sizes are different
					if (rDelimStr.back() != tDelimStr.back()){
						std::wcout << rDelimStr.back() << L'\n' << tDelimStr.back() << std::endl;
						//Make a new folder of both files in the target

						/*std::wstring mergedDir = (targetFolder + L"/" + *(tManifestLines.end() - 2) + L"/");
						std::wstring newRName, newTName;
						newRName = *(rDelimStr.end() - 2) + L"_MR";
						newTName = *(tDelimStr.end() - 2) + L"_MT";
						for (std::wstring x : rManifestLines) {
							std::vector<std::wstring> tokens = split(x, L"/");
							//delete first 3 elements because it is REPO/REPO343/SRCTREE
							tokens.erase(tokens.begin());
							tokens.erase(tokens.begin());
							tokens.erase(tokens.begin());
							//delete last element since it is artifact ID
							tokens.pop_back();
							std::wstring filePathIncludingTargetFolder = targetFolder;
							for (auto x : tokens) {
								filePathIncludingTargetFolder = filePathIncludingTargetFolder + L"/" + x;
							}
							if (!CopyFileW(x.c_str(), filePathIncludingTargetFolder.c_str(), false)) {
								std::cout << "Could not copy file!.\n" << std::endl;
							}
						}*/
						//CreateDirectory(mergedDir.c_str(), NULL);
						//CopyFile((mergedDir + newRName).c_str(), );
					}
				}
				std::wcout << temp << std::endl;
				i++;
			}
			//here is where we take the leftovers in both vectors and add them back in
			//they are files that didn't exist in either ptree
		}

		std::cout << " " << std::endl;
	}
	

	else if (arg1.compare("test") == 0) {
		std::wcout << findAncestor(L"repo343/manifest/2016-05-06 @ 01;19;32 PM.txt", L"repo343/manifest/2016-05-06 @ 01;17;35 PM.txt");

	}
	return 0;
}