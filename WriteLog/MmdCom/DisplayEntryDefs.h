#pragma once
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

