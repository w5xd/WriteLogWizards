// dllmain.cpp : Implementation of DllMain.

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "$safeprojectname$_i.h"
#include "dllmain.h"

C$safeprojectname$Module _AtlModule;
HINSTANCE g_hInstance;


// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	g_hInstance = hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved);
}
