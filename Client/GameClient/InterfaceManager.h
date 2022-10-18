
/**

인터페이스 모듈 랩핑 및 다이얼로그 리스트 관리 클래스

@author
-Kim Min Wook < taiyo@webzen.co.kr >

@since

@remarks
- 소스 추가 / 정리(재영)
- InterfaceWndProc()를 메인 윈도 메시지 프로시저에서 호출되어야 한다.
- BOOL Init(), VOID Release(), VOID Render(), VOID Update()
- 

@TODO
- 
*/

#pragma once

#include <SolarHashTable.h>
#include <SCSlotHeader.h>
#include "IConMgr.h"
#include "SCSlot.h"
#include "Object.h"
#include "StringTableInfo.h"
#include "Pet.h"
#include "Rider.h"
#include "Player.h"

using namespace util;

class SCSlotContainer;
class CDrawBase;
class CDialogManager;
class SolarDialog;
struct MSG_BASE;
class MonsterGauge;
class SkillScriptInfo;
class Clone;
class SCItemSlot;
class PlayerAttributes;
class Rider;

class StatusIconManager;
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
class DiceCastManager;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
class DamageMesh;

//------------------------------------------------------------------------------
// User Interface
#include "uiBase.h"
#include "uiBaseMan.h"

//------------------------------------------------------------------------------

extern const WzColor		WzEliteNameColor;
extern const WzColor		WzUniqueNameColor;
extern const WzColor		WzLimitedItemColor;


typedef list<SKILL_EFFECT_IMAGE> SKILL_EFFECT_IMAGE_LIST;
typedef vector<DAMAGE_POPUP> DAMAGE_LIST;
typedef SolarHashTable<SolarDialog*>::iterator DIALOG_HASHTABLE_ITERATOR;


typedef void (*fn_Password)( bool bYes );

enum eIMG_TYPE
{
	eIMG_NUMBER,
	eIMG_SLASH,
	eIMG_PERCENT,
	eIMG_DOT,
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	eIMG_STAR,
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
};

enum eIMAGE_NUMBER_TYPE
{
    eIMAGE_NUMBER_DEFAULT_YELLOW = 0,
    eIMAGE_NUMBER_LARGE_YELLOW = 1,
    eIMAGE_NUMBER_WHITE = 2,
    eIMAGE_NUMBER_RED = 3,
    eIMAGE_NUMBER_YELLOW = 4,
    eIMAGE_NUMBER_GREEN = 5,
    eIMAGE_NUMBER_BLUE = 6,
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	eIMAGE_NUMBER_ORANGE = 7,
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
};


struct FPOINT
{
    float x;
    float y;
    FPOINT() : x(0), y(0){}
};

typedef struct Temp_Render_Struct
{
    WzRenderInfo* proto_render_info_ptr;
    WzColor render_color_[NUM_WRIC];   // 그리는 색상(Diffuse, Specular, Ambient)
}stTemp_RenderInfo;

#include "GameMessage.h"

