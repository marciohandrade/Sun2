/**
 *	@file		GlobalFunc.cpp
 *	@brief		SendPacket 및 기본적인 함수를 모아둔다.
 *  @author		Unknown
 *	@version	Unknown
 */

//------------------------------------------------------------------------------
//  GlobalFunc.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "GlobalFunc.h"
#include "soundbgm.h"
#include "map.h"
#include "BGMSoundInfoParser.h"
#include "interfacemanager.h"
#include "SolarDialog.h"
#include "Hero.h"
#include "HeroActionInput.h"
#include "LoginScene.h"
#include "BattleScene.h"
#include "CompetitionScene.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "uiSystemVerify/uiSystemVerify.h"
#include "uiSystemConfirm/uiSystemConfirm.h"
#include "uiVendorMan/uiVendorMan_def.h"
#include "uiVendorBuy/uiVendorBuy.h"
#include "uiVendorSell/uiVendorSell.h"
#include "uiVendorPopup/uiVendorPopup.h"
#include "uiVendorTitlePopup/uiVendorTitlePopup.h"
#include "uiPartyLeavePopup/uiPartyLeavePopup.h"
#include "uiTrade/uiTrade.h"
#include "uiTradeMan/uiTradeMan.h"
#include "uiCharPopupMenu/uiCharPopupMenu.h"
#include "uiHelpMenu/uiHelpMenu.h"
#include "uiHelpWindow/uiHelpWindow.h"
#include "BattleLobby.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDialog.h"
    #include "GuildSystem/uiGuildWareHouse.h"
#else
    #include "BankDialog.h"
    #include "inputnumbankdialog.h"
    #include "uiGuildWareHouse/uiGuildWareHouse.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "inputnumtradedialog.h"
#include "ChatDialog.h"
#include "uiQuestMan/uiQuestMan.h"
#include "uiItemTotalComposite/uiItemTotalComposite.h"
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
	#include "uiVendorBuyPopup.h"
#endif
#include "uiVendorList/uiVendorList.h"
#include "ItemManager.h"
#include "MouseHandler.h"
#include "KeyQueueManager.h"
#include "RadarInfoParser.h"
#include "RadarFullDialog.h"
#include "Application.h"
#include "SolarDateTime.h"
#include "uiPasswordMan/uiPassWordMan.h"
#include "uiPasswordDlg/uiPasswordDlg.h"
#include "uiMenuMan/uiMenuMan.h"
#include "uiRadarMan/uiRadarMan.h"
#include "InventoryDialog.h"
    #include "uiCashItemShopMan/uiCashItemShopMan.h"
#ifdef __CN_0_20070507_SHOP_MILEAGE
	    #include "ShopDefine.h"
#endif	//	__CN_0_20070507_SHOP_MILEAGE

#include "uiBankMan/uiBankMan.h"
#include "uiInventoryMan/uiInventoryMan.h"
#include "Message.h"
#include "uiHelpMan/uiHelpMan.h"
#include "uiLoginMan/uiLoginMan.h"

#ifdef __CN_269_MATRIX_CARD_AUTH
	#include "MatrixCardAuth.h"	
#endif //__CN_269_MATRIX_CARD_AUTH
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
    #include "GuildSystem/GuildSystemData.h"
    #include "GuildSystem/GuildSystemFunction.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
    #include "uiGuildMenu/uiGuildMenu.h"
    #include "uiGuildCreate/uiGuildCreate.h"
    #include "uiGuildWindow/uiGuildWindow.h"
    #include "uiGuildMain/uiGuildMain.h"
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiWayPointMan/uiWayPointMan.h"
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uiMissionRankMan2/uiMissionRankMan2.h"
#else
#include "uiMissionRankMan/uiMissionRankMan.h"
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "MapInfoParser.h"
#include "uiPartyMan/uiPartyMan.h"
#include "GameConst_Color.h"
#include "ClientVer.h"
#include <PacketControl/PacketControl.h>
#include "CharacterScene.h"
#include "LoadScene.h"
#include "uiSSQMan/uiSSQManager.h"
#include "ItemFormula.h"

#ifdef __NA_001358_ENCHANT_UNIFICATION
    #include "InstantEnchantParser.h"
    #include "uiEnchantMan/uiEnchantMan.h"
#else
    #include "EnchantParser.h"
#endif //__NA_001358_ENCHANT_UNIFICATION

#include "uiPersonalWayPointMan/uiPersonalWayPointMan.h"
#include "uiPersonalWayPointWindow/uiPersonalWayPointWindow.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"
#include "ProjectileManagerEx.h"
#include <SSQ/SSQPacketProc.h>
#include "uiChatMan/uiChatMan_def.h"
#include "ObjectManager.h"

#ifdef _NHN_USA_CHANNELING
 #include <HanAuth/HanReportForClient.h>
#include <HanAuth/HanAuthforClient.h>
#endif//_NHN_USA_CHANNELING(북미채널링)
                                                                  
#include "uiNoticeMan/uiNoticeMan.h"
#include "uiGuildNoticeDialog/uiGuildNoticeDialog.h"

#include "uiBattleScoreMan\uiBattleScoreMan.h"
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
#include "uiLoginOneTimePassword/uiLoginOneTimePassword.h"
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_

#include "uiToolTipMan/uiToolTipMan.h"
#include "ShopDialog.h"
#include "CastLeaveFieldDialog.h"
#include "ItemInfoParser.h"
#include "NPC.h"
#include "OptionListParser.h"

#if defined(__CN_20100909_SECURITY_TOKEN__)
    #include "uiLoginOneTimePassword/uiLoginOneTimePassword.h"
#endif //__CN_20100909_SECURITY_TOKEN__

#include "Item.h"
#include "ItemShareSystem.h"
#include "../PartyLib/PartyDefine.h"

#include "uiGuideSystemMan/uiGuideSystemMan.h"

#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2List/uiBattlezone2List.h"
#include "uiBattlezone2Tree/uiBattlezone2Tree.h"
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2Reservation/uiBattlezone2Reservation.h"
#endif//_DH_BATTLEZONE2_

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "BattleGroundInfoParser.h"
#include "ChaosZoneInfoParser.h"
#include "SystemDialog.h"
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "ChaosSystem/uiChaosZonePVP.h"
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
  #include "uiSmartNpcDialogMan/uiSmartNpcDialogMan.h"
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007086_20140318_MONSTERBOOK
  #include "uiMonsterBookMan/uiMonsterBookMan.h"
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
  #include "uiAttendanceCheck/uiAttendanceCheckMan.h"
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "uiSUNRankingMan/uiSUNRankingMan.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
#include "uiMiniGameMan/uiMiniGameMan.h"
#endif // _NA_008405_20150716_MINIGAME_BINGO

#include "DominationInfoParser.h"
#include <winconsole.h>
#include <time.h>
#include "GlobalFunc.h"

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "uiDominationTimer/uiDominationTimer.h"
#endif //_NA_008334_20150608_SONNENSCHEIN


#ifdef _JBH_MOD_MONEY_OUTPUT
	#define MONEY_COLOR_1_000			WzColor_RGBA(255,255,255,255)
	#define MONEY_COLOR_10_000			WzColor_RGBA(211,165,223,255)
	#define	MONEY_COLOR_100_000			WzColor_RGBA(8,252,54,255)
	#define MONEY_COLOR_1_000_000		WzColor_RGBA(235,227,117,255)
	#define MONEY_COLOR_10_000_000		WzColor_RGBA(44,64,252,255)
	#define MONEY_COLOR_100_000_000		WzColor_RGBA(255,1,1,255)
#endif

DWORD g_dwTotalLoadingTime = 0;
bool g_bLoadingTimeSave = false;
char g_szDevOption[1024] = {0};

#define MODFLOAT(ratio) ((ratio)<0 ? (-0.5f):(0.5f))

using namespace nsSSQ;
using namespace nsSSQ::Observer;


//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
GlobalFunc::SendPacketToGameServer( void * pBuf, int iLength )
{
	return SendPacket( CI_GAMESERVERIDX, pBuf, iLength );
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
GlobalFunc::SendPacketToWorldServer( void * pBuf, int iLength )
{
	return SendPacket( CI_CHATSERVERIDX, pBuf, iLength );
}
#ifdef _NHN_USA_CHANNELING
//-------------------------------------------------------------------------------------------
/** NHN_USA 지표보고패킷(존이동시작)
	401:필드	---> 필드	 
	500:필드	---> 배틀존 
	502:배틀존	---> 필드		 
	403:포탈이동
*/
int GlobalFunc::CheckMoveToZone(MAPID OldMapID,MAPID NewMapID,BOOL bPortal)
{
	//step1:현재존과 가야할존을 체크
	const sMAPINFO * pOld= MapInfoParser::Instance()->FindMapInfo(OldMapID);
	const sMAPINFO * pNew= MapInfoParser::Instance()->FindMapInfo(NewMapID);

	if(!pOld || !pNew)
	{
		return -1;
	}

	if(pOld->byMKind == eZONETYPE_MAX ||  pNew->byMKind == eZONETYPE_MAX)
	{
		return -1;
	}

	//step2:여기서부터 어떤존인지 값을 삽입한다.(enum정의 없이 eZoneType으로 일괄처리)
	BYTE arZone[2] = {pOld->byMKind ,pNew->byMKind };

	for(int i=0;i<2;++i)
	{
		switch(arZone[i])
		{
		//일반존이였을경우(마을.필드)
		case eZONETYPE_TUTORIAL_FIELD:
		case eZONETYPE_VILLAGE:
		case eZONETYPE_FIELD:
			{
				arZone[i] = eZONETYPE_FIELD;
			}
			break;
		//배틀존이였을경우(미션.헌팅.저탑.pvp)
		case eZONETYPE_MISSION:			
		case eZONETYPE_HUNTING:		
		case eZONETYPE_PVP:
		case eZONETYPE_CHUNTING:	
		case eZONETYPE_INSTANCE:
			{
				arZone[i] = eZONETYPE_MISSION;
			}
			break;
		default:
			{
				//디폴트는 이렇게 MAX(case 지정된값만 값을 변경)
				arZone[i] =  eZONETYPE_MAX;
			}
			break;
		}
	}

	//step3:존에대한판단을한다(eZONETYPE_MAX:case값(필드.배틀존)만판단)
	if(arZone[0] ==eZONETYPE_MAX || arZone[1] == eZONETYPE_MAX)
	{
		return -1;
	}
	
	int	nReportVal = -1;
	//현재 필드였었다면
	if(arZone[0]==eZONETYPE_FIELD)
	{
		nReportVal = (arZone[1]==eZONETYPE_FIELD) ? (401):(500);
	}
	else
	{
		//(배틀존->배틀존은 보고안함)
		nReportVal = (arZone[1]==eZONETYPE_FIELD) ? (502):(-1);
	}

	if(nReportVal!=-1)	
	{
		if(bPortal)
		{
			nReportVal = 403;
		}
		//존이동시작보고를 날린다
		SendHanReportPacket(nReportVal);
		return nReportVal;
	}
	return -1;
}
//-------------------------------------------------------------------------------------------
/** NHN_USA 서비스 지표보고패킷
*/
int GlobalFunc::SendHanReportPacket(int nVal)
{
	//300:클라 시작			//301:클라 종료
	//305:캐릭터 선택		//305:캐릭터 선택종료
	//310:채널접속			//311:채널접속 종료

	int nRet = 0;
	
	if(g_szGameString)
	{
		char szMemberId[21]={0x00}; //21 is Max
		nRet = HanAuthGetId(g_szGameString,szMemberId,sizeof(szMemberId));
		char szReportString[INTERFACE_STRING_LENGTH]={0x00};
		Snprintf (szReportString,INTERFACE_STRING_LENGTH-1, "id=%s&value=%d",szMemberId,nVal);
		nRet = HanReportSendPosLog(szReportString);
	}
	return nRet;
}
#endif//_NHN_USA_CHANNELING(북미채널링)


//-------------------------------------------------------------------------------------------
/** RIDER 라이딩 애니메이션
*/
WzID GlobalFunc::GetRiderAnimationByPlayerAni(ePLAYER_ANI eState, bool is_spread)
{
	WzID wzID = NULL;

	switch(eState)
	{
    case eANI_STAND:			wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N101"); break; //기본스탠드
    case eANI_IDLE:				wzID = (is_spread == false) ? StrToWzID("N002") : StrToWzID("N102"); break; //idle 
    case eANI_IDLE2:			wzID = (is_spread == false) ? StrToWzID("N003") : StrToWzID("N103"); break; //idle2 
    case eANI_SIT:				wzID = (is_spread == false) ? StrToWzID("N004") : StrToWzID("N104"); break; //앉기
    case eANI_SIT_IDLE:			wzID = (is_spread == false) ? StrToWzID("N005") : StrToWzID("N105"); break; //앉기idle
    case eANI_GETUP:			wzID = (is_spread == false) ? StrToWzID("N016") : StrToWzID("N116"); break; //서기
    case eANI_RUN:			
        {
            //뛰기
            //wzID = (is_spread == false) ? StrToWzID("N007") : StrToWzID("N107"); 
            if (is_spread == true)
            {
                if (g_HeroInput.IsLeftKeyPressed())
                {
                    wzID = StrToWzID("N120");
                }
                else if (g_HeroInput.IsRightKeyPressed())
                {
                    wzID = StrToWzID("N121");
                }
                else
                {
                    wzID = StrToWzID("N107"); 
                }
            }
            else
            {
                wzID = StrToWzID("N007");
            }
        }
        break; //뛰기
    case eANI_R_MOVE:			wzID = (is_spread == false) ? StrToWzID("N010") : StrToWzID("N110"); break; //오른쪽이동
    case eANI_L_MOVE:			wzID = (is_spread == false) ? StrToWzID("N009") : StrToWzID("N109"); break; //왼쪽이동
    case eANI_B_MOVE:			wzID = (is_spread == false) ? StrToWzID("N011") : StrToWzID("N111"); break; //뒤로이동
    case eANI_R_ROTATE:			wzID = (is_spread == false) ? StrToWzID("N013") : StrToWzID("N113"); break; //우회전
    case eANI_L_ROTATE:			wzID = (is_spread == false) ? StrToWzID("N012") : StrToWzID("N112"); break; //좌회전
    case eANI_JUMP_START:		wzID = (is_spread == false) ? StrToWzID("J001") : StrToWzID("J101"); break; //점프
    case eANI_JUMP_LOOP:		wzID = (is_spread == false) ? StrToWzID("J002") : StrToWzID("J102"); break; //점프중
    case eANI_JUMP_END:			wzID = (is_spread == false) ? StrToWzID("J003") : StrToWzID("J103"); break; //착지(제자리)
    case eANI_JUMP_LAND:		wzID = (is_spread == false) ? StrToWzID("J004") : StrToWzID("J104"); break; //착지(이동중)
    case eANI_HJUMP_END:		wzID = (is_spread == false) ? StrToWzID("J005") : StrToWzID("J105"); break; //높은곳착지
    case eANI_HJUMP_END_LOOP:	wzID = (is_spread == false) ? StrToWzID("J006") : StrToWzID("J106"); break; //높으곳착지후 잠시대기
    case eANI_HJUMP_GETUP:		wzID = (is_spread == false) ? StrToWzID("J007") : StrToWzID("J107"); break; //높은곳착지후 일어나기 
    case eANI_RIDER_SPREAD:		wzID = StrToWzID("F001"); break;    //!< 탈것이 펼때
    case eANI_RIDER_FURL:		wzID = StrToWzID("F002"); break;    //!< 탈것이 접을때
    case eANI_SPREAD_OTHERMOTION_MOVE:    wzID = StrToWzID("J110"); break; //!< MOVE 의 다른모션(SPACE KEY누르면 취하는 모션)
    case eANI_SPREAD_OTHERMOTION_IDLE:    wzID = StrToWzID("J111"); break; //!< IDLE 의 다른모션(SPACE KEY누르면 취하는 모션)
	}      

	return wzID;
}
//-------------------------------------------------------------------------------------------
/** PLAYER 라이딩 애니메이션
*/
WzID GlobalFunc::GetPlayerAnimationByRiding(ePLAYER_ANI eState, char cType)
{
	tagWZID AnimID;

	switch(eState)
	{
	case eANI_STAND:			AnimID.id =  StrToWzID("T001"); break; //기본스탠드
	case eANI_IDLE:				AnimID.id =  StrToWzID("T002"); break; //idle	
    case eANI_IDLE2:            AnimID.id =  StrToWzID("T003"); break; //idle2
	case eANI_SIT:				AnimID.id =  StrToWzID("T004"); break; //앉기
	case eANI_SIT_IDLE:			AnimID.id =  StrToWzID("T005"); break; //앉기idle
	case eANI_GETUP:			AnimID.id =  StrToWzID("T016"); break; //서기
	case eANI_RUN:				AnimID.id =  StrToWzID("T007"); break; //뛰기
	case eANI_L_MOVE:			AnimID.id =  StrToWzID("T009"); break; //왼쪽이동
	case eANI_R_MOVE:			AnimID.id =  StrToWzID("T010"); break; //오른쪽이동
	case eANI_B_MOVE:			AnimID.id =  StrToWzID("T011"); break; //뒤로이동
	case eANI_R_ROTATE:			AnimID.id =  StrToWzID("T013"); break; //우회전
	case eANI_L_ROTATE:			AnimID.id =  StrToWzID("T012"); break; //좌회전
	case eANI_JUMP_START:		AnimID.id =  StrToWzID("T051"); break; //점프
	case eANI_JUMP_LOOP:		AnimID.id =  StrToWzID("T052"); break; //점프중
	case eANI_JUMP_END:			AnimID.id =  StrToWzID("T053"); break; //착지(제자리)
	case eANI_JUMP_LAND:		AnimID.id =  StrToWzID("T054"); break; //착지(이동중)
	case eANI_HJUMP_END:		AnimID.id =  StrToWzID("T055"); break; //높은곳착지
	case eANI_HJUMP_END_LOOP:	AnimID.id =  StrToWzID("T056"); break; //높으곳착지후 잠시대기
	case eANI_HJUMP_GETUP:		AnimID.id =  StrToWzID("T057"); break; //높은곳착지후 일어나기	
    case eANI_RIDER_SPREAD:		AnimID.id =  StrToWzID("T070"); break; //탈것이 펼때
    case eANI_RIDER_FURL:		AnimID.id =  StrToWzID("T071"); break; //탈것이 접을때

    //! IDLE/MOVE 의 다른모션(SPACE KEY누르면 취하는 모션)
    //! J110/J111 애니코드는 탈것 종류에 상관없이 모두 같이 사용하게 함
    case eANI_SPREAD_OTHERMOTION_MOVE:  return StrToWzID("J110");
    case eANI_SPREAD_OTHERMOTION_IDLE:  return StrToWzID("J111");
	}                                                                 
	AnimID.code[1] = cType;

	return AnimID.id;
}

//------------------------------------------------------------------------------ 
WzID GlobalFunc::GetAnimationByPolymorph(ePLAYER_ANI eState)
{
	switch( eState )
	{
	case eANI_RUN:				
	case eANI_L_MOVE:	//왼쪽이동
	case eANI_R_MOVE:	//오른쪽이동
	case eANI_B_MOVE:	//뒤로이동
	case eANI_L_ROTATE:	//좌회전
	case eANI_R_ROTATE:	//우회전
		return StrToWzID("N007");
	}

	return StrToWzID("N001");
}

//------------------------------------------------------------------------------ 
WzID GlobalFunc::GetPlayerAnimationBySpreadWings(ePLAYER_ANI eState)
{
    WzID wzID = NULL;

    switch(eState)
    {
    case eANI_STAND:			wzID = StrToWzID("N501"); break;//기본스탠드
    case eANI_IDLE:				wzID = StrToWzID("N502"); break; //idle	
    case eANI_IDLE2:			wzID = StrToWzID("N502"); break; //idle	
    case eANI_SIT:				wzID = StrToWzID("N502"); break; //앉기
    case eANI_SIT_IDLE:			wzID = StrToWzID("N502"); break; //앉기idle
    case eANI_GETUP:			wzID = StrToWzID("N502"); break; //서기
    case eANI_RUN:				
        {
            //뛰기
            if (g_HeroInput.IsLeftKeyPressed())
            {
                wzID = StrToWzID("N522");
            }
            else if (g_HeroInput.IsRightKeyPressed())
            {
                wzID = StrToWzID("N523");
            }
            else
            {
                wzID = StrToWzID("N507");
            }
        }
        break; 
    case eANI_L_MOVE:			wzID = StrToWzID("N509"); break; //왼쪽이동
    case eANI_R_MOVE:			wzID = StrToWzID("N510"); break; //오른쪽이동
    case eANI_B_MOVE:			wzID = StrToWzID("N511"); break; //뒤로이동
    case eANI_L_ROTATE:			wzID = StrToWzID("N512"); break; //좌회전
    case eANI_R_ROTATE:			wzID = StrToWzID("N513"); break; //우회전
    case eANI_JUMP_START:		wzID = StrToWzID("J501"); break; //점프
    case eANI_JUMP_LOOP:		wzID = StrToWzID("J502"); break; //점프중
    case eANI_JUMP_END:			wzID = StrToWzID("J503"); break; //착지(제자리)
    case eANI_JUMP_LAND:		wzID = StrToWzID("J504"); break; //착지(이동중)
    case eANI_HJUMP_END:		wzID = StrToWzID("J505"); break; //높은곳착지
    case eANI_HJUMP_END_LOOP:	wzID = StrToWzID("J506"); break; //높으곳착지후 잠시대기
    case eANI_HJUMP_GETUP:		wzID = StrToWzID("J507"); break; //높은곳착지후 일어나기	
    case eANI_WINGS_SPREAD:	    wzID = StrToWzID("N520"); break; //!< 날개 펼때
    case eANI_WINGS_FURL:		wzID = StrToWzID("N521"); break; //!< 날개 접을때
    case eANI_SPREAD_OTHERMOTION_MOVE:  wzID = StrToWzID("J508"); break; //!< MOVE 의 다른모션(SPACE KEY누르면 취하는 모션)
    case eANI_SPREAD_OTHERMOTION_IDLE:  wzID = StrToWzID("J509"); break; //!< IDLE 의 다른모션(SPACE KEY누르면 취하는 모션)
    }                                                                                                      
                                           
    return wzID;                           
}
//------------------------------------------------------------------------------ 
WzID GlobalFunc::GetWingsAnimationByPlayerAni(ePLAYER_ANI eState, bool is_spread)
{
    WzID wzID = NULL;

    switch(eState)
    {
    case eANI_STAND:			wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N501");break;//기본스탠드
    case eANI_IDLE:				wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N502"); break; //idle	
    case eANI_IDLE2:			wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N502"); break; //idle	
    case eANI_SIT:				wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N502"); break; //앉기
    case eANI_SIT_IDLE:			wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N502"); break; //앉기idle
    case eANI_GETUP:			wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N502"); break; //서기
    case eANI_RUN:				wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N507"); break; //뛰기
    case eANI_L_MOVE:			wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N509"); break; //왼쪽이동
    case eANI_R_MOVE:			wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N510"); break; //오른쪽이동
    case eANI_B_MOVE:			wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N511"); break; //뒤로이동
    case eANI_L_ROTATE:			wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N512"); break; //좌회전
    case eANI_R_ROTATE:			wzID = (is_spread == false) ? StrToWzID("N001") : StrToWzID("N513"); break; //우회전
    case eANI_JUMP_START:		wzID = (is_spread == false) ? StrToWzID("J001") : StrToWzID("J501"); break; //점프
    case eANI_JUMP_LOOP:		wzID = (is_spread == false) ? StrToWzID("J002") : StrToWzID("J502"); break; //점프중
    case eANI_JUMP_END:			wzID = (is_spread == false) ? StrToWzID("J003") : StrToWzID("J503"); break; //착지(제자리)
    case eANI_JUMP_LAND:		wzID = (is_spread == false) ? StrToWzID("J004") : StrToWzID("J504"); break; //착지(이동중)
    case eANI_HJUMP_END:		wzID = (is_spread == false) ? StrToWzID("J005") : StrToWzID("J505"); break; //높은곳착지
    case eANI_HJUMP_END_LOOP:	wzID = (is_spread == false) ? StrToWzID("J006") : StrToWzID("J506"); break; //높으곳착지후 잠시대기
    case eANI_HJUMP_GETUP:		wzID = (is_spread == false) ? StrToWzID("J007") : StrToWzID("J507"); break; //높은곳착지후 일어나기	
    case eANI_WINGS_SPREAD:	    wzID = StrToWzID("N520"); break; //!< 날개 펼때
    case eANI_WINGS_FURL:		wzID = StrToWzID("N521"); break; //!< 날개 접을때
    case eANI_SPREAD_OTHERMOTION_MOVE:  wzID = StrToWzID("J508"); break; //!< MOVE 의 다른모션(SPACE KEY누르면 취하는 모션)
    case eANI_SPREAD_OTHERMOTION_IDLE:  wzID = StrToWzID("J509"); break; //!< IDLE 의 다른모션(SPACE KEY누르면 취하는 모션)

    }                                                                                                      

    return wzID;                           
}
//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
GlobalFunc::SendPacketToAuthServer( void * pBuf, int iLength )
{
	return SendPacket( CI_AUTHSERVERIDX, pBuf, iLength );
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL GlobalFunc::SendPacket(eCONNECTION_INDEX idx, void * pBuf, int iLength )
{
	using namespace nsPacket;
	if( g_pNetwork && GENERALPARAM->IsNet() )
	{
		if ( g_pNetwork->IsConnected(idx) )
		{
//#ifdef _DEV_VER
//			if(IsShowNetSendLog())
//			{
//				MSG_BASE * pPacket = (MSG_BASE * )pBuf;
//
//				char szText[1024] = {0,};
//
//				Sprintf(szText, "[SEND] Idx : %d, Catagory : %d, Protocol : %d, Size : %d \n\r", 
//					idx, pPacket->m_byCategory, pPacket->m_byProtocol, iLength);
//
//				OutputDebugString(szText);
//			}
//
    #ifdef _DEBUG //#ifdef _LTJ_PACKET_SHOW_FOR_DEBUG
            if ((g_pPacketShow) && IsShowPacketLog())
            {
                MSG_BASE* packet = (MSG_BASE*)pBuf;
                if( const CHAR* msg_str = g_pPacketShow->fnQuery(packet->m_byCategory, packet->m_byProtocol, NULL) )
                {
                    bool writelog = true;

                    if( packet->m_byCategory == CG_SYNC )
                        writelog = false;

                    if( packet->m_byCategory == CG_CONNECTION && packet->m_byProtocol == CG_CONNECTION_HEARTBEAT_SYN )
                        writelog = false;

                    if( packet->m_byCategory == CW_CONNECTION && packet->m_byProtocol == CW_HEARTBEAT )
                        writelog = false;

                    if( writelog == true )
                    {
                        Write_Packet_Log("[SEND] %s\n", msg_str);

    #if (WZENGINEVER >= 480)
                        WZLOG( WZPKT2, msg_str );
    #endif
                    }
                }
            }
    #endif //_LTJ_PACKET_SHOW_FOR_DEBUG

#ifdef _NA002676_WORLD_SERVER_RENEWAL
            // CHANGES: CI_CHATSERVERIDX same with CI_GAMESERVERIDX
            // since _NA002676_WORLD_SERVER_RENEWAL.
            // To support the accurate world service synchronization added the CW protocol filters
            if (idx == CI_GAMESERVERIDX)
            {
                if (g_pNetwork->CheckSendableMessage(\
                        reinterpret_cast<MSG_BASE*>(pBuf)->m_byCategory) == false)
                {
                    return false;
                };
            };
#endif
			//if( CI_GAMESERVERIDX == idx )
			//{
			//	//PacketInfo packetInfo = PacketControl::Pack( (MSG_BASE *)pBuf, iLength );
			//	return g_pNetwork->Send( idx, (VOID*)pBuf, (INT)iLength );
			//}
            BOOL ret = g_pNetwork->Send( idx, pBuf, iLength );
			return ret;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
void GlobalFunc::SendGMCommand( const std::string& CommandText )
{
    if( CommandText.size() >= MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH )
        return;

    MSG_CG_GM_STRING_CMD_SYN msg;
    ZeroMemory(msg.m_szStringCmd, MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
    StrnCopy(msg.m_szStringCmd, CommandText.c_str(), CommandText.size() );
    GlobalFunc::SendPacket(CI_GAMESERVERIDX,&msg,sizeof(msg));
}


//----------------------------------------------------------------------------
/**
*/
void 
GlobalFunc::PLAY_BGM(int iType, CODETYPE FieldCode, eZONETYPE eRoomType)
{
	BASE_BGMSoundInfo* pInfo = BGMSoundInfoParser::Instance()->GetBGMInfo(iType, (WORD)FieldCode, eRoomType); 

	if (pInfo != NULL)
	{
        // BGMInfo 스크립트에서 BGM코드가 0이면 BGM 을 플레이하지 않는다.
		g_SoundBGM.Stop();
        if (pInfo->m_wBGMNum != 0)
        {
            g_SoundBGM.Play(pInfo->m_wBGMNum);
        }
	}
	else
	{
#ifdef _DEV_VER
		OutputMsg_Notice(eOUTPUT_MSG_CHAT, "[FieldCode: %d]Failed BGM Play", FieldCode);
#endif //_DEV_VER
	}
}

void GlobalFunc::InitRandom()
{
	srand( (unsigned int)time(NULL));
}

int GlobalFunc::GetRandom(int nRandomLimitNum)
{
	assert(nRandomLimitNum > 0);

	return (rand() % nRandomLimitNum);
}

//-------------------------------------------------------------------------------------------
/**
*/
DWORD 
GlobalFunc::WzColorToRGBA(WzColor wzColor)
{
	DWORD dwResult = 0;
	dwResult = 
		RGBA( 
			Red_WzColor(wzColor), 
			Green_WzColor(wzColor), 
			Blue_WzColor(wzColor), 
			Alpha_WzColor(wzColor)  
		);
	return dwResult;
}

//-------------------------------------------------------------------------------------------
/**
*/
WzColor 
GlobalFunc::RGBAToWzColor(DWORD dwColor)
{	
	WzColor wzResultColor = 
		WzColor_RGBA( GetRValue(dwColor),GetGValue(dwColor),GetBValue(dwColor),GetAValue(dwColor) );
	return wzResultColor;	
}

//----------------------------------------------------------------------------
/**
*/
void
GlobalFunc::DeadConfirmInHunting(bool bYes)
{
	if (bYes)
	{
        if (g_pHero && g_pHero->IsDead())
        {
            // 제자리 부활
            MSG_CG_STATUS_RESURRECTION_SYN	SendResurrectSyn;				// 부활 요청
            SendPacket( CI_GAMESERVERIDX, &SendResurrectSyn, sizeof(SendResurrectSyn));
        }
	}
}

//------------------------------------------------------------------------------
/**
*/
void
GlobalFunc::DeadConfirmInCHunting(bool bYes)
{
    if (bYes)
    {
        if (g_pHero && g_pHero->IsDead())
        {
            // 제자리 부활
            MSG_CG_STATUS_RESURRECTION_SYN	SendResurrectSyn;				// 부활 요청
            SendPacket( CI_GAMESERVERIDX, &SendResurrectSyn, sizeof(SendResurrectSyn));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GlobalFunc::DeadConfirmInCHuntingBeforeStart(bool bYes)
{
    if (bYes)
    {
        if (g_pHero && g_pHero->IsDead())
        {
            // 시작위치 부활
		    MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN SendSafeZonePacket;
		    SendPacket( CI_GAMESERVERIDX, &SendSafeZonePacket, sizeof(SendSafeZonePacket));
        }
    }
}


//-------------------------------------------------------------------------------------------
/**
*/
void 
GlobalFunc::GetStateInitItemNumConfirm(bool bValid, const char * pString)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];

	MouseHandler::Instance()->SetTryUseChageItem(FALSE);

	if (bValid)
	{
		if ( !pString )
			return;

		int iNum = atoi(pString);

		POSTYPE FromPos = MouseHandler::Instance()->GetPendingUseChargeItemPos();
		SCSlotContainer * pContainer = ItemManager::Instance()->GetContainer( SI_INVENTORY );
		if (!pContainer)
			return;

		// 위치 체크
		if ( FromPos >= pContainer->GetMaxSlotNum() || FromPos < 0)
			return;

		// 비워있는지 체크
		if ( pContainer->IsEmpty(FromPos) )
			return;

		if ( iNum <= 0 )
		{
			// 5887	사용할 아이템의 수량을 입력해주세요
			g_InterfaceManager.GetInterfaceString( 5887, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage );
			return;
		}

		SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot(FromPos);		
		if ( rItemSlot.GetNum() < iNum )
		{
			//5889	사용 가능한 아이템 개수를 초과하였습니다.	
			g_InterfaceManager.GetInterfaceString( 5889, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage );
			return;
		}

		// TODO : 기타 예외처리를 하고 정상적인 행동일때만 패킷 보낸다.
		ItemManager::Instance()->SendStatInitItem(
			MouseHandler::Instance()->GetPendingUseChargeItemPos(), 
			iNum );
	}	
}
//
////-------------------------------------------------------------------------------------------
///**
//*/
//void 
//GlobalFunc::DeadConfirmInField(bool bYes)
//{   
//	// 아무 행동도 하지 않는다. 
//	if ( bYes )
//	{
//        if (g_pHero && g_pHero->IsDead())
//        {
//		    MSG_CG_ZONE_RETURN_VILLAGE_SYN	SendLeaveSyn;
//		    SendPacket( CI_GAMESERVERIDX, &SendLeaveSyn, sizeof(SendLeaveSyn));
//        }
//	}
//	else
//	{
//		// 대기~
//		MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN SendSafeZonePacket;
//		SendPacket( CI_GAMESERVERIDX, &SendSafeZonePacket, sizeof(SendSafeZonePacket));
//	}
//}
//------------------------------------------------------------------------------ 
void GlobalFunc::DeadConfirmInField(int confirm_value)
{   
    switch (confirm_value)
    {
    case kConfirmSelect1:
        {
            if (g_pHero && g_pHero->IsDead())
            {
                MSG_CG_ZONE_RETURN_VILLAGE_SYN	SendLeaveSyn;
                SendPacket( CI_GAMESERVERIDX, &SendLeaveSyn, sizeof(SendLeaveSyn));
            }
        }
        break;
    case kConfirmSelect2:
        {
            MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN SendSafeZonePacket;
            SendPacket( CI_GAMESERVERIDX, &SendSafeZonePacket, sizeof(SendSafeZonePacket));
        }
        break;
    case kConfirmClose:
        {
            // no operation
        }
        break;
    }
}
#ifdef _DH_BATTLEZONE2_
void GlobalFunc::PartyRoomJoinConfirm(bool confirm_value)
{
    uiBattlezone2Tree* battlezone_tree_dialog = 
        GET_CAST_DIALOG(uiBattlezone2Tree, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE);

    uiBattlezone2Reservation* battlezone_reservation_dialog = 
        GET_CAST_DIALOG(uiBattlezone2Reservation, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_RESERVATION);

    if (confirm_value)
    {
        uiBattlezone2List* battlezone_list_dialog = 
            GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);

        if (battlezone_list_dialog && battlezone_tree_dialog)
        {
            battlezone_list_dialog->EnterRoom(battlezone_tree_dialog->save_party_key(), NULL, NULL);
            battlezone_tree_dialog->set_save_party_key(0);//설정 초기화
            battlezone_tree_dialog->set_is_creating(false);
        }
    }
    else
    {
        if (battlezone_tree_dialog)
        {
            if (battlezone_tree_dialog->is_creating())
            {
                battlezone_tree_dialog->CreateRoom();
            }
        }
        if (battlezone_reservation_dialog)
        {
            if (battlezone_reservation_dialog->is_reservation())
            {
                battlezone_reservation_dialog->ReservationStandByRoom();
            }
        }
    }    
}

void GlobalFunc::PartyContinueConfirm(bool confirm_value)
{
    uiBattlezone2List* battlezone_list_dialog = 
        GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);

    if (battlezone_list_dialog && confirm_value)
    {
        MSG_CG_PARTY_INVITE_PARTY_ROOM_ANS send_packet;
        send_packet.join_party_key_ = battlezone_list_dialog->follow_party_key();
        send_packet.member_response_ = confirm_value;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }
}

void GlobalFunc::PartyLeaveAndJoinConfirm(bool confirm_value)
{
    uiBattlezone2Tree* battlezone_tree_dialog = 
        GET_CAST_DIALOG(uiBattlezone2Tree, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE);

    uiBattlezone2List* battlezone_list_dialog = 
        GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);

    if (confirm_value)
    {
        if (battlezone_tree_dialog &&battlezone_list_dialog)
        {
            if (battlezone_tree_dialog->is_creating())
            {
                battlezone_tree_dialog->CreateRoom();
            }
            else
            {
                battlezone_list_dialog->EnterRoom(battlezone_tree_dialog->save_party_key(), NULL, NULL);
                battlezone_tree_dialog->set_save_party_key(0);//설정 초기화
            }
        }
    }
    else
    {
        //nop
    }    
}
#endif//_DH_BATTLEZONE2_
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
void GlobalFunc::PartyLeaveAndFreePVPJoinConfirm(bool confirm_value)
{
    if (confirm_value)
    {
        uiChaosZonePVP* pvp_dialog = GET_CAST_DIALOG(uiChaosZonePVP, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PVP);
        if (pvp_dialog)
        {
            pvp_dialog->SendFreePVPJoinPacket();
        }
    }
    else
    {
        //nop
    } 
}
void GlobalFunc::PartyLeaveAndFreePVPCreateConfirm(bool confirm_value)
{
    if (confirm_value)
    {
        uiChaosZonePVP* pvp_dialog = GET_CAST_DIALOG(uiChaosZonePVP, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PVP);
        if (pvp_dialog)
        {
            pvp_dialog->SendFreePVPCreatePacket();
        }
    }
    else
    {
        //nop
    } 
}
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
//------------------------------------------------------------------------------
/**
*/

void GlobalFunc::_fn_uiBankPasswordLockingSyn(bool bYes)
{
	uiBankMan* pBankMan =
		static_cast<uiBankMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BANK));
	assert (pBankMan);

	if(pBankMan)
	{
		if (bYes)
		{
			pBankMan->Send_CG_WAREHOUSE_PWD_LOCKING_SYN();
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
void GlobalFunc::_fn_uiInvenPasswordLockingSyn(bool bYes)
{
	uiInventoryMan* pInvenMan = GET_DIALOG_MANAGER( uiInventoryMan, UIMAN_INVENTORY );
		//static_cast<uiInventoryMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_INVENTORY));
	assert (pInvenMan);

	if(pInvenMan)
	{
		if (bYes)
		{
			pInvenMan->Send_CG_ITEM_INVENTORY_PWD_LOCKING_SYN();
		}
        else
        {
            pInvenMan->set_open_password_confirm(false);
        }
	}
}

void GlobalFunc::_fn_uiBankPassword_Clear(bool bYes)
{
	uiBankMan* pBankMan =
		static_cast<uiBankMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BANK));
	assert (pBankMan);

	if(pBankMan)
	{
		if (bYes)
		{
			pBankMan->Send_CG_WAREHOUSE_PWD_CHECK_SYN( MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_CLEAR);
		}
		else
		{
			uiPasswordMan* pPasswordMan =
				static_cast<uiPasswordMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD));
			assert (pPasswordMan);
			if(pPasswordMan)
			{
				//	비밀번호를 입력해주세요
				pBankMan->SetPasswordState(eBankDlgState_Check_Unlock);

                BOOL showPassChgBtn = TRUE;

				showPassChgBtn = ( pBankMan->GetBankLockState() & MSG_CG_WAREHOUSE_START_ACK::DISABLED );

				pPasswordMan->PasswordDlg(5551, GlobalFunc::_fn_uiBankPassword, 4, 8, showPassChgBtn, ePassWordCalledState_WareHouse);
			}
		}
	}
}

void GlobalFunc::_fn_uiInvenPassword_Clear(bool bYes)
{
	uiInventoryMan* pInvenMan = GET_DIALOG_MANAGER( uiInventoryMan, UIMAN_INVENTORY );
		//static_cast<uiInventoryMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_INVENTORY));
	assert (pInvenMan);

	if(pInvenMan)
	{
		if (bYes)
		{
			pInvenMan->Send_CG_ITEM_INVENTORY_PWD_CHECK_SYN( MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_CLEAR);
		}
		else
		{
			uiPasswordMan* pPasswordMan =
				static_cast<uiPasswordMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD));
			assert (pPasswordMan);
			if(pPasswordMan)
			{
				//	비밀번호를 입력해주세요
				pInvenMan->SetPasswordState(eInventoryDlgState_Check_Unlock);

                BOOL showPassChgBtn = TRUE;

				showPassChgBtn = ( pInvenMan->GetInventoryLockState() & MSG_CG_WAREHOUSE_START_ACK::DISABLED );

				pPasswordMan->PasswordDlg(5551, GlobalFunc::_fn_uiInvenPassword, 4, 8, showPassChgBtn, ePassWordCalledState_Inven);
			}
		}
	}
}

