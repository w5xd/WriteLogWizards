#pragma once
/* helper subclasses of std::map that enable a lookup without adding entries */
class CMultiplierMap : public std::map<short, int>
{
public:
    bool worked(short m) const
    {        return numWorked(m) != 0;      }
    int numWorked(short m) const
    {
        const_iterator itor = find(m);
        if (itor == end())
            return 0;
        return itor->second;    
    }
};


class CMultiplierMapByBand : public std::map<short, CMultiplierMap>
{
public:
    bool worked(short band, short m) const
    {        return numWorked(band, m) != 0;    }
    int numWorked(short band, short m) const
    {
        const_iterator itor = find(band);
        if (itor == end())
            return 0;
        return itor->second.numWorked(m);    
    }
};

// newer wizard generates these ****************************************************
/* helper subclasses of std::map that enable a lookup without adding entries */
class CMultiplierMap2 
{
public:
    typedef std::map<short, int> MapMultToWorked_t;
    bool worked(short m, short b, short r) const
    {        return numWorked(m, b, r) != 0;    }
    int numWorked(short m, short, short) const
    {
        auto itor = m_worked.find(m);
        if (itor == m_worked.end())
            return 0;
        return itor->second;
    }
    void clear() { m_worked.clear(); }
    unsigned total() const
    {
        unsigned ret(0);
        for (auto n: m_worked)
            ret += n.second;
        return ret;
    }
    int mark(int nm, int band, short)
    {        return m_worked[nm]++;    }
    int unmark(int nm, int band, short)
    {        return --m_worked[nm];    }
    const MapMultToWorked_t & worked(short) const
    {   return m_worked; }
protected:
    std::map<short, int> m_worked;
};

class CMultiplierMapByRegion
{
public:
    typedef std::map<short, int> MapMultToWorked_t;
    bool worked(short m, short, short region ) const
    {        return numWorked(m, 0, region) != 0;    }
    int numWorked(short m, short, short region ) const
    {
        auto itor = m_worked.find(region);
        if (itor == m_worked.end())
            return 0;
        auto itor2 = itor->second.find(m);
        if (itor2 != itor->second.end())
            return itor2->second;
        return 0;
    }
    void clear() { m_worked.clear(); }
    unsigned total() const
    {
        unsigned ret(0);
        for (auto r : m_worked)
            for (auto n : r.second)
                ret += n.second;
        return ret;
    }
    int mark(int nm, int, short region )
    {        return m_worked[region][nm]++;    }
    int unmark(int nm, int, short region )
    {        return --m_worked[region][nm];    }
    const MapMultToWorked_t&worked(short region) const
    {
        auto itor = m_worked.find(region);
        if (itor != m_worked.end())
            return itor->second;
        return m_empty;
    }
protected:
    std::map<short, std::map<short, int>> m_worked;
    const MapMultToWorked_t m_empty;
};

class CMultiplierMapByBand2
{
public:
    bool worked(short m, short band, short region) const
    {        return numWorked(m, band, region) != 0;    }
    int numWorked(short m, short band, short region) const
    {
        auto itor = m_worked.find(band);
        if (itor == m_worked.end())
            return 0;
        return itor->second.numWorked(m, band, region);
    }
    void clear() { m_worked.clear(); }
    unsigned total() const
    {
        unsigned ret = 0;
        for (auto b: m_worked)
            ret += b.second.total();
        return ret;
    }
    int mark(int nm, int band, short r)
    {        return m_worked[band].mark(nm, band, r);    }
    int unmark(int nm, int band, short r)
    {        return m_worked[band].unmark(nm, band, r);    }
protected:
    std::map<short, CMultiplierMap2> m_worked;
};

class CMultiplierMapByRegionByBand
{
public:
    bool worked(short m, short band, short region) const
    {        return numWorked(m, band, region) != 0;    }
    int numWorked(short m, short band, short region) const
    {
        auto itor = m_worked.find(region);
        if (itor == m_worked.end())
            return 0;
        auto itor2 =  itor->second.find(band);
        if (itor2 != itor->second.end())
            return itor2->second.numWorked(m, band, region);
        return 0;
    }
    void clear() { m_worked.clear(); }
    unsigned total() const
    {
        unsigned ret = 0;
        for (auto r : m_worked)
            for (auto b : r.second)
                ret += b.second.total();
        return ret;
    }
    int mark(int nm, int band, short region)
    {        return m_worked[region][band].mark(nm, band, region);    }
    int unmark(int nm, int band, short region)
    {        return m_worked[region][band].unmark(nm, band, region);    }
protected:
    std::map<short, std::map<short, CMultiplierMap2>> m_worked;
};

