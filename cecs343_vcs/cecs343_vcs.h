#ifndef cecs343_vcs
#define cecs343_vcs
#include <string>
#include <Windows.h>

int CheckSum(LPCWSTR filepath);
__int64 FileSize64(LPCWSTR szFileName);
void TrackFile(LPCWSTR filepath);

#pragma once
#endif cecs343_vcs