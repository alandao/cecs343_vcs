//This module contains functions for the version control system.
#pragma once

#include <Windows.h>


//returns the filesize of a given file.
__int64 FileSize64(LPCWSTR szFileName);

//test commit f

//gets the files bytes, and modulos it by 256 to get a number from 0 to 255.
//returns -1 if filesize fails.
int CheckSum(LPCWSTR filepath);

/*
ex:
filepath = "a.txt" - > Windows Explorer(TARGETFOLDER\repo343\a.txt\<checksum>.txt)
= "folder/anotherfolder/
*/
void TrackFile(LPCWSTR filepath, LPCWSTR targetfolder);