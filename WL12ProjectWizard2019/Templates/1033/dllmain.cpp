// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "[!output PROJECT_NAME]_i.h"
#include "dllmain.h"

C[!output PROJECT_NAME]Module _AtlModule;

HINSTANCE g_hInstance;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	g_hInstance = hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
