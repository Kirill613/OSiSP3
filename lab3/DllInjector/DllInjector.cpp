#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
DWORD GetProcessId(std::string process)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            std::wstring name(process.begin(), process.end());
            if (wcscmp(entry.szExeFile, name.c_str()) == 0)
            {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        }
    }
}

bool Inject(DWORD pid, const char* data) {
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (!process) return false;
    int size = strlen(data) + 1;

    LPVOID loadLibrary = (LPVOID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "LoadLibraryA");
    LPVOID memory = (LPVOID)VirtualAllocEx(process, 0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(process, (LPVOID)memory, data, size, 0); 
    HANDLE thread = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)loadLibrary, (LPVOID)memory, 0, 0);
    

    WaitForSingleObject(thread, INFINITE);

    VirtualFreeEx(process, (LPVOID)memory, 0, MEM_RELEASE);
    CloseHandle(thread);
    CloseHandle(process);

    return true;
}
int main()
{
    DWORD pID = GetProcessId("TestApp.exe");
    if (Inject(pID, "DllForInjection.dll"))
    std::cout << "Dll injected" << std::endl;
    else
        std::cout << "Dll not injected" << std::endl;
    std::cin.get();
}