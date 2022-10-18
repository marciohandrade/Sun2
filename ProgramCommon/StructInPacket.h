#ifndef __STRUCTINPACKET_H__
#define __STRUCTINPACKET_H__

#pragma once

#include <Define.h>
#include <ProgramCommon/WzDataType.h>
#include <SCSlotStruct.h>
#include <Struct.h>
#include <ArchiveOfStream.h>
#include <SolarDateTime.h>

#pragma pack(push,1)
#pragma warning(push)
#pragma warning(disable : 4201)

//==================================================================================================
//==================================================================================================
// 기본 슬롯 정보
//==================================================================================================
struct BASESLOT
{
	POSTYPE m_Pos;
};
// RENDER_ITEMSTREAM -> OPT_ITEMSTREAMEX로 구조체 변경
struct ITEMSLOT : public BASESLOT
{
	OPT_ITEMSTREAMEX m_Stream;
};

struct ITEMSLOTEX : public BASESLOT
{
	RENDER_ITEMSTREAMEX m_Stream;
};

// (f100527.4L)
struct ITEMSLOTEX_INSERVER : public BASESLOT
{
    ITEMSTREAMEX m_Stream;
};

struct SKILLSLOT : public BASESLOT
{
	SKILLSTREAM m_Stream;
};


struct QUICKSLOT : public BASESLOT
{
	QUICKSTREAM m_Stream;
};

struct STYLESLOT : public BASESLOT
{
	STYLESTREAM m_Stream;
};

struct DELETESLOT : public BASESLOT
{
	DURATYPE	m_Num;	//< 삭제할 개수
};

struct VENDOR_ITEMSLOT
{
	MONEY		m_Money;		
	//DURATYPE	m_Num;			//< 개수
	POSTYPE		m_VendorPos;	//< 개인상점 창의 아이템 위치
	POSTYPE		m_OrgPos;		//< 인벤토리의 아이템 위치
};

struct VENDOR_ITEMSLOTEX
{
	MONEY		m_Money;
	BYTE		m_bLocked;
	ITEMSLOTEX	m_Stream;
};

struct EVENTSLOT
{
	WORD		m_ItemCode;
	BYTE		m_ItemCount;
	DWORD		m_DateTime;		//< 유료화 아이템의 기간(분단위)
};

struct EVENTSLOTEX : public EVENTSLOT
{
	INT			m_ItemSeq;
};

// 미션 정보
struct MISSIONSLOT : public MISSIONPART
{
	BYTE		m_MissionCode;
};

// declared by __NA001164_20081024_CHAR_WAY_POINT_SET_DB_PACKET
// 개인 웨이 포인트 정보
struct WAYPOINTSLOT
{
	WAYPOINTSLOT() 
		: m_IdxWayPoint(0), m_MapCode(0), m_X(0), m_Y(0), m_Z(0) {};

	DWORD		m_IdxWayPoint;
	MAPCODE		m_MapCode;
	SHORT		m_X;		
	SHORT		m_Y;		
	SHORT		m_Z;		
};

//==================================================================================================

// (f100527.4L)
struct EQUIP_ITEM_INFO : public _BASESPEC_SLOT_TOTAL_INFO<EQUIP_ITEM_INFO, ITEMSLOTEX,
                                                          MAX_EQUIPMENT_SLOT_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(EQUIP_ITEM_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

// 뒤에 ITEMSLOT 정보가 m_Count 만큼 따라온다.
// NOTE: data stream specification,
//  [EQUIP_ITEM_BASEINFO][ITEMSLOT]...
struct EQUIP_ITEM_BASEINFO
{
#ifdef _NA_002935_20110704_ITEM_SCORE
    typedef ITEMSLOTEX data_type;
#else
    typedef ITEMSLOT data_type;
#endif
    //
    DWORD       m_dwObjectKey;
    BYTE        m_Count;

    uint16_t GetSize() const {
        return static_cast<uint16_t>(sizeof(*this) +  sizeof(data_type) * m_Count);
    }
};

//==================================================================================================

// (f100527.4L)
struct INVENTORY_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<INVENTORY_TOTAL_INFO, ITEMSLOTEX,
                                                               MAX_INVENTORY_SLOT_NUM,
                                                               MAX_TEMPINVENTORY_SLOT_NUM,
                                                               0>
{
    POSTYPE m_InvenCount;
    POSTYPE m_TmpInvenCount;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(INVENTORY_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize +
            sizeof(m_Slot[0]) * (m_InvenCount + m_TmpInvenCount));
    }
};

// (f100527.4L)
struct JUST_INVENTORY_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<JUST_INVENTORY_TOTAL_INFO,
                                                                    ITEMSLOTEX,
                                                                    MAX_INVENTORY_SLOT_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(JUST_INVENTORY_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

// (f100527.4L)
struct INVEN_DELETE_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<INVEN_DELETE_TOTAL_INFO,
                                                                  DELETESLOT,
                                                                  MAX_INVENTORY_SLOT_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); m_Count = 0;}
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(INVEN_DELETE_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }

    INVEN_DELETE_TOTAL_INFO() {Clear();}
};

// (f100527.4L) use server only?
struct ITEM_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<ITEM_TOTAL_INFO,
                                                          ITEMSLOTEX,
                                                          MAX_EQUIPMENT_SLOT_NUM,
                                                          MAX_INVENTORY_SLOT_NUM,
                                                          MAX_TEMPINVENTORY_SLOT_NUM>
{
    POSTYPE m_EquipCount;
    POSTYPE m_InvenCount;
    POSTYPE m_TmpInvenCount;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(ITEM_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize +
            sizeof(m_Slot[0]) * (m_EquipCount + m_InvenCount + m_TmpInvenCount));
    }
};

// (f100527.4L) irregular specification
struct TRADE_TOTAL_INFO
{
    typedef ITEMSLOTEX SLOT_TYPE;
    enum { MAX_SLOT_NUM = MAX_TRADE_SLOT_NUM, };
    POSTYPE m_InvenCount;
    POSTYPE m_TmpInvenCount;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize +
            sizeof(m_Slot[0]) * (m_InvenCount + m_TmpInvenCount));
    }
};

#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
// (f100527.4L) why a slot type is the ITEMSTREAMEX instead of the ITEMSLOTEX
struct REPURCHASITEM_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<REPURCHASITEM_TOTAL_INFO,
                                                                   ITEMSTREAMEX,
                                                                   MAX_REPURCHASE_ITEM_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(REPURCHASITEM_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};
#endif 

//#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
struct Character_Achievement
{
	WORD                    index_number_;
	bool                    is_reward_;
	WORD                    object_value_[MAX_ACHIEVEMENT_OBJECT];
};

BOOST_STATIC_ASSERT(sizeof(Character_Achievement) == ACHIEVEMENT_UINT_SIZE);

struct Character_Achievements
{
	Character_Achievements() {achievement_count_ = 0;};
    void clear()
    {
        achievement_count_ = 0;
        memset(&achievement_info_, 0, sizeof(achievement_info_));
    };

	WORD	achievement_count_;
	Character_Achievement achievement_info_[MAX_ACHIEVEMENT_COUNT];	
	uint16_t GetSize() const
	{
        const size_t kMinSize = sizeof(*this) - sizeof(achievement_info_);
        return static_cast<uint16_t>(kMinSize + sizeof(achievement_info_[0]) * achievement_count_);
	}
};

struct DBCharacter_Achievement : public Character_Achievement
{
	enum Achievement_Status {EMPTY = 0, NORMAL, MODIFICATION};
    bool update_lock;
	
	void init()				{set_empty(); unlock();};
	bool is_empty()			{return achievement_status == EMPTY;};
	bool is_normal()		{return achievement_status == NORMAL;};
	bool is_modification()	{return achievement_status == MODIFICATION;};
	void set_empty()		{achievement_status = EMPTY;};		
	void set_normal()		{achievement_status = NORMAL;};		
	void set_modification()	{achievement_status = MODIFICATION;};

    bool is_update_lock()   {return update_lock;};
    void lock()             {update_lock = true;};
    void unlock()           {update_lock = false;};
	
	Achievement_Status achievement_status;
    // NOTE: f110520.1L, the approach using operator overloading don't recommend by style guide
    // because generic operator keywords do not look our eyes whether the keyword is overloaded.
    friend bool operator!=(DBCharacter_Achievement* const db_achievement, const Character_Achievement& char_achievement)
    {        
        if(db_achievement->is_reward_ != char_achievement.is_reward_)
            return true;
        
        for(int i = 0 ; i < MAX_ACHIEVEMENT_OBJECT ; ++i)
        {
            if(db_achievement->object_value_[i] != char_achievement.object_value_[i])
                return true;
        }

        return false;
    }
};

struct DBCharacter_Achievements
{
	void init()
	{
		achievement_max_index_ = 0;
        memset(&achievement_info_, 0, sizeof(achievement_info_));

		for(int i = 0 ; i < MAX_ACHIEVEMENT_COUNT ; i++)
			achievement_info_[i].init();
	};

    DBCharacter_Achievement* find(WORD index_number)
    {
        for(int i = 0; i < achievement_max_index_; i++)
        {
            if(achievement_info_[i].index_number_ == index_number)
                return &achievement_info_[i];
        }

        return NULL;
    }

	WORD	achievement_max_index_;
	DBCharacter_Achievement achievement_info_[MAX_ACHIEVEMENT_COUNT];
};

//BOOST_STATIC_ASSERT(sizeof(CHARACTER_ACHIEVEMENT_INFO) == MAX_ACSTREAM_SIZE);
//#endif

// [11/27/2009 namka199]
//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
struct ZONE_UPDATE_BATTLE_PLAYER_INFO
{
    typedef GAMEROOM_BATTLE_UPDATE_INFO BATTLE_INFO;
    enum { MAX_PLAYER = 40 };
    BYTE        m_PlayerCount;
    BATTLE_INFO m_Info[MAX_PLAYER];
    int GetSize() const
    {
        return (sizeof(*this)-(MAX_PLAYER-m_PlayerCount)*sizeof(BATTLE_INFO));
    }
};
struct ZONE_INSERT_BATTLE_PLAYER_INFO
{
    typedef GAMEROOM_BATTLE_ENTER_INFO PLAYER_INFO;
    enum { MAX_PLAYER = 40 };
    BYTE        m_PlayerCount;
    PLAYER_INFO m_Info[MAX_PLAYER];
    int GetSize() const
    {
        return (sizeof(*this)-(MAX_PLAYER-m_PlayerCount)*sizeof(PLAYER_INFO));
    }
};
struct ZONE_REMOVE_BATTLE_PLAYER_INFO
{
    typedef GAMEROOM_BATTLE_LEAVE_INFO PLAYER_INFO;
    enum { MAX_PLAYER = 40 };
    BYTE        m_PlayerCount;
    PLAYER_INFO m_Info[MAX_PLAYER];
    int GetSize() const
    {
        return (sizeof(*this)-(MAX_PLAYER-m_PlayerCount)*sizeof(PLAYER_INFO));
    }
};

