#pragma once
template <class TARGET>
class ATL_NO_VTABLE CCabrilloIterator : public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>
    , public IWlogCabrillo
{
public:
    // ctor/dtor
    CCabrilloIterator()
    {    }
    ~CCabrilloIterator()
    {}

    void setTarget(TARGET* p) {
        m_target = p;
    }

    DECLARE_NOT_AGGREGATABLE(CCabrilloIterator)

    BEGIN_COM_MAP(CCabrilloIterator)
        COM_INTERFACE_ENTRY(IWlogCabrillo)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

protected:
    ATL::CComPtr<TARGET> m_target;

    // Inherited via IWlogCabrillo
    virtual HRESULT __stdcall ConfirmFieldsFilled(HWND w) override
    {
        return m_target->ConfirmFieldsFilled(w);
    }
    virtual HRESULT __stdcall SetCallsign(const char* Call) override
    {
        return m_target->SetCallsign(Call);
    }
    virtual HRESULT __stdcall GetHeaderLineCount(short* pLines) override
    {
        return m_target->GetHeaderLineCount(pLines);
    }
    virtual HRESULT __stdcall GetHeaderLine(short LineNumber, char* Buf) override
    {
        return m_target->GetHeaderLine(LineNumber, Buf);
    }
    virtual HRESULT __stdcall GetContestName(char* Buf) override
    {
        return m_target->GetContestName(Buf);
    }
    virtual HRESULT __stdcall GetClaimedScore(long* Score) override
    {
        return m_target->GetClaimedScore(Score);
    }
    virtual HRESULT __stdcall GetTxFieldCount(short* pCount) override
    {
        return m_target->GetTxFieldCount(pCount);
    }
    virtual HRESULT __stdcall FormatTxField(QsoPtr_t q, short Field, char* Buf) override
    {
        return m_target->FormatTxField(q, Field, Buf);
    }
    virtual HRESULT __stdcall GetRxFieldCount(short* pCount) override
    {
        return m_target->GetRxFieldCount(pCount);
    }
    virtual HRESULT __stdcall FormatRxField(QsoPtr_t q, short Field, char* Buf) override
    {
        return m_target->FormatRxField(q, Field, Buf);
    }

};