void GlobalFunc::_fn_uiBankPassword(bool bYes)
{
	uiPasswordMan* pPasswordMan =
		static_cast<uiPasswordMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD));
	assert (pPasswordMan);

	uiBankMan* pBankMan =
		static_cast<uiBankMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BANK));
	assert (pBankMan);

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };

	if (bYes)
	{
		if(pPasswordMan && pBankMan)
		{
			switch(pBankMan->GetPasswordState())
			{

			case eBankDlgState_Setting_SSN:
				{
					pBankMan->Send_CG_WAREHOUSE_PWD_SETTING_SYN(MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_WITH_SSN, pPasswordMan->m_pUIPasswordDlg->GetStrPassword());
				}
				break;

			case eBankDlgState_Check_Clear:
				{
					pBankMan->Send_CG_WAREHOUSE_PWD_CHECK_SYN(MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_WITH_SSN, pPasswordMan->m_pUIPasswordDlg->GetStrPassword());
				}
				break;

			case eBankDlgState_Setting_PasswordCmp:
				{
					Snprintf(szMessage, INTERFACE_STRING_LENGTH, _T("%s"), pPasswordMan->m_pUIPasswordDlg->GetStrPassword());
					pBankMan->SetPasswordStr(szMessage);
					// 비밀번호 확인
                    BOOL showPassChgBtn = TRUE;

					showPassChgBtn = ( pBankMan->GetBankLockState() & MSG_CG_WAREHOUSE_START_ACK::DISABLED );

					pPasswordMan->PasswordDlg(5553, GlobalFunc::_fn_uiBankPassword, 4, MAX_WAREHOUSE_INVENTORY_PWD_LENGTH, showPassChgBtn, ePassWordCalledState_WareHouse);

					pBankMan->SetPasswordState(eBankDlgState_Setting_PasswordSend);
				}
				break;

			case eBankDlgState_Setting_PasswordSend:
				{
					Snprintf(szMessage, INTERFACE_STRING_LENGTH, _T("%s"), pPasswordMan->m_pUIPasswordDlg->GetStrPassword());
					if(StrnCmp(szMessage, pBankMan->GetPAsswordStr(), MAX_WAREHOUSE_INVENTORY_PWD_LENGTH) == NULL)
					{
						pBankMan->Send_CG_WAREHOUSE_PWD_SETTING_SYN(MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_WITH_PWD, pPasswordMan->m_pUIPasswordDlg->GetStrPassword());
					}
					else
					{
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							pBankMan->SetPasswordState(eBankDlgState_Setting_PasswordCmp);
							//	비밀번호가 틀렸습니다.
							g_InterfaceManager.GetInterfaceString( 5555, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, GlobalFunc::_fn_uiBankPassword_invalidPassword);
						}
					}
				}
				break;

			case eBankDlgState_Check_Unlock:
				{
					pBankMan->Send_CG_WAREHOUSE_PWD_CHECK_SYN( MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_WITH_PWD, pPasswordMan->m_pUIPasswordDlg->GetStrPassword() );
				}
				break;

			default:
				break;
			}
		}
	}
	else
	{
		if(pBankMan)
		{
			switch(pBankMan->GetPasswordState())
			{
			case eBankDlgState_Setting_SSN:
			case eBankDlgState_Setting_PasswordCmp:
			case eBankDlgState_Setting_PasswordSend:
				{
					pBankMan->Send_CG_WAREHOUSE_PWD_SETTING_SYN(MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_CANCEL);
				}
				break;
			case eBankDlgState_Check_Unlock:
			case eBankDlgState_Check_Clear:
				{
					pBankMan->Send_CG_WAREHOUSE_PWD_CHECK_SYN(MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_CANCEL);
				}
			}
			pBankMan->ClearPassword();
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void GlobalFunc::_fn_uiInvenPassword(bool bYes)
{
	uiPasswordMan* pPasswordMan = GET_DIALOG_MANAGER( uiPasswordMan, UIMAN_PASSWORD );
		//static_cast<uiPasswordMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD));
	assert (pPasswordMan);

	uiInventoryMan* pInvenMan = GET_DIALOG_MANAGER( uiInventoryMan, UIMAN_INVENTORY );
		//static_cast<uiInventoryMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_INVENTORY));
	assert (pInvenMan);

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };

	if (bYes)
	{
		if(pPasswordMan && pInvenMan)
		{
			switch(pInvenMan->GetPasswordState())
			{
			case eInventoryDlgState_Setting_SSN:
				{
					pInvenMan->Send_CG_ITEM_INVENTORY_PWD_SETTING_SYN(MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_WITH_SSN, pPasswordMan->m_pUIPasswordDlg->GetStrPassword());
				}
				break;

			case eInventoryDlgState_Check_Clear:
				{
					pInvenMan->Send_CG_ITEM_INVENTORY_PWD_CHECK_SYN(MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_WITH_SSN, pPasswordMan->m_pUIPasswordDlg->GetStrPassword());
				}
				break;

			case eInventoryDlgState_Setting_PasswordCmp:
				{
					Snprintf(szMessage, INTERFACE_STRING_LENGTH, _T("%s"), pPasswordMan->m_pUIPasswordDlg->GetStrPassword());
					pInvenMan->SetPasswordStr(szMessage);
					// 비밀번호 확인
                    BOOL showPassChgBtn = TRUE;

					showPassChgBtn = ( pInvenMan->GetInventoryLockState() & MSG_CG_WAREHOUSE_START_ACK::DISABLED );

                    pPasswordMan->PasswordDlg(5553, GlobalFunc::_fn_uiInvenPassword, 4, MAX_WAREHOUSE_INVENTORY_PWD_LENGTH, showPassChgBtn, ePassWordCalledState_Inven);
					pInvenMan->SetPasswordState(eInventoryDlgState_Setting_PasswordSend);
				}
				break;

			case eInventoryDlgState_Setting_PasswordSend:
				{
					Snprintf(szMessage, INTERFACE_STRING_LENGTH, _T("%s"), pPasswordMan->m_pUIPasswordDlg->GetStrPassword());
					if(StrnCmp(szMessage, pInvenMan->GetPAsswordStr(), MAX_WAREHOUSE_INVENTORY_PWD_LENGTH) == NULL)
					{
						pInvenMan->Send_CG_ITEM_INVENTORY_PWD_SETTING_SYN( MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_WITH_PWD, pPasswordMan->m_pUIPasswordDlg->GetStrPassword() );
					}
					else
					{
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							pInvenMan->SetPasswordState(eInventoryDlgState_Setting_PasswordCmp);
							//	비밀번호가 틀렸습니다.
							g_InterfaceManager.GetInterfaceString( 5555, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify( szMessage, GlobalFunc::_fn_uiInvenPassword_invalidPassword);
						}
					}
				}
				break;

			case eInventoryDlgState_Check_Unlock:
				{
					pInvenMan->Send_CG_ITEM_INVENTORY_PWD_CHECK_SYN( MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_WITH_PWD, pPasswordMan->m_pUIPasswordDlg->GetStrPassword() );
				}
				break;

			default:
				break;
			}
		}
	}
	else
	{
		if(pInvenMan)
		{
			switch(pInvenMan->GetPasswordState())
			{
			case eInventoryDlgState_Setting_SSN:
			case eInventoryDlgState_Setting_PasswordCmp:
			case eInventoryDlgState_Setting_PasswordSend:
					pInvenMan->Send_CG_ITEM_INVENTORY_PWD_SETTING_SYN(MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_CANCEL);
				break;

			case eInventoryDlgState_Check_Unlock:
			case eInventoryDlgState_Check_Clear:
					pInvenMan->Send_CG_ITEM_INVENTORY_PWD_CHECK_SYN(MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_CANCEL);
				break;
			}
			pInvenMan->ClearPassword();
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void GlobalFunc::_fn_uiBankPassword_invalidPassword( void )
{
	//	비밀번호를 입력해 주세요
	BOOL showPassChgBtn = TRUE;


	uiBankMan* pBankMan = static_cast<uiBankMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BANK));
	assert (pBankMan);
	if(pBankMan)
	{
		showPassChgBtn = ( pBankMan->GetBankLockState() & MSG_CG_WAREHOUSE_START_ACK::DISABLED );
	}

	uiPasswordMan* pPasswordMan =
		static_cast<uiPasswordMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD));
	assert (pPasswordMan);
	if(pPasswordMan)
	{
		pPasswordMan->PasswordDlg(5551, GlobalFunc::_fn_uiBankPassword, 4, MAX_WAREHOUSE_INVENTORY_PWD_LENGTH, showPassChgBtn, ePassWordCalledState_WareHouse);
	}
}

void GlobalFunc::_fn_uiBankPassword_invalidSSN( void )
{
	//	주민등록 번호를 입력해 주세요
	BOOL showPassChgBtn = TRUE;

	uiBankMan* pBankMan = static_cast<uiBankMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BANK));
	assert (pBankMan);
	if(pBankMan)
	{
		showPassChgBtn = ( pBankMan->GetBankLockState() & MSG_CG_WAREHOUSE_START_ACK::DISABLED );
	}

	uiPasswordMan* pPasswordMan =
		static_cast<uiPasswordMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD));
	assert (pPasswordMan);

	if(pPasswordMan)
	{
		pPasswordMan->PasswordDlg(5579, GlobalFunc::_fn_uiBankPassword, 0, MAX_SSN_COUNT, showPassChgBtn, ePassWordCalledState_WareHouse);
	}
}

//------------------------------------------------------------------------------
/**
*/
void GlobalFunc::_fn_uiInvenPassword_invalidPassword( void )
{	
	//	비밀번호를 입력해 주세요
	BOOL showPassChgBtn = TRUE;

	uiInventoryMan* pInvenMan = GET_DIALOG_MANAGER( uiInventoryMan, UIMAN_INVENTORY );//static_cast<uiInventoryMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_INVENTORY));
	assert (pInvenMan);

	if(pInvenMan)
	{
		showPassChgBtn = ( pInvenMan->GetInventoryLockState() & MSG_CG_WAREHOUSE_START_ACK::DISABLED );
	}

	uiPasswordMan* pPasswordMan =
		static_cast<uiPasswordMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD));
	assert (pPasswordMan);

	if(pPasswordMan)
	{
		pPasswordMan->PasswordDlg(5551, GlobalFunc::_fn_uiInvenPassword, 4, MAX_WAREHOUSE_INVENTORY_PWD_LENGTH, showPassChgBtn, ePassWordCalledState_Inven);
	}
}

void GlobalFunc::_fn_uiInvenPassword_invalidSSN( void )
{
	//	주민등록 번호를 입력해 주세요
	BOOL showPassChgBtn = TRUE;

	uiInventoryMan* pInvenMan = GET_DIALOG_MANAGER( uiInventoryMan, UIMAN_INVENTORY );//static_cast<uiInventoryMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_INVENTORY));
	assert (pInvenMan);

	if(pInvenMan)
	{
		showPassChgBtn = ( pInvenMan->GetInventoryLockState() & MSG_CG_WAREHOUSE_START_ACK::DISABLED );
	}

	uiPasswordMan* pPasswordMan =
		static_cast<uiPasswordMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD));
	assert (pPasswordMan);
	if(pPasswordMan)
	{
		pPasswordMan->PasswordDlg(5579, GlobalFunc::_fn_uiInvenPassword, 0, MAX_SSN_COUNT, showPassChgBtn, ePassWordCalledState_Inven);
	}
}


//------------------------------------------------------------------------------
/**
*/
void GlobalFunc::_fn_TutorialSkip(bool bYes)
{
	if (bYes)
	{
		uiMenuMan* puiMan = static_cast<uiMenuMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_MENU));

		if(puiMan)
		{
			if(TUTO_FIELD == g_pMap->GetMapID())
				puiMan->Send_CG_ZONE_EX_TUTORIAL_FIELD_LEAVE_SYN();
			else if(TUTO_MISSION == g_pMap->GetMapID())
				puiMan->Send_CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_SYN();

		}
	}
}

