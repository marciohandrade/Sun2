/**
 *	@file		GlobalFunc.h
 *	@brief		SendPacket 및 기본적인 함수를 모아둔다.
 *  @author		Unknown
 *	@version	Unknown
 */

////=======================================================================================================================
///** GlobalFunc
//	@author
//			유재영
//	@since
//			
//	@remarks
//	 - SendPacket 및 기본적인 함수를 모아둔다.
//
//*/

#ifndef GALOBALFUNC_H
#define GALOBALFUNC_H

#include "uiSystemMan/uiSystemMan.h"

struct tm;
class CCtrlDxIMEEditBox;
class CCtrlStaticWZ;
class CCtrlButtonWZ;
class CCtrlListWZ;
class SolarDialog;
class MapObject;
class NPC;
class Item;

enum SCENE_TYPE;
struct BASE_OptionList;

#include <MapInfoParser.h>
#include "FTextRender.h"
#include "FTextParser.h"
#include "FTextSentence.h"

enum OSTYPE
{
	NO_INDENFIED_VERSION,
	WINDOWS_2000,
	WINDOWS_XP,
	WINDOWS_SERVER_2003,
	WINDOWS_VISTA,
};

namespace GlobalFunc
{ // begin of GlobalFunc namespace
//////////////////////////////////////////////////////////////////////////

	BOOL SendPacketToGameServer( void * pBuf, int iLength );
	BOOL SendPacketToWorldServer( void * pBuf, int iLength );
	BOOL SendPacketToAuthServer( void * pBuf, int iLength );
	BOOL SendPacket(eCONNECTION_INDEX idx, void * pBuf, int iLength );
	void PLAY_BGM(int iType, CODETYPE FieldCode, eZONETYPE eRoomType);

    void SendGMCommand( const std::string& CommandText );

	void InitRandom();

	int GetRandom(int nRandomLimitNum);
	DWORD WzColorToRGBA(WzColor wzColor);
	WzColor RGBAToWzColor(DWORD dwColor);
    
	void ChangeMultiEditInput(WCHAR *pwszInput, TCHAR *pszOutput);
    void ChangeMultiEditOutput(TCHAR *pszInput, WCHAR *pszOutput);

    LPCSTR ReplaceFileExt(LPCSTR FileName, LPCSTR NewExt ); // 파일의 확장자를 교채한다 NewExt는 .뺴고 넣어줘

	LONG GetTextWidth(TCHAR* pText, WzID wiFont);
	LONG GetTextHeight(TCHAR* pText, WzID wiFont);
	BOOL IsSlang(TCHAR* pText, bool bShowMsg);
    bool ReplaceSlangWord(TCHAR* source_text, TCHAR* dest_buffer, int buffer_size);
	void GetWideString(TCHAR *ptszInString, TCHAR *ptszOutString, int iStringLen);
	// 2008.03.06 lammy : 개행 관련된 속성 인자 삭제 ( \n, # )은 무조건 체크 하여 개행한다.
	bool ProcessWordWrapByFontSize(std::vector<util::_tstring> *vstr, LPCTSTR szText, WzID wiFont, int strLen, int cutLen, bool bChatDialog = false);

#ifdef _NA_000000_20130114_RENEWER_UI
	bool ProcessWordWrapByFontSize2(std::vector<util::_tstring> *vstr, LPCTSTR szText, WzID wiFont, int strLen, int cutLen, bool bChatDialog = false,bool bTapCheck = false);
#endif //#ifdef _NA_000000_20130114_RENEWER_UI

    //! Output 문자열에 '\n' 삽입(자동 개행)
    //! return : int 라인수
    int ProcessWordWrapByFontSize(OUT TCHAR* out_msg, IN LPCTSTR in_msg, IN WzID font_id, 
                                IN int str_len, IN int area_width, 
                                IN int valid_line = -1,
                                IN bool is_reducing = false);

    void SetWordWarpText( CControlWZ* control );

	void DeadConfirmInHunting(bool bYes);
    void DeadConfirmInCHunting(bool bYes);
	void DeadConfirmInCHuntingBeforeStart(bool bYes);

