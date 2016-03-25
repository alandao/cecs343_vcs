//This module contains functions for navigating the Windows Filesystem.
#pragma once

#include <Windows.h>
#include <string>
#include <io.h>

//Will dump out all the info. Folders are printed out in brackets[]
void DumpEntry(_finddata_t &data, const char * address);

/*will find all files in the given address*/
int findFiles(const char*);

std::string Chop(std::string &str);

// Retrieve the system error message for the last-error code
void DisplayErrorBox(LPTSTR lpszFunction);