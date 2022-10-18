#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include "Player.h"


typedef union 
{
	struct 
	{
		BYTE byClass;
		BYTE byPart;
		WORD wPartID;
	};

	DWORD dwID;

} HERO_VARIATION_PART;

const int MAX_VARIATION_RESOURCE = 3;

struct BASE_HeroVariationInfo 
{
	HERO_VARIATION_PART partID;
    DWORD name_code;        //이름
    DWORD desc_code;        //설명
    DWORD resource_code;    //캐쉬냐? 기본이냐?
	DWORD    m_dResourceID[MAX_VARIATION_RESOURCE];
};


class HeroVariationInfoParser	: public Singleton<HeroVariationInfoParser> ,public IParser
{
public:
	HeroVariationInfoParser();
	~HeroVariationInfoParser();

	VOID            Init(DWORD dwPoolSize);

	VOID            Init(DWORD dwPoolSize , char *szPackfileName);
	VOID            Release();
    virtual	BOOL    LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	BASE_HeroVariationInfo* GetHeroVariationInfo(DWORD dwKey);
	int                     GetVariationCount(BYTE byClass,int iVariation);

    std::vector<DWORD>& GetList(WORD type, WORD parts, bool is_cash);

protected:
    void cash_variation_increase(WORD type, WORD parts);
    std::vector<DWORD> cash_data_[eCHAR_TYPE_MAX][PLAYER_VARIATION_MAX + 1];
    std::vector<DWORD> base_data_[eCHAR_TYPE_MAX][PLAYER_VARIATION_MAX + 1];
    int total_variation_count_[eCHAR_TYPE_MAX][PLAYER_VARIATION_MAX + 1];
	int m_iVariationCount[eCHAR_TYPE_MAX][PLAYER_VARIATION_MAX + 1];

private:
	VOID		   Unload();
	util::SolarHashTable<BASE_HeroVariationInfo*>*  m_pDataTable;
};


inline std::vector<DWORD>& HeroVariationInfoParser::GetList(WORD type, WORD parts, bool is_cash)
{
    if (is_cash)
    {
        return  cash_data_[type][parts];
    }
    return  base_data_[type][parts];
}