//----------------------------------------------------------------------------
/**
*/
void GlobalFunc::DeadVerifyInMission(void)
{
	// 아무 행동도 하지 않는다. 
}
//----------------------------------------------------------------------------
/**
*/
#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
void GlobalFunc::DeadConfirmInDomination(DWORD lParam, DWORD wParam)
{
    uiDominationTowerMan* domination_man = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (domination_man)
    {
        domination_man->ShowDominationResurrectionPopup();
    }
}
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
//----------------------------------------------------------------------------
/**
*/
void GlobalFunc::ShrinkBoundingVolume(WzBoundingVolume *pBV,float fRate)
{
	if (pBV->m_eBvType == BVT_AABB)
	{
		WzVector vCenter = (pBV->m_aabb.m_wvMin + pBV->m_aabb.m_wvMax) * 0.5f;
		WzVector vDiff = (pBV->m_aabb.m_wvMax - pBV->m_aabb.m_wvMin) * 0.5f;

		// 축소율 적용;
		vDiff *= fRate;

		pBV->m_aabb.m_wvMax = vCenter + vDiff;
		pBV->m_aabb.m_wvMin = vCenter - vDiff;
	}
	else if (pBV->m_eBvType == BVT_SPHERE)
	{
		pBV->m_BvSphere.m_fRadius *= fRate;
	}
}
//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::SystemExitConfirm(bool bYes)
{
	if (bYes)
	{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        GuildSystem::Function::CloseGuildWareHouse();
#else
        uiGuildMan* pGuildMan = (uiGuildMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
        if(pGuildMan)
        {
            if(pGuildMan->IsGuildWareHouseWindow())
            {
                pGuildMan->SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN();
            }
        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

        MSG_CG_CONNECTION_NOTIFY_LOGOUT_SYN logout_packet;
        SendPacket(CI_GAMESERVERIDX, &logout_packet, sizeof(logout_packet));

#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
        LogOutGame();
#else
        uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
        if (guide_manager != NULL)
        {
            //타이머 체크 후 종료
            guide_manager->set_waiting_repeat_save_recv(true);
            guide_manager->set_waiting_library_save_recv(true);
        }

        //로그아웃 함수는 GuideSystemMan 에서 타이머 체크 후 호출합니다. (송찬종)
        //LogOutGame();
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT

        
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::QuestAbandon(bool bYes)
{
	if ( bYes ) 
	{
		uiQuestMan * pQuestMan = (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
		if ( pQuestMan )
		{
			pQuestMan->NET_SEND_CG_QUEST_ABANDON_SYN( pQuestMan->GetPendingAbandonQuestCode() );
		}
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::RepairConfirm(bool bYes)
{
	if ( bYes )
	{
        if (g_pHero && g_pHero->IsTransForm() &&
            MouseHandler::Instance()->GetPendingRepairSlotIdx() == SI_EQUIPMENT)
        {
            // 5139	변신 상태에서는 이용이 가능하지 않습니다.
            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(5139, message, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
            return;
        }

        ItemManager::Instance()->SendItemRepairMsg(
            MouseHandler::Instance()->GetPendingRepairSlotIdx(),
            MouseHandler::Instance()->GetPendingRepairPos(), 
            MouseHandler::Instance()->GetPendingRepairAll() != FALSE );	
    }
}


void GlobalFunc::DeleteItemConfirm(bool bYes)
{
	if ( bYes ) 
	{
		ItemManager::Instance()->SendDeleteItemMsg( 
			ItemManager::Instance()->GetPendingDeleteItemPos() );
	}
	else
	{
		MouseHandler::Instance()->RollbackItemAtHand();
		MouseHandler::Instance()->UnLockWasteDeleteItem();
	}
}



void GlobalFunc::SaveCurrentPosAtItem(bool bYes)
{
	if ( bYes ) 
	{
		ItemManager::Instance()->SendItemUseMsg( 
			SI_INVENTORY, 
			MouseHandler::Instance()->GetPendingSavePositionPos() );
	}

	MouseHandler::Instance()->SetSavingPosition(FALSE);
}

void GlobalFunc::UsePoralCoordItem(bool bYes)
{
	if ( bYes ) 
	{
		
		POSTYPE OutPos = INVALID_SLOT_POS;
		BOOL bRet = ItemManager::Instance()->FindFirstItem( eITEMTYPE_PORTAL, OutPos );
		if ( !bRet )
		{
			// 포탈아이템이 없습니다.( 5931 )
			TCHAR szResultMsg[INTERFACE_STRING_LENGTH];
			g_InterfaceManager.GetInterfaceString(5931, szResultMsg, INTERFACE_STRING_LENGTH);			
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResultMsg);

			MouseHandler::Instance()->SetTryPortal(FALSE);
		}
		else
		{
			keyMsg msg; 
			ZeroMemory(&msg,sizeof(msg)); 
			msg.dwType		= eDoType_GeneralWarp; 
			msg.DoSomething	= GlobalFunc::isOutUseCoordItem; 
			msg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING; 
			msg.wParam		= eDoType_GeneralWarp; 
			g_KeyQueueManager.PushBack(msg); 

			// 딜레이 캐스트 창을 띄우고 타이머를 세팅한다.
			StartLeaveFieldCastTimer( eDoType_GeneralWarp, DEALY_SPAWN_VILLAGE );   
		}		
	}

	
}



void GlobalFunc::isOutUseCoordItem(DWORD wParam,DWORD lParam)
{
	CastLeaveFieldDialog* castleave_dialog = GET_CAST_DIALOG(CastLeaveFieldDialog, StrToWzID("0281"));    
	if (castleave_dialog == NULL)
	{
		return;
	}

	castleave_dialog->ShowWindow(FALSE);
	MouseHandler::Instance()->SetTryPortal(FALSE);
}

void GlobalFunc::isRunUsePoralCoordItem()
{
	POSTYPE OutPos = INVALID_SLOT_POS;
	BOOL bRet = ItemManager::Instance()->FindFirstItem( eITEMTYPE_PORTAL, OutPos );

	ItemManager::Instance()->SendItemPortalMsg( 
		MouseHandler::Instance()->GetPendingUseCoordPortalPos(), OutPos );

	MouseHandler::Instance()->SetTryPortal(FALSE);
}

void GlobalFunc::SavePersnalWaypoint(bool bYes)
{
	uiPersonalWayPointWindow* pDlg = 
		GET_CAST_DIALOG(uiPersonalWayPointWindow, IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG);
	if (pDlg)
	{
		if (bYes)
		{
			pDlg->SEND_CG_CHAR_WAYPOINT_INFO_SAVE_SYN();
		}
		pDlg->SetDialogBtnPending(false);
	}
}

void GlobalFunc::DeletePersnalWaypoint(bool bYes)
{
	uiPersonalWayPointWindow* pDlg = 
		GET_CAST_DIALOG(uiPersonalWayPointWindow, IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG);
	if (pDlg)
	{
		if ( bYes )
		{
			pDlg->SEND_CG_CHAR_WAYPOINT_INFO_DEL_SYN();
		}
		pDlg->SetDialogBtnPending(false);
	}
}

void GlobalFunc::ExecutePersnalWaypoint(bool bYes)
{
	uiPersonalWayPointWindow* pDlg = 
		GET_CAST_DIALOG(uiPersonalWayPointWindow, IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG);
	if (pDlg)
	{
		if ( bYes )
		{
			pDlg->SEND_CG_CHAR_WAYPOINT_INFO_EXE_CMD();
			pDlg->ShowWindow(false);
		}
		pDlg->SetDialogBtnPending(false);
	}
}

int GlobalFunc::GetUsedWayPointSlotCount()
{
	uiPersonalWayPointWindow* pDlg = 
		GET_CAST_DIALOG(uiPersonalWayPointWindow, IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG);
	if (pDlg)
	{
		return pDlg->GetUsedWayPointSlotCount();
	}
	return 0;
}

bool GlobalFunc::IsWayPointItemPending()
{
	uiPersonalWayPointWindow* pDlg = 
		GET_CAST_DIALOG(uiPersonalWayPointWindow, IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG);
	if (pDlg)
	{
		return pDlg->IsWayPointItemPending();
	}
	return true;
}
void GlobalFunc::SetWayPointItemPending(bool bFlag)
{
	uiPersonalWayPointWindow* pDlg = 
		GET_CAST_DIALOG(uiPersonalWayPointWindow, IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG);
	if (pDlg)
	{
		return pDlg->SetWayPointItemPending(bFlag);
	}
}

bool GlobalFunc::IsBattleZone()
{
	if (GameFramework::GetCurrentScene() && 
		(SCENE_TYPE_VILLAGE != GameFramework::GetCurrentScene()->GetType() &&
		SCENE_TYPE_FIELD != GameFramework::GetCurrentScene()->GetType() ) )
		return true;
	else
		return false;
}



void GlobalFunc::_fn_UseTopOfTrialItem(bool bYes)
{
	if ( bYes ) 
	{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        GuildSystem::Function::CloseGuildWareHouse();
#else
        uiGuildMan* pGuildMan = (uiGuildMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
        if(pGuildMan)
        {
            if(pGuildMan->IsGuildWareHouseWindow())
            {
                pGuildMan->SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN();
            }
        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

		ItemManager::Instance()->SendTopOfTrialItemMsg( MouseHandler::Instance()->GetPendingUseChargeItemPos() );
	}
	MouseHandler::Instance()->SetTryUseChageItem(FALSE);
}

void GlobalFunc::_fn_UseSSQItem(bool b)
{
	if (b)
	{
        LEVELTYPE	level = g_pHero->GetLevel();
        MONEY		money = g_pHero->GetMoney();

        const SSQInfo * pInfo = SSQInfoParser::Instance()->FindSSQInfoEnterableLevel( level );

        //해당레벨에 대한 정보가 없다는것은 정의가 되지않았다고 판단
        if( pInfo == NULL )
        {
            //6430:입장할수없는 레벨입니다.
            GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString( 6430 ) );
        }
        else if( money < pInfo->m_Cost )
        {
            //6411: 입장료가 부족합니다.
            GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString( 6411 ) );
        }
        else
        {
            MSG_CG_ZONE_EX_SSQ_JOIN_INFO_SYN	SendPacketAck;
            SendPacketAck.m_ItemPos	= MouseHandler::Instance()->GetPendingUseChargeItemPos();
            GlobalFunc::SendPacket( CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
        }
    }

	MouseHandler::Instance()->SetTryUseChageItem(FALSE);
}

void GlobalFunc::_fn_UsePortalQuestItem(bool bYes)
{
	if( bYes )
	{
		MSG_CG_ZONE_EX_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN SendMsg;
		SendMsg.m_ItemPos = MouseHandler::Instance()->GetPendingUseChargeItemPos();
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof( SendMsg ));
	}
	MouseHandler::Instance()->SetTryUseChageItem(FALSE);
}

void GlobalFunc::_fn_UseCharacterNameChangeItem(uiSystemMessage::ButtonIndex button_index)
{
    if (button_index == uiSystemMessage::kConfirm_Button)
    {
        if (GameFramework::GetCurrentScene() && 
            (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_VILLAGE))
        {
            SLOTIDX slot_index = MouseHandler::Instance()->GetSendSlotIdx();
            POSTYPE	pos	= MouseHandler::Instance()->GetSendAtPos();

            ItemManager::Instance()->SendItemUseMsg(slot_index, pos);
        }
    }

}

void GlobalFunc::_fn_CharacterNameChangeAck(uiSystemMessage::ButtonIndex button_index)
{
    if (button_index == uiSystemMessage::kVerify_Button)
    {
        // 시스템종료
        GlobalFunc::LogOutGame();
    }

}

void GlobalFunc::UseChargeItem(bool bYes)
{
	if ( bYes ) 
	{
		ItemManager::Instance()->SendItemUseMsg(
			SI_INVENTORY,
			MouseHandler::Instance()->GetPendingUseChargeItemPos());
	}

	MouseHandler::Instance()->SetTryUseChageItem(FALSE);
}



void GlobalFunc::UseChargeItemStatInitAll(bool bYes)
{
	if ( bYes ) 
	{
		ItemManager::Instance()->SendStatInitItem(
			MouseHandler::Instance()->GetPendingUseChargeItemPos(), 
			1 );
	}

	MouseHandler::Instance()->SetTryUseChageItem(FALSE);
}

void GlobalFunc::SendCheckUseItemByPopup( bool b )
{
	if( b )
	{
		SLOTIDX SlotIdx = MouseHandler::Instance()->GetSendSlotIdx();
		POSTYPE	AtPos	= MouseHandler::Instance()->GetSendAtPos();	 	

		ItemManager::Instance()->SendItemUseMsg( SlotIdx, AtPos );
	}
}


void GlobalFunc::SendStateInitSyn( bool bYes )
{
	if( bYes )
	{
		// 패킷 보내자.
		MSG_CG_STATUS_FREE_INIT_STAT_UNDER_20_SYN sendPacket;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sendPacket, sizeof (sendPacket));
	}
}

BOOL GlobalFunc::IsDeadField()
{
    if(g_pHero && g_pHero->IsDead() && g_pMap && g_pMap->IsField())
    {   
        //필드사망시 시스템창띄우기막기
        return TRUE;
    }
    return FALSE;
}

BOOL GlobalFunc::IsexceptionPercentageType( int OptionKind)
{
	switch( OptionKind )
	{
	case 52: //사망시스스로부활할확률	
	case 56: //방어력무시확율	
	case 60: //스킬효과무시확율	
	case 43: //데미지 반사
	case 44: //하임 증가
	case 45: //경험치 증가
	case 46: //다중공격 확률
	case 53: //데미지 두배 확률
		return TRUE;
	}

	return FALSE;
}


#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
void GlobalFunc::AddRandomOption( bool bYes)
{
	if (bYes)		
	{
		// TODO : 특정 패킷 을 보낸다.
	}

	MouseHandler::Instance()->SetTryAddRandomOption(FALSE);
}
#endif // _ADD_CHARGE_ITEM_RANDOM_OPTION


//////////////////////////////////////////////////////////////////////////
// use caes 
//   1. 퀘스트 창에서 미션보상 정보 보여줄때.
//	 2. 미션 랭킹에서 미션 정보 보여줄때.
// 대표 멥정보를 가져온다.
sMAPINFO * GlobalFunc::GetMainMapFromGroup(MAPCODE MapKind)
{
	MapGroup * pGroup = MapInfoParser::Instance()->GetMapGroup( MapKind );

	if (!pGroup)
	{
		return NULL;
	}

	sMAPINFO * pMapInfo = NULL; 
	int nMaxCnt = pGroup->GetMapInfoHash().size();
	
	//메인(0)부터 참조
	pMapInfo = (sMAPINFO *)(const sMAPINFO *)pGroup->GetMapInfoIndex(0);
	
	//GS_BUG_FIX:예외 메인이없고 서브가 있을때
	if(!pMapInfo && nMaxCnt>0)
	{
		for ( int i = 0; i < nMaxCnt; ++i )
		{
			pMapInfo = (sMAPINFO *)(const sMAPINFO *)pGroup->GetMapInfoIndex(i+1);

			if (pMapInfo)
			{
				break;
			}			
		}
	}
	return pMapInfo;
}

//////////////////////////////////////////////////////////////////////////
// use caes 
//   1. 퀘스트 창에서 미션보상 정보 보여줄때.
//	 2. 미션 랭킹에서 미션 정보 보여줄때.
// 대표 멥정보를 가져온다.
sMAPINFO * GlobalFunc::GetMainMapFromGroup(MapGroup * pGroup)
{
	if (!pGroup)
	{
		return NULL;
	}

	sMAPINFO * pMapInfo = NULL;	
	
	int nMaxCnt = pGroup->GetMapInfoHash().size();

	pMapInfo = (sMAPINFO *)(const sMAPINFO *)pGroup->GetMapInfoIndex(0);
	
	//GS_BUG_FIX:예외 메인이없고 서브가 있을때
	if(!pMapInfo && nMaxCnt>0)
	{
		for ( int i = 0; i < nMaxCnt; ++i )
		{
			pMapInfo = (sMAPINFO *)(const sMAPINFO *)pGroup->GetMapInfoIndex(i+1);
			if (pMapInfo)
			{
				break;
			}			
		}	
	}
	return pMapInfo;
}
void GlobalFunc::LogOutGame(bool is_send_notify/* = true*/)
{
    if (is_send_notify == true)
    {
        MSG_CG_CONNECTION_NOTIFY_LOGOUT_SYN SendLogoutPacket;
        SendPacket(CI_GAMESERVERIDX, &SendLogoutPacket, sizeof(SendLogoutPacket));
    }    

	if( g_hWndMain )
	{		
		SendMessage(g_hWndMain, WM_CLOSE, 0, 0);
	}
	else
	{
		g_bExitFlag = true;
	}
}



void GlobalFunc::DoSystemDialog(DWORD wParam,DWORD lParam)
{
#ifdef _GS_GSP_BLOCK_SYSTEMDLG_THEN_DEAD
    if(IsDeadField())
    {   
        //필드사망시 시스템창띄우기막기
        return;
    }
#endif//_GS_GSP_BLOCK_SYSTEMDLG_THEN_DEAD

    SolarDialog* trade_dialog = g_InterfaceManager.GetDialog(IM_TRADE_MANAGER::TRADE);
    if ((trade_dialog != NULL) && (trade_dialog->IsVisible() == TRUE))
    {
        TCHAR message[INTERFACE_STRING_LENGTH] = {NULL, };

        //! 5392	거래중에는 사용할 수 없는 기능입니다.
        g_InterfaceManager.GetInterfaceString(5392, message, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
        return;
    }

	SolarDialog* pSystemDlg=g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SYSTEM);

	if(pSystemDlg)
	{
		BOOL val = pSystemDlg->IsVisibleWindow();
		pSystemDlg->ShowDialog(val ^= TRUE );
	}	
}

void GlobalFunc::DoAutoAttack(DWORD wParam,DWORD lParam)
{
	g_HeroInput.SetAutoAttack(FALSE,FALSE);
}

void GlobalFunc::DoAutoRun(DWORD wParam,DWORD lParam)
{
	g_HeroInput.SetAutoRun(FALSE,FALSE);
}

void GlobalFunc::DoAreaSkill(DWORD wParam,DWORD lParam)
{
	g_HeroInput.SetAreaSkill(FALSE);
    g_SkillQueue.Pop();
}

void GlobalFunc::DoTarget(DWORD wParam,DWORD lParam)
{
	g_HeroInput.SetCurrentTarget(0);	
}

void GlobalFunc::DoCastCancel(DWORD wParam,DWORD lParam)
{
	//캐스팅 취소 패킷 날리자.
	if(g_pHero&&g_pHero->IsCasting())
	{
		if( GENERALPARAM->IsNet()==FALSE)// 싱글 모드 일경우 
		{
			g_pHero->SetNextState(STATE::IDLE,g_CurTime);
			g_pHero->CommitChangeState(g_CurTime);
		}
		else
		{
			//if(g_pHero->GetCurrentAction()->ActionID==ACTION_SKILL&&BIT_CHECK( g_pHero->m_dwPacketStatus, PACKET_STATUS_CAST_CANCEL )==FALSE)
			if(ACTION_SKILL == g_pHero->GetCurrentAction()->ActionID)
			{
				//g_InterfaceManager.ShowMessageBox("보냈다",1000);
				//BIT_ADD( g_pHero->m_dwPacketStatus, PACKET_STATUS_CAST_CANCEL );
				MSG_CG_SKILL_CASTING_CANCEL_SYN Packet;
				Packet.m_byCategory = CG_SKILL;
				Packet.m_byProtocol = CG_SKILL_CASTING_CANCEL_SYN;
				Packet.m_SkillCode = (SLOTCODE)g_pHero->GetCurrentAction()->SKILL.dwSkillID;
				g_pHero->SendPacket(&Packet,sizeof(Packet));
			}
            else if(g_pHero->GetCurrentAction()->ActionID==ACTION_SUMMON_RIDER)
            {
                //소환캐스팅취소패킷
                MSG_CG_ITEM_RIDER_CONTROL_SYN Packet;
                Packet.header.request = Packet.header.eReq_SummonCancel;
                g_pHero->SendPacket(&Packet,sizeof(Packet));
            }    


            else if (g_pHero->GetCurrentAction()->ActionID == ACTION_ETHERIA_EXTRACT)
            {
                //에테리아 캐스팅 취소패킷
                MSG_CG_ITEM_ETHERIA_CONTROL_SYN packet;
                packet.header.request = packet.header.eReq_CastingCancel;
                g_pHero->SendPacket(&packet, sizeof(packet));
            }  

		}
	}
}

void GlobalFunc::DoLotteryOpenCancel(DWORD wParam,DWORD lParam)
{
	// 판도라상자 오픈취소
	g_InterfaceManager.ShowWindow((InterfaceManager::eDIALOG_ID)lParam, FALSE);
	g_KeyQueueManager.DeleteMsg(eDoType_Lottery_OpenDlg);
	g_InterfaceManager.UnLockDlgs();
}

void GlobalFunc::DoLeaveFieldCancel(DWORD wParam,DWORD lParam)
{
	// 필드를 떠나는 행위를 취소한다.
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	if ( eDoType_ServerSelectStandingBy == wParam )
	{
		GlobalFunc::SendServerSelectRequestPacket(MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_SYN::eQuery_StopTransaction);
        return;
	}
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__	
    
    CastLeaveFieldDialog* castleave_dialog = GET_CAST_DIALOG(CastLeaveFieldDialog, StrToWzID("0281"));    
    if (castleave_dialog == NULL)
    {
        return;
    }

    castleave_dialog->ShowWindow(FALSE);


    if (g_pHero && g_pHero->IsDead())
    {
        //Scene에 맞춰 부활 캐스팅 시전
        GlobalFunc::ReviveCastFromSceneType();
    }
}

void GlobalFunc::DoLogin(DWORD wParam,DWORD lParam)
{
}

//WzId로 다이얼로그를 닫는다
void GlobalFunc::DoShowWindowEx(DWORD wParam,DWORD lParam)
{
	SolarDialog* pDlg=g_InterfaceManager.GetDialog((WzID)wParam);

	if(pDlg)
	{
		pDlg->ShowDialog(FALSE);
	}

}

void GlobalFunc::DoShowWindow(DWORD wParam,DWORD lParam)
{
	SolarDialog* pDlg=g_InterfaceManager.GetDialog((InterfaceManager::eDIALOG_ID)wParam);

	if(pDlg)
	{
        ShopDialog* shop_dialog = static_cast<ShopDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SHOP));
        if ((shop_dialog != NULL) && 
            (shop_dialog == pDlg) && 
            (shop_dialog->IsVisible() == TRUE))
        {
            if ((MouseHandler::Instance()->GetMouseState() == MouseHandler::BUY) ||
                (MouseHandler::Instance()->GetMouseState() == MouseHandler::SELL) || 
                (MouseHandler::Instance()->GetMouseState() == MouseHandler::REPAIR))
            {
                shop_dialog->SetCheckTrade(ShopDialog::SHOP_METHOD_NONE);

                MouseHandler::Instance()->RemoveMouseState(MouseHandler::BUY);
                MouseHandler::Instance()->RemoveMouseState(MouseHandler::SELL);
                MouseHandler::Instance()->RemoveMouseState(MouseHandler::REPAIR);

                return;
            }
        }

		pDlg->ShowDialog(FALSE);
	}
}



void GlobalFunc::DoDeleteWindow(DWORD wParam,DWORD lParam)
{
	SolarDialog* pDlg=g_InterfaceManager.GetDialog((WzID) wParam);
	if(pDlg)
	{
		pDlg->ShowDialog(FALSE);
		pDlg->SetDestroyWindow(TRUE);
	}		
}


void GlobalFunc::DoShowQuestWindow(DWORD wParam,DWORD lParam)
{
	uiQuestMan * pQuestMan = (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
	if ( !pQuestMan )
	{
		return;
	}

	switch((eQUEST_WINDOW_TYPE)wParam)
	{
	case QUEST_WINDOW_PLAYER:
		{
			if ( pQuestMan->IsOpenQuestList() )
			{
				pQuestMan->CloseQuestList();
			}
			else
			{
				;
			}

			g_KeyQueueManager.DeleteMsg( eDoType_dialog_Quest_Player );
		}
		break;

	case QUEST_WINDOW_NPC:
		{
			if ( pQuestMan->IsOpenQuestNPC() )
			{
				pQuestMan->CloseQuestNPC();
			}
			else
			{
				;
			}

			g_KeyQueueManager.DeleteMsg( eDoType_dialog_Quest_NPC );
		}
		break;
	}
}

void GlobalFunc::DoRadarFullDialog(DWORD wParam, DWORD lParam)
{
	RadarFullDialog*  pfullRadar = GET_CAST_DIALOG(RadarFullDialog, IM_RADAR_MANAGER::RADAR_DIALOG_FULL);

    if (pfullRadar)
    {
		pfullRadar->ResetRadarState();
        pfullRadar->ShowWindow(FALSE);
    }
}

void GlobalFunc::DoContinentMapDlg(DWORD wParam, DWORD lParam)
{
	uiBase* pCMDlg = GET_DIALOG(IM_RADAR_MANAGER::CONTINENT_MAP_DLG);
	if(pCMDlg)
        pCMDlg->ShowWindow(FALSE);
    pCMDlg = GET_DIALOG(IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG);
    if (pCMDlg)
    {
        pCMDlg->ShowWindow(FALSE);
    }
}
void GlobalFunc::DoQuestListDlg(DWORD wParam, DWORD lParam)
{
	uiBase* pQLDlg = GET_DIALOG(IM_QUEST_MANAGER::QUEST_LIST_DLG);
	if(pQLDlg)
		pQLDlg->ShowWindow(FALSE);
}

void GlobalFunc::DoQuestInfoDlg(DWORD wParam, DWORD lParam)
{
	uiBase* pQLDlg = GET_DIALOG(IM_QUEST_MANAGER::QUEST_LIST_MISSION);
	if(pQLDlg)
		pQLDlg->ShowWindow(FALSE);
}

void GlobalFunc::DoBattleCreate(DWORD wParam,DWORD lParam)
{
	uiBattleZoneCreate* pDlg=(uiBattleZoneCreate*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLEZONE_CREATE);

	if(pDlg)
	{
        pDlg->ExitZoneCreate();
	}
}


void  GlobalFunc::DoZoneList(DWORD wParam,DWORD lParam)
{
	uiBattleZoneList* pDlg=(uiBattleZoneList*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLEZONE_LIST);

	if(pDlg)
	{
		pDlg->ExitZoneList();
	}

}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#else
void GlobalFunc::DoGuildMenu(DWORD wParam,DWORD lParam)
{
    uiGuildMenu* pDlg =
        (uiGuildMenu *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_GUILD_MENU);

    if (pDlg)
    {
        pDlg->ShowInterface(FALSE);
    }
}

void GlobalFunc::DoGuildMain(DWORD wParam,DWORD lParam)
{
    uiGuildMain* pDlg =
        (uiGuildMain *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_GUILD_MAIN);

    if (pDlg)
    {
        pDlg->ShowInterface(FALSE);
    }
}

void GlobalFunc::DoGuildCreate(DWORD wParam,DWORD lParam)
{
    uiGuildCreate* pDlg =
        (uiGuildCreate *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_GUILD_CREATE);

    if (pDlg)
    {
        pDlg->ShowInterface(FALSE);
    }
}

void GlobalFunc::DoGuildWindow(DWORD wParam,DWORD lParam)
{
    uiGuildWindow* pDlg =
        (uiGuildWindow *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_GUILD_WINDOW);

    if (pDlg)
    {
        pDlg->ShowInterface(FALSE);
    }
}
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

void GlobalFunc::DoAreaConquest(DWORD wParam,DWORD lParam)
{
    uiBase* pDlg = GET_DIALOG(IM_AREACONQUEST_MANAGER::AREACONQUEST_DIALOG);
	if (pDlg)
	{
		pDlg->ShowDialog(FALSE);
	}
}

void GlobalFunc::DoTotalComposite(DWORD wParam,DWORD lParam)
{	
	uiItemTotalcomposite * pDlg = (uiItemTotalcomposite *)g_InterfaceManager.GetDialog
								(InterfaceManager::DIALOG_ITEM_TOTAL_COMPOSITE);
	if ( pDlg )
	{
		pDlg->ShowDialog(FALSE);
	}
}

void GlobalFunc::DoDeleteItemCancel(DWORD wParam,DWORD lParam)
{
	MouseHandler::Instance()->RemoveMouseState( MouseHandler::THROW_WASTE_BASKET );
	g_KeyQueueManager.DeleteMsg( eDoType_dialog_delete_cancel );
}


void GlobalFunc::DoRankDownCancle(DWORD wParam,DWORD lParam)
{
	MouseHandler::Instance()->RemoveMouseState( MouseHandler::RANK_DOWN);
	g_KeyQueueManager.DeleteMsg(eDoType_Rank_Down_Cancle);
}

void GlobalFunc::DoRepairLimitedCancle(DWORD wParam,DWORD lParam)
{
	MouseHandler::Instance()->RemoveMouseState( MouseHandler::REPAIR_LIMITED);
	g_KeyQueueManager.DeleteMsg(eDoType_Repair_Limited_Cancle);
}

void GlobalFunc::DoStatChangeItemCancel(DWORD wParam,DWORD lParam)
{
	MouseHandler::Instance()->RemoveMouseState( MouseHandler::ITEM_STAT_CHANGE );
	g_KeyQueueManager.DeleteMsg( eDoType_Stat_Change_Item );
}

#ifdef __NA_001358_ENCHANT_UNIFICATION
void GlobalFunc::DoInstantEnchantCancle(DWORD wParam,DWORD lParam)
{
	MouseHandler::Instance()->RemoveMouseState( MouseHandler::ENCHANT );
	g_KeyQueueManager.DeleteMsg( eDoType_dialog_InstantEnchant_Cancle );
}
#endif // __NA_001358_ENCHANT_UNIFICATION

void GlobalFunc::DoWaxGlossItemCancle(DWORD wParam,DWORD lParam)
{
    MouseHandler::Instance()->RemoveMouseState( MouseHandler::RIDER_WAX );
    g_KeyQueueManager.DeleteMsg( eDoType_Cursor_Rider_Wax );
}

void GlobalFunc::ReviveCastFromSceneType()
{
    // 2011.08.09 / 송찬종 / 리스폰 캐스팅 바와 마을로 가기 캐스팅 바가 곂치면서 다이얼로그 락이 안풀리는 현상 수정
    // 마을로 가기 캐스팅 바가 끝날 때 캐릭터가 죽은 상태라면 리스폰 캐스팅 시작
    // 2012.10.15 / 송찬종 / 점령전에 이어 전장과 자유전투실에도 같은 룰 적용

    //_NA_000000_121015_LEAVE_CANCEL_TO_REVIVE_RE_CASTING
    //Scene에 맞춰 부활 캐스팅 시전
    if (SCENE_TYPE_DOMINATION == GameFramework::GetCurScene())
    {

#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
        DeadConfirmInDomination(0,0);
#else 
        const sDOMINATION_INFO* domination_info =
            DominationInfoParser::Instance()->GetDominationInfo(g_pMap->GetCurrentFieldID());
        if (domination_info)
        {
            StartLeaveFieldCastTimer( eDoType_Delay_Spawn_DominationWar, domination_info->m_RespawnDelay);

            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR szResultInfo[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(3326, szMessage, INTERFACE_STRING_LENGTH);
            Sprintf( szResultInfo, szMessage, domination_info->m_RespawnDelay/1000);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResultInfo);
        }
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
    }
#ifdef _NA_000000_20120527_CHAOS_ZONE
 #ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    else if (SCENE_TYPE_BATTLEGROUND == GameFramework::GetCurScene()
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        || SCENE_TYPE_GOLDRUSH == GameFramework::GetCurScene()
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        )
    {
        const BattleGroundInfo* battle_ground_info =
            BattleGroundInfoParser::Instance()->FindData(g_pMap->GetCurrentFieldID());
        if (battle_ground_info)
        {
            StartLeaveFieldCastTimer(eDoType_Delay_Spawn_BattleGround, battle_ground_info->dead_time*1000);
        }
    }
 #endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
 #ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    else if (SCENE_TYPE_FREEPVP == GameFramework::GetCurScene())
    {
        StartLeaveFieldCastTimer(eDoType_Delay_Spawn_FreePVP, DEALY_SPAWN_VILLAGE);
    }
 #endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#endif //_NA_000000_20120527_CHAOS_ZONE
#ifdef _NA_008334_20150608_SONNENSCHEIN
    else if (SCENE_TYPE_SONNENSCHEIN == GameFramework::GetCurScene()
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        || SCENE_TYPE_GUILDMISSION == GameFramework::GetCurScene()
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        )
    {
        ShowSonnenScheinResurrectionPopup(0,0);
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
    else
    {
        //no op
    }
}

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
void GlobalFunc::DoExtendItemDate(DWORD l_param, DWORD r_param)
{
    MouseHandler::Instance()->RemoveMouseState(MouseHandler::EXTEND_ITEMDATE);
    g_KeyQueueManager.DeleteMsg(eDoType_Extend_ItemDate);
}
#endif//_NA_0_20111012_ITEM_DATE_EXTEND

#ifdef _NA_003740_20111122_RANDOM_ITEM
void GlobalFunc::DoRandomizeItem(DWORD l_param, DWORD r_param)
{
    MouseHandler::Instance()->RemoveMouseState(MouseHandler::RANDOMIZE_ITEM);
    g_KeyQueueManager.DeleteMsg(eDoType_Randomize_Item);
}
#endif//_NA_003740_20111122_RANDOM_ITEM

void GlobalFunc::DoEtheriaExtract(DWORD l_prarm, DWORD r_prarm)
{
    MouseHandler::Instance()->RemoveMouseState(MouseHandler::ETHERIA_EXTRACT);
    g_KeyQueueManager.DeleteMsg(eDoType_Etheria_Extract);
}
void GlobalFunc::DoEtheriaCombination(DWORD l_prarm, DWORD r_prarm)
{
    MouseHandler::Instance()->RemoveMouseState(MouseHandler::ETHERIA_COMBINATION);
    g_KeyQueueManager.DeleteMsg(eDoType_Etheria_Combine);
}

void GlobalFunc::DoSocketIdentify(DWORD l_prarm, DWORD r_prarm)
{
    MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_IDENTIFY);
    g_KeyQueueManager.DeleteMsg(eDoType_SocketIdentify);
}
void GlobalFunc::DoSocketRecreate(DWORD l_prarm, DWORD r_prarm)
{
    MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_RECREATE);
    g_KeyQueueManager.DeleteMsg(eDoType_SocketRecreate);
}

void GlobalFunc::DoCrystalization( DWORD l_param, DWORD r_param )
{
    MouseHandler::Instance()->RemoveMouseState(MouseHandler::CRYSTALIZATION);
    g_KeyQueueManager.DeleteMsg(eDoType_Crystalization);
    InventoryDialog* inventory_dialog_ptr = static_cast<InventoryDialog*>(GET_DIALOG(IM_INVENTORY_MANAGER::INVEN_DLG));
    if (inventory_dialog_ptr)
    {
        inventory_dialog_ptr->RevertCrystalizationItemIconDeActiveRender();
        // 경고음 출력
        g_SoundEffect.PlayUI(UI_SOUND_ITEM_PUT_CRYSTALIZATION);

        // 5849 : 결정화가 모두 취소되었습니다.
        TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
        g_InterfaceManager.GetInterfaceString(5849, format_string);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, format_string);
    }
}

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
void GlobalFunc::DoChaoszoneDialog(DWORD l_prarm, DWORD r_prarm)
{
    uiChaosSystemMan* chaos_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
    if (chaos_manager)
    {
        chaos_manager->ToggleChaoszoneDialog(true); //창 닫기
    }
    g_KeyQueueManager.DeleteMsg(eDoType_ChaoszoneDialog);
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
void GlobalFunc::DoSmartNpcMenuDialog(DWORD wParam,DWORD lParam)
{
    g_InterfaceManager.DeleteInterface(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU);
}
void GlobalFunc::DoSmartNpcReplyDialog(DWORD wParam,DWORD lParam)
{
    g_InterfaceManager.DeleteInterface(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY);
}
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007086_20140318_MONSTERBOOK
void GlobalFunc::DoMonsterBookDialog(DWORD l_prarm, DWORD r_prarm)
{
    uiMonsterBookMan* monsterbook_man = GET_DIALOG_MANAGER(uiMonsterBookMan, UIMAN_MONSTERBOOK);
    if (monsterbook_man)
    {
        monsterbook_man->toggle_monster_book(TRUE);
    }
}
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
void GlobalFunc::DoAttendanceCheckDialog(DWORD l_prarm, DWORD r_prarm)
{
    uiAttendanceCheckMan* manager = GET_DIALOG_MANAGER(uiAttendanceCheckMan, UIMAN_ATTENDANCE_CHECK);
    if (manager)
    {
        manager->toggle_window(TRUE);
    }
}
void GlobalFunc::AttendanceCheckRewardConfirm(bool confirm_value)
{
    if (confirm_value)
    {
        MSG_CG_CHARINFO_GET_ATTENDANCE_REWARD_PACKET packet;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof(MSG_CG_CHARINFO_GET_ATTENDANCE_REWARD_PACKET));
    }
    return;
}
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
void GlobalFunc::DoSUNRankingDialog(DWORD l_param, DWORD r_param)
{
	uiSUNRankingMan* sun_ranking_manager = GET_DIALOG_MANAGER(uiSUNRankingMan, UIMAN_SUNRANKING);
	if ( sun_ranking_manager )
	{
		sun_ranking_manager->ToggleSUNRankingDialog();
	}
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
void GlobalFunc::DoBingoGameDialog( DWORD l_param, DWORD r_param )
{
	uiMiniGameMan* mini_game_manager = GET_DIALOG_MANAGER(uiMiniGameMan, UIMAN_MINIGAME);
	if ( mini_game_manager )
	{
		mini_game_manager->ToggleBingoGameDialog();
	}
}
#endif // _NA_008405_20150716_MINIGAME_BINGO

#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
//----------------------------------------------------------------------------
/** 렌덤옵션 추가.
*/
void GlobalFunc::DoAddRandomOption(DWORD wParam, DWORD lParam)
{
	MouseHandler::Instance()->RemoveMouseState( MouseHandler::ADD_RANDOM_OPTION);	
	g_KeyQueueManager.DeleteMsg( eDoType_dialog_Random_Option_cancel );
}

#endif

void GlobalFunc::DoBank(DWORD wParam,DWORD lParam)
{
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    uiBankDialog *pBankDlg = GET_CAST_DIALOG(uiBankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#else
    BankDialog *pBankDlg = GET_CAST_DIALOG(BankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM

	if(pBankDlg)
	{
		pBankDlg->SendWareHouseEnd();
	}
}

void GlobalFunc::DoGuildWareHouse(DWORD wParam, DWORD lParam)
{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    DWORD dialog_id = uiGuildWareHouse::kDialog_ID;
#else
    DWORD dialog_id = IM_GUILD_MANAGER::GUILD_WAREHOUSE;
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    uiGuildWareHouse* guildwareouse_dlg_ptr = GET_CAST_DIALOG(uiGuildWareHouse, dialog_id);    
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    if (guildwareouse_dlg_ptr != NULL)
    {
        guildwareouse_dlg_ptr->SendWareHouseEnd();
    }
#else
	uiGuildMan* pGuildMan = (uiGuildMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
	if(pGuildMan)
	{
        if ((MouseHandler::Instance()->GetMouseState() == MouseHandler::GUILD_WITHDRAW) ||
            (MouseHandler::Instance()->GetMouseState() == MouseHandler::GUILD_DEPOSIT))
        {
            if (guildwareouse_dlg_ptr != NULL)
            {
                guildwareouse_dlg_ptr->InitButton();
            }
            return;
        }

		pGuildMan->SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN();
	}
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM    
}


void GlobalFunc::DoVentorBuy(DWORD wParam,DWORD lParam)
{
	uiVendorBuy* pDlg=(uiVendorBuy*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_VENDOR_BUY);

	if(pDlg)
	{
        pDlg->Exit();
	}

}

void GlobalFunc::DoVentorSell(DWORD wParam,DWORD lParam)
{
	uiVendorSell* pDlg=(uiVendorSell*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_VENDOR_SELL);

	if(pDlg)
	{
		pDlg->Exit();
	}

}

void GlobalFunc::DoTrade(DWORD wParam,DWORD lParam)
{
    uiTrade* pDlg = (uiTrade *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_TRADE);

    if (pDlg)
    {
        uiTradeMan* tradeMan =
            static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));
        assert (tradeMan);
        tradeMan->SendTradeCancel();

        pDlg->ShowInterface(FALSE);
    }
}

//------------------------------------------------------------------------------ 
void GlobalFunc::DoSystemConfirm(DWORD wParam, DWORD lParam)
{
    uiSystemConfirm* system_confirm = GET_CAST_DIALOG(uiSystemConfirm, lParam);
    if (system_confirm != NULL)
    {
        system_confirm->OnClose();
    }
}

//------------------------------------------------------------------------------
/**
*/
void GlobalFunc::DoSystemVerify(DWORD wParam,DWORD lParam)
{
    if( uiSystemVerify* pDlg = (uiSystemVerify *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SYSTEM_VERIFY) )
    {
        pDlg->Exit();
    }
}

void GlobalFunc::DoInputNumBank(DWORD wParam,DWORD lParam)
{
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    // no operation
#else
	if( InputNumBankDialog* pDlg = (InputNumBankDialog*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_INPUT_NUM) )
	{
		pDlg->Exit();
	}
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
}

void GlobalFunc::DoInputNumTradeBank(DWORD wParam,DWORD lParam)
{
	if( InputNumTradeDialog* pDlg = (InputNumTradeDialog*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_INPUT_NUM) )
	{
		pDlg->Exit();
	}
}


void GlobalFunc::DoVentorPopup(DWORD wParam,DWORD lParam)
{
	uiVendorPopup* pDlg=(uiVendorPopup*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_VENDOR_POPUP);

	if(pDlg)
	{
		pDlg->Exit();
	}
}

void GlobalFunc::DoVentorTitlePopup(DWORD wParam,DWORD lParam)
{
    uiVendorTitlePopup* pDlg=(uiVendorTitlePopup*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_VENDOR_TITLE_POPUP);

    if(pDlg)
    {
        pDlg->Exit();
    }
}

void GlobalFunc::DoPartyLeave(DWORD wParam,DWORD lParam)
{
	uiPartyLeavePopup * pDlg = GET_CAST_DIALOG( uiPartyLeavePopup, IM_PARTY_MANAGER::PARTY_LEAVE_POPUP );

	if(pDlg)
	{
		pDlg->Exit();
	}
}

void GlobalFunc::DoInvokeSceneEvent(DWORD wParam,DWORD lParam)
{
	if (GameFramework::GetCurrentScene())
	{
		GameFramework::GetCurrentScene()->KeyManagerProc(wParam,lParam);
	}
}

#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
void GlobalFunc::DoVendorBuyPopup(DWORD wParam, DWORD lParam)
{
	uiVendorBuyPopup * pDlg = GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_BUY_POPUP );
	if( pDlg )
	{
		pDlg->ShowWindow(FALSE);
	}
}
#endif

void GlobalFunc::DoVendorList(DWORD wParam, DWORD lParam)
{
	uiVendorList * pDlg = GET_CAST_DIALOG( uiVendorList, IM_VENDOR_MANAGER::VENDOR_LIST );
	if( pDlg )
	{
		pDlg->NET_SEND_CG_VENDOR_LIST_END_SYN();
	}
}

void GlobalFunc::ShowMessageBox_Debug( LPCTSTR szMessage,int iDelay, WzColor *pTextColor )
{
#ifdef _DEBUG
	g_InterfaceManager.ShowMessageBox( szMessage, iDelay, pTextColor, 0 );
#endif
}

//------------------------------------------------------------------------------
/**
*/

void GlobalFunc::SetCaptionByPasswordType(CCtrlStaticWZ* pCtrlTxt, INOUT LPTSTR strPassword)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	int iCount = _tcslen(strPassword);

	for(int i = 0; i < iCount; i++)
	{
		_tcscat(szMessage, _T("*"));
	}

	if( pCtrlTxt )
	{
		pCtrlTxt->SetTextWZ( szMessage );
	}
}


//------------------------------------------------------------------------------
/**
*/
void GlobalFunc::SetCaption( CCtrlStaticWZ *pControl, LPCTSTR pMsg )
{
	if( pControl )
	{
		pControl->SetTextWZ( const_cast<LPTSTR>(pMsg) );
	}
}

void GlobalFunc::SetCaption( CCtrlButtonWZ *pControl, LPCTSTR pMsg )
{
	if( pControl )
	{
		pControl->SetTextWZ( const_cast<LPTSTR>(pMsg) );
	}
}

void GlobalFunc::GetText( CCtrlDxIMEEditBox *pControl, OUT TCHAR *pMsg, int iSize )
{
	if( pControl )
	{
		pControl->GetTextWZ( pMsg, iSize );
	}
}

void GlobalFunc::SetText( CCtrlDxIMEEditBox *pControl, LPCTSTR pMsg )
{
	if( pControl )
	{
		pControl->SetTextWZ( const_cast<LPTSTR>(pMsg) );
	}
}

void GlobalFunc::SetItemText( CCtrlListWZ *pControl, int iIndex, LPCTSTR pMsg )
{
	if( pControl )
	{
		pControl->SetItemTextWZ( iIndex, const_cast<LPTSTR>(pMsg) );
	}
}

void GlobalFunc::SetItemText( CCtrlListWZ *pControl, int iIndex, int iSubIndex, LPCTSTR pMsg )
{
	if( pControl )
	{
		pControl->SetItemTextWZ( iIndex, iSubIndex, const_cast<LPTSTR>(pMsg) );
	}
}

void GlobalFunc::SetItem( CCtrlListWZ *pControl, int nIndex, TCHAR* pszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor )
{
	if( pControl )
	{
		pControl->SetItemWZ( nIndex, pszText, dwFontID, dwTextStyle, BKColor, TextColor, SelTextColor );
	}
}

void GlobalFunc::SetItem( CCtrlListWZ *pControl, int nIndex, int nSubIndex, TCHAR* pszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor )
{
	if( pControl )
	{
		pControl->SetItemWZ( nIndex, nSubIndex, pszText, dwFontID, dwTextStyle, BKColor, TextColor, SelTextColor );
	}
}

void GlobalFunc::SetReportButtonInfo( CCtrlListWZ *pControl, int nIndex,int nLength, LPCTSTR pszSZ )
{
	if( pControl )
	{
		pControl->SetReportButtonInfo( nIndex, nLength, const_cast<TCHAR*>( pszSZ ) );
		//pControl->SetReportButtonInfo( nIndex, nLength, pszSZ );
	}
}

void GlobalFunc::SetFocus( SolarDialog * pDlg, CCtrlDxEditBox * pControl)
{
	if ( pDlg && pControl)
	{
		pControl->SetFocusWZ();
	}
}


//------------------------------------------------------------------------------
/**
*/
void GlobalFunc::SetMoneyText( CCtrlStaticWZ *pControl, MONEY money)
{
	if(!pControl)
		return;

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };

	TCHAR szResult[INTERFACE_STRING_LENGTH] ={0,};
	TCHAR szMoney[INTERFACE_STRING_LENGTH] ={0,};
	// 돈을 셋팅한다.
    Sprintf(szMoney, _T("%I64u"), money);

	::GetNumberFormat(
		LOCALE_SYSTEM_DEFAULT, 
		NULL, 
		szMoney, 
		&nFmt, 
		szResult, 
		sizeof(szResult));

	SetCaption( pControl, szResult );
}

void GlobalFunc::SetPointText(TCHAR *result_string, INT64 number)
{
    if(result_string == NULL)
    {
        return;
    }
    NUMBERFMT format = { 0, 0, 3, _T("."), _T(","), 1 };// 3자리마다 ',' 찍기
    TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};

    // 숫자를 셋팅한다.
	Sprintf(buffer_string, _T("%I64u"), number);
	::GetNumberFormat(LOCALE_SYSTEM_DEFAULT, NULL, buffer_string, &format, 
        result_string, sizeof(buffer_string));
}
//------------------------------------------------------------------------------
/**
*/
void GlobalFunc::SetMoneyText(CCtrlStaticWZ *control, MONEY money, BYTE money_type, bool is_minus)
{
	if (!control)
    {
		return;
    }

	NUMBERFMT format = { 0, 0, 3, _T("."), _T(","), 1 };
	TCHAR result_string[INTERFACE_STRING_LENGTH] ={0,};
	TCHAR money_string[INTERFACE_STRING_LENGTH] ={0,};
	TCHAR message_string[INTERFACE_STRING_LENGTH] ={0,};
	TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    int string_code = 0;

	// 돈을 셋팅한다.
	Sprintf(money_string, _T("%I64u"), money);
	::GetNumberFormat(LOCALE_SYSTEM_DEFAULT, NULL, money_string, &format, result_string, sizeof(result_string));

	switch (money_type)
	{
	case CASH_ITEM_SHOP_DEFAULT:
		{
            if (is_minus)
            {
                string_code = 155361;   // 155361	-%s 캐시
            }
            else
            {
                string_code = 155362;   // 155362	%s 캐시
            }
		}
		break;

	case CASH_ITEM_SHOP_HAIM:
		{
            if (is_minus)
            {
                string_code = 155363;   // 155363	-%s 하임
            }
            else
            {
                string_code = 155364;   // 155364	%s 하임
            }
		}
		break;
	case CASH_ITEM_SHOP_MILEAGE:
		{
            if (is_minus)
            {
                string_code = 155388;   // 155388	%s 마일리지
            }
            else
            {
                string_code = 155389;   // 155389	-%s 마일리지
            }
		}
		break;
    case CASH_ITEM_SHOP_CCASH:
        {
            if (is_minus)
            {
                string_code = 155446;   // 155388	-%s cash(c)
            }
            else
            {
                string_code = 155445;   // 155389	%s cash(c)
            }
        }
        break;
    case CASH_ITEM_SHOP_PCASH:
        {
            if (is_minus)
            {
                string_code = 155448;   // 155388	-%s cash(p)
            }
            else
            {
                string_code = 155447;   // 155389	%s cash(p)
            }
        }
        break;
	default:
        {
            _asm nop;
        }
		break;
	}	

    if (string_code)
    {
        g_InterfaceManager.GetInterfaceString(string_code, temp_string ,INTERFACE_STRING_LENGTH);
	    Snprintf(message_string ,INTERFACE_STRING_LENGTH , temp_string , result_string);
	    SetCaption(control, message_string);
    }
}

