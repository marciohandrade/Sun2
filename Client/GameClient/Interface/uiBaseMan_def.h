#ifndef     __UIBASEMAN_DEFINE_H__
#define     __UIBASEMAN_DEFINE_H__

#include "ItemUnitRenderer.h"

#pragma warning(disable : 4995)

//------------------------------------------------------------------------------
// user interface enumeration
//
enum
{
    UIMAN_RANDOM=0,

    // login
    UIMAN_LOGIN,

    // party
    UIMAN_PARTY,

    // battlezone
    UIMAN_BATTLEZONE,

    // guild
    UIMAN_GUILD,

    // skill
    UIMAN_SKILL,

    // trade
    UIMAN_TRADE,

    // vendor
    UIMAN_VENDOR,

    // system
    UIMAN_SYSTEM,

	// char popup
	UIMAN_CHAR_POPUP,

	// help 
	UIMAN_HELP,

	UIMAN_ITEM_COMPOSITE,

	UIMAN_QUEST,

	UIMAN_EVENT,

#ifdef _KIKI_UI_WEB_DIALOG
	UIMAN_WEB,
#endif

    UIMAN_ITEM_DIV,

	UIMAN_WAYPOINT,

	UIMAN_PCBANG_EVENT_TIME,
	UIMAN_RADAR,

#ifdef _NA_000000_20130114_RENEWER_UI
#else
	UIMAN_OPTIONMENU,		
#endif

	UIMAN_CHAT,
	UIMAN_INVENTORY,
	UIMAN_ASSIST,
	UIMAN_MENU,
	UIMAN_QUICKCONTAINER,
	UIMAN_HEROGAUGE,		
	UIMAN_BANK,
	UIMAN_CREATECHARACTER,
	UIMAN_TARGET,
	UIMAN_SUMMONTARGET,
	UIMAN_REWARD,
	UIMAN_ENCHANT,
	UIMAN_COMPOSITE,
	UIMAN_SUMMONSKILL,
	UIMAN_CAST,	
	UIMAN_BATTLELOBBY,		
	UIMAN_SKILLQUEUE,
	UIMAN_NPCPORTAL,

	UIMAN_TIREDSYSTEM,
	UIMAN_AREACONQUEST,
	UIMAN_ENDURANCE,	
	UIMAN_SHOP,
	UIMAN_CHARACTERSELECT,
	UIMAN_CHARACTERSTATUS,
	UIMAN_TOOLTIP,

	UIMAN_MISSION_RANK,

	UIMAN_CHAO,

	UIMAN_COMMUNITY,

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    //! 2011.7.22 / i4u4me / 캐시샵 작업 

#else
	UIMAN_CASH_ITEM_SHOP,
#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


	UIMAN_SOCKET_REMOVE,
	UIMAN_PASSWORD,


#ifdef __CN_269_MATRIX_CARD_AUTH
	UIMAN_MATRIX_CARD,
#endif//__CN_269_MATRIX_CARD_AUTH

	UIMAN_MISSION_RELAY,

	UIMAN_FATE_ITEM_SYSTEM,


#ifdef _YJ_FISHING_SYSTEM_000933
	UIMAN_FISHING_SYSTEM,
#endif //_YJ_FISHING_SYSTEM_000933

	UIMAN_TOOLTIP2,

	UIMAN_ITEM_STAT_CHANGE,

	UIMAN_SSQ_SYSTEM,
	UIMAN_PERSONAL_WAYPOINT,
	UIMAN_PET_MAN,

	UIMAN_LOTTO_MAN,
	UIMAN_NOTICE,
	UIMAN_DOMINATIONWAR_TOWER,

	UIMAN_BATTLE_SCORE,

    UIMAN_CHAR_CUSTOMIZE,
    UIMAN_MISSION_OBJECT,

#ifdef _YMS_AUTOBOT_GAME
    UIMAN_AUTOBOT,  //! 2010.8.18 / i4u4me / 오토툴 
#endif //_YMS_AUTOBOT_GAME

    UIMAN_NEW_SOCKET, //신규 소켓관련(소켓쟈드합성,...)

#ifdef _DEV_VER
    UIMAN_GM_WINDOW,   // GM 윈도우
#endif//DEV_VER //_DH_GM_WINDOW

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    UIMAN_CONVENIENCESTORE, //! 2011.6.13 / i4u4me / 캐시샵 리뉴얼 
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    UIMAN_GUIDE_SYSTEM, // 정보 열람실

#ifdef _DH_BATTLEZONE2_
    UIMAN_BATTLE_ZONE_MANAGER, // 배틀존 2 (2011.07버전) by elflee
#endif//_DH_BATTLEZONE2_

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    UIMAN_CHAOS,    //전장
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

    UIMAN_OPTION2,

#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
	UIMAN_TEXTURE_VIEWER_MANAGER, // 20130312 Texture_Viewer_Manager 홍성현
#endif
#endif

#ifdef _NA_007086_20140318_MONSTERBOOK
    UIMAN_MONSTERBOOK,    // 몬스터도감
#endif//_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    UIMAN_FPGAUGE,        // FP게이지
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    UIMAN_ATTENDANCE_CHECK, // 출석체크
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
	UIMAN_SUNRANKING,     // SUNRanking
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	UIMAN_EQUIPMENT_WAKEUP, // 장비 각성
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	UIMAN_EQUIPMENT_EVOLUTION, // 장비 진화
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
	UIMAN_NOTIFICATIONS_SYSTEM, // 알림 메시지
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
	UIMAN_MINIGAME, 
#endif // _NA_008405_20150716_MINIGAME_BINGO

	UIMAN_MAX,
};

//------------------------------------------------------------------------------
// class prototypes
//
class InterfaceManager;
class uiBase;
class uiBaseMan;

// LOGIN
class uiLoginMan;

// PARTY
class uiPartyMan;

// BATTLEZONE
class uicRealyInfo;
class uiBattleZoneMan;

// GUILD
class uiGuildMan;

// SKILL
class uiSkillMan;

// TRADE
class uiTradeMan;


// VENDOR
class uiVendorMan;

// SYSTEM
class uiSystemMan;

class uiEventMan;

#ifdef _KIKI_UI_WEB_DIALOG
class uiWebDialogMan;
#endif

class uiCharPopupMan;
class uiHelpMan;
class SkillScriptInfo;
class uiWayPointMan;
class uiPersonalWayPointMan;
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
class uiMissionRankMan2;
#else
class uiMissionRankMan;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
class uiCommunityMan;
class uiCashItemShopMan;
class uiPasswordMan;


#ifdef __CN_269_MATRIX_CARD_AUTH
    class uiMatrixCardMan;
#endif//__CN_269_MATRIX_CARD_AUTH

class uiFateItemSystemMan;

#ifdef _YJ_FISHING_SYSTEM_000933
    class uiFishingMan;
#endif //_YJ_FISHING_SYSTEM_000933

//------------------------------------------------------------------------------

class uicRelayInfo
{
public:
    uicRelayInfo() {}
    ~uicRelayInfo() {}
    uicRelayInfo(const uicRelayInfo& src);
    uicRelayInfo& operator=(const uicRelayInfo& rhs);

    KEYTYPE			    m_HuntingKey;		//< 참가할 헌팅 키
    CODETYPE			m_MapCode;			//< 생성할 맵 정보
    BASE_ROOMINFO		m_RoomInfo;			//< 생성할 방 정보
    ADDITIONAL_ROOMINFO	m_AdditionalInfo;	//< 생성할 부가 정보
    BYTE		        m_FieldInUseClient;
    BYTE				m_byRoomPublic;							//< eZONEPUBLIC
    TCHAR				m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH];
};

