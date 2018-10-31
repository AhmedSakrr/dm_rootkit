/*
	##############################################
	### (C) Ratinho                           ####
	###     Modifications by vbCrLf           ####
	###                                       ####
	###      Written for DigitalWhisper       ####
	###    http://www.digitalwhisper.co.il/   ####
	##############################################
*/

#ifndef _IATHOOKER_H_
#define _IATHOOKER_H_
 
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
using namespace std;

#define IMPORT_TABLE_OFFSET 1

bool HookIAT(HMODULE hInstance, LPCSTR targetFunction, DWORD newFunc, DWORD *oldFunc);
bool rewriteThunk(PIMAGE_THUNK_DATA pThunk, DWORD newFunc, DWORD *oldFunc);
PIMAGE_IMPORT_DESCRIPTOR getImportTable(HMODULE);
BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved);
 
#endif /* _IATHOOKER_H_ */