void GlobalFunc::CalcTime(DWORD64 dwTime IN, TCHAR *ptszOutString OUT)
{
	SYSTEMTIME sysTime;
	YYYYMMDDHHMMSSToSYSTEMTIME(dwTime, sysTime);

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	// 155365	%d년 %d월 %d일
	g_InterfaceManager.GetInterfaceString( 155365 , szMessage ,INTERFACE_STRING_LENGTH);
	Snprintf(ptszOutString, INTERFACE_STRING_LENGTH-1, szMessage, sysTime.wYear, sysTime.wMonth, sysTime.wDay);
}


//----------------------------------------------------------------------
void GlobalFunc::CalcTime(DWORD dwSec IN, int& min OUT, int& sec OUT, TCHAR *ptszOutString OUT)
{
	min = 0;
	sec = 0;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	if(dwSec)
	{
		min = dwSec / 60;
		sec = dwSec % 60;

		if(min && sec)
		{
			// 155366	%d분 %d초
			g_InterfaceManager.GetInterfaceString(155366 , szMessage ,INTERFACE_STRING_LENGTH);
			Snprintf(ptszOutString,INTERFACE_STRING_LENGTH-1, szMessage , min, sec );
		}
		else if(min)
		{
			// 155367	%d분
			g_InterfaceManager.GetInterfaceString( 155367 , szMessage ,INTERFACE_STRING_LENGTH);
			Snprintf(ptszOutString,INTERFACE_STRING_LENGTH-1, szMessage , min);
		}
		else if(sec)
		{
			// 155368	%d초
			g_InterfaceManager.GetInterfaceString( 155368 , szMessage ,INTERFACE_STRING_LENGTH);
			Snprintf(ptszOutString,INTERFACE_STRING_LENGTH-1, szMessage, sec );
		}
	}
}

//----------------------------------------------------------------------
void GlobalFunc::CalcTime(DWORD dwMin IN, int& day OUT, int& hour OUT, int& min OUT, TCHAR *ptszOutString OUT)
{
	day = 0;
	hour = 0;
	min = 0;
	
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

	if(dwMin)	
	{
		hour = dwMin / 60;
		min = dwMin %  60;
		
		if(hour)
		{
			day = hour / 24;

			if(day)
			{
				//155369	%d일
				g_InterfaceManager.GetInterfaceString(155369 , szMessage ,INTERFACE_STRING_LENGTH);
				Snprintf(ptszOutString, INTERFACE_STRING_LENGTH-1, szMessage , day);
			}
			else if(hour && min)
			{
				//155370	%d시간 %d분
				g_InterfaceManager.GetInterfaceString(155370 , szMessage ,INTERFACE_STRING_LENGTH);
				Snprintf(ptszOutString, INTERFACE_STRING_LENGTH-1, szMessage , hour, min);
			}
			else if(hour)
			{
				// 155371	%d시간
				g_InterfaceManager.GetInterfaceString(155371 , szMessage ,INTERFACE_STRING_LENGTH);
				Snprintf(ptszOutString, INTERFACE_STRING_LENGTH-1, szMessage , hour);
			}
			else if(min)
			{
				// 155367	%d분
				g_InterfaceManager.GetInterfaceString(155367 , szMessage ,INTERFACE_STRING_LENGTH);
				Snprintf(ptszOutString, INTERFACE_STRING_LENGTH-1, szMessage , min);
			}
		}
		else
		{
			if(min)
			{
				//  155367	%d분
				g_InterfaceManager.GetInterfaceString(155367 , szMessage ,INTERFACE_STRING_LENGTH);
				Snprintf(ptszOutString, INTERFACE_STRING_LENGTH-1, szMessage, min);
			}
		}
	}
	else
	{
		Snprintf(ptszOutString, INTERFACE_STRING_LENGTH-1, "0" );
	}
}

//----------------------------------------------------------------------
bool GlobalFunc::CompareLimitText(CCtrlDxIMEEditBox * m_pEditCtrl, TCHAR* szOutMessage, int iSize, int iErrorNum)
{
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	GetText(m_pEditCtrl, szMessage, INTERFACE_STRING_LENGTH);

	if(_tcslen(szMessage) > (size_t)iSize)
	{
		g_InterfaceManager.GetInterfaceString(iErrorNum, szTemp, INTERFACE_STRING_LENGTH);
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_Verify(szTemp, FALSE);
		return false;
	}
	else
	{
		CopyMemory(szOutMessage, szMessage, iSize);
		return true;
	}
}

void GlobalFunc::ProcessChatMessage( TCHAR* message )
{
    if( ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG) )
    {
        pChatDlg->OnProcessChatMessage( message );
    }
}

bool GlobalFunc::BeforeLoadingInputLock()
{
#ifdef _DH_BATTLEZONE2_
    // 배틀존 입장 Lock
    uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
    if (ui_battlezone2_manager_ && ui_battlezone2_manager_->is_wait())
    {
        return true;
    }
#endif //_DH_BATTLEZONE2_

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    //전장 입장 Lock
    if (ChaosSystem::GetChaosZoneData().is_wait() == true)
    {
        return true;
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

    return false;
}
//----------------------------------------------------------------------
// 메시지 출력 관련

void GlobalFunc::default_OutputMsg( int eMsgType, TCHAR * pszMessage, int iDisplayTime/* = 10000*/ )
{	
	if ( eMsgType & eOUTPUT_MSG_TOP_SHOW )
	{
		WzColor color = c_Chat_NoticeColor;
		g_InterfaceManager.ShowMessageBox(pszMessage , iDisplayTime , &color, 0);
	}
	
	if ( eMsgType & eOUTPUT_MSG_CENTER_SHOW )
	{
		WzColor color = c_Chat_NoticeColor;
		g_InterfaceManager.ShowCenterMessageBox( pszMessage, iDisplayTime , &color, FALSE );
	}

	if ( eMsgType & eOUTPUT_MSG_CHAT )
	{
		ChatDialog * pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);//(ChatDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);
		if ( pChatDlg )
		{
			pChatDlg->AddString( pszMessage );
		}
	}

	if ( eMsgType & eOUTPUT_MSG_OUPUT_DEBUG )
	{
		OutputDebugString( pszMessage );
	}

#ifdef _USE_LUA_SCRIPT
    if ( eMsgType & eOUTPUT_MSG_LUA_CONSOLE )
    {
        CWinConsole::Write(pszMessage);
    }
#endif//
}

//----------------------------------------------------------------------------
/**
*/
void GlobalFunc::OutputMsg_SysAlertMsg( const TCHAR * pszMessage, ...)
{
	TCHAR	szOutputMsg[512] = {0,};

	va_list args; 
	va_start(args, pszMessage); 
	_vsntprintf( szOutputMsg, 512, pszMessage, args ); 
	va_end(args); 

	// 채팅창과 화면 중앙 위쪽에 동시에 출력
	default_OutputMsg( eOUTPUT_MSG_CHAT, szOutputMsg);
	g_InterfaceManager.ShowMessageBox( szOutputMsg, -1, NULL, 0 );	// 다른 메시지 오기전까지 계속 노출됨
}

//----------------------------------------------------------------------------
/**
*/
void GlobalFunc::OutputMsg_SysMsg( const TCHAR * pszMessage, ...)
{
	TCHAR	szOutputMsg[512] = {0,};

	va_list args; 
	va_start(args, pszMessage); 
	_vsntprintf( szOutputMsg, 512, pszMessage, args ); 
	va_end(args); 

	// 채팅창과 화면 중앙 위쪽에 동시에 출력
	default_OutputMsg( eOUTPUT_MSG_CHAT, szOutputMsg);
	g_InterfaceManager.ShowMessageBox(szOutputMsg, 10000, NULL, 0);
}

//----------------------------------------------------------------------------
/**
*/
void GlobalFunc::OutputMsg_Notice( int eMsgType, const TCHAR * pszMessage, ...)
{
	TCHAR	szOutputMsg[512] = {0,};

	va_list args; 
	va_start(args, pszMessage); 
	_vsntprintf( szOutputMsg, 512, pszMessage, args ); 
	va_end(args); 

	default_OutputMsg( eMsgType, szOutputMsg);
}

void GlobalFunc::OutputMsg_PCBang_Notice(TCHAR* pszMessage, ...)
{
	TCHAR	szOutputMsg[512] = {0,};

	va_list args; 
	va_start(args, pszMessage); 
	_vsntprintf( szOutputMsg, 512, pszMessage, args ); 
	va_end(args); 

	default_OutputMsg( eOUTPUT_MSG_CENTER_SHOW, szOutputMsg, 20000);
}

//----------------------------------------------------------------------------
/**
*/
void 
GlobalFunc::OutputMsg_Error( int eMsgType, const TCHAR * pszMessage, ...)
{
	TCHAR	szOutputMsg[512] = {0,};

	va_list args; 
	va_start(args, pszMessage); 
	_vsntprintf( szOutputMsg, 512, pszMessage, args ); 
	va_end(args); 

	default_OutputMsg( eMsgType, szOutputMsg);

}

//----------------------------------------------------------------------------
/**
*/
void 
GlobalFunc::OutputMsg_Guide( int eMsgType, const TCHAR * pszMessage, ...)
{
	TCHAR	szOutputMsg[512] = {0,};

	va_list args; 
	va_start(args, pszMessage); 
	_vsntprintf( szOutputMsg, 512, pszMessage, args ); 
	va_end(args); 

	default_OutputMsg( eMsgType, szOutputMsg);
}

//----------------------------------------------------------------------------
/**
*/
void 
GlobalFunc::OutputMsg_Warning( int eMsgType, const TCHAR * pszMessage, ...)
{
	TCHAR	szOutputMsg[512] = {0,};

	va_list args; 
	va_start(args, pszMessage); 
	_vsntprintf( szOutputMsg, 512, pszMessage, args ); 
	va_end(args); 

	default_OutputMsg( eMsgType, szOutputMsg);
}

