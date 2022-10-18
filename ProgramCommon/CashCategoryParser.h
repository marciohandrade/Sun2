#ifndef SUN_GAMESERVER_CASHCATEGORYPARSER_H_
#define SUN_GAMESERVER_CASHCATEGORYPARSER_H_


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <SCItemSlot.h>
#include <ScriptCode.IParser.h>


class CashCategoryParser : public util::Singleton<CashCategoryParser>, public IParser
{
public:
    struct CategoryInfo
    {
        uint item_code;
        uint category_index;
    };

    CashCategoryParser(void);
	virtual	~CashCategoryParser(void);

    VOID Init();
	VOID Init( char * pszPackFileName );
	VOID Release();

	virtual	BOOL LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload );

    int get_category_index(uint item_code);

private:
    BOOL _Load( BOOL bReload );
    typedef STLX_HASH_MAP<uint, int> CategoryHashMap;
    CategoryHashMap category_hashmap_;
};

inline int CashCategoryParser::get_category_index(uint item_code)
{
    CategoryHashMap::iterator iter = category_hashmap_.find(item_code);
    if (iter != category_hashmap_.end())
        return iter->second;
    return 17;//(SHOP_CATEGORY_INVALID);
}


#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

#endif //SUN_GAMESERVER_CASHCATEGORYPARSER_H_
