$if$ (0) /*Copyright (c) 2025 by Wayne E. Wright, W5XD
This template was converted from the version used for the old vsz template technology.
The old style used [!if ]. The new one uses the "if" that commands the processor to skip this very commentary.
The difference between the two is (a) that the old tech supported nested "if" clauses while the new one does not,
and (b) the old style supported && and || operators and the new one only supports string compare for "if" clauses.
The way I made this work was to extract all the compounded conditional expressions that in the old tech were
evaluated by the template processor, and make the wizard here evaluate the expressions and write a separate
for each expression.*/$endif$#pragma once
#include <CabrilloIterator.h>

// $MM_CABRILLOITERATOR_CLASS_NAME$

class ATL_NO_VTABLE $MM_CABRILLOITERATOR_CLASS_NAME$ : public CCabrilloIterator<$MM_CLASS_NAME$>
    , public IWlogCabrillo2
{
public:
    // ctor/dtor
    $MM_CABRILLOITERATOR_CLASS_NAME$ () : m_Cabrillo2LineNumber(-1)
        , m_Cabrillo2QsoLineIdx(-1)
    {}
    ~$MM_CABRILLOITERATOR_CLASS_NAME$()
    {}

    BEGIN_COM_MAP($MM_CABRILLOITERATOR_CLASS_NAME$)
        COM_INTERFACE_ENTRY(IWlogCabrillo2)
        COM_INTERFACE_ENTRY_CHAIN(CCabrilloIterator<$MM_CLASS_NAME$>)
    END_COM_MAP()


    // IWlogCabrillo2
$if$ ($MULTIPLE_NAMED_IN_QSO_TX$ == 1)    HRESULT __stdcall FormatTxField(QsoPtr_t q, short Field, char* Buf) override
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
                if ((which >= 0) && (which < m_target->m_namedMults[$MM_CLASS_NAME$::REGION_NAME_FIXME1].m_NumNamed))
                    m_target->m_namedMults[$MM_CLASS_NAME$::REGION_NAME_FIXME1].m_pNamedMults->NameFromIndex(which, reinterpret_cast<unsigned char**>(&Name));
                wsprintf(Buf, "%-6s ", Name);
            }
            return S_OK;
        default:
            break;
        }
        return __super::FormatTxField(q, Field, Buf);
    }
$endif$
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
                if ((which >= 0) && (which < m_target->m_namedMults[$MM_CLASS_NAME$::REGION_NAME_FIXME1].m_NumNamed))
                    m_target->m_namedMults[$MM_CLASS_NAME$::REGION_NAME_FIXME1].m_pNamedMults->NameFromIndex(which, reinterpret_cast<unsigned char**>(&Name));
                wsprintf(Buf, "%-6s ", Name);
            }
            return S_OK;
        default: break;
        }

        return __super::FormatRxField(q, Field, Buf);
    }

    HRESULT __stdcall LinesForQSO(QsoPtr_t q, short* pLines) override
    {
        m_Cabrillo2His = m_target->FindNamed($MM_CLASS_NAME$::REGION_NAME_FIXME1, m_target->fRCVD(q).str());
        m_Cabrillo2HisItor = m_Cabrillo2His.begin();
        m_Cabrillo2LineNumber = 0;
        m_Cabrillo2QsoLineIdx = q->GlobalIndex;
$if$ ($MULTIPLE_NAMED_IN_QSO_TX$ == 0)        * pLines = static_cast<short>(m_Cabrillo2His.size());
$endif$$if$ ($MULTIPLE_NAMED_IN_QSO_TX$ == 1)$endif$$if$ ($AM_ROVER$$MULTIPLE_NAMED_IN_QSO_TX$ == 11)        m_Cabrillo2Mine = m_target->FindNamed($MM_CLASS_NAME$::REGION_NAME_FIXME1, m_target->fMYQTH(q).str());
$endif$$if$ ($AM_ROVER$$MULTIPLE_NAMED_IN_QSO_TX$ == 01)        m_Cabrillo2Mine = m_target->FindNamed($MM_CLASS_NAME$::REGION_NAME_FIXME1, m_target->currentDupeSheet().key().c_str());
$endif$$if$ ($MULTIPLE_NAMED_IN_QSO_TX$ == 1)        m_Cabrillo2MineItor = m_Cabrillo2Mine.begin();
        * pLines = static_cast<short>(m_Cabrillo2Mine.size() * m_Cabrillo2His.size());
$endif$        return S_OK;
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
$if$ ($MULTIPLE_NAMED_IN_QSO_TX$ == 1)                    if (!m_Cabrillo2Mine.empty())
                    {
                        m_Cabrillo2MineItor++;
                        if (m_Cabrillo2MineItor == m_Cabrillo2Mine.end())
                            m_Cabrillo2MineItor = m_Cabrillo2Mine.begin();
                    }
$endif$                }
            }
$if$ ($MULTIPLE_NAMED_IN_QSO_TX$ == 1)            else if (!m_Cabrillo2Mine.empty())
            {
                m_Cabrillo2MineItor++;
                if (m_Cabrillo2MineItor == m_Cabrillo2Mine.end())
                    m_Cabrillo2MineItor = m_Cabrillo2Mine.begin();
            }
$endif$        }
        return S_OK;
    }

    short                           m_Cabrillo2LineNumber;
$if$ ($MULTIPLE_NAMED_IN_QSO_TX$ == 1)    $MM_CLASS_NAME$::Named_t                         m_Cabrillo2Mine;
    $MM_CLASS_NAME$::Named_t::const_iterator         m_Cabrillo2MineItor;
$endif$    $MM_CLASS_NAME$::Named_t                         m_Cabrillo2His;
    $MM_CLASS_NAME$::Named_t::const_iterator         m_Cabrillo2HisItor;
    unsigned long                   m_Cabrillo2QsoLineIdx;

};