void GlobalFunc::DisplayMessageForTrigger( BOOL bDone, int iTextID, BYTE byIsParty, int iDisplayWindow, int iDisplaySwitch, DWORD dwObjectKey)
{
	if (bDone)
	{
		return;
	}

	if( MESSAGE_INFO* pMessageInfo = MessageInfoParser::Instance()->GetMessage( iTextID ) )
	{
		uiHelpMan* helpMan =
			static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

		int iType = (int)pMessageInfo->byType;
		switch(iType)
		{
		case MESSAGE_TYPE_AREA_NAME:		// 지역명
            if( iDisplaySwitch == 0 )
			{
				g_InterfaceManager.ShowMissionAreaInfo( pMessageInfo->pszMessage, pMessageInfo->iTime, NULL );
			}					
			break;

		case MESSAGE_TYPE_NORMAL:			// 가운데 시스템 메시지 + 채팅창 시스템 메시지
			if ( iDisplaySwitch == 0 )
			{
				g_InterfaceManager.ShowMessageBox( pMessageInfo->pszMessage, pMessageInfo->iTime, NULL, 0 );

				TCHAR szBuffer[MAX_SHOWMESSAGE_LENGTH] = {0,};
				TCHAR szMessage[MAX_SHOWMESSAGE_LENGTH] = {0,};
				StrnCopy(szBuffer,pMessageInfo->pszMessage, pMessageInfo->MessageLen);

				// 다이얼로그 창 내부에서 길이 개행 및 길이 처리
				GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szBuffer );					
			}
			break;

		case MESSAGE_TYPE_NPC:				// 가운데 시스템 메시지 + 이그니스창 
            if( iDisplaySwitch == 0 )
			{
				g_InterfaceManager.ShowMessageBox( pMessageInfo->pszMessage, pMessageInfo->iTime, NULL, 0 );

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
                if (helpMan)
				{
					helpMan->ShowHelpAnnounce(pMessageInfo->dwDCode,TRUE);
				}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW
			}
			break;

		case MESSAGE_TYPE_TRIGGER:			// 가운데 시스템 메시지 + 아이템 정보창
            if( iDisplaySwitch == 0 )
			{
                g_InterfaceManager.ShowMessageBox( pMessageInfo->pszMessage, pMessageInfo->iTime, NULL, 0 );

                if (helpMan)
                {
                    helpMan->AddHelpMission(iTextID, pMessageInfo->dwNCode, pMessageInfo->dwDCode, pMessageInfo->dwImgCode);
                }
            }
            else
            {
                // 아이템 제거
                if (helpMan)
                {
                    helpMan->DelHelpMission(iTextID);
                }
            }
			break;

		case MESSAGE_TYPE_NPC_ONLY:			// 이그니스창 
			if( iDisplaySwitch == 0 )
			{
#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
				if (helpMan)
				{
					helpMan->ShowHelpAnnounce(pMessageInfo->dwDCode,TRUE);
				}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW
			}
			break;

		case MESSAGE_TYPE_TRIGGER_ONLY: //아이템 정보창
            if( iDisplaySwitch == 0 )
            {
                if (helpMan)
                {
                    helpMan->AddHelpMission(iTextID, pMessageInfo->dwNCode, pMessageInfo->dwDCode, pMessageInfo->dwImgCode);
                }
            }
            else
            {
				// 아이템 제거
				if (helpMan)
				{
					helpMan->DelHelpMission(iTextID);
				}
			}
			break;

		case MESSAGE_TYPE_CENTER_MSG_ONLY:
			{
				default_OutputMsg(eOUTPUT_MSG_TOP_SHOW, pMessageInfo->pszMessage, pMessageInfo->iTime );
			}
			break;

		case MESSAGE_TYPE_GUILD_MESSAGE:
			if( uiNoticeMan* pUIMan = GET_DIALOG_MANAGER(uiNoticeMan, UIMAN_NOTICE) )
			{
				pUIMan->ShowNoticeMessage(eGuild_Notice, pMessageInfo->pszMessage, pMessageInfo->iTime);
			}
			break;

		case MESSAGE_TYPE_BATTLE_RESULT:			// 
			if( uiBattleScoreMan* battle_score_manager_ptr = 
                static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE)) )
            {
			    if (battle_score_manager_ptr)
			    {
                    battle_score_manager_ptr->set_toggle_ui_open(true);
				    battle_score_manager_ptr->SendPacketBattleResultSYN();
			    }
            }
			break;
            // 오브젝트 말풍선 대사
        case MESSAGE_TYPE_OBJECT_BALLOON_MSGBOX:
            // 오브젝트를 얻어와서 말풍선 출력
            if( Object* object = g_ObjectManager.GetObject(dwObjectKey) )
            {
                object->AddChatMessage(pMessageInfo->pszMessage);
            }
            break;
        case MESSAGE_TYPE_OBJECT_BALLOON_MSGBOX_WITH_SYSTEM:
            // 오브젝트 말풍선 대사 및 시스템메시지
            // 오브젝트를 얻어와서 말풍선 출력
            if( Object* object = g_ObjectManager.GetObject(dwObjectKey) )
            {
                object->AddChatMessage(pMessageInfo->pszMessage);
            }
            // 시스템메시지에 출력
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, pMessageInfo->pszMessage);
            break;
		}
	}
	else 
	{
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "Invalid MessageInfo: %d", iTextID);
	}
}

//----------------------------------------------------------------------------
/**
*/
LONG 
GlobalFunc::GetTextHeight(TCHAR* pText, WzID wiFont)
{
	SIZE	TextSize;
	int iTextLength = (int)_tcslen(pText);

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont(wiFont);
	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(pText, iTextLength, &TextSize );
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();

	return TextSize.cy;

}

//----------------------------------------------------------------------------
/**
*/
LPCSTR GlobalFunc::ReplaceFileExt(LPCSTR FileName, LPCSTR NewExt )
{
    static char szBufStr[MAX_PATH] = { 0, };
    static char szDir[MAX_PATH] = { 0, };
    static char szFName[MAX_PATH] = { 0, };
    static char szExt[MAX_PATH] = { 0, };

    _snprintf( szBufStr, MAX_PATH, "%s", FileName);
    _splitpath(szBufStr, NULL, szDir, szFName, szExt);

    sprintf(szBufStr, "%s%s.%s", szDir, szFName, NewExt);

    return szBufStr;        // 항상 같은 주소를 리턴하는것에 주의
}

//----------------------------------------------------------------------------
/**
*/
LONG 
GlobalFunc::GetTextWidth(TCHAR* pText, WzID wiFont)
{
	SIZE	TextSize;
	int iTextLength = (int)_tcslen(pText);

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont(wiFont);
	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(pText, iTextLength, &TextSize );
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();

	return TextSize.cx;

}

//----------------------------------------------------------------------------
/**
*/
BOOL 
GlobalFunc::IsSlang(TCHAR* pText, bool bShowMsg)
{
#if defined(_GLOBAL) || defined(_RUSSIA)
    return FALSE;
#endif//defined(_GLOBAL) || defined(_RUSSIA)
	char	szString[MAX_MESSAGE_LENGTH] = {0,};
	TCHAR	szMessage[MAX_MESSAGE_LENGTH] = {0,};

	// 금지단어 check
	if (g_slangFilter.IsHaveReplace(pText))
	{
		//	사용 금지 단어입니다
		if(bShowMsg)
		{
			g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );
			OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/** 
    source_text   : 금칙어 검사를 할 원본 텍스트
    dest_buffer   : 금칙어를 대체문자로 대치하여 저장할 텍스트 버퍼
    buffer_length : 버퍼의 길이

    return value
    true  : 금칙어가 존재하며 금칙어를 대체문자로 대치하여 버퍼에 출력함.
    false : 금칙어가 존재하지 않음.
*/
bool GlobalFunc::ReplaceSlangWord(TCHAR* source_text, TCHAR* dest_buffer, int buffer_size)
{
    bool is_replace = g_slangFilter.IsHaveReplace(source_text, dest_buffer, buffer_size);
    return is_replace;
}
//----------------------------------------------------------------------------
/**
*/
void 
GlobalFunc::GetWideString(TCHAR *ptszInString, TCHAR *ptszOutString, int iStringLen)
{
	TCHAR	szWideString[INTERFACE_STRING_LENGTH] = {0,};

	StrnCopy(szWideString, ptszInString, iStringLen);

	StrnCopy(ptszOutString, szWideString, iStringLen);
}
//----------------------------------------------------------------------------
/**
*/
bool GlobalFunc::CovertSimpleText( TCHAR *pMsg,int nSize ,DWORD dwFontID,RECT &rc)
{
	if(!_tcslen(pMsg)) return false;

	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	int  step1 = 0;  // szText Inc
	int  step2 = 0;  // szTemp Inc
	int  width  = 0;
	BOOL complate = FALSE;
	int  nRectWidth = rc.right-rc.left;
	char * ellipsis = " ...";

	int ellipwidth = GetTextWidth( ellipsis, dwFontID );
	ellipwidth += 20;	

	while(1)
	{
		if (_ismbblead(pMsg[step1]))
		{
			szTemp[step2] = pMsg[step1];
			step1++;
			step2++;
			szTemp[step2] = pMsg[step1];
			step1++;
			step2++;
		}
		else
		{
			szTemp[step2] = pMsg[step1];
			step1++;
			step2++;
		}

		if( pMsg[step1] == '\0' )
		{
			complate = TRUE;
			break;
		}

		width = GetTextWidth( szTemp, dwFontID );

		if( width >= nRectWidth-ellipwidth )
			break;
	}

	if( complate )
	{
        StrnCopy(szMessage, szTemp, INTERFACE_STRING_LENGTH - 1);
		//Snprintf( szMessage, INTERFACE_STRING_LENGTH-1, szTemp );
	}
	else
	{
        StrnCopy(szMessage, szTemp, INTERFACE_STRING_LENGTH - 1);
        strcat(szMessage, ellipsis);
		//Snprintf( szMessage, INTERFACE_STRING_LENGTH-1, _T("%s%s"), szTemp, ellipsis );
	}

	memcpy(pMsg,szMessage,nSize);

	return (complate) ? (false):(true);
}
//----------------------------------------------------------------------------
/**
*/
bool GlobalFunc::SetCaptionSimple( CControlWZ *pControl, INOUT TCHAR *pMsg,int nType)
{
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	CCtrlStaticWZ* pStaticWZ=(CCtrlStaticWZ*)pControl;
	CCtrlButtonWZ* pButtonWZ=(CCtrlButtonWZ*)pControl;
	
	DWORD dwFontID = (nType==0) ? (pStaticWZ->GetFontID() ):(pButtonWZ->GetFontID());
	
	RECT rc = pControl->GetSize();
    
    int step1 = 0;  // szText Inc
    int step2 = 0;  // szTemp Inc
	int width  = 0;
	BOOL complate = FALSE;
	char * ellipsis = " ...";

	int ellipwidth = GetTextWidth( ellipsis, dwFontID );
	ellipwidth += 20;	

    while(1)
    {
        if (_ismbblead(pMsg[step1]))
        {
            szTemp[step2] = pMsg[step1];
            step1++;
            step2++;
            szTemp[step2] = pMsg[step1];
            step1++;
            step2++;
        }
        else
        {
            szTemp[step2] = pMsg[step1];
            step1++;
            step2++;
        }

		if( pMsg[step1] == '\0' )
		{
			complate = TRUE;
			break;
		}

		width = GetTextWidth( szTemp, dwFontID );

		if( width >= rc.right - ellipwidth )
			break;
    }

	if( complate )
	{
		Snprintf( szMessage, INTERFACE_STRING_LENGTH-1, szTemp );
	}
	else
	{
		Snprintf( szMessage, INTERFACE_STRING_LENGTH-1, _T("%s%s"), szTemp, ellipsis );
	}


	if( pControl )
	{
		if(nType==0)
			pStaticWZ->SetTextWZ( szMessage );
		else
			pButtonWZ->SetTextWZ( szMessage );
	}

	return (complate) ? (false):(true);
}

//----------------------------------------------------------------------------
/**
*/
bool
GlobalFunc::ProcessWordWrapByFontSize(std::vector<util::_tstring> *vstr, LPCTSTR szText, WzID wiFont, int strLen, int cutLen, bool bChatDialog /* = false */)
{
    if (!vstr)
        return false;

    TCHAR szTemp[4096];
    ZeroMemory(&szTemp, 4096);

    int step1 = 0;  // szText Inc
    int step2 = 0;  // szTemp Inc
	
	int iWidth = 0;

	BOOL bLineChange = FALSE;
	BOOL bMBC = FALSE;
    while(1)
    {
		//2줄 이상부터는 공란 2칸을 포함한다(GM 사칭방지)
		if( bChatDialog && bLineChange)
		{
			szTemp[step2] = ' ';
			step2++;
			szTemp[step2] = ' ';
			step2++;
			bLineChange = FALSE;
		}

		if( bChatDialog == false )
		{
			if( ( szText[step1] == '\n' ) || ( szText[step1] == '#' ) )
			{
				// lammy : 처음부터 개행이라면 패스
				if( step2 == 0 )
				{
					step1++;
					continue;
				}
				vstr->push_back(szTemp);
				ZeroMemory(&szTemp, 4096);
				step1 += 1;
				step2 = 0;
				continue;
			}

			if( !_ismbblead( szText[step1] ) )
			{
				if( ( szText[step1] == '\\' ) )
				{
					if (szText[step1 + 1] == 'n')
					{
						vstr->push_back(szTemp);
						ZeroMemory(&szTemp, 4096);
						step1 += 2;
						step2 = 0;
						continue;
					}
				}
			}
		}

        if (_ismbblead(szText[step1]))
        {
            szTemp[step2] = szText[step1];
            step1++;
            step2++;
            szTemp[step2] = szText[step1];
            step1++;
            step2++;
			bMBC = TRUE;
        }
        else
        {
			// lammy : 젤 처음이 공백이라면 넣지 말자.
			if( step2 == 0 )				
			{
				if( szText[step1] == ' ' )
				{
					step1++;
					continue;
				}
			}

            szTemp[step2] = szText[step1];
            step1++;
            step2++;
			bMBC = FALSE;
        }

		iWidth = GetTextWidth(szTemp, wiFont);
		if( iWidth >= cutLen - 6 )
        {
			int backstep = 1;
			if(bMBC)
			{
				backstep = 2;

			}
			step1-= backstep;
			step2-= backstep;
			memset(&szTemp[step2], 0, backstep);	
#ifdef	_INTERNATIONAL_UI
			if(!bChatDialog)
			{
				BOOL bReCover = FALSE;

				TCHAR szReCover[4096];
				memcpy(szReCover,szTemp,sizeof(szTemp));

				if( !_ismbblead( szText[step1] ) )
				{
					TCHAR * tok = " ";

					if( _tcscmp( &szTemp[step1], tok ) != 0 )
					{
						int n = step2;

						for( ; ; )
						{
							if( _tcscmp( &szTemp[n], tok ) == 0 )
								break;

							szTemp[n] = '\0';
							n--;
							
							//safecode
							if(n<=0)
							{
								bReCover = TRUE;
								break;
							}
						}

						//safecode
						if(bReCover)
						{
							memcpy(szTemp,szReCover,sizeof(szTemp));
						}
						else
						{
							step1 -= (step2 - n);
						}	
					}
				}
			}
#endif

			// lammy : 마지막문자가 마침표라면 같이 붙여 주자.
			if( _ismbblead(szText[step1]) )
			{
				// 일본 버전 전용
				// 、。일경우는 개행하지 않고 끝에 붙여준다.
				if( szText[step1 + 1] == 0x41 || szText[step1 + 1] == 0x42 )
				{
					szTemp[step2] = szText[step1];
					step1++;
					step2++;
					szTemp[step2] = szText[step1];
					step1++;
					step2++;
				}
			}
			else
			{
				if( szText[step1] == '.' || szText[step1] == ',' )
				{
					szTemp[step2] = szText[step1];
					step1++;
					step2++;
				}
			}

            vstr->push_back(szTemp);
            ZeroMemory(&szTemp, 4096);
            step2 = 0;
			bLineChange = TRUE;
        }

        if( step1 >= strLen )
        {
            vstr->push_back(szTemp);
            break;
        }
	}
    return true;
}

#ifdef _NA_000000_20130114_RENEWER_UI
bool GlobalFunc::ProcessWordWrapByFontSize2( std::vector<util::_tstring> *vstr, LPCTSTR szText, WzID wiFont, 
											int strLen, int cutLen, bool bChatDialog /*= false*/,bool bTapCheck /*= false*/ )
{
	if (!vstr)
		return false;

	TCHAR szTemp[4096];
	ZeroMemory(&szTemp, 4096);

	int step1 = 0;  // szText Inc
	int step2 = 0;  // szTemp Inc

	int iWidth = 0;

	BOOL bLineChange = FALSE;
	BOOL bMBC = FALSE;
	while(1)
	{
		//2줄 이상부터는 공란 2칸을 포함한다(GM 사칭방지)
		if( bChatDialog && bLineChange)
		{
			szTemp[step2] = ' ';
			step2++;
			szTemp[step2] = ' ';
			step2++;
			bLineChange = FALSE;
		}

		if( bChatDialog == false )
		{
			if( ( szText[step1] == '\n' ) || ( szText[step1] == '#' ) )
			{
				// lammy : 처음부터 개행이라면 패스
				if( step2 == 0 )
				{
					step1++;
					continue;
				}
				vstr->push_back(szTemp);
				ZeroMemory(&szTemp, 4096);
				step1 += 1;
				step2 = 0;
				continue;
			}

			if( !_ismbblead( szText[step1] ) )
			{
				if( ( szText[step1] == '\\' ) )
				{
					if (szText[step1 + 1] == 'n')
					{
						vstr->push_back(szTemp);
						ZeroMemory(&szTemp, 4096);
						step1 += 2;
						step2 = 0;
						continue;
					}
				}
			}
		}

		if (_ismbblead(szText[step1]))
		{
			szTemp[step2] = szText[step1];
			step1++;
			step2++;
			szTemp[step2] = szText[step1];
			step1++;
			step2++;
			bMBC = TRUE;
		}
		else
		{
			// lammy : 젤 처음이 공백이라면 넣지 말자.
			if( step2 == 0  && bTapCheck == false)				
			{
				if( szText[step1] == ' ' )
				{
					step1++;
					continue;
				}
			}

			szTemp[step2] = szText[step1];
			step1++;
			step2++;
			bMBC = FALSE;
		}

		iWidth = GetTextWidth(szTemp, wiFont);
		if( iWidth >= cutLen - 6 )
		{
			int backstep = 1;
			if(bMBC)
			{
				backstep = 2;

			}
			step1-= backstep;
			step2-= backstep;
			memset(&szTemp[step2], 0, backstep);	
#ifdef	_INTERNATIONAL_UI
			if(!bChatDialog)
			{
				BOOL bReCover = FALSE;

				TCHAR szReCover[4096];
				memcpy(szReCover,szTemp,sizeof(szTemp));

				if( !_ismbblead( szText[step1] ) )
				{
					TCHAR * tok = " ";

					if( _tcscmp( &szTemp[step1], tok ) != 0 )
					{
						int n = step2;

						for( ; ; )
						{
							if( _tcscmp( &szTemp[n], tok ) == 0 )
								break;

							szTemp[n] = '\0';
							n--;

							//safecode
							if(n<=0)
							{
								bReCover = TRUE;
								break;
							}
						}

						//safecode
						if(bReCover)
						{
							memcpy(szTemp,szReCover,sizeof(szTemp));
						}
						else
						{
							step1 -= (step2 - n);
						}	
					}
				}
			}
#endif

			// lammy : 마지막문자가 마침표라면 같이 붙여 주자.
			if( _ismbblead(szText[step1]) )
			{
				// 일본 버전 전용
				// 、。일경우는 개행하지 않고 끝에 붙여준다.
				if( szText[step1 + 1] == 0x41 || szText[step1 + 1] == 0x42 )
				{
					szTemp[step2] = szText[step1];
					step1++;
					step2++;
					szTemp[step2] = szText[step1];
					step1++;
					step2++;
				}
			}
			else
			{
				if( szText[step1] == '.' || szText[step1] == ',' )
				{
					szTemp[step2] = szText[step1];
					step1++;
					step2++;
				}
			}

			vstr->push_back(szTemp);
			ZeroMemory(&szTemp, 4096);
			step2 = 0;
			bLineChange = TRUE;
		}

		if( step1 >= strLen )
		{
			vstr->push_back(szTemp);
			break;
		}
	}
	return true;
}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

//------------------------------------------------------------------------------ 
// 텍스트 복사가 좀 많으므로 랜더나 업데이트등에서의 호출은 자제
void GlobalFunc::SetWordWarpText( CControlWZ* control )
{
    if( control != NULL )
    {
        TCHAR new_text[4096];
        ZeroMemory(&new_text,4096);

        TCHAR old_text[4096];
        control->GetTextWZA( old_text, 4096 );

        if( ProcessWordWrapByFontSize( new_text, old_text, control->GetFontID(), _tcslen(old_text), control->GetSize().right, -1, true ) )
        {
            control->SetTextWZA( new_text );
        }
    }
}

//------------------------------------------------------------------------------ 
int GlobalFunc::ProcessWordWrapByFontSize(OUT TCHAR* out_msg, IN LPCTSTR in_msg, IN WzID font_id, 
                                          IN int str_len, IN int area_width, 
                                          IN int valid_line/* = -1*/, IN bool is_reducing/* = false*/)
{
    std::vector<util::_tstring> msg_vector;
    msg_vector.clear();
    if (GlobalFunc::ProcessWordWrapByFontSize(&msg_vector, in_msg, font_id, _tcslen(in_msg), area_width) == true)
    {
        std::vector<util::_tstring>::iterator begin = msg_vector.begin();
        std::vector<util::_tstring>::iterator end = msg_vector.end();
        int line_index = 0;
        for (; begin != end; ++begin, ++line_index)
        {
            if ((valid_line > 0) && (line_index >= valid_line))
            {
                break;
            }
            if ((is_reducing == true) && (valid_line > 0) && (line_index == valid_line - 1))
            {
                //마지막줄 이면 말줄임
                int dot_width = GetTextWidth(_T("......"), font_id);

                //std::vector<util::_tstring> sub_msg_vector;
                //sub_msg_vector.clear();
                //if (GlobalFunc::ProcessWordWrapByFontSize(&sub_msg_vector, (TCHAR*)(begin->c_str()), font_id, _tcslen(begin->c_str()), area_width - dot_width) == true)
                //{
                //    _tcscat(out_msg, sub_msg_vector.begin()->c_str());
                //    if (sub_msg_vector.size() > 1)
                //    {
                //        _tcscat(out_msg, _T("......"));
                //    }
                //}

                int msg_width = GetTextWidth((TCHAR*)(begin->c_str()), font_id);
                if (msg_width + dot_width > area_width)
                {
                    // 다시 자른다.
                    std::vector<util::_tstring> sub_msg_vector;
                    sub_msg_vector.clear();
                    if (GlobalFunc::ProcessWordWrapByFontSize(&sub_msg_vector, (TCHAR*)(begin->c_str()), font_id, _tcslen(begin->c_str()), area_width - dot_width) == true)
                    {
                        _tcscat(out_msg, sub_msg_vector.begin()->c_str());
                        _tcscat(out_msg, _T("......"));
                    }
                    else
                    {
                        _tcscat(out_msg, begin->c_str());
                    }
                }
                else
                {
                    _tcscat(out_msg, begin->c_str());
                }
            }
            else
            {
                _tcscat(out_msg, begin->c_str());
            }
            _tcscat(out_msg, _T("\n"));
        }
    }

    return static_cast<int>(msg_vector.size());
}
//----------------------------------------------------------------------------
/**
*/
void
GlobalFunc::ChangeMultiEditInput(WCHAR *pwszInput, TCHAR *pszOutput)
{
	int iSize = wcslen(pwszInput);
	
	for(int i=0; i<iSize; ++i)
	{
		if(pwszInput[i] == '♧')
			pwszInput[i] = L'\n';
	}

	WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, pwszInput, -1, pszOutput, INTERFACE_STRING_LENGTH, NULL, NULL );

}
//----------------------------------------------------------------------------
/**
*/
void
GlobalFunc::ChangeMultiEditOutput(TCHAR *pszInput, WCHAR *pszwOutput)
{
    int iSize = strlen(pszInput);
    int focus = 0;

    int i = 0;

    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszInput, -1, pszwOutput, INTERFACE_STRING_LENGTH);
    //strcpy(pszOutput, pszInput);
    for(int i=0; i<iSize; ++i)
    {
        if(pszwOutput[i] == L'\n')
        {
            pszwOutput[i] = '♧';
        }
    }
    //WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, pwszInput, -1, pszOutput, INTERFACE_STRING_LENGTH, NULL, NULL );
}
//----------------------------------------------------------------------------
/**
*/
typedef struct MONEY_COLOR_TABLE
{
    DWORD money_color;    
    MONEY money_unit;
}MONEY_COLOR;

DWORD GlobalFunc::GetMoneyColor(MONEY money)
{
    DWORD money_color = RGBA(255,255,255,255);
    MONEY_COLOR money_colors[] = { 
#ifdef _DH_MONEY_COLOR_CHANGE
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
		{ RGBA(153,102,255,255), 100000000000 }, // 1000억 0xff9966ff
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION
        { RGBA(255,0,0,255),     10000000000 }, // 100억 붉은색
        { RGBA(227,108,10,255),  1000000000 }, // 10억 주황색
        { RGBA(255,255,0,255),   100000000 }, // 1억 노랑색
        { RGBA(255,0,255,255),   10000000 }, // 1000만 핑크색
        { RGBA(0,176,80,255),    1000000 }, // 100만 녹색
        { RGBA(0,176,240,255),   100000 }, // 10만 하늘색
        { RGBA(255,255,255,255), 0 }, //흰색 0
#else
        { RGBA(123,76,166,255),  1000000000 }, // 10억 보라색
        { RGBA(189,156,89,255),  100000000 }, // 1억 금색
        { RGBA(255,32,32,255),   10000000 }, // 1000만 붉은색
        { RGBA(71,255,1,255),    1000000 }, // 100만 녹색
        { RGBA(86,185,226,255),  100000 }, // 10만 하늘색
        { RGBA(255,184,226,255), 10000 }, // 1만 연핑크
        { RGBA(255,255,255,255), 0 }, // 0 흰색
#endif//DH_MONEY_COLOR_CHANGE
    };

    unsigned char unit = 0;
    while (money_colors[unit].money_unit != 0)
    {
        if (money >= money_colors[unit].money_unit)
        {
            money_color = money_colors[unit].money_color;
            break;
        }
        ++unit;
    }

    /*DWORD money_color = RGBA(255,255,255,255);
    if( money >=  1000000000)
    {
        money_color = RGBA(123,76,166,255);
    }
    else if( money >=  100000000)
    {
        money_color = RGBA(189,156,89,255);
    }
    else if( money >=  10000000)
    {
        money_color = RGBA(255,32,32,255);
    }
    else if ( money >= 1000000)
    {
        money_color = RGBA(71,255,1,255);
    }
    else if ( money >= 100000 )
    {
        money_color = RGBA(86,185,226,255);
    }
    else if (money >= 10000)
    {
        money_color = RGBA(255,184,226,255);
    }*/
    
    return money_color;
}

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
DWORD GlobalFunc::GetMoneyWzColor( MONEY money )
{
	DWORD money_color = WzColor_RGBA(255,255,255,255);
	MONEY_COLOR money_colors[] = { 
		{ WzColor_RGBA(153,102,255,255), 100000000000 }, // 1000억 보라색
		{ WzColor_RGBA(255,0,0,255),     10000000000 }, // 100억 붉은색
		{ WzColor_RGBA(227,108,10,255),  1000000000 }, // 10억 주황색
		{ WzColor_RGBA(255,255,0,255),   100000000 }, // 1억 노랑색
		{ WzColor_RGBA(255,0,255,255),   10000000 }, // 1000만 핑크색
		{ WzColor_RGBA(0,176,80,255),    1000000 }, // 100만 녹색
		{ WzColor_RGBA(0,176,240,255),   100000 }, // 10만 하늘색
		{ WzColor_RGBA(255,255,255,255), 0 }, //흰색 0
	};

	unsigned char unit = 0;
	while (money_colors[unit].money_unit != 0)
	{
		if (money >= money_colors[unit].money_unit)
		{
			money_color = money_colors[unit].money_color;
			break;
		}
		++unit;
	}

	return money_color;
}

std::string GlobalFunc::GetMoneyColorHexForString( MONEY money )
{
	std::string color_string;

	struct MoneyColorForString
	{
		std::string money_color_string;
		MONEY money_unit;
	};

	MoneyColorForString money_colors[] = {
	    { "ff9966ff",     100000000000 }, // 1000억 0xff9966ff
	    { "ffff0000",     10000000000 }, // 100억 붉은색
	    { "ffe36c0a",     1000000000 }, // 10억 주황색
	    { "ffffff00",     100000000 }, // 1억 노랑색
	    { "ffff00ff",     10000000 }, // 1000만 핑크색
	    { "ff00b050",     1000000 }, // 100만 녹색
	    { "ff00b0f0",     100000 }, // 10만 하늘색
	    { "ffffffff",     0 }, //흰색 0
	};

	unsigned char unit = 0;
	while (money_colors[unit].money_unit != 0)
	{
		if (money >= money_colors[unit].money_unit)
		{
			color_string = money_colors[unit].money_color_string;
			break;
		}
		++unit;
	}

	if (color_string == "")
	{
		color_string = "ffffffff";
	}

	return color_string;
}
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION

#ifdef _JBH_MOD_MONEY_OUTPUT
// 돈을 한글로 읽어주는 함수.
VOID GlobalFunc::ChangeMoneyToStringMoney ( MONEY money , TCHAR * pszResult)
{
	// 국내 버전에서만 적용한다. 스트링 테이블 필요 X
	// Generalparam 참조하여 KOREA가 아니면 툴팁을 등록 시키지 않도록 처리.
	TCHAR szNumber[9][3] = 
	{
		{"일"},
		{"이"},
		{"삼"},
		{"사"},
		{"오"},
		{"육"},
		{"칠"},
		{"팔"},
		{"구"},
	};

	if( money > 0 )
	{
		int count = 0;
		int value = 0;
		TCHAR StringMoney[20];
		TCHAR OneChar;
		_itot(money , StringMoney , 10);
		int Stringlength = _tcslen(StringMoney);

		pszResult[0] = '\0';

		for( int i = 0 ; i < Stringlength ; i++ )
		{
			int length = _tcslen(pszResult);
			OneChar  = StringMoney[i];
			value = atoi(&OneChar) ;
			int index = Stringlength - i;
			if( value ==  1 )
			{
				// ' 일' 자는 억단위 이상에만 붙인다.
				if( index > 8 || index == 1 )
					StrnCopy(&pszResult[length] , szNumber[value -1] , 3 );
			}
			else if ( value > 1 )
			{
				// 이 , 삼 , 사 , 오 ~~ 는 모든 수에 붙인다.
				StrnCopy(&pszResult[length] , szNumber[value -1] , 3 );
			}
			// 단위 입력
			
			switch ( index )
			{
			case  9:
				{
					_tcscat(pszResult , _T("억") );
				}
				break;
			case 5:
				{
					if( money - 100000000 >= 10000)
						_tcscat(pszResult , _T("만") );
				}
				break;
			case 2:
			case 6:
			case 10:
				{
					if( value > 0 )
						_tcscat(pszResult , _T("십") );
				}
				break;

			case 3:
			case 7:
			case 11:
				{
					if( value > 0)
						_tcscat(pszResult , _T("백") );
				}
				break;

			case 4:
			case 8:
			case 12:
				{
					if( value > 0)
						_tcscat(pszResult , _T("천") );	
				}
				break;
			}
		}
		_tcscat(pszResult, _T(" 하임") );

		int length = _tcslen(pszResult);
		pszResult[length] = '\0';
	}
}

