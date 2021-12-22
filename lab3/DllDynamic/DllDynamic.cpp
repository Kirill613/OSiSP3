#include "Windows.h"
#include "iostream"
typedef void __cdecl TReplaceString(DWORD pid, const char* src, const char* res);
using namespace std;
int main()
{
	DWORD pid = GetCurrentProcessId();
	HINSTANCE lib;
	lib = LoadLibrary(L"Dll.dll");
	if (lib != NULL) {
		TReplaceString* proc = (TReplaceString*)GetProcAddress(lib, "ReplaceString");
		if (NULL != proc) {
			const char src[] = "ABCDEFG";
			const char res[] = "HIGJKLM";
			cout << "Old string: " << src << endl;
			proc(pid, src, res);
			cout << "New string: " << src << endl << endl;
		}
		else {
			cout << "Proc not found";
		}
		FreeLibrary(lib);
	}
	cin.get();
	return 0;
}