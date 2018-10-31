/*
	##############################################
	### (C) vbCrLf (at GMail), 2011           ####
	###     http://www.MerkazHaKfar.co.cc/    ####
	###                                       ####
	###      Written for DigitalWhisper       ####
	###    http://www.digitalwhisper.co.il/   ####
	##############################################
*/

#include "dll_main.h"
#include "iathooker.h"
#include "hooks.h"

struct Hook
{
	char *name;
	PVOID address;
	DWORD* original;
};
Hook hooks[] = {
	{ "FindFirstFileExA",	(PVOID)&newFindFirstFileExA,	&originalFindFirstFileExA},
	{ "FindFirstFileExW",	(PVOID)&newFindFirstFileExW,	&originalFindFirstFileExW},
	{ "FindNextFileA",		(PVOID)&newFindNextFileA,		&originalFindNextFileA	},
	{ "FindNextFileW",		(PVOID)&newFindNextFileW,		&originalFindNextFileW	},
	{ NULL,					NULL,							NULL					}
};

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved)
{
	static HMODULE self = GetModuleHandleA(NULL),
				   shell32 = GetModuleHandleA("shell32");

	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
			{
				for(Hook *hook = hooks; hook->name != NULL; hook++)
				{
					HookIAT(self, hook->name, (DWORD)hook->address, hook->original);
					HookIAT(shell32, hook->name, (DWORD)hook->address, hook->original);
				}
			}
			break;

		case DLL_PROCESS_DETACH:
			for(Hook *hook = hooks; hook->name != NULL; hook++)
			{
				HookIAT(shell32, hook->name, (DWORD)hook->original, NULL);
				HookIAT(self, hook->name, (DWORD)hook->original, NULL);
			}
			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;
	}
	return true;
}