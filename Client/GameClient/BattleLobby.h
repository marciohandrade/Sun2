//----------------------------------------------------------------------------
/**
	@author : 유 재 영( y2jinc@webzen.co.kr )

	@remark
*/

#ifndef BattleLobby_H
#define BattleLobby_H

#include "solardialog.h"
#include "BattleRoomEntry.h"
#include "UserSlotSet.h"

#include "IconMgr.h"
#include "uibattlezoneman/uiBattleZoneMan_def.h"

class uiBattleZoneMan;

#define MAX_RESULT_STRING_LENGTH	512

#define MAX_VIEW_USER_SLOT			10



class BattleLobby : public SolarDialog
{
public:
    friend class uiBattleZoneMan;

	enum eUseTexture
	{
		USE_TEXTURE_READY = 0,
		MAX_USE_TEXTURE,
	};

	BattleLobby(void);
	virtual ~BattleLobby(void);

	// 기본 enum
	enum eBattleLobby_POS
	{
		BattleLobby_ST_ROOM_NUMBER = 0,		// >> 텍스트 정보관련 컨트롤
		BattleLobby_ST_ROOM_NAME,
		BattleLobby_ST_ROOM_PASSWORD,
		BattleLobby_ST_MAP_NAME,
		BattleLobby_ST_MAP_LIMIT_LEVEL,
		BattleLobby_ST_MAP_RECOMMEND_LEVEL,
		BattleLobby_ST_ZONE_TYPE,

		BattleLobby_ST_USER_INFO_LEVEL_1,
		BattleLobby_ST_USER_INFO_LEVEL_2,
		BattleLobby_ST_USER_INFO_LEVEL_3,
		BattleLobby_ST_USER_INFO_LEVEL_4,
		BattleLobby_ST_USER_INFO_LEVEL_5,
		BattleLobby_ST_USER_INFO_LEVEL_6,
		BattleLobby_ST_USER_INFO_LEVEL_7,
		BattleLobby_ST_USER_INFO_LEVEL_8,
		BattleLobby_ST_USER_INFO_LEVEL_9,
		BattleLobby_ST_USER_INFO_LEVEL_10,
		BattleLobby_ST_USER_INFO_NAME_1,
		BattleLobby_ST_USER_INFO_NAME_2,
		BattleLobby_ST_USER_INFO_NAME_3,
		BattleLobby_ST_USER_INFO_NAME_4,
		BattleLobby_ST_USER_INFO_NAME_5,
		BattleLobby_ST_USER_INFO_NAME_6,
		BattleLobby_ST_USER_INFO_NAME_7,
		BattleLobby_ST_USER_INFO_NAME_8,
		BattleLobby_ST_USER_INFO_NAME_9,
		BattleLobby_ST_USER_INFO_NAME_10,	// << 텍스트 정보관련 컨트롤			
		BattleLobby_BT_VIEW_USER_1,
		BattleLobby_BT_VIEW_USER_2,
		BattleLobby_BT_VIEW_USER_3,
		BattleLobby_BT_VIEW_USER_4,
		BattleLobby_BT_VIEW_USER_5,
		BattleLobby_BT_VIEW_USER_6,
		BattleLobby_BT_VIEW_USER_7,
		BattleLobby_BT_VIEW_USER_8,
		BattleLobby_BT_VIEW_USER_9,
		BattleLobby_BT_VIEW_USER_10,
		BattleLobby_CHECK_USER_BLOCK_2,
		BattleLobby_CHECK_USER_BLOCK_3,
		BattleLobby_CHECK_USER_BLOCK_4,
		BattleLobby_CHECK_USER_BLOCK_5,
		BattleLobby_CHECK_USER_BLOCK_6,
		BattleLobby_CHECK_USER_BLOCK_7,
		BattleLobby_CHECK_USER_BLOCK_8,
		BattleLobby_CHECK_USER_BLOCK_9,
		BattleLobby_CHECK_USER_BLOCK_10,
		BattleLobby_TI_START,				
		BattleLobby_TI_LEAVE,		
		BattleLobby_TI_TEAM_CHANGE,
		BattleLobby_BT_USER_EXIT_2,
		BattleLobby_BT_USER_EXIT_3,
		BattleLobby_BT_USER_EXIT_4,
		BattleLobby_BT_USER_EXIT_5,
		BattleLobby_BT_USER_EXIT_6,
		BattleLobby_BT_USER_EXIT_7,
		BattleLobby_BT_USER_EXIT_8,
		BattleLobby_BT_USER_EXIT_9,
		BattleLobby_BT_USER_EXIT_10,
		BattleLobby_BT_EXIT,				 // << 액션
		BattleLobby_BT_FRIEND_LIST,
		BattleLobby_BT_GUILD_LIST,
		BattleLobby_BT_ROOM_NAME_CHANGE,