//_KR_0_20091021_FIRENDINVITATION_EXTEND
struct CHAR_INVITATEDFRIEND_INFO
{
    BYTE				friend_state_;	    //5: 내가 초대한친구, 6:나를 초대한 친구 
    CHARGUID            character_guid_;    //친구의 guid    
};
//_KR_0_20091021_FIRENDINVITATION_EXTEND
struct CHAR_INVITATEDFRIEND_INFOs
{
    BYTE                friend_count_;
    CHAR_INVITATEDFRIEND_INFO   friend_[MAX_FRIEND_STAT_NUM];
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(friend_);
        return static_cast<uint16_t>(kMinSize + sizeof(friend_[0]) * friend_count_);
    };
};

// (f100527.4L)
struct SKILL_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<SKILL_TOTAL_INFO,
                                                           SKILLSLOT,
                                                           MAX_SKILL_SLOT_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(SKILL_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

// (f100527.4L)
struct WAREHOUSE_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<WAREHOUSE_TOTAL_INFO,
                                                               ITEMSLOTEX,
                                                               MAX_WAREHOUSE_SLOT_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(WAREHOUSE_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

// (f100527.4L)
struct VENDOR_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<VENDOR_TOTAL_INFO,
                                                            VENDOR_ITEMSLOTEX,
                                                            MAX_PERSONAL_VENDOR_SLOT_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(VENDOR_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

struct VENDOR_LIST_INFO
{
    char    m_pszTitle[MAX_VENDOR_TITLE_LENGTH + 1];
    char    m_pszID[ID_MAX_LEN + 1];
    DWORD   m_dwPlayerKey;
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    WORD    zone_key_;
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    void Output(ArchiveOfStream& archive) const;
    void Input(const ArchiveOfStream& archive);
};

struct VENDOR_LIST_TOTAL_INFO
{
    enum { MAX_SLOT_NUM = COUNT_VENDOR_IN_PAGE,};
    BYTE m_AllPage; //총 페이지 수
    BYTE m_Count;   //한 페이지에 출력할 리스트 갯수
    VENDOR_LIST_INFO m_List[MAX_SLOT_NUM];
    void Output(ArchiveOfStream& archive) const;
    void Input(const ArchiveOfStream& archive);
};

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
struct VENDOR_SEARCH_ITEM_INFO
{
    VENDOR_LIST_INFO vendor_info_;
    VENDOR_ITEMSLOTEX item_info_ex_;
};

struct ENHANCED_VENDOR_SEARCH_RESULT
{
    int current_page_;
    int total_page_;
    int item_count_;
    VENDOR_SEARCH_ITEM_INFO vendor_items[VENDOR_ITEMS_PER_PAGE];
};

struct ESTABLISHER_VENDOR_TOTAL_INFO :
    public _BASESPEC_SLOT_TOTAL_INFO<ESTABLISHER_VENDOR_TOTAL_INFO,
                                     VENDOR_ITEMSLOT,
                                     MAX_PERSONAL_VENDOR_SLOT_NUM, 0, 0>
{
    typedef struct {
        SLOT_TYPE m_Slot; //< 판매할 아이템의 인벤토리 위치
        TCHAR szName[(MAX_ITEMNAME_LENGTH)*2+1];
    } SELL_ITEM;

    POSTYPE   m_Count;
    SELL_ITEM sell_items_[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(sell_items_);
        return static_cast<uint16_t>(kMinSize + sizeof(sell_items_[0]) * (m_Count));
    }
};
#else
// (f100527.4L)
struct ESTABLISHER_VENDOR_TOTAL_INFO :
    public _BASESPEC_SLOT_TOTAL_INFO<ESTABLISHER_VENDOR_TOTAL_INFO,
                                     VENDOR_ITEMSLOT,
                                     MAX_PERSONAL_VENDOR_SLOT_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM]; //< 판매할 아이템의 인벤토리 위치
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(ESTABLISHER_VENDOR_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

// (f100527.4L)
struct QUICK_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<QUICK_TOTAL_INFO,
                                                           QUICKSLOT,
                                                           MAX_QUICK_SLOT_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(QUICK_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

// (f100527.4L)
struct STYLE_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<STYLE_TOTAL_INFO,
                                                           STYLESLOT,
                                                           MAX_STYLE_SLOT_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(STYLE_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

// (f100527.4L)
struct EVENT_ITEM_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<EVENT_ITEM_TOTAL_INFO,
                                                                EVENTSLOT,
                                                                MAX_EVENT_INVENTORY_SLOT_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(EVENT_ITEM_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

// (f100527.4L)
struct EVENT_ITEM_TOTAL_INFO_EX : public _BASESPEC_SLOT_TOTAL_INFO<EVENT_ITEM_TOTAL_INFO_EX,
                                                                   EVENTSLOTEX,
                                                                   MAX_EVENT_INVENTORY_SLOT_NUM,
                                                                   0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(EVENT_ITEM_TOTAL_INFO_EX) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

// (f100527.4L)
struct MISSION_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<MISSION_TOTAL_INFO,
                                                             MISSIONSLOT,
                                                             MAX_MISSION_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(MISSION_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

// (f100527.4L)
struct WAYPOINT_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<WAYPOINT_TOTAL_INFO,
                                                              MAPCODE,
                                                              MAX_MAP_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(WAYPOINT_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};



// declared by __NA001164_20081024_CHAR_WAY_POINT_SET_DB_PACKET
// (f100527.4L)
struct CHAR_WAYPOINT_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<CHAR_WAYPOINT_TOTAL_INFO,
                                                                   WAYPOINTSLOT,
                                                                   MAX_CHAR_WAYPOINT_NUM, 0, 0>
{
    POSTYPE   m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    // (WARNING) a WAYPOINTSLOT has a constructor, but that's enough.
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(CHAR_WAYPOINT_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

//==================================================================================================

//{__NA001283_20090225_COLLECTION_USE
// (NOTE) (WAVERIX) operator=와 자동형변환 가능한 operator에 스스로 자멸했던 나로써는 영 찜찜한 구조.

struct C_COLLECTKEY
{
	C_COLLECTKEY() : Key(0) {}
	const DWORD operator = (const DWORD rhs) { return (Key = MAP_OBJECT_KEY>rhs?rhs+MAP_OBJECT_KEY:rhs); }
	operator DWORD() { return Conv_MapObjectKey(Key); }

	DWORD Get() { return Conv_MapObjectKey(Key); }
private:
	C_COLLECTKEY(C_COLLECTKEY& rhs) { __UNUSED(&rhs); }
	DWORD Key;
};

typedef C_COLLECTKEY S_COLLECTKEY;

//==================================================================================================

struct COLLECT_RENDER_INFO
{
	C_COLLECTKEY	m_dwObjectKey;			// 채집물품.
	WzID			m_CollectionID;			// 채집물품ID.

	// 삭제될 정보
	DWORD			m_Collector;			// 채집자.		(채집중일경우 != 0)
	DWORD			m_dwRemainTime;			// 남은시간.	(채집중일경우 != 0)
};

struct COLLECT_PLAYER_RENDER_INFO 
{
	BYTE			m_bUseCollectRanderInfo;// 사용여부.
	C_COLLECTKEY	m_dwObjectKey;			// 채집물품.
	WzID			m_CollectionID;			// 채집물품ID.
	DWORD			m_dwRemainTime;			// 남은시간.	(채집중일경우 != 0)

	void Output( ArchiveOfStream & archive )
	{
		if( m_bUseCollectRanderInfo )
			archive << *this;
		else
			archive << m_bUseCollectRanderInfo;
	}
	void Input( ArchiveOfStream & archive )
	{
		if( m_bUseCollectRanderInfo )	
			archive >> *this;
		else
			archive >> m_bUseCollectRanderInfo;
	}
};
//}__NA001283_20090225_COLLECTION_USE

// 쿠키 정보
struct COOKIETYPE
{
	BOOL						IsEqualTo( COOKIETYPE & cookie )
	{
								if( cookie.dwCookie == dwCookie ) 
									return TRUE;
								else
									return FALSE;
	}
	DWORD						dwCookie;
};

// NOTE: f110502.4L, remove unused section
//// 슬롯 상의 캐릭터 정보
//struct CHAR_SLOT
//{
//	DWORD						m_dwCharGuid;
//	BYTE						m_bySlot;
//	BYTE						m_byClass;
//	WORD						m_wLevel;
//	char						m_lpszName[MAX_CHARNAME_LENGTH];
//};

//==================================================================================================

struct STATE_DETAIL_INFO
{
    WORD     m_wStateCode;       // 상태 코드
    SLOTCODE m_SkillCode;        // 스킬 코드
    // (WAVERIX) (090709) (CHANGES) Specification: __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__
    // MAX_ABILITY_NUM == 5 (3Bit)
    // m_byAbilityIndex:3 - AbilityIndex
    // m_byAbilityIndex:5 - bit field, skill option
    BYTE     m_byAbilityIndex;   // 어빌리티 인덱스
    int      m_iRemainTime;      // 남은 시간(-1이면 무한대)
    WORD     m_wInterval;        // 주기
};

struct STATE_INFO
{
    SLOTCODE m_SkillCode;        // 스킬 코드
    bool     m_bReflect;         // 피격류인지 구분
    BYTE     m_byAbilityIndex;   // 어빌리티 인덱스
    int      m_iLeavedTime;      // 남은 시간(-1이면 무한대)
};

// 스킬로 생성되지 않는 상태
struct ETC_STATE_INFO
{
    WORD m_wStateCode;       // 상태 코드
    int  m_iLeavedTime;      // 남은 시간(-1이면 무한대)
};

struct STATE_GM_INFO
{
    BYTE m_MoveSpeedLevel : 4;    // 속도
    BYTE m_TransOn        : 2;    // 투명
    BYTE m_UndeadOn       : 2;    // 무적
};

//_NA_0_20100208_DOMINATION_AREA_INFO
// 길드가 가지는 상태(점령전 인장 관련 정보)
struct DOMINATION_STATE_INFO
{
    FIELDCODE m_wDominationCode;
};

//==================================================================================================

typedef sTEMPLATE_TOTAL_INFO<STATE_DETAIL_INFO> STATE_DETAIL_TOTAL_INFO;
typedef sTEMPLATE_TOTAL_INFO<STATE_INFO>		STATE_TOTAL_INFO;
typedef sTEMPLATE_TOTAL_INFO<ETC_STATE_INFO>    ETC_STATE_TOTAL_INFO;
//_NA_0_20100208_DOMINATION_AREA_INFO
typedef sTEMPLATE_TOTAL_INFO<DOMINATION_STATE_INFO> DOMINATION_STATE_TOTAL_INFO;

// (WAVERIX) (NOTE) 이제 슬슬 제거해도 되는 값들이 포함되어 있다. 확인하고 지울 수 있도록 할 것.
#define HARDCODED_SPECIAL_RENDER_EFFECT_ITEMS() \
    ((eSPECIAL_RENDER_EFFECT)( eSPECIAL_RENDER_EFFECT_SOLO_BADGE        \
                             | eSPECIAL_RENDER_EFFECT_COUPLE_BADGE      \
                             | eSPECIAL_RENDER_EFFECT_2nd_EFFECT_ITEM   \
                             | eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_01    \
                             | eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_02    \
                             | eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_03    \
                             | eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_04    \
                             | eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_05))

//==================================================================================================
//==================================================================================================
#pragma pack(push, 4)
struct PLAYER_RENDER_INFO_EX
{
    BOOLEAN     is_player_render;   // true : PLAYER_RENDER_INFO, false : PLAYER_VILLAGE_INFO
    WORD        player_key;
    PLAYERHP    hp;
    PLAYERHP    max_hp;
    LEVELTYPE   level;
    char        char_name[MAX_CHARNAME_LENGTH + 1];
    WzVector    position;
    SLOTCODE    selected_style_code;
    WORD        move_speed_ratio;   // 이동속도(100을 곱한 %수치)
    WORD        attack_speed_ratio; // 공격속도(100을 곱한 %수치)

    struct { // bits
        BYTE    transparent;    //2 = 2     투명( 0:일반, 1:GM투명상태, 2:섀도우스킬하이드)
        BYTE    helmet;         //1 = 3    투구 옵션(0: 보이기, 1: 안보이기)
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        BYTE    wing;    // 1 = 4
#endif
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
        BYTE    costume; // 1 = 5
#endif // _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
        BYTE    gm_grade;       //3 = 6    GM 등급( 0: 일반, 1이상: GM, 4 : 가상아이디 )
        BYTE    pcroom;         //1 = 7    PC방 유저(0: 일반, 1 : PC방)
        BYTE    char_condition; //1 = 8    현재상태(앉기/서기) eCHAR_CONDITION
        BYTE    char_class;     //3 = 11    eCHAR_TYPE
        BYTE    boradori;       //1 = 12    보라돌이 상태 (t/f)
        BYTE    chao_state;     //2 = 14    eCHAO_STATE = { 0, 1, 2 }
        BYTE    behave_state;   //5 = 19    ePLAYER_BEHAVE_STATE
        BYTE    spread_wings;   //1 = 20    날개 펼침 유무
    };
    struct { // bits
    #ifdef _NA_002050_20110216_ADD_GENDER
        BYTE    gender; // 2 = 2 성별(0:모두, 1:남성, 2:여성) Const.h GenderType 참고
    #endif
        BYTE    hair_style;     //6 = 8     머리모양
        BYTE    height;         //3 = 11    키
        BYTE    face;           //4 = 15    얼굴모양        
    };

    // 4byte - next info - duplicable by bits
    eSPECIAL_RENDER_EFFECT special_effects; // <REF> 렌더로 보여야 할 다양한 비주얼 효과 정의
    //__NA_0_PET_SYSTEM, __NA_1131_PET_NAME_SETTING(901)
    BOOLEAN     is_summoned_pet;
    struct { // if (is_summoned_pet != 0)
        WORD    pet_index;
        BYTE    pet_fullness;
        BYTE    pet_intimacy;
        char    pet_name[MAX_PET_NAME_LENGTH + 1]; // 펫네임이 있는 경우 pet_name[0] != '\0'
    };
    BYTE        ether_device_index;   // 0이면 비활성화
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    WORD honor_title; // 칭호코드
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_004035_20120227_COSTUME_ITEM
    struct  //코스튬 아이템
    {
        bool is_equipped_costume; // 코스튬 활성화 여부
        SLOTCODE costume_item_code; // 코스튬 아이템 코드

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        bool is_equipped_decorate_costume; // 데코 코스튬 활성화 여부
        SLOTCODE decorate_costume_item_code; // 데코 코스튬 아이템 코드
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
    };
#endif // _NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    SLOTCODE spa_ticket_code; // 온천 코스튬 아이템코드
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
    DWORD total_item_score_;    // 장비평점을 보내주자
#endif // _NA_000000_20130206_BROADCAST_ITEM_SCORE

    BOOLEAN     is_riding; //__NA001390_090915_RIDING_SYSTEM__

    RENDER_ITEMSTREAMEX rider_item_slot;

    COLLECT_PLAYER_RENDER_INFO collection_render_info;
    //
    STATE_TOTAL_INFO state_total_info; // 어빌리티 상태 정보

    //----------------------------------------------------------------------------------------------
    eSPECIAL_RENDER_EFFECT GetChangeOfClassSteps() const {
        const eSPECIAL_RENDER_EFFECT BITS = (eSPECIAL_RENDER_EFFECT)
                                          (eSPECIAL_RENDER_EFFECT_CHANGEofCLASS_1st
                                          /*|(~~)*/);
            return (eSPECIAL_RENDER_EFFECT)(BITS & special_effects);
    }
    eSPECIAL_RENDER_EFFECT GetBadgeEventValue() const {
        const eSPECIAL_RENDER_EFFECT BITS = HARDCODED_SPECIAL_RENDER_EFFECT_ITEMS();
        return (eSPECIAL_RENDER_EFFECT)(BITS & special_effects);
    }
    //----------------------------------------------------------------------------------------------
    // returns : encoded length (bytes)
    static int ConvertForSend(BOOLEAN is_player_render, // true : PLAYER_RENDER_INFO, false : PLAYER_VILLAGE_INFO
                              const PLAYER_RENDER_INFO_EX& input,
                              PLAYER_RENDER_INFO_EX* packed_output); //packe
    // returns : decoded length (bytes)
    static int ConvertForRecv(const PLAYER_RENDER_INFO_EX* packed_input, PLAYER_RENDER_INFO_EX* output);
};
/*
    ------   PLAYER_RENDER_INFO_EX used sample    ------
    --- 클라이언트에서의 파싱 처리 ---

    PLAYER_RENDER_INFO_EX* received_packet = ~~ // 패킹된 정보이다. 그냥 읽으면 문제 발생!!!
    PLAYER_RENDER_INFO_EX output; // 초기화 필요 없음.
    PLAYER_RENDER_INFO_EX::ConvertForRecv(*recieved_packet, &output);

    // output에 대해 연산하면 된다.
    if(output.is_player_render)  <- 이전 PLAYER_RENDER_INFO타입으로 읽으면 된다.
    {
        DWORD player_object_key = output.player_key;
        ...
    }

*/

#pragma pack(pop) //for PLAYER_RENDER_INFO_EX pack

//==================================================================================================
//==================================================================================================


//==================================================================================================
// 뒤에 STATE_INFO 정보가 m_byCount 만큼 따라온다.
struct MONSTER_RENDER_INFO
{
    DWORD       m_dwObjectKey;
    MONSTERCODE m_dwMonsterCode;
    WzVector    m_wvPos;
    DWORD       m_dwMaxHP; //_NA001385_20090924_DOMINATION_NPC_MAXHP
    DWORD       m_dwHP;
    WORD        m_wMoveSpeedRatio   : 12;   // 이동속도(100을 곱한 %수치)
    WORD        m_wAttSpeedRatio    : 11;   // 공격속도(100을 곱한 %수치)
    WORD        m_wAngle            : 9;

    STATE_TOTAL_INFO m_StateTotalInfo;       // 상태 토탈 정보
    //
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_StateTotalInfo);
        return static_cast<uint16_t>(kMinSize + m_StateTotalInfo.GetSize());
    }
};

//==================================================================================================
// 소환체 렌더인포
struct SUMMONED_RENDER_INFO
{
    BYTE        m_byCurCommand;         // 현재 명령 상태
    DWORD       m_dwSummonerObjKey;     // 소환자 오브젝트키
    DWORD       m_dwObjectKey;
    MONSTERCODE m_dwMonsterCode;
    WzVector    m_wvPos;
    DWORD       m_dwMaxHP;
    DWORD       m_dwHP;
    WORD        m_wMoveSpeedRatio;      // 이동속도(100을 곱한 %수치)
    WORD        m_wAttSpeedRatio;       // 공격속도(100을 곱한 %수치)
    //__NA_S00015_20080828_NEW_HOPE_SHADOW | _NA001385_20090924_DOMINATION_SKILL
    BYTE        m_bySummonType;         // 1:일반소환, 2:죽은몬스터소환, 3: 크리스탈 워프(m_dwSummonerObjKey에 GUILDGUID가 들어간다)
    STATE_TOTAL_INFO m_StateTotalInfo; // 상태 토탈 정보
    //
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_StateTotalInfo);
        return static_cast<uint16_t>(kMinSize + m_StateTotalInfo.GetSize());
    }
};

//==================================================================================================

struct MAPNPC_RENDER_INFO
{
    DWORD  m_dwMapNpcID;
    MONSTER_RENDER_INFO m_sMonsterRenderInfo[1];
    //
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_sMonsterRenderInfo);
        return static_cast<uint16_t>(kMinSize + m_sMonsterRenderInfo->GetSize());
    }
};

//==================================================================================================

//_NA_0_20100222_UNIT_TRIGGERS_REGEN
// 몬스터와 다른 타입 정보 필요.
// MONSTER_RENDER_INFO 보다 적은양의 정보만 있어도 되나. 
// 많이 발생 하지 않으므로 전체 몬스터 정보 전달.
struct MERCHANT_RENDER_INFO
{
    MONSTER_RENDER_INFO m_sMonsterRenderInfo[1];
    //
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_sMonsterRenderInfo);
        return static_cast<uint16_t>(kMinSize + m_sMonsterRenderInfo->GetSize());
    }
};

//==================================================================================================

struct TOTEMNPC_RENDER_INFO
{
    DWORD       m_dwObjectKey;
    WzVector    m_wvPos;
    WORD        m_ItemCode;
    MONSTERCODE m_dwMonsterCode;
    //
    uint16_t GetSize() const {
        return static_cast<uint16_t>(sizeof(*this));
    }
};

//==================================================================================================

struct ITEM_RENDER_INFO
{
    enum eFIELDITEMTYPE{ eFIELDITEM_ITEM, eFIELDITEM_MONEY, };
    DWORD     m_dwObjectKey;
    PLAYERKEY m_dwOwnerPlayerKey;
    BYTE      m_byFieldItemType;
    union
    {
        ITEMSTREAMEX m_ItemStream;
        MONEY        m_Money;
    };
    float     m_fPos[3];
#ifdef _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE
    DWORD     m_dwRemainedTime;
#endif
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    BYTE dicecast_item_type_;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
};

//==================================================================================================

struct GUILD_RENDER_INFO
{
    // (WAVERIX) (090920) (CHANGES) no-define work
    GUILD_RENDER_INFO() {
        ZeroMemory(this, sizeof(*this));
        //m_RelationType = eGUILD_RELATION_NONE; <--
        BOOST_STATIC_ASSERT(eGUILD_RELATION_NONE == 0);
    }
    //GUILD_RENDER_INFO() 
    //    : m_dwObjectKey(0),
    //    m_GuildGuid(0),
    //    m_PatternIndex(0),
    //    m_BackGroundIndex(0),
    //    m_RelationType(eGUILD_RELATION_NONE),
    //    m_GuildGrade(0)
    //{
    //    ZeroMemory(m_tszGuildName,MAX_GUILDNAME_LENGTH+1);
    //};

public:
	DWORD m_dwObjectKey;
	GUILDGUID m_GuildGuid;
	GUILDMARKIDX m_PatternIndex;
	GUILDMARKIDX m_BackGroundIndex;
	eGUILD_RELATION m_RelationType;

    char m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	//< NULL포함

#ifdef _NA_008334_20150608_SONNENSCHEIN
    BYTE sonnenschein_phase;
#endif //_NA_008334_20150608_SONNENSCHEIN

    //_NA_0_20100208_DOMINATION_AREA_INFO
    BYTE m_GuildGrade;                              //< eGUILD_GRADE
    BYTE m_GuildDuty;                               //< eGUILD_DUTY
    DOMINATION_STATE_TOTAL_INFO m_stateTotalInfo;   //< 점령전 상태 정보
    //
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_stateTotalInfo);
        return static_cast<uint16_t>(kMinSize + m_stateTotalInfo.GetSize());
    }