	//////////////////////////////////////////////////////////////////////////
	// 넘버 입력관련.
	void GetStateInitItemNumConfirm(bool bValid, const char * pString);

	//void DeadConfirmInField(bool bYes);
    void DeadConfirmInField(int confirm_value);
	void DeadVerifyInMission(void);
#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
    void DeadConfirmInDomination(DWORD lParam, DWORD wParam);
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM

	void ShrinkBoundingVolume(WzBoundingVolume *pBV,float fRate);
	void SystemExitConfirm(bool bYes);


	void	_fn_uiBankPassword(bool bYes);
	void	_fn_uiBankPassword_invalidPassword( void );
	void	_fn_uiBankPassword_invalidSSN( void );
	void	_fn_uiBankPassword_Clear(bool bYes);
	void	_fn_uiBankPasswordLockingSyn(bool bYes);
	void	_fn_uiInvenPassword(bool bYes);
	void	_fn_uiInvenPassword_invalidPassword( void );
	void	_fn_uiInvenPassword_invalidSSN( void );
	void	_fn_uiInvenPassword_Clear(bool bYes);
	void	_fn_uiInvenPasswordLockingSyn(bool bYes);
	void	_fn_TutorialSkip(bool bYes);

	void QuestAbandon(bool bYes);
	void RepairConfirm(bool bYes);
	void DeleteItemConfirm(bool bYes);
	void SaveCurrentPosAtItem(bool bYes);
	void UsePoralCoordItem(bool bYes);
	void isRunUsePoralCoordItem();
	void isOutUseCoordItem(DWORD wParam,DWORD lParam);
	void SavePersnalWaypoint(bool bYes);
	void DeletePersnalWaypoint(bool bYes);
	void ExecutePersnalWaypoint(bool bYes);
	int  GetUsedWayPointSlotCount();
	bool IsWayPointItemPending();
	void SetWayPointItemPending(bool bFlag);
	bool IsBattleZone();
	void _fn_UseTopOfTrialItem(bool bYes);
	void _fn_UseSSQItem(bool b);
	void _fn_UsePortalQuestItem(bool bYes);

    void _fn_UseCharacterNameChangeItem(uiSystemMessage::ButtonIndex button_index);
    void _fn_CharacterNameChangeAck(uiSystemMessage::ButtonIndex button_index);

	void NET_SEND_CG_ITEM_USE_SKILL_ACTION_SYN(eITEMTYPE ItemType, WzVector wvTargetPos);

    void NET_SEND_CG_ITEM_USE_SKILL_ACTION_SYN(eSlotContainerIndex container_index, 
                                                POSTYPE at_pos, 
                                                DWORD target_objectkey,
                                                WzVector& target_pos);

	void UseChargeItem(bool bYes);
	void UseChargeItemStatInitAll(bool bYes);
	void SendCheckUseItemByPopup( bool b );

	void fnSendSelectObserverMode( bool b );	
	BOOL SendObserverCtrl( BYTE KeyCmd );
	BOOL SendObserverPacket( BYTE RequestType );
	const WzVector GetObserverAngle( int index );

	void SendStateInitSyn( bool bYes );

	BOOL IsexceptionPercentageType( int OptionKind);

    BOOL IsDeadField();

#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
	void AddRandomOption( bool bYes);
#endif

#ifdef __CN_269_MATRIX_CARD_AUTH
	void MatrixCodeSubmit( bool bYes);
#endif

#ifdef _NHN_USA_CHANNELING
	int CheckMoveToZone(MAPID OldMapID,MAPID NewMapID,BOOL bPortal=FALSE);
	int SendHanReportPacket(int nVal);
#endif//_NHN_USA_CHANNELING

	//------------------------------------------------------------------------------
	//! Returns:   WzID
	//! Qualifier: 탈것의 애니메이션 코드를 얻어옴
	//! Parameter: ePLAYER_ANI eState : 플레이어 애니메이션 상태
	//! Parameter: bool is_spread : 탈것 탄 상태에서 날고 있는지 여부
	//------------------------------------------------------------------------------
	WzID GetRiderAnimationByPlayerAni(ePLAYER_ANI eState, bool is_spread);