#endif


HANDLE GlobalFunc::CreateEffectBeUpToOption(WzID wzEffID, WzVector wvPos,DWORD dwId,DWORD dwId2)
{
	HANDLE hHandle=INVALID_HANDLE_VALUE;

    WzVector wvScale = {1.f, 1.f, 1.f};
    WzVector wvRot   = {0.f, 0.f, 0.f};

#ifdef _DEBUG
    //! 디버깅용
    char str[256] = {NULL, };
    WzIDToString( str, wzEffID );
#endif

    hHandle = CreateEffectBeUpToOption(wzEffID, wvPos, wvScale, wvRot, dwId, dwId2);

	return hHandle;
}

HANDLE GlobalFunc::CreateEffectBeUpToOption(WzID wzEffID, WzVector wvPos, WzVector wvScale,DWORD dwId,DWORD dwId2)
{
	HANDLE hHandle=INVALID_HANDLE_VALUE;

    //WzVector wvRot = {1.f, 1.f, 1.f};
    WzVector wvRot = {0.0f, 0.0f, 0.0f};
    hHandle = CreateEffectBeUpToOption(wzEffID, wvPos, wvScale, wvRot, dwId, dwId2);
                
	return hHandle;
}

HANDLE GlobalFunc::CreateEffectBeUpToOption(WzID wzEffID, WzVector wvPos, WzVector wvScale, WzVector wvRot,DWORD dwId,DWORD dwId2)
{
#ifdef _DEBUG
    //! 디버깅용
    char str[256] = {NULL, };
    WzIDToString( str, wzEffID );
    if (Stricmp(str, "B006") == 0)
    {
        int a = 0;
    }
    
#endif
	HANDLE hHandle = INVALID_HANDLE_VALUE;

    if ((g_pHero == NULL) && 
        (GameFramework::GetCurScene() != SCENE_TYPE_CHARSELECT) && 
        (GameFramework::GetCurScene() != SCENE_TYPE_LOAD) && 
        (GameFramework::GetCurScene() != SCENE_TYPE_NONE))
    {
        return hHandle;
    }

    if ((GameFramework::GetCurScene() == SCENE_TYPE_CHARSELECT) || 
        (GameFramework::GetCurScene() == SCENE_TYPE_LOAD) || 
        (GameFramework::GetCurScene() == SCENE_TYPE_NONE))
	{
		hHandle = g_pEffectManager->CreateEffect( wzEffID, wvPos, wvScale, wvRot );
	}
	else
	{
		St_BodyCreate* pBC = g_pEffectManager->GetEffectInfo(wzEffID);
		if (pBC)
		{
			St_Create_Info* pCI = (St_Create_Info*)(*pBC->m_stdList_Create.begin());//바디
			if (NULL == pCI)
            {
				return hHandle;
            }

			if (0 == pCI->m_byLoopType) //루프아님
			{
				WzVector wvLen = g_pHero->GetVisiblePos() - wvPos;
				float fLen2 = VectorLength2(&wvLen);
 
                if (g_pHero->GetObjectKey() == dwId || g_pHero->GetObjectKey() == dwId2 || (0 == dwId && 0 == dwId2))
                {
                    hHandle = g_pEffectManager->CreateEffect( wzEffID, wvPos, wvScale, wvRot, 100);
                }
                else
                {
                    //////////////////////////////////////////////////////////////////////////
                    //거리값의 제곱(fLen2)에 가중치(cEFFECT_DIST_WEIGHT)를 곱하기 때문에 
                    //거리가 멀어질수록 Rate는 제곱으로 떨어진다.
                    int lod_grade = GAMEOPTION->GetLOD();
                    if (lod_grade == eOPTION_LEVEL_HIGH)
                    {
                        fLen2 *= cEFFECT_DIST_WEIGHT_HIGH;
                    }
                    else if (lod_grade == eOPTION_LEVEL_MIDDLE)
                    {
                        fLen2 *= cEFFECT_DIST_WEIGHT_MIDDLE;
                    }
                    else if (lod_grade == eOPTION_LEVEL_LOW)
                    {
                        fLen2 *= cEFFECT_DIST_WEIGHT_LOW;
                    }

                    const float dist2 = DIST_FROM_NEAR * DIST_FROM_NEAR;
                    if (fLen2 > dist2){fLen2 = dist2;}
                    if (fLen2 < 0.f){fLen2 = 0.f;}
                    float temp_rate = 1.0f - fLen2/dist2;
                    int rate = static_cast<int>(100.f * temp_rate);

                    hHandle = g_pEffectManager->CreateEffect( wzEffID, wvPos, wvScale, wvRot, rate);
                    //////////////////////////////////////////////////////////////////////////
                }
			}
			else
			{
				hHandle = g_pEffectManager->CreateEffect( wzEffID, wvPos, wvScale, wvRot );
			}
		}
	}

	return hHandle;
}


