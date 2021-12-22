#include "pch.h"

extern "C" void __declspec(dllexport) __cdecl ReplaceString(DWORD pid,
	const char* src,	const char* res)
{
	HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE 
		| PROCESS_QUERY_INFORMATION, FALSE, pid);

	if (hProcess)
	{
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);

		MEMORY_BASIC_INFORMATION memoryInfo;
		std::vector<char> buffer;
		char* baseAddress = 0;

		while (baseAddress < systemInfo.lpMaximumApplicationAddress)
		{
			if (VirtualQueryEx(hProcess, baseAddress, &memoryInfo, sizeof(memoryInfo)) == sizeof(memoryInfo))
			{
				if (memoryInfo.State == MEM_COMMIT && memoryInfo.AllocationProtect == PAGE_READWRITE)
				{
					baseAddress = (char*)memoryInfo.BaseAddress;

					buffer.resize(memoryInfo.RegionSize);
					SIZE_T bytesRead;

					try
					{
						if (ReadProcessMemory(hProcess, baseAddress, &buffer[0], memoryInfo.RegionSize, &bytesRead))
						{
							for (size_t i = 0; i < (bytesRead - strlen(src)); ++i)
							{
								if (memcmp(src, &buffer[i], strlen(src)) == 0)
								{
									char* srcAddress = (char*)baseAddress + i;

									for (int j = 0; j < strlen(res); j++)
										srcAddress[j] = res[j];

									srcAddress[strlen(res)] = 0;
								}
							}
						}
					}
					catch (std::bad_alloc& e)
					{

					}
				}
				baseAddress += memoryInfo.RegionSize;
			}
		}
	}
}