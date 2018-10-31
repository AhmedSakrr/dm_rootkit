/*
	##############################################
	### (C) Ratinho                           ####
	###     Modifications by vbCrLf           ####
	###                                       ####
	###      Written for DigitalWhisper       ####
	###    http://www.digitalwhisper.co.il/   ####
	##############################################
*/

#include "iathooker.h"

bool HookIAT(HMODULE hInstance, LPCSTR targetFunction, DWORD newFunc, DWORD *oldFunc)
{
	bool flag = false;

	PIMAGE_IMPORT_DESCRIPTOR importedModule;
	PIMAGE_THUNK_DATA pFirstThunk,pOriginalFirstThunk;
	PIMAGE_IMPORT_BY_NAME pFuncData;

	importedModule=getImportTable(hInstance);
	//pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hInstance, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize); - You can just call this function to get the Import Table
	while(*(WORD*)importedModule!=0) //over on the modules (DLLs)
	{
		pFirstThunk=(PIMAGE_THUNK_DATA)((PBYTE)hInstance+ importedModule->FirstThunk);//pointing to its IAT
		pOriginalFirstThunk=(PIMAGE_THUNK_DATA)((PBYTE)hInstance+ importedModule->OriginalFirstThunk);//pointing to OriginalThunk
		pFuncData=(PIMAGE_IMPORT_BY_NAME)((PBYTE)hInstance+ pOriginalFirstThunk->u1.AddressOfData);// and to IMAGE_IMPORT_BY_NAME
		while(*(WORD*)pFirstThunk!=0 && *(WORD*)pOriginalFirstThunk!=0) //moving over IAT and over names' table
		{
			if (!(pOriginalFirstThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG) && strcmp(targetFunction, (char*)pFuncData->Name)==0)//checks if we are in the Target Function
				rewriteThunk(pFirstThunk, newFunc, oldFunc);

			pOriginalFirstThunk++; // next node (function) in the array
			pFuncData=(PIMAGE_IMPORT_BY_NAME)((PBYTE)hInstance+ pOriginalFirstThunk->u1.AddressOfData);
			pFirstThunk++;// next node (function) in the array
		}
		importedModule++; //next module (DLL)
	}

	return false;
}

PIMAGE_IMPORT_DESCRIPTOR getImportTable(HMODULE hInstance)
{
	PIMAGE_DOS_HEADER dosHeader;
	IMAGE_OPTIONAL_HEADER optionalHeader;
	PIMAGE_NT_HEADERS ntHeader;
	IMAGE_DATA_DIRECTORY dataDirectory;

	dosHeader=(PIMAGE_DOS_HEADER)hInstance;//cast hInstance to (IMAGE_DOS_HEADER *) - the MZ Header
	ntHeader=(PIMAGE_NT_HEADERS)((PBYTE)dosHeader+dosHeader->e_lfanew);//The PE Header begin after the MZ Header (which has size of e_lfanew)
	optionalHeader=(IMAGE_OPTIONAL_HEADER)(ntHeader->OptionalHeader); //Getting OptionalHeader
	dataDirectory=(IMAGE_DATA_DIRECTORY)(optionalHeader.DataDirectory[IMPORT_TABLE_OFFSET]);//Getting the import table of DataDirectory
	return (PIMAGE_IMPORT_DESCRIPTOR)((PBYTE)hInstance + dataDirectory.VirtualAddress);//ImageBase+RVA to import table

}

bool rewriteThunk(PIMAGE_THUNK_DATA pThunk, DWORD newFunc, DWORD *oldFunc)
{
	DWORD CurrentProtect;
	DWORD junk;
	VirtualProtect(pThunk,4096, PAGE_READWRITE, &CurrentProtect);//allow write to the page
	if (oldFunc != NULL)
		*oldFunc = pThunk->u1.Function;
	pThunk->u1.Function = newFunc; // rewrite the IAT to new function
	VirtualProtect(pThunk,4096, CurrentProtect,&junk);//return previous premissions
	return true;
}