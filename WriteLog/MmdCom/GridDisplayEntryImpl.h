#pragma once
#include "wlmuldsp.h"

/* IMultDisplayEntry implementations
** These help with display of:
**    Grid squares:  CGridSquareDisplayHelper
*/

template <class GRIDENTRY, int ID, short BANDOFFSET = 0>
class CGridSquareDisplayHelper : public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>
    , public IMultDisplayEntry
{
protected:
    ~CGridSquareDisplayHelper() {}
public:
    CGridSquareDisplayHelper() : m_GridEntry(0){}

    BEGIN_COM_MAP(CGridSquareDisplayHelper)
        COM_INTERFACE_ENTRY(IMultDisplayEntry)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()
    HRESULT FinalConstruct()    { 
        return S_OK;
    }
    void FinalRelease()    {    }

    void Init(
        IMultDisplayPage2 *m,
        short bands,
        short bandColorOffset,
        GRIDENTRY *p,
        short columnCount = static_cast<short>(18 * 10),
        short gridcount = (short)(18 * 18 * 10 * 10)
        )
    {
        m_GridEntry = p; 
        m->put_MultCount(gridcount);
        m->put_ColumnCount(columnCount);
        m->put_BandCount(bands);
        m->put_BandColorOffset(bandColorOffset);
        m->put_LookupFcn(this);
    }

    // IMultDisplayEntry
    STDMETHODIMP get_MultTitle(short Mult, const char **Title)
    {    
        if (!m_GridEntry)
            return E_UNEXPECTED;
        return m_GridEntry->get_MultTitle(Mult, Title);    
    }

    STDMETHODIMP get_Column(short Mult, short *Column)
    {        return E_NOTIMPL;    }

    STDMETHODIMP get_ColumnTitle(short Column, const char **Title)
    {        return E_NOTIMPL;    }

    STDMETHODIMP get_MultWorked(short Mult, short band)
    {
        if (!m_GridEntry)
            return E_UNEXPECTED;
        return m_GridEntry->get_MultWorked(ID, Mult, band + BANDOFFSET);    
    }

protected:
    GRIDENTRY          *m_GridEntry;
};