	//------------------------------------------------------------------------------
	//! Returns:   WzID
	//! Qualifier: 캐릭터의 애니메이션 코드를 얻어옴
	//! Parameter: ePLAYER_ANI eState : PC 애니메이션 상태
	//! Parameter: char cType : 탈것 유형 코드
	//------------------------------------------------------------------------------
	WzID GetPlayerAnimationByRiding(ePLAYER_ANI eState, char cType);
    WzID GetPlayerAnimationBySpreadWings(ePLAYER_ANI eState);
	WzID GetAnimationByPolymorph(ePLAYER_ANI eState);

    //------------------------------------------------------------------------------
    //! Returns:   WzID
    //! Qualifier: 날개 애니메이션 코드를 얻어옴
    //! Parameter: ePLAYER_ANI eState : PC 애니메이션 상태
    //! Parameter: bool is_spread : 날고 있는지 여부
    //------------------------------------------------------------------------------
    WzID GetWingsAnimationByPlayerAni(ePLAYER_ANI eState, bool is_spread);


	// 대표 하는 맵을 가져온다.(메인 / 서브1 / 서브 2, 3,4 ../ 순으로)
	sMAPINFO * GetMainMapFromGroup(MAPCODE MapKind);
	sMAPINFO * GetMainMapFromGroup(MapGroup * pGroup);

    void LogOutGame(bool is_send_notify = true);

	//esc 전역 함수 
	void DoSystemDialog(DWORD wParam,DWORD lParam);
	void DoAutoAttack(DWORD wParam,DWORD lParam);
	void DoAutoRun(DWORD wParam,DWORD lParam);
	void DoAreaSkill(DWORD wParam,DWORD lParam);
	void DoTarget(DWORD wParam,DWORD lParam);
	void DoLogin(DWORD wParam,DWORD lParam);

	void DoShowWindow(DWORD wParam,DWORD lParam);
	void DoDeleteWindow(DWORD wParam,DWORD lParam);
	void DoShowQuestWindow(DWORD wParam, DWORD lParam);
	
	//변경된 DLG생성
	void DoShowWindowEx(DWORD wParam,DWORD lParam);

	void DoBattleCreate(DWORD wParam,DWORD lParam);
	void DoZoneList(DWORD wParam,DWORD lParam);
	void DoBank(DWORD wParam,DWORD lParam);
	void DoGuildWareHouse(DWORD wParam,DWORD lParam);
    void DoTrade(DWORD wParam,DWORD lParam);
    void DoSystemVerify(DWORD wParam,DWORD lParam);
    void DoSystemConfirm(DWORD wParam,DWORD lParam);
    void DoRadarFullDialog(DWORD wParam, DWORD lParam);
	void DoContinentMapDlg(DWORD wParam, DWORD lParam);
	void DoQuestListDlg(DWORD wParam, DWORD lParam);
	void DoQuestInfoDlg(DWORD wParam, DWORD lParam);
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#else
    void DoGuildMenu(DWORD wParam,DWORD lParam);
    void DoGuildMain(DWORD wParam,DWORD lParam);
    void DoGuildCreate(DWORD wParam,DWORD lParam);
    void DoGuildWindow(DWORD wParam,DWORD lParam);
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
	void DoVentorBuy(DWORD wParam,DWORD lParam);
	void DoVentorSell(DWORD wParam,DWORD lParam);
	void DoVentorPopup(DWORD wParam,DWORD lParam);
    void DoVentorTitlePopup(DWORD wParam,DWORD lParam);
	void DoPartyLeave(DWORD wParam,DWORD lParam);
	void DoInputNumBank(DWORD wParam,DWORD lParam);
	void DoInputNumTradeBank(DWORD wParam,DWORD lParam);
	void DoCastCancel(DWORD wParam,DWORD lParam);
	void DoLeaveFieldCancel(DWORD wParam,DWORD lParam);
	void DoLotteryOpenCancel(DWORD wParam,DWORD lParam);
	void DoInvokeSceneEvent(DWORD wParam,DWORD lParam);	
	void DoAreaConquest(DWORD wParam,DWORD lParam);
	void DoTotalComposite(DWORD wParam,DWORD lParam);
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
	void DoVendorBuyPopup(DWORD wParam, DWORD lParam);
#endif
	void DoVendorList(DWORD wParam, DWORD lParam);
	void DoDeleteItemCancel(DWORD wParam,DWORD lParam);
	void DoRankDownCancle(DWORD wParam,DWORD lParam); 
	void DoRepairLimitedCancle(DWORD wParam,DWORD lParam); 
#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
	void DoAddRandomOption(DWORD wParam, DWORD lParam);
#endif

	void DoStatChangeItemCancel(DWORD wParam,DWORD lParam);

#ifdef __NA_001358_ENCHANT_UNIFICATION
	void DoInstantEnchantCancle(DWORD wParam,DWORD lParam);				
#endif // __NA_001358_ENCHANT_UNIFICATION
    DWORD GetMoneyColor(MONEY money);
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	DWORD GetMoneyWzColor(MONEY money);
	std::string GetMoneyColorHexForString(MONEY money);
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION
#ifdef _JBH_MOD_MONEY_OUTPUT
	void  ChangeMoneyToStringMoney ( MONEY money , TCHAR * pszResult);
#endif

    //_NA_000000_121015_LEAVE_CANCEL_TO_REVIVE_RE_CASTING
    void ReviveCastFromSceneType(); //Scene에 맞춰 부활 캐스팅 시전

    void DoWaxGlossItemCancle(DWORD wParam,DWORD lParam);

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
    void DoExtendItemDate(DWORD l_param, DWORD r_param);
#endif//_NA_0_20111012_ITEM_DATE_EXTEND

#ifdef _NA_003740_20111122_RANDOM_ITEM
    void DoRandomizeItem(DWORD l_param, DWORD r_param);
#endif//_NA_003740_20111122_RANDOM_ITEM


   void DoEtheriaExtract(DWORD l_prarm, DWORD r_prarm);
   void DoEtheriaCombination(DWORD l_prarm, DWORD r_prarm);

   void DoSocketIdentify(DWORD l_prarm, DWORD r_prarm);
   void DoSocketRecreate(DWORD l_prarm, DWORD r_prarm);

   void DoCrystalization(DWORD l_param, DWORD r_param);

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
   void DoChaoszoneDialog(DWORD l_prarm, DWORD r_prarm);
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
   void DoSmartNpcMenuDialog(DWORD l_prarm, DWORD r_prarm);
   void DoSmartNpcReplyDialog(DWORD l_prarm, DWORD r_prarm);
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007086_20140318_MONSTERBOOK
   void DoMonsterBookDialog(DWORD l_prarm, DWORD r_prarm);
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
   void DoAttendanceCheckDialog(DWORD l_prarm, DWORD r_prarm);
   void AttendanceCheckRewardConfirm(bool confirm_value);
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
   void DoSUNRankingDialog(DWORD l_param, DWORD r_param);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
   void DoBingoGameDialog(DWORD l_param, DWORD r_param);
#endif // _NA_008405_20150716_MINIGAME_BINGO

	void ErrorCode_Player_State(ePLAYER_BEHAVE_STATE eState);

	void ShowMessageBox_Debug(LPCTSTR szMessage,int iDelay, WzColor *pTextColor = NULL );
	void GetText( CCtrlDxIMEEditBox *pControl, OUT TCHAR *pMsg, int iSize );
	void SetText( CCtrlDxIMEEditBox *pControl, LPCTSTR pMsg );
	void SetCaption( CCtrlStaticWZ *pControl, LPCTSTR pMsg );
	void SetCaption( CCtrlButtonWZ *pControl, LPCTSTR pMsg );
	bool SetCaptionSimple( CControlWZ *pControl, INOUT TCHAR* pMsg,int nType=0);
    void SetCaptionByPasswordType(CCtrlStaticWZ* pCtrlTxt, INOUT LPTSTR strPassword);
	bool CovertSimpleText( TCHAR *pMsg,int nSize ,DWORD dwFontID,RECT &rc);

	void SetItemText( CCtrlListWZ *pControl, int iIndex, LPCTSTR pMsg );
	void SetItemText( CCtrlListWZ *pControl, int iIndex, int iSubIndex, LPCTSTR pMsg );
	void SetItem( CCtrlListWZ *pControl, int nIndex, TCHAR* pszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor); 
	void SetItem( CCtrlListWZ *pControl, int nIndex, int nSubIndex, TCHAR* pszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor); 
	void SetReportButtonInfo( CCtrlListWZ *pControl, int nIndex,int nLength, LPCTSTR pszSZ );
	
	void SetFocus( SolarDialog * pDlg, CCtrlDxEditBox * pControl);
	void SetMoneyText( CCtrlStaticWZ *pControl, MONEY money);

    void SetMoneyText(CCtrlStaticWZ *control, MONEY money, BYTE money_type, bool is_minus = false);
    void SetPointText(TCHAR *result_string, INT64 number);//숫자를 받아 3자리마다 ','구분점 스트링 반환
	void CalcTime(DWORD dwSec IN, int& min OUT, int& sec OUT, TCHAR *ptszOutString OUT);
	void CalcTime(DWORD dwMin IN, int& day OUT, int& hour OUT, int& min OUT, TCHAR *ptszOutString OUT);
	void CalcTime(DWORD64 dwTime IN, TCHAR *ptszOutString OUT);
	bool CompareLimitText(CCtrlDxIMEEditBox * m_pEditCtrl, TCHAR* szOutMessage, int iSize, int iErrorNum);

	/////////////////////////////////////////////////////////
	// 메시지 출력관련												// or 연산시 캐스팅이 필요해서 int로 바꿈 / 07.09.05 / lammy
	void default_OutputMsg( int eMsgType, TCHAR * pszMessage, int iDisplayTime = 10000);  // 외부에서 호출하지 않는다.
	void OutputMsg_SysAlertMsg( const TCHAR * pszMessage, ...);				
	void OutputMsg_SysMsg( const TCHAR * pszMessage, ...);
	void OutputMsg_Notice( int eMsgType, const TCHAR * pszMessage, ...);
	void OutputMsg_Error( int eMsgType, const TCHAR * pszMessage, ...);
	void OutputMsg_Guide( int eMsgType, const TCHAR * pszMessage, ...);
	void OutputMsg_Warning( int eMsgType, const TCHAR * pszMessage, ...);
	void OutputMsg_PCBang_Notice(TCHAR* pszMessage, ...);
    void DisplayMessageForTrigger( BOOL bDone, int iTextID, BYTE byIsParty, int iDisplayWindow, int iDisplaySwitch, DWORD dwObjectKey);

    //------------------------------------------------------------------------------
    //! Returns:   void
    //! Qualifier: 날개 시스템의 에러 처리
    //! Parameter: uint8_t result : WingControl::eResult
    //! Parameter: uint16_t result_code : WingControl::eResultCode
    //------------------------------------------------------------------------------
    void OutputMsg_WingControl_ResultCode(uint8_t result, uint16_t result_code);


	TCHAR * GetCorpsName(int iCorps);

	char* AddNationName(char * szString);

    char* AddNationName(IN LanguageType language_type, IN const char* file_name, OUT util::_tstring& new_file_name);
    
    //! 해당 파일이 있는지체크 함
    bool HasFile(const char* file_name);
    bool HasFile_Nation(const char* file_name, LanguageType language_type);

    void DebugOutputMsg( eOUTPUT_MSG_TYPE eMsgType, TCHAR * pszMessage, ...);
	void AlwaysOutputMsg( eOUTPUT_MSG_TYPE eMsgType, TCHAR * pszMessage, ...);

    //꼭 출력되어야 할 이펙트라면 (dwId = 0 , dwId2 = 0)으로 값 입력
	HANDLE CreateEffectBeUpToOption(WzID wzEffID, WzVector wvPos,DWORD dwId=0,DWORD dwId2=0);
	HANDLE CreateEffectBeUpToOption(WzID wzEffID, WzVector wvPos, WzVector wvScale,DWORD dwId=0,DWORD dwId2=0);
	HANDLE CreateEffectBeUpToOption(WzID wzEffID, WzVector wvPos, WzVector wvScale, WzVector wvRot,DWORD dwId=0,DWORD dwId2=0);

	void	GetCrashReportFileName(TCHAR* pszFileName);
	void	GetLogPath(TCHAR* pszLogPath);
	
#ifdef _DEV_VER
	void WriteErrLog(eLOG_LEVEL eLevel, WORD wOutputStyle, TCHAR * pszMessage, ...);
#else
	inline void	WriteErrLog(eLOG_LEVEL eLevel, WORD wOutputStyle, TCHAR * pszMessage, ...) {}
#endif



	
	
	void	SetLogPath();
	DWORD	StartTimeCheck();
	void	EndTimeCheck(DWORD dwStartTime, bool bSave, TCHAR * pszMessage);
	DWORD	StartScriptLoading(TCHAR* pszMessage);
	void	EndScriptFileLoading(DWORD dwStartTime, TCHAR* pszMessage);
	void	ClearTotalLoadingTime();
	void	PrintTotalLoadingTime();
	void	WriteLoadingTimeLog(TCHAR * pszMessage, ...);
	void	SetLoadingTimeSaveMode(bool bSave);
	bool	CheckIncludeStringOnAbroad(const char * pszMessage);
	
	void	SetLogLevel(eLOG_LEVEL eLogLevel);
	eLOG_LEVEL		GetLogLevel();
	
	void	ShowNetRecvLog(bool bShow);
	bool	IsShowNetRecvLog();

	void	ShowNetSendLog(bool bShow);
	bool	IsShowNetSendLog();

    void	ShowPacketLog(bool bShow);
    void	TogglePacketLog();
    bool	IsShowPacketLog();

	BOOL	IsDigit(TCHAR * pszMessage);

	SCENE_TYPE ConvertClientZoneType(const eZONETYPE eCommonZoneType, const eINSTANCE_DUNGEON_KIND eIDK = INDUN_KIND_MAX);
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    SCENE_TYPE ConvertClientZoneType(const eZONETYPE eCommonZoneType, const eINSTANCE_DUNGEON_KIND eIDK, MAPCODE map_code);
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
	
	// 스트링을 리스트 크기에 맞게 파싱해서 넣어준다.
	BYTE	AddDescribeString( CCtrlListWZ *pControl, TCHAR * pszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor );
	BOOL	SysMessageBox( LPCTSTR szMessage, BOOL shutdown = FALSE );
	void	SysConfirmBox( LPCTSTR szMessage, void (*_fn_sysconfirm)(bool),
						   LPCTSTR pszTitle = NULL, LPCTSTR pszSubject = NULL, DWORD dwStrOKID = 100, DWORD dwStrCancleID = 101, bool bLock = false );
	void	SysConfirmExtendBox( LPCTSTR szMessage, void (*_fn_sysconfirm)(bool),
								 LPCTSTR pszTitle = NULL, LPCTSTR pszSubject = NULL, DWORD dwStrOKID = 100, DWORD dwStrCancleID = 101, bool bLock = false );
#ifdef _DH_BATTLEZONE2_
    void PartyRoomJoinConfirm(bool confirm_value);
    void PartyLeaveAndJoinConfirm(bool confirm_value);
    void PartyContinueConfirm(bool confirm_value);
#endif//_DH_BATTLEZONE2_

	BOOL	IsParty( DWORD dwKey );

	BOOL	GetModuleFileNameDirectory( LPTSTR lpBuffer, DWORD nBufferLength );
	OSTYPE	GetOsVersion();
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	void	SendServerSelectRequestPacket( MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_SYN::eQuery eQueryTransaction );
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__
	void	GoToServerList();
	void	InitGameInfo();
	
	//착용제한수치 래핑
    WORD	CalcLimitStat(WORD ItemLimitStr, SCItemSlot* ItemSlot, LimitStatCompositeType limitstat_composite_type = kLimitStatCompositeNormal, BOOL bIgnoreLimit = FALSE);

	void	InsertItemToInvenOfHero( const ITEMSLOTEX * pSlot, int count );
	const std::string CalculateThousandDigit( MONEY money );

