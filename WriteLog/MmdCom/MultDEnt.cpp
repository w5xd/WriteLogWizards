#include <ole2.h>
#include <string>
#include "dxpref.h"
#include "wlmuldsp.h"
#include "MultDEnt.h"

#define DIM(X) (sizeof(X)/sizeof(X[0]))

CDxccDispContainer::Continent_t
CDxccDispContainer::Continents[NUM_CONTINENTS+1] =
{
    {"AS", "Asia",},
    {"OC", "Oceania"},
    {"NA", "North America"},
    {"SA", "South America"},
    {"EU", "Europe"},
    {"AF", "Africa"},
	{"MM", "Maritime Mobile"},
};


//CMultDisplayEntryBase*********************************

CMultDisplayEntryBase::CMultDisplayEntryBase()
{
    m_RefCount = 1;
}

CMultDisplayEntryBase::~CMultDisplayEntryBase()
{
}

ULONG CMultDisplayEntryBase::AddRef()
{
    return ++m_RefCount;
}

ULONG CMultDisplayEntryBase::Release()
{
    --m_RefCount;
    if (m_RefCount)
        return m_RefCount;
    else
    {
        delete this;
        return 0;
    }
}

STDMETHODIMP CMultDisplayEntryBase::QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
{
    IUnknown FAR *Unk;
    Unk = 0;
    if (IsEqualIID(riid, IID_IUnknown) || 
        IsEqualIID(riid, __uuidof(IMultDisplayEntry)))
    {
        Unk = this;
    }

    *ppvObj = (LPVOID) Unk;
    if (Unk)
    {
        Unk->AddRef();
        return NOERROR;
    }
    else
        return ResultFromScode(E_NOINTERFACE);
}

    //IMultDisplayEntry
