#pragma once

#include <string>
#include <Windows.h>

int CheckSum(LPCWSTR filepath);
__int64 FileSize64(LPCWSTR szFileName);
void TrackFile(LPCWSTR filepath, LPCWSTR targetfolder);