//------------------------------------------------------------------------------ 
class InterfaceManager
    : public GameMessage
{
public:
	enum eITEM_TYPE
	{
		eITEM_WEAPON = 0,
		eITEM_ARMOR,
		eITEM_ACCESSORY,
		eITEM_TYPEMAX,
	};

    enum eITEM_MARK_TYPE
    {
        MARK_TYPE_START = 0,

        MARK_TYPE_QUEST = MARK_TYPE_START,
        MARK_TYPE_QUEST_START,
        MARK_TYPE_AC,

        MARK_TYPE_CASH_ITEM,
        MARK_TYPE_UNIQUE,
        MARK_TYPE_ELITE,
        MARK_TYPE_DEVINE,
        MARK_TYPE_RARE,
        
        MARK_TYPE_MAX
    };

    enum eBATTLE_ICON_TYPE
    {
        eBATTLE_ICON_PCBANG_SUPERMASTER_ROOM,
        eBATTLE_ICON_ITEM_SUPERMASTER_ROOM,
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        eBATTLE_ICON_GUILD_PVP_ROOM,
#endif 
#ifdef _DH_BATTLEZONE2_
        eBATTLE_ICON_LOCKED_ROOM,
#endif//_DH_BATTLEZONE2_
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        eBATTLE_ICON_PARTY_OBTAIN_ROOM,  // 아이템 모두분배 배틀존
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        eBATTLE_ICON_MAX,
    };

	enum eDIALOG_ID
	{
        DIALOG_LOGIN=0,
        DIALOG_SERVER_SELECTION,
		DIALOG_EQUIP,
		DIALOG_INVENTORY,
		DIALOG_EVENT_INVENTORY,
		DIALOG_CHAT,
		DIALOG_CHATMENU,
		DIALOG_QUICK,
		DIALOG_RADAR,
		DIALOG_RADAR_PLUS,
		DIALOG_RADAR_FULL,
		DIALOG_HEROGAUGE,
		//./DIALOG_HEROGAUGECONTROL,
		DIALOG_MAIN,
		DIALOG_MAIN_OPTION,
        // from uiBase
        DIALOG_LOGIN_BACK,
        DIALOG_LOGIN_ACCOUNT,
        DIALOG_LOGIN_NOTICE,
        DIALOG_LOGIN_SERVER_LIST,
        DIALOG_LOGIN_CHANNEL_LIST,
		DIALOG_LOGIN_15YEAR,
        DIALOG_TRADE,
#ifdef	_K2P_SOFTWARE_NUMBER_KEYBOARD_000913
		DIALOG_TRADE_NUMBER_KEYBOARD,
#endif

#ifndef _NA_000000_20130114_RENEWER_UI
        DIALOG_OPTION_MENU,
        DIALOG_OPTION_SYSTEM,
		DIALOG_OPTION_GAME,
		DIALOG_OPTION_KEYHELP,
		DIALOG_OPTION_SOUND,
#else
        DIALOG_OPTION,
#endif
        DIALOG_OPTION_KEYBINDING,

		DIALOG_VENDOR_SELL,
        DIALOG_VENDOR_BUY,
        DIALOG_VENDOR_POPUP,
        DIALOG_VENDOR_TITLE_POPUP,
        DIALOG_BATTLEZONE_LIST,
        DIALOG_BATTLEZONE_CREATE,
        DIALOG_BATTLEZONE_PASSWORD,
        DIALOG_BATTLEZONE_RELAY,
        DIALOG_BATTLEZONE_READY_MISSIONCFG,

        DIALOG_BATTLEZONE_READY_HUNTINGCFG,
        DIALOG_BATTLEZONE_READY_CHUNTINGCFG,



		DIALOG_BATTLEZONE_CHUNTING_WIN,


        DIALOG_BATTLEZONE_CHUNTING_SCORE_BOARD,



		DIALOG_BATTLEZONE_CHUNTING_DOING,


        DIALOG_PARTY_LEADER,
        DIALOG_PARTY_MEMBER1,
        DIALOG_PARTY_MEMBER2,
        DIALOG_PARTY_MEMBER3,
        DIALOG_PARTY_MEMBER4,
        DIALOG_PARTY_MEMBER5,
        DIALOG_PARTY_MEMBER6,
        DIALOG_PARTY_MEMBER7,
        DIALOG_PARTY_MEMBER8,
        DIALOG_PARTY_MEMBER9,
        DIALOG_PARTY_OPTION,

        DIALOG_GUILD_CREATE,
        DIALOG_GUILD_WINDOW,
        DIALOG_GUILD_MENU,
        DIALOG_GUILD_MAIN,
        DIALOG_GUILD_INPUTMESSAGE,

        DIALOG_SYSTEM_VERIFY,
        DIALOG_SYSTEM_CONFIRM,
		DIALOG_SYSTEM_IDNUMBER_CONFIRM,
		DIALOG_SYSTEM_INPUTMSG_CONFIRM,
		DIALOG_SEARCH_ID,
		DIALOG_INPUT_PASSWD,
        DIALOG_CHARSELECT,	
		DIALOG_FONT_CHARSELECT,	
		DIALOG_SHOP,
		DIALOG_INPUT_NUM,
		DIALOG_BANK,
		DIALOG_CHAR_STATUS,
        DIALOG_SYSTEM,
        DIALOG_HELP,
		DIALOG_MISSION_SELECTION,
		DIALOG_CREATE_CHARACTER,
        DIALOG_ASK,          //확인,취소 지완 추가  
		DIALOG_WARNING,
		DIALOG_TARGET,
		DIALOG_SUMMON_TARGET,

//		DIALOG_TOOLTIP,

		DIALOG_REWARD,
		DIALOG_ENCHANT,
		DIALOG_RANKDOWN,

		DIALOG_SELECT_COMPOSITE_MENU,
		DIALOG_STYLE_CONTAINER,

		DIALOG_SUMMON_SKILL_CONTAINER,
		DIALOG_DELAY_CASTING,		// 현재는 마을로 돌아갈때 사용되는 캐스팅 창이다.
		DIALOG_CASTING,
		DIALOG_BATTLE_LOBBY,
		DIALOG_CREATECHARACTER_LEFT,
		DIALOG_CREATECHARACTER_BOTTOM,		
		DIALOG_CREATECHARACTER_SELECT,	
		DIALOG_CREATECHARACTER_ROTATE,

		DIALOG_MAGAZINE,
		DIALOG_PVP_SCORE,
		DIALOG_SKILL_QUEUE,
		
		DIALOG_NPC_PORTAL,
		DIALOG_BATTLE_CONFIG,

		DIALOG_CHAR_POPUP_MENU,

		DIALOG_HELP_MENU,
		DIALOG_HELP_WINDOW,
		DIALOG_HELP_ANNOUNCE_WINDOW,

		DIALOG_ITEM_COMPOSITE,

		DIALOG_ITEM_ETC_COMPOSE,

		DIALOG_TIREDSYSTEM,

		DIALOG_VOICE_CHAT,

		DIALOG_AREA_CONQUEST,

		DIALOG_QUEST_COMMON_FORWARD,
		DIALOG_QUEST_REWARD,
		DIALOG_QUEST_LIST_MISSION,
		DIALOG_QUEST_LIST_CONTENT,
		DIALOG_QUEST_MISSION_LIST,
		DIALOG_QUEST_TIME_INFO,
		DIALOG_QUEST_AUTO_ACEEPT_ALRAM,

		DIALOG_EVENT_WINDOW,

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
		DIALOG_GUILD_EVENT_WINDOW,
#endif
		DIALOG_EVENT_ITEM_SELECT,

		DIALOG_EVENT_X_MAS_CONFIRM,
		DIALOG_EVENT_X_MAS_SANTADLG,
		DIALOG_EVENT_X_MAS_DONATION_CONFIRM,
		DIALOG_EVENT_X_MAS_DONATION_REWARD,
		DIALOG_EVENT_X_MAS_SOLOCOUPLE_REWARD,
		DIALOG_EVENT_X_MAS_SANTA_REWARD,

#ifdef _KIKI_UI_WEB_DIALOG
		DIALOG_WEB_DIALOG,
#endif

        DIALOG_ITEM_DIV,
		DIALOG_ENDURANCE,
		DIALOG_ITEM_TOTAL_COMPOSITE,

		DIALOG_SYSTEM_CHAT,
		DIALOG_WAYPOINT,
		

		DIALOG_PCBANG_EVENT_TIME,

		DIALOG_MISSION_RANK_TIMER,
		DIALOG_MISSION_RANK_ENDING,
		DIALOG_MISSION_RANK_LIST,
		DIALOG_MISSION_RANK_GAMBLE,

		DIALOG_GUILD_POINT_EXCHANGE,
		DIALOG_GUILD_DONATE_UP,
		DIALOG_GUILD_NOTICE,
		DIALOG_GUILD_RANKUP,

#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
		DIALOG_VENDOR_BUY_POPUP,
#endif

		DIALOG_HELP_MISSION,
		DIALOG_VENDOR_LIST,
		DIALOG_COMMUNITY_DLG,
		DIALOG_COMMUNITY_INFO,
		DIALOG_COMMUNITY_READ_MAIL,
		DIALOG_COMMUNITY_WRITE_MAIL,
		DIALOG_VENDOR_SEARCH,
		DIALOG_CHAT_PROPERTY,
		DIALOG_CASH_ITEM_SHOP,
		DIALOG_CASH_ITEM_SHOP_BUY,
		DIALOG_CASH_ITEM_SHOP_BUY_CHECK,
		DIALOG_CASH_ITEM_SHOP_BUY_COMPLETE,
		DIALOG_CASH_ITEM_SHOP_CART,
		DIALOG_CASH_ITEM_SHOP_NPC_POPUP,
		DIALOG_CASH_ITEM_SHOP_PRESENT_DLG,
		DIALOG_CASH_ITEM_SHOP_PRESENT_CHECK,
		DIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE,
		DIALOG_CASH_ITEM_SHOP_PACKAGE_DLG,
		DIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK,
		DIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE,

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
		DIALOG_CASH_ITEM_SHOP_MILEAGE_DLG,
		DIALOG_CASH_ITEM_SHOP_MILEAGE_BUY,
		DIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK,
		DIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE,
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

		DIALOG_CASH_ITEM_SHOP_EXTENSION_BUY,
		DIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE,

		DIALOG_SOCKET_REMOVE,
		DIALOG_TUTORIAL_MISSION_INVITE,
		DIALOG_PASSWORD_DLG,

#ifdef __CN_269_MATRIX_CARD_AUTH
		DIALOG_MATRIX_CARD_AUTH,
#endif //__CN_269_MATRIX_CARD_AUTH

		DIALOG_MISSION_RELAY,

		DIALOG_CHANGE_ROOM_NAME,

		DIALOG_CHAT_PERSONAL_OPTION_DIALOG,
		DIALOG_PARTY_ITEM_SHARE_SYSTEM,

		DIALOG_GUILD_DUTY_VEST,
		DIALOG_GUILD_RIGHT_DELEGATION,
		DIALOG_GUILD_MEMBER_RIGHT_VIEW,
		DIALOG_GUILD_CORPS_CHANGE,

		DIALOG_GUILD_MARK_COMBINATION,
		DIALOG_GUILD_MARK_REGISTE_CONFIRM,

		DIALOG_GUILD_ALLIANCE,
		DIALOG_GUILD_HOSTILITY,
		DIALOG_GUILD_ONE_SIDE_HOSTILITY,
		DIALOG_GUILD_RELATION_ASK,
		DIALOG_GUILD_ALLIANCE_REQUEST,

		DIALOG_FATE_ITEM_SYSTEM,

		DIALOG_GUILD_WAREHOUSE,
		DIALOG_GUILD_WAREHOUSE_RIGHT,
		DIALOG_GUILD_WAREHOUSE_LOG,

#ifdef _YJ_FISHING_SYSTEM_000933
		DIALOG_FISHING_SHOP,
		DIALOG_FISHING_TRADE,
		DIALOG_FISHING_ACTION,
#endif //_YJ_FISHING_SYSTEM_000933

		DIALOG_PERSONAL_WAYPOINT,

        DIALOG_CASH_ITEM_PUSCHASE_DLG,
#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
		DIALOG_TEXTURE_VIEWER,              // Texture_Viewer를 띄우기 위한 Dialog -20130311홍성현-
#endif
#endif
		DIALOG_MAX
	};

private:
    static BOOL backup_old2dmode;
    static BOOL backup_oldrealsize;
    static BOOL backup_zwrite;
    static BOOL backup_ztest;
    static BOOL backup_fog;

    static DWORD    backup_alpharef;
    static WzColor  backup_oldcolor;
    static ENUM_CMP_FUNC    backup_alphafunc;
    static ENUM_CMP_FUNC	backup_zfunc;
    static ENUM_ALPHABLEND	backup_alphablendmode;
    static ENUM_CULL_FACE   backup_cullfacemode;
    static ENUM_MESH_DRAW   backup_drawmode;

public:
    static void Begin2DMode(RECT* viewport,
                            DWORD clear_flags = CR_DEPTH/*CR_DEFAULT*/, 
                            WzColor* clear_color = NULL, 
                            float* clear_z = NULL, 
                            DWORD* clear_stencil = NULL);
#ifdef _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
    static void BeginPlayerRender2DMode(Object* objectr_ptr, RECT* viewport = NULL,
        DWORD clear_flags = CR_DEPTH/*CR_DEFAULT*/,
        WzColor* clear_color = NULL,
        float* clear_z = NULL,
        DWORD* clear_stencil = NULL);
#endif // _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST
                            
    static void End2DMode();

public:
	InterfaceManager();
	~InterfaceManager();

	BOOL								Init( HWND hWnd, CDrawBase * pDrawBase);
	VOID								Release();
	VOID								Render();
	VOID								Update();
    RECT *                              GetWindowRC()
    {
        return &m_rcWindow;
    }
    void                                SetWindowRC(RECT* rcWindow)
    {
        m_rcWindow = *rcWindow;
    }

	BOOL								IsIntersectRect(POINT & rPoint);

	VOID								NetworkProc( eDIALOG_ID id, MSG_BASE * pMsg );
	VOID								NetworkProc( WzID	    id, MSG_BASE * pMsg );

	BOOL        						InterfaceWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam ,SI_MESSAGE *stMessage);