public:
	bool IsExist() const { return ( m_GuildGuid != 0); }
};

//==================================================================================================

// 개인 상점 관련 정보 
struct VENDOR_RENDER_INFO
{
    BYTE m_byTitleLength;
    char m_tszVendorTitle[MAX_VENDOR_TITLE_LENGTH+1];
    //
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_tszVendorTitle);
        return static_cast<uint16_t>(kMinSize + sizeof(m_tszVendorTitle[0]) * m_byTitleLength);
    }
};

//==================================================================================================

struct ROOM_LIST_INFO
{
    DWORD m_dwNo;                                     // 방 번호
    BYTE  m_byType;                                   // 방 종류
    BYTE  m_byMemberCount;                            // 방 참가 인원 수
    DWORD m_dwMapID;                                  // 맵 아이디
    char  m_lpszName[MAX_ROOMTITLE_LENGTH];           // 방 제목
};

//==================================================================================================
//  (WAVERIX) (090920) (CHANGES) no-define work Output&Input -> moved to code-file
struct CLIENT_CHARACTER_PART
{
    BYTE   m_bySlot;
    char   m_tszCharName[MAX_CHARNAME_LENGTH+1];
    BYTE   m_byHeight;
    BYTE   m_byFace;
    BYTE   m_byHair;
    BYTE   m_byClass;
    LEVELTYPE m_LV;
    INT    m_iRegion;
    short  m_sX;
    short  m_sY;
    short  m_sZ;
    BYTE   m_ChangeOfClassStep;
    EQUIP_ITEM_INFO m_EquipItemInfo;
    BYTE   m_DelChk;   //< 0: 삭제안된 캐릭, 1,2:사용안함(DB에서사용) 3: 삭제된 캐릭
    char   m_tszDelDate[MAX_SMALLDATETIME_SIZE + 1];

