#include "SunClientPrecompiledHeader.h"
#include "CompetitionScene.h"
#include "Map.h"
#include "BGMSoundInfoParser.h"
#include "Hero.h"
#include "uiBattleZoneDoing/uiBattleZoneDoing.h"
#include "uiPartyMan/uiPartyMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include <PacketControl/WarMessages_Sync.h>
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
#include "uibattlezoneman/uiBattleZoneMan.h"
#endif
#include "uiRadarMan/uiRadarMan.h"

#include "uiBattleScoreMan\uiBattleScoreMan.h"

CompetitionScene		g_CompetitionScene;
const int c_iRightGap = 10;

#ifdef _INTERNATIONAL_UI
	const WzID c_Font	= StrToWzID("sn26");
#else
	const WzID c_Font	= StrToWzID("BL24");	
#endif//_INTERNATIONAL_UI

CompetitionScene::CompetitionScene(void)
{
	m_SceneType = SCENE_TYPE_CHUNTING;
    m_statusInfo.clear();
    m_winInfo.clear();

	m_wMyTeamKey = -1;
}

CompetitionScene::~CompetitionScene(void)
{
}

BOOL CompetitionScene::Init( void )
{
	BattleScene::Init();

	if ( g_pMap )
	{
		GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_CHUNTING );
	}

	ZeroMemory(&m_baseRect,sizeof(m_baseRect));

    m_bDeathMessage = false;
	ResetProcessDeathMessage();

    m_winInfo.clear();
    m_statusInfo.clear();
    uiPartyMan* partyMan =
        static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
    if (partyMan)
    {
        partyMan->uiShowAll();
    }

    g_KeyQueueManager.Reset();
    g_KeyQueueManager.SetKey(DIK_ESCAPE);
    g_KeyQueueManager.SetDefaultFun(GlobalFunc::DoSystemDialog);

	uiBattleScoreMan* battle_score_manager_ptr = 
        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
	if (battle_score_manager_ptr && 
		(battle_score_manager_ptr->GetIsInit() == false))
	{
		battle_score_manager_ptr->SetMode(uiBattleScoreMan::eBTS_HUNTING);
	}		

    return TRUE;
}

void CompetitionScene::ProcessKeyInput( DWORD dwTick )
{
}
 
