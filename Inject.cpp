#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <sstream>

//#include "dirent.h"
#include "INIParser.h"
#include "LoadLibraryR.h"

#define BREAK_WITH_ERROR( e ) { printf( "[-] %s. Error=%d\n", e, GetLastError() ); break; }

using namespace cppiniparser;

//LPCSTR FindFile(LPCSTR fileType);

DWORD GetProcessID(LPCSTR processName);

int main()
{
	SetConsoleTitle("Injector");

	HANDLE hFile			= NULL;
	HANDLE hModule			= NULL;
	HANDLE hProcess			= NULL;
	HANDLE hToken			= NULL;
	HRSRC hResource			= NULL;
	HGLOBAL hLoaded			= NULL;
	LPVOID lpLock			= NULL;
	LPVOID lpBuffer			= NULL;
	DWORD dwLength			= 0;
	DWORD dwBytesRead		= 0;
	DWORD dwProcessId		= 0;
	TOKEN_PRIVILEGES priv	= { 0 };

	INIConfig config;
	INIParser::Read(config, "config.ini");

	bool autoClose = config.GetBoolean("settings", "autoclose");
	int closeDelay = config.GetInteger("settings", "closedelay");

	char szDllFile[MAX_PATH] = { 0 }; //FindFile(".dll");
	char szProcName[MAX_PATH] = { 0 };

	strcpy_s(szDllFile, config.Get("settings", "dll").c_str());
	strcpy_s(szProcName, config.Get("settings", "proc").c_str());

	printf("DLL : %s\n", szDllFile);
	printf("Process : %s\n\n", szProcName);

	do
	{
		if (szDllFile[0] == '\0')
			BREAK_WITH_ERROR("No DLL file specified");

		if (szProcName[0] == '\0')
			BREAK_WITH_ERROR("No target process specified");

		hFile = CreateFileA(szDllFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			BREAK_WITH_ERROR("Failed to open the DLL file");

		printf("[+] Waiting for %s...\n", szProcName);

		do {
			dwProcessId = GetProcessID(szProcName);
			Sleep(100);
		} while (!dwProcessId);

		dwLength = GetFileSize(hFile, NULL);
		if (dwLength == INVALID_FILE_SIZE || dwLength == 0)
			BREAK_WITH_ERROR("Failed to get the DLL file size");

		lpBuffer = HeapAlloc(GetProcessHeap(), 0, dwLength);
		if (!lpBuffer)
			BREAK_WITH_ERROR("Failed to get the DLL file size");

		if (ReadFile(hFile, lpBuffer, dwLength, &dwBytesRead, NULL) == FALSE)
			BREAK_WITH_ERROR("Failed to alloc a buffer!");

		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			priv.PrivilegeCount = 1;
			priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
				AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

			CloseHandle(hToken);
		}

		hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId);
		if (!hProcess)
			BREAK_WITH_ERROR("Failed to open the target process");
		
		hModule = LoadRemoteLibraryR(hProcess, lpBuffer, dwLength, NULL);
		if (!hModule)
			BREAK_WITH_ERROR("Failed to inject the DLL");

		printf("[+] Injected the '%s' DLL into process %d.\n", szDllFile, dwProcessId);

		WaitForSingleObject(hModule, -1);

	} while (0);

	if (lpBuffer)
		HeapFree(GetProcessHeap(), 0, lpBuffer);

	if (hProcess)
		CloseHandle(hProcess);

	if (!autoClose)
		system("pause");
	else
		Sleep(closeDelay);

	return 0;
}

//LPCSTR FindFile(LPCSTR fileType)
//{
//	dirent* entry;
//	int ret = 1;
//	DIR *dir;
//	char path[MAX_PATH];
//	GetCurrentDirectory(MAX_PATH, path);
//	dir = opendir(path);
//
//	while ((entry = readdir(dir)) != NULL)
//		if (strstr(entry->d_name, fileType)) return entry->d_name;
//
//	return NULL;
//}

DWORD GetProcessID(LPCSTR processName)
{
	PROCESSENTRY32 pe32;
	HANDLE hSnapshot;
	BOOL bFound;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	if (Process32First(hSnapshot, &pe32))
	{
		do {
			if (bFound = strcmp(pe32.szExeFile, processName) == 0)
				break;
		} while (Process32Next(hSnapshot, &pe32));
	}

	return bFound ? pe32.th32ProcessID : 0;
}