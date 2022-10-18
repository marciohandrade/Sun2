#include "SunClientPrecompiledHeader.h"
#include "interfacemanager.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "uiDominationTowerMainDlg/uiDominationTowerMainDlg.h"
#include "uiDominationTowerBettingListDlg/uiDominationTowerBettingListDlg.h"

#include "uiDominationTowerManagerDlg/uiDominationTowerManagerDlg.h"
#include "uiDominationTowerAreaDlg/uiDominationTowerAreaDlg.h"
#include "uiDominationTimer/uiDominationTimer.h"
#include "uiDominationZoneMarkDlg/uiDominationZoneMarkDlg.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemData.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "GuildMarkInfoParser.h"
#include "uiNoticeMan/uiNoticeMan.h"
#include "uiGuildNoticeDialog/uiGuildNoticeDialog.h"
#include "MapInfoParser.h"
#include "GlobalFunc.h"
#include "ResultCode.h"
#include "Hero.h"
#include "GameFramework.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "uiLoginMan/uiLoginMan.h"
#include "uiLoginMan/uiLoginMan_def.h"
#include "uiDominationStamp/uiDominationStamp.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "DominationInfoParser.h"
#include "Map.h"
#include "uiBattleScoreMan\\uiBattleScoreMan.h"
#include "ItemManager.h"
#include "SCItemSlot.h"
#include "SlotKeyGenerator.h"
#include "ItemUnitRenderer.h"
#include "MouseHandler.h"

#include "uiRadarMan/uiRadarMan.h"
#include "ContinentMapDlg.h"
#include "uiContinentMapEP2/ContinentMapEp2Dialog.h"

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
#include "uiDominationJoinList1/uiDominationJoinList1.h"
#include "uiDominationJoinList2/uiDominationJoinList2.h"
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
#include "uiDominationTowerApplyBettingDlg/uiDominationTowerApplyBettingDlg.h"
#include "uiDominationTowerApplyBettingDlg2/uiDominationTowerApplyBettingDlg2.h"
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

#include "uiDominationBettingList/uiDominationBettingList.h"
#include "SCItemSlotContainer.h"

#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
#include "uiDominationResurrection/uiDominationResurrection.h"
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
#include "BattleScene.h"
#include "DominationWarScene.h"
#include "uiDominationAreaOwnerTutorial/uiDominationAreaOwnerTutorial.h"
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL



const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TOWER_MAIN_DIALOG	= StrToWzID("9722");
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST_DIALOG	= StrToWzID("9723");
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_AREA_DIALOG			= StrToWzID("9721");
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TIMER_DIALOG			= StrToWzID("9727");
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_ZONEMARK_DIALOG		= StrToWzID("9729");
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_STAMP_DIALOG			= StrToWzID("9728");

const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST = StrToWzID("9731");

#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_MANAGER_DIALOG		= StrToWzID("9725");
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_APPLY_BETTING_DIALOG = StrToWzID("9724");
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_APPLY_BETTING_DIALOG2= StrToWzID("9730");
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_JOIN_LIST1 = StrToWzID("9733");
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_JOIN_LIST2 = StrToWzID("9732");
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_RESURRECTION_POPUP = g_IDGenerator.GenerateID();
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
const DWORD IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_AREA_OWNER_TUTORIAL_DIALOG = StrToWzID("0438");
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

uiDominationTowerMan::uiDominationTowerMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    domination_object_key_current_index_ = -1;
    is_character_enter_ = FALSE;
#else
	m_dwObjectKey = 0;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	m_bShowDominationObject = false;

#ifdef _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL
    is_domination_manage_dummy_packet_ = false;
#endif //_TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::Clear()
{
	memset(&m_DominationGuildInfo,0,sizeof(m_DominationGuildInfo));
	memset(&m_ChallengerGuildInfo,0,sizeof(m_ChallengerGuildInfo));
	m_bShowDominationGuildInfo = false;
	m_OwnHeim = 0;
	m_MostOfferHeim = 0;
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    domination_object_key_current_index_ = -1;
    domination_object_key_vector_.clear();
    ZeroMemory(mondschien_guild_name_, MAX_GUILDNAME_LENGTH+1);
    is_character_enter_ = FALSE;
#else
	m_dwObjectKey = 0;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	m_bShowDominationObject = false;
	m_mapDominatioGuild.clear();
	m_pDominationZoneMark->Clear();
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::InitPKBooleans()
{
	this->m_vPKBool.clear();

	for (int i=ePKBOOL_DOMINATION_BASE; i<ePKBOOL_DOMINATIONLOGIN_MAX; ++i)
	{
		uicPacketControl packetControl;
		this->m_vPKBool.push_back(packetControl);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::OnRelease()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::OnInitialize()
{
	m_pMainDialog = CreateUIDialog<uiDominationTowerMainDlg>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TOWER_MAIN_DIALOG, "Data\\Interface\\97_22_area.iwz", this);
	assert(m_pMainDialog);
	m_DominationArea = CreateUIDialog<uiDominationTowerAreaDlg>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_AREA_DIALOG, "Data\\Interface\\97_21_guild_name.iwz", this, TRUE);
	assert(m_DominationArea);
	m_pDominationTimer = CreateUIDialog<uiDominationTimer>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TIMER_DIALOG, "Data\\Interface\\97_27_mercenary_time.iwz", this);
	assert(m_pDominationTimer);
	m_pDominationZoneMark = CreateUIDialog<uiDominationZoneMarkDlg>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_ZONEMARK_DIALOG, "Data\\Interface\\97_29_zone_mark.iwz", this, TRUE);
	assert(m_pDominationZoneMark);
	m_pDominationStamp = CreateUIDialog<uiDominationStamp>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_STAMP_DIALOG, "Data\\Interface\\97_28_counter.iwz", this);
    assert(m_pDominationStamp);

    m_pDominationBettingList = CreateUIDialog<uiDominationBettingList>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST, "Data\\Interface\\97_31_area_entry.iwz", this);
    assert(m_pDominationBettingList);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    m_pBettingList = CreateUIDialog<uiDominationTowerBettingListDlg>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST_DIALOG, "Data\\Interface\\97_23_area_entry.iwz", this, TRUE);
    assert(m_pBettingList);
    m_pDominationJoinList1 = CreateUIDialog<uiDominationJoinList1>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_JOIN_LIST1, "Data\\Interface\\97_33_List.iwz", this, TRUE);
    assert(m_pDominationJoinList1);
    m_pDominationJoinList2 = CreateUIDialog<uiDominationJoinList2>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_JOIN_LIST2, "Data\\Interface\\97_32_List.iwz", this, TRUE);
    assert(m_pDominationJoinList2);
    m_pApplyBetting = CreateUIDialog<uiDominationTowerApplyBettingDlg>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_APPLY_BETTING_DIALOG, "Data\\Interface\\97_24_area_offer.iwz", this);
    assert(m_pApplyBetting);
    m_pApplyBetting2 = CreateUIDialog<uiDominationTowerApplyBettingDlg2>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_APPLY_BETTING_DIALOG2, "Data\\Interface\\97_30_area_register.iwz", this);
    assert(m_pApplyBetting2);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
    m_DominationManager = CreateUIDialog<uiDominationTowerManagerDlg>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_MANAGER_DIALOG, "Data\\Interface\\97_25_area_edit.iwz", this, TRUE);
    assert(m_DominationManager);
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL

#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
    ui_domination_resurrection_ = CreateUIDialog<uiDominationResurrection>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_RESURRECTION_POPUP, "Data\\Interface\\82_asking_window.iwz", this, TRUE, TRUE);
    assert(ui_domination_resurrection_);
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    ui_domination_owner_tutorial_dialog_ = CreateUIDialog<uiDominationAreaOwnerTutorial>(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_AREA_OWNER_TUTORIAL_DIALOG, "Data\\Interface\\43_8_Mont_Tutorial.iwz", this, TRUE, TRUE);
    assert(ui_domination_owner_tutorial_dialog_);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	Clear();

}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::Update()
{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    if (domination_object_key_vector_.empty() || 
        static_cast<unsigned int>(domination_object_key_current_index_) >= domination_object_key_vector_.size())
        return;

    if (domination_object_key_vector_[domination_object_key_current_index_])
    {
        MapObject* pObject = 
            static_cast<MapObject*>(g_ObjectManager.GetObject(domination_object_key_vector_[domination_object_key_current_index_]));
#else
	if (m_dwObjectKey)
	{
		MapObject* pObject = static_cast<MapObject*>(g_ObjectManager.GetObject(m_dwObjectKey));
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
		if(pObject && g_pHero)
		{
			WzVector vDiff = pObject->GetPosition() - g_pHero->GetVisiblePos();
			float distance = VectorLength2(&vDiff);
			if (distance > (DISTANCE_NPC_MEET * DISTANCE_NPC_MEET))
			{
				m_pMainDialog->ShowInterface(FALSE);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
				m_pBettingList->ShowInterface(FALSE);
                m_pApplyBetting->ShowInterface(FALSE);
                m_pApplyBetting2->ShowInterface(FALSE);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
				
#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
                m_DominationManager->ShowInterface(FALSE);
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
                domination_object_key_current_index_ = -1;
#else
				m_dwObjectKey = 0;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
			}
		}
	}	
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::NetworkProc( MSG_BASE * pMsg )
{
	switch (pMsg->m_byCategory)
	{
	case CG_BATTLE:
		{
			switch (pMsg->m_byProtocol)
			{
            case CG_DOMINATION_NOTICE_ACK:
                {
                    RECV_DOMINATION_NOTICE_ACK(pMsg);
                }
                break;
#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
			case CG_DOMINATION_BATTING_HEIM_ACK:
				{
                    RECV_DOMINATION_BATTING_HEIM_ACK(pMsg);
				}
				break;
            case CG_DOMINATION_ACCEP_HEIM_ACK:
                {					
                    RECV_DOMINATION_ACCEP_HEIM_ACK(pMsg);
                }
                break;
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
			case CG_DOMINATION_MANAGE_ACK:
				{
                    RECV_DOMINATION_MANAGE_ACK(pMsg);
				}
				break;
			case CG_DOMINATION_SAVE_GATESTAT_ACK:
				{
                    RECV_DOMINATION_SAVE_GATESTAT_ACK(pMsg);
				}
				break;
            case CG_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK:
                {
                    RECV_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK(pMsg);
                }
                break;


#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
            case CG_DOMINATION_DEFENSE_GUILD_ALLIENCE_LIST_BRD:
                {
                    RECV_DOMINATION_DEFENSE_GUILD_ALLIENCE_LIST_BRD(pMsg);
                }
                break;
#else
            case CG_DOMINATION_APPLY_ACK:
                {
                    RECV_DOMINATION_APPLY_ACK(pMsg);	
                }
                break;
            case CG_DOMINATION_MEMBERLIST_NTF:
                {
                    RECV_DOMINATION_MEMBERLIST_NTF(pMsg);
                }
                break;
            case CG_DOMINATION_MEMBER_LEAVE_ACK:
                {
                    RECV_DOMINATION_MEMBER_LEAVE_ACK(pMsg);
                }
                break;
            case CG_DOMINATION_MEMBER_JOIN_ACK:
                {
                    RECV_DOMINATION_MEMBER_JOIN_ACK(pMsg);
                }
                break;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
			case CG_DOMINATION_MINIMAP_SET_BRD:
				{
					m_bShowDominationGuildInfo = false;
				}
				break;
			case CG_DOMINATION_MINIMAP_LIST_ACK:
				{
                    RECV_DOMINATION_MINIMAP_LIST_ACK(pMsg);
				}
				break;
			case CG_DOMINATION_SELECTED_BRD:
				{
					MSG_CG_DOMINATION_SELECTED_BRD* pRecvPacket = (MSG_CG_DOMINATION_SELECTED_BRD*)pMsg;
					{
						uiNoticeMan* pUIMan = GET_DIALOG_MANAGER(uiNoticeMan, UIMAN_NOTICE);
						//uiGuildNoticeDialog* pDlg = GET_CAST_DIALOG(uiGuildNoticeDialog, IM_NOTICE_MANAGER::GUILD_NOTICE_DIALOG);
						const sMAPINFO* pFieldInfo = MapInfoParser::Instance()->FindMapInfo(pRecvPacket->m_DominationFieldCode);
						if(pUIMan && pFieldInfo)
						{
							TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
							TCHAR szFieldName[INTERFACE_STRING_LENGTH] = {0, };
							TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0, };
							
							g_InterfaceManager.GetInterfaceString(pFieldInfo->dwNCode, szFieldName, INTERFACE_STRING_LENGTH);
							g_InterfaceManager.GetInterfaceString(3319, szTemp, INTERFACE_STRING_LENGTH);
							Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, szFieldName);
							pUIMan->ShowNoticeMessage(eGuild_Notice, szMessage, 10000);
						}	
					}			
				}
				break;				
			case CG_DOMINATION_AREA_INFO_CMD:
				{
					MSG_CG_DOMINATION_AREA_INFO_CMD* recv_packet = (MSG_CG_DOMINATION_AREA_INFO_CMD*)pMsg;
                    Player* player = static_cast<Player*>(g_ObjectManager.GetObject(recv_packet->m_dwPlayerKey));
                    if (player != NULL)
                    {
                        player->SetDominationStateInfo(&recv_packet->m_stateTotalInfo);

                        // 자신의 인장UI(zonemark) 업데이트
                        if (g_pHero && (g_pHero->GetObjectKey() == recv_packet->m_dwPlayerKey))
                        {                        
                            uiDominationTowerMan::RefreshZoneMarkInfo();
                        }
                    }
				}
				break;

			case CG_DOMINATION_TIME_SYNC_BRD:
				{
					MSG_CG_DOMINATION_TIME_SYNC_BRD* pRecvPacket = (MSG_CG_DOMINATION_TIME_SYNC_BRD*)pMsg;
					uiDominationTimer* pDlg = GET_CAST_DIALOG(uiDominationTimer, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TIMER_DIALOG);
					if(pDlg)
					{
						if(DOMINATION_EVENT_END == pRecvPacket->m_byEventState)
						{
							if(TRUE == pDlg->IsVisible())
								pDlg->ShowInterface(FALSE);

#ifdef _NA_008334_20150608_SONNENSCHEIN
                            pDlg->set_event_state(pRecvPacket->m_byEventState);
#endif //_NA_008334_20150608_SONNENSCHEIN

							break;
						}

						if(FALSE == pDlg->IsVisible())
							pDlg->ShowInterface(TRUE);

						pDlg->SetTimer(pRecvPacket->m_dwRemainedTime, true);
						if(DOMINATION_EVENT_WAIT == pRecvPacket->m_byEventState)
							pDlg->SetStartTimer(false);
						else if(DOMINATION_EVENT_START == pRecvPacket->m_byEventState)
							pDlg->SetStartTimer(true);

#ifdef _NA_008334_20150608_SONNENSCHEIN
                        pDlg->set_event_state(pRecvPacket->m_byEventState);
#endif //_NA_008334_20150608_SONNENSCHEIN
					}
				}
				break;

			case CG_DOMINATION_COUNT_NOSES_BRD:
				{
					MSG_CG_DOMINATION_COUNT_NOSES_BRD* pRecvPacket = (MSG_CG_DOMINATION_COUNT_NOSES_BRD*)pMsg;
					uiDominationTimer* pDlg = GET_CAST_DIALOG(uiDominationTimer, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TIMER_DIALOG);
					if(pDlg)
					{
						if(FALSE == pDlg->IsVisible())
							pDlg->ShowInterface(TRUE);

						pDlg->SetPlayerCnt(pRecvPacket->m_byCount);
					}
				}
				break;

			case CG_DOMINATION_KICK_NOTIFY_BRD:
				{
					uiNoticeMan* pUIMan = GET_DIALOG_MANAGER(uiNoticeMan, UIMAN_NOTICE);
					//uiGuildNoticeDialog* pGuildNoticeDlg = GET_CAST_DIALOG(uiGuildNoticeDialog, IM_NOTICE_MANAGER::GUILD_NOTICE_DIALOG);
					if(pUIMan)
					{
						//3317	시스템 준비를 위해 타워 입구로 강제 이동됩니다.\n비행선을 통해 재 입장 하시기 바랍니다.
						TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
						g_InterfaceManager.GetInterfaceString(3317, szMessage);

						pUIMan->ShowNoticeMessage(eGuild_Notice, szMessage, 10000);
					}
				}
				break;

			case CG_DOMINATION_START_NOTIFY_BRD:
				{
                    RECV_DOMINATION_START_NOTIFY_BRD(pMsg);
				}
				break;

			case CG_DOMINATION_BEGIN_STAMP_BRD:
				{
					MSG_CG_DOMINATION_BEGIN_STAMP_BRD* pRecvPacket = (MSG_CG_DOMINATION_BEGIN_STAMP_BRD*)pMsg;

					uiDominationStamp* pStampDlg = GET_CAST_DIALOG(uiDominationStamp, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_STAMP_DIALOG);
					if(pStampDlg)
					{
						pStampDlg->SetStampID(pRecvPacket->m_nStampID);
						pStampDlg->ShowInterface(TRUE);

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
                        pStampDlg->set_stamp_guild_name(pRecvPacket->m_stamp_guild_name);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
					}
				}	
				break;

			case CG_DOMINATION_CANCLE_STAMP_BRD:
				{
					MSG_CG_DOMINATION_CANCLE_STAMP_BRD* pRecvPacket = (MSG_CG_DOMINATION_CANCLE_STAMP_BRD*)pMsg;

					uiDominationStamp* pStampDlg = GET_CAST_DIALOG(uiDominationStamp, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_STAMP_DIALOG);
					if(pStampDlg && pStampDlg->IsVisibleWindow())
						pStampDlg->ShowInterface(FALSE);
				}
				break;

			case CG_DOMINATION_CRYSTALWARP_STATE_BRD:
				{
					MSG_CG_DOMINATION_CRYSTALWARP_STATE_BRD* pRecvPacket = (MSG_CG_DOMINATION_CRYSTALWARP_STATE_BRD*)pMsg;
					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					switch(pRecvPacket->m_byState)
					{
					case eCRYSTALWARP_STATE_CREATE:
						break;
					case eCRYSTALWARP_STATE_DESTROY:
					case eCRYSTALWARP_STATE_DEAD:
						{
							if(g_pHero)
								g_pHero->SetCrystalWarpKey(pRecvPacket->m_dwCrystalWarpKey, false);

							//3322	워프수정이 파괴되었습니다.
							g_InterfaceManager.GetInterfaceString(3322, szMessage);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
						}
						break;
					default:
						break;
					}
				}
				break;

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
            case CG_DOMINATION_TURNOVER_BRD:
                {
                    MSG_CG_DOMINATION_TURNOVER_BRD* recv_packet = static_cast<MSG_CG_DOMINATION_TURNOVER_BRD*>(pMsg);

                    if(NULL == g_pHero)
                        break;

                    uiDominationStamp* stamp_dialog_ptr = GET_CAST_DIALOG(uiDominationStamp, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_STAMP_DIALOG);
                    if(stamp_dialog_ptr && stamp_dialog_ptr->IsVisibleWindow())
                        stamp_dialog_ptr->ShowInterface(FALSE);

                    TCHAR temp_string[INTERFACE_STRING_LENGTH + 1] = {0,};
                    TCHAR message_string[INTERFACE_STRING_LENGTH + 1] = {0,};
                    TCHAR map_name_string[INTERFACE_STRING_LENGTH + 1] = {0,};
                    const sMAPINFO* cMI = MapInfoParser::Instance()->FindMapInfo(recv_packet->m_mapCode);
                    
                    if(NULL == cMI)
                    {
#ifdef _DEV_VER
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "MSG_CG_DOMINATION_IN_GUILD_INFO::m_mapCode = %d", recv_packet->m_mapCode);
#endif
                        break;
                    }
                    
                    g_InterfaceManager.GetInterfaceString(cMI->dwNCode, map_name_string);

                    uiNoticeMan* notice_manager = GET_DIALOG_MANAGER(uiNoticeMan, UIMAN_NOTICE);
                    if (notice_manager)
                    {
                        //3477	%s길드가 요새의 각인에 성공하였습니다. \n이제부터 %s길드가 성을 수비하게 됩니다.
                        g_InterfaceManager.GetInterfaceString(3477, temp_string);

                        recv_packet->win_guild_name[MAX_GUILDNAME_LENGTH] = '\0';
                        Snprintf(message_string, INTERFACE_STRING_LENGTH, temp_string, recv_packet->win_guild_name, recv_packet->win_guild_name);

                        notice_manager->ShowNoticeMessage(eGM_Notice, message_string, 10000);
                    }

                    if (g_pMap->GetMapID() == recv_packet->m_mapCode)
                    {
                        //점령길드 교체
                        m_DominationGuildInfo.m_GuildGuid = recv_packet->m_winGuid;

                        //현재 보이는 몬스터들 정보도 변경
                        std::vector<Monster*> vecMons;
                        g_ObjectManager.GetMonsters(vecMons);
                        int iCnt = (int)vecMons.size();
                        for(int i = 0; i < iCnt; ++i)
                        {
                            if(eNPC_DOMINATION_MAPOBJECT_NPC == vecMons[i]->GetMonsterInfo()->m_byGrade)
                                vecMons[i]->SetMasterGuildID(m_DominationGuildInfo.m_GuildGuid);
                        }
                        vecMons.clear();
                    }
                }
                break;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

			case CG_DOMINATION_RESULT_BRD:
				{
					MSG_CG_DOMINATION_RESULT_BRD* pRecvPacket = (MSG_CG_DOMINATION_RESULT_BRD*)pMsg;
					if(NULL == g_pHero)
						break;

#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
                    if (m_DominationManager)
                    {
                        m_DominationManager->InitPoint(DOMINATION_DEFFENSWALL_POINT);
                        m_DominationManager->ShowPictureControlByPoint();
                    }
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL

					TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szMapName[INTERFACE_STRING_LENGTH + 1] = {0,};
					const sMAPINFO* cMI = MapInfoParser::Instance()->FindMapInfo(pRecvPacket->m_mapCode);
					if(NULL == cMI)
					{
#ifdef _DEV_VER
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "MSG_CG_DOMINATION_RESULT_BRD::m_mapCode = %d", pRecvPacket->m_mapCode);
#endif 
						break;
					}

					g_InterfaceManager.GetInterfaceString(cMI->dwNCode, szMapName);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
                    uiNoticeMan* notice_manager = GET_DIALOG_MANAGER(uiNoticeMan, UIMAN_NOTICE);
                    if (notice_manager)
                    {
                        // 공성성공 메세지인지 수성성공 메세지인지
                        bool is_offence_win_message = true;

                        //GetDominationGuildInfo()->size() 가 0 이면 B창을 한번도 안눌렀거나 지역점령전 입장 창을 안 열어본 사람. 즉 밖에 있는 사람이다
                        if (GetDominationGuildMap().size() != 0)
                        {
                            DOMINATIONMAP_ITR itr = GetDominationGuildMap().find(pRecvPacket->m_mapCode);
                            DOMINATION_GUILD_INFO *guild_info = &(itr->second);
                            if (guild_info && guild_info->m_GuildGuid == pRecvPacket->m_winGuid)
                            {
                                // 승리길드가 알고있던 점령길드면 수성이 성공했다는 의미.
                                is_offence_win_message = false;
                            }
                        }

                        if (is_offence_win_message)
                        {
                            //3327	%s 길드가 %s 지역을 점령하였습니다.
                            g_InterfaceManager.GetInterfaceString(3327, szTemp);
                        }
                        else
                        {
                            //7399	%s길드가 %s지역 방어에 성공하였습니다.
                            g_InterfaceManager.GetInterfaceString(7399, szTemp);
                        }
                        pRecvPacket->win_guild_name[MAX_GUILDNAME_LENGTH] = '\0';
                        Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, pRecvPacket->win_guild_name, szMapName);

                        notice_manager->ShowNoticeMessage(eGM_Notice, szMessage, 5000);
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
                        if (GlobalFunc::IsMondschienMap() == TRUE)
                        {
                            // 3502 몬트샤인 지역을 차지한 길드와 그 동맹들은 [몬트샤인의 통로] 오브젝트의 [보상 아이템 지급] 버튼을 이용하여 특수 보상을 수령 할 수 있습니다.
                            ZeroMemory(szTemp, INTERFACE_STRING_LENGTH);
                            g_InterfaceManager.GetInterfaceString(3502, szTemp);
                            notice_manager->ShowNoticeMessage(eGM_Notice, szTemp, 10000);
                        }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

                    }

                    if (g_pMap->GetMapID() == pRecvPacket->m_mapCode)
                    {
                        uiDominationStamp* pStampDlg = GET_CAST_DIALOG(uiDominationStamp, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_STAMP_DIALOG);
                        if(pStampDlg && pStampDlg->IsVisibleWindow())
                            pStampDlg->ShowInterface(FALSE);

                        //점령길드 교체
                        m_DominationGuildInfo.m_GuildGuid = pRecvPacket->m_winGuid;

                        //현재 보이는 몬스터들 정보도 변경
                        std::vector<Monster*> vecMons;
                        g_ObjectManager.GetMonsters(vecMons);
                        int iCnt = (int)vecMons.size();
                        for(int i = 0; i < iCnt; ++i)
                        {
                            if(eNPC_DOMINATION_MAPOBJECT_NPC == vecMons[i]->GetMonsterInfo()->m_byGrade)
                                vecMons[i]->SetMasterGuildID(m_DominationGuildInfo.m_GuildGuid);
                        }
                        vecMons.clear();

                        if (pRecvPacket->m_winGuid == g_pHero->GetPlayerGuildID())
                        {
                            // 보상옵션 안내 메시지 출력
                            const sDOMINATION_INFO* pDI = DominationInfoParser::Instance()->GetDominationInfo(pRecvPacket->m_mapCode);
                            if (pDI && notice_manager)
                            {
                                g_InterfaceManager.GetInterfaceString(3329, szTemp);
                                Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, szMapName, szMapName, pDI->m_RewardDropHeimRate/10);
                                notice_manager->ShowNoticeMessage(eGuild_Notice, szMessage, 10000);
                            }
                        }
                    }
#else
                    uiDominationStamp* pStampDlg = GET_CAST_DIALOG(uiDominationStamp, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_STAMP_DIALOG);
                    if(pStampDlg && pStampDlg->IsVisibleWindow())
                        pStampDlg->ShowInterface(FALSE);

                    if(pRecvPacket->m_winGuid == g_pHero->GetPlayerGuildID())
					{
						uiNoticeMan* pUIMan = GET_DIALOG_MANAGER(uiNoticeMan, UIMAN_NOTICE);
						if(pUIMan)
						{
							//3327	%s 길드가 %s 지역을 점령하였습니다.
							g_InterfaceManager.GetInterfaceString(3327, szTemp);
							Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, g_pHero->GetGuildName(), szMapName);

							pUIMan->ShowNoticeMessage(eGuild_Notice, szMessage, 10000);
						}

						if(g_pMap->GetMapID() != pRecvPacket->m_mapCode)
							break;

						//점령길드 교체
						m_DominationGuildInfo.m_GuildGuid = g_pHero->GetPlayerGuildID();

						//현재 보이는 몬스터들 정보도 변경
						std::vector<Monster*> vecMons;
						g_ObjectManager.GetMonsters(vecMons);
						int iCnt = (int)vecMons.size();
						for(int i = 0; i < iCnt; ++i)
						{
							if(eNPC_DOMINATION_MAPOBJECT_NPC == vecMons[i]->GetMonsterInfo()->m_byGrade)
								vecMons[i]->SetMasterGuildID(m_DominationGuildInfo.m_GuildGuid);
						}
						vecMons.clear();

						// 보상옵션 안내 메시지 출력
						const sDOMINATION_INFO* pDI = DominationInfoParser::Instance()->GetDominationInfo(pRecvPacket->m_mapCode);
						if (pDI && pUIMan)
						{
							g_InterfaceManager.GetInterfaceString(3329, szTemp);
							Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, szMapName, szMapName, pDI->m_RewardDropHeimRate/10);
							pUIMan->ShowNoticeMessage(eGuild_Notice, szMessage, 10000);
						}
					}
					else if(pRecvPacket->m_loseGuid == g_pHero->GetPlayerGuildID())
					{
						uiNoticeMan* pUIMan = GET_DIALOG_MANAGER(uiNoticeMan, UIMAN_NOTICE);
						if(pUIMan)
						{
							//3328	%s 지역' 점령전에서 패배하였습니다.\n10초 후 타워 입구로 자동 소환합니다.
							g_InterfaceManager.GetInterfaceString(3328, szTemp);
							Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, szMapName);

							pUIMan->ShowNoticeMessage(eGuild_Notice, szMessage, 10000);
						}
					}
                    uiBattleScoreMan* battle_score_manager_ptr = 
                        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
                    if(battle_score_manager_ptr)
                    {
                        battle_score_manager_ptr->SendPacketBattleResultSYN();
                    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
				}
				break;
			}
		}
		break;		
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::SEND_DOMINATION_NOTICE_SYN(DWORD dwObjectKey)
{
	if (SafePKBool(ePKBOOL_DOMINATION_NOTICE_SYN))
	{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        int find_index = -1;
        if (dwObjectKey != 0)
        {
            find_index = find_domination_object_index_(dwObjectKey);
        }
        domination_object_key_current_index_ = find_index;
#else
		m_dwObjectKey = dwObjectKey;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
		MSG_CG_DOMINATION_NOTICE_SYN sync;
        MAPCODE mapcode = GetDominationManagedMapCode();
        sync.m_MapCode = mapcode;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
		// 패킷 팬딩 셋
		SetPKBool(ePKBOOL_DOMINATION_NOTICE_SYN, TIMEOUT_PACKET_PENDING);
	}	
}

