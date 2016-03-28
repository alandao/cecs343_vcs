//This module contains functions for the version control system.
#pragma once

#include <Windows.h>


//returns the filesize of a given file.
__int64 FileSize64(LPCWSTR szFileName);

//test commit f

//gets the files bytes, and modulos it by 256 to get a number from 0 to 255.
//returns -1 if filesize fails.
int CheckSum(LPCWSTR filepath);


//Tracks a file into the repo. Returns an string log that will be recorded in the manifest.
std::wstring TrackFile(LPCWSTR filepath, LPCWSTR targetfolder);