#include "Windows.h"
#include <iostream>
extern "C" void __declspec(dllexport) __cdecl ReplaceString(DWORD pid,
	const char* src, const char* res);
using namespace std;
int main()
{
	DWORD pid = GetCurrentProcessId();
	const char src[] = "ABCDEFG";
	const char res[] = "HIGJKLM";
	cout << "Old string: " << src << endl;
	ReplaceString(pid, src, res);
	cout << "New string: " << src << endl << endl;
	cin.get();
	return 0;
}