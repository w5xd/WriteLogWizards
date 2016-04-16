#ifndef MMDOLE_H
#define MMDOLE_H
#ifdef __cplusplus 
      
#ifndef WIN32      
#ifndef _VARIANT_H_
#include <variant.h>
#endif
#endif

class MmdPersistStorage : public IPersistStorage {

public:
	 MmdPersistStorage(IUnknown FAR *, CLSID);
	 virtual ~MmdPersistStorage();

	 STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
	 STDMETHOD_(ULONG, AddRef)(void);
	 STDMETHOD_(ULONG, Release)(void);

	 // *** IPersist methods ***

	 STDMETHOD(GetClassID) (LPCLSID lpClassID);

	 // *** IPersistStorage methods ***
	 STDMETHOD(IsDirty) (void);
	 STDMETHOD(InitNew) ( LPSTORAGE pStg);
	 STDMETHOD(Load) ( LPSTORAGE pStg);
	 STDMETHOD(Save) ( LPSTORAGE pStgSave, BOOL fSameAsLoad);
	 STDMETHOD(SaveCompleted) ( LPSTORAGE pStgNew);
	 STDMETHOD(HandsOffStorage) (void);

protected:
	 virtual HRESULT UserRead(IStream FAR *);
    virtual HRESULT UserWrite(IStream FAR *);

    HRESULT LoadCommon(LPSTORAGE ,  const OLECHAR *,short *,    int );
    HRESULT SaveCommon(LPSTORAGE ,  const OLECHAR *,short *,    int );
    IUnknown FAR *UnkOuter;
    unsigned long   RefCount;
    CLSID ClassId;
    };
#endif
#endif                      
