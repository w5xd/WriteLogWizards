#pragma once
class CMultiplierMap : public std::map<short, int>
{
    // don't add an entry on failure to find
public:
    bool worked(short m) const
    {
        const_iterator itor = find(m);
        if (itor == end())
            return false;
        return itor->second != 0;    
    }
};

class CMultiplierMapByBand : public std::map<short, CMultiplierMap>
{
public:
    bool worked(short band, short m) const
    {
        const_iterator itor = find(band);
        if (itor == end())
            return false;
        return itor->second.worked(m);    
    }
};