//------------------------------------------------------------------------------
/**
*/
inline
uicRelayInfo::uicRelayInfo(const uicRelayInfo& src) :
    m_HuntingKey (src.m_HuntingKey),
    m_MapCode (src.m_MapCode),
    m_byRoomPublic (src.m_byRoomPublic),
    m_FieldInUseClient (src.m_FieldInUseClient)
{
    StrCopy (this->m_pszRoomPWD, src.m_pszRoomPWD);
    memcpy ((BASE_ROOMINFO *)&m_RoomInfo,
        (BASE_ROOMINFO *)&src.m_RoomInfo,
        sizeof (BASE_ROOMINFO));
    memcpy ((ADDITIONAL_ROOMINFO *)&m_AdditionalInfo,
        (ADDITIONAL_ROOMINFO *)&src.m_AdditionalInfo,
        sizeof (ADDITIONAL_ROOMINFO));
}

//------------------------------------------------------------------------------
/**
*/
inline
uicRelayInfo&
uicRelayInfo::operator=(const uicRelayInfo& rhs)
{
    StrCopy (this->m_pszRoomPWD, rhs.m_pszRoomPWD);
    memcpy ((BASE_ROOMINFO *)&m_RoomInfo,
        (BASE_ROOMINFO *)&rhs.m_RoomInfo,
        sizeof (BASE_ROOMINFO));
    memcpy ((ADDITIONAL_ROOMINFO *)&m_AdditionalInfo,
        (ADDITIONAL_ROOMINFO *)&rhs.m_AdditionalInfo,
        sizeof (ADDITIONAL_ROOMINFO));

    this->m_HuntingKey = rhs.m_HuntingKey;
    this->m_MapCode = rhs.m_MapCode;
    this->m_byRoomPublic = rhs.m_byRoomPublic;
    this->m_FieldInUseClient = rhs.m_FieldInUseClient;

    return *this;
}

//------------------------------------------------------------------------------
/**
    스킬 정보
*/
class uicSkillInfo
{
public:
    uicSkillInfo() { Clear(); }

    uicSkillInfo(const uicSkillInfo& src);
    uicSkillInfo& operator=(const uicSkillInfo& rhs);

    void Clear()
    {
        m_pCurrentSkillInfo = NULL;
        m_pNextSkillInfo = NULL;
        m_State = SLOT_RENDER_STATE_NONE;
        m_BlockState = SLOT_RENDER_STATE_NONE;
        m_bEnableLevelup = false;
        m_TabIndex = 1;
        m_IsMaxLevel = false;
        m_IsActive = false;
    }

    SkillScriptInfo*    m_pCurrentSkillInfo;      // 현재 레벨 스킬 정보
    SkillScriptInfo*    m_pNextSkillInfo;      // 다음 레벨 스킬 정보
    int                 m_SlotIndex;            // GetSlot (iSlotNo)
    eSLOT_RENDER_STATE  m_State;              // 활성화 상태
	eSLOT_RENDER_STATE  m_BlockState;         // 블록 상태    
    bool                m_bEnableLevelup;      // 레벨업 가능
    int                 m_TabIndex;                // 텝
    bool                m_IsMaxLevel;       // 최대레벨
    bool                m_IsActive;          // 활성화여부 (습득 여부)
};

//------------------------------------------------------------------------------
/**
*/
inline
uicSkillInfo::uicSkillInfo(const uicSkillInfo& src)
{
    this->m_pCurrentSkillInfo = src.m_pCurrentSkillInfo;
    this->m_pNextSkillInfo = src.m_pNextSkillInfo;
    this->m_SlotIndex = src.m_SlotIndex;
    this->m_State = src.m_State;
    this->m_bEnableLevelup = src.m_bEnableLevelup;
    this->m_TabIndex = src.m_TabIndex;
	this->m_BlockState=src.m_BlockState;
    this->m_IsMaxLevel = src.m_IsMaxLevel;
    this->m_IsActive = src.m_IsActive;

}