void GlobalFunc::ErrorCode_Player_State(ePLAYER_BEHAVE_STATE eState)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	BOOL  m_bPrint = TRUE;
	switch ( eState )
	{
	case PLAYER_BEHAVE_TRADE_STATE:
		{
			//5814	상대방이 거래중입니다.
			g_InterfaceManager.GetInterfaceString(5814, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE:
		{
			// 5815	상대방이 개인상점을 개설 중입니다.
			g_InterfaceManager.GetInterfaceString(5815 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case PLAYER_BEHAVE_VENDOR_OBSERVER_STATE:
		{
			// 5816	상대방이 개인상점을 관람 중입니다.
			g_InterfaceManager.GetInterfaceString(5816 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case PLAYER_BEHAVE_NPCSHOP_STATE:
		{
			// 5817	상대방이 상점을 이용 중입니다.
			g_InterfaceManager.GetInterfaceString(5817 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case PLAYER_BEHAVE_PARTY_THINKING_LEAVE:
	case PLAYER_BEHAVE_PARTY_THINKING_JOIN:
		{
			// 5818	상대방의 파티초대 수락여부를 기다리고 있는 중입니다.
			g_InterfaceManager.GetInterfaceString(5818 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;
	case PLAYER_BEHAVE_GROUP_THINKING_LEAVE:
	case PLAYER_BEHAVE_GROUP_THINKING_JOIN:
		{
			// 7055	상대방의 그룹초대 수락여부를 기다리고 있는 중입니다.
			g_InterfaceManager.GetInterfaceString(7055 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case PLAYER_BEHAVE_SYSTEM_LOCK:
		{
			// 5819	상대방이 응답할 수 없는 상태입니다.
			g_InterfaceManager.GetInterfaceString(5819 , szMessage , INTERFACE_STRING_LENGTH);
		}
		break;

	case PLAYER_BEHAVE_BANK_STATE:
		{
			// 5820	상대방이 창고를 이용 중입니다.
			g_InterfaceManager.GetInterfaceString(5820 , szMessage , INTERFACE_STRING_LENGTH);
		}
		break;

	default:
		{
			// 표시하지 않는다.
			m_bPrint = FALSE;
		}
	}
	if( m_bPrint) 
	{
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage );
	}
}

#ifdef __CN_269_MATRIX_CARD_AUTH
void GlobalFunc::MatrixCodeSubmit( bool bYes)
{
    MatrixCardAuth * pMatrixCardAuth = ( MatrixCardAuth *) g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_MATRIX_CARD_AUTH);

	if( !pMatrixCardAuth)
		return;

	if( bYes )
	{
		uiPasswordMan * pPasswordMan = (uiPasswordMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD);
		if( !pPasswordMan )
			return;

		int iMatrixCode =  _ttoi(pPasswordMan->m_pUIPasswordDlg->GetStrPassword());
#ifdef __CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT
		pMatrixCardAuth->SEND_AUTH_MATRIX_CARD_ACK(iMatrixCode);
#else// __CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT
		pMatrixCardAuth->SetMatrixCardCode(iMatrixCode);
		if( pMatrixCardAuth->IsFinishInput() == FALSE)
		{

			pPasswordMan->m_pUIPasswordDlg->ShowInterface(TRUE);
			pPasswordMan->m_pUIPasswordDlg->_SetPsaawordMinMaxCount( 1, 3);
			
			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
			// 1231 보안 카드 인증
			g_InterfaceManager.GetInterfaceString(1231 , szMessage , INTERFACE_STRING_LENGTH);
			pPasswordMan->m_pUIPasswordDlg->_SetPasswordMessage(szMessage);

			RECT rt;
			pMatrixCardAuth->GetDialogWZ()->GetClientRect(&rt);
			pPasswordMan->m_pUIPasswordDlg->MoveWindowWZ( rt.right , rt.top );
		}
#endif //__CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT
	}
	else
	{
		pMatrixCardAuth->ShowInterface(FALSE);
		g_LoginScene.Disconnect();
		g_LoginScene.Connect();

		uiLoginMan * pLoginMan = ( uiLoginMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN);
		if( pLoginMan )
		{
			pLoginMan->resetTryReconnect();
		}	
	}
}
#endif


char * GlobalFunc::AddNationName(char * szString)
{


    static util::_tstring file_name;

    file_name.clear();

    return GlobalFunc::AddNationName((LanguageType)CGameOption::Instance()->GetLanguageType(), szString, file_name);
}

//------------------------------------------------------------------------------
char* GlobalFunc::AddNationName(IN LanguageType language_type, IN const char* file_name, OUT util::_tstring& new_file_name)
{
    if (NULL == file_name)
    {
        return NULL;
    }

    static char szFileName[512] = {0,};

    ZeroMemory(szFileName , 512);

    StrnCopy(szFileName, file_name, strlen(file_name) - 4);

    switch (language_type)
    {
        case kLanguageGlobal:   strcat( szFileName, "_en.txt");    break;
        case kLanguageKorea:    strcat( szFileName, ".txt");       break;
        case kLanguageChina:    strcat( szFileName, "_cn.txt");    break;
        case kLanguageTaiwan:   strcat( szFileName, "_tw.txt");    break;
        case kLanguageJapan:    strcat( szFileName, "_jp.txt");    break;
        case kLanguageGermany:  strcat( szFileName, "_ger.txt");   break;
        case kLanguageRussia:   strcat( szFileName, "_ru.txt");    break;
        default:                strcat( szFileName, ".txt");       break;
    }

    new_file_name = szFileName;

    return szFileName;
}

//------------------------------------------------------------------------------
bool GlobalFunc::HasFile(const char* file_name)
{
    ePACK_FILE_NAME disk_type = ePFN_NONE;

    if (CGeneralGameParam::Instance()->IsUsePackFile() == TRUE)
    {
        disk_type = ePFN_RESOURCE;
    }
    else
    {
        disk_type = ePFN_NOT_USE_PACKFILE;
    }

    DA_FILE hf = g_pApplication->GetDiscAccess()->OpenFile(disk_type, 
                                                            file_name, 
                                                            DA_READ, 
                                                            SM_READ, 
                                                            OD_OPEN_EXISTING);

    if (hf == INVALID_DA_FILE)
    {
        return false;
    }

    g_pApplication->GetDiscAccess()->CloseFile(hf);

    return true;
}

//------------------------------------------------------------------------------
bool GlobalFunc::HasFile_Nation(const char* file_name, LanguageType language_type)
{
    util::_tstring nation_file_name;

    if (AddNationName(language_type, file_name, OUT nation_file_name) == NULL)
    {
        return false;
    }

    return HasFile(nation_file_name.c_str());
}

//-------------------------------------------------------------------------------------------
/**
*/

void GlobalFunc::GetCrashReportFileName(TCHAR* pszFileName)
{
	TCHAR szFileName[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szLogPath[MAX_PATH] = {0,};
	GetLogPath(szLogPath);
	Snprintf(szFileName, INTERFACE_STRING_LENGTH-1, _T("%s\\%s"), szLogPath, _T("CrashReport.log"));

	StrnCopy(pszFileName, szFileName, _tcslen(szFileName));
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::SetLogPath()
{
	TCHAR szLogPath[MAX_PATH] = {0,};
	TCHAR szCurDirectory[MAX_PATH] = {0,};
	TCHAR szLogDirectory[MAX_PATH] = {0,};

	GetCurrentDirectory(MAX_PATH, szCurDirectory); 
	Snprintf(szLogDirectory, MAX_PATH-1, _T("%s\\log"), szCurDirectory);
	_tmkdir(szLogDirectory);	
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::GetLogPath(TCHAR* pszLogPath)
{
	TCHAR szLogPath[MAX_PATH] = {0,};

	TCHAR szCurDirectory[MAX_PATH] = {0,};
	TCHAR szLogDirectory[MAX_PATH] = {0,};

	GetCurrentDirectory(MAX_PATH, szCurDirectory); 
	Snprintf(szLogDirectory, MAX_PATH-1, _T("%s\\log"), szCurDirectory);

	StrnCopy(pszLogPath, szLogDirectory, _tcslen(szLogDirectory));
}


//-------------------------------------------------------------------------------------------
/**
*/
#ifdef _DEV_VER
void GlobalFunc::WriteErrLog(eLOG_LEVEL eLevel, WORD wOutputStyle, TCHAR * pszMessage, ...)
{
	if((eLevel <= eLOG_LEVEL_NONE) || (eLevel > eLOG_LEVEL_TRA))
		return;

	if(GetLogLevel() >= eLevel)
	{
		TCHAR	szOutputMsg[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szErrLog[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szTempErrMsg[INTERFACE_STRING_LENGTH] = {0,};

		va_list args; 
		va_start(args, pszMessage); 
		_vsntprintf( szErrLog, INTERFACE_STRING_LENGTH-1, pszMessage, args ); 
		va_end(args); 


#if (WZENGINEVER >= 480)
        // 로그에 적용
        WZLOG( (WZLOG_TYPE)eLevel, szErrLog );
#endif

		static const char cloglevel[6] = {'N','E','W','I','D','T'};
		Snprintf(szTempErrMsg, INTERFACE_STRING_LENGTH-1, _T("[%c] %s"), cloglevel[eLevel], szErrLog);

		if(BIT_CHECK(wOutputStyle, c_wOUTPUT_LOG_SCREEN))
		{
			char sz[4] = {0,};
			Snprintf(sz, INTERFACE_STRING_LENGTH-1, _T("%c"), cloglevel[eLevel]);
			CustomAssertion::Instance()->PushMsg(sz,szErrLog);

		}

		if(BIT_CHECK(wOutputStyle, c_wOUTPUT_LOG_SYSCHAT))
		{
			OutputMsg_Error(eOUTPUT_MSG_CHAT, szTempErrMsg);
		}

		if(BIT_CHECK(wOutputStyle, c_wOUTPUT_LOG_CENTER))
		{
			OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, szTempErrMsg);
		}

		if(BIT_CHECK(wOutputStyle, c_wOUTPUT_LOG_CONSOLE))
		{
			OutputDebugString(szTempErrMsg);
			OutputDebugString("\n");
		}

		if(BIT_CHECK(wOutputStyle, c_wOUTPUT_LOG_FILE))
		{
			struct tm * today;
			time_t		ltime;
			time( &ltime );
			today = localtime( &ltime );

			Snprintf(szOutputMsg, INTERFACE_STRING_LENGTH-1, _T("[%02d:%02d:%02d] %s\n"), today->tm_hour, today->tm_min, today->tm_sec, szTempErrMsg);

			TCHAR szFileName[MAX_PATH] = {0,};
			Snprintf(szFileName, MAX_PATH-1,
				_T("log\\ClientErrMsg_%02d%02d%02d_%s.log"), 
				today->tm_year+1900, 
				today->tm_mon+1, 
				today->tm_mday, 
				c_tcBUILD_VERSION);

			FILE * fp = _tfopen(szFileName, _T("a+t"));
			fwrite( szOutputMsg, _tcslen(szOutputMsg), 1, fp );
			fclose( fp );
		}
		
	}

}
#endif

//-------------------------------------------------------------------------------------------
/**
*/
DWORD GlobalFunc::StartTimeCheck()
{
	if(g_bLoadingTimeSave)
		return clock_function::GetTickCount();
	else
		return 0;
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::EndTimeCheck(DWORD dwStartTime, bool bSave, TCHAR * pszMessage)
{
	if(g_bLoadingTimeSave)
	{
		DWORD dwEndTime=clock_function::GetTickCount();
		TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};
		
		DWORD msec = dwEndTime-dwStartTime;
		DWORD tempTime= msec / 1000;
		
		Snprintf(szText, INTERFACE_STRING_LENGTH-1, _T("%s [%d Second] (%d msec)"), pszMessage, tempTime, msec);
		WriteLoadingTimeLog(szText);

		if(bSave)
			g_dwTotalLoadingTime += msec;
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
DWORD	GlobalFunc::StartScriptLoading(TCHAR* pszMessage)
{
	if(g_bLoadingTimeSave)
	{
		TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
		Snprintf(szText, INTERFACE_STRING_LENGTH-1, _T("	%s Load Start"), pszMessage);
		WriteLoadingTimeLog(szText);

		return clock_function::GetTickCount();
	}
	else
	{
		return 0;
	}

}
//-------------------------------------------------------------------------------------------
/**
*/
void	GlobalFunc::EndScriptFileLoading(DWORD dwStartTime, TCHAR* pszMessage)
{
	if(g_bLoadingTimeSave)
	{
		DWORD dwEndTime = clock_function::GetTickCount();
		TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};
		
		DWORD msec = dwEndTime-dwStartTime;
		DWORD tempTime = msec / 1000;
		
		Snprintf(szText, INTERFACE_STRING_LENGTH-1, _T("	%s End [%d Second] (%d msec)"), pszMessage, tempTime, msec);
		WriteLoadingTimeLog(szText);
	}
}


//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::ClearTotalLoadingTime() 
{	
	g_dwTotalLoadingTime = 0; 
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::PrintTotalLoadingTime()
{
	if(g_bLoadingTimeSave)
	{
		WriteLoadingTimeLog(_T("Total: %d sec (%d msec)"), g_dwTotalLoadingTime/1000, g_dwTotalLoadingTime);
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::WriteLoadingTimeLog(TCHAR * pszMessage, ...)
{
	if(g_bLoadingTimeSave)
	{
		TCHAR	szOutputMsg[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szErrLog[INTERFACE_STRING_LENGTH] = {0,};

		va_list args; 
		va_start(args, pszMessage); 
		_vsntprintf( szErrLog, INTERFACE_STRING_LENGTH-1, pszMessage, args ); 
		va_end(args); 

		
		struct tm * today;
		time_t		ltime;
		time( &ltime );
		today = localtime( &ltime );

		Snprintf(szOutputMsg, INTERFACE_STRING_LENGTH-1, _T("[%02d:%02d:%02d] %s\n"), today->tm_hour, today->tm_min, today->tm_sec, szErrLog);

		TCHAR szFileName[MAX_PATH] = {0,};
		Snprintf(szFileName, MAX_PATH-1, _T("log\\LoadingTime.log"));

		FILE * fp = _tfopen(szFileName, _T("a+t"));
		fwrite( szOutputMsg, _tcslen(szOutputMsg), 1, fp );
		fclose( fp );
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::SetLoadingTimeSaveMode(bool bSave)
{
	g_bLoadingTimeSave = bSave;
}


//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::SetLogLevel(eLOG_LEVEL eLogLevel)
{
	g_eLogLevel = eLogLevel;
}

//-------------------------------------------------------------------------------------------
/**
*/
eLOG_LEVEL GlobalFunc::GetLogLevel()
{
	return g_eLogLevel;
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::ShowNetRecvLog(bool bShow)
{
	g_bNetRecvLog = bShow;
}

//-------------------------------------------------------------------------------------------
/**
*/
bool GlobalFunc::IsShowNetRecvLog()
{
	return g_bNetRecvLog;
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::ShowPacketLog(bool bShow)
{
    g_bShowPacket = bShow;
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::TogglePacketLog()
{
    g_bShowPacket = !g_bShowPacket;
}

//-------------------------------------------------------------------------------------------
/**
*/
bool GlobalFunc::IsShowPacketLog()
{
    return g_bShowPacket;
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::ShowNetSendLog(bool bShow)
{
	g_bNetSendLog = bShow;
}

//-------------------------------------------------------------------------------------------
/**
*/
bool GlobalFunc::IsShowNetSendLog()
{
	return g_bNetSendLog;
}

//-------------------------------------------------------------------------------------------
/**
*/
bool GlobalFunc::CheckIncludeStringOnAbroad(const char * pszMessage)
{
#if defined(_CHINA)
    if (PROGRAMPARAM->nation_type() == kLanguageTaiwan)       //!< 2:중국, 3:대만
    {
        return (g_slangNameFilter.CheckIncludeStringTaiwan(pszMessage));
    }
    else
    {
        return ( g_slangNameFilter.CheckIncludeStringChina(pszMessage) );
    }
#elif defined(_JAPAN)
    return (g_slangNameFilter.CheckIncludeStringJapan(pszMessage));    
#elif defined(_RUSSIA)
    return (g_slangNameFilter.CheckIncludeStringRussia(pszMessage));
#endif 
		return false;
}

BOOL GlobalFunc::IsDigit(TCHAR * pszMessage)
{
	int len = lstrlen(pszMessage);

	for (int i=0; i<len; ++i)
	{
		if (pszMessage[i]<_T('0') || pszMessage[i]>_T('9'))
		{
			return FALSE;
		}
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 리스트에 문자열을 사이즈에 맞춰계산 삽입
	리턴값0 실패 0보다크면 리스트열수리턴
*/

BYTE GlobalFunc::AddDescribeString( CCtrlListWZ * pControl, TCHAR * pszText, DWORD dwFontID, 
								    DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor )
{
	pControl->SetUnuseSelBar( FALSE );
	pControl->SetUseReport(FALSE);

	std::vector<util::_tstring> vstrMsg;
	vstrMsg.clear();
	
	if(!_tcslen(pszText))
		return 0;

	RECT rc = pControl->GetSizeRect();
    int ListWidth = rc.right - rc.left + 2;
	ProcessWordWrapByFontSize( &vstrMsg, pszText, dwFontID, _tcslen(pszText), ListWidth );

	pControl->DeleteItemAll();
	pControl->SetColorSelBar( WzColorToRGBA( WzColor_RGBA(255,255,255,0) ) );
	int iListIndex = -1;

	for( size_t i = 0; i<vstrMsg.size(); ++i )
	{
		iListIndex = pControl->InsertItemBack();

		TCHAR *pStr = const_cast<TCHAR *>( vstrMsg[i].c_str() );
		SetItem( pControl, iListIndex, 0, pStr,	dwFontID, dwTextStyle, BKColor, TextColor, SelTextColor );
	}   

	return vstrMsg.size();
}

//-------------------------------------------------------------------------------------------
/**
*/

BOOL GlobalFunc::SysMessageBox( LPCTSTR szMessage, BOOL shutdown )
{
	uiSystemMan * pSystemMan = GET_DIALOG_MANAGER( uiSystemMan, UIMAN_SYSTEM );

	if(pSystemMan)
	{
		pSystemMan->Sys_VerifyLock( szMessage, shutdown );
		return TRUE;
	}
	return FALSE;
}

//-------------------------------------------------------------------------------------------
/**
*/

void GlobalFunc::SysConfirmBox( LPCTSTR szMessage, void (*_fn_sysconfirm)(bool),
							    LPCTSTR pszTitle, LPCTSTR pszSubject, DWORD dwStrOKID, DWORD dwStrCancleID, bool bLock )
{

	uiSystemMan * pSystemMan = GET_DIALOG_MANAGER( uiSystemMan, UIMAN_SYSTEM );

	if( pSystemMan )
		pSystemMan->Sys_Confirm( szMessage, _fn_sysconfirm, pszTitle, pszSubject, dwStrOKID, dwStrCancleID, bLock );
}

//-------------------------------------------------------------------------------------------
/**
*/
void GlobalFunc::SysConfirmExtendBox( LPCTSTR szMessage, void (*_fn_sysconfirm)(bool),
									  LPCTSTR pszTitle, LPCTSTR pszSubject, DWORD dwStrOKID, DWORD dwStrCancleID, bool bLock )
{
	uiSystemMan * pSystemMan = GET_DIALOG_MANAGER( uiSystemMan, UIMAN_SYSTEM );

	if( pSystemMan )
		pSystemMan->Sys_ConfirmExtend( szMessage, _fn_sysconfirm, pszTitle, pszSubject, dwStrOKID, dwStrCancleID, bLock );
}

//-------------------------------------------------------------------------------------------
/**
*/

BOOL GlobalFunc::IsParty( DWORD dwKey )
{
	uiPartyMan * pPartyManager = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY );

	if( pPartyManager )
	{
		if( pPartyManager->IsPartyMember( dwKey ) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

//-------------------------------------------------------------------------------------------
/**
*/


BOOL GlobalFunc::GetModuleFileNameDirectory( LPTSTR lpBuffer, DWORD nBufferLength )
{
	if( GetModuleFileName( NULL, lpBuffer, nBufferLength ) == 0 )
		return FALSE;

	size_t len = _tcslen( lpBuffer );

	for( size_t i = len; i > 0; i-- )
	{
		if( lpBuffer[i] == '\\' )
			break;

		lpBuffer[i] = '\0';
	}
	
	return TRUE;
}

OSTYPE	GlobalFunc::GetOsVersion()
{
	OSVERSIONINFO OsInfo;
	OsInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx( &OsInfo );

	switch( OsInfo.dwMajorVersion )
	{
	case 5:
		{
			switch( OsInfo.dwMinorVersion )
			{
			case 0:
				return WINDOWS_2000;
			case 1:
				return WINDOWS_XP;
			case 2:
				return WINDOWS_SERVER_2003;
			}
		}
		break;
	case 6:
		return WINDOWS_VISTA;
	}

	return NO_INDENFIED_VERSION;
}

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
void GlobalFunc::SendServerSelectRequestPacket( MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_SYN::eQuery eQueryTransaction )
{
	// 새로운 플로우대로 패킷 전송
	// 이후 응답을 받고 처리
	MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_SYN sync;
	sync.m_bQueryType = eQueryTransaction;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
	
}
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__

void GlobalFunc::GoToServerList()
{
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
	assert(!"채널링 이후 GlobalFunc::GoToServerList() 는 호출되면 안됨 - 종료시킴");
	GlobalFunc::LogOutGame();
	return;

#else //_COMMON_CHANNELING_LOGIC_DEFINE
	#ifdef _JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_
	if (uiLoginMan::IsUseParameterAuthInfo() == true)
	{
		GlobalFunc::LogOutGame();
		return;
	}
	#endif //_JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_ 
	g_isChangeServer = true;
	g_CharSelectScene.m_bConnected = false;

    if (g_pNetwork)
    {
	    g_pNetwork->Disconnect(CI_GAMESERVERIDX);
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
	    g_pNetwork->Disconnect(CI_CHATSERVERIDX);
	    g_pNetwork->SetEnableSendChatHeartBeat(FALSE);
#endif
	    g_pNetwork->SetEnableSendHeartBeat(FALSE);
    }

	g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_LOGIN, 11001, 0);
	GameFramework::ChangeScene( SCENE_TYPE_LOAD );
#endif //_COMMON_CHANNELING_LOGIC_DEFINE
}

//-------------------------------------------------------------------------------------------
/**
*/
SCENE_TYPE GlobalFunc::ConvertClientZoneType(const eZONETYPE eCommonZoneType, const eINSTANCE_DUNGEON_KIND eIDK /*= INDUN_KIND_MAX*/)
{
	SCENE_TYPE eSceneType = SCENE_TYPE_NONE;
	switch( eCommonZoneType)
	{
	case eZONETYPE_VILLAGE:
		eSceneType = SCENE_TYPE_VILLAGE;
		break;
	case eZONETYPE_MISSION:
		eSceneType = SCENE_TYPE_MISSION;
		break;
	case eZONETYPE_CHUNTING:
		eSceneType = SCENE_TYPE_CHUNTING;
		break;
	case eZONETYPE_INSTANCE:
        {
            if (eIDK == INDUN_KIND_CHAOS_ZONE)
            {
                eSceneType;
            }
            else
            {
                eSceneType = SCENE_TYPE_INSTANCE_DUNGEON;
            }
        }
		break;
	case eZONETYPE_PVP:
		eSceneType = SCENE_TYPE_PVP;
		break;
    case eZONETYPE_AIRSHIP_FIELD:
	case eZONETYPE_FIELD:
		eSceneType = SCENE_TYPE_FIELD;
		break;
	case eZONETYPE_DOMINATION_FIELD:
		eSceneType = SCENE_TYPE_DOMINATION;
        break;
	}
	return eSceneType;
}

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
SCENE_TYPE GlobalFunc::ConvertClientZoneType(const eZONETYPE eCommonZoneType, const eINSTANCE_DUNGEON_KIND eIDK, MAPCODE map_code)
{
    if ((eCommonZoneType == eZONETYPE_INSTANCE) && (eIDK == INDUN_KIND_CHAOS_ZONE))
    {
        // 카오스존 일 때
        ChaosZoneType chaos_zone_type = ChaosZoneInfoParser::Instance()->GetType(map_code);
        if (chaos_zone_type == kBattleGround)
        {
            return SCENE_TYPE_BATTLEGROUND;
        }
        else if (chaos_zone_type == kFreePVP)
        {
            return SCENE_TYPE_FREEPVP;
        }
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        else if (chaos_zone_type == eCHAOSZONE_TYPE_GOLDRUSH)
        {
            return SCENE_TYPE_GOLDRUSH;
        }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
        else if (chaos_zone_type == eCHAOSZONE_TYPE_SONNENSCHEIN)
        {
            return SCENE_TYPE_SONNENSCHEIN;
        }
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        else if (chaos_zone_type == eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION)
        {
            return SCENE_TYPE_GUILDMISSION;
        }
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
    }

    return ConvertClientZoneType(eCommonZoneType, eIDK);
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
//-------------------------------------------------------------------------------------------
/**
*/
void	GlobalFunc::InitGameInfo()
{
	uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

	if (helpMan)
	{
		helpMan->InitHelpMenuList();
	}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    GuildSystem::GetGuildSystemData().Clear();
#else
    uiGuildMan* guildMan = 
        static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

    if(guildMan)
    {
        guildMan->Clear();
        guildMan->ClearNotice();
    }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

	uiWayPointMan* waypoint = GET_DIALOG_MANAGER(uiWayPointMan, UIMAN_WAYPOINT);

	if(waypoint)
	{
		waypoint->Clear();
	}

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    uiMissionRankMan2* rank_manager_ptr2 =  GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );

    if ( rank_manager_ptr2 )
    {
        rank_manager_ptr2->ClearRankData();
        rank_manager_ptr2->InitMission();
    }
#else
	uiMissionRankMan* pRankMan =  GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );

	if(pRankMan)
	{
		pRankMan->InitMissionGroup();
		pRankMan->ClearRankData();
		pRankMan->InitMission();
	}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
}

TCHAR *  GlobalFunc::GetCorpsName(int iCorps)
{
	static TCHAR szCorpsName[256];
	ZeroMemory(szCorpsName, sizeof(szCorpsName));
	if( iCorps == 0 )
	{
		//1268	직할
		g_InterfaceManager.GetInterfaceString(1268, szCorpsName,255);
	}
	else
	{
		_itoa(iCorps, szCorpsName, 10);
	}
	return szCorpsName;
}


void GlobalFunc::NET_SEND_CG_ITEM_USE_SKILL_ACTION_SYN(eITEMTYPE ItemType, WzVector wvTargetPos)
{
		MSG_CG_ITEM_USE_SKILL_ACTION_SYN SendMsg;
		SendMsg.m_wvMainTargetPos = wvTargetPos;
		SendMsg.m_atIndex = SI_INVENTORY;
		SendMsg.m_atPos = MouseHandler::Instance()->GetPendingSkillItemPos();
        SendMsg.byAttackPropensity = MouseHandler::Instance()->GetPendingCtrlKeyDown();
		SendMsg.target_code_ = 0; // 아이템 사용 대상의 코드
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
}


//------------------------------------------------------------------------------ 
void GlobalFunc::NET_SEND_CG_ITEM_USE_SKILL_ACTION_SYN(eSlotContainerIndex container_index, 
                                                       POSTYPE at_pos, 
                                                       DWORD target_objectkey,
                                                       WzVector& target_pos)
{

    MSG_CG_ITEM_USE_SKILL_ACTION_SYN msg;
    msg.m_atIndex = container_index;
    msg.m_atPos = at_pos;
    msg.target_code_ = target_objectkey;
    msg.m_wvMainTargetPos = target_pos;

    msg.byAttackPropensity = false;

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &msg, sizeof(msg));

}

//----------------------------------------------------------------------------
/** 착용제한 수치 랩핑
*/
WORD GlobalFunc::CalcLimitStat(WORD ItemLimitStr, SCItemSlot* ItemSlot, LimitStatCompositeType limitstat_composite_type, BOOL bIgnoreLimit)
{
    if (ItemSlot == NULL)
    {
        return 0;
    }

	WORD (*PCalcLimitStat)(WORD, BYTE, eRANK, LEVELTYPE, float);

	//==========================================
	// 무기타입적용(웨폰 or 아머)
	//==========================================
	if (ItemSlot->GetItemInfo()->IsWeapon())		
	{
		PCalcLimitStat = CalcLimitStatWeapon;
	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if (ItemSlot->GetItemInfo()->IsArmor() || ItemSlot->IsWingItem())
#else
	else if (ItemSlot->GetItemInfo()->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		PCalcLimitStat = CalcLimitStatArmor;
	}
	else
    {
        assert(!"뭥미 -_- 여긴오면안됨");
    }

	//==========================================
	// 리미티드 계산부부분(반올림적용)
	//==========================================
	if (ItemSlot->IsLimited() && (bIgnoreLimit == FALSE))
	{		
		int nRatio = ItemSlot->GetLimitValueRatio(); 
		ItemLimitStr = (ItemLimitStr + (WORD)((float)ItemLimitStr * nRatio / 100.0f + MODFLOAT(nRatio)));
	}


	//==========================================
	// 조합타입(0:없음 1:랭크업  2:인첸트)
	// 조합창에서 다음랭크를 보기위해쓰임
	//==========================================
	int   iRank		= ItemSlot->GetRank();
	BYTE  byEnchant = ItemSlot->GetEnchant();
	LEVELTYPE lv = ItemSlot->GetItemInfo()->m_LV;

	switch (limitstat_composite_type)
	{
	    case kLimitStatCompositeRankup: 
            ++iRank; 
            break;

	    case kLimitStatCompositeEnchant: 
            ++byEnchant; 
            break;
	}

	//==========================================
	// 인첸트 리뉴얼적용부분
	//==========================================
#ifdef __NA_001358_ENCHANT_UNIFICATION
	float	fLimitRatio = InstantEnchantParser::Instance()->GetLimitRatio(byEnchant);
#else
	float	fLimitRatio = EnchantParser::Instance()->GetLimitRatio(lv, 
                                                                   byEnchant, 
                                                                   ItemSlot->GetItemInfo()->IsWeapon(), 
                                                                   ItemSlot->IsLimited() );
#endif //__NA_001358_ENCHANT_UNIFICATION
	
	return (WORD)(PCalcLimitStat(ItemLimitStr, byEnchant, (eRANK)iRank, lv, fLimitRatio));
}

void GlobalFunc::InsertItemToInvenOfHero( const ITEMSLOTEX * pSlot, int count )
{
	if( g_pHero == NULL )
		return;

	const ITEMSLOTEX * pTempSlot = pSlot;
	SCItemSlot	Item;

	for( int i = 0; i < count; i++ )
	{
		Item.Clear();
		Item.Copy( pTempSlot[i].m_Stream );

		g_pHero->GetSlotContainer( SI_INVENTORY )->InsertSlot( pTempSlot[i].m_Pos, Item );
	}
}

const std::string GlobalFunc::CalculateThousandDigit( MONEY money )
{	
	TCHAR szMoney[MAX_PATH];

	_i64tot( money, szMoney, 10 );

	std::string	strTemp( szMoney );

	size_t len = strTemp.length();

	if( len > 3 )
	{
		int count = 1;
		for( int i = len - 1; i > 0; i-- )
		{
			if( count % 3 == 0 )
			{
				strTemp.insert( i, "," ); 
			}

			count++;
		}
	}
	
	return strTemp;
}

BOOL  GlobalFunc::IsSSQScene()
{
	if( g_LoadScene.GetNextSceneAfterLoaded() == SCENE_TYPE_SSQ || 
		( GameFramework::GetCurrentScene() && GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SSQ ) )
		return TRUE;

	return FALSE;
}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
BOOL GlobalFunc::IsBattleGroundScene()
{
    if( g_LoadScene.GetNextSceneAfterLoaded() == SCENE_TYPE_BATTLEGROUND || 
        ( GameFramework::GetCurrentScene() && GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_BATTLEGROUND ) )
        return TRUE;

    return FALSE;
}
void GlobalFunc::BattleGroundBackToTheVillageConfirm(bool confirm_value)
{
    if (confirm_value)
    {
        BattleScene::BackToTheVillageTransact();
    }
    else
    {
        //nop
    } 
}
void GlobalFunc::BattleGroundCharacterSelectConfirm(bool confirm_value)
{
    if (confirm_value)
    {
        SystemDialog* system_dialog = GET_CAST_DIALOG(SystemDialog, InterfaceManager::DIALOG_SYSTEM);
        if (system_dialog)
        {
            system_dialog->CharacterSelectStandingBy();
        }
    }
    else
    {
        //nop
    } 
}
void GlobalFunc::BattleGroundServerSelectConfirm(bool confirm_value)
{
    if (confirm_value)
    {
        SystemDialog* system_dialog = GET_CAST_DIALOG(SystemDialog, InterfaceManager::DIALOG_SYSTEM);
        if (system_dialog)
        {
            system_dialog->ServerSelectStandingBy();
        }
    }
    else
    {
        //nop
    } 
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
BOOL GlobalFunc::IsGoldRushScene()
{
    if( g_LoadScene.GetNextSceneAfterLoaded() == SCENE_TYPE_GOLDRUSH || 
        ( GameFramework::GetCurrentScene() && GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_GOLDRUSH ) )
        return TRUE;

    return FALSE;
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
BOOL GlobalFunc::IsDominationScene()
{
    if( g_LoadScene.GetNextSceneAfterLoaded() == SCENE_TYPE_DOMINATION || 
        ( GameFramework::GetCurrentScene() && GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_DOMINATION ) )
        return TRUE;

    return FALSE;
}

BOOL GlobalFunc::IsMondschienMap()
{
    if( g_LoadScene.GetNextSceneAfterLoaded() == SCENE_TYPE_DOMINATION || 
        ( GameFramework::GetCurrentScene() && GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_DOMINATION ) )
    {
        if (!g_pMap)
            return FALSE;

        const sDOMINATION_INFO* domination_info_ptr = 
            DominationInfoParser::Instance()->GetDominationInfo(g_pMap->GetCurrentFieldID());
        if (domination_info_ptr && domination_info_ptr->m_DominationType == DOMINATION_TYPE_MONDSCHIEN)
        {
            return TRUE;
        }
    }

    return FALSE;
}
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
BOOL GlobalFunc::IsFreePVPScene()
{
    if ((g_LoadScene.GetNextSceneAfterLoaded() == SCENE_TYPE_FREEPVP) || 
        (GameFramework::GetCurrentScene() && GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_FREEPVP) )
    {
        return TRUE;
    }

    return FALSE;
}
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
BOOL GlobalFunc::IsSpaScene()
{
    if( g_LoadScene.GetNextSceneAfterLoaded() == SCENE_TYPE_SPA || 
        ( GameFramework::GetCurrentScene() && GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SPA ) )
        return TRUE;

    return FALSE;
}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008334_20150608_SONNENSCHEIN
BOOL GlobalFunc::IsSonnenScheinScene()
{
    if( g_LoadScene.GetNextSceneAfterLoaded() == SCENE_TYPE_SONNENSCHEIN || 
        ( GameFramework::GetCurrentScene() && GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SONNENSCHEIN ) )
        return TRUE;

    return FALSE;
}
BOOL GlobalFunc::HideSonnenScheinnBuffScene()
{
    // 소넨샤인 버프 가리는 곳 (서버에선 막았는데 클라에서도 막도록)
    if ((GlobalFunc::IsSSQScene() == TRUE)
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        || (GlobalFunc::IsBattleGroundScene() == TRUE)
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
        || (GlobalFunc::IsFreePVPScene() == TRUE)
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        || (GlobalFunc::IsGoldRushScene() == TRUE)
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        )
    {
        return TRUE;
    }

    // 점령상태가 온일때
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    if (GlobalFunc::IsDominationScene())
    {
        uiDominationTimer* domination_timer = GET_CAST_DIALOG(uiDominationTimer, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TIMER_DIALOG);
        if (domination_timer)
        {
            if ((domination_timer->event_state() == DOMINATION_EVENT_WAIT) ||
                (domination_timer->event_state() == DOMINATION_EVENT_START))
            {
                return TRUE;
            }
        }
    }
#endif //_NA_007115_20140313_MONDSCHIEN_RENEWAL

    if (GlobalFunc::IsSonnenScheinScene())
    {
        uiDominationTimer* domination_timer = GET_CAST_DIALOG(uiDominationTimer, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TIMER_DIALOG);
        if (domination_timer)
        {
            if ((domination_timer->event_state() == SONNENSCHEIN_EVENT_STATE_WAIT) ||
                (domination_timer->event_state() == SONNENSCHEIN_EVENT_STATE_START))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}
#endif //_NA_008334_20150608_SONNENSCHEIN

WzVector GlobalFunc::GetObserverPos( int index )
{
	WzVector vPos[4];

	SetVector( &vPos[0], 200.00f, 177.42f, 8.77f );
	SetVector( &vPos[1], 222.10f, 199.89f, 8.77f );
	SetVector( &vPos[2], 200.00f, 223.44f, 8.77f );
	SetVector( &vPos[3], 176.96f, 199.87f, 8.77f );

	return vPos[index];
}


const WzVector GlobalFunc::GetObserverAngle( int index )
{
	WzVector vTarget;
	SetVector( &vTarget, 200.58f, 200.13f, 9.03f );
	const WzVector * vPos = nsSSQ::Observer::Controller::GetPosByIndex( (Info::eCoordinates)index );
	
	const WzVector vTemp = vTarget - *vPos;

	WzVector vResult;
	vResult.x = Math_GetAngleFromVector( &vTemp );
	vResult.y = 0.30f;
	vResult.z = 0.0f;

	return vResult;
}

void GlobalFunc::fnSendSelectObserverMode( bool b )
{
	BYTE RequestType = Info::eRequest_None;

	if( b )
	{
		RequestType = Info::eRequest_Observer;
	}
	else
	{
		RequestType = Info::eRequest_Leave;
	}

	SendObserverPacket( RequestType );
}

BOOL GlobalFunc::SendObserverCtrl( BYTE KeyCmd )
{
	MSG_CG_ZONE_EX_SSQ_OBSERVER_SYN Msg;
	Msg.Request					= (BYTE)Info::eRequest_ChangeCoord;
	Msg.ObserverCmd.PressedKey	= KeyCmd;

	return SendPacket( CI_GAMESERVERIDX, &Msg, sizeof( Msg ) );
}

BOOL GlobalFunc::SendObserverPacket( BYTE RequestType )
{
	MSG_CG_ZONE_EX_SSQ_OBSERVER_SYN Msg;
	Msg.Request				= RequestType;

	return SendPacketToGameServer( &Msg, sizeof( Msg ) );
}

BOOL GlobalFunc::IsInventoryLocknMessage()
{
	uiInventoryMan* pInvenMan = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY );
	if (pInvenMan)
	{
		int LockState = pInvenMan->GetInventoryLockState();
		if ( LockState & PLAYERINFO_FOR_PACKET::DISABLED )
		{
			uiSystemMan* pSysMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_INVENTORY );

			TCHAR szMessage[INTERFACE_STRING_LENGTH];
			g_InterfaceManager.GetInterfaceString(5572, szMessage, INTERFACE_STRING_LENGTH);

			pSysMan->Sys_Verify(szMessage);

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}


void GlobalFunc::UsePossessionItem(bool bUse)
{
	if ( bUse )
	{
		MSG_CG_ITEM_IDENTIFY_POSSESSION_SYN Msg;
		POSTYPE ToPos;
		MouseHandler::Instance()->GetPossessionItemPOS(Msg.m_ItemPos, ToPos);
		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &Msg, sizeof(Msg) );
	}
	MouseHandler::Instance()->SetPossessionWin(false);
}

#ifdef __NA_001358_ENCHANT_UNIFICATION
void GlobalFunc::UseInstantEnchantGem(bool bEnchant)
{
	uiEnchantMan* pUIMan = GET_DIALOG_MANAGER(uiEnchantMan, UIMAN_ENCHANT);
	if(pUIMan)
	{	
		pUIMan->UnsetPendingAsk();
		if ( bEnchant )
		{
			pUIMan->SendPacketInstantEnchant();
		}
	}	
}
#endif //__NA_001358_ENCHANT_UNIFICATION

void GlobalFunc::ApplayOptionShowPlayers()
{
	if (NULL == g_pHero)
	{
		return;
	}
	//Object* pObject = g_pHero->GetTargetObject();
	Object* pObject = g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());
	if (pObject && pObject->IsKindOfObject(PLAYER_OBJECT))
	{
		Player* pPlayer = (Player*)pObject;
		if ( false == pPlayer->IsShowByGameOption() )
		{
			g_HeroInput.SetCurrentTarget(NULL);
		}
	}
}

#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
void GlobalFunc::RetryOneTimePassword()
{
    uiLoginOneTimePassword* pUI = GET_CAST_DIALOG(uiLoginOneTimePassword, IM_LOGIN_MANAGER::LOGIN_ONETIMEPASSWORD);
    if (pUI)
    {
        if (uiLoginOneTimePassword::UseNRetryOneTimePassword == pUI->GetOTPUserStamp())
        {
            pUI->ShowInterface(TRUE);
        }
        else if (uiLoginOneTimePassword::UseNExpiredOneTimePassword != pUI->GetOTPUserStamp())
        {   
            uiLoginMan* pUIMan = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);
            if (pUIMan)
            {
                pUIMan->resetTryReconnect();
            }
        }
    }
}

void GlobalFunc::EncourageOTP(bool bYes)
{
    uiLoginMan* pUIMan = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);
    SUN_ASSERT(pUIMan);

    if (bYes)
    {
        // OTP 웹페이지 열기
        TCHAR szREGISTER_URL[512];
        // 마이폴더 홈페이지에 접속하렴
        SHELLEXECUTEINFO ShExecInfo;

        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        ShExecInfo.fMask = NULL;
        ShExecInfo.hwnd = g_hWndMain;
        ShExecInfo.lpVerb = NULL;
        g_InterfaceManager.GetInterfaceString(5638, szREGISTER_URL, 512);
        ShExecInfo.lpFile = szREGISTER_URL;
        ShExecInfo.lpParameters = NULL;
        ShExecInfo.lpDirectory = NULL;
        ShExecInfo.nShow = SW_MAXIMIZE;
        ShExecInfo.hInstApp = NULL;

        ShellExecuteEx(&ShExecInfo);

        if (pUIMan)
        {
            pUIMan->resetTryReconnect();
        }
    }
    else
    {
        // 서버선택목록 요청하기 - 혹은 서버목록을 받아둔상태에서 보여주는 것으로 변경이 필요할지도...
        if (pUIMan)
        {
            pUIMan->SEND_AUTH_SERVER_LIST_SYN();
        }    
    }    
}
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_

#if defined(__KR) || defined(_KOREA)
#if defined(_DEV_VER)

void GlobalFunc::LoadDevIniFile()
{
	char lpszFilePath[MAX_PATH];
 	GetCurrentDirectoryA( MAX_PATH, lpszFilePath);
	strcat( lpszFilePath, "\\");
	strcat( lpszFilePath, "dev.ini");

	GetPrivateProfileStringA( "Command", "option", 0, g_szDevOption, 1023, lpszFilePath);
}


char* GlobalFunc::GetDevOption()
{
	return g_szDevOption;
}

#endif	//#if defined(_DEV_VER)
#endif	//#if defined(__KR) || defined(_KOREA)


void GlobalFunc::UseRandomItem(bool bUse)
{
	if ( bUse )
	{
		MSG_CG_ITEM_USE_RANDOM_ITEM_SYN Msg;
		MouseHandler::Instance()->GetRandomItemPOS(Msg.m_TargetPos);
		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &Msg, sizeof(Msg) );


	}
	MouseHandler::Instance()->SetRandomWin(false);
}

void GlobalFunc::GoDefaultVillage()
{
    if (g_pMap == NULL)
    {
        return;
    }

    MSG_CG_GM_STRING_CMD_SYN msg;
    ZeroMemory(msg.m_szStringCmd,MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
    TCHAR cmd_message[MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH] = {0,};

    //무조건 맵 이동은 하게 된다.
    if (g_pMap->GetMapID() != 10002)
    {
        Sprintf(cmd_message, "go 10002");
    }
    else
    {
        Sprintf(cmd_message, "go 10001");
    }

    StrnCopy(msg.m_szStringCmd, cmd_message, MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
    GlobalFunc::SendPacket(CI_GAMESERVERIDX,&msg,sizeof(msg));
}

void GlobalFunc::GetMonGradeDesc(eNPC_GRADE npc_grade, TCHAR* desc_str)
{
    switch(npc_grade)
    {
    case eNPC_GENERAL:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("일반"));
        break;
    case eNPC_ELITE:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("정예"));
        break;
    case eNPC_LEADER:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("리더"));
        break;
    case eNPC_MIDDLEBOSS:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("캡틴"));
        break;
    case eNPC_BOSS:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("보스"));
        break;
    case eNPC_MERCHANT_NPC:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("NPC(상인)"));
        break;
    case eNPC_SUMMON_NPC:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("NPC(소환체)"));
        break;
    case eNPC_MAPOBJECT_NPC:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("NPC(맵오브젝트)"));
        break;
    case eNPC_LUCKY_MONSTER:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("럭키몬스터"));
        break;
    case eNPC_TOTEM:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("토템"));
        break;
    case eNPC_SSQ_MONSTER:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("SSQ 몬스터"));
        break;
    case eNPC_CRYSTAL_WARP:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("워프수정"));
        break;
    case eNPC_DOMINATION_MAPOBJECT_NPC:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("지역점령전_맵오브젝트"));
        break;
    case eNPC_FRIEND_MAPOBJECT_NPC:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("일반 몬스터와는 적대적, Player와 우호 적인 맵 오브젝트"));
        break;
    case eNPC_FRIEND_MONSTER:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("일반 몬스터와는 적대적, Player와 우호형 몬스터"));
        break;
    case eNPC_MOVE_SYNC_MERCHANT:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("장거리 이동 NPC등을 위한 타입"));
        break;
    default:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("????"));
        break;

    }
}

void GlobalFunc::GetAICodeDesc(WORD ai_code, TCHAR* desc_str)
{
    switch(ai_code)
    {
    case 1:
    case 2:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("일반"));
        break;
    case 3:
    case 4:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("럭키"));
        break;
    case 5:
    case 6:
    case 7:
    case 8:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("SSQ 몬스터"));
        break;
    default:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("????"));
        break;

    }
}

void GlobalFunc::GetAttackAttitudeDesc(eATTACK_ATTITUDE att_attitude, TCHAR* desc_str)
{
    switch(att_attitude)
    {
    case ATTACK_ATTITUDE_AFTER:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("비선공"));
        break;
    case ATTACK_ATTITUDE_NEARES_FIRST:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("선공(거리)"));
        break;
    case ATTACK_ATTITUDE_LOW_HP_FIRST:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("선공(체력)"));
        break;
    case ATTACK_ATTITUDE_LOW_LEVEL_FIRST:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("선공(레벌)"));
        break;
    case ATTACK_ATTITUDE_HIGH_MP_FIRST:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("선공(마나)"));
        break;
    case ATTACK_ATTITUDE_ONE_TARGET:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("단순무식"));
        break;
    case ATTACK_ATTITUDE_NO_ATTACK:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("공격안함"));
        break;
    default:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("????"));
        break;
    }
}

void GlobalFunc::GetAttTypeDesc(eATTACK_TYPE att_type, TCHAR* desc_str)
{
    switch(att_type)
    {
    case eATTACK_TYPE_ALL_OPTION:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("모든 옵션"));
        break;
    case eATTACK_TYPE_MELEE:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("근접"));
        break;
    case eATTACK_TYPE_RANGE:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("원거리"));
        break;
    case eATTACK_TYPE_WATER:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("물"));
        break;
    case eATTACK_TYPE_FIRE:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("불"));
        break;
    case eATTACK_TYPE_WIND:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("바람"));
        break;
    case eATTACK_TYPE_EARTH:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("대지"));
        break;
    case eATTACK_TYPE_DARKNESS:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("암흑"));
        break;
    case eATTACK_TYPE_DIVINE:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("신성"));
        break;
    case eATTACK_TYPE_PHYSICAL_OPTION:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("물리 옵션"));
        break;
    case eATTACK_TYPE_MAGIC_OPTION:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("마법 옵션"));
        break;
    case eATTACK_TYPE_ALL_MAGIC:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("모든 마법속성"));
        break;
    case eATTACK_TYPE_MAGIC:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("마법"));
        break;
    default:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("????"));
        break;
    }
}

void GlobalFunc::GetMoveAttitudeDesc(eNPC_MOVE_ATTITUDE move_attitude, TCHAR* desc_str)
{
    switch(move_attitude)
    {
    case eMOVE_ATTITUDE_WANDER:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("이동형"));
        break;
    case eMOVE_ATTITUDE_ETERNAL_STOP:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("완전 정지형"));
        break;
    case eMOVE_ATTITUDE_SPAWN_STOP:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("스폰 정지형(적이 나타나기 전까지만 정지)"));
        break;
    case eMOVE_ATTITUDE_PATROL:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("패트롤"));
        break;
    case eMOVE_ATTITUDE_SEARCH_AREA:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("특정 영역을 우선적으로 검색"));
        break;
    case eMOVE_ATTITUDE_PATHLIST:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("경로 이동형 (설정된 경로 1회 이동)"));
        break;
    case eMOVE_ATTITUDE_FOLLOW:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("주위 아군 따라가기"));
        break;
    case eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("완전 정지(공격 안함)"));
        break;
    default:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("????"));
        break;
    }
}

void GlobalFunc::GetMeleeTypeDesc(eMELEE_TYPE melee_type, TCHAR* desc_str)
{
    switch(melee_type)
    {
    case eMELEE_TYPE_NOT:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("타입없음(공격없음)"));
        break;
    case eMELEE_TYPE_SLASH:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("베기"));
        break;
    case eMELEE_TYPE_PIERCE:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("찌르기"));
        break;
    case eMELEE_TYPE_HIT:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("타격"));
        break;
    case eMELEE_TYPE_MAGIC:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("마법(오브, 지팡이)"));
        break;
    default:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("????"));
        break;
    }
}

void GlobalFunc::GetArmorTypeDesc(eARMOR_TYPE armor_type, TCHAR* desc_str)
{
    switch(armor_type)
    {
    case eARMOR_HARD:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("HEAVY"));
        break;
    case eARMOR_MEDIUM:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("MEDIUM"));
        break;
    case eARMOR_SOFT:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("LIGHT"));
        break;
    case eARMOR_SIEGE:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("SIEGE"));
        break;
    case eARMOR_UNARMOR:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("UNARMOR"));
        break;
    default:
        Snprintf(desc_str, INTERFACE_STRING_LENGTH, _T("????"));
        break;
    }
}

DWORD GlobalFunc::GetClassNameCode(BYTE class_type)
{
    DWORD class_name_code = eST_CHARACTER_CLASS_NAME_BERSERKER;
    switch (class_type)
    {
    case eCHAR_BERSERKER:
        {
            class_name_code = eST_CHARACTER_CLASS_NAME_BERSERKER;
        }
        break;
    case eCHAR_DRAGON:
        {
            class_name_code = eST_CHARACTER_CLASS_NAME_DRAGON;
        }
        break;
    case eCHAR_SHADOW:
        {
            class_name_code = eST_CHARACTER_CLASS_NAME_SHADOW;
        }
        break;
    case eCHAR_VALKYRIE:
        {
            class_name_code = eST_CHARACTER_CLASS_NAME_VALKYRIE;
        }
        break;
    case eCHAR_ELEMENTALIST:
        {
            class_name_code = eST_CHARACTER_CLASS_NAME_MAGICIAN;
        }
        break;
    case eCHAR_MYSTIC: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        {
            class_name_code = eST_CHARACTER_CLASS_NAME_MYSTIC;
        }
        break;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        {
            class_name_code = eST_CHARACTER_CLASS_NAME_HELLROID;
        }
        break;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        {
            class_name_code = eST_CHARACTER_CLASS_NAME_WITCHBLADE;
        }
        break;
    }

    return class_name_code;
}

