//This module contains functions for navigating the Windows Filesystem.
#pragma once

#include <Windows.h>
#include <string>
#include <io.h>

//Will dump out all the info. Folders are printed out in brackets[]
void DumpEntry(_finddata_t &data, const char * address);

//Richard:: Send this method the address of a directory, and a vector where it will store the address of all the files
//NOTE: Vector is passed by reference. Works recursively with all subfolders. Omits "." & ".."
int findFiles(std::wstring directoryAddress, std::vector<std::wstring>& addressVector);

std::string Chop(std::string &str);

// Retrieve the system error message for the last-error code
void DisplayErrorBox(LPTSTR lpszFunction);

//Local time returned in YYYY-MM-DD @ HH;MM;SS format
//Windows is dumb and won't let me use colons (:). Thanks Bill Gates.
const std::string currentDateTime();