//------------------------------------------------------------------------------
/**
*/
#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
void uiDominationTowerMan::SEND_DOMINATION_APPLY_SYN()
{
	if (SafePKBool(ePKBOOL_DOMINATION_APPLY_SYN))
	{
		MSG_CG_DOMINATION_APPLY_SYN sync;
        MAPCODE mapcode = GetDominationManagedMapCode();
        sync.m_MapCode = mapcode;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
		// 패킷 팬딩 셋
		SetPKBool(ePKBOOL_DOMINATION_APPLY_SYN, TIMEOUT_PACKET_PENDING);
	}		
}
void uiDominationTowerMan::SEND_DOMINATION_BATTING_HEIM_SYN()
{
	if (SafePKBool(ePKBOOL_DOMINATION_BATTING_HEIM_SYN))
	{
		MSG_CG_DOMINATION_BATTING_HEIM_SYN sync;
        MAPCODE mapcode = GetDominationManagedMapCode();
        sync.m_MapCode = mapcode;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
		// 패킷 팬딩 셋
		SetPKBool(ePKBOOL_DOMINATION_BATTING_HEIM_SYN, TIMEOUT_PACKET_PENDING);
	}
}
void uiDominationTowerMan::SEND_DOMINATION_ACCEP_HEIM_SYN(MONEY BettingHeim)
{
    if (SafePKBool(ePKBOOL_DOMINATION_ACCEP_HEIM_SYN))
    {
        MSG_CG_DOMINATION_ACCEP_HEIM_SYN sync;
        MAPCODE mapcode = GetDominationManagedMapCode();
        sync.m_MapCode = mapcode;

        sync.betting_item_count_ = static_cast<BYTE>(BettingHeim);
        sync.m_TokenPos = INVALID_SLOT_POS;  // 인벤에 있는 총 개수이므로 pos는 의미없음. 현재 미사용

        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
        // 패킷 팬딩 셋
        SetPKBool(ePKBOOL_DOMINATION_ACCEP_HEIM_SYN, TIMEOUT_PACKET_PENDING);
    }
}
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
//------------------------------------------------------------------------------
/**
*/
/*
void uiDominationTowerMan::SEND_DOMINATION_ACCEP_HEIM_SYN()
{
    if (item_slot_from_inventory_ == NULL)
    {
        return;
    }
    if (SafePKBool(ePKBOOL_DOMINATION_ACCEP_HEIM_SYN))
    {
        MSG_CG_DOMINATION_ACCEP_HEIM_SYN sync;
        MAPCODE mapcode = GetDominationManagedMapCode();
        sync.m_MapCode = mapcode;

        sync.m_TenderCost = item_slot_from_inventory_->GetNum();
        sync.m_TokenPos = item_slot_from_inventory_->GetPos();
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
        // 패킷 팬딩 셋
        SetPKBool(ePKBOOL_DOMINATION_ACCEP_HEIM_SYN, TIMEOUT_PACKET_PENDING);
    }		
}
*/
//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::SEND_DOMINATION_MANAGE_SYN()
{
	if (SafePKBool(ePKBOOL_DOMINATION_MANAGE_SYN))
	{
		MSG_CG_DOMINATION_MANAGE_SYN sync;
        MAPCODE mapcode = GetDominationManagedMapCode();
        sync.m_MapCode = mapcode;

		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
		// 패킷 팬딩 셋
		SetPKBool(ePKBOOL_DOMINATION_MANAGE_SYN, TIMEOUT_PACKET_PENDING);
	}		
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::SEND_DOMINATION_SAVE_GATESTAT_SYN(BYTE* pDefenseWall)
{
	if (SafePKBool(ePKBOOL_DOMINATION_SAVE_GATESTAT_SYN))
	{
		MSG_CG_DOMINATION_SAVE_GATESTAT_SYN sync;
        MAPCODE mapcode = GetDominationManagedMapCode();
        sync.m_MapCode = mapcode;

		for (int index = 0; index < MAX_DOMINATION_DEFENSEWALL_NUM; ++index)
		{
			sync.m_DefenseWall[index] = pDefenseWall[index];
		}
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
	}	

#ifdef _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL
    is_domination_manage_dummy_packet_ = true;
    SEND_DOMINATION_MANAGE_SYN();
#endif //_TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL
}
//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::SEND_DOMINATION_ADD_DEFFENSEWALL_SYN()
{
    if (SafePKBool(ePKBOOL_DOMINATION_ADD_DEFFENSEWALL_SYN))
    {
        POSTYPE inventory_position = INVALID_POSTYPE_VALUE;
        // 일반 방어블럭 아이템을 먼저 찾고
        BOOL result = ItemManager::Instance()->FindFirstItem(eITEMTYPE_DOMINATION_DEFFENSE_POINT, inventory_position);
        if (result == FALSE) // 없으면
        {   // 캐쉬 방어블럭 아이템을 찾는다.
            result = ItemManager::Instance()->FindFirstItem(eITEMTYPE_DOMINATION_DEFFENSE_CASH_POINT, inventory_position);
        }

        if (result && inventory_position != INVALID_POSTYPE_VALUE) // 일반 혹은 캐쉬 방어블럭이 있으면
        {
            SCSlotContainer* inven_container = g_pHero->GetSlotContainer(SI_INVENTORY);
            if (inven_container)
            {
                SCItemSlot& item_slot = static_cast<SCItemSlot &>(inven_container->GetSlot(inventory_position));
                const BASE_ITEMINFO* item_infomation_ptr = item_slot.GetItemInfo();
                if (item_infomation_ptr)
                {
                    MSG_CG_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN  sync;
                    MAPCODE map_code = GetDominationManagedMapCode();
                    sync.map_code_ = map_code;
                    sync.item_code_ = item_infomation_ptr->m_Code;
                    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
                }
            }
        }
    }		
}
//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::SEND_DOMINATION_MINIMAP_LIST_SYN()
{
	if (SafePKBool(ePKBOOL_DOMINATION_MINIMAP_LIST_SYN))
	{
		MSG_CG_DOMINATION_MINIMAP_LIST_SYN sync;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
		// 패킷 팬딩 셋
		SetPKBool(ePKBOOL_DOMINATION_MINIMAP_LIST_SYN, TIMEOUT_PACKET_PENDING);
	}
}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
void uiDominationTowerMan::SEND_DOMINATION_REFRESH_MEMBERLIST_REQ(MAPCODE map_code)
{
    MSG_CG_DOMINATION_REFRESH_MEMBERLIST_REQ sync;
    sync.map_code_ = map_code;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
}
void uiDominationTowerMan::SEND_DOMINATION_MEMBER_JOIN_SYN(MAPCODE map_code)
{
    if (SafePKBool(ePKBOOL_DOMINATION_MEMBER_JOIN_SYN))
    {
        MSG_CG_DOMINATION_MEMBER_JOIN_SYN sync;
        sync.map_code_ = map_code;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
        // 패킷 팬딩 셋
        SetPKBool(ePKBOOL_DOMINATION_MEMBER_JOIN_SYN, TIMEOUT_PACKET_PENDING);
    }
}
void uiDominationTowerMan::SEND_DOMINATION_MEMBER_LEAVE_SYN(MAPCODE map_code, CHARGUID char_guid)
{
    if (SafePKBool(ePKBOOL_DOMINATION_MEMBER_LEAVE_SYN))
    {
        MSG_CG_DOMINATION_MEMBER_LEAVE_SYN sync;
        sync.map_code_ = map_code;
        sync.memeber_guid_ = char_guid;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
        // 패킷 팬딩 셋
        SetPKBool(ePKBOOL_DOMINATION_MEMBER_LEAVE_SYN, TIMEOUT_PACKET_PENDING);
    }
}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::SEND_LEAVE_DOMINATION()
{
	MAPCODE FieldMapCode = g_pMap->GetCurrentFieldID();
	const sDOMINATION_INFO* pDI = DominationInfoParser::Instance()->GetDominationInfo(FieldMapCode);

	if (pDI)
	{
		WORD wIndex = MapInfoParser::Instance()->GetCurrentMapPortalIndexVer2(pDI->m_DominationMapCode, pDI->m_LinkMapCode);

		MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN sync;
		sync.m_wIndex = wIndex;
		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sync, sizeof(sync));
	}
}


// 패킷 수신 함수
void uiDominationTowerMan::RECV_DOMINATION_NOTICE_ACK(MSG_BASE * pMsg)
{
    UnsetPKBool(ePKBOOL_DOMINATION_NOTICE_SYN);
    MSG_CG_DOMINATION_NOTICE_ACK* pRecvPacket = (MSG_CG_DOMINATION_NOTICE_ACK*)pMsg;
    if ( RC::DOMINATION_RESULT_SUCCESS == pRecvPacket->m_ResultCode )
    {
        DWORD map_mode = GetContinentMapMode();
        if (map_mode == kContinentNormalMode)
        {
            m_pMainDialog->ShowInterface(TRUE);
            m_pMainDialog->SetShowButtonFlag(pRecvPacket);
        }
        else
        {
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
            m_pDominationJoinList2->SetShowButtonFlag(pRecvPacket);

            m_pApplyBetting->ShowInterface(FALSE);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else

  #ifdef _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL
            // 임시패킷 : 성문관리가 가능한 상태면 가패킷으로 성문관리요청을 보내 성문정보를 받아낸다.
            if (pRecvPacket->m_ManageFlag == TRUE)
            {
                is_domination_manage_dummy_packet_ = true;
                SEND_DOMINATION_MANAGE_SYN();
            }
  #else
            // 대륙맵 점령전 관리모드
            SEND_DOMINATION_APPLY_SYN();
  #endif //_TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL

            m_pDominationBettingList->ShowInterface(TRUE);
            m_pDominationBettingList->SetShowButtonFlag(pRecvPacket);

            m_DominationManager->ShowInterface(FALSE);
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
        }

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
        SetChallengerGuildInfo(&(pRecvPacket->m_GuildInfo));
        friend_member_list_order_ = kListOrderNone;
        enemy_member_list_order_ = kListOrderNone;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    }
    else
    {
        ProcessErrorCode(pRecvPacket->m_ResultCode);
    }	
}

#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
void uiDominationTowerMan::RECV_DOMINATION_APPLY_ACK(MSG_BASE * pMsg)
{
    UnsetPKBool(ePKBOOL_DOMINATION_APPLY_SYN);
    MSG_CG_DOMINATION_APPLY_ACK* pRecvPacket = (MSG_CG_DOMINATION_APPLY_ACK*)pMsg;
    if ( RC::DOMINATION_RESULT_SUCCESS == pRecvPacket->m_ResultCode )
    {
        DWORD map_mode = GetContinentMapMode();
        if (map_mode != kContinentNormalMode)
        {
            // 분리된 UI에 리스트 세팅
            m_pDominationBettingList->SetBettingListData(pRecvPacket);
            //m_pDominationBettingList->ShowInterface(TRUE);
        }
        else
        {
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
            m_pBettingList->SetBettingListData(pRecvPacket->m_ActionList, pRecvPacket->m_DataNum);
            m_pBettingList->SetBettingButton(pRecvPacket->m_CheckBatting);
            if (FALSE == m_pBettingList->IsVisibleWindow())
            {
                m_pBettingList->ShowInterface(TRUE);
            }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
        }
    }
    else
    {
        ProcessErrorCode(pRecvPacket->m_ResultCode);
    }
}
void uiDominationTowerMan::RECV_DOMINATION_BATTING_HEIM_ACK(MSG_BASE * pMsg)
{
    UnsetPKBool(ePKBOOL_DOMINATION_BATTING_HEIM_SYN);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    MSG_CG_DOMINATION_BATTING_HEIM_ACK* pRecvPacket = (MSG_CG_DOMINATION_BATTING_HEIM_ACK*)pMsg;
    switch (pRecvPacket->m_ResultCode)
    {
    case RC::DOMINATION_RESULT_SUCCESS:
        {
            m_OwnHeim = pRecvPacket->m_RemainCost;
            m_pApplyBetting->ShowInterface(TRUE);
        }
        break;
    case RC::DOMINATION_RESULT_OVERAP_APPLY:
        {
            m_OwnHeim = pRecvPacket->m_RemainCost;
            m_pApplyBetting2->ShowInterface(TRUE);
        }
        break;

    default:
        {
            ProcessErrorCode(pRecvPacket->m_ResultCode);
        }
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}
void uiDominationTowerMan::RECV_DOMINATION_ACCEP_HEIM_ACK(MSG_BASE * pMsg)
{
    UnsetPKBool(ePKBOOL_DOMINATION_ACCEP_HEIM_SYN);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    MSG_CG_DOMINATION_ACCEP_HEIM_ACK* pRecvPacket = (MSG_CG_DOMINATION_ACCEP_HEIM_ACK*)pMsg;
    if ( RC::DOMINATION_RESULT_SUCCESS == pRecvPacket->m_ResultCode )
    {				

        m_pApplyBetting->ShowInterface(FALSE);

        if (g_pHero)
        {
            TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR notice_message[INTERFACE_STRING_LENGTH] = {0,};

            _itot(pRecvPacket->betting_item_count_, temp_message, 10);

            // 7396 해당 지역에 왕의증표 %s개 입찰이 정상적으로 등록되었습니다
            g_InterfaceManager.GetInterfaceStringFormat(
                notice_message,
                INTERFACE_STRING_LENGTH,
                7396,
                temp_message);

            GlobalFunc::default_OutputMsg(eOUTPUT_MSG_CENTER_SHOW | eOUTPUT_MSG_CHAT, notice_message);
        }												

    }
    else
    {
        ProcessErrorCode(pRecvPacket->m_ResultCode);
    }

    SEND_DOMINATION_APPLY_SYN();
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL

void uiDominationTowerMan::RECV_DOMINATION_MANAGE_ACK(MSG_BASE * pMsg)
{
    UnsetPKBool(ePKBOOL_DOMINATION_MANAGE_SYN);
    MSG_CG_DOMINATION_MANAGE_ACK* pRecvPacket = (MSG_CG_DOMINATION_MANAGE_ACK*)pMsg;
    if ( RC::DOMINATION_RESULT_SUCCESS == pRecvPacket->m_ResultCode )
    {
#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
  #ifdef _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL
        if (is_domination_manage_dummy_packet_ == true)
        {
            is_domination_manage_dummy_packet_ = false;
            m_pDominationBettingList->SetManageListData(pRecvPacket);
        }
        else
        {
            m_DominationManager->ShowInterface(TRUE);
            m_DominationManager->SetDefenseWall(pRecvPacket->m_TotalPoint, pRecvPacket->m_DefenseWall);						
        }
  #else
        m_DominationManager->ShowInterface(TRUE);
        m_DominationManager->SetDefenseWall(pRecvPacket->m_TotalPoint, pRecvPacket->m_DefenseWall);
  #endif //_TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
    }
    else
    {
        ProcessErrorCode(pRecvPacket->m_ResultCode);
    }
}

void uiDominationTowerMan::RECV_DOMINATION_SAVE_GATESTAT_ACK(MSG_BASE * pMsg)
{
    UnsetPKBool(ePKBOOL_DOMINATION_SAVE_GATESTAT_SYN);
    MSG_CG_DOMINATION_SAVE_GATESTAT_ACK* pRecvPacket = (MSG_CG_DOMINATION_SAVE_GATESTAT_ACK*)pMsg;					
    if ( RC::DOMINATION_RESULT_SUCCESS == pRecvPacket->m_ResultCode )
    {						
        // 3334	방어벽 세팅이 완료되었습니다.
        TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0, };
        g_InterfaceManager.GetInterfaceString(3334, szMessage);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
    else
    {
        ProcessErrorCode(pRecvPacket->m_ResultCode);
    }

#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
    m_DominationManager->ShowInterface(FALSE);
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
}

void uiDominationTowerMan::RECV_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK(MSG_BASE* pMsg)
{
    UnsetPKBool(ePKBOOL_DOMINATION_ADD_DEFFENSEWALL_SYN);
    MSG_CG_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK* recv_packet = (MSG_CG_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK*)pMsg;					
    RC::eDOMINATION_RESULT result_code = recv_packet->result_code_;
    if (RC::DOMINATION_RESULT_SUCCESS == result_code)
    {						
        // 이펙트(깜빡 깜빡)
#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
        if (m_DominationManager)
        {
            m_DominationManager->SetAnimation();
        }
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
    }
    else
    {
        ProcessErrorCode(result_code);
    }
}

void uiDominationTowerMan::RECV_DOMINATION_MINIMAP_LIST_ACK(MSG_BASE * pMsg)
{
    UnsetPKBool(ePKBOOL_DOMINATION_MINIMAP_LIST_SYN);
    MSG_CG_DOMINATION_MINIMAP_LIST_ACK* pRecvPacket = (MSG_CG_DOMINATION_MINIMAP_LIST_ACK*)pMsg;
    if ( RC::DOMINATION_RESULT_SUCCESS == pRecvPacket->m_ResultCode )
    {	
        m_mapDominatioGuild.clear();
        PAIR_DOMINATIONGUILD dominationguild;
        for (int Index = 0; Index < pRecvPacket->m_NumDominationGuild; ++Index)
        {
            dominationguild = PAIR_DOMINATIONGUILD(pRecvPacket->m_GuildInfo[Index].m_MapCode, pRecvPacket->m_GuildInfo[Index]);
            dominationguild.second.m_tszGuildName[MAX_GUILDNAME_LENGTH] = '\0';
            dominationguild.second.m_tszGuildMaster[MAX_GUILDNAME_LENGTH] = '\0';
            m_mapDominatioGuild.insert(dominationguild);
        }
        m_bShowDominationGuildInfo = true;
    }
    else
    {
        ProcessErrorCode(pRecvPacket->m_ResultCode);
    }
}

void uiDominationTowerMan::RECV_DOMINATION_START_NOTIFY_BRD(MSG_BASE * pMsg)
{
    uiNoticeMan* pUIMan = GET_DIALOG_MANAGER(uiNoticeMan, UIMAN_NOTICE);
    if (pUIMan == NULL)
    {
        return;
    }

    MSG_CG_DOMINATION_START_NOTIFY_BRD* pRecvPacket = (MSG_CG_DOMINATION_START_NOTIFY_BRD*)pMsg;

    switch (pRecvPacket->notify_type)
    {
    case DOMINATION_WAR_TIME:
        {
            if(0 != pRecvPacket->m_byTime)
            {
                TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
                TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
                if (pRecvPacket->m_domination_type == DOMINATION_TYPE_MONDSCHIEN)
                {
                    // 3479	%d 분 후에 몬트샤인 요새 공방전이 시작될 예정입니다.\n해당 지역에 입장하시기 바랍니다.
                    g_InterfaceManager.GetInterfaceString(3479, szTemp);
                }
                else
                {
                    //3318	%s 분 후에 점령전이 시작될 예정입니다.\n해당 지역 타워에 입장하시기 바랍니다.
                    g_InterfaceManager.GetInterfaceString(3318, szTemp);
                }
#else
                //3318	%s 분 후에 점령전이 시작될 예정입니다.\n해당 지역 타워에 입장하시기 바랍니다.
                g_InterfaceManager.GetInterfaceString(3318, szTemp);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
                Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, pRecvPacket->m_byTime);

                pUIMan->ShowNoticeMessage(eGuild_Notice, szMessage, 10000);
            }
            else if (0 == pRecvPacket->m_byTime && SCENE_TYPE_FIELD == GameFramework::GetCurScene())
            {
                TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
                if (pRecvPacket->m_domination_type == DOMINATION_TYPE_MONDSCHIEN)
                {
                    // 3485 몬트샤인 요새 공방전'이 시작되었습니다.
                    g_InterfaceManager.GetInterfaceString(3485, szMessage);
                }
                else
                {
                    //3376	피의 성전' 점령전이 시작되었습니다.
                    g_InterfaceManager.GetInterfaceString(3376, szMessage);
                }
#else
                //3376	피의 성전' 점령전이 시작되었습니다.
                g_InterfaceManager.GetInterfaceString(3376, szMessage);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

                pUIMan->ShowNoticeMessage(eGuild_Notice, szMessage, 10000);
            }
        }
        break;
    case DOMINATION_JOIN_TIME:
        {
            if(0 != pRecvPacket->m_byTime)
            {
                // 3460 %d분 후 피의성전 출전 신청이 마감됩니다. 출전 신청을 완료해 주시기를 바랍니다
                TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
                TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
                g_InterfaceManager.GetInterfaceString(3460, szTemp);
                Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, pRecvPacket->m_byTime);

                pUIMan->ShowNoticeMessage(eGuild_Notice, szMessage, 10000);
            }            
        }
        break;
    }

}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
void uiDominationTowerMan::RECV_DOMINATION_DEFENSE_GUILD_ALLIENCE_LIST_BRD(MSG_BASE* pMsg)
{
    MSG_CG_DOMINATION_DEFENSE_GUILD_ALLIENCE_LIST_BRD* recv_packet = (MSG_CG_DOMINATION_DEFENSE_GUILD_ALLIENCE_LIST_BRD*)pMsg;

    uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (ui_domination_manager == NULL)
    {
        return;
    }

    ui_domination_manager->defence_guild_alliance_container().clear();

    // 수성 연합 리스트 받음
    for (int i = 0; i < MAX_ALLIENCE_GUILD_NUM; ++i)
    {
        if (recv_packet->allience_list[i] == 0)
        {
            break;
        }

        ui_domination_manager->defence_guild_alliance_container().push_back(recv_packet->allience_list[i]);
    }

    // 전체 플레이어들의 길드관계를 리셋
    std::vector<Player*> vector_players;
    g_ObjectManager.GetPlayers(vector_players);
    int count = vector_players.size();
    for (int i = 0; i < count; ++i)
    {
        Player* player = vector_players[i];
        if (player)
        {
            player->CheckDominationRelation();
        }
    }
    vector_players.clear();
}
#else
void uiDominationTowerMan::RECV_DOMINATION_MEMBERLIST_NTF(MSG_BASE* pMsg)
{
    MSG_CG_DOMINATION_MEMBERLIST_NTF* recv_packet = (MSG_CG_DOMINATION_MEMBERLIST_NTF*)pMsg;

    // 참여 멤버 리스트세팅 및 UI Show
    // 2가지 UI 중에 한곳에 세팅
    DWORD map_mode = GetContinentMapMode();
    if (map_mode == kContinentNormalMode)
    {
        // 리스트1에 세팅
        m_pDominationJoinList1->ShowInterface(TRUE);
        m_pDominationJoinList1->SetDominationJoinList(recv_packet);
    }
    else
    {
        // 리스트2에 세팅
        //m_pDominationJoinList2->ShowInterface(TRUE);
        m_pDominationJoinList2->SetDominationJoinList(recv_packet);
    }
}