    //__NA001084_PET_IMAGE_DRAW_IN_CHARACTER_SELECT_SCREEN
    ITEMSTREAMEX m_PetSlotStream;
    BOOL m_bSummoned;
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE gender; // 성별
#endif
#ifdef _NA_004035_20120227_COSTUME_ITEM
    bool is_equipped_costume;
    POSTYPE costume_item_pos;
    SLOTCODE costume_item_code;
#endif  // _NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    bool is_equipped_decorate_costume;
    POSTYPE decorate_costume_item_pos;
    SLOTCODE decorate_costume_item_code;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

    void   Output(ArchiveOfStream& archive);
    void   Input(ArchiveOfStream& archive);
};

//==================================================================================================

struct HONGPOS
{
	WzVector wvPos;
	short wTile;

};

struct GM_CHARINFO
{
	char			m_szCharName[MAX_CHARNAME_LENGTH];
	BYTE			m_byClass;
	LEVELTYPE		m_LV;
};

//==================================================================================================


struct AREACONQUEST_TOTAL_INFO
{
//	DWORD dwValue[MAX_AREA_CONQUEST_CONDITION];
};

// (WAVERIX) (NOTE) 더 이상 사용하지 않는 것으로 알고 있다. 확인 후 제거해 버리자.
struct PARTY_MEMBER_INFO
{
	DWORD				m_dwObjKey;								// 오브젝트 키
#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
	char				m_szAccountID[ID_MAX_LEN + 1];			// 계정 ID
#else
    char				m_szAccountID[ID_MAX_LEN];			    // 계정 ID
#endif
	char				m_szCharName[MAX_CHARNAME_LENGTH];		// 캐릭터 ID
	WORD				m_wLevel;								// 레벨
	BYTE				m_bMaster			: 2;				// 파티장인가
	BYTE				m_byClassType		: 3;				// 클래스 타입(eCHAR_TYPE)
	BYTE				m_byChaoState		: 3;				// 카오상태
	BYTE				m_byHPRatio;							// 현재 HP 비율
	BYTE				m_byMPRatio;							// 현재 MP 비율

	PARTY_MEMBER_INFO()
	{
		m_dwObjKey = 0;
		memset( m_szCharName, 0, sizeof(char)*MAX_CHARNAME_LENGTH );
		m_wLevel = 0;
		m_byChaoState = eCHAO_STATE_NORMAL;
		m_byHPRatio = 0;
		m_byMPRatio = 0;
	}
};

//==================================================================================================

struct DAMAGE_INFO
{
	DWORD			m_dwTargetKey;		// 공격 당하는 오브젝트 키
    DAMAGETYPE		m_wDamage;			// 공격 데미지
    DWORD			m_dwTargetHP;		// 공격 후 타겟의 HP
};

struct EFFECT_BOMB_INFO : DAMAGE_INFO
{
	WORD			m_wStateID;			// 추가되는 상태코드
};

//==================================================================================================


struct CRASH_INFO
{
	DWORD	m_dwCrahAddress;
	DWORD	m_dwModuleStartAddress;
	DWORD	m_dwModuleEndAddress;
	DWORD	m_dwModuleCrashOffset;
	char	m_szModuleName[MAX_MODULE_NAME_SIZE];
	char	m_szBuildVer[MAX_BUILD_VERSION_SIZE];
	DWORD64	m_dw64CrashTime;
};

struct COMPUTER_INFO
{
	char	m_szOS[MAX_OS_NAME_SIZE];
	char	m_szProcess[MAX_PROCESS_NAME_SIZE];
	char	m_szPhysicalMemory[MAX_PHYSICAL_MEMORY_SIZE];
	char	m_szDirectXVersion[MAX_DIRECTX_NAME_SIZE];
	char	m_szDescription[MAX_DESCRIPT_SIZE];
	char	m_szDisplayMemory[MAX_DISPLAY_MEMORY_SIZE];
	char	m_szDriverVersion[MAX_DRIVER_VERSION_SIZE];
};

//==================================================================================================

struct GUILDWAREHOUSE_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<GUILDWAREHOUSE_TOTAL_INFO,
                                                                    ITEMSLOTEX,
                                                                    MAX_GUILD_WAREHOUSE_SLOT_NUM,
                                                                    0,
                                                                    0>
{
    POSTYPE m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(GUILDWAREHOUSE_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

//==================================================================================================

//{__NA001283_20090227_COLLECTION_LIST
//클라 페킷 필요 없음.
//캐릭터기 채집한 채집물 정보
struct COLLECTED_SLOT
{
	WzID	m_CollectedID;				// 채집물품 ID (UserID)
	WORD	m_wCount;					// 채집 갯수
};

struct COLLECTED_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<COLLECTED_TOTAL_INFO,
                                                               COLLECTED_SLOT,
                                                               MAX_COLLECTION_LIST_NUM,
                                                               0,
                                                               0>
{
    POSTYPE m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(COLLECTED_TOTAL_INFO) == kDerivedClassSize);
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};
//}

//==================================================================================================

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
#pragma pack(push)
#pragma pack()
struct QuestPortalInfo
{
    PORTALKEY   portalKey;          // Portal에 할당된 키 | 해당 Room을 위해 예약된 키
    MAPCODE     enterableMapCode;   // 진입 가능 맵코드 (미션, 인던, 경쟁헌팅 등등...)
    WORD        requesterKey;       // 포탈 생성 요청자
    WORD        questCode;          // : QCODE 퀘스트에 의한 포탈인 경우 해당 QuestCode
    DWORD       portalLifetime;     // 포탈 유지 시간 (10')
    WzVector    portalPosition;     // 포탈 생성 위치
};
#pragma pack(pop)
//}

//==================================================================================================
//
//
#ifndef __SSQ_MSG_HDR__
#define __SSQ_MSG_HDR__
// <Prototype of WAR_MSG_HDR>
//  전체 메시지의 길이가 담겨진다. (variable stream)
struct WAR_MSG_HDR
{
    typedef ulong MSG_SIZE_T; // expanded _NA001605_EP2_CHANGE_SKILL_BROADCASTER_
    MSG_SIZE_T size;
};

//	<Raw Header of Each Message>
//	-	각 메시지의 타입들이 기재된다.
//	-	1Byte, 하지만 raw stream인 WAR_TAG_V의 이것을 직접 사용하는 경우는 없어야 한다.
//		(이것 common routine에서의 offset연산 편의성이 목적이다.)
//		모든 메시지들은 실질적으로 WAR_TAG1_<id, type>을 상속해서 구현되게 되어 있다.
//	<목적>
//	-	Server/Client 각각 pointer연산을 수행하면서 버그를 유발할 수 있다.
//		하나의 로직으로 serializing input/output이 가능해지도록 하는 것이 목표다.
typedef struct WAR_TAG_V
{
	BYTE				_TAG;
} *WAR_TAG_P;
#endif //__SSQ_MSG_HDR__


namespace nsSSQ
{
	class SSQTicketInfo	// Packet Message
	{
	public:		enum eTM {
					eTM_None				,	// 미설정
					eTM_Transaction			,	// 질의중
					eTM_EnterableTimeout	,	// 시작 ~ 입장가능
					eTM_CloseTimeout		,	// 입장가능 ~ 입장종료
				};

	protected:	BYTE				tmTableIndex;	// 입장 시간 기재된 테이블에서의 인덱스
				BYTE				tmMode;			// 해당 인덱스 기준 : eTM
				DWORD				countdownTick;	// 해당 기준점까지 남은 틱 (서버 기준)
	};
};	// end namespace nsSSQ

//==================================================================================================

//#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
namespace nsPacket {

struct ReAuthInfoBase
{
    enum eReAuthStep {
        eReAuthStep_None,
        eReAuthStep_Requested,
        eReAuthStep_Accepted,
        eReAuthStep_Canceled,
        eReAuthStep_Count,
    };
    typedef WORD ReAuthKeyType;
};

struct ReAuthTokenStream
{
    enum {
        MAX_AUTHKEY_STREAM_SIZE = 36 + 1,
        MAX_ID_STREAM_SIZE      = ID_MAX_LEN + 1,
        MAX_STREAM_SIZE         = MAX_AUTHKEY_STREAM_SIZE + MAX_ID_STREAM_SIZE,
    };
    DWORD   type_code;
    BYTE    stream[MAX_STREAM_SIZE];
};

//__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
struct HanAuthTokenStream
{
    enum {
        MAX_MEMBER_ID           = 50,
        MAX_AUTHSTRING          = (4 << 10),
        MAX_AUTHKEY_STREAM_SIZE = MAX_MEMBER_ID + 1 + MAX_AUTHSTRING + 1,
    };
    DWORD   type_code;
    BYTE    stream[MAX_AUTHKEY_STREAM_SIZE];
};

// ref: _JP000000_ONETIMEPASSWORD_AUTHFLOW_
struct OneTimePassword
{
    static const size_t kLength = 20;
    char value[kLength + 1];
};

struct OneTimePasswordStream
{
    enum {
        MAX_OTP_LENGTH = 20, // no-null terminated string length == strlen(otp_string);
        MAX_OTP_STREAM_SIZE = MAX_OTP_LENGTH + 10,
    };
    DWORD   type_code;
    char    stream[MAX_OTP_STREAM_SIZE];
};

}; //end of namespace 'nsPacket'
//#endif

//dev : LoginFront 와 Clent가 공유하는 인증 정보 구조체(IngambaAuthInfo)
#ifdef _RU_INGAMBA_AUTH_PROCESS 
struct IngambaAuthInfo
{
    enum {
        MAX_ID_LEN          = 50,
        MAX_PWD_LEN         = 24,
        MAX_IP_LEN          = 18,
    };
    DWORD   type_code_;
    char user_id_[MAX_ID_LEN];
    char user_password_[MAX_PWD_LEN];
};

struct IngambaUserInfo
{
    int status_ID;
    bool status_in_use;
    int status_remaining_time;
    char user_account[ID_MAX_LEN];
};
#endif
//#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
struct GameChuInfoStream
{
    enum {MAX_FIRST_PARAMETER = 512 + 1, MAX_SECOND_PARAMETER = 8 + 1};
        
    DWORD type_code;

    wchar_t first_parameter[MAX_FIRST_PARAMETER];
    wchar_t second_parameter[MAX_SECOND_PARAMETER];
};
//#endif

//__CN_20100909_SECURITY_TOKEN__
struct SecurityTokenCode
{
	enum {MAX_SECURITY_LENGTH = 12 + 1
	};

	DWORD	type_code_;
	char	security_token_code_[MAX_SECURITY_LENGTH];
};


//==================================================================================================

//__NA001336_090514_PARTY_NAVI_SUPPORT__
struct PlayerStatEvent
{
    enum eFlag {
        eFlag_None          = 0,
        eFlag_ChangedHP     = 1 << 0,
        eFlag_ChangedMP     = 1 << 1,
        eFlag_ChangedPos    = 1 << 2,
        eFlag_ChangedSD     = 1 << 3,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        eFlag_ChangedFP     = 1 << 4,
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };
};

//==================================================================================================

//__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {

// 하임 로또 추첨 결과 건 레코드
struct LottoResultRecord
{
    WORD  wDrawNum;                     // 추첨 회차
    BYTE  bNumWinners;                  // 1등 당첨자 수
    BYTE  bWinningNum[LOTTO_NUM_PICKS]; // 당첨 번호
    INT64 i64Jackpot;                   // 1등 당첨금
};

// 하임 로또 응모 건 레코드
struct LottoEntryRecord
{
    WORD wDrawNum;                      // 응모 회차
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // 응모 번호
};

// 하임 로또 당첨 안내 쪽지 레코드
struct LottoNoticeRecord
{
    INT64 i64Index;                     // 응모 건 ID
    WORD  wDrawNum;                     // 응모 회차
    BYTE  bRank;                        // 등위
    INT64 i64Prize;                     // 상금
};

//}

//==================================================================================================

// Added by __NA000000_SHOP_USERID_50_PASSING_PROC__
//  (WARNING) 인증에서도 사용하는 구조체이며, 이것이 틀려질 경우는 주의해야 한다.
struct QueryUserIdInfo {
    DWORD requester_user_key_;                  //#1   G->D : Exist Query Syn (Auth<->Agent<->Game)
    // query informations...
    char char_name_[MAX_CHARNAME_LENGTH + 1];   //#1   G->D : Exist Query Syn
    USERGUID user_guid_;                        //#2.1 D->G : Exist Query Ack
    CHARGUID char_guid_;                        //#2.2 D->G : Exist Query Ack
    BYTE char_class_;                           //#2.3 D->G : Exist Query Ack
    LEVELTYPE char_level_;                      //#2.4 D->G : Exist Query Ack
    //#3   G->Auth : Request User ID {in AuthProtocol}
    char user_id_[ID_MAX_LEN + 1];              //#3   G->Auth : Get User ID {in AuthProtocol }
#ifdef _RU_20110801_SEND_GIFT_FRIEND_CHECK
    CHARGUID from_char_guid_;                    //요청하는 캐릭터의 GUID
#endif
};


//==================================================================================================
//__NA001390_090915_RIDING_SYSTEM__
struct RiderControl {
    enum eRequest { // Client -> Server Request
        eReq_None           = 0,
        eReq_SummonStart    , // 소환 시작 캐스팅
        eReq_SummonCancel   , // 소환 캐스팅 취소
        eReq_Summon         ,
        eReq_SummonRelease  ,
        eReq_EquipParts     ,
        eReq_UnEquipParts   ,
        eReq_WaxGloss       , // 왁스 아이템으로 파츠 광내기
        eCmd_SyncRider      , // 
        eReq_SummonBattleRelease  ,
        eReq_Counts // upperbound
    };

    enum eResult { // Client <- Server Result
        eRet_Success        = 0,
        eRet_Failed         ,   // ref: RC::RC_RIDER_~~
        eRet_ItemFailed     ,   // ref: RC::RC_ITEM_~~
        eRet_RoomFailed     ,   // ref: RC::RC_ROOM_~~
        eRet_StatusFailed   ,   // ref: RC::RC_STATUS_~~
        eRet_Counts // upperbound
    };

    //enum eError { //eRet_Failed
    //    eErr_NoDefined      = 0,
    //    eErr_Transaction    ,   // Rider related transaction
    //    eErr_UnexpectedStatus,
    //    eErr_CantFindInfo,      // ref RidingInfo index
    //    eErr_AlreadySummoned,
    //    eErr_AlreadySummonReleased,
    //    eErr_Counts // upperbound
    //};

    enum eCommand { // Client <- Server Command
        eCmd_None           = 0,
        eCmd_Counts         ,
    };

    //----------------------------------------------------------------------------------------------
    // data fields
    union {
        // Client -> Server -> Client Protocol
        struct {
            // Client -> Server : ref(eRequest)
            BYTE request;
            // Client -> Server -> Client : ref(eResult) <- eRequest에 대한 응답이며, 두 값 모두 설정된다.
            BYTE result;
            WORD result_code; // eResult::eRet_~~Failed일 경우 그 세부 에러 코드, 에러 타입에 따라 달리 그룹설정
        };

        // Server Command Protocol
        struct {
            BYTE command;   // ref(eCommand)
            BYTE reason;    // ref(eResult)
            BYTE reason_code; //
        };

        // for Finite state machine
        struct {
            BYTE state; // current state
        };
    };

    // check - in reserved
    // data fields for request
    struct sReq_SummonStart {
        POSTYPE slot_pos;   // must be inventory for summon
    };

    struct sReq_SummonCancel {
    };

    struct sReq_Summon { // for 'Summon | SummonRelease'
        POSTYPE slot_pos;   // must be inventory & equals sReq_SummonStart::slot_pos
    };

    struct sReq_Equip {     // for 'Equip'
        POSTYPE from;       // must be inventory
        eRIDER_PARTS_POS to;// summoned rider equip pos
    };

    struct sReq_UnEquip {   // for 'UnEquip'
        POSTYPE to;         // must be inventory
        eRIDER_PARTS_POS from;// summoned rider equip pos
        POSTYPE where;      // from's rider item : must be inventory
    };

    struct sReq_WaxGloss {
        POSTYPE from_wax;   // Wax item pos
        POSTYPE to_parts;   // Rider parts pos for gloss
    };

    struct sCmd_SyncRider {
        POSTYPE where;      // Need update rider item pos
    };

    // Request arguments
    union { // 3Byte
        sReq_SummonStart    casting_start;
        sReq_SummonCancel   casting_cancel;
        sReq_Summon         summon;
        sReq_Equip          equip;
        sReq_UnEquip        unequip;
        sReq_WaxGloss       wax_gloss;
        sCmd_SyncRider      sync;
    } args;

    void Clear() {
        ZeroMemory(this, sizeof(*this));
    }
}; //end of 'RiderControl'

//--------------------------------------------------------------------------------------------------
struct RiderControlData
{
    enum eDataType {
        eDataType_None        = 0,
        eDataType_Actor       ,
        eDataType_ItemAction  , // target : rider item
        eDataType_Counts //upperbound
    };

    struct sNone { // 1byte
    };

    struct sActor {
        WORD requester; // 요청자 ~~_SYN -> broadcast for action | rendering
    };

    struct sItemAction {
        WORD requester; // 요청자 ~~_SYN -> broadcast for action | rendering
        nsSlot::RiderUpdateBits update_bits; // sub option bits + add option bits
        RENDER_ITEMSTREAMEX item_slot;
    };

    //----------------------------------------------------------------------------------------------
    // data fields
    BYTE data_type; // ref(eDataType)
    union {
        sNone       none;
        sActor      actor;
        sItemAction item_action;
    };

    //----------------------------------------------------------------------------------------------

    void Clear() {
        ZeroMemory(this, sizeof(*this));
    }

    WORD GetSize() const {
        const WORD DefaultDataMaxSize = sizeof(*this) -
                                        //
                                        max(sizeof(none),
                                        max(sizeof(actor),
                                        max(sizeof(item_action),
                                        max(0, 0)))); //위 목록 추가시마다 ')'도 추가할 것

        if(data_type == eDataType_Actor)
            return DefaultDataMaxSize + sizeof(actor);
        if(data_type == eDataType_ItemAction)
            return DefaultDataMaxSize + sizeof(item_action);
        return DefaultDataMaxSize + sizeof(none);
    }
}; //end of 'RiderControlData'

// _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
struct EtheriaControl
{
    //--------------------------------------------------------
    // definitions about a request and a result
    enum eRequest {  // Client -> Server request commands
        eReq_None       = 0,
        eReq_Extract    ,
        eReq_Combine    ,
        eReq_CastingStart,  // this has a different presentation from a complex riding casting
        eReq_CastingCancel, // same as an above comment
        eReq_Counts // upper bound
    };
    enum eResult {  // Client <- Server reply result
        eRet_Success    = 0,
        eRet_Failed     , // reference 'eResultCode'
        eRet_ItemFailed ,
        eRet_Counts // upper bound
    };
    enum eResultCode {
        eRetCode_Success = 0,
        eRetCode_Unknown,
        eRetCode_CastingToggleError, // 캐스팅 중이거나 아니거나의 짝이 맞지 않는다.
        eRetCode_ExtractNeedCasting, // 에테리아 추출은 캐스팅 필요 (내부)
        eRetCode_Counts // upper bound
    };
    static const int kCastingDelay = 5 * 1000;
    //--------------------------------------------------------
    // definitions about a request argument and a result data
    struct Req_Casting {
        POSTYPE slot_pos; // a item wishes to extract
        POSTYPE etherealizer_pos; // an using item to extract, activated by _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_
    };
    struct Req_Extract {
        POSTYPE slot_pos; // a item wishes to extract
        POSTYPE etherealizer_pos; // an using item to extract, activated by _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_
    };
    struct Req_Combine {
        POSTYPE from_etheria; // a etheria item including options
        POSTYPE to_shell;     // a shell item want to combine
    };
    //--------------------------------------------------------
    // data fields about a request
    struct {
        uint8_t  request;    // Client -> Server (ref:eRequest)
        uint8_t  result;     // Client -> Server -> Client (the result about a request)
        uint16_t result_code;
    };
    union {
        Req_Casting casting;
        Req_Extract extract;
        Req_Combine combine;
    } args;
    //
    void Clear() {
        ZeroMemory(this, sizeof(*this));
    }
}; //end of class 'EtheriaControl'

struct EtheriaControlData
{
    enum eDataType {
        eDataType_None  = 0,
        eDataType_Actor, // to support broadcast
        eDataType_Extract,
        eDataType_Combine,
        eDataType_Changed,
        eDataType_Counts // upper bound
    };

    struct None { // 1byte
    };
    struct Actor {
        WORD requester;
    };
    struct Extract {
        RENDER_ITEMSTREAMEX shell_item;
        ITEMSLOTEX etheria_item;
    };
    struct Combine {
        RENDER_ITEMSTREAMEX combined_item;
        // client work to do :
        //   delete the etheria item related to a header.args.combine.from_etheria
    };
    struct Changed { // declared by _NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_
        bool is_etheria; // false=shell, true=etheria
        RENDER_ITEMSTREAMEX changed_item;
    };
    //--------------------------------------------------------
    // data fields
    BYTE data_type; // ref(eDataType)
    union {
        None none;
        Actor actor;
        Extract extract;
        Combine combine;
        Changed changed;
    };
    //
    void Clear() {
        ZeroMemory(this, sizeof(*this));
    }
    uint16_t GetSize() const {
        const uint16_t kDefaultDataMaxSize = sizeof(*this) -
                                             //
                                             max(sizeof(none),
                                             max(sizeof(actor),
                                             max(sizeof(extract),
                                             max(sizeof(combine),
                                             max(sizeof(changed),
                                             max(0, 0))))));
        if (data_type == eDataType_Actor) {
            return kDefaultDataMaxSize + sizeof(actor);
        };
        if (data_type == eDataType_Extract) {
            return kDefaultDataMaxSize + sizeof(extract);
        };
        if (data_type == eDataType_Combine) {
            return kDefaultDataMaxSize + sizeof(combine);
        };
        if (data_type == eDataType_Changed) {
            return kDefaultDataMaxSize + sizeof(changed);
        }
        //data_type == eDataType_None
        return kDefaultDataMaxSize + sizeof(none);
    }
}; //end of class 'EtheriaControlData'

//==================================================================================================

struct PerkControl
{
    //--------------------------------------------------------
    // definitions about a request and a result
    enum eRequest {  // Client -> Server request commands
        eReq_None       = 0,
        eReq_Register, // register to an active slot
        eReq_Delete, // delete a cash perk slot after time expiration
        eReq_Plants, // a plant is an action that a cash item is moved to a cash perk slot
        eReq_Uproot, // an uproot is an action that a cash perk slot is moved to an item on an inventory
        eReq_Status, // simplex command, a game server command a status changed events to client
        eReq_Unregister, // unregister an activated perk
        // operations on server
        eReq_InsertS2S,
        eReq_DeleteS2S,
        //
        eReq_Counts // upper bound
    };
    enum eResult {  // Client <- Server reply result
        eRet_Success    = 0,
        eRet_Failed     , // reference 'eResultCode'
        eRet_SkillFailed , // reference 'RC::eSKILL_RESULT
        eRet_ItemFailed ,  // reference 'RC::eITEM_RESULT
        eRet_Counts // upper bound
    };
    enum eResultCode {
        eRetCode_Success = 0,
        eRetCode_Unknown,
        // this is a single side command by server
        // if a cash perk containing the expiration date is expired.
        eReqCode_TimeExpiration,
        eRetCode_Counts // upper bound
    };
    //
    //
    struct Req_Register {
        ulong perk_uid; // registering perk
        ulong active_slot_uid; // active slot to register about 65001~
    };
    struct Req_Unregister {
        ulong perk_uid; // unregistering perk
    };
    struct Req_Delete {
        ulong perk_uid; // delete a cash perk
    };
    // Action: remove the item -> register a new cash perk
    struct Req_Plants {
        POSTYPE from_item_pos; // the item position of an inventory to a cash item change to a perk 
    };
    // Action: unregister a cash perk -> insert the item changed from a perk
    struct Req_Uproot {
        ulong perk_uid; // uprooting perk
        POSTYPE to_item_pos; // the item position of an inventory to a perk change to a cash item
    };
    struct Req_Status {
        ulong perk_uid; // a perk containing changed status
    };
    struct Req_InsertS2S {
        ulong perk_uid;
    };
    struct Req_DeleteS2S {
        ulong perk_uid;
    };

    //--------------------------------------------------------
    // data fields about a request
    struct {
        uint8_t  request;    // Client -> Server (ref:eRequest)
        uint8_t  result;     // Client -> Server -> Client (the result about a request)
        uint16_t result_code;
        DWORD    char_guid;
    };
    union {
        Req_Register regist;
        Req_Unregister unregist;
        Req_Delete deletion;
        Req_Plants plants;
        Req_Uproot uproot;
        Req_Status status;
        // operations on server
        Req_InsertS2S insert_s2s;
        Req_DeleteS2S delete_s2s;
    } args;
    //
    void Clear() {
        ZeroMemory(this, sizeof(*this));
    }
    WORD GetSize() const {
        return sizeof(*this);
    }
};

struct PerkControlData
{
    enum eDataType {
        eDataType_None  = 0,
        eDataType_Plants,
        eDataType_Uproot,
        eDataType_Status,
        //
        eDataType_Counts // upper bound
    };

    struct None { // 1byte
    };
    struct Plants {
        ulong perk_uid;
        int64_t expiration_date; // f101228.2L, changes
    };
    struct Uproot {
        RENDER_ITEMSTREAMEX maked_item;
    };
    struct Status {
        uint8_t level; // changed level
        EXPTYPE exp;  // changed exp
    };
    //--------------------------------------------------------
    // data fields
    BYTE data_type; // ref(eDataType)
    union {
        None none;
        Plants plants;
        Uproot uproot;
        Status status;
    };
    //
    void Clear() {
        ZeroMemory(this, sizeof(*this));
    }
    uint16_t GetSize() const {
        const uint16_t kDefaultDataMaxSize = sizeof(*this) -
                                             //
                                             max(sizeof(none),
                                             max(sizeof(plants),
                                             max(sizeof(uproot),
                                             max(sizeof(status),
                                             max(0, 0)))));
        if (data_type == eDataType_Plants) {
            return kDefaultDataMaxSize + sizeof(plants);
        }
        if (data_type == eDataType_Uproot) {
            return kDefaultDataMaxSize + sizeof(uproot);
        };
        if (data_type == eDataType_Status) {
            return kDefaultDataMaxSize + sizeof(status);
        }
        //data_type == eDataType_None
        return kDefaultDataMaxSize + sizeof(none);
    }
};

struct PerkStatusData
{
    enum eDataType {
        eDataType_None  = 0,
        eDataType_ExpUpdate,
        //
        eDataType_Counts // upper bound
    };
    //
    struct None { // 1byte
    };

    struct __Status {
        uint8_t index_; // n-th slot in active slots
        uint8_t level_; // snapshot level
        EXPTYPE exp_;   // snapshot experience
    };

    struct StatusList {
        uint8_t number_of_nodes_;
        __Status nodes_[PerkDataList::kNumberOfActiveSlots];
        uint16_t GetSize() const {
            return static_cast<uint16_t>(sizeof(number_of_nodes_) +
                                        (sizeof(nodes_[0]) * number_of_nodes_));
        }
    };
    //
    uint8_t data_type; // ref(eDataType)
    union {
        None none;
        StatusList status_list;
    };
    //
    void Clear() {
        ZeroMemory(this, sizeof(*this));
    };
    uint16_t GetSize() const
    {
        const uint16_t kDefaultDataMaxSize = sizeof(*this) -
                                             //
                                             max(sizeof(none),
                                             max(sizeof(status_list),
                                             max(0, 0)));
        if (data_type == eDataType_ExpUpdate) {
            return kDefaultDataMaxSize + status_list.GetSize();
        }
        //data_type == eDataType_None
        return kDefaultDataMaxSize + sizeof(none);
    }
};

//==================================================================================================
// NOTE: f110315.2L, _NA001955_110210_WING_COSTUME
struct WingControl
{
    //--------------------------------------------------------
    // definitions about a request and a result
    enum eRequest {  // Client -> Server request commands
        eReq_None        = 0,
        eReq_SpreadWings ,
        eReq_FurlWings   ,
        eReq_Counts // upper bound
    };
    enum eResult {  // Client <- Server reply result
        eRet_Success    = 0,
        eRet_Failed     , // reference 'eResultCode'
        eRet_ItemFailed , // RC::RC_ITEM_~~
        eRet_Counts // upper bound
    };
    enum eResultCode {
        eRetCode_Success = 0,
        eRetCode_Unknown,
        eRetCode_NotWingsItem,
        eRetCode_InvalidCharState,
        eRetCode_TimeExpired,
        eRetCode_AlreadySpread,
        eRetCode_AlreadyFurled,
        eRetCode_FurledByHit,
        eRetCode_CantDuplicateAction, // it's occurable that player is riding on a mount.
        // NOTE: f110411.2L, some of riding items enabled flying
        // like a kalbus riding item work in inventory.
        eRetCode_NotToFlyingItem,
        eRetCode_Inactivated, // NOTE: f110521.2L, added by _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
        eRetCode_Counts // upper bound
    };

    //--------------------------------------------------------
    // definitions about a request argument and a result data
    struct Req_SpreadWings {
        POSTYPE wings_pos; // the position of a wings item
        SLOTIDX wings_container; // CHANGES: f110411.2L, the slot index containing wings item
    };
    struct Req_FurlWings {
        POSTYPE wings_pos; // the position of a wings item
        SLOTIDX wings_container; // CHANGES: f110411.2L, the slot index containing wings item
    };
    //--------------------------------------------------------
    // data fields about a request
    struct {
        uint8_t  request;    // Client -> Server (ref:eRequest)
        uint8_t  result;     // Client -> Server -> Client (the result about a request)
        uint16_t result_code;
    };
    union {
        Req_SpreadWings spread;
        Req_FurlWings furl;
    } args;
    //
    void Clear() {
        ZeroMemory(this, sizeof(*this));
    }
}; //end of class 'WingControl'

struct WingControlData
{
    enum eDataType {
        eDataType_None  = 0,
        eDataType_Actor, // to support broadcast
        eDataType_Counts // upper bound
    };

    struct None { // 1byte
    };
    struct Actor {
        uint16_t requester;
    };
    //--------------------------------------------------------
    // data fields
    BYTE data_type; // ref(eDataType)
    union {
        None none;
        Actor actor;
    };
    //
    void Clear() {
        ZeroMemory(this, sizeof(*this));
    }
    uint16_t GetSize() const {
        const uint16_t kDefaultDataMaxSize = sizeof(*this) -
                                             //
                                             max(sizeof(none),
                                             max(sizeof(actor),
                                             max(0, 0)));
        if (data_type == eDataType_Actor) {
            return kDefaultDataMaxSize + sizeof(actor);
        };
        //data_type == eDataType_None
        return kDefaultDataMaxSize + sizeof(none);
    }
}; //end of class 'WingControlData'

//==================================================================================================

//_KR_0_20091021_FIRENDINVITATION_EXTEND //CG_EVENT_INVITATEFRIEND_REWPRD_ACK 필요한 구조체
struct InvitateFriendRewardInfo
{    
    CHARGUID	InvitateFriendGuid;     //보상을 받게 하는 캐릭터 GUID (레벨 달성하는 GUID)    
    WORD        Level;                   //달성 레벨
    TCHAR       CharName[MAX_CHARNAME_LENGTH + 1]; //SendRewardCharGuid 의 캐릭터 명
    BYTE        RewardSts;              //보상 받았는지 여부 (1:받았다, 0:안 받았다.)
};

//==================================================================================================

#ifdef _NA_0_20100819_AGGRO_RENEWAL
struct AggroValueInfo
{    
	char	character_name_[MAX_CHARNAME_LENGTH+1];	//플레이어 캐릭터 이름
	DWORD	aggro_value_;							//어그로 수치
    BYTE    class_type_;
    CODETYPE    name_code_;
};
#endif //_NA_0_20100819_AGGRO_RENEWAL

//_NA_0_20110308_SKILL_COOL_TIME_STORAGE
struct SkillCoolTime
{
    struct CoolTimeInfo
    {
        SLOTCODE    skill_code;
        uint        remain_second;
    };

    uint8_t count;
    CoolTimeInfo info[MAX_SKILL_COUNT];

    uint16_t GetSize() const
    {
        BOOST_STATIC_ASSERT(_countof(info) < UCHAR_MAX);
        const size_t kMinSize = sizeof(*this) - sizeof(info);
        return static_cast<uint16_t>(kMinSize + sizeof(info[0]) * count);
    };
};

struct ItemCompositionOrDecompositionHistory // 아이템 합성 또는 분해 내역
{
    enum { kMaxHistories = 15 }; // 최대 내역 개수
    enum { kTempHistoryID = 0 }; // 임시 내역 ID
    enum 
    { 
        kUndefinedRequest = 0, 
        kZardRequest = 1, 
        kCubeRequest = 2
    };
    enum { kMaxMaterialSize = 9 }; // 최대 재료 개수
    enum { kMaxRewardSize = 9 }; // 최대 보상 개수
    enum { kMaxUsedItemSize = kMaxMaterialSize + kMaxRewardSize }; // 최대 사용 아이템 개수
    typedef ulong HistoryID;
    typedef BYTE RequestType;
    typedef DWORD64 HistoryDateTime;
    typedef BYTE UsedItemListSize;
    struct UsedItem // 합성 또는 분해에 사용된 아이템
    {
        typedef BYTE IGType;

        bool is_material; // 재료 여부 (재료, 보상)
        CODETYPE item_code; // 아이템 코드
        DURATYPE number_of_item; // 아이템 개수
        IGType ig_type; // 아이템 종류 (일반, 디바인, 일반 에테리아, 디바인 에테리아, 외형)

        UsedItem() : is_material(true), item_code(0), number_of_item(0), ig_type(0)
        {
        }
    };
    struct ModifyCondition // 내역을 수정할 조건
    {
        HistoryID history_id; // 수정할 내역 ID
        bool is_memorize; // 내역 기억 여부        

        ModifyCondition() : history_id(kTempHistoryID), is_memorize(false)
        {
        }
    };
    HistoryID history_id; // 합성 또는 분해 내역 ID
    RequestType request_type; // 요청 종류
    bool is_memorized; // 기억 여부
    bool is_composition; // 합성 여부 (합성, 분해)
    HistoryDateTime history_date_time; // 합성 또는 분해가 이루어진 일자 및 시간
    UsedItemListSize used_item_list_size; // 합성 또는 분해에 사용된 아이템 개수
    UsedItem used_item_list[kMaxUsedItemSize]; // 합성 또는 분해에 사용된 아이템 목록
    
    ItemCompositionOrDecompositionHistory() : 
        history_id(kTempHistoryID), 
        request_type(kUndefinedRequest), 
        is_memorized(false), 
        is_composition(true), 
        history_date_time(0), 
        used_item_list_size(0)
    {
    }
    // 사용된 아이템을 추가한다.
    bool AddUsedItem(const UsedItem& used_item)
    {
        if (used_item_list_size == kMaxUsedItemSize)
        {
            return false;
        }
        used_item_list[used_item_list_size] = used_item;
        ++used_item_list_size;
        return true;
    }
};

//------------------------------------------------------------------------------ 
//! 2011.3.28 / i4u4me / 자동 사냥 저장 구조체
struct  AutobotSaveData
{
    // 아이템 줍기
    enum { kPickupItemMax = 8 };

    // 스킬 등록할수 있는 슬롯 갯수
    enum { kSkillSlotCount = 12 };

    struct SkillSlot
    {
        BYTE container_index;   // 원형 (eSlotContainerIndex)
        BYTE slot_type;         // 원형 (eSlotType)
        SLOTCODE slot_code;     //

        // 보조 스킬 설정시에만 사용
        DWORD interval_millisecond;
    };

    //------------------------------------------------------------------------------
    //uint16_t data_version;

    // 아이템 줍기
    bool pickup_item_type[kPickupItemMax];

    // 마을 이동
    BYTE inventory_full_percent;   // 인벤 포화도 이상이면 귀환(원형 float)
    DURATYPE warning_dura;         // 내구도 이하일때 귀환
    NUMTYPE heal_item_num;         // 회복 아이템 갯수 이하이면 귀환
    NUMTYPE feed_item_num;         // 사료 갯수 이하이면 귀환

    // 회복 설정
    BYTE hp_potion_percent;         // hp 이하이면 물약 사용(원형 float)
    BYTE immediate_potion_percent;  // hp 이하이면 즉시 회복 물약 사용(원형 float)
    BYTE mp_potion_percent;         // mp 이하이면 포션 사용(원형 float)
    BYTE feed_potion_percent;       // 펫 hp 이하이면 사료 사용(원형 float)
    BYTE relax_hp_percent;          // hp 이하이면 앉아서 휴식(원형 float)
    BYTE relax_mp_percent;          // mp 이하이면 앉아서 휴식(원형 float)

    // 보조 스킬 설정
    SkillSlot   interval_skill[kSkillSlotCount];

    // 공격 스킬 설정
    SkillSlot   action_skill[kSkillSlotCount];

    // 자동부활
    BYTE revival_type;              // 원형(RevivalType)

    // 자동 구매
    bool    sell_item_type[kPickupItemMax];
    NUMTYPE buy_recall_item_count;  // 귀환스클롤
    NUMTYPE buy_feed_item_count;    // 일반 사료
    BYTE    buy_hp_item_level;      // 회복 아이템 구매 레벨
    NUMTYPE buy_hp_item_count;      // 회복 아이템 구매 레벨
    bool    repair;                 // 수리

    // 사냥 설정
    BYTE hunting_type_;             // 사냥 종류(원형:HuntingType)
    bool is_boss_avoid;             // 보스 발견시 도망
    BYTE area_range;                // 원형(float)
    
    // 커뮤니티
    BYTE channel_level;             // 콤보 박스 index를 저장한다.  //TCHAR channel_level[MAX_ROOMTITLE_LENGTH];
    BYTE channel_name;              // 콤보 박스 index를 저장한다.  //TCHAR channel_name_[MAX_ROOMTITLE_LENGTH];
    BYTE shouting_type;             // (ShoutingType)
    int shouting_second;            //
    TCHAR shouting_message[MAX_CHAT_LEN];
    BYTE party_type;                // 파티수락(PartyType)
    bool save_chat_message;         // 채팅저장
};

#ifdef _NA_0_20110329_GAMEOPTION_SETTING
struct GameOption
{
    enum Type
    {
        lower_boundary = 0, 
        //
        kAutoHunting = 0,
        kKeyCustomizing = 1,// 계정 단위 단축키세팅
        kGuidePerUser = 2, // 계정 단위 사용자 가이드
        kGuidePerChar = 3, // 캐릭터 단위 사용자 가이드
        kNoticePerChar = 4, // 캐릭터 단위 알림창
        kKeyCustomizingPerChar = 5,// 캐릭터 단위 단축키세팅
        kLastSelectedChar = 6, // 마지막으로 선택된 캐릭터
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        kPerkListChar = 7, // Perk 셋팅 관련
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
        //
        max_type_count // upper boundary
    };

    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(binary_data_);
        return static_cast<uint16_t>(kMinSize + sizeof(binary_data_[0]) * binary_data_size_);
    }

    void SetValue(const uchar* data, uint size, GameOption::Type type)
    {
        assert(size >= 0 && size <= sizeof(binary_data_));
        option_type_ = type;
        binary_data_size_ = size;
        if (size && data) {
            CopyMemory(binary_data_, data, size);
        };
    }

    GameOption::Type  option_type_;
    uint        binary_data_size_;
    uchar       binary_data_[MAX_GAMEOPTIONSTREAM_SIZE];
};
#endif 