public:

	BOOL								LoadInterface( SolarDialog * pSolarDialog, char * pszFileName, DWORD dwCompareID, BOOL bShow = FALSE, BOOL bCompare = TRUE );
	BOOL								LoadInterfaceChangeID( SolarDialog * pSolarDialog, char * pszFileName, DWORD dwChangeID, BOOL bShow );
	BOOL								CloneInterface( SolarDialog * pSolarDialog, char * pszFileName, DWORD dwNewKey );
	void								ReleaseDialogTexture();

	DWORD								NewInterface( SolarDialog * pSolarDialog, char * pszFileName, BOOL bShow );
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
    BOOL                                InsertInterface(SolarDialog * solar_dialog, CDialogWZ* dialog, BOOL is_show); // 로드된 ui를 연결
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

	BOOL								DeleteInterface( DWORD dwCompareID );
	
	CDialogManager *					GetDialogManager( void ) { return m_pDialogManager; }
	SolarDialog *						GetDialog( eDIALOG_ID eDlgID );	
	
	SolarDialog *						GetDialog( WzID DlgWzID );	
    
	void                                RemoveDialog( WzID DlgWzID );   // to use in uibaseMan, maybe caution other place using this func.

	void								SetDialogWzIDToChangeID(eDIALOG_ID eDlgID, DWORD dwID) {m_wzId[eDlgID] = dwID;}
	WzID								GetDialogWzID( eDIALOG_ID eDlgID );

	// 최상위 윈도우 세팅관련
	VOID								AttachFirstDialog(eDIALOG_ID eDlgID);
	VOID								AttachFirstDialog(WzID DlgWzID);

	VOID								DeAttachFirstDialog(eDIALOG_ID eDlgID);
	VOID								DeAttachFirstDialog(WzID DlgWzID);

	VOID								MoveWindow( WzID DlgWzID, int x, int y );
	VOID								MoveWindow( eDIALOG_ID eDlgID, int x, int y );
    void                                MoveDefaultWindowPos();
	
	VOID								ShowGameInterface( BOOL bFlag );
	BOOL								IsShowGameInterface() {return m_bShowGameInterface;}

	BOOL								ShowWindow( WzID wzDlgID, BOOL val );
	BOOL								ShowWindow( eDIALOG_ID id, BOOL val );
    BOOL								ShowWindow(SolarDialog* dialog, BOOL val);
 	BOOL								ShowDialog( eDIALOG_ID id, BOOL val );
	BOOL								ShowDialog( WzID wzDlgID, BOOL val );		

	VOID								ShowDialogInterface( eDIALOG_ID id );

	BOOL								IsShowDialog( eDIALOG_ID id ) const;
	BOOL								IsShowDialog( WzID id ) const;
	BOOL								IsHaveFocus( eDIALOG_ID id ) const;
	BOOL								IsHaveFocus( WzID id ) const;
		
	BOOL								IsValid() { return m_bValid;	}

	BOOL								IsHaveFocusAtEdit();

    StatusIconManager*                  GetStatusIconManager()  {   return status_icon_manager_; }

	//-----------------------------------------------------------------------------------------------
	// 데미지 띠우는데 필요한것들
	//-----------------------------------------------------------------------------------------------
	void                                AddDamageList(DAMAGE_POPUP &popup);
	void                                ResetDamageList();
	void                                RenderDamageList(DWORD dwTick);
	

	//-----------------------------------------------------------------------------------------------
	// 이동포인터
	//-----------------------------------------------------------------------------------------------
	void                                SetMovePointer(BOOL bFlag,WzVector *pPos);
	void                                RenderMovePointer(DWORD dwTick);

	//-----------------------------------------------------------------------------------------------
	// 지역 지정 포인터
	//-----------------------------------------------------------------------------------------------
	void                                SetAreaPointer(BOOL bShow, WzVector* pPos, float fAreaSize, WzColor color=0xffffffff);
    void                                SetAreaTargetPointerTexture( HANDLE AreaTargetTexture );
    HANDLE                              GetAreaTargetTextureHandle( eCHAR_TYPE CharacterType );
	void                                RenderAreaPointer(DWORD dwTick);

	//-----------------------------------------------------------------------------------------------
	// attack 포인터
	//-----------------------------------------------------------------------------------------------
	void                                RenderTargetPointer(DWORD dwTick);
	void								RenderTargetEffect();

