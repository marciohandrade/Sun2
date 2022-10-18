#pragma once


#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <SCItemSlot.h>
#include <ScriptCode.IParser.h>

// [10/20/2009 namka199]
// Crystallize Code List Selector �߰�
// sITEM_CRYSTALIZE_LIST ����ü ����
#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
struct sITEM_CRYSTALIZE_LIST
{
    enum 
    { 
        MAX_MATERIALS = 4    // �ִ� ����� ��, ��ũ��Ʈ�� �����.
    };

    // ����ü ���� ����.
    struct sITEM_FOR_CRYSTALIZE 
    {
        SLOTCODE	m_MaterialCode;     // ����ü �ڵ�
        BYTE		m_MaterialCount;    // ����ü ���� ����
        BYTE        m_ResultRatio;      // ����ü ���� Ȯ��
        MONEY       m_MaterialMoney;    // ����ü 1�� ������ �ݾ�
    };

    // ����ȭ �ڵ�
    SLOTCODE	m_Code;
    char		m_pszDebugName[MAX_ITEMNAME_LENGTH];

    // �ҽ� ������ ����
    WORD		m_ItemType;     // �������� ����( ��,â,���,�尩 ...)
    WORD        item_subtype;   // �������� ���( �Ϲ�,����,����Ʈ...)
    LEVELTYPE	m_ItemLevel;
    LEVELTYPE	m_EnchantLv;

    sITEM_FOR_CRYSTALIZE        m_pResult[sITEM_CRYSTALIZE_LIST::MAX_MATERIALS];
};

class ItemCrystalizeListParser : public util::Singleton<ItemCrystalizeListParser>, public IParser
{
    // [10/20/2009 namka199]
    // Ư�� Code�� ã������ ���� Ÿ���� ����Ʈ.
    // �������� ����� ������� ����ϴµ�, 
    // ���� �ϰ��� ������ Ŭ������ ���� ���� �ϵ��� ����.
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
    VOID                    AddCrystallizeSelector( WORD wItemType, sITEM_CRYSTALIZE_LIST* pInfo );     // [10/20/2009 namka199] Crystallize Code List Selector �߰�
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

    // ����ü ���� ����.
    struct sITEM_FOR_CRYSTALIZE 
    {
        SLOTCODE	m_MaterialCode;     // ����ü �ڵ�
        BYTE		m_MaterialCount;    // ����ü ���� ����
        BYTE        m_ResultRatio;      // ����ü ���� Ȯ��
        MONEY       m_MaterialMoney;    // ����ü 1�� ������ �ݾ�
    };

    // ���� ����ü ���� ����
    struct sITEM_FOR_RANDOM_CRYSTALIZE 
    {
        SLOTCODE	m_MaterialCode;         // ����ü �ڵ�
        BYTE		m_MaterialCount;        // ����ü ���� ����
        DWORD       m_RandomRatio;          // ����ü ���� Ȯ��
        DWORD       m_RandomStandardRatio;  // ���� Ȯ�� �ִ���
    };

    // ����ȭ �ڵ�
    SLOTCODE	m_Code;
    char		m_pszDebugName[MAX_ITEMNAME_LENGTH];
    CODETYPE	m_NCode;

    // �ҽ� ������ ����
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
    // Ư�� Code�� ã������ ���� Ÿ���� ����Ʈ.
    // �������� ����� ������� ����ϴµ�, 
    // ���� �ϰ��� ������ Ŭ������ ���� ���� �ϵ��� ����.
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
    VOID                    AddCrystallizeSelector( WORD wItemType, sITEM_CRYSTALIZE_LIST* pInfo );     // [10/20/2009 namka199] Crystallize Code List Selector �߰�
    VOID                    RemoveCrystallizeSelector();                                                // :

protected:	
    VOID					Unload();

private:
    _CRYSTALIZELIST_HASH *	m_pCrystalizeListHash;
    char					m_pszFileName[MAX_PATH];
    CRYSTALLIZE_SELECTOR    m_CrystallizeSelector;
};

#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE