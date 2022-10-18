#pragma once


#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <SCItemSlot.h>
#include <ScriptCode.IParser.h>

// [10/20/2009 namka199]
// Crystallize Code List Selector 추가
// sITEM_CRYSTALIZE_LIST 구조체 수정
#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
struct sITEM_CRYSTALIZE_LIST
{
    enum 
    { 
        MAX_MATERIALS = 4    // 최대 결과물 수, 스크립트에 맞춘다.
    };

    // 결정체 생성 정보.
    struct sITEM_FOR_CRYSTALIZE 
    {
        SLOTCODE	m_MaterialCode;     // 결정체 코드
        BYTE		m_MaterialCount;    // 결정체 생성 갯수
        BYTE        m_ResultRatio;      // 결정체 생성 확률
        MONEY       m_MaterialMoney;    // 결정체 1개 생성시 금액
    };

    // 결정화 코드
    SLOTCODE	m_Code;
    char		m_pszDebugName[MAX_ITEMNAME_LENGTH];

    // 소스 아이템 정보
    WORD		m_ItemType;     // 아이템의 종류( 검,창,헬멧,장갑 ...)
    WORD        item_subtype;   // 아이템의 등급( 일반,레어,엘리트...)
    LEVELTYPE	m_ItemLevel;
    LEVELTYPE	m_EnchantLv;

    sITEM_FOR_CRYSTALIZE        m_pResult[sITEM_CRYSTALIZE_LIST::MAX_MATERIALS];
};

class ItemCrystalizeListParser : public util::Singleton<ItemCrystalizeListParser>, public IParser
{
    // [10/20/2009 namka199]
    // 특정 Code를 찾기위한 동일 타입의 리스트.
    // 몇몇군데에서 비슷한 방식으로 사용하는데, 
    // 차후 일관된 형태의 클래스를 만들어서 적용 하도록 하자.
    struct sCRYSTALLIZE_SELECTOR
    {
        struct sVALUE
        {
            SLOTCODE    m_Code;
            LEVELTYPE   m_ItemLevel;
            LEVELTYPE   m_EnchantLevel;
            WORD        item_subtype;

            sVALUE(SLOTCODE code, LEVELTYPE itemlevel, LEVELTYPE enchantlevel, WORD subtype) :
                m_Code(code), m_ItemLevel(itemlevel), m_EnchantLevel(enchantlevel), item_subtype(subtype) {}
            sVALUE() {}
        };

        WORD    m_Count;
        STLX_VECTOR<sVALUE> values;

        sCRYSTALLIZE_SELECTOR() {}
        ~sCRYSTALLIZE_SELECTOR() {
            values.clear();
        }
    };


    typedef stdext::hash_map<WORD, sCRYSTALLIZE_SELECTOR *>    CRYSTALLIZE_SELECTOR;
    typedef util::SolarHashTable<sITEM_CRYSTALIZE_LIST *> _CRYSTALIZELIST_HASH;

public:
    ItemCrystalizeListParser(void);
    virtual	~ItemCrystalizeListParser(void);

public:
    VOID					Init( DWORD dwCrystalizeListSize );
    VOID					Init( DWORD dwCrystalizeListSize, char * pszPackFileName );
    VOID					Release();
    VOID					Reload();

public:
    sITEM_CRYSTALIZE_LIST*	GetCrystalizeListInfo( SLOTCODE Code ) { return m_pCrystalizeListHash->GetData( Code ); }
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    SLOTCODE				GetCrystalizeCode( SCItemSlot & rItemSlot, BYTE enchant_level );
#else
    SLOTCODE				GetCrystalizeCode( SCItemSlot & rItemSlot );
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

public:
    virtual	BOOL			LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload );

private:
    BOOL					_Load( BOOL bReload );
    VOID                    AddCrystallizeSelector( WORD wItemType, sITEM_CRYSTALIZE_LIST* pInfo );     // [10/20/2009 namka199] Crystallize Code List Selector 추가
    VOID                    RemoveCrystallizeSelector();                                                // :

protected:	
    VOID					Unload();

private:
    _CRYSTALIZELIST_HASH *	m_pCrystalizeListHash;
    char					m_pszFileName[MAX_PATH];
    CRYSTALLIZE_SELECTOR    m_CrystallizeSelector;
};

#else
struct sITEM_CRYSTALIZE_LIST
{
    enum 
    { 
        MAX_RESULT = 1, 
        MAX_RANDOMRESULT = 1 
    };

    enum
    {
        NORMAL_ITEM_TYPE,
        LUCKY_ITEM_TYPE,
        ELITE_1ST_ITEM_TYPE,
        ELITE_1ST_HALF_ITEM_TYPE,
        ELITE_2ND_ITEM_TYPE,
        ELITE_2ND_HALF_ITEM_TYPE = 5,
        ELITE_3RD_ITEM_TYPE = 6,
        ELITE_3RD_HALF_ITEM_TYPE = 7,
		ELITE_4RD_ITEM_TYPE = 8, 
    };