#ifdef MAP_EDIT_MODE
	void                                RenderTerrainTargetPointer(DWORD dwTick);
#endif 

	//-----------------------------------------------------------------------------------------------
	// 인터페이스 숨기기 관련
	//-----------------------------------------------------------------------------------------------
	void								SaveAppearWindows();
	void								LoadAppearWindows();
	void								HideAllWindows();
	//void                                UpdateAllWindowPosition();
	void                                ShowInterface(BOOL bFlag);
	BOOL                                GetShowFlag();
	VOID								SetRenderState( BOOL bRender ) { m_bRender = bRender; }

	////-----------------------------------------------------------------------------------------------
	//// 메시지 출력관련
	////-----------------------------------------------------------------------------------------------
	//void                                ShowMessageBox( LPCTSTR szMessage,int iDelay=1000, WzColor *pTextColor = NULL ,WzID strID=0 );
	//void                                ShowCenterMessageBox( LPCTSTR szMessage,int iDelay, WzColor *pTextColor = NULL, BOOL bDrawBox = TRUE );
	//void                                ShowMissionAreaInfo( LPCTSTR szMessage,int iDelay, WzColor *pTextColor = NULL );
	//void                                HideMessageBox();
	//void                                HideCenterMessageBox();
	//void                                RenderMessageBox();
	//void                                RenderCenterMessageBox();
	void                                ShowGetMoneyMessage(MONEY money);

	//void                                RenderMissionAreaMessageBox();
	void                                Process(DWORD dwTick);

	//-----------------------------------------------------------------------------------------------
	// 에러 코드에 의한 정보 출력 관련
	//-----------------------------------------------------------------------------------------------
	BOOL								GetStringZoneError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen);
	BOOL								GetStringPVPError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen);
	BOOL								GetStringQuestError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen);
	BOOL								GetStringRewardError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen);
	BOOL								GetStringWarehouseError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen );
	BOOL								GetStringEnterVillageError( DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen );
	BOOL								GetStringACError(DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen );
	BOOL								GetStringStatusError(DWORD dwErrorCode, TCHAR * pszOutMessage, int iLen );

	void								UpdateQuestInfo();
	
    void RenderRectWithTextureAndZTest(RECT area, float fDepth, HANDLE tex_handle);
	void RenderRectWithTextureAndZTest(RECT rt, float fDepth,int iRow, int iColumn);
	void RenderGuildMark4MapObject(WzVector* pPos1, WzVector* pPos2, int iRow, int iColumn);

	void								RenderGuildMarkBackGround(RECT rc, float fDepth, eGUILD_MARK_RELATION eRelation);
	void								RenderCharNameBackGround(LONG lx, LONG ly, LONG lTextx, LONG lTexty, float fDepth, eGUILD_MARK_RELATION eRelation, bool bGuild, bool bGuildMark);
	void                                RenderRect(RECT rc,WzColor color);	
	void                                RenderRectWithZTest(RECT rc,WzColor color,float fDepth);	
	void                                RenderChatRect(RECT rc,WzColor color, float fDepth = 0.f);
	void                                RenderShadowRect(RECT rc,WzColor color);
	
public:
	// 기타
	void RenderActivateMark(int x,int y,int w =0, int h=0);
    void RenderActiveAutobot(int x, int y);
    void RenderActivateMarkIcon(int dx,int dy, int dw, int dh, int animation_index);
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    void RenderActivateMarkPerkGroup(int x, int y, int width, int height);
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT
    void RenderComboIconEffect(int x,int y,int w, int h);
#endif
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    DiceCastManager* GetDiceCastManager() {return dice_cast_manager_;}
    WzVector GetDiceAngle(int party_index, int dice_value, DWORD animation_tick);
    WzColor GetDiceColor(DWORD player_key, DWORD high_number_player, DWORD animation_tick);
    void SetDiceAngle();
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING


	void                                GetInterfaceString( int iStringIndex,TCHAR *szText,int bufsize = INTERFACE_STRING_LENGTH );
	LPCTSTR	                            GetInterfaceString( int iStringIndex );
    bool                                GetInterfaceStringWithCheck(int iStringIndex,TCHAR *szText,int bufsize);

    LPCTSTR                             GetItemName( CODETYPE ItemCode );
	BOOL								GetItemName( CODETYPE ItemCode, TCHAR * OUT pszOutNameResult, int iLen = MAX_ITEMNAME_LENGTH );
	BOOL								GetItemClassName(CODETYPE ItemCode, TCHAR * OUT pszOutClassName, int iLen = MAX_ITEMNAME_LENGTH);

    void    GetInterfaceStringFormat(OUT TCHAR* msg, int msg_size, int format_string_idx, ...);

	void                                SetShowItemName( BOOL bShow );
	BOOL                                ShowItemName();
	BOOL								IsLoadFontAndString() { return m_bLoadFontAndString;}
	BOOL								IsLockDlg() { return m_bLockDlg;}


    void                                RestoreCaptionFromStringTable();

    BYTE                                GetFontCharacterSet();

protected:
	BOOL								_loadDialog( char * pszFileName, DWORD dwCompareID, BOOL bCompare );

private:
	BOOL								m_bRender;	
	BOOL								m_bLockDlg;
	CDrawBase *							m_pDraw;

	CDialogManager *					m_pDialogManager;

	BOOL								m_bValid;
	SolarHashTable<SolarDialog *> *		m_pDialogHashTable;
	SolarHashTable<SolarDialog *> *		m_pNotLoadDlgTable;
	
	util::CObjKeyGenerator<DWORD> *		m_pDynamicDialogKeyGenerator;
	static WzID							m_wzId[ DIALOG_MAX ];
	
    DamageMesh*                         m_DamageMgr;

	// 이동 포인터
	BOOL                                m_bMovePointerFlag;	
	WzVector                            m_vMovePointerPos;	
	CWzUnit                             *m_pMovePointerUnit;
	CWzUnitDraw                         *m_pMovePointerUnitDraw;

	// 지역 지정 포인터
	BOOL                                m_bAreaPointerFlag;	
	WzVector                            m_vAreaPointerPos;	
	CWzUnit                             *m_pAreaPointerUnit;
	CWzUnitDraw                         *m_pAreaPointerUnitDraw;
	WzSimpleMesh                        *m_pAreaPointer;
	HANDLE                              m_hTextureAreaPointer;
    HANDLE                              m_hTextureAreaPointerForMystic;
#ifdef _NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER
    HANDLE                              m_hTextureAreaPointerForShadow;
#endif //_NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    HANDLE                              texture_area_pointer_for_witchblade_;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	WzColor								m_AreaColor;				

	// attack target
	HANDLE                              m_hTextureTargetPointer;
	CWzUnit                             *m_pTargetPointerUnit;
	CWzUnitDraw                         *m_pTargetPointerUnitDraw;
	
	// activate
    HANDLE                              m_hTextureActivate;
	HANDLE                              m_hTextureChat;
#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT
    HANDLE                              m_hTextureLinkSkill;
#endif
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    HANDLE                              button_activate_texture_handle_;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    HANDLE m_hTextureActivateAutobot;

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    DiceCastManager*                    dice_cast_manager_;
    CWzUnit*		                    dice_unit_;
    CWzUnitDraw*	                    dice_unit_draw_;
    float                               dice_angle_x_[MAX_PARTYMEMBER_NUM];
    float                               dice_angle_z_[MAX_PARTYMEMBER_NUM];

public:
    CWzUnitDraw* dice_unit_draw() { return dice_unit_draw_; }
    HANDLE GetPartyDiceTexture(int dice_tex_index);
private:

#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

	// 아그들 3d얼굴 텍스쳐
	HANDLE                              m_hTextureMyHead;
	HANDLE                              m_hTextureTargetHead;
	HANDLE                              m_hTextureSummonTargetHead;
	HANDLE                              m_hTexturePartyHead[MAX_PARTYMEMBER_NUM];

    //! 파티원 주사위
    HANDLE  m_hTexturePartyDice[MAX_PARTYMEMBER_NUM];

	BOOL                                m_bUpdateMyHeadTexture;
	BOOL                                m_bUpdateTargetHeadTexture;
	BOOL                                m_bUpdateSummonTargetHeadTexture;
	BOOL                                m_bUpdatePartyHeadTexture[MAX_PARTYMEMBER_NUM];

	HANDLE                              m_hTestRenderTarget;

	HANDLE								m_hTextureBuffBG;   // 버프 백그라운드 텍스쳐

	Clone                               *m_pHeadClone;

	vector<DWORD>						m_vecShowWindow;

	WzSimpleMesh                        *m_pMessageMesh;


    RECT                                m_rcWindow;
	const SCSlot						*m_pInfoSlot;
	
    StatusIconManager*                  status_icon_manager_;
	BOOL								m_bHaveFocusAtEdit;
	DWORD                               m_dwInterfaceTick;
	BOOL                                m_bShowItemName;

	BOOL								m_bShowGameInterface;

	//	임시 스트링 그때그때 바뀐다.
	std::string							m_tempString;
    
    IconImage* pet_etheria_icon_;    
    IconImage* etheria_icon_;
    HANDLE warning_check_item_;
    HANDLE grid_effect_texture_;
    FRECT grid_rectangles_[8];//8방향
#ifdef _NA_002935_20110704_ITEM_SCORE
#ifndef _NA_000000_20130114_RENEWER_UI
    HANDLE item_score_texture;
#endif // _NA_000000_20130114_RENEWER_UI
#endif //_NA_002935_20110704_ITEM_SCORE
    HANDLE battle_icon_texure_;

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	std::vector<std::map<WORD, WORD> > sun_ranking_grade_mark_vector_;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    std::map<WORD, int> gold_rush_mark_map_;

public:
    int get_gold_rush_mark_index(const WORD& gold_grade) const;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	int get_ranking_mark_index(const eSUN_RANKING_TYPE& ranking_type, const eSUN_RANKING_GRADE_TYPE& grade);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

	// CK UI
public:

	HANDLE                              m_hMonsterGauageTexture;


    IconImage* item_mark_ptr[MARK_TYPE_MAX];
    
    IconImage* pet_etheria_icon() const { return pet_etheria_icon_; }
    void set_pet_etheria_icon(IconImage* val) { pet_etheria_icon_ = val; }

    IconImage* etheria_icon() const { return etheria_icon_; }
    void set_etheria_icon(IconImage* val) { etheria_icon_ = val; }
    
    HANDLE get_warning_check_item() const { return warning_check_item_; }
    void set_warning_check_item(HANDLE handle) { warning_check_item_ = handle; }
    void RenderCheckItem(RECT& rect);

	HANDLE                              m_hPetInfoTexure;
	IconImage *							m_pPetIconLockImg;

	IconImage *							m_pIdentifyPossessionImg;

#ifndef _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
	IconImage *							m_pIdentifyDamegeImg[eITEM_TYPEMAX];
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

	POINT                               RenderDrawSign(POINT &rPos, eIMG_TYPE eType, eIMAGE_NUMBER_TYPE nIndex = eIMAGE_NUMBER_DEFAULT_YELLOW );
	POINT                               RenderDrawNumber(RECT &rc,int nNumber,DWORD dwStyle, eIMAGE_NUMBER_TYPE nIndex = eIMAGE_NUMBER_DEFAULT_YELLOW );
#ifdef _NA_002935_20110704_ITEM_SCORE
#ifndef _NA_000000_20130114_RENEWER_UI
    void RenderDrawItemScore(RECT start_rect,int score);
#endif // _NA_000000_20130114_RENEWER_UI
#endif //_NA_002935_20110704_ITEM_SCORE
    void RenderBattleRoomIcon(eBATTLE_ICON_TYPE icon_type,
        float dest_x, float dest_y,
        float dest_width, float dest_height);
    void RenderBattleRoomIcon(eBATTLE_ICON_TYPE icon_type,
        RECT* dest_rect);

    bool                                InitInterfaceManagerGroups();
    void                                ReleaseInterfaceManagerGroups();
    uiBaseMan*                          GetUserInterfaceManager(UINT uID);
	void								SetUserInterfaceManager(UINT uID, uiBaseMan* pBaseMan){m_vUserInterfaceMans[uID] = pBaseMan;} 
    int                                 GetUserInterfaceManagerNum();
    void                                UpdateUserInterfaceManagerNetworkMessage(MSG_BASE *pMsg);
    void                                UpdateUserInterfaceManagetNetworkMessageWorld(MSG_BASE *pMsg);
	void								PreRenderInterfaceManager(DWORD dwTick);
	void								PostRenderInterfaceManager(DWORD dwTick);

    SCSlotContainer*                    GetUISkillSlotContainer();
    uiBase* GetUISkillCurrentDialog();
    void                                SetSkillInfos(const SKILL_TOTAL_INFO & SkillInfo);
    void                                InitSkillInfos();
    void                                ReadySkillInfos();
	void                                RefreshQuickSlotFromSkillInfos();


    void                                CloseUIPartys();
    void                                ShowUIPartys();
    void                                ResetUIPartys();
    void                                ToggleUIPartys();

	void								RenderRect2(RECT rc, WzColor color, DWORD eSpecialTooltip = eSPECIAL_TOOLTIP_NONE);

	void RenderObjectOnRect(Object *pObject,RECT viewport,float fVolumeWidth,float fVolumeHeight, WzColor wzColor = 0xffffffff);
	void RenderCloneOnRect(Clone *pClone,RECT viewport,float fVolumeWidth,float fVolumeHeight,WzColor wzColor=0x000000ff);
	void RenderRiderOnRect(Rider *pRider,RECT viewport,float fVolumeWidth,float fVolumeHeight,WzColor wzColor=0xffffffff);
    void RenderUnitDrawOnRect(CWzUnitDraw* unit_draw, RECT area, float rot_radin_value, FPOINT org, float zoom);
    void RenderUnitDrawOnRect(CWzUnitDraw* unit_draw, 
                                RECT area, 
                                WzVector& pos, 
                                WzVector& rot, 
                                FPOINT org, 
                                float zoom,
                                DWORD clear_flags, 
                                WzColor& clear_color);
    void RenderUnitToTexture(CWzUnitDraw* unit_draw, 
                            HANDLE tex_handle,
                            WzVector& pos, 
                            WzVector& rot, 
                            FPOINT org, 
                            float zoom,
                            DWORD clear_flags, 
                            WzColor& clear_color);

    void RenderPetOnRect(Pet* pet, WzBoundingVolume* bounding_volume, RECT area, float rot_radin_value, FPOINT org, float zoom);
    void RenderRiderOnRect(Rider* rider, RECT area, float rot_radin_value, FPOINT org, float zoom);
    void RenderPlayerOnRect(Player* player, RECT area, float rot_radin_value, FPOINT org, float zoom);
#ifdef _NA_007086_20140318_MONSTERBOOK
    void RenderMonsterOnRect(Monster* monster, RECT area, float rot_radin_value, FPOINT org, float zoom);
#endif //_NA_007086_20140318_MONSTERBOOK
   
	RECT                                CalcuateViewPortWithRect(RECT rc);           

	void LockDlgsAll();
    void LockDlgs(DWORD dwLockDlg,BOOL bWzStyle = FALSE);
    void UnLockDlg(WzID dialog_id, bool is_monopoly_msg);
	void UnLockDlgs();

	void                                UpdateMyHeadTexture();
	void                                UpdateTargetHeadTexture();
	void                                UpdateSummonTargetHeadTexture();
	void                                UpdatePartyHeadTexture(int iIndex);

	void UpdateRenderTarget();
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    void RenderToDiceTexture();
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

	HANDLE                              GetMyHeadTexture();
	HANDLE                              GetTargetHeadTexture();
	HANDLE                              GetSummonTargetHeadTexture();
	HANDLE                              GetPartyHeadTexture(int iIndex);

    int									ProcessWordWrapByLine( TCHAR (*szMessage)[MAX_MESSAGE_LENGTH], LPCTSTR szText, int MaxLine );

	/*!
	 *	로드 되지 않은 다이얼로그를 저장한다.
	 *  \param[in]	dwKey	다이얼로그 키
	 *	\param[in]	pDlg	생성한 다이얼로그 포인터
	 */
	BOOL								RegisterDialog( DWORD dwKey, SolarDialog * pDlg );
    SolarDialog*                        FocusDialog();
    void                                MouseMoveLock(bool Lock);
    char* InterfaceManager::Trim(char *str);
#ifdef _DEV_VER
	DWORD LoadItemNameCode();
    #ifdef _DH_GM_NPC_WINDOW
        DWORD LoadNpcNameCode();
    #endif//_DH_GM_NPC_WINDOW
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH   

private:
    uiBaseMan *                         m_vUserInterfaceMans[UIMAN_MAX];

	BOOL								m_bLoadFontAndString;

	HANDLE                              m_hNumberTexture;
	void                                GetNumberImgInfo(eIMAGE_NUMBER_TYPE nIndex, RECT *rcOut, eIMG_TYPE eType);


#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    HANDLE                              common_bg01_texture_;
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

    void InitBlinkGrid();
	//---------------------------------------------------------------------------------------------
	//		옵션 설정
	//---------------------------------------------------------------------------------------------

public:

	////		택스쳐 퀄리티( 0, 1, 2  -->  상, 중 하 개념 ) : 실제 값 설정은 알아서 3단계로 처리하시오
	//int									GetTextureQuality( void );
	//int									GetLOD( void );
	//int									GetNormalTextureQuality( void );
	//int									GetShadowQuality( void );


	////		이펙트 관련( TRUE, FALSE ) --> 켜고 끄기
	//BOOL								GetBlurState( void );
	//BOOL								GetFogState( void );
	//BOOL								GetMapDistantView( void );


	//		현재 설정된 값들 모두 적용( 다이얼로그에서 "확인" 버튼 누르면 전체 적용되니 남발하지 마시오 )
	void								ApplyOption( BOOL bCheckApply = TRUE ,BOOL bProgramInit = FALSE);


	BOOL								LoadFontAndStringTable();
	
#ifdef _GS_ADD_BIGEVENT_MSG_1122
	void								ShowBigEventMsg();
#endif//_GS_ADD_BIGEVENT_MSG_1122

	void								SetDiplayDropItem(BOOL bProcess);

    void ResetUIManager();
    void ResetSolarDialog();

    HANDLE                              GetNumberTexture() { return m_hNumberTexture; }

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    HANDLE                              common_bg01_texture() { return common_bg01_texture_; }
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

    void SetUIBlinkGrid(DWORD dialog_id, DWORD control_id, bool is_blink);
    HANDLE grid_effect_texture(){ return grid_effect_texture_; }
    FRECT* grid_rectangles(){ return grid_rectangles_; }
};

//------------------------------------------------------------------------------
/**
*/
inline
uiBaseMan *
InterfaceManager::GetUserInterfaceManager(UINT uID)
{
    assert(uID<UIMAN_MAX);
    if (uID >= UIMAN_MAX)
        return NULL;

    return m_vUserInterfaceMans[uID];
}

//------------------------------------------------------------------------------
/**
*/
inline
int
InterfaceManager::GetUserInterfaceManagerNum()
{
    return UIMAN_MAX;
}

// 러시아DX폰트 생성시 문자셋 설정이 필요함.
// DEFAULT_CHARSET 로 세팅하여 폰트를 생성하면 ::GetTextExtentPoint() 함수의 값이 정확하지 않음
inline BYTE InterfaceManager::GetFontCharacterSet()
{
#if defined(_RUSSIA)
    BYTE character_set = RUSSIAN_CHARSET;
#else
    BYTE character_set = DEFAULT_CHARSET;
#endif

    return character_set;
}

//------------------------------------------------------------------------------
