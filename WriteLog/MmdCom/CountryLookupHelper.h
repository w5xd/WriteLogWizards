#pragma once
#include "dxcclist.h"
#include "dxpref.h"
#include "condefs.h"
#include "QsoFieldMgr.h"
#include "dxcchome.h"

class CCountryLookupHelper {
public:
    CCountryLookupHelper() : m_DxContext(0), m_MyCountryIndex(-1)
    {    }

    ~CCountryLookupHelper()
    {        Release();    }

    HRESULT Init(const std::string &dbName)
    {
        Release();
        m_DxContext = pref_init(dbName.c_str());
        return m_DxContext == 0 ? E_FAIL : S_OK;
    }

    void Release()
    {
        if (m_DxContext)
            pref_free(m_DxContext);
        m_DxContext = 0;
    }

    int CountryFromQsoPrefix(QsoPtr_t q,  CQsoField &fCPRF)
    {        return pref_cvt(m_DxContext, fCPRF(q));    }

    int FillQso(QsoPtr_t q,
         CQsoField &fCALL,
         CQsoField &fCPRF,  CQsoField &fCOUNTRY,  CQsoField &fAMBF) const
    {
        char            prefix[CALL_WID];  /*place to calc call sign prefix*/
        pref_calc(m_DxContext, prefix, fCALL(q));
        int i = -1;
        if (!fCPRF(q).empty())
            i = pref_cvt(m_DxContext, fCPRF(q));
        if (i < 0)
            i = pref_find(m_DxContext, prefix);
        if (i != -1 /*-1 means not in list*/)
        {
            int             dummy;
            if (i < 0)      /*it was ambiguous*/
                i = pref_reso(m_DxContext, i,
                    *fAMBF(q), &dummy); /*resolve ambiguous*/
            const struct country_stru &pCountry = cList()[i];
            fCOUNTRY(q) = pCountry.country;
            fCPRF(q) = pCountry.usual_prefix;/*leave our tag*/
        }
        return i;
    }

    int dxcc_Home()
    {        return ::dxcc_Home(m_DxContext);    }

    int CheckQso (QsoPtr_t q,
        bool canWriteIntoQso,
        CQsoField &fCALL,
        CQsoField &fCOUNTRY,
        CQsoField &fCPRF,
        CQsoField &fAMBF,
        bool &ambiguous) const
    {
        char  prefix[CALL_WID];  /*place to calc call sign prefix*/
        pref_calc(m_DxContext, prefix, fCALL(q));
        int ambig = 0;      /*assume not ambiguous*/
        int i = pref_find(m_DxContext, prefix);
        if (i < -1)      /*resolve ambiguous ?*/
        {
            char amb(*fAMBF(q));
            int j = pref_reso(m_DxContext, i, amb, &ambig);
            if (ambig && canWriteIntoQso)
            {   // still ambigous
                char buffer[64]; // build prompt
                pref_ambig(m_DxContext, i, buffer);
                fCOUNTRY(q) = buffer; // write prompt to COUNTRY column
            }
            else
                i = j;
        }
        if ((i >= 0) && canWriteIntoQso)
        {
            fCOUNTRY(q) = cList()[i].country;
            fCPRF(q) = "";
        }
        ambiguous = ambig != 0;
        return i;
    }
    const struct country_stru* cList() const 
    {  return pref_list(m_DxContext);    }

protected:
	DxContext_t						m_DxContext;
    int								m_MyCountryIndex;
};