        Dialog_Max
	};


	enum 
	{
		MAX_DIFFICULT		= 3,
		MAX_BONUS			= 2,
	};

	// 기본 virtual methods
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual VOID	OnUpdateSolarDialog();
	virtual VOID	OnRenderSolarDialog();

	virtual VOID	OnShowWindow(BOOL val);

public:
	CCtrlImageStaticWZ*	GetCtrlTIStartNReady()	{	return m_pCtrlTIStartNReady;}

	// 방장만 
	
	BOOL			BlockPlayer(int index);
	void			AttachChatDialog();

	// 기본 proected methods
protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	VOID			initInfo();

	// 인터 페이스 처리 관련
	//void            UpdatePosition();
	VOID			initSetUIForBasicInfo();
	VOID			initSetUIForMaster();
	VOID			initSetUIForMember();
	VOID			updateUserSlotUI();
	VOID			initPVPUI();
	VOID			updateMapInfoUI();
	VOID			updateZoneType();
	VOID			updatePVPUI();
	void			showZoneStatusWindow();
private:
	// 포인터로 받을려다가 혹시 delete 되는 문제로 인해서 
	// 문제가 발생할 여지가 있어 그냥 인스턴스 하나를 가지고 있는다.

	RECT					m_rtSuperRoomIcon;	
	ADDITIONAL_PVPINFO		m_PendingAdditionalPVPInfo;

	HANDLE					m_hTexture[MAX_USE_TEXTURE];
	St_TextureContainer *	m_pTextureInfo[MAX_USE_TEXTURE];

	// controls 
	CCtrlButtonWZ *			m_pCtrlBtnBan[MAX_VIEW_USER_SLOT-1];
	CCtrlButtonWZ *			m_pCtrlBtnUserSlotView[MAX_VIEW_USER_SLOT];
	CCtrlButtonCheckWZ *	m_pCtrlCheckBtnUserSlotBlock[MAX_VIEW_USER_SLOT-1];
	CCtrlStaticWZ *			m_pCtrlStaticRoomNo;
	CCtrlStaticWZ *			m_pCtrlStaticRoomTitle;
	CCtrlStaticWZ *			m_pCtrlStaticRoomPW;
	CCtrlStaticWZ *			m_pCtrlStaticUserLevel[MAX_VIEW_USER_SLOT];
	CCtrlStaticWZ *			m_pCtrlStaticUserName[MAX_VIEW_USER_SLOT];
	CCtrlStaticWZ *			m_pCtrlStaticZoneType;
	CCtrlImageStaticWZ *	m_pCtrlImageStaticTeamChange;
	CCtrlStaticWZ *			m_pCtrlStaticMapName;
	CCtrlStaticWZ *			m_pCtrlStaticLimitLevel;
	CCtrlStaticWZ *			m_pCtrlStaticRecommendLevel;
	CCtrlButtonWZ *			m_pCtrlRoomNameChangeBtn;
public:
	void					ChangeRoomTitle(TCHAR* szMessage);

private:
	CCtrlImageStaticWZ *	m_pCtrlTIStartNReady;		

	TCHAR					m_szResult[MAX_RESULT_STRING_LENGTH];
	COLORREF				m_LimitTextColor;
			
	uiBattleZoneMan*		m_pBattleZoneMan;

	bool					m_bGameStart;

	// 기본 attributes
private:
	static WzID	    m_wzId[Dialog_Max];
};

#endif
// BattleLobby_H