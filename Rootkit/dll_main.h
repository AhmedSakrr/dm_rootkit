/*
	##############################################
	### (C) vbCrLf (at GMail), 2011           ####
	###     http://www.MerkazHaKfar.co.cc/    ####
	###                                       ####
	###      Written for DigitalWhisper       ####
	###    http://www.digitalwhisper.co.il/   ####
	##############################################
*/

#ifndef _DLL_MAIN_H_
#define _DLL_MAIN_H_

#include <Windows.h>

#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else
# define DLLIMPORT __declspec (dllimport)
#endif

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved);

#endif /* _DLL_MAIN_H_ */