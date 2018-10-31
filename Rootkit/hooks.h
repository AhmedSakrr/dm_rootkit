/*
	##############################################
	### (C) vbCrLf (at GMail), 2011           ####
	###     http://www.MerkazHaKfar.co.cc/    ####
	###                                       ####
	###      Written for DigitalWhisper       ####
	###    http://www.digitalwhisper.co.il/   ####
	##############################################
*/

#ifndef _HOOKED_H_
#define _HOOKED_H_

#include <Windows.h>

#define HIDE "hideit_"
#define HIDEW L"hideit_"

// FindFirstFileExA (Unicode)
extern DWORD originalFindFirstFileExA;
typedef HANDLE (WINAPI *ptrFindFirstFileExA)(__in LPCTSTR lpFileName, __in FINDEX_INFO_LEVELS fInfoLevelId, __out LPVOID lpFindFileData, __in FINDEX_SEARCH_OPS fSearchOp, __reserved  LPVOID lpSearchFilter, __in DWORD dwAdditionalFlags);
HANDLE WINAPI newFindFirstFileExA(__in LPCTSTR lpFileName, __in FINDEX_INFO_LEVELS fInfoLevelId, __out LPVOID lpFindFileData, __in FINDEX_SEARCH_OPS fSearchOp, __reserved  LPVOID lpSearchFilter, __in DWORD dwAdditionalFlags);

// FindFirstFileExW (Unicode)
extern DWORD originalFindFirstFileExW;
typedef HANDLE (WINAPI *ptrFindFirstFileExW)(__in LPCWSTR lpFileName, __in FINDEX_INFO_LEVELS fInfoLevelId, __out LPVOID lpFindFileData, __in FINDEX_SEARCH_OPS fSearchOp, __reserved  LPVOID lpSearchFilter, __in DWORD dwAdditionalFlags);
HANDLE WINAPI newFindFirstFileExW(__in LPCWSTR lpFileName, __in FINDEX_INFO_LEVELS fInfoLevelId, __out LPVOID lpFindFileData, __in FINDEX_SEARCH_OPS fSearchOp, __reserved  LPVOID lpSearchFilter, __in DWORD dwAdditionalFlags);

// FindNextFileA (ANSI)
extern DWORD originalFindNextFileA;
typedef BOOL (WINAPI *ptrFindNextFileA)(__in HANDLE hFindFile, __out  LPWIN32_FIND_DATAA lpFindFileData);
BOOL WINAPI newFindNextFileA(__in HANDLE hFindFile, __out  LPWIN32_FIND_DATAA lpFindFileData);

// FindNextFileW (Unicode)
extern DWORD originalFindNextFileW;
typedef BOOL (WINAPI *ptrFindNextFileW)(__in HANDLE hFindFile, __out  LPWIN32_FIND_DATAW lpFindFileData);
BOOL WINAPI newFindNextFileW(__in HANDLE hFindFile, __out  LPWIN32_FIND_DATAW lpFindFileData);

#endif /* _HOOKED_H_ */