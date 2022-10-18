#pragma once

#include "ClientSingleton.h"
#include "heroDef.h"
#include <SCCharacter.h>

#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
#include "LuaMacroSystem/LuaMacroSystem.h"
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

class cToggleItemData
{
private:
    nsSlot::TOGGLE_ITEMS m_Data;

public:
    cToggleItemData() { m_Data.clear(); }

    bool IsActive() const { return m_Data.is_equiped_ == true; }
    POSTYPE GetItemPosition() const { return m_Data.item_pos_; }
    SLOTCODE GetItemCode() const { return m_Data.item_code_; }

    void Reset() { m_Data.clear(); }

    void SetData( const nsSlot::TOGGLE_ITEMS& data ) { m_Data = data; }
};

class cHeroData
    : public Client::Singleton<cHeroData>
{
private:
    HERO_INFO m_HeroInfo;

    cToggleItemData m_BadgeItemData;
    cToggleItemData m_PCBangItemData;

#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
    MacroSystem macro_system_;  // 어플리케이션 생성 때 cHeroData가 한번만 생성되기에 그냥 멤버변수로 포함시킴
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

public:
    cHeroData();
    ~cHeroData();

    void Initialize();
    void Release();

    const HERO_INFO* GetHeroInfo() { return &m_HeroInfo; }

#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
    MacroSystem* macro_system() { return &macro_system_; }
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

    LPCSTR GetName() { return m_HeroInfo.m_CharInfo.m_tszCharName; }
    BYTE GetClassCode() { return m_HeroInfo.m_CharInfo.m_byClassCode; }
    BYTE GetGender() { return m_HeroInfo.m_CharInfo.gender; }
    DWORD GetObjectKey() { return m_HeroInfo.m_dwKey; }

    void SetInventoryLockState( PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE State ) { m_HeroInfo.m_CharInfo.m_iInventoryLock = State; }
    PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE GetInventoryLockState() { return (PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE)m_HeroInfo.m_CharInfo.m_iInventoryLock; }


    void SetHeroInfo( const CLIENT_CHARACTER_PART& CharacterPart, const PLAYERINFO_FOR_PACKET& PlayerInfo );

    void SetObjectKey( DWORD ObjectKey ) { m_HeroInfo.m_dwKey = ObjectKey; }
    void SetClassCode( BYTE ClassCode ) { m_HeroInfo.m_CharInfo.m_byClassCode = ClassCode; }


    void SaveHero( Hero* pHero );

    void SetSkillTotalInfo( const SKILL_TOTAL_INFO* pSkillTotalInfo );
    void SetQuickTotalInfo( const QUICK_TOTAL_INFO* pQuickTotalInfo );
    void SetEquipItemInfo( const EQUIP_ITEM_INFO* pEquipItemInfo );
    void SetItemStream( const BYTE* pStreamBuffer, int StreamLength );

    void SetHideHelmet( BOOL bHide ) { m_HeroInfo.m_bHideHelmet = bHide; }

#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    void SetHideWing( BOOL bHide ) { m_HeroInfo.m_bHideWing = bHide; }
#endif //_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    void SetHideCostume(BOOL isHide) { m_HeroInfo.m_bHideCostume = isHide;}
#endif

    int GetAttackStyle() { return m_HeroInfo.m_CharInfo.m_iSelectStyleCode; }
    void SetOldAttackStyle( int OldAttackStyle ) { m_HeroInfo.m_OldAttackStyle = OldAttackStyle;} //현재 스타일을 저장해둠

    void ReleaseGuildInfo();


public:
    const cToggleItemData& GetBadgeItemData() const { return m_BadgeItemData; }
    void SetBadgeItemData( const nsSlot::TOGGLE_ITEMS& data ) { m_BadgeItemData.SetData( data ); }
    const cToggleItemData& GetPCBangItemData() const { return m_PCBangItemData; }
    void SetPCBangItemData( const nsSlot::TOGGLE_ITEMS& data ) { m_PCBangItemData.SetData( data ); }

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
public:
	void SetClientPlayerRankingGrade(const CLIENT_PLAYER_RANKING_GRADE_INFO& client_player_ranking_grade_info);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
};

#define xGetHeroData()  cHeroData::Instance()
