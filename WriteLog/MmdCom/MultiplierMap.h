#pragma once
/* helper subclasses of std::map that enable a lookup without adding
** entries */
class CMultiplierMap : public std::map<short, int>
{
public:
    bool worked(short m) const
    {
        return numWorked(m) != 0;    
    }
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
    {
        return numWorked(band, m) != 0;
    }
    int numWorked(short band, short m) const
    {
        const_iterator itor = find(band);
        if (itor == end())
            return 0;
        return itor->second.numWorked(m);    
    }
};
