#pragma once
#include <iwritelg.h>
#include <atlcomcli.h>
#include <functional>
class TabToCntyHelper {
/* Helper class to use in state QSO parties to deal with rapid-fire receipt of county information
** This is triggered if, immediately after logging one CNTY received, the user types TAB (or SPACE)
** If this class detects that situation, it commands WriteLog to set its cursor to the CNTY column, but empty */
    public:
        TabToCntyHelper(unsigned long n, IWriteLog* Parent) : n(n), Parent(Parent)
        {}
        TabToCntyHelper(IWriteLog* Parent) : Parent(Parent), n(0)
        {            if (Parent) Parent->NumberQsos(&n);        }
        typedef std::function<void(QsoPtr_t)> OnUpdateQsoFcn;
#pragma warning( push )
#pragma warning( disable : 4996 )
        void tab(QsoPtr_t q, short cntyField, const OnUpdateQsoFcn &f = OnUpdateQsoFcn())
        {
            static const unsigned GET_CURRENT_ENTRY_ID = 10;
            QsoPtr_t prevQ(0);
            if (n > 0 && SUCCEEDED(Parent->QsoIth(n-1, &prevQ)) && prevQ)
            {   // sanity check. band must match band
                if (prevQ->band == q->band)
                {
                    CComQIPtr<IDispatch> wldisp(Parent);
                    if (wldisp)
                    {
                        CComVariant varResult;
                        DISPPARAMS dp({});
                        auto hresult = wldisp->Invoke(GET_CURRENT_ENTRY_ID,
                            IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD,
                            &dp, &varResult, NULL, NULL);
                        if (SUCCEEDED(hresult))
                        {   // retrieved the CURRENT_ENTRY_WINDOW
                            CComPtr<IDispatch> entry = varResult.pdispVal;
                            if (entry)
                            {
                                static const unsigned CALLSIGN_ID = 2;
                                static const unsigned CURRENT_FIELD_NUMBER_ID = 3;

                                CComVariant callsign;
                                DISPPARAMS noargs({});
                                auto hr = entry->Invoke(CALLSIGN_ID, IID_NULL, LOCALE_SYSTEM_DEFAULT,
                                    DISPATCH_PROPERTYGET, 
                                    &noargs, &callsign, NULL, NULL);
                                if (SUCCEEDED(hr))
                                {   // only overwrite blank CALL
                                    if ((callsign.vt == VT_BSTR) &&
                                        ::SysStringLen(callsign.bstrVal) == 0)
                                        {
                                            DISPID Put = DISPID_PROPERTYPUT;
                                            DISPPARAMS pdispparam({});
                                            CComVariant arg(cntyField);
                                            pdispparam.rgvarg= &arg;
                                            pdispparam.cArgs = 1;
                                            pdispparam.rgdispidNamedArgs = &Put;
                                            pdispparam.cNamedArgs = 1;
                                            hr = entry->Invoke(CURRENT_FIELD_NUMBER_ID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT,
                                                &pdispparam, 0, NULL, NULL);
                                            if (SUCCEEDED(hr))
                                            {
                                                strncpy(q->stn, prevQ->stn, sizeof(q->stn));
                                                if (f)  // caller wants to overwrite other entries
                                                    f(prevQ);
                                            }
                                        }
                                }
                            }
                        }
                    }
                }
            }
        }
#pragma warning( pop ) 
protected:
        CComPtr<IWriteLog> Parent;
        unsigned long n;
};