//#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL

// 스크립트 버젼 관리 정보
struct SalesZoneScriptVersion
{
    SalesZoneScriptVersion()
    : sales_zone_(0)
    , year_(0)
    , year_identity_(0)
    {
    }
    bool IsValid()
    {
        return (sales_zone_ && year_ && year_identity_) ? true : false;
    }
    bool IsEqual(const SalesZoneScriptVersion& version)
    {
        if (sales_zone_ == version.sales_zone_ && year_ == version.year_ && 
            year_identity_ == version.year_identity_)
            return true;
        return false;
    }
    ushort sales_zone_;     
    ushort year_;
    ushort year_identity_;
};

// 배너 스크립트 버젼.
struct BannerZoneScriptVersion
{
    ushort banner_zone_;     
    ushort year_;
    ushort year_identity_;
};

struct ShopScriptVersion
{
    SalesZoneScriptVersion  sales_zone_;
    BannerZoneScriptVersion banner_zone_;
};

struct CashInfo
{
    double cash_;     // default cash
    double pp_cash_;  // pp card cash (global)  
    double mileage_;  // mileage service (china)
};

struct ShopItemInfo
{
    long package_product_sequence_;  // 패키지 번호
    long product_display_sequence_;  // 카테고리 번호
    long price_sequence_;            // 가격 번호
    uchar cash_type_;                // 0:캐시, 1:PP, 2:마일리지.
};