STDMETHODIMP CMultDisplayEntryBase::get_Column(short Mult, short *Column)
{
    *Column = -1;
    return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CMultDisplayEntryBase::get_ColumnTitle(short Column, const char **Title)
{
    *Title = 0;
    return ResultFromScode(E_NOTIMPL);
}

//CStateDisplayEntry

STDMETHODIMP CStateDisplayEntry::get_MultTitle(short Mult, const char **Title)
{
    *Title = m_States[Mult].Name;
    return NOERROR;
}

CStateDisplayEntry::CStateDisplayEntry(
            const struct StateDef_t *States, 
            int NumStates,
            IMultDisplayPage *m,
            int bands)
{
    m_States = States;
    m_NumStates = NumStates;
    m->put_MultCount(NumStates);
    m->put_BandCount(bands);
    m->put_LookupFcn(this);
}

/*CStateDisplayEntrySingleBand ***********************************/
CStateDisplaySingleBand::CStateDisplaySingleBand(
			const int *Mults,												 
            const struct StateDef_t *States, 
            int NumStates,
            IMultDisplayPage *m) : CStateDisplayEntry(States,
											NumStates,
											m,
											0)
{
	m_Mults = Mults;
}
													
STDMETHODIMP CStateDisplaySingleBand::get_MultWorked(short Mult, short band)
{
    return m_Mults[Mult] ? ResultFromScode(S_OK) : ResultFromScode(S_FALSE);
}


/*CNamedMultDisplayEntry **************************************/
CNamedMultDisplayEntry::CNamedMultDisplayEntry(
            IWlNamedMult *States, 
            IMultDisplayPage *m,
            int bands)
{
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

STDMETHODIMP CNamedMultDisplayEntry::get_MultTitle(short Mult, 
												   const char **Title)
{
	return m_NamedMult->get_MultTitle(Mult, (unsigned char **)Title);
}

STDMETHODIMP CNamedMultDisplayEntry::get_Column(short Mult, short *Column)
{
	return m_NamedMult->get_Column(Mult, Column);
}

STDMETHODIMP CNamedMultDisplayEntry::get_ColumnTitle(short Column, const char **Title)
{
	return m_NamedMult->get_ColumnTitle(Column, (unsigned char **)Title);
}

/*CNamedMultDisplaySingleBand******************************/
CNamedMultDisplaySingleBand::CNamedMultDisplaySingleBand(
			const int *Mults,												 
            IWlNamedMult *States, 
            IMultDisplayPage *m) : CNamedMultDisplayEntry(States,
											m,
											1)
{
	m_Mults = Mults;
}
													
STDMETHODIMP CNamedMultDisplaySingleBand::get_MultWorked(short Mult, short band)
{
    return m_Mults[Mult] ? ResultFromScode(S_OK) : ResultFromScode(S_FALSE);
}


/*CDxccDisplayEntry**********************************************/


STDMETHODIMP CDxccDisplayEntry::get_MultTitle(short Mult, const char **Title)
{
    *Title = m_cList[m_MyIndexes[Mult]].usual_prefix;
    return NOERROR;
}

CDxccDisplayEntry::CDxccDisplayEntry(
                        const struct country_stru *cList,
                        struct CountryMap_t *CountryMap,
                        int  ContinentIndex,
                        const char *Continent,
                        IMultDisplayPage *m,
                        int bands)
{
    int i;
    m_cList = cList;
    m_MyContinent = Continent;

    i = 0;
    for (;;)
    {
        if (!cList->usual_prefix)
            break;
        if (strcmp(cList->continent, Continent) == 0)
            i += 1;
         cList += 1;
   }

    m_MyIndexes = new int [i];
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

CDxccDisplayEntry::~CDxccDisplayEntry()
{
	if (m_MyIndexes)
		delete [] m_MyIndexes;
}

//CDxccDisplayEntrySingleBand*************************************************
CDxccDisplayEntrySingleBand::CDxccDisplayEntrySingleBand( 
                        const struct country_stru *cList,
                        struct CountryMap_t *CountryMap,
                        int  ContinentIndex,
                        const char *Continent,
                        IMultDisplayPage *m) :
							CDxccDisplayEntry(cList,
											CountryMap,
											ContinentIndex,
												Continent,
												m,
												0)
{
								m_Mults = 0;
}

STDMETHODIMP CDxccDisplayEntrySingleBand::get_MultWorked(short Mult, short band)
{
    return m_Mults[m_MyIndexes[Mult]] ? ResultFromScode(S_OK) : ResultFromScode(S_FALSE);
}


//CDxccDispContainer**************************************

CDxccDispContainer::CDxccDispContainer()
{
    int i;
    for (i = 0; i < DIM(m_DxccDisplay); i += 1)
    {
        m_DxccDisplay[i] = 0;
        m_DxccDispEntry[i] = 0;
    }
    for (i = 0; i < COUNTRY_MAX; i += 1)
    {
        m_CountryMap[i].Continent = -1;
        m_CountryMap[i].ContinentSubIndex = -1;
    }
}

CDxccDispContainer::~CDxccDispContainer()
{
    int i;
    for (i = 0; i < DIM(m_DxccDisplay); i += 1)
    {
        if (m_DxccDispEntry[i])
        {
            m_DxccDispEntry[i]->Release();
        }
        if (m_DxccDisplay[i])
            m_DxccDisplay[i]->Release();
    }

}

void CDxccDispContainer::MakeDxccDisplays(IMultDisplayContainer *pDispCon,
											int	MultiBand,
											int FixedColumns,
											int IncludeMM)
{
    int i;
		//go through MM continent under caller control
    for (i = 0; i < (NUM_CONTINENTS + IncludeMM); i += 1)
    {
        pDispCon->MakeDisplay(MultiBand, FixedColumns,
                        __uuidof(IMultDisplayPage),
                        (IUnknown **) &m_DxccDisplay[i]);
    }

}

void CDxccDispContainer::InitializeEntry(
                                         const struct country_stru *cList)
{
    InitializeEntry(0, cList);
}

void CDxccDispContainer::InitializeEntry(const char* prefix, const country_stru*cList)
{
    int i;
    for (i = 0; i < DIM(m_DxccDisplay); i += 1)	//DO go through MM continent
    {
        if (m_DxccDisplay[i])
        {
            if (!m_DxccDispEntry[i])
            {
                std::string title = Continents[i].FullName;
                if (prefix && *prefix)
                    title = prefix + title;
                m_DxccDisplay[i]->put_Title(title.c_str());
                m_DxccDispEntry[i] =
                    CreateEntry(cList, i);
            }
        }
    }
}

void CDxccDispContainer::InvalidateCountry(int i)
{
    int ContinentIndex;
    int ContinentSubIndex;
    ContinentIndex = m_CountryMap[i].Continent;
    ContinentSubIndex = m_CountryMap[i].ContinentSubIndex;
    if ((ContinentIndex >= 0) && (ContinentSubIndex >= 0))
        m_DxccDisplay[ContinentIndex]->Invalidate(ContinentSubIndex);
}

void CDxccDispContainer::InvalidateAll()
{
    for (unsigned i = 0; i < DIM(m_DxccDisplay); i++)
        if (m_DxccDisplay[i])
            m_DxccDisplay[i]->Invalidate(-1);
}

//CDxccDispSbContainer********************************************

IMultDisplayEntry *CDxccDispSbContainer::CreateEntry(
                                         const struct country_stru *cList,
										 int i)
{
	return new CDxccDisplayEntrySingleBand(
                                            cList,
                                            m_CountryMap,
                                            i,
                                            Continents[i].Abbr,
                                            m_DxccDisplay[i]);
}

void CDxccDispSbContainer::SetMults(const int *Mults)
{
    int i;
    for (i = 0; i < NUM_CONTINENTS; i += 1)
    {
        if (m_DxccDisplay[i])
        {
            if (m_DxccDispEntry[i])
            {
                ((CDxccDisplayEntrySingleBand *)m_DxccDispEntry[i])->SetMultPtr(Mults);
            }
        }
    }
}