BOOL CompetitionScene::Process( DWORD dwTick )
{
    BOOL bRet = BattleScene::Process( dwTick );
	ProcessKeyInput(dwTick);

	SendHeroSectorIndex();

    switch (m_statusInfo.m_status)
    {
    case eCOMPETITION_SCENE_START_READY:
        {
        }
        break;

    case eCOMPETITION_SCENE_START:
        {
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, _T("Competition Start !!!"));
            m_statusInfo.m_status = eCOMPETITION_SCENE_COMPETITON;

			uiBase * pDialog = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_DOING );

			if( pDialog )
			{
				pDialog->ShowWindow(TRUE);
			}
        }
        break;
    }

    if (g_pHero)
    {
        BattleRoomEntry* entry = BattleScene::GetPendingBattleRoomEntry();

        if (entry)
        {
            const ADDITIONAL_CHUNTINGINFO* info = entry->GetAdditionalCHuntingInfo(NULL);

            if (info)
            {
                if (info->m_Mode == eCOMPETITION_MODE_DEATH_MATCH)
                {
					if ((m_statusInfo.m_status == eCOMPETITION_SCENE_START) ||
                        (m_statusInfo.m_status == eCOMPETITION_SCENE_COMPETITON))
					{
						// ������ ������: �׳� ��������
						// �����ϸ�: ������ġ���� �ڵ���Ȱ			
					}
					else
					{
						if(m_statusInfo.m_renderstatus != eCOMPETITION_RENDER_LOSE)
						{
							if (!m_ProcessDeathMessage && g_pHero->IsDead())	// ����� ���� ���� ������
							{
								if (!m_bDeathMessage)
								{
									m_bDeathMessage = true;
									this->ConfirmDeadMessageBoxBeforeStart();	// ������ġ��Ȱ/��� ����â
								}
								else
								{
									m_bDeathMessage = false;
								}
							}
						}
					}
                }
                else if (info->m_Mode == eCOMPETITION_MODE_TIME_ATTACK)
                {
					if ((m_statusInfo.m_status == eCOMPETITION_SCENE_START) ||
                        (m_statusInfo.m_status == eCOMPETITION_SCENE_COMPETITON))
					{
						if(m_statusInfo.m_renderstatus != eCOMPETITION_RENDER_LOSE)
						{
							if (!m_ProcessDeathMessage && g_pHero->IsDead())
							{
								if (!m_bDeathMessage)
								{
									m_bDeathMessage = true;
									this->ConfirmDeadMessageBoxBeforeStart();	// ������ġ��Ȱ/��� ����â
								}
							}
							else
							{
								m_bDeathMessage = false;
							}
						}
					}
					else
					{
						if(m_statusInfo.m_renderstatus != eCOMPETITION_RENDER_LOSE)
						{
							// ����� ���� ���� ������
							if (!m_ProcessDeathMessage && g_pHero->IsDead())
							{
								if (!m_bDeathMessage)
								{
									m_bDeathMessage = true;
									this->ConfirmDeadMessageBoxBeforeStart();	// ������ġ��Ȱ/��� ����â
								}
							}
							else
							{
								m_bDeathMessage = false;
							}
						}
					}
                }
                else if (info->m_Mode == eCOMPETITION_MODE_SURVIVAL)
                {
					if ((m_statusInfo.m_status == eCOMPETITION_SCENE_START) ||
                        (m_statusInfo.m_status == eCOMPETITION_SCENE_COMPETITON))
                    {
						//������ ��쿣 ������ ���Ƿ�, ���ڸ���Ȱ/��Ȱ��� �޽��� ǥ�� ���� ����.
						// ������ �ƴϸ�, �׳� �����־���� : �޽��� ǥ�� ����.
                    }
                    else
                    {
						if(m_statusInfo.m_renderstatus != eCOMPETITION_RENDER_LOSE)
						{
							if (!m_ProcessDeathMessage && g_pHero->IsDead())	// ����� ���� ���� ������
							{
								if (!m_bDeathMessage)
								{
									m_bDeathMessage = true;
									this->ConfirmDeadMessageBoxBeforeStart();	// ������ġ��Ȱ/��� ����â
								}
							}
							else
							{
								m_bDeathMessage = false;
							}
						}
                    }
                }
            }
        }
    }

	return bRet;
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
CompetitionScene::Release( void )
{
    BattleScene::Release();

	uiBattleScoreMan* battle_score_manager_ptr =
        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
	if (battle_score_manager_ptr)
	{
		battle_score_manager_ptr->Reset();
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL
CompetitionScene::Render( DWORD dwTick )
{
    BattleScene::Render(dwTick);

    if (m_statusInfo.m_status == eCOMPETITION_SCENE_COMPETITON)
    {
        renderTimer();
    }

	renderWin();

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void
CompetitionScene::renderTimer()
{
	if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_FULL)->IsVisible())
        return;

	if(GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->IsVisible())
    {
		uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
		pDlg->GetDialogWZ()->GetClientRect(&m_baseRect);
    }
	else if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG))
    {
		uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG);
		pDlg->GetDialogWZ()->GetClientRect(&m_baseRect);
    }
    
	if(m_baseRect.left == 0)
	{
		return;
	}

    DWORD currentTick = clock_function::GetTickCount();
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP

	uiBattleZoneMan * pBattleZoneMan = ( uiBattleZoneMan * ) g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE);
	BASE_ROOMINFO * pRoomInfo = pBattleZoneMan->GetBaseRoomInfo();
	
	DWORD endTick = 0;
	if( pRoomInfo && pRoomInfo->IsSpecialType(eROOM_SPECIAL_GUILD_PVP) )	
		endTick = m_statusInfo.m_dwTickCompStart + CHUNTING_SURVIVAL_EXPIRE_TIME_GUILD_PVP;
	else
		endTick = m_statusInfo.m_dwTickCompStart + CHUNTING_SURVIVAL_EXPIRE_TIME;
