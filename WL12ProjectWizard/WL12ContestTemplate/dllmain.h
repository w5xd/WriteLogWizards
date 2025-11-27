// dllmain.h : Declaration of module class.

class C$safeprojectname$Module : public ATL::CAtlDllModuleT< C$safeprojectname$Module >
{
public :
	DECLARE_LIBID(LIBID_$safeprojectname$Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_$safeprojectname$, "{$guid1$}")
};

extern class C$safeprojectname$Module _AtlModule;
