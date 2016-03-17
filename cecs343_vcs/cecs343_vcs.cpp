// cecs343_vcs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <iostream>
using namespace std;

/*Will receive the desired path of new directory as LPCWTSTR

Returns true if succesffuly created; false otherwise
*/
bool CreateFolder(LPCWSTR &path) {
	return (CreateDirectory(path, NULL));
}


int main()
{
	LPCWSTR directory = L"C:\\Users\\Richard\\Directory";
	bool test = CreateFolder(directory);
	if (test) {
		cout << "Directory succesfully created";
	}

	else if (ERROR_ALREADY_EXISTS == GetLastError()) {
		cout << "Already exists";
	}
	else {
		cout << "Failed for some other reason";
	}
}