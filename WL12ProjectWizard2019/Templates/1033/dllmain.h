// dllmain.h : Declaration of module class.

class C[!output PROJECT_NAME]Module : public CAtlDllModuleT< C[!output PROJECT_NAME]Module >
{
public :
	DECLARE_LIBID(LIBID_[!output PROJECT_NAME]Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_[!output PROJECT_NAME_CAPS], "{[!output APPID_REGISTRY_FORMAT]}")
};

extern class C[!output PROJECT_NAME]Module _AtlModule;