struct UseStorageItem
{
    long storage_sequence_;         // 보관함 순번
    long storage_item_sequence_;    // 보관함 상품 시퀀스
    char item_type_;                // 아이템 타입. 보관함 내의 ('C' : 캐시, 'P' : 상품)
};

struct ShopItemInfoEx : public ShopItemInfo
{
    long product_left_count_;       // 상품 판매 가능 잔여 수량 (-1 : 무한대)
    int error_code_;
};

struct StorageItemInfo
{
    long storage_sequence_;                         // 보관함 순번
    long storage_item_sequence_;				    // 보관함 상품 순번 or 캐시 항목 순번
    long product_sequence_;					        // 단위 상품 코드
    long price_sequence_;                           // 가격 번호
    char storage_item_type_;					    // 상품 캐시 구분 (P:상품, C:캐시)
    uint delete_date_;                              // 상품 삭제 날짜.
};

struct StorageItemInfoBuy : public StorageItemInfo
{
    char buy_character_name_[MAX_CHARNAME_LENGTH+1];
};

struct StorageItemInfoGift : public StorageItemInfo
{
    char send_character_name_[MAX_CHARNAME_LENGTH+1];
    char recv_character_name_[MAX_CHARNAME_LENGTH+1];
    char gift_message_[GIFT_MESSAGE_LENGTH];
};


