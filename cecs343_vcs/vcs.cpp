#include <Windows.h>
#include <sstream>
#include <vector>
#include <string>

#include "vcs.h"


__int64 FileSize64(LPCWSTR szFileName)
{
	struct __stat64 fileStat;
	int err = _wstat64(szFileName, &fileStat);
	if (err != 0)
		return -1;
	return fileStat.st_size;
}


int CheckSum(LPCWSTR filepath) {
	int filesize = FileSize64(filepath);
	if (filesize == -1)
		return -1; //something bad happened.
	return filesize % 256;
}


std::wstring TrackFile(LPCWSTR filepath, LPCWSTR tgtFolder) {
	std::wstring path = std::wstring(tgtFolder) + std::wstring(L"repo343/") +
		std::wstring(filepath);

	//string manipulation hoops.
	std::vector<std::wstring> folders;
	std::wistringstream f(path);
	std::wstring next;
	while (std::getline(f,next,L'/')) {
		folders.push_back(next);
	}
	//recreate filepaths from list of folders.
	std::vector<std::wstring> folderpaths;
	for (auto it = folders.begin(); it != folders.end(); it++) {
		std::wstring buffer(L"");
		for (auto it1 = folders.begin(); it1 != it; it1++) {
			if (buffer.compare(L"") == 0)
				buffer += *it1;
			else
				buffer += L"/" + *it1;
		}
		if (buffer.compare(L"") == 0)
			buffer += *it;
		else
			buffer += L"/" + *it;
		folderpaths.push_back(buffer);
	}
	//make folders.
	for (std::wstring x : folderpaths) {
		CreateDirectory(x.c_str(), NULL);
	}
	std::wstring artifactID = std::to_wstring(CheckSum(filepath));
	std::wstring newFilePath = (std::wstring(tgtFolder) + std::wstring(L"repo343/") + filepath + L"/" + artifactID + std::wstring(L".txt"));
	//copy file from src to target folder and rename as artifact id.
	CopyFile(filepath, newFilePath.c_str(), true);

	std::wstring action(L"A " + newFilePath);
	return action;
}