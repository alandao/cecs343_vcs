//This module contains functions for navigating the Windows Filesystem.
#pragma once

#include <Windows.h>
#include <string>
#include <io.h>

//Used for counting amount of manfiests
int numberOfFilesinDirectory(std::wstring directoryAddress);

//I don't know what this does, but it literally makes Files
struct Files {
	std::wstring filename;
	FILETIME tm;
};

//Richard:: Send this method the address of a directory, and a vector where it will store the address of all the files
//NOTE: Vector is passed by reference. Works recursively with all subfolders. Omits "." & ".."
int findFiles(std::wstring directoryAddress, std::vector<std::wstring>& addressVector);

/*
Parameters: Send it two FULL addresses of manifests. /repo343/manifest/blah.txt
Return: Will return to you the address of their most recent common ancestor.
	NOTE: It will return just the name of the file. If you want the full address, add "directoryAddress" to "return left"
Works recursively by comparing the string names. It compares time in string format, and calls the parent of most recent file.
	Will traverse upwards in a binary tree fashion.
	Worst case: n+n
	Best case: 1
*/
std::wstring findAncestor(std::wstring left, std::wstring right, std::wstring repoPath);

//Richard:: This method will copy the entire folder structure from a directory to a target folder.

int copyStructure(std::wstring directoryAddress, std::wstring target);


// Retrieve the system error message for the last-error code
void DisplayErrorBox(LPTSTR lpszFunction);

//Local time returned in YYYY-MM-DD @ HH;MM;SS format
//Windows is dumb and won't let me use colons (:). Thanks Bill Gates.
const std::string currentDateTime();

//Sorts by date, returns a bool if something is greater. 
//yeah.
bool sortOnDate(const Files& fA, const Files& fB);

//returns a full filepath from a relative path.
std::wstring fullFilePath(const std::wstring& relativePath);

//returns the nTh line of a file.
std::wstring lineFromFile(const std::wstring& filename, int n);

//splits string on a delimiter
std::vector<std::wstring> split(const std::wstring &s, std::wstring delim);