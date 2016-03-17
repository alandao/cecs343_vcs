// cecs343_vcs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <string>
using namespace std;

int main()
{
	string h = "c:\\Users\\Richard\\";
	LPCWSTR a = L"C:\\Users\\Richard";
	CreateDirectory(a, NULL);;
}

/*
void CreateFolder(const char * path) {
	if (!CreateDirectory(path, NULL)) {
		return;
	}
}
*/