	BOOL  IsSSQScene( void );
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BOOL IsBattleGroundScene();
    void BattleGroundBackToTheVillageConfirm(bool confirm_value);
    void BattleGroundCharacterSelectConfirm(bool confirm_value);
    void BattleGroundServerSelectConfirm(bool confirm_value);
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    BOOL IsFreePVPScene();
    void PartyLeaveAndFreePVPJoinConfirm(bool confirm_value);
    void PartyLeaveAndFreePVPCreateConfirm(bool confirm_value);
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    BOOL IsGoldRushScene();    
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    BOOL IsDominationScene();
    BOOL IsMondschienMap(); // 원래는 그냥 점령전 Scene이다.
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    BOOL IsSpaScene();
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
    BOOL IsSonnenScheinScene();
    BOOL HideSonnenScheinnBuffScene(); // 소넨샤인 버프 가리는 곳
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	bool HeimTradingvolumeCheck(const MONEY& money);
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

	WzVector GetObserverPos( int index );

	void UseRandomItem(bool bUse);	// 랜덤 아이템

	BOOL	IsInventoryLocknMessage();
	void	UsePossessionItem(bool bUse);

void GlobalFunc::UseRandomItem(bool bUse);

#ifdef __NA_001358_ENCHANT_UNIFICATION
	void UseInstantEnchantGem(bool bEnchant);
#endif //__NA_001358_ENCHANT_UNIFICATION
	void ApplayOptionShowPlayers();
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    void RetryOneTimePassword();
    void EncourageOTP(bool bYes);
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_

    void RetrySecurityToken(bool bYes);

#if defined(__KR) || defined(_KOREA)
#if defined(_DEV_VER)
	void LoadDevIniFile();
	char* GetDevOption();
#endif	//#if defined(_DEV_VER)
#endif	//#if defined(__KR) || defined(_KOREA)
	
    void GoDefaultVillage();

    void GetMonGradeDesc(eNPC_GRADE npc_grade, TCHAR* OUT desc_str);
    void GetAICodeDesc(WORD ai_code, TCHAR* OUT desc_str);
    void GetAttackAttitudeDesc(eATTACK_ATTITUDE att_type, OUT TCHAR* desc_str);
    void GetAttTypeDesc(eATTACK_TYPE att_type, TCHAR* OUT desc_str);
    void GetMoveAttitudeDesc(eNPC_MOVE_ATTITUDE move_attitude, TCHAR* OUT desc_str);
    void GetMeleeTypeDesc(eMELEE_TYPE melee_type, TCHAR* OUT desc_str);
    void GetArmorTypeDesc(eARMOR_TYPE armor_type, TCHAR* OUT desc_str);

    DWORD GetClassNameCode(BYTE class_type);
    bool IsMyItem(Item* item_pointer);
    bool CanGetItem(Item* item_pointer);
    bool CanGetHaim(Item* item_pointer);
    bool IsNumberic(BYTE option_kind, BASE_OptionList* option_list_info);
    bool IsPercentage(BYTE option_kind, BASE_OptionList* option_list_info);
    void AddOptionSuffix(BYTE option_kind, BASE_OptionList* option_list_info, TCHAR* INOUT string_desc);

    void GetDateString(TCHAR* OUT date_string, int string_size, int string_code, SYSTEMTIME& system_time);
    LPCTSTR GetClassString(eCHAR_TYPE char_type);
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    LPCTSTR GetRaceString(eCHAR_TYPE race_type);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    void MoveToTopDialog(DWORD dialog_id);
    void ExecWebSite(DWORD string_code);