void uiDominationTowerMan::RECV_DOMINATION_MEMBER_JOIN_ACK(MSG_BASE* pMsg)
{
    UnsetPKBool(ePKBOOL_DOMINATION_MEMBER_JOIN_SYN);
    MSG_CG_DOMINATION_MEMBER_JOIN_ACK* recv_packet = (MSG_CG_DOMINATION_MEMBER_JOIN_ACK*)pMsg;

    if (recv_packet->result_code_ == DOMINATION_RESULT_SUCCESS)
    {
        DWORD continent_mode = GetContinentMapMode();
        if ((continent_mode == kContinentNormalMode) && 
            (m_pMainDialog != NULL))
        {   
            m_pMainDialog->RECV_DOMINATION_MEMBER_JOIN_ACK(recv_packet);
        }
        else if ((continent_mode == kContinentDominationManagerMode) ||
            (continent_mode == kContinentDominationManagerSelectedAreaMode))
        {
            if (m_pDominationJoinList2 != NULL)
            {
                m_pDominationJoinList2->RECV_DOMINATION_MEMBER_JOIN_ACK(recv_packet);
            }
        }
    }
    else
    {
        ProcessErrorCode(recv_packet->result_code_);
    }
}

void uiDominationTowerMan::RECV_DOMINATION_MEMBER_LEAVE_ACK(MSG_BASE* pMsg)
{
    UnsetPKBool(ePKBOOL_DOMINATION_MEMBER_LEAVE_SYN);
    MSG_CG_DOMINATION_MEMBER_LEAVE_ACK* recv_packet = (MSG_CG_DOMINATION_MEMBER_LEAVE_ACK*)pMsg;

    if (recv_packet->result_code_ == DOMINATION_RESULT_SUCCESS)
    {
        DWORD continent_mode = GetContinentMapMode();
        if ((continent_mode == kContinentNormalMode) && 
            (m_pMainDialog != NULL))
        {   
            m_pMainDialog->RECV_DOMINATION_MEMBER_LEAVE_ACK(recv_packet);
        }
        else if ((continent_mode == kContinentDominationManagerMode) ||
                 (continent_mode == kContinentDominationManagerSelectedAreaMode))
        {
            if (m_pDominationJoinList2 != NULL)
            {
                m_pDominationJoinList2->RECV_DOMINATION_MEMBER_LEAVE_ACK(recv_packet);
            }
        }
    }
    else
    {
        ProcessErrorCode(recv_packet->result_code_);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::SetChallengerGuildInfo(DOMINATION_GUILD_INFO* pGuildInfo)
{
    memset(&m_ChallengerGuildInfo,0,sizeof(m_ChallengerGuildInfo));
    if (pGuildInfo->m_GuildGuid)
    {
        memcpy(&m_ChallengerGuildInfo, pGuildInfo, sizeof(m_ChallengerGuildInfo));			
        m_ChallengerGuildInfo.m_tszGuildName[MAX_CHARNAME_LENGTH] = '\0';
        m_ChallengerGuildInfo.m_tszGuildMaster[MAX_CHARNAME_LENGTH] = '\0';        
    }	
}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::SetDominationGuildInfo(DOMINATION_GUILD_INFO* pGuildInfo)
{
    memset(&m_DominationGuildInfo,0,sizeof(m_DominationGuildInfo));
    if ((pGuildInfo != NULL) && (pGuildInfo->m_GuildGuid))
    {
        SCENE_TYPE sceneType = GameFramework::GetCurScene();
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        if ((sceneType == SCENE_TYPE_FIELD) || (sceneType == SCENE_TYPE_VILLAGE) || (sceneType == SCENE_TYPE_DOMINATION)
  #ifdef _NA_008334_20150608_SONNENSCHEIN
            || (sceneType == SCENE_TYPE_SONNENSCHEIN)
  #endif //_NA_008334_20150608_SONNENSCHEIN
            )
#else
        if ((sceneType == SCENE_TYPE_FIELD) || (sceneType == SCENE_TYPE_DOMINATION))
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
        {
            memcpy(&m_DominationGuildInfo, pGuildInfo, sizeof(m_DominationGuildInfo));			
            m_DominationGuildInfo.m_tszGuildName[MAX_CHARNAME_LENGTH] = '\0';
            m_DominationGuildInfo.m_tszGuildMaster[MAX_CHARNAME_LENGTH] = '\0';        
        }	
    }
    // 필드입장시 점령 길드 출력
    m_DominationArea->SetGuildName(m_DominationGuildInfo.m_tszGuildName);
}
//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMan::ProcessErrorCode(DWORD dwErrorcode)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
	DWORD dwStringcode = 70017;
	switch (dwErrorcode)
	{
	case RC::DOMINATION_RESULT_FAIL:
	case RC::DOMINATION_RESULT_DBERROR:
		// 70017	잠시 후 다시 시도 하십시오.
		dwStringcode = 70017;
		break;
	case RC::DOMINATION_RESULT_NOT_ENOUGHMONEY:
        {
            // 3383 소지금이 부족하여 피의성전 점령전의 참여 신청을 할 수 없습니다.
            dwStringcode = 3383;
        }
        break;
	case RC::DOMINATION_RESULT_LIMIT_ENOUGHMONEY:
		dwStringcode = 3348;
		break;
	case RC::DOMINATION_RESULT_BEST_ENOUGHMONEY:
		dwStringcode = 3349;
		break;
	case RC::DOMINATION_RESULT_DEFFENSEWALL_OVERPOINT:
		dwStringcode = 3350;
		break;
	case RC::DOMINATION_RESULT_USER_LOCATION_FAIL:
		dwStringcode = 3351;
		break;
	case RC::DOMINATION_RESULT_NOT_GUILDMASTER:
		dwStringcode = 3346;
		break;
	case RC::DOMINATION_RESULT_GUILDSTAT_DESTROYWAIT:
		dwStringcode = 6236;
		break;
	case RC::DOMINATION_RESULT_NOT_GUILDMEMBER:
		dwStringcode = 3346;
		break;
    case RC::DOMINATION_RESULT_LESS_TOKEN:
        // 3384 왕의 증표 아이템을 최소 1개 이상 등록하셔야 신청하실 수 있습니다.
        dwStringcode = 3384;
        break;
    case RC::DOMINATION_RESULT_CAN_JOIN_ONLYONE_AREA:
    case RC::DOMINATION_RESULT_CAN_BETTING_ONLYONE_AREA:
        // 3451 피의성전 점령전 신청은 한 지역만 가능합니다.
        dwStringcode = 3451;
        break;
#ifdef _DEV_VER
	case RC::DOMINATION_RESULT_GUILDLEVEL_FAIL:
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, "[DEBUG] Domination ErrorCode[%d] - DOMINATION_RESULT_GUILDLEVEL_FAIL", dwErrorcode);
		break;	
#endif	
	default:
#ifdef _DEV_VER
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, "[DEBUG] Undefined Domination ErrorCode[%d]", dwErrorcode);
#endif		
		return;
	}

#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
    GlobalFunc::VerifyMessage(dwStringcode);
#else
    g_InterfaceManager.GetInterfaceString( dwStringcode, szMessage, INTERFACE_STRING_LENGTH );
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL	
}

void uiDominationTowerMan::SetDominationObject(DWORD dwObjectKey)
{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    domination_object_key_vector_.push_back(dwObjectKey);
#else
	m_dwObjectKey = dwObjectKey;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	bool bShow = false;
	MAPCODE FieldMapCode = g_pMap->GetCurrentFieldID();
	const sDOMINATION_INFO* pDI = DominationInfoParser::Instance()->GetDominationInfo(FieldMapCode);
	uiLoginMan* pLoginMan = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);
	if (pLoginMan)
	{
		AuthSetverData* pServerData = pLoginMan->GetCurrentServerData();
		bShow = pDI && pServerData && pServerData->m_ChannelID == PROGRAMPARAM->GetDominationChannel();
	}
	m_bShowDominationObject = bShow;
}

void uiDominationTowerMan::RenderGuildMark(CControlWZ* pControl, DOMINATION_GUILD_INFO* pGuildInfo, BYTE byAlpha)
{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    HANDLE hGuildMarkTexture = GuildSystem::GetGuildSystemData().GetGuildMarkTexture();
#else
    // 길드마크 세팅
    uiGuildMan* pUIGuildMan = GET_DIALOG_MANAGER(uiGuildMan, UIMAN_GUILD);
    if (pUIGuildMan == NULL)
    {
        return;
    }

    HANDLE hGuildMarkTexture = pUIGuildMan->GetGuildMarkTexture();
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER	

    //점령 길드 마크 출력
    if (hGuildMarkTexture != INVALID_HANDLE_VALUE)
    {
        ENUM_ALPHABLEND eOldAlphaMode =	g_pSunRenderer->GetAlphaBlendMode();
        BASE_GuildMarkInfo* pMarkInfo = NULL;

        RECT rtSize = pControl->GetSizeRect();

        float fWidth = (float) rtSize.right - rtSize.left;
        float fHeight = (float) rtSize.bottom - rtSize.top;
        g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);

        WzColor Color[4] = {WzColor_RGBA(255,255,255,byAlpha)
            , WzColor_RGBA(255,255,255,byAlpha)
            , WzColor_RGBA(255,255,255,byAlpha)
            , WzColor_RGBA(255,255,255,byAlpha)};

        for(int i =0; i < 2; ++i)
        {
            if( i == 0)
                pMarkInfo =  GuildMarkInfoParser::Instance()->GetGuildMarkInfo(pGuildInfo->m_BackGroundIndex);
            else
                pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(pGuildInfo->m_PatternIndex);

            if( pMarkInfo )
            {
                g_pSunRenderer->RenderTexture(hGuildMarkTexture, (float)rtSize.left , (float)rtSize.top, fWidth , fHeight,
                    (float)pMarkInfo->iRow * 32.0f , (float) pMarkInfo->iColumn * 32.0f, 32.0f, 32.0f, Color);
            }
        }
        g_pSunRenderer->SetAlphaBlendMode(eOldAlphaMode);
    }
}

void uiDominationTowerMan::RenderGuildMark(CControlWZ* pControl, DOMINATION_GUILD_INFO* pGuildInfo, BYTE byAlpha, bool bTooltipGuildName, DWORD TooltipParentDialogKey)
{
    uiDominationTowerMan::RenderGuildMark(pControl, pGuildInfo, byAlpha);
    if ((bTooltipGuildName == true) && (pControl != NULL))
    {
        int iMouseX = Mouse::Instance()->GetMouseX();
        int iMouseY = Mouse::Instance()->GetMouseY();
        RECT ControlRect = pControl->GetSizeRect();
        if ((ControlRect.left <= iMouseX) && 
            (ControlRect.right >= iMouseX) &&
            (ControlRect.top <= iMouseY) && 
            (ControlRect.bottom >= iMouseY))
        {
            uiToolTipMan* pTooltipMan = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
            if (pTooltipMan != NULL)
            {
                pTooltipMan->RegisterTooltip(pGuildInfo->m_tszGuildName);
				pTooltipMan->SetTooltipDialogID(TooltipParentDialogKey);
            }
        }        
    }
}

void uiDominationTowerMan::HideDominationManagmentDialog()
{
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    uiBase* join_list = GET_DIALOG(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_JOIN_LIST2);
    if (join_list != NULL)
    {
        join_list->ShowInterface(FALSE);
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

    uiBase* betting_list = GET_DIALOG(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST);
    if (betting_list != NULL)
    {
        betting_list->ShowInterface(FALSE);
    }
}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
bool uiDominationTowerMan::IsDefenceGuildAlliance(GUILDGUID target_guid)
{
    // 수성측 리스트에 있는 길드인지
    STLX_VECTOR<GUILDGUID>::iterator itr_point = defence_guild_alliance_container().begin();
    STLX_VECTOR<GUILDGUID>::iterator end_itr = defence_guild_alliance_container().end();
    for ( ; itr_point != end_itr; ++itr_point)
    {
        GUILDGUID& guild_guid = (*itr_point);
        if (guild_guid == target_guid)
        {
            return true;
        }
    }
    return false;
}
void uiDominationTowerMan::ComfirmDominationEnter(bool is_enter)
{
    if (is_enter == true)
    {
        MAPCODE FieldMapCode = g_pMap->GetCurrentFieldID();
        MAPCODE DominationMapCode = DominationInfoParser::Instance()->GetDominationFieldCode(FieldMapCode);
        WORD wIndex = MapInfoParser::Instance()->GetCurrentMapPortalIndexVer2(FieldMapCode, DominationMapCode);

        MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN sync;
        sync.m_wIndex = wIndex;
        GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sync, sizeof(sync));
    }
}
#else
void uiDominationTowerMan::ComfirmDominationJoin(bool is_join)
{
    if (is_join == true)
    {
        uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
        if (ui_domination_manager != NULL)
        {
            MAPCODE map_code = ui_domination_manager->GetDominationManagedMapCode();
            ui_domination_manager->SEND_DOMINATION_MEMBER_JOIN_SYN(map_code);
        }        
    }
}

void uiDominationTowerMan::ComfirmDominationLeave(bool is_leave)
{
    if (is_leave == true)
    {
        uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
        if (ui_domination_manager != NULL)
        {
            MAPCODE map_code = ui_domination_manager->GetDominationManagedMapCode();
            CHARGUID char_guid = ui_domination_manager->leave_member_info().member_guid_;
            ui_domination_manager->SEND_DOMINATION_MEMBER_LEAVE_SYN(map_code, char_guid);
        }        
    }
}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

bool uiDominationTowerMan::SortMemberListByClassLess(const DominationMemberInfo& member_info_ref_1, 
                                                     DominationMemberInfo& member_info_ref2)
{
    return member_info_ref_1.member_class_ < member_info_ref2.member_class_;
}

bool uiDominationTowerMan::SortMemberListByClassGreater(const DominationMemberInfo& member_info_ref_1, 
                                                        DominationMemberInfo& member_info_ref2)
{
    return member_info_ref_1.member_class_ > member_info_ref2.member_class_;
}

bool uiDominationTowerMan::SortMemberListByLevelLess(const DominationMemberInfo& member_info_ref_1, 
                                                     DominationMemberInfo& member_info_ref2)
{
    return member_info_ref_1.member_level_ < member_info_ref2.member_level_;
}

bool uiDominationTowerMan::SortMemberListByLevelGreater(const DominationMemberInfo& member_info_ref_1, 
                                                        DominationMemberInfo& member_info_ref2)
{
    return member_info_ref_1.member_level_ > member_info_ref2.member_level_;
}

bool uiDominationTowerMan::SortMemberListByNameLess(const DominationMemberInfo& member_info_ref_1, 
                                                    DominationMemberInfo& member_info_ref2)
{
    return Strcmp(member_info_ref_1.member_name_, member_info_ref2.member_name_) < 0;
}

bool uiDominationTowerMan::SortMemberListByNameGreater(const DominationMemberInfo& member_info_ref_1, 
                                                       DominationMemberInfo& member_info_ref2)
{
    return Strcmp(member_info_ref_1.member_name_, member_info_ref2.member_name_) > 0;
}


void uiDominationTowerMan::RefreshZoneMarkInfo()
{
    if (g_pHero == NULL)
    {
        return;
    }

    uiDominationZoneMarkDlg* zonemark_dialog = GET_CAST_DIALOG(uiDominationZoneMarkDlg, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_ZONEMARK_DIALOG);
    if (zonemark_dialog)
    {
        zonemark_dialog->SetZoneMarkInfo(g_pHero->GetDominationStateInfo()
#ifdef _NA_008334_20150608_SONNENSCHEIN
            , GuildSystem::GetGuildSystemData().get_sonnenschein_phase()
#endif //_NA_008334_20150608_SONNENSCHEIN
            );
        zonemark_dialog->ShowInterface(TRUE);
        g_pHero->UpdateSkillAttr();
    }
}



DWORD uiDominationTowerMan::GetContinentMapMode()
{
    ContinentMapDlg* continent_map_dialog = 
        GET_CAST_DIALOG(ContinentMapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_DLG);
    if ((continent_map_dialog != NULL) && 
        (continent_map_dialog->IsVisible() == TRUE))
    {
        return continent_map_dialog->continent_map_mode();
    }
    ContinentEp2MapDlg* continent_map_dialog_ep2 = 
        GET_CAST_DIALOG(ContinentEp2MapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG);
    if ((continent_map_dialog_ep2 != NULL) && 
        (continent_map_dialog_ep2->IsVisible() == TRUE))
    {
        return continent_map_dialog_ep2->continent_map_mode();
    }
    return (kContinentNormalMode);
}

MAPCODE uiDominationTowerMan::GetDominationManagedMapCode()
{
    MAPCODE mapcode = 0;
    ContinentMapDlg* continent_map_dialog = 
        GET_CAST_DIALOG(ContinentMapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_DLG);
    if ((continent_map_dialog != NULL) && 
        (continent_map_dialog->IsVisible() == TRUE))
    {
        ContinentMapMode map_mode = continent_map_dialog->continent_map_mode();
        if (map_mode == kContinentDominationManagerSelectedAreaMode)
        {
            mapcode = continent_map_dialog->GetDominationManagedMapcode();
            return mapcode;
        }
    }
    ContinentEp2MapDlg* continent_map_dialog_ep2 = 
        GET_CAST_DIALOG(ContinentEp2MapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG);
    if ((continent_map_dialog_ep2 != NULL) && 
        (continent_map_dialog_ep2->IsVisible() == TRUE))
    {
        ContinentMapMode map_mode = continent_map_dialog_ep2->continent_map_mode();
        if (map_mode == kContinentDominationManagerSelectedAreaMode)
        {
            mapcode = continent_map_dialog_ep2->GetDominationManagedMapcode();
            return mapcode;
        }
    }

    if (g_pMap != NULL)
    {
        MAPCODE current_mapcode = g_pMap->GetCurrentFieldID();
        const sDOMINATION_INFO* domination_info = 
            DominationInfoParser::Instance()->GetDominationInfo(current_mapcode);
        if (domination_info != NULL)
        {
            mapcode = domination_info->m_DominationMapCode;
            return mapcode;
        }
    }
    return mapcode;
}

#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
void uiDominationTowerMan::ShowDominationResurrectionPopup()
{
    if (ui_domination_resurrection_ == NULL)
    {
        return;
    }

    ui_domination_resurrection_->ShowInterface(TRUE);
}
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
DWORD uiDominationTowerMan::GetDominationObject()
{
    if (domination_object_key_current_index_ < 0 && 
        static_cast<unsigned int>(domination_object_key_current_index_) >= domination_object_key_vector_.size())
        return 0;

    return domination_object_key_vector_[domination_object_key_current_index_];
}

int uiDominationTowerMan::find_domination_object_index_( const DWORD& object_key )
{
    if (object_key == 0 || 
        domination_object_key_vector_.empty())
        return -1;
    int find_index = 0;
    std::vector<DWORD>::iterator iter = domination_object_key_vector_.begin();
    for ( ; iter != domination_object_key_vector_.end(); ++iter)
    {
        if ((*iter) == object_key)
        {
            return find_index;
        }
        ++find_index;
    }
    // 여기까지 왔다는 것은 인덱스가 없다는 뜻이다.
    return -1;
}

void uiDominationTowerMan::set_domination_object_show( const BOOL& is_show )
{
    std::vector<DWORD>::iterator iter = domination_object_key_vector_.begin();
    for ( ; iter != domination_object_key_vector_.end(); ++iter)
    {
        Object* object_ptr = g_ObjectManager.GetObject((*iter));
        if (object_ptr)
        {
            object_ptr->SetShow(is_show);
        }
    }
}

void uiDominationTowerMan::domination_object_key_relation_clear()
{
    // 벡터와 현재 인덱스 초기화
    domination_object_key_vector_.clear();
    domination_object_key_current_index_ = -1;
}

void uiDominationTowerMan::set_mondschien_guild_name( TCHAR* mondschien_guild_name )
{    
    ZeroMemory(mondschien_guild_name_, MAX_GUILDNAME_LENGTH+1);
    if (NULL == mondschien_guild_name)
    {
        _tcscpy(mondschien_guild_name_, "");
    }
    else
    {
        _tcsncpy(mondschien_guild_name_, mondschien_guild_name, lstrlen(mondschien_guild_name));
    }
}

TCHAR* uiDominationTowerMan::get_mondschien_guild_name()
{
    return mondschien_guild_name_;
}

void uiDominationTowerMan::ShowDominationAreaOwnerDialog( const BOOL& is_show )
{
    if (is_show == TRUE && _tcscmp(mondschien_guild_name_, "") != 0)
    {
        if (ui_domination_owner_tutorial_dialog_)
        {
            ui_domination_owner_tutorial_dialog_->OnShowWindow(is_show);
        }
    }
    else
    {
        ui_domination_owner_tutorial_dialog_->OnShowWindow(is_show);
    }
}

#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
void uiDominationTowerMan::set_domination_area_owner_guild_name_tutorial( TCHAR* area_owner_guild_name, const sDOMINATION_CONTINENT_LORD_ALLIENCE_INFO* allience_guild_infos, const BYTE& allience_guild_count )
{
    if (area_owner_guild_name == NULL)
        return;
    if (_tcscmp(area_owner_guild_name, "") == 0)
        return;

    ui_domination_owner_tutorial_dialog_->SetAreaOwnerGuildName(area_owner_guild_name, allience_guild_infos, allience_guild_count);
    set_is_character_enter_(TRUE);
}
#else
void uiDominationTowerMan::set_domination_area_owner_guild_name_tutorial( TCHAR* area_owner_guild_name )
{
    if (area_owner_guild_name == NULL)
        return;
    if (_tcscmp(area_owner_guild_name, "") == 0)
        return;

    ui_domination_owner_tutorial_dialog_->SetAreaOwnerGuildName(area_owner_guild_name);
    set_is_character_enter_(TRUE);
}
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
//------------------------------------------------------------------------------
/**
*/
