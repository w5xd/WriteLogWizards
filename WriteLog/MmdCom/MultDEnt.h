#ifndef MULTDENT_H
#define MULTDENT_H

#ifndef __NmdMul_h__
#include "nmdmul.h"
#endif

#define NUM_CONTINENTS 6
#define COUNTRY_MAX 400


struct  StateDef_t
    {
        const char *Name;
        int        CallArea;
    };

struct CountryMap_t
{
	int	Continent;
	int	ContinentSubIndex;
};


class CMultDisplayEntryBase : public IMultDisplayEntry
{
public:
    //IUnknown:
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release() ;
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID FAR* ppvObj);

    //IMultDisplayEntry
    STDMETHODIMP get_Column(short Mult, short *Column);
    STDMETHODIMP get_ColumnTitle(short Column, const char **Title);

    CMultDisplayEntryBase();


protected:
	virtual ~CMultDisplayEntryBase();
    unsigned long   m_RefCount;
};

class CStateDisplayEntry : public CMultDisplayEntryBase
{
public:
    STDMETHODIMP get_MultTitle(short Mult, const char **Title);

    CStateDisplayEntry(
			const struct StateDef_t *, 
			int,
			IMultDisplayPage *,
			int);
protected:
	const struct StateDef_t *m_States;
	int			m_NumStates;
    
};

class CStateDisplaySingleBand : public CStateDisplayEntry
{
public:
    CStateDisplaySingleBand(const int *, 
			const struct StateDef_t *, 
			int,
			IMultDisplayPage *);
    STDMETHODIMP get_MultWorked(short Mult, short band);

protected:
	const int *m_Mults;

};


class CNamedMultDisplayEntry : public CMultDisplayEntryBase
{
public:
    STDMETHODIMP get_MultTitle(short Mult, const char **Title);
    //IMultDisplayEntry
    STDMETHODIMP get_Column(short Mult, short *Column);
    STDMETHODIMP get_ColumnTitle(short Column, const char **Title);

    CNamedMultDisplayEntry(
			IWlNamedMult *,
			IMultDisplayPage *,
			int);
protected:
	IWlNamedMult *m_NamedMult;    
};

class CNamedMultDisplaySingleBand : public CNamedMultDisplayEntry
{
public:
    CNamedMultDisplaySingleBand(const int *, 
			IWlNamedMult *,
			IMultDisplayPage *);
    STDMETHODIMP get_MultWorked(short Mult, short band);

protected:
	const int *m_Mults;

};


class CDxccDisplayEntry : public CMultDisplayEntryBase
{
public:
    STDMETHODIMP get_MultTitle(short Mult, const char **Title);
    CDxccDisplayEntry(	const struct country_stru *,
						struct CountryMap_t *,
						int		ContinentIndex,
                        const char *,
						IMultDisplayPage *,
						int);
protected:
	~CDxccDisplayEntry();
	const	char *m_MyContinent;
	int		*m_MyIndexes;
	const struct country_stru *m_cList;
};

class CDxccDisplayEntrySingleBand : public CDxccDisplayEntry
{
public:
	CDxccDisplayEntrySingleBand(
						const struct country_stru *,
						struct CountryMap_t *,
						int		ContinentIndex,
                        const char *,
						IMultDisplayPage *);
    STDMETHODIMP get_MultWorked(short Mult, short band);

	void SetMultPtr(const int *m){m_Mults = m;}
protected:
	const int *m_Mults;
};

class CDxccDispContainer
{
public:
	CDxccDispContainer();
	virtual ~CDxccDispContainer();
	void InvalidateCountry(int);
	virtual void MakeDxccDisplays(IMultDisplayContainer *, int, int, int = 0);
	void InitializeEntry(const country_stru *);
	static struct Continent_t
	{
			const char *Abbr;
			const char *FullName;
	} Continents[NUM_CONTINENTS + 1];

protected:
	IMultDisplayEntry   *(m_DxccDispEntry[NUM_CONTINENTS+1]);
	IMultDisplayPage        *(m_DxccDisplay[NUM_CONTINENTS+1]);
	struct CountryMap_t	m_CountryMap[COUNTRY_MAX];
	virtual IMultDisplayEntry *CreateEntry(const struct country_stru *cList,
										 int) = 0;
};

class CDxccDispSbContainer : public CDxccDispContainer
{
public:
	IMultDisplayEntry *CreateEntry(const struct country_stru *cList,
										 int);
	void SetMults(const int *);
};


#endif                   
