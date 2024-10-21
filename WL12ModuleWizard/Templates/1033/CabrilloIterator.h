#pragma once
#include <CabrilloIterator.h>

// [!output MM_CABRILLOITERATOR_CLASS_NAME]

class ATL_NO_VTABLE [!output MM_CABRILLOITERATOR_CLASS_NAME] : public CCabrilloIterator<[!output MM_CLASS_NAME]>
    , public IWlogCabrillo2
{
public:
    // ctor/dtor
    [!output MM_CABRILLOITERATOR_CLASS_NAME] () : m_Cabrillo2LineNumber(-1)
        , m_Cabrillo2QsoLineIdx(-1)
    {}
    ~[!output MM_CABRILLOITERATOR_CLASS_NAME]()
    {}

    BEGIN_COM_MAP([!output MM_CABRILLOITERATOR_CLASS_NAME])
        COM_INTERFACE_ENTRY(IWlogCabrillo2)
        COM_INTERFACE_ENTRY_CHAIN(CCabrilloIterator<[!output MM_CLASS_NAME]>)
    END_COM_MAP()


    // IWlogCabrillo2
[!if MULTIPLE_NAMED_IN_QSO_TX]
    HRESULT __stdcall FormatTxField(QsoPtr_t q, short Field, char* Buf) override
    {
        if (q->GlobalIndex != m_Cabrillo2QsoLineIdx)
            m_Cabrillo2Mine.clear();
        switch (Field)
        {
        case 0:	//TODO
            if (m_Cabrillo2Mine.size() < 2)
                wsprintf(Buf, "%-6s ", m_target->currentDupeSheet().key().c_str());
            else
            {
                char* Name = "";
                auto which = *m_Cabrillo2MineItor;
                if ((which >= 0) && (which < m_target->m_namedMults[[!output MM_CLASS_NAME]::REGION_NAME_FIXME1].m_NumNamed))
                    m_target->m_namedMults[[!output MM_CLASS_NAME]::REGION_NAME_FIXME1].m_pNamedMults->NameFromIndex(which, reinterpret_cast<unsigned char**>(&Name));
                wsprintf(Buf, "%-6s ", Name);
            }
            return S_OK;
        default:
            break;
        }
        return __super::FormatTxField(q, Field, Buf);
    }
[!endif]

    HRESULT __stdcall FormatRxField(QsoPtr_t q, short Field, char* Buf) override
    {
        if (q->GlobalIndex != m_Cabrillo2QsoLineIdx)
            m_Cabrillo2His.clear();
        switch (Field)
        {
        case 2:
            if (m_Cabrillo2His.size() < 2)
                wsprintf(Buf, "%-6s ", m_target->fRCVD(q).str());
            else
            {
                char* Name = "";
                auto which = *m_Cabrillo2HisItor;
                if ((which >= 0) && (which < m_target->m_namedMults[[!output MM_CLASS_NAME]::REGION_NAME_FIXME1].m_NumNamed))
                    m_target->m_namedMults[[!output MM_CLASS_NAME]::REGION_NAME_FIXME1].m_pNamedMults->NameFromIndex(which, reinterpret_cast<unsigned char**>(&Name));
                wsprintf(Buf, "%-6s ", Name);
            }
            return S_OK;
        default: break;
        }

        return __super::FormatRxField(q, Field, Buf);
    }

    HRESULT __stdcall LinesForQSO(QsoPtr_t q, short* pLines) override
    {
        m_Cabrillo2His = m_target->FindNamed([!output MM_CLASS_NAME]::REGION_NAME_FIXME1, m_target->fRCVD(q).str());
        m_Cabrillo2HisItor = m_Cabrillo2His.begin();
        m_Cabrillo2LineNumber = 0;
        m_Cabrillo2QsoLineIdx = q->GlobalIndex;
[!if !MULTIPLE_NAMED_IN_QSO_TX]
        * pLines = static_cast<short>(m_Cabrillo2His.size());
[!else]
[!if AM_ROVER]
        m_Cabrillo2Mine = m_target->FindNamed([!output MM_CLASS_NAME]::REGION_NAME_FIXME1, m_target->fMYQTH(q).str());
[!else]
        m_Cabrillo2Mine = m_target->FindNamed([!output MM_CLASS_NAME]::REGION_NAME_FIXME1, m_target->currentDupeSheet().key().c_str());
[!endif]
        m_Cabrillo2MineItor = m_Cabrillo2Mine.begin();
        * pLines = static_cast<short>(m_Cabrillo2Mine.size() * m_Cabrillo2His.size());
[!endif]
        return S_OK;
    }

    HRESULT __stdcall SetCurrentLineNumber(short LineNo) override
    {
        auto diff = LineNo - m_Cabrillo2LineNumber;
        m_Cabrillo2LineNumber = LineNo;
        if (diff > 0)
        {
            if (!m_Cabrillo2His.empty())
            {
                m_Cabrillo2HisItor++;
                if (m_Cabrillo2HisItor == m_Cabrillo2His.end())
                {
                    m_Cabrillo2HisItor = m_Cabrillo2His.begin();
[!if MULTIPLE_NAMED_IN_QSO_TX]
                    if (!m_Cabrillo2Mine.empty())
                    {
                        m_Cabrillo2MineItor++;
                        if (m_Cabrillo2MineItor == m_Cabrillo2Mine.end())
                            m_Cabrillo2MineItor = m_Cabrillo2Mine.begin();
                    }
[!endif]
                }
            }
[!if MULTIPLE_NAMED_IN_QSO_TX]
            else if (!m_Cabrillo2Mine.empty())
            {
                m_Cabrillo2MineItor++;
                if (m_Cabrillo2MineItor == m_Cabrillo2Mine.end())
                    m_Cabrillo2MineItor = m_Cabrillo2Mine.begin();
            }
[!endif]
        }
        return S_OK;
    }

    short                           m_Cabrillo2LineNumber;
[!if MULTIPLE_NAMED_IN_QSO_TX]
    [!output MM_CLASS_NAME]::Named_t                         m_Cabrillo2Mine;
    [!output MM_CLASS_NAME]::Named_t::const_iterator         m_Cabrillo2MineItor;
[!endif]
    [!output MM_CLASS_NAME]::Named_t                         m_Cabrillo2His;
    [!output MM_CLASS_NAME]::Named_t::const_iterator         m_Cabrillo2HisItor;
    unsigned long                   m_Cabrillo2QsoLineIdx;

};
