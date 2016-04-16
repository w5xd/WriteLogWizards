#include <windows.h>
#include <ole2.h>

#if !defined(WIN32)
#include <olenls.h>
#include <dispatch.h>
#else
#include <oleauto.h>
#endif

int InitDisp(const char * ClientName,
            int Max,
                                IDispatch FAR *Dispatch,
                                const OLECHAR **MethodNames,
                                DISPID *Disps)
{   // We learn *all* the member IDs up front. A more sophisticated
    // implementation might defer learning about the IDs for a given
     // method until the first time the method is invoked, thereby
    // amortizing the creation costs.
    //
#ifdef _DEBUG
    char MsgBuf[100];
#endif
    int i;
    HRESULT hresult;
    for(i = 0; i < Max; ++i)
    {
            hresult = Dispatch->GetIDsOfNames(
                IID_NULL,
                (OLECHAR **)&MethodNames[i],
                1,
                LOCALE_SYSTEM_DEFAULT,
                &Disps[i]);
            if(hresult != NOERROR)
				{
					Disps[i] = -1;
#ifdef _DEBUG
                    char MethodNameMB[512];
                    ::WideCharToMultiByte(CP_ACP, 0, MethodNames[i], -1,
				        MethodNameMB, sizeof(MethodNameMB), 0, 0);
					wsprintf(MsgBuf, "Unrecognized member name: %s", MethodNameMB);
				// the following should probably be an assertion
					 MessageBox(NULL, MsgBuf, ClientName, MB_OK);
#endif
            }
    }
    return 1;
}