bool GlobalFunc::IsMyItem(Item* item_pointer)
{
    //내 것의 아이템인지 ([*]표시를 보일지)
    if (uiAreaConquestMan::CanPickACItem(item_pointer,false) == false)    //소유가 있더라도 완료한 AC템이면 제외 (1순위검사)
    {
        return FALSE;
    }
    if ((item_pointer->GetOwner() == 0) ||
        (g_pHero && g_pHero->GetObjectKey() == item_pointer->GetOwner()))
    {
        return TRUE;
    }
    
    uiPartyMan* partyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (partyMan && partyMan->IsPartyMember(item_pointer->GetOwner()))
    {
        const BASE_ITEMINFO* Item_Info = item_pointer->GetItemSlot().GetItemInfo();
        if ((Item_Info != NULL) &&
            ((Item_Info->m_wType == eITEMTYPE_AC) || 
            (Item_Info->m_wType == eITEMTYPE_QUEST) ||
            (Item_Info->m_wType == eITEMTYPE_EVENT_FIELD_PARTY_ALL_MEMBER_PICK_UP)))
        {
            return TRUE;
        }

        ItemShareSystem* item_share = GET_CAST_DIALOG(ItemShareSystem,IM_PARTY_MANAGER::PARTY_ITEM_SHARE_SYSTEM);
        if ((item_share != NULL) && 
            (item_share->GetItemShareType() == eITEM_DISTRIBUTION_DEFAULT)
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
            || (item_share->GetItemShareType() == eITEM_DISTRIBUTION_DICE_AND_DEFAULT)
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
            )
        {
            return TRUE;
        }
    }
    return FALSE;
}

bool GlobalFunc::CanGetItem(Item* item_pointer)
{
    //내가 줍을 수 있는 아이템인지
    if ((item_pointer->GetOwner() == 0) ||
        (g_pHero && g_pHero->GetObjectKey() == item_pointer->GetOwner()))
    {
        return TRUE;
    }
    //줍기에 대해서 파티원 내의 소유권이면 모두 먹을 수 있게 함
    uiPartyMan* partyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (partyMan && partyMan->IsPartyMember(item_pointer->GetOwner()))
    {
        return TRUE;
    }
    return FALSE;
}
bool GlobalFunc::CanGetHaim(Item* item_pointer)
{
    if (item_pointer->GetOwner() == 0 || 
        (g_pHero && g_pHero->GetObjectKey() == item_pointer->GetOwner()))
    {
        return TRUE;
    }
    
    uiPartyMan* partyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (partyMan && partyMan->IsPartyMember(item_pointer->GetOwner()))
    {
        return TRUE;
    }

    return FALSE;
}

bool GlobalFunc::IsNumberic(BYTE option_kind, BASE_OptionList* option_list_info)
{
    //__UNUSED(option_list_info);
    if (option_kind == VALUE_TYPE_VALUE)
    {
        return true;
    }
    return false;
}

bool GlobalFunc::IsPercentage(BYTE option_kind, BASE_OptionList* option_list_info)
{
    //__UNUSED(option_list_info);
    if (option_kind != VALUE_TYPE_VALUE)
    {
        return true;
    }
    return false;
}

void GlobalFunc::AddOptionSuffix(BYTE option_kind, BASE_OptionList* option_list_info, TCHAR* INOUT string_desc)
{
    if ((option_list_info == NULL) || (string_desc == NULL))
    {
        assert(option_list_info && "잘못된 OptionList 정보 값이 들어왔습니다.");
        return;
    }
    TCHAR string_suffix[INTERFACE_STRING_LENGTH + 1] = {0,};

    switch(option_list_info->valuetype)
    {
    case OptionListParser::eOPTION_VALUE_TYPE_NORMAL:
        {
            if (option_kind == VALUE_TYPE_VALUE)
            {
                return;
            }
            else
            {
                //11299	%
                g_InterfaceManager.GetInterfaceString(11299, string_suffix, INTERFACE_STRING_LENGTH);
            }
        }
        break;
    case OptionListParser::eOPTION_VALUE_TYPE_PERCENTAGE:
        {
            //11299	%
            g_InterfaceManager.GetInterfaceString(11299, string_suffix, INTERFACE_STRING_LENGTH);
        }
        break;
    case OptionListParser::eOPTION_VALUE_TYPE_SECOND:
        {
            //11300	초
            g_InterfaceManager.GetInterfaceString(11300, string_suffix, INTERFACE_STRING_LENGTH);
        }
        break;
    }
    
    _tcscat(string_desc,string_suffix);

    return;
}


//------------------------------------------------------------------------------
void GlobalFunc::Write_Packet_Log(CHAR* message, ...)
{
#ifdef _DEV_VER

    char szErrLog[1024] = {0,};

    va_list args; 
    va_start(args, message); 
    _vsntprintf( szErrLog, 1023, message, args ); 
    va_end(args); 

    char szOutputMsg[1024] = {0,};
    _snprintf(szOutputMsg, 1023, _T("%s\n"), szErrLog);


#ifdef _DEBUG

    SYSTEMTIME time;
    GetSystemTime(&time);
    char str[256] = {NULL, };
    sprintf(str, "[%02d:%02d:%02d] ", time.wHour, time.wMinute, time.wSecond);
    OutputDebugString(str);

    OutputDebugString(szOutputMsg);
#else
    char szFileName[MAX_PATH] = {0,};
    _snprintf(szFileName, MAX_PATH-1, _T("log\\packet.log"));

    FILE* fp = _tfopen(szFileName, _T("a+t"));
    if (fp)
    {
        fwrite( szOutputMsg, _tcslen(szOutputMsg), 1, fp );
        fclose( fp );
    }
#endif
#endif //_DEV_VER
}
//------------------------------------------------------------------------------ 
void GlobalFunc::OutputMsg_WingControl_ResultCode(uint8_t result, uint16_t result_code)
{
    WingControl::eResult res = static_cast<WingControl::eResult>(result);
    WingControl::eResultCode res_code = static_cast<WingControl::eResultCode>(result_code);

    if (res == WingControl::eRet_Success)
    {
    }
    else if (res == WingControl::eRet_Failed)
    {
        DWORD string_code = 0;
        switch (res_code)
        {
        case WingControl::eRetCode_Unknown:
        case WingControl::eRetCode_NotWingsItem:
            break;
        case WingControl::eRetCode_InvalidCharState:
            string_code = 5249;
            break;
        case WingControl::eRetCode_TimeExpired:
        case WingControl::eRetCode_AlreadySpread:
        case WingControl::eRetCode_AlreadyFurled:
        case WingControl::eRetCode_FurledByHit:
        case WingControl::eRetCode_CantDuplicateAction:
        case WingControl::eRetCode_NotToFlyingItem:
            break;
        }

        if (string_code > 0)
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {NULL, };
            g_InterfaceManager.GetInterfaceString(string_code, message, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
        }
    }
    else if (res == WingControl::eRet_ItemFailed)
    {
        ItemManager::Instance()->NoticeReasonItemErrorCode(static_cast<DWORD>(result_code));
    }
}

//------------------------------------------------------------------------------ 
void GlobalFunc::DebugOutputMsg( eOUTPUT_MSG_TYPE eMsgType, TCHAR * pszMessage, ...)
{
#if defined ( _DEBUG ) || defined (_GAMECLIENT_VER)
    static TCHAR	szDebugOutputMsg[512];
    static TCHAR	szResultMsg[512];

    va_list args; 
    va_start(args, pszMessage);
    _vsntprintf( szDebugOutputMsg, 512, pszMessage, args ); 
    va_end(args); 

    Sprintf(szResultMsg, _T("[DEBUG]%s"), szDebugOutputMsg);

    default_OutputMsg( eMsgType, szDebugOutputMsg);

#endif
}
//------------------------------------------------------------------------------ 
void GlobalFunc::AlwaysOutputMsg( eOUTPUT_MSG_TYPE eMsgType, TCHAR * pszMessage, ...)
{
    default_OutputMsg( eMsgType, pszMessage);
}
//------------------------------------------------------------------------------ 
#if defined(__CN_20100909_SECURITY_TOKEN__)
//------------------------------------------------------------------------------ 
void GlobalFunc::RetrySecurityToken(bool bYes)
{
    if (bYes)
    {
        uiLoginOneTimePassword* token_ui = GET_CAST_DIALOG(uiLoginOneTimePassword, IM_LOGIN_MANAGER::LOGIN_ONETIMEPASSWORD);
        if (token_ui)
        {
            token_ui->ShowInterface(TRUE);
        }
    }
    else
    {
        GlobalFunc::LogOutGame();
    }
}
#endif // __CN_20100909_SECURITY_TOKEN__

void GlobalFunc::GetDateString(TCHAR* OUT date_string, int string_size, int string_code, SYSTEMTIME& system_time)
{
    // string_code 를 통해 포맷 스트링을 얻음
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };
    g_InterfaceManager.GetInterfaceString(string_code, temp_string);

#ifdef _INTERNATIONAL_DATE_TOOLTIP
    
    TCHAR month_string[32] = {'\0', };
    int month_string_code = 11071;
    uiToolTipMan* ui_tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (ui_tooltip_manager != NULL)
    {
        month_string_code = ui_tooltip_manager->GetStringMonth(system_time.wMonth);
    }
    g_InterfaceManager.GetInterfaceString(month_string_code, month_string, 32);

    #ifdef _RUSSIA
    Snprintf(date_string, 
        string_size - 1, 
        temp_string, 
        month_string,        //달
        system_time.wDay,    //일
        system_time.wYear,   //년도
        system_time.wHour,   //시
        system_time.wMinute);//분
    #else //_RUSSIA
    Snprintf(date_string, 
        string_size - 1, 
        temp_string, 
        month_string,       //달
        system_time.wDay,   //일
        system_time.wHour,  //시
        system_time.wMinute,//분 
        system_time.wYear); //년도
    #endif //_RUSSIA

#else

    Snprintf(date_string, 
        string_size - 1, 
        temp_string, 
        system_time.wYear, 
        system_time.wMonth, 
        system_time.wDay, 
        system_time.wHour, 
        system_time.wMinute );
#endif//_INTERNATIONAL_DATE_TOOLTIP
}

//------------------------------------------------------------------------------ 
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
LPCTSTR GlobalFunc::GetRaceString(eCHAR_TYPE race_type)
{
    DWORD race_name_code = 70395;
    switch (race_type)
    {
    case eCHAR_BERSERKER:
        race_name_code = 10122;//루크
        break;
    case eCHAR_DRAGON:
        race_name_code = 10123;//로드아일
        break;
    case eCHAR_SHADOW:
        race_name_code = 10124;//헬론
        break;
    case eCHAR_VALKYRIE:
        race_name_code = 10125;//에이셔
        break;
    case eCHAR_ELEMENTALIST:
        race_name_code = 10126;//카린
        break;
    case eCHAR_MYSTIC:
        race_name_code = 10127;//미오
        break;
    case eCHAR_HELLROID:  //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        race_name_code = 46693; // HEL-P7
        break;
    case eCHAR_WITCHBLADE:   //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        race_name_code = 46707; // 사라드
        break;
    }
    return g_InterfaceManager.GetInterfaceString(race_name_code);
}
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------ 
LPCTSTR GlobalFunc::GetClassString(eCHAR_TYPE char_type)
{
    switch (char_type)
    {
        case eCHAR_BERSERKER:       return g_InterfaceManager.GetInterfaceString(10001);	break;
        case eCHAR_DRAGON:          return g_InterfaceManager.GetInterfaceString(10004);	break;
        case eCHAR_SHADOW:          return g_InterfaceManager.GetInterfaceString(10005);	break;
        case eCHAR_VALKYRIE:        return g_InterfaceManager.GetInterfaceString(10007);	break;
        case eCHAR_ELEMENTALIST:    return g_InterfaceManager.GetInterfaceString(10010);    break;
            //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        case eCHAR_MYSTIC:          return g_InterfaceManager.GetInterfaceString(10002);    break;
            //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        case eCHAR_HELLROID:        return g_InterfaceManager.GetInterfaceString(10006);    break; 
            //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAR_WITCHBLADE:      return g_InterfaceManager.GetInterfaceString(2685);    break; 
    }
    return g_InterfaceManager.GetInterfaceString(70395);
}
//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------ 
void GlobalFunc::SetText_SignMoney(CControlWZ* control, INT64 money)
{
    if (control != NULL)
    {
        TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
        GlobalFunc::MakeMessage_SignMoney(msg, INTERFACE_STRING_LENGTH, money);

        control->SetTextWZ(msg);
    }
}
//------------------------------------------------------------------------------ 
void GlobalFunc::SetTextFromStringTable(CControlWZ* control, int format_string_code)
{
    if (control != NULL)
    {
        TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
        g_InterfaceManager.GetInterfaceString(format_string_code, msg, INTERFACE_STRING_LENGTH);

        control->SetTextWZ(msg);
    }
}
//------------------------------------------------------------------------------ 
void GlobalFunc::SetTextFromStringTable(CControlWZ* control, int format_string_code, int val)
{
    if (control != NULL)
    {
        TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
        GlobalFunc::MakeMessageFromStringTable(msg, INTERFACE_STRING_LENGTH, format_string_code, val);

        control->SetTextWZ(msg);
    }
}
//------------------------------------------------------------------------------ 
void GlobalFunc::SetTextFromStringTable_Money(CControlWZ* control, int format_string_code, MONEY money)
{
    if (control != NULL)
    {
        TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
        GlobalFunc::MakeMessageFromStringTable_Money(msg, INTERFACE_STRING_LENGTH, format_string_code, money);
 
        control->SetTextWZ(msg);
    }
}
//------------------------------------------------------------------------------ 
void GlobalFunc::SetTextFromStringTable(CControlWZ* control, int format_string_code, TCHAR* val)
{
    if (control != NULL)
    {
        TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
        GlobalFunc::MakeMessageFromStringTable(msg, INTERFACE_STRING_LENGTH, format_string_code, val);

        control->SetTextWZ(msg);
    }
}
//------------------------------------------------------------------------------ 
void GlobalFunc::SetTextFromStringTable(CControlWZ* control, int format_string_code, TCHAR* val1, TCHAR* val2)
{
    if (control != NULL)
    {
        TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
        GlobalFunc::MakeMessageFromStringTable(msg, INTERFACE_STRING_LENGTH, format_string_code, val1, val2);

        control->SetTextWZ(msg);
    }
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MakeMessage_SignMoney(OUT TCHAR* msg, int msg_size, INT64 money)
{
    NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
    TCHAR temp[INTERFACE_STRING_LENGTH] = {NULL, };

    Snprintf(temp, INTERFACE_STRING_LENGTH, _T("%I64d"), money);

    ::GetNumberFormat(LOCALE_SYSTEM_DEFAULT, NULL, temp, &nFmt, msg, msg_size);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, int val)
{
    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(format_string_code, string_format, INTERFACE_STRING_LENGTH);

    TCHAR parameter[32] = {NULL, };
    Snprintf(parameter, _countof(parameter) - 1, _T("%d"), val);

    Snprintf(msg, msg_size, string_format, parameter);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MakeMessageFromStringTable_Money(OUT TCHAR* msg, int msg_size, int format_string_code, MONEY money)
{
    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(format_string_code, string_format, INTERFACE_STRING_LENGTH);

    TCHAR parameter[32] = {NULL, };
    Snprintf(parameter, _countof(parameter) - 1, _T("%I64u"), money);

    Snprintf(msg, msg_size, string_format, parameter);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, TCHAR* val)
{
    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(format_string_code, string_format, INTERFACE_STRING_LENGTH);

    Snprintf(msg, msg_size, string_format, val);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, TCHAR* val1, TCHAR* val2)
{
    if (val1 == NULL || val2 == NULL)
    {
        return;
    }

    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(format_string_code, string_format, INTERFACE_STRING_LENGTH);

    Snprintf(msg, msg_size, string_format, val1, val2);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, 
                                            TCHAR* val1, TCHAR* val2, int val3, TCHAR* val4)
{
    if (val1 == NULL || val2 == NULL || val4 == NULL)
    {
        return;
    }

    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(format_string_code, string_format, INTERFACE_STRING_LENGTH);

    TCHAR parameter[32] = {NULL, };
    Snprintf(parameter, _countof(parameter) - 1, _T("%d"), val3);

    Snprintf(msg, msg_size, string_format, val1, val2, parameter, val4);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, 
                                            int val1, TCHAR* val2, int val3, TCHAR* val4)
{
    if (val2 == NULL || val4 == NULL)
    {
        return;
    }

    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(format_string_code, string_format, INTERFACE_STRING_LENGTH);

    TCHAR parameter[32] = {NULL, };
    Snprintf(parameter, _countof(parameter) - 1, _T("%d"), val1);
    TCHAR parameter2[32] = {NULL, };
    Snprintf(parameter2, _countof(parameter2) - 1, _T("%d"), val3);

    Snprintf(msg, msg_size, string_format, parameter, val2, parameter2, val4);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, 
                                            int val1, TCHAR* val2, TCHAR* val3, TCHAR* val4)
{
    if (val2 == NULL || val4 == NULL)
    {
        return;
    }

    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(format_string_code, string_format, INTERFACE_STRING_LENGTH);

    TCHAR parameter[32] = {NULL, };
    Snprintf(parameter, _countof(parameter) - 1, _T("%d"), val1);

    Snprintf(msg, msg_size, string_format, parameter, val2, val3, val4);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MakeMessageFromStringTable(OUT TCHAR* msg, int msg_size, int format_string_code, 
                                            TCHAR* val1, TCHAR* val2, TCHAR* val3, TCHAR* val4)
{
    if (val2 == NULL || val4 == NULL)
    {
        return;
    }

    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(format_string_code, string_format, INTERFACE_STRING_LENGTH);

    Snprintf(msg, msg_size, string_format, val1, val2, val3, val4);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MakeMessageFromStringTable_YYYYMMDD(OUT TCHAR* msg, int msg_size, int format_string_code, 
                                                     DWORD64 wztime64_value)
{
    if (msg == NULL || msg_size <= 0)
    {
        return;
    }

    SYSTEMTIME systime;
    util::YYYYMMDDHHMMSSToSYSTEMTIME(wztime64_value, systime);
    TCHAR msg_time[INTERFACE_STRING_LENGTH] = {NULL, };
    Snprintf(msg_time, INTERFACE_STRING_LENGTH, " %04d.%02d.%02d", systime.wYear, systime.wMonth, systime.wDay);

    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(format_string_code, string_format, INTERFACE_STRING_LENGTH);

    Snprintf(msg, msg_size, string_format, msg_time);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MakeMessageFromStringTable_tm(OUT TCHAR* msg, int msg_size, int format_string_code, tm* time_stmp)
{
    if (msg == NULL || msg_size <= 0)
    {
        return;
    }

    TCHAR msg_time[INTERFACE_STRING_LENGTH] = {NULL, };
    Snprintf(msg_time, INTERFACE_STRING_LENGTH, " %04d.%02d.%02d", time_stmp->tm_year, time_stmp->tm_mon, time_stmp->tm_mday);

    TCHAR string_format[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(format_string_code, string_format, INTERFACE_STRING_LENGTH);

    Snprintf(msg, msg_size, string_format, msg_time);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::MoveToTopDialog(DWORD dialog_id)
{
    g_InterfaceManager.AttachFirstDialog(dialog_id);
    g_InterfaceManager.DeAttachFirstDialog(dialog_id);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::ExecWebSite(DWORD string_code)
{
    TCHAR register_url[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(string_code, register_url, INTERFACE_STRING_LENGTH);

    SHELLEXECUTEINFO shell_execute_info;
    {
        shell_execute_info.cbSize = sizeof(SHELLEXECUTEINFO);
        shell_execute_info.fMask = NULL;
        shell_execute_info.hwnd = g_hWndMain;
        shell_execute_info.lpVerb = NULL;
        shell_execute_info.lpFile = register_url;
        shell_execute_info.lpParameters = NULL;
        shell_execute_info.lpDirectory = NULL;
        shell_execute_info.nShow = SW_MAXIMIZE;
        shell_execute_info.hInstApp = NULL;
    }
    ShellExecuteEx(&shell_execute_info);
}
//------------------------------------------------------------------------------ 
void GlobalFunc::SetFTextFromStringTable(FTextRender& render, FTextParser& parser, FTextSentence& sentence, int format_string_code)
{
    TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
    g_InterfaceManager.GetInterfaceString(format_string_code, msg, INTERFACE_STRING_LENGTH);

    if (parser.Parser(msg, sentence) == TRUE)
    {
        render.Append(sentence);
    }
}
//------------------------------------------------------------------------------ 
void GlobalFunc::SetFTextFromStringTable(FTextRender& render, FTextParser& parser, FTextSentence& sentence, int format_string_code, int val1)
{
    TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
    GlobalFunc::MakeMessageFromStringTable(msg, INTERFACE_STRING_LENGTH, format_string_code, val1);

    if (parser.Parser(msg, sentence) == TRUE)
    {
        render.Append(sentence);
    }
}
//------------------------------------------------------------------------------ 
void GlobalFunc::SetFTextFromMsg(FTextRender& render, FTextParser& parser, FTextSentence& sentence, const TCHAR* msg)
{
    if (parser.Parser(msg, sentence) == TRUE)
    {
        render.Append(sentence);
    }
}

DWORD GlobalFunc::VerifyMessage(DWORD message_code, 
                                DWORD title_code /*= 0*/, 
                                uiSystemMessage::MessageCallback callback_function /*= NULL*/, 
                                bool is_modal /*= false*/)
{
    DWORD message_dialog_key = 0;
    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager == NULL)
    {
        return message_dialog_key;
    }

    TCHAR title_stinrg[INTERFACE_STRING_LENGTH] = {'\0', };
    if (title_code == 0)
    {
        // 4533 알림!
        g_InterfaceManager.GetInterfaceString(4533, title_stinrg);
    }
    else
    {
        g_InterfaceManager.GetInterfaceString(title_code, title_stinrg);
    }

    message_dialog_key = ui_system_manager->Sys_SystemMessage(
        g_InterfaceManager.GetInterfaceString(message_code), 
        title_stinrg, 
        callback_function, 
        uiSystemMessage::kVerify_Type, 
        is_modal);

    return message_dialog_key;
}

DWORD GlobalFunc::VerifyMessage(const TCHAR* message, 
                                const TCHAR* title, 
                                uiSystemMessage::MessageCallback callback_function /*= NULL*/, 
                                bool is_modal /*= false*/)
{
    DWORD message_dialog_key = 0;
    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager == NULL)
    {
        return message_dialog_key;
    }

    message_dialog_key = ui_system_manager->Sys_SystemMessage(
        message, 
        title, 
        callback_function, 
        uiSystemMessage::kVerify_Type, 
        is_modal);

    return message_dialog_key;
}
DWORD GlobalFunc::ConfirmMessage(DWORD message_code, 
                                 DWORD title_code /*= 0*/, 
                                 uiSystemMessage::MessageCallback callback_function /*= NULL*/, 
                                 bool is_modal /*= false*/)
{
    DWORD message_dialog_key = 0;
    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager == NULL)
    {
        return message_dialog_key;
    }

    TCHAR title_stinrg[INTERFACE_STRING_LENGTH] = {'\0', };
    if (title_code == 0)
    {
        // 4533 알림!
        g_InterfaceManager.GetInterfaceString(4533, title_stinrg);
    }
    else
    {
        g_InterfaceManager.GetInterfaceString(title_code, title_stinrg);
    }

    message_dialog_key = ui_system_manager->Sys_SystemMessage(
        g_InterfaceManager.GetInterfaceString(message_code), 
        title_stinrg, 
        callback_function, 
        uiSystemMessage::kConfirm_Type, 
        is_modal);

    return message_dialog_key;
}

DWORD GlobalFunc::ConfirmMessage(const TCHAR* message, 
                                 const TCHAR* title, 
                                 uiSystemMessage::MessageCallback callback_function /*= NULL*/, 
                                 bool is_modal /*= false*/)
{
    DWORD message_dialog_key = 0;
    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager == NULL)
    {
        return message_dialog_key;
    }

    message_dialog_key = ui_system_manager->Sys_SystemMessage(
        message, 
        title, 
        callback_function, 
        uiSystemMessage::kConfirm_Type, 
        is_modal);

    return message_dialog_key;
}

void GlobalFunc::EnableControl( CControlWZ* control_ptr, bool enable_state )
{
    if (control_ptr == NULL)
    {
        return;
    }

    bool current_state = (control_ptr->GetDrawState() == E_DISABLE) ? false : true;
    if (current_state != enable_state)
    {
        control_ptr->EnableWindowWZ(enable_state);
    }
}

float GlobalFunc::GetCameraHeight( eCHAR_TYPE CharType )
{
	static float default_eye_height = 1.0f;
	if( CharType == eCHAR_MYSTIC )
		return default_eye_height;

	return 1.3f;	// default
}

void GlobalFunc::EquipmentPlayerRefreshAppearRender( const BOOL& is_bone_refresh /*= FALSE*/ )
{
    Player* equipment_player_ptr = static_cast<Player*>(g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY));
    if ( equipment_player_ptr )
    {
        equipment_player_ptr->RefreshAppearRender(is_bone_refresh);
    }
}

#ifdef _NA_000000_20140519_GAMEGUARD_LOG
void GlobalFunc::Write_GameGuard_Log( TCHAR* message )
{
    struct tm * today;
    time_t		ltime;
    time( &ltime );
    today = localtime( &ltime );
    TCHAR szOutputMsg[INTERFACE_STRING_LENGTH] = {0, };

    Snprintf(szOutputMsg, INTERFACE_STRING_LENGTH-1, _T("[%02d:%02d:%02d] %s\n"), today->tm_hour, today->tm_min, today->tm_sec, message);

    TCHAR szFileName[MAX_PATH] = {0,};
    Snprintf(szFileName, MAX_PATH-1,
        _T("ClientGameGuardLog_%02d%02d%02d_%s.log"), 
        today->tm_year+1900, 
        today->tm_mon+1, 
        today->tm_mday, 
        c_tcBUILD_VERSION);

    FILE * fp = _tfopen(szFileName, "a");
    fwrite( szOutputMsg, _tcslen(szOutputMsg), 1, fp );
    fclose( fp );
}
#endif // _NA_000000_20140519_GAMEGUARD_LOG

#ifdef _NA_008334_20150608_SONNENSCHEIN
void GlobalFunc::ShowSonnenScheinResurrectionPopup(DWORD l_prarm, DWORD r_prarm)
{
    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
    uiSystemMan* system_manager =
        static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
    if (system_manager)
    {
        g_KeyQueueManager.SetDefaultFun(NULL);
        CastLeaveFieldDialog* castleave_dialog = GET_CAST_DIALOG(CastLeaveFieldDialog, StrToWzID("0281"));    
        if (castleave_dialog)
        {
            castleave_dialog->ShowWindow(FALSE);
        }

        // 891 캐릭터가 사망했습니다.
        g_InterfaceManager.GetInterfaceString(891, string, INTERFACE_STRING_LENGTH - 1);
        system_manager->Sys_ConfirmDontCloseButton(string, GlobalFunc::SonnenScheinResurrectionConfirm, NULL, NULL, 3476 /* 부활하기 */, 3301 /*필드로 이동*/);
    }
}
void GlobalFunc::SonnenScheinResurrectionConfirm(int confirm_value)
{
    if (confirm_value == kConfirmSelect1)
    {
        // 부활하기
        if (g_pHero && g_pHero->IsDead())
        {
            keyMsg msg;
            ZeroMemory(&msg,sizeof(msg));
            msg.dwType		= eDoType_Delay_Spawn_SonnenSchein;
            msg.DoSomething	= ShowSonnenScheinResurrectionPopup;
            msg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
            msg.wParam		= eDoType_Delay_Spawn_SonnenSchein;
            g_KeyQueueManager.PushBack(msg);

            StartLeaveFieldCastTimer( eDoType_Delay_Spawn_SonnenSchein, 5000);

            // 3326	%d초 후에 시작지점으로 리스폰 됩니다.
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR szResultInfo[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(3326, szMessage, INTERFACE_STRING_LENGTH);
            Sprintf( szResultInfo, szMessage, 5);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResultInfo);
        }
        else
        {
            g_KeyQueueManager.SetDefaultFun(GlobalFunc::DoSystemDialog);
        }

        //ShowInterface(FALSE);
    }
    else if (confirm_value == kConfirmSelect2)
    {
        BattleScene::BackToTheVillageTransact();
    }
}
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
bool GlobalFunc::HeimTradingvolumeCheck( const MONEY& money)
{
	const MONEY& heim_tradingvolume = GAMEOPTION->GetHeimTradingvolumeNotification();
	if (heim_tradingvolume != 0 && 
		money >= heim_tradingvolume)
	{
		return true;
	}
	else
	{
		return false;
	}
}
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION