#include <windows.h>
#include <ole2.h>
#include "mmdcf.h"


//Count number of objects and number of locks.
static ULONG       g_cObj=0;
static ULONG       g_cLock=0;


/*
 * DllGetClassObject
 *
 * Purpose:
 *  Provides an IClassFactory for a given CLSID that this DLL is
 *  registered to support.  This DLL is placed under the CLSID
 *  in the registration database as the InProcServer.
 *
 * Parameters:
 *  clsID           REFCLSID that identifies the class factory desired.
 *                  Since this parameter is passed this DLL can handle
 *                  any number of objects simply by returning different
 *                  class factories here for different CLSIDs.
 *
 *  riid            REFIID specifying the interface the caller wants
 *                  on the class object, usually IID_ClassFactory.
 *
 *  ppv             LPVOID FAR * in which to return the interface pointer.
 *
 * Return Value:
 *  HRESULT         NOERROR on success, otherwise contains an error SCODE.
 */

HRESULT FAR PASCAL DllGetClassObject(REFCLSID rclsid, REFIID riid
    , LPVOID FAR *ppv)
	{
#ifdef _AFXDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
#endif
	int ClassIndex;

	ClassIndex = SupportedClsid(rclsid);
	if (ClassIndex < 0)
        return ResultFromScode(E_FAIL);

    //Check that we can provide the interface
    if (!IsEqualIID(riid, IID_IUnknown) && !IsEqualIID(riid, IID_IClassFactory))
        return ResultFromScode(E_NOINTERFACE);

    //Return our IClassFactory for Koala objects
	*ppv=(LPVOID)new CWlMmdFactory(ClassIndex);

    if (NULL==*ppv)
        return ResultFromScode(E_OUTOFMEMORY);

    //Don't forget to AddRef the object through any interface we return
    ((LPUNKNOWN)*ppv)->AddRef();

    return NOERROR;
    }

/*
 * DllCanUnloadNow
 *
 * Purpose:
 *  Answers if the DLL can be freed, that is, if there are no
 *  references to anything this DLL provides.
 *
 * Parameters:
 *  None
 *
 * Return Value:
 *  BOOL            TRUE if nothing is using us, FALSE otherwise.
 */

STDAPI DllCanUnloadNow(void)
    {
    SCODE   sc;

    //Our answer is whether there are any object or locks
    sc=(0L==g_cObj && 0==g_cLock) ? S_OK : S_FALSE;
    return ResultFromScode(sc);
    }


/*
 * ObjectDestroyed
 *
 * Purpose:
 *  Function for the Koala object to call when it gets destroyed.
 *  Since we're in a DLL we only track the number of objects here
 *  letting DllCanUnloadNow take care of the rest.
 *
 * Parameters:
 *  None
 *
 * Return Value:
 *  None
 */

static void FAR PASCAL ObjectDestroyed(void)
    {
    g_cObj--;
    return;
    }

void CWlMmdFactory::AdjustObjectCount(int i)
{
	g_cObj += i;
}

/*
 * CWlMmdFactory::CWlMmdFactory
 * CWlMmdFactory::~CWlMmdFactory
 *
 * Constructor Parameters:
 *  None
 */

CWlMmdFactory::CWlMmdFactory(int ClassId)
    {
	m_cRef=0L;
	ClsId = ClassId;
    return;
    }


CWlMmdFactory::~CWlMmdFactory(void)
    {
    return;
    }

/*
 * CWlMmdFactory::QueryInterface
 * CWlMmdFactory::AddRef
 * CWlMmdFactory::Release
 */

STDMETHODIMP CWlMmdFactory::QueryInterface(REFIID riid, LPVOID FAR *ppv)
    {
    *ppv=NULL;

    //Any interface on this object is the object pointer.
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory))
        *ppv=(LPVOID)this;

    /*
     * If we actually assign an interface to ppv we need to AddRef it
     * since we're returning a new pointer.
     */
    if (NULL!=*ppv)
        {
        ((LPUNKNOWN)*ppv)->AddRef();
        return NOERROR;
        }

    return ResultFromScode(E_NOINTERFACE);
    }


STDMETHODIMP_(ULONG) CWlMmdFactory::AddRef(void)
    {
    return ++m_cRef;
    }


STDMETHODIMP_(ULONG) CWlMmdFactory::Release(void)
    {
    ULONG           cRefT;

    cRefT=--m_cRef;

    if (0L==m_cRef)
        delete this;

    return cRefT;
    }


/*
 * CWlMmdFactory::CreateInstance
 *
 * Purpose:
 *  Instantiates a Koala object that supports the IPersist
 *  and IUnknown interfaces.  If the caller asks for a different
 *  interface than these two then we fail.
 *
 * Parameters:
 *  punkOuter       LPUNKNOWN to the controlling IUnknown if we are
 *                  being used in an aggregation.
 *  riid            REFIID identifying the interface the caller desires
 *                  to have for the new object.
 *  ppvObj          LPVOID FAR * in which to store the desired interface
 *                  pointer for the new object.
 *
 * Return Value:
 *  HRESULT         NOERROR if successful, otherwise contains E_NOINTERFACE
 *                  if we cannot support the requested interface.
 */

STDMETHODIMP CWlMmdFactory::CreateInstance(LPUNKNOWN punkOuter
    , REFIID riid, LPVOID FAR *ppvObj)
    {
    IUnknown FAR *      pObj;
    HRESULT             hr;
#ifdef _AFXDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
#endif

    *ppvObj=NULL;
    hr=ResultFromScode(E_OUTOFMEMORY);

    //Verify that if there is a controlling unknown it's asking for IUnknown
    if (NULL!=punkOuter && !IsEqualIID(riid, IID_IUnknown))
        return ResultFromScode(E_NOINTERFACE);

    //Create the object telling it a function to notify us when it's gone.
	pObj= MmdCreateInstance(ClsId, punkOuter, ObjectDestroyed);

    if (NULL==pObj)
        return hr;

	pObj->AddRef();
	g_cObj++;

	hr=pObj->QueryInterface(riid, ppvObj);
	//Kill the object if initial creation failed.
	pObj->Release();

	return hr;
    }

/*
 * CWlMmdFactory::LockServer
 *
 * Purpose:
 *  Increments or decrements the lock count of the DLL.  If the lock
 *  count goes to zero and there are no objects, the DLL is allowed
 *  to unload.  See DllCanUnloadNow.
 *
 * Parameters:
 *  fLock           BOOL specifying whether to increment or decrement the
 *                  lock count.
 *
 * Return Value:
 *  HRESULT         NOERROR always.
 */

STDMETHODIMP CWlMmdFactory::LockServer(BOOL fLock)
    {
    if (fLock)
        g_cLock++;
    else
        g_cLock--;

    return NOERROR;
    }