    //------------------------------------------------------------------------------
    // 스트링 테이블에 있는 모든 포멧은 %s로 되어 있다고 가정.
    //------------------------------------------------------------------------------ 
    void SetText_SignMoney(CControlWZ* control, INT64 money);
    void SetTextFromStringTable(CControlWZ* control, int format_string_code);
    void SetTextFromStringTable(CControlWZ* control, int format_string_code, int val);
    void SetTextFromStringTable_Money(CControlWZ* control, int format_string_code, MONEY money);
    void SetTextFromStringTable(CControlWZ* control, int format_string_code, TCHAR* val);
    void SetTextFromStringTable(CControlWZ* control, int format_string_code, TCHAR* val1, TCHAR* val2);
    void MakeMessage_SignMoney(OUT TCHAR* msg, int msg_size, INT64 money);
    void MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, int val);
    void MakeMessageFromStringTable_Money(OUT TCHAR* msg, int msg_size, int format_string_code, MONEY money);
    void MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, TCHAR* val);
    void MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, TCHAR* val1, TCHAR* val2);
    void MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, TCHAR* val1, TCHAR* val2, int val3, TCHAR* val4);
    void MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, int val1, TCHAR* val2, int val3, TCHAR* val4);
    void MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, int val1, TCHAR* val2, TCHAR* val3, TCHAR* val4);
    void MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, TCHAR* val1, TCHAR* val2, TCHAR* val3, TCHAR* val4);
    void MakeMessageFromStringTable_YYYYMMDD(OUT TCHAR* msg, int msg_size, int format_string_code, DWORD64 wztime64_value);
    void MakeMessageFromStringTable_tm(OUT TCHAR* msg, int msg_size, int format_string_code, tm* time_stmp);

    void SetFTextFromStringTable(FTextRender& render, FTextParser& parser, FTextSentence& sentence, int format_string_code);
    void SetFTextFromStringTable(FTextRender& render, FTextParser& parser, FTextSentence& sentence, int format_string_code, int val1);
    void SetFTextFromMsg(FTextRender& render, FTextParser& parser, FTextSentence& sentence, const TCHAR* msg);

    //------------------------------------------------------------------------------
    // uiSystemMessage function
    //------------------------------------------------------------------------------ 
    DWORD VerifyMessage(DWORD message_code, 
                        DWORD title_code = 0, 
                        uiSystemMessage::MessageCallback callback_function = NULL, 
                        bool is_modal = false);
    DWORD VerifyMessage(const TCHAR* message, 
                        const TCHAR* title, 
                        uiSystemMessage::MessageCallback callback_function = NULL, 
                        bool is_modal = false);
    DWORD ConfirmMessage(DWORD message_code, 
                         DWORD title_code = 0, 
                         uiSystemMessage::MessageCallback callback_function = NULL, 
                         bool is_modal = false);
    DWORD ConfirmMessage(const TCHAR* message, 
                         const TCHAR* title, 
                         uiSystemMessage::MessageCallback callback_function = NULL, 
                         bool is_modal = false);

    void EnableControl(CControlWZ* control_ptr, bool enable_state);

    //------------------------------------------------------------------------------
    // Qualifier : 해당 내용을 log\\packet.log 에 씀
    //------------------------------------------------------------------------------
    void Write_Packet_Log(CHAR* message, ...);

    void ProcessChatMessage( TCHAR* message );

    bool BeforeLoadingInputLock(); // 미션&전장 진입 중  입력 락

	float GetCameraHeight( eCHAR_TYPE CharType );	// 클래스별 캐릭터의 높이 (카메라용..)

    void EquipmentPlayerRefreshAppearRender(const BOOL& is_bone_refresh = FALSE);    // 인벤토리안에 있는 내 캐릭터를 외형 갱신

#ifdef _NA_000000_20140519_GAMEGUARD_LOG
    void Write_GameGuard_Log(TCHAR* message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG

#ifdef _NA_008334_20150608_SONNENSCHEIN
    void ShowSonnenScheinResurrectionPopup(DWORD l_prarm, DWORD r_prarm);
    void SonnenScheinResurrectionConfirm(int confirm_value);
#endif //_NA_008334_20150608_SONNENSCHEIN

} // end of GlobalFunc namespace



#ifdef _DEV_VER

	#ifdef assert
	#undef assert
	#endif

	#define assert(expr);	ASSERT(expr); \
		if (!(expr)) \
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, _T("assert: %s"), #expr);

	#define	WRITELOG	GlobalFunc::WriteErrLog

#else // else of _DEV_VER

	#define	WRITELOG	__noop

#endif // end of _DEV_VER


#endif
// GALOBALFUNC_H