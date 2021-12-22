// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <iostream>
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	DWORD pid = GetCurrentProcessId();
	const char src[] = "ABCDEFG";
	const char res[] = "HIGJKLM";
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			ReplaceString(pid, src, res);
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