//------------------------------------------------------------------------------
/**
*/
inline
uicSkillInfo&
uicSkillInfo::operator=(const uicSkillInfo& rhs)
{
    this->m_pCurrentSkillInfo = rhs.m_pCurrentSkillInfo;
    this->m_pNextSkillInfo = rhs.m_pNextSkillInfo;
    this->m_SlotIndex = rhs.m_SlotIndex;
    this->m_State = rhs.m_State;
    this->m_bEnableLevelup = rhs.m_bEnableLevelup;
    this->m_TabIndex = rhs.m_TabIndex;
	this->m_BlockState=rhs.m_BlockState;
    this->m_IsMaxLevel = rhs.m_IsMaxLevel;
    this->m_IsActive = rhs.m_IsActive;

    return *this;
}

//------------------------------------------------------------------------------
/**
    스타일 정보
*/
class uicStyleInfo
{
public:
    uicStyleInfo() { Clear(); }

    uicStyleInfo(const uicStyleInfo& src)
    {
        this->m_pCurrentStyleInfo = src.m_pCurrentStyleInfo;
        this->m_pNextStyleInfo = src.m_pNextStyleInfo;
        this->m_SlotIndex = src.m_SlotIndex;
        this->m_State = src.m_State;
        this->m_bEnableLevelup = src.m_bEnableLevelup;
    }

    uicStyleInfo& operator=(const uicStyleInfo& rhs)
    {
        this->m_pCurrentStyleInfo = rhs.m_pCurrentStyleInfo;
        this->m_pNextStyleInfo = rhs.m_pNextStyleInfo;
        this->m_SlotIndex = rhs.m_SlotIndex;
        this->m_State = rhs.m_State;
        this->m_bEnableLevelup = rhs.m_bEnableLevelup;

        return *this;
    }

    void Clear()
    {
        m_pCurrentStyleInfo = NULL;
        m_pNextStyleInfo = NULL;
        m_State = SLOT_RENDER_STATE_NONE;
        m_bEnableLevelup = false;
    }

    BASE_STYLEINFO*     m_pCurrentStyleInfo;      // 현재 레벨 스타일 정보
    BASE_STYLEINFO*     m_pNextStyleInfo;      // 다음 레벨 스타일 정보
    int                 m_SlotIndex;            // GetSlot (iSlotNo)
    eSLOT_RENDER_STATE  m_State;              // 활성화 상태
    bool                m_bEnableLevelup;      // 레벨업 가능
};

//------------------------------------------------------------------------------
/**
    패킷 관리 정보
*/
class uicPacketControl
{
public:
    uicPacketControl() { clear(); }

    uicPacketControl(const uicPacketControl& src)
    {
        m_second = src.m_second;
        m_working = src.m_working;
    }
    uicPacketControl& operator=(const uicPacketControl& rhs)
    {
        m_second = rhs.m_second;
        m_working = rhs.m_working;

        return *this;
    }

    //------------------------------------------------------------------------------
    /**
    */
    void clear()
    {
        m_working = false;
        m_second  = 0;
    }

    //------------------------------------------------------------------------------
    bool timeOut()
    {
        if ( m_second &&
            (m_second < clock_function::GetTickCount()) )
            return true;

        return false;
    }

    //------------------------------------------------------------------------------
    /**
    */
    bool getWorking()
    {
        return m_working;
    }

    //------------------------------------------------------------------------------
    /**
    */
    void setWorking(DWORD sec=0)
    {
        if (sec>=1)
        {
            m_second = (clock_function::GetTickCount() + (1000 * sec));
        }
        else
        {
            m_second = 0;
        }

        m_working = true;
    }

    //------------------------------------------------------------------------------
    /**
    */
    DWORD    getSecond() { return m_second; }

private:
    DWORD   m_second;
    bool    m_working;
};


//------------------------------------------------------------------------------
#endif  //  __UIBASEMAN_DEFINE_H__
