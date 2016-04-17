//This module contains functions for navigating the Windows Filesystem.
#pragma once

#include <Windows.h>
#include <string>
#include <io.h>

//Used for counting amount of manfiests
int numberOfFilesinDirectory(std::wstring directoryAddress);

//I don't know what this does, but it literally makes Files
struct Files {
	std::string filename;
	FILETIME tm;
};

//Richard:: Send this method the address of a directory, and a vector where it will store the address of all the files
//NOTE: Vector is passed by reference. Works recursively with all subfolders. Omits "." & ".."
int findFiles(std::wstring directoryAddress, std::vector<std::wstring>& addressVector);

// Retrieve the system error message for the last-error code
void DisplayErrorBox(LPTSTR lpszFunction);

//Local time returned in YYYY-MM-DD @ HH;MM;SS format
//Windows is dumb and won't let me use colons (:). Thanks Bill Gates.
const std::string currentDateTime();

//Sorts by date, returns a bool if something is greater. 
//yeah.
bool sortOnDate(const Files& fA, const Files& fB);