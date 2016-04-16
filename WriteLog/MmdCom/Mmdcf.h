#ifndef MMDCF_H
#define MMDCF_H

#ifndef PORT_H
#include "port.h"
#endif

#ifdef __cplusplus

typedef void  (FAR PASCAL * DestroyFunction)(void);
int FAR PASCAL SupportedClsid(REFCLSID rclsid);
IUnknown FAR *MmdCreateInstance(int, LPUNKNOWN, DestroyFunction);

class CWlMmdFactory : public IClassFactory
    {
    protected:
        ULONG           m_cRef;         //Reference count on class object
		int		ClsId;

    public:
		CWlMmdFactory(int);
        ~CWlMmdFactory(void);
		static void AdjustObjectCount(int);

        //IUnknown members
        STDMETHODIMP         QueryInterface(REFIID, LPVOID FAR *);
        STDMETHODIMP_(ULONG) AddRef(void);
        STDMETHODIMP_(ULONG) Release(void);

        //IClassFactory members
        STDMETHODIMP         CreateInstance(LPUNKNOWN, REFIID, LPVOID FAR *);
        STDMETHODIMP         LockServer(BOOL);
    };

#endif

#endif