#else
    DWORD endTick = m_statusInfo.m_dwTickCompStart + CHUNTING_SURVIVAL_EXPIRE_TIME;
#endif
    DWORD deltaTick = (endTick - currentTick);
    DWORD minTick   = (deltaTick / SEC_PER_TICK) / 60;
    DWORD secTick   = (deltaTick / SEC_PER_TICK) % 60;
    DWORD msecTick  = (deltaTick % 60);

	if(endTick < currentTick)
		return;

	if((minTick <= 0) && (secTick <= 0) && (msecTick <= 0))
		return;


	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

    RECT rc;

    BOOL bTest = g_pSunRenderer->IsEnableZTest();
    g_pSunRenderer->SetZBufferTest(FALSE);

    WzColor wzColor = WzColor_RGBA(0,0,0,255);

    g_pSunRenderer->x_pManagerTextOut->StartToUseFont(c_Font);
	Snprintf(szMessage,INTERFACE_STRING_LENGTH-1,  _T("%02d'%02d''%02d"), minTick, secTick, msecTick);

    BYTE    fontH = 30;
	int iHeight = 10 + 30;

	LONG textWidth = GlobalFunc::GetTextWidth(szMessage, c_Font);

	SetRect(&rc,
        m_baseRect.right-textWidth-c_iRightGap,
        m_baseRect.bottom + iHeight,
        m_baseRect.right,
        m_baseRect.bottom + iHeight + fontH);

    g_pSunRenderer->x_pManagerTextOut->DrawText(szMessage,
        &rc,
        WzColor_RGBA(255, 255, 255, 255),
        WzColor_RGBA(0,0,0,0),
        TP_HLEFT,
        ETS_OUTLINE,
        wzColor);

    g_pSunRenderer->SetZBufferTest(bTest);
}

//------------------------------------------------------------------------------
/**
*/
void
CompetitionScene::renderWin()
{	
	if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_FULL)->IsVisible())
		return;

	if(GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->IsVisible())
	{
		uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
		pDlg->GetDialogWZ()->GetClientRect(&m_baseRect);
	}
	else if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG))
	{
		uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG);
		pDlg->GetDialogWZ()->GetClientRect(&m_baseRect);
	}

	if(m_baseRect.left == 0)
	{
		return;
	}

	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	
    RECT rc;

    BOOL bTest = g_pSunRenderer->IsEnableZTest();
    g_pSunRenderer->SetZBufferTest(FALSE);

    WzColor wzColor = WzColor_RGBA(0,0,0,255);

    g_pSunRenderer->x_pManagerTextOut->StartToUseFont(c_Font);

	Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d WIN POINT"), m_winInfo.m_wWinPoint);
	
	BYTE    fontH = 30;
	int iHeight = 10 + 30 + 10 + 30;	
	LONG textWidth = GlobalFunc::GetTextWidth(szMessage, c_Font);

    SetRect(&rc,
        m_baseRect.right-textWidth-c_iRightGap,
        m_baseRect.bottom + iHeight,
        m_baseRect.right,
        m_baseRect.bottom + iHeight + fontH);

    g_pSunRenderer->x_pManagerTextOut->DrawText(szMessage,
        &rc,
        WzColor_RGBA(255, 255, 255, 255),
        WzColor_RGBA(0,0,0,0),
        TP_HLEFT,
        ETS_OUTLINE,
        wzColor);

    Snprintf (szMessage,INTERFACE_STRING_LENGTH-1,  _T("%d KILL POINT"), m_winInfo.m_wKillPoint);
	
    fontH = 30;

	textWidth = GlobalFunc::GetTextWidth(szMessage, c_Font);

	SetRect(&rc,
        m_baseRect.right-textWidth-c_iRightGap,
        m_baseRect.bottom + iHeight + fontH,
        m_baseRect.right,
        m_baseRect.bottom + iHeight + fontH + fontH);

    g_pSunRenderer->x_pManagerTextOut->DrawText(szMessage,
        &rc,
        WzColor_RGBA(255, 255, 255, 255),
        WzColor_RGBA(0,0,0,0),
        TP_HLEFT,
        ETS_OUTLINE,
        wzColor);

    g_pSunRenderer->SetZBufferTest(bTest);
}

//------------------------------------------------------------------------	------
/**
*/
void
CompetitionScene::renderLose()
{
}

//------------------------------------------------------------------------------
/**
*/
void
CompetitionScene::OnUsePotion(BASE_ITEMINFO *pItemInfo)
{
}

//------------------------------------------------------------------------------
/**
*/
void				
CompetitionScene::ConfirmDeadMessageBox()
{
    m_ProcessDeathMessage = true;

    TCHAR szMessage[1024] = {0, };
    TCHAR szTitle[INTERFACE_STRING_LENGTH], szSubject[INTERFACE_STRING_LENGTH];

	// �������� �Ǿ� �ִ� ��Ʈ���� ���ο� ��Ʈ������ ��ħ
	// 827	������ġ ��Ȱ'�� �����Ͻø� ������ġ���� ��Ȱ�մϴ�.
	//		\n'��Ȱ���'�� �����ø� ��Ȱ�� �����ðų� ������ ���ư��� ������ ���� ���¸� �����մϴ�.
	g_InterfaceManager.GetInterfaceString(826, szMessage, 512);
	g_InterfaceManager.GetInterfaceString(891, szTitle, 512);
	g_InterfaceManager.GetInterfaceString(892, szSubject, 512);

	uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
	if(pSystemMan)
	{
		pSystemMan->Sys_ConfirmExtend( szMessage, GlobalFunc::DeadConfirmInCHunting, szTitle, szSubject, 888, 889);
	}
}

//------------------------------------------------------------------------------
/**
*/
void
CompetitionScene::ConfirmDeadMessageBoxBeforeStart()
{
	m_ProcessDeathMessage = true;

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szTitle[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szSubject[INTERFACE_STRING_LENGTH] = {0,};

	// �������� �Ǿ� �ִ� ��Ʈ���� ���ο� ��Ʈ������ ��ħ
	// 827	������ġ ��Ȱ'�� �����Ͻø� ������ġ���� ��Ȱ�մϴ�.
	//		\n'��Ȱ���'�� �����ø� ��Ȱ�� �����ðų� ������ ���ư��� ������ ���� ���¸� �����մϴ�.
	g_InterfaceManager.GetInterfaceString(827, szMessage, 512);
	g_InterfaceManager.GetInterfaceString(891, szTitle, 512);
	g_InterfaceManager.GetInterfaceString(892, szSubject, 512);

	uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
	if(pSystemMan)
	{
		pSystemMan->Sys_ConfirmExtend( szMessage, GlobalFunc::DeadConfirmInCHuntingBeforeStart, szTitle, szSubject, 870, 889);
	}
}
//------------------------------------------------------------------------------
/** compose���� ��Ŷ ó��
*/
void
CompetitionScene::DoSomethingPacket(BYTE byProtocol)
{
    switch(byProtocol)
    {
    case SYNC_CHARACTER_RESURRECTION_BRD:
        {
            //������ ���ΰ�(��� üũ�ؼ� ȣ����)
            m_bDeadFlag = FALSE;
            this->ResetProcessDeathMessage();
        }
        break;
    default:
        {
            _asm nop;
        }
        break;
    }
}
//------------------------------------------------------------------------------
/**
*/
void
CompetitionScene::ResetProcessDeathMessage()
{
    m_ProcessDeathMessage = false;
}

RELATIONKIND::TYPE  CompetitionScene::GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy )
{
	return GameFunc::GetRelationKindOfPlayerInHuntingZone( pObject, bForceEnemy );
}