#include <windows.h>
#include <ole2.h>
#include <oleauto.h>
#include <string>
int InitDisp(const char * ClientName,
            int Max,
                                IDispatch FAR *Dispatch,
                                const OLECHAR **MethodNames,
                                DISPID *Disps
)
{   // We learn *all* the member IDs up front. A more sophisticated
    // implementation might defer learning about the IDs for a given
     // method until the first time the method is invoked, thereby
    // amortizing the creation costs.
    //
#ifdef _DEBUG
    std::wstring errorMessage;
#endif
    int i;
    HRESULT hresult;
    for (i = 0; i < Max; ++i)
    {
        hresult = Dispatch->GetIDsOfNames(
            IID_NULL,
            (OLECHAR **)&MethodNames[i],
            1,
            LOCALE_SYSTEM_DEFAULT,
            &Disps[i]);
        if (hresult != NOERROR)
        {
            Disps[i] = -1;
#ifdef _DEBUG
            {
                errorMessage += L"Unrecognized member name: ";
                errorMessage += MethodNames[i];
                errorMessage += L"\r\n";
            }
#endif
        }
    }
#ifdef _DEBUG
    if (!errorMessage.empty())
    {
        wchar_t ClientNameW[512];
        ::MultiByteToWideChar(CP_ACP, 0, ClientName, -1,
            ClientNameW, 512);
        MessageBoxW(0, errorMessage.c_str(), ClientNameW, 0);
    }
#endif
    return 1;
}

