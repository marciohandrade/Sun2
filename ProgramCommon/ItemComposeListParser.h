#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>


struct sITEM_COMPOSE_LIST
{
	enum { 
		MAX_COMPOSE_ITEM_NUM	= 6, 
		RESULTTYPE_REWARDCODE	= 0,
		RESULTTYPE_ITEMCODE		= 1,
		MAX_COMPOSE_MAP_NUM		= 10,
	};
	struct sITEM_FOR_COMPOSE {
		SLOTCODE	m_ItemCode;
		DWORD		m_ItemTypeIndex;
		BYTE		m_byItemNum;
	};
	SLOTCODE	m_Code;
	char		m_pszDebugName[MAX_ITEMNAME_LENGTH];
	CODETYPE	m_NCode;

	DWORD		m_LimitCharClass;
	DWORD		m_LimitGuildClass;
	DWORD		m_ItemType;				//< 조합되어져 나오는 아이템의 eITEMTYPE
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
    DWORD       m_dCategoryCount;
#endif

#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
    //최대트리 10
    DWORD       m_pCategory[10];
    CODETYPE    m_pCategoryCode[10];
#else
	DWORD		m_dwCategory1;
	CODETYPE	m_Category1NCode;
	DWORD		m_dwCategory2;
	CODETYPE	m_Category2NCode;
#endif

	// 재료물
	BYTE		m_byMatCount;								//< 파싱하면서 계산해둠
	sITEM_FOR_COMPOSE m_pMaterial[MAX_COMPOSE_ITEM_NUM];	//< 재료 / 개수
	MONEY		m_Money;

	// 결과물
	BYTE		m_byCreateRatio;							//< 성공 확률
	BYTE		m_byResultType;								//< enum 참조 RESULTTYPE_ITEMCODE or RESULTTYPE_REWARDCODE
	sITEM_FOR_COMPOSE m_Result;							//< 결과물 / 개수

	// 조합가능 맵
	MAPCODE		m_MapCode[MAX_COMPOSE_MAP_NUM];
};

#ifndef _SERVER //Client Only
#define MAKE_CATEGORY_ORDER(compose_code, main_category, sub_category) ((main_category)<<24 | (sub_category)<<16 | (compose_code))
typedef STLX_MAP<DWORD, sITEM_COMPOSE_LIST*> ComposeCategotyMap;
#endif //_SERVER

typedef util::SolarHashTable<sITEM_COMPOSE_LIST *> _COMPOSELIST_HASH;

class ItemComposeListParser : public util::Singleton<ItemComposeListParser>, public IParser
{
public:		ItemComposeListParser(void);
	virtual	~ItemComposeListParser(void);

public:		VOID					Init( DWORD dwComposeListSize );
			VOID					Init( DWORD dwComposeListSize, char * pszPackFileName );
			VOID					Release();

			VOID					Reload();

            sITEM_COMPOSE_LIST*		GetComposeListInfo( SLOTCODE Code ) { return m_pComposeListHash->GetData( Code ); }
            _COMPOSELIST_HASH*		GetHashTable() { return m_pComposeListHash; }

#ifndef _SERVER //Client Only
            ComposeCategotyMap& GetComposeCategotyMap() { return m_ComposeCategoryMap; }
#endif //_SERVER

public:
	virtual	BOOL					LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload );
private:	BOOL					_Load( BOOL bReload );
	VOID ValidCheck(sITEM_COMPOSE_LIST* pComposeInfo);

protected:
	VOID				Unload();

	_COMPOSELIST_HASH * m_pComposeListHash;
#ifndef _SERVER //Client Only
    ComposeCategotyMap m_ComposeCategoryMap;
#endif //_SERVER
    __DISABLE_COPY(ItemComposeListParser);
};