//#endif //_NA_0_20110613_GAME_SHOP_RENEWAL

//==================================================================================================
//_NA000000_SPEED_HACK_PREVENTION_SUPPORT
#pragma pack(push)
#pragma pack()
namespace nsSync {
;

struct KBMoveSerialNode
{
    static const int kNumberOfMoveKeys = 18;
    static const int kNumberOfStopKeys = 6;
    //
    void Create();
    //
    uint16_t serial_guid;
    uint8_t process_index;
    uint8_t process_stop_index;
    ulong serial_key_begin;
    ulong stop_serial_key_begin;
};

}; //end of namespace
#pragma pack(pop)
//==================================================================================================


//==================================================================================================

#ifdef _NA_003027_20111013_HONOR_SYSTEM
struct HONOR_TITLE
{
    WORD title_index_;
};

struct HonorTitleTable
{
    WORD count_;
    HONOR_TITLE title_[MAX_HONORTITLE_COUNT];
    // CHANGES: f110829.4L
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(title_);
        return static_cast<uint16_t>(kMinSize + sizeof(title_[0]) * count_);
    };
};

struct HONOR_POINT_INFO
{
    enum { FAME = 1, REPUTE = 2 };
    BYTE type;              // type = 1 :명성 / 2 = 평판..
    WORD honor_point_code_; // 명성/평판 코드
    DWORD honor_point_;     // 해당 명성/평판의 점수(누적)
};

struct HonorPointTable
{
    WORD count_;
    HONOR_POINT_INFO point_table_[MAX_HONOR_POINT_INFO_COUNT]; // 명성/평판 점수
    // CHANGES: f110829.4L
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(point_table_);
        return static_cast<uint16_t>(kMinSize + sizeof(point_table_[0]) * count_);
    };
};

#define MAX_TODAYQUEST_COUNT    (200) // for 1302 - arycoat 2013.05.23. 

struct QUEST_TODAY_QUEST_INFO
{
    WORD quest_group;
    WORD quest_code;
};
#endif //_NA_003027_20111013_HONOR_SYSTEM

//!~ _NA004034_20120102_GUILD_POINT_SYSTEM
struct GUILD_RANKING_INFO
{
    enum { MAX_GUILD_RANKING_INFO = 50, };

    DWORD guild_guid;
    TCHAR guild_name[MAX_GUILDNAME_LENGTH+1];
    TCHAR master_name[MAX_CHARNAME_LENGTH+1];
    DWORD guild_level;
    DWORD domination_count;
    DWORD guild_point;
    DWORD member_count;
};

struct GUILD_INTRODUCTIONS
{
    WORD introduction_length_;
    CHAR introduction_strings_[MAX_GUILD_INTRODUCTION_LENGTH+1];
    
    GUILD_INTRODUCTIONS()
    {
        introduction_length_ = 0;
        memset(&introduction_strings_, 0, sizeof(introduction_strings_));
    }
};
//~! _NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
struct SUN_RANKING_GRADE_INFO
{
    SUN_RANKING_GRADE_INFO()
    {
        m_dwObjectKey = 0;
        ranking_type = eSUN_RANKING_TYPE_END;
        grade = eSUN_RANKING_GRADE_NONE;
    }

    eSUN_RANKING_TYPE ranking_type;
    eSUN_RANKING_GRADE_TYPE grade;
    DWORD m_dwObjectKey;

    uint16_t GetSize() const {
        return sizeof(*this);
    }
};
struct StatueInfo
{
    eSTATUE_TYPE  statue_type;
    eSTATUE_INDEX statue_index;

    CLIENT_CHARACTER_PART client_character_part;
    int                   pattern_mark_index;
    int                   background_mark_index;
    TCHAR				  guild_name[MAX_GUILDNAME_LENGTH+1];
    SCORE                 total_equip_score;

    void clear()
    {
        memset(this, 0, sizeof(*this));
    }
};
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#pragma warning(pop)
#pragma pack(pop)
//==================================================================================================




#endif // __STRUCTINPACKET_H__