    enum
    {
        ELITE_1ST_ITEM_LEVEL = ItemCustomLevel::kElite1_0stLV,
        ELITE_1ST_HALF_ITEM_LEVEL = ItemCustomLevel::kElite1_5stLV,
        ELITE_2ND_ITEM_LEVEL = ItemCustomLevel::kElite2_0stLV,
        ELITE_2ND_HALF_ITEM_LEVEL = ItemCustomLevel::kElite2_5stLV,
        ELITE_3RD_ITEM_LEVEL      = ItemCustomLevel::kElite3_0stLV,
        ELITE_3RD_HALF_ITEM_LEVEL = ItemCustomLevel::kElite3_5stLV,
		ELITE_4RD_ITEM_LEVEL      = ItemCustomLevel::kElite4_0stLV, 
    };

    // 결정체 생성 정보.
    struct sITEM_FOR_CRYSTALIZE 
    {
        SLOTCODE	m_MaterialCode;     // 결정체 코드
        BYTE		m_MaterialCount;    // 결정체 생성 갯수
        BYTE        m_ResultRatio;      // 결정체 생성 확률
        MONEY       m_MaterialMoney;    // 결정체 1개 생성시 금액
    };

    // 랜덤 결정체 생성 정보
    struct sITEM_FOR_RANDOM_CRYSTALIZE 
    {
        SLOTCODE	m_MaterialCode;         // 결정체 코드
        BYTE		m_MaterialCount;        // 결정체 생성 갯수
        DWORD       m_RandomRatio;          // 결정체 생성 확률
        DWORD       m_RandomStandardRatio;  // 생성 확률 최대점
    };

    // 결정화 코드
    SLOTCODE	m_Code;
    char		m_pszDebugName[MAX_ITEMNAME_LENGTH];
    CODETYPE	m_NCode;

    // 소스 아이템 정보
    WORD		m_ItemType;
    WORD		m_LuckyItemType;
    LEVELTYPE	m_ItemLevel;
    LEVELTYPE	m_EnchantLv;

    sITEM_FOR_CRYSTALIZE        m_pResult[sITEM_CRYSTALIZE_LIST::MAX_RESULT];
    sITEM_FOR_RANDOM_CRYSTALIZE m_pRandomResult[sITEM_CRYSTALIZE_LIST::MAX_RANDOMRESULT];
};

class ItemCrystalizeListParser : public util::Singleton<ItemCrystalizeListParser>, public IParser
{
    // [10/20/2009 namka199]
    // 특정 Code를 찾기위한 동일 타입의 리스트.
    // 몇몇군데에서 비슷한 방식으로 사용하는데, 
    // 차후 일관된 형태의 클래스를 만들어서 적용 하도록 하자.
    struct sCRYSTALLIZE_SELECTOR
    {
        enum { MAX_COUNT = 100 };
        struct sVALUE
        {
            SLOTCODE    m_Code;
            LEVELTYPE   m_ItemLevel;
            LEVELTYPE   m_EnchantLevel;
            WORD        m_LuckyItemType;
        };

        WORD    m_Count;
        sVALUE  m_Value[sCRYSTALLIZE_SELECTOR::MAX_COUNT];
    };


    typedef stdext::hash_map<WORD, sCRYSTALLIZE_SELECTOR *>    CRYSTALLIZE_SELECTOR;
    typedef util::SolarHashTable<sITEM_CRYSTALIZE_LIST *> _CRYSTALIZELIST_HASH;

public:
    ItemCrystalizeListParser(void);
    virtual	~ItemCrystalizeListParser(void);

public:
    VOID					Init( DWORD dwCrystalizeListSize );
    VOID					Init( DWORD dwCrystalizeListSize, char * pszPackFileName );
    VOID					Release();
    VOID					Reload();

public:
    sITEM_CRYSTALIZE_LIST*	GetCrystalizeListInfo( SLOTCODE Code ) { return m_pCrystalizeListHash->GetData( Code ); }
    SLOTCODE				GetCrystalizeCode( SCItemSlot & rItemSlot );

public:
    virtual	BOOL			LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload );

private:
    BOOL					_Load( BOOL bReload );
    VOID                    AddCrystallizeSelector( WORD wItemType, sITEM_CRYSTALIZE_LIST* pInfo );     // [10/20/2009 namka199] Crystallize Code List Selector 추가
    VOID                    RemoveCrystallizeSelector();                                                // :

protected:	
    VOID					Unload();

private:
    _CRYSTALIZELIST_HASH *	m_pCrystalizeListHash;
    char					m_pszFileName[MAX_PATH];
    CRYSTALLIZE_SELECTOR    m_CrystallizeSelector;
};

#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE