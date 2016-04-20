#pragma once
#include "nmdmul.h"
#include "DisplayEntryDefs.h"
#include "MultDEnt.h"

template <class T, int ID>
class CStateDisplayHelper : public CComObjectRootEx<CComSingleThreadModel>
    , public IMultDisplayEntry
{
protected:
    ~CStateDisplayHelper() {}
public:
    CStateDisplayHelper() : m_States(0), m_NumStates(0){}

    BEGIN_COM_MAP(CStateDisplayHelper)
        COM_INTERFACE_ENTRY(IMultDisplayEntry)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()
    HRESULT FinalConstruct()    { 
        return S_OK;
    }
    void FinalRelease()    {    }

    void Init(
        const struct StateDef_t *States,
        int NumStates,
        IMultDisplayPage *m,
        int bands,
        T *p)
    {
        m_Module = p; 
        m_States = States;
        m_NumStates = NumStates;
        m->put_MultCount(NumStates);
        m->put_BandCount(bands);
        m->put_LookupFcn(this);
    }

    // IMultDisplayEntry
    STDMETHODIMP get_MultTitle(short Mult, const char **Title)
    {        
        *Title = m_States[Mult].Name;
        return S_OK;
    }

    STDMETHODIMP get_Column(short Mult, short *Column)
    {        return E_NOTIMPL;    }

    STDMETHODIMP get_ColumnTitle(short Column, const char **Title)
    {        return E_NOTIMPL;    }

    STDMETHODIMP get_MultWorked(short Mult, short band)
    {        return m_Module->get_MultWorked(ID, Mult, band);    }

protected:
	const struct StateDef_t *m_States;
    T *m_Module;
	int			m_NumStates;
};

template <class T, int ID = 0>
class CDxccDisplayHelper : public CComObjectRootEx<CComSingleThreadModel>
    , public IMultDisplayEntry
{
public:
    CDxccDisplayHelper() : m_Module(0) , m_MyContinent(0), m_MyIndexes(0),m_cList(0)
    {}
    ~CDxccDisplayHelper()
    {  delete [] m_MyIndexes; }

   BEGIN_COM_MAP(CDxccDisplayHelper)
        COM_INTERFACE_ENTRY(IMultDisplayEntry)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()
    HRESULT FinalConstruct()    
    {        return S_OK;    }

    void FinalRelease()    {    }

    void Init(T *p)
    {
        m_Module = p;
    }

    void Init(
        const struct country_stru *cList,
        struct CountryMap_t *CountryMap,
        int  ContinentIndex,
        const char *Continent,
        IMultDisplayPage *m,
        int bands = 0)
    {
        m_cList = cList;
        m_MyContinent = Continent;

        int i = 0;
        for (;;)
        {
            if (!cList->usual_prefix)
                break;
            if (strcmp(cList->continent, Continent) == 0)
                i += 1;
            cList += 1;
        }

        m_MyIndexes = new int[i];
        i = 0;
        int j = 0;
        cList = m_cList;
        for (;;)
        {
            if (!cList->usual_prefix)
                break;
            if (strcmp(cList->continent, Continent) == 0)
            {
                m_MyIndexes[i] = j;
                CountryMap[j].Continent = ContinentIndex;
                CountryMap[j].ContinentSubIndex = i;
                i += 1;
            }
            j += 1;
            cList += 1;
        }

        m->put_MultCount(i);
        m->put_BandCount(bands);
        m->put_LookupFcn(this);
    }

    STDMETHODIMP get_Column(short Mult, short *Column)
    {        return E_NOTIMPL;    }

    STDMETHODIMP get_ColumnTitle(short Column, const char **Title)
    {        return E_NOTIMPL;    }

    STDMETHODIMP get_MultTitle(short Mult, const char **Title)
    {
        *Title = m_cList[m_MyIndexes[Mult]].usual_prefix;
        return S_OK;
    }

    STDMETHODIMP get_MultWorked(short Mult, short band)
    {
        return m_Module->get_MultWorked(ID, m_MyIndexes[Mult], band); 
    }

protected:
    T * m_Module;
	const	char *m_MyContinent;
	int		*m_MyIndexes;
	const struct country_stru *m_cList;
};

template <class T, class U>
class CDxDispContainerHelper : public CDxccDispContainer
{
public:
    CDxDispContainerHelper() {}

    IMultDisplayEntry *CreateEntry(		const struct country_stru *cList,
										int i)
    {
        CComObject<U> *ret = 0;
        CComObject<U>::CreateInstance(&ret);
        ret->AddRef(); // CreateEntry returns refcount of 1
        ret->Init(cList,
                m_CountryMap,
                i,
                Continents[i].Abbr,
                m_DxccDisplay[i]);
        return ret;
    }

    void SetMults(T *p, int Bands = 0)
    {
        int i;
        for (i = 0; i < NUM_CONTINENTS; i += 1)
        {
            if (m_DxccDisplay[i])
            {
                if (m_DxccDispEntry[i])
                {
                    (dynamic_cast<U *>(m_DxccDispEntry[i]))->Init(p);
                    if (Bands > 0)
				        m_DxccDisplay[i]->put_BandCount(Bands);
                }
            }
        }
    }
};

template <class T, int ID>
class CNamedDisplayHelper : public CComObjectRootEx<CComSingleThreadModel>
    , public IMultDisplayEntry
{
protected:
    ~CNamedDisplayHelper() {}
public:
    CNamedDisplayHelper() : m_Module(0){}

    BEGIN_COM_MAP(CNamedDisplayHelper)
        COM_INTERFACE_ENTRY(IMultDisplayEntry)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()
    HRESULT FinalConstruct()    { 
        return S_OK;
    }
    void FinalRelease()    {    }

    void Init(
		IWlNamedMult *States, 
            IMultDisplayPage *m,
            int bands,
			T *p)
    {
	    m_Module = p;
	    short Count;
	    short ColCount;
        m_NamedMult = States;
	    States->get_ColumnCount(&ColCount);
	    m->put_ColumnCount(ColCount);
	    States->get_MultCount(&Count);
        m->put_MultCount(Count);
        m->put_BandCount(bands);
        m->put_LookupFcn(this);
    }

    // IMultDisplayEntry
    STDMETHODIMP get_MultTitle(short Mult, const char **Title)
    { return m_NamedMult->get_MultTitle(Mult, (unsigned char **)Title);    }

    STDMETHODIMP get_Column(short Mult, short *Column)
    { return m_NamedMult->get_Column(Mult, Column);    }

    STDMETHODIMP get_ColumnTitle(short Column, const char **Title)
    { return m_NamedMult->get_ColumnTitle(Column, (unsigned char **)Title);    }

    STDMETHODIMP get_MultWorked(short Mult, short band)
    {  return m_Module->get_MultWorked(ID, Mult, band);    }

protected:
    T *m_Module;
	IWlNamedMult *m_NamedMult;    
};

template <class T, int ID>
class CAygDisplayHelper : public CComObjectRootEx<CComSingleThreadModel>
    , public IMultDisplayEntry
{
protected:
    ~CAygDisplayHelper() {}
public:
    CAygDisplayHelper() : m_MultCount(0){}

    BEGIN_COM_MAP(CAygDisplayHelper)
        COM_INTERFACE_ENTRY(IMultDisplayEntry)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()
    HRESULT FinalConstruct()    {
        return S_OK;
    }
    void FinalRelease()    {    }

    void Init(T *mm,
					int bands,
						IMultDisplayPage *m)
    {
	    m_Page = m;
	    m_Module = mm;
	    m_MultCount = m_Module->AygCount();
        m->put_MultCount(m_MultCount);
        m->put_BandCount(bands);
        m->put_LookupFcn(this);
    }

    STDMETHODIMP get_MultWorked(short Mult, short band)
    {  return m_Module->get_MultWorked(ID, Mult, band);     }

    STDMETHODIMP get_MultTitle(short Mult, const char **Title)
    { return m_Module->get_MultTitle(ID, Mult, Title);    }

    STDMETHODIMP get_Column(short Mult, short *Column)
    {        return E_NOTIMPL;    }

    STDMETHODIMP get_ColumnTitle(short Column, const char **Title)
    {        return E_NOTIMPL;    }
    
    void ReleasePage()	//out-of-band for clearing circular reference
    {	    m_Page.Release();    }

    void Invalidate(int)
    {
	    if (m_Page)
	    {
		    if (m_MultCount < m_Module->AygCount())
		    {
			    m_MultCount = m_Module->AygCount();
			    m_Page->put_MultCount(m_MultCount);
		    }
		    m_Page->Invalidate(i);
	    }
    }

protected:
    CComPtr<IMultDisplayPage> m_Page;
    T *m_Module;
	int		m_MultCount;

};
