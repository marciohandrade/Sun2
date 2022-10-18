#include "SunClientPrecompiledHeader.h"
#include "PVPScene.h"
#include "NPC.h"
#include "ObjectManager.h"
#include "hero.h"
#include "NPCInfoParser.h"
#include "BattleLobby.h"
#include "LoadScene.h"
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "ChatDialog.h"
#include "UserSlot.h"
#include "ProjectileManagerEx.h"
#include "PVPManager.h"
#include "HeroActionInput.h"
#include "BattleScene.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiBattleScoreMan\uiBattleScoreMan.h"


//----------------------------------------------------------------------------
/**
*/
void 
PVPScene::ParsePacket( MSG_BASE * pMsg )
{
	switch ( pMsg->m_byCategory )
	{
	case CG_SYNC:
		{
			OnRecvCG_SYNC( pMsg );
		}
		break;

	case CG_STATUS:
		{
			OnRecvCG_STATUS( pMsg );
		}
		break;

	case CG_PVP:
		{
			OnRecvCG_PVP( pMsg );
		}
		break;

	default:
		{
			BattleScene::ParsePacket( pMsg );
		}
		break;
	}
}




//----------------------------------------------------------------------------
/** 
	@Remark 
		- !! ��, PVP �����߿� ������ ���� �� ����.
		   ��ȹ���� ���ǵ� ����.
*/
void	
PVPScene::OnRecvCG_SYNC( MSG_BASE * pMsg )
{
	BattleScene::OnRecvCG_SYNC( pMsg );	

	TCHAR szMessage[INTERFACE_STRING_LENGTH], szText[INTERFACE_STRING_LENGTH];

	switch ( pMsg->m_byProtocol )
	{
	case CG_SYNC_PLAYER_ENTER_ACK:
		{
			MSG_CG_SYNC_PLAYER_ENTER_ACK * pRecvMsg = (MSG_CG_SYNC_PLAYER_ENTER_ACK *)pMsg;

			pRecvMsg->Decode();

			initPVP();
			// �������Լ� PVP �غ� â�� ����ش�.	
			// CenterShowMessageBox ����
			// �ƹ� Ű�� �����ּ���.\n
			// PVP_PRESS_KEY_LEAVE_TIME / 1000 (%d)�� ���� �Է��� ������ ������ ���ư��� �˴ϴ�.

			// ���� ��� ����. 
			if ( g_pHero )
				g_pHero->SetInvincible(TRUE);

			g_HeroInput.SetCannotAttack(TRUE);

			// �ƹ�Ű�� ������.
			m_bCheckPressAnyKey = TRUE;
			m_pPressAnykeyDelay->SetTimer(PVP_PRESS_KEY_LEAVE_TIME);

			// �ӽ÷� ä�ÿ��ٰ� ǥ���ϴ°���. 
			//	�ƹ� Ű�� �����ּ���.
			const int	c_iPressAnyKey = 5606;
			g_InterfaceManager.GetInterfaceString( c_iPressAnyKey, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.ShowCenterMessageBox( szMessage, 10000, NULL, FALSE );
			DWORD dwTime = PVP_PRESS_KEY_LEAVE_TIME;
			dwTime /= SEC_PER_TICK;

			//	%ld �� ���� �Է��� ������ ������ �ڵ����� ���ư��� �˴ϴ�.
			const int	c_iNotPressKeyReturn = 5608;
			g_InterfaceManager.GetInterfaceString( c_iNotPressKeyReturn, szText, INTERFACE_STRING_LENGTH );
			Sprintf( szMessage, szText, dwTime );

			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		}
		break;

	case CG_SYNC_PLAYER_ENTER_BRD:
		{
			MSG_CG_SYNC_PLAYER_ENTER_BRD *pRecvPacket = (MSG_CG_SYNC_PLAYER_ENTER_BRD *)pMsg;

			PLAYER_RENDER_INFO_EX *pRenderInfo = (PLAYER_RENDER_INFO_EX*)( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_PLAYER_ENTER_BRD) );
			PLAYER_RENDER_INFO_EX output; // �ʱ�ȭ �ʿ� ����.
			int nSize = PLAYER_RENDER_INFO_EX::ConvertForRecv(pRenderInfo, &output);

			Player* pPlayer = (Player *)g_ObjectManager.GetObject( output.player_key );
			if( !pPlayer )	
				return;

			if (g_pHero && 
				g_pHero->GetObjectKey() == output.player_key)
			{
				pPlayer->SetInvincible(TRUE);
				g_HeroInput.SetCannotAttack(TRUE);
				return;
			}

			if(m_PVPManager.CheckLeaveUser(output.player_key))
			{
				pPlayer->SetInvincible(TRUE);
			}
		}
		break;

	case CG_SYNC_PLAYER_LEAVE_BRD:
		{
			MSG_CG_SYNC_PLAYER_LEAVE_BRD * pRecvMsg = (MSG_CG_SYNC_PLAYER_LEAVE_BRD * )pMsg;

			for (int i = 0; i < pRecvMsg->m_byCount; ++i)
			{
				DWORD dwObjectKey = pRecvMsg->m_PlayerKey[i];

				this->m_PVPManager.Remove( dwObjectKey );

				m_PVPManager.InsertLeaveUser(dwObjectKey);

			}	

			this->updatePVPInfo();
		}
		break;

	case CG_SYNC_ALLPLAYERS_CMD:
		{
			MSG_CG_SYNC_ALLPLAYERS_CMD * pRecvPacket = (MSG_CG_SYNC_ALLPLAYERS_CMD *)pMsg;
			// ���� �ִ���� ��� ���� ���

			BYTE * pPacket = ( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_ALLPLAYERS_CMD) );

			PLAYER_RENDER_INFO_EX *	pRenderInfo = NULL;

			ETC_STATE_TOTAL_INFO *pShield = NULL;
			VENDOR_RENDER_INFO *	pVendorInfo = NULL;

			uiBattleScoreMan* battle_score_manager_ptr = 
                static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));

			for (int i = 0; i < pRecvPacket->m_byCount; i++)
			{
				pRenderInfo = (PLAYER_RENDER_INFO_EX *)pPacket; 
				PLAYER_RENDER_INFO_EX output; // �ʱ�ȭ �ʿ� ����.
				int nSize = PLAYER_RENDER_INFO_EX::ConvertForRecv(pRenderInfo, &output);

				pShield = (ETC_STATE_TOTAL_INFO*)( (BYTE*)(pPacket) + nSize );
				pVendorInfo = (VENDOR_RENDER_INFO *)( (BYTE*)(pShield) + pShield->GetSize() );

				if (g_pHero)
				{		
					if (g_pHero->GetObjectKey() == output.player_key)
					{
						assert(!"Hero�� ALLPLAYER_CMD�� �Դ�!");
						pPacket += GetPlayerSize(pPacket,nSize);
						continue;
					}
				}

				// ���� ��� ����.
				Player * pPlayer = (Player *)g_ObjectManager.GetObject( output.player_key );
				if (pPlayer)
				{
					if(m_PVPManager.CheckLeaveUser(output.player_key))	
					{
						pPlayer->SetInvincible(TRUE);	
					}
				}

				pPlayer->SetMarketTitle(pVendorInfo->m_tszVendorTitle, pVendorInfo->m_byTitleLength);

				pPacket += GetPlayerSize(pPacket,nSize);
			}

			this->updatePVPInfo();
		}
		break;

	}
}


//----------------------------------------------------------------------------
/**
*/
void	
PVPScene::OnRecvCG_STATUS( MSG_BASE * pMsg )
{
	BattleScene::OnRecvCG_STATUS( pMsg );

	switch ( pMsg->m_byProtocol )
	{
	case CG_STATUS_RESURRECTION_BRD:
		{
			MSG_CG_STATUS_RESURRECTION_BRD * pRecvMsg = (MSG_CG_STATUS_RESURRECTION_BRD *)pMsg;

			// ���⼱ ���� �÷��׸� �����ش�.

			Player * pPlayer = (Player * )g_ObjectManager.GetObject( pRecvMsg->m_dwObjectKey );
			if ( !pPlayer )
			{
				return;
			}

			// �������
			pPlayer->SetInvincible( TRUE );
		
			m_PVPManager.SetUserState(pRecvMsg->m_dwObjectKey,TRUE);

			if (g_pHero && (g_pHero->GetObjectKey() == pRecvMsg->m_dwObjectKey))
			{
				g_HeroInput.SetCannotAttack( TRUE );
			}
		}
		break;

	case CG_STATUS_DEAD_BRD :
		{
			MSG_CG_STATUS_DEAD_BRD *pRecvPacket = (MSG_CG_STATUS_DEAD_BRD *)pMsg;

			DWORD dwKey	= pRecvPacket->m_dwObjectKey;	// ����
			WzVector deadPos = pRecvPacket->m_wvCurPos;
			EXPTYPE exp = pRecvPacket->m_i64Exp;

			Player * pPlayer = (Player *)g_ObjectManager.GetObject(dwKey);

			if( pPlayer )
			{
				// ���� ���
				pPlayer->SetInvincible( TRUE );
				pPlayer->SetExp( exp );
		
				if (g_pHero && pPlayer == g_pHero)
				{
					g_HeroInput.SetCannotAttack( TRUE );

					// ���� �׾����� PVP ���� â�� �����ش�.
					showPVPInfoDialog();
				}
			}

			m_PVPManager.SetUserState(dwKey,TRUE);
		}
		break;

	default:
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
void	
PVPScene::OnRecvCG_PVP( MSG_BASE * pMsg )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH], szText[INTERFACE_STRING_LENGTH];

	switch ( pMsg->m_byProtocol )
	{
	case CG_PVP_INFO_CMD:
		{
			MSG_CG_PVP_INFO_CMD * pRecvMsg = (MSG_CG_PVP_INFO_CMD *)pMsg;

			if ( g_pHero )
				g_pHero->SetTeam(pRecvMsg->m_Team);

			for ( int i = 0; i < pRecvMsg->m_Count; ++i )
			{
				GlobalFunc::DebugOutputMsg(
					eOUTPUT_MSG_OUPUT_DEBUG, 
					_T("[PVP << CG_PVP_INFO_CMD] : %ld Team\n"), 
					pRecvMsg->m_Team );
				
				// ����ϰ�
				this->m_PVPManager.Add( pRecvMsg->m_Info[i].m_dwPlayerKey );
				
				// ������ ��
				PVPPlayerInfo PVPInfo;
				PVPInfo.iTeam = pRecvMsg->m_Info[i].m_Team;
				this->m_PVPManager.ModifyPVPInfo( pRecvMsg->m_Info[i].m_dwPlayerKey, &PVPInfo);

				// ���� Check
				if ( PVPInfo.iTeam > 0 )
				{
					m_PVPManager.SetTeamPlay(TRUE);
				}

				Player * pPlayer = 
					(Player * )g_ObjectManager.GetObject( pRecvMsg->m_Info[i].m_dwPlayerKey );
				if ( !pPlayer )
				{
					assert(0);
					continue;
				}

				pPlayer->SetTeam( pRecvMsg->m_Info[i].m_Team );

				GlobalFunc::DebugOutputMsg(
					eOUTPUT_MSG_OUPUT_DEBUG, 
					_T("[PVP << MSG_CG_PVP_INFO_CMD] : %d Key, %d Team\n" ),
					pPlayer->GetObjectKey(), pPlayer->GetTeam());
			}
		}
		break;

	case CG_PVP_INFO_BRD: 
		{
			MSG_CG_PVP_INFO_BRD * pRecvMsg = (MSG_CG_PVP_INFO_BRD *)pMsg;			

			GlobalFunc::DebugOutputMsg(
				eOUTPUT_MSG_OUPUT_DEBUG, 
				_T("[PVP << MSG_CG_PVP_INFO_BRD] : %ld User Key \n") ,
				pRecvMsg->m_dwPlayerKey);

			// ������ ��
			PVPPlayerInfo PVPInfo;
			BOOL bGetPVPInfo = this->m_PVPManager.GetPVPInfo( pRecvMsg->m_dwPlayerKey, &PVPInfo);
			if (!bGetPVPInfo)
			{
				// ���� ���߿� ������ �༮�ϰŴ�. 
				this->m_PVPManager.Add(pRecvMsg->m_dwPlayerKey);
			}

			PVPInfo.iTeam = pRecvMsg->m_Team;
			this->m_PVPManager.ModifyPVPInfo( pRecvMsg->m_dwPlayerKey, &PVPInfo);

			Player * pPlayer = 
					(Player * )g_ObjectManager.GetObject( pRecvMsg->m_dwPlayerKey );
			if ( !pPlayer )
			{
				GlobalFunc::DebugOutputMsg(
					eOUTPUT_MSG_OUPUT_DEBUG, 
					_T("[PVP << MSG_CG_PVP_INFO_BRD:error] : %ld Key �÷��̾ ����\n"), pRecvMsg->m_dwPlayerKey);

				return;
			}

			pPlayer->SetTeam( pRecvMsg->m_Team );			
		}
		break;

	case CG_PVP_PRESS_KEY_ACK:
		{
			MSG_CG_PVP_PRESS_KEY_ACK * pRecvMsg = 
				(MSG_CG_PVP_PRESS_KEY_ACK *)pMsg;

			BIT_REMOVE(this->m_packetStatus, PVP_PACKET_STATUS_PRESS_KEY);		

			m_bCheckPressAnyKey = FALSE;

			//	���� �غ���...
			const int	c_iReadyPVP = 5612;
			g_InterfaceManager.GetInterfaceString( c_iReadyPVP, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.ShowCenterMessageBox( szMessage, 10000, NULL, FALSE );
		}
		break;

	case CG_PVP_PRESS_KEY_NAK:
		{
			MSG_CG_PVP_PRESS_KEY_NAK * pRecvMsg = (MSG_CG_PVP_PRESS_KEY_NAK *)pMsg;
			
			pRecvMsg->m_byErrorCode;

			BIT_REMOVE(this->m_packetStatus, PVP_PACKET_STATUS_PRESS_KEY);
		}
		break;

	case CG_PVP_MATCHLESS_MODE_START_CMD: // �ڽſ��Ը� ���� ��Ŷ
		{
			MSG_CG_PVP_MATCHLESS_MODE_START_CMD * pRecvMsg = 
						(MSG_CG_PVP_MATCHLESS_MODE_START_CMD *)pMsg;

			GlobalFunc::DebugOutputMsg(
				eOUTPUT_MSG_OUPUT_DEBUG, 
				_T("[PVP << CG_PVP_MATCHLESS_MODE_START_CMD] : Ư�� �ð� �� ������ ���۵ȴ�.\n") );

			if(g_pHero)
				g_pHero->SetInvincible( TRUE );

			g_HeroInput.SetCannotAttack( TRUE );

			//PVP_MATCHLESSS_DELAY_TIME
			m_pStartMatchDelay->SetTimer( PVP_MATCHLESSS_DELAY_TIME );

			// TODO : �̻ڰ� ȭ�鿡 �����ִ°� ó���ؾ��Ѵ�.
			DWORD dwDelayTick = PVP_MATCHLESSS_DELAY_TIME;
			dwDelayTick /= SEC_PER_TICK;

			//	%ld �� �Ŀ� ������ ���۵˴ϴ�.
			const int	c_iStartPVP = 5609;
			g_InterfaceManager.GetInterfaceString( c_iStartPVP, szText, INTERFACE_STRING_LENGTH );
			Sprintf( szMessage, szText, dwDelayTick);

			updatePVPInfo();

			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CENTER_SHOW, szMessage );
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage );
		}
		break;

	case CG_PVP_MATCH_MODE_FIRST_START_CMD: // ó�� �����Ҷ� ���� ��Ŷ
		{
			MSG_CG_PVP_MATCH_MODE_FIRST_START_CMD * pRecvMsg = 
						(MSG_CG_PVP_MATCH_MODE_FIRST_START_CMD *)pMsg;

			GlobalFunc::DebugOutputMsg(
				eOUTPUT_MSG_OUPUT_DEBUG, 
				_T("[PVP << CG_PVP_MATCH_MODE_FIRST_START_CMD]\n") );


			// ���� ��� ����
			this->UnInvincibleToAllUser();
			m_PVPManager.SetAllUserState(FALSE);
			g_HeroInput.SetCannotAttack(FALSE);
            g_HeroInput.SetAutoAttack(FALSE);

			//	���� ����!!!
			const int	c_iLetsPVP = 5613;
			g_InterfaceManager.GetInterfaceString( c_iLetsPVP, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.ShowCenterMessageBox( szMessage, 10000, NULL, FALSE );
		}
		break;

	case CG_PVP_MATCH_MODE_START_CMD:
		{
			MSG_CG_PVP_MATCH_MODE_START_CMD * pRecvMsg = 
						(MSG_CG_PVP_MATCH_MODE_START_CMD *)pMsg;

			GlobalFunc::DebugOutputMsg(
				eOUTPUT_MSG_OUPUT_DEBUG, 
				_T("[PVP << CG_PVP_MATCH_MODE_FIRST_START_CMD]\n") );

			assert(g_pHero);
			if (g_pHero)
			{
				g_pHero->SetInvincible(FALSE);
			}

			g_HeroInput.SetCannotAttack(FALSE);
            g_HeroInput.SetAutoAttack(FALSE);

			// TODO : �̻ڰ� ȭ�鿡 �����ִ°� ó���ؾ��Ѵ�.
			//	���� ����!!!
			const int	c_iLetsPVP = 5613;
			g_InterfaceManager.GetInterfaceString( c_iLetsPVP, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.ShowCenterMessageBox( szMessage, 10000, NULL, FALSE );
		}
		break;

	case CG_PVP_MATCH_MODE_START_BRD: // ���� ������ 
		{
			MSG_CG_PVP_MATCH_MODE_START_BRD * pRecvMsg = (MSG_CG_PVP_MATCH_MODE_START_BRD * )pMsg;
	
			Player * pPlayer = (Player *)g_ObjectManager.GetObject( pRecvMsg->m_dwPlayerKey );
			if (pPlayer)
			{
				pPlayer->SetInvincible(FALSE);

				//	%s ���� ���� ���� �����Դϴ�.
				const int	c_iNameCanPVP = 5607;
				g_InterfaceManager.GetInterfaceString( c_iNameCanPVP, szText, INTERFACE_STRING_LENGTH );
				Sprintf( szMessage, szText, pPlayer->GetName() );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

			GlobalFunc::DebugOutputMsg(
				eOUTPUT_MSG_OUPUT_DEBUG, 
				_T("[PVP << MSG_CG_PVP_MATCH_MODE_START_BRD] : %s\n"), 
				szMessage );
			}

			m_PVPManager.SetUserState(pRecvMsg->m_dwPlayerKey,FALSE);

		}
		break;

		// ����� �������� �޴´�.( ��ȹ�� ���� �ʿ� )
	case CG_PVP_SCORE_CMD:				
		{
			MSG_CG_PVP_SCORE_CMD * pRecvMsg = (MSG_CG_PVP_SCORE_CMD *)pMsg;

			if ( g_pHero )
			{
				PVPPlayerInfo TempPVPInfo;
				TempPVPInfo.Score = pRecvMsg->m_Score;
				this->m_PVPManager.ModifyPVPInfo(g_pHero->GetObjectKey(), &TempPVPInfo);

				//	%ld �� ������ ȹ�� �Ͽ����ϴ�.
				const int	c_iGetPVPScore = 5610;
				g_InterfaceManager.GetInterfaceString( c_iGetPVPScore, szText, INTERFACE_STRING_LENGTH );
				Sprintf(szMessage, szText, pRecvMsg->m_Score);

				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}

			this->updatePVPInfo();
		}
		break;

	case CG_PVP_RESULT_BRD:
		{
			MSG_CG_PVP_RESULT_BRD * pRecvMsg = (MSG_CG_PVP_RESULT_BRD *)pMsg;
			
			PVPPlayerInfo TempPVPInfo;

			GlobalFunc::DebugOutputMsg(
				eOUTPUT_MSG_OUPUT_DEBUG, 
				_T("[PVP << MSG_CG_PVP_RESULT_BRD] : KillerKey(%ld), DierKey(%ld)\n"), 
				pRecvMsg->m_KillerKey, pRecvMsg->m_DierKey);

			// ���� Ƚ�� ���
			Object * pKillObject = g_ObjectManager.GetObject( pRecvMsg->m_KillerKey );
			assert(pKillObject);
			if ( pKillObject )
			{
				if (g_pHero && (g_pHero->GetObjectKey() == pRecvMsg->m_KillerKey))
				{
					if ( g_HeroInput.GetCurrentTarget() == pRecvMsg->m_DierKey )
					{
						// ������ ������� �ٷ� ���� ���ص� ���¶�� ���� ���� Ǯ���ش�.
						g_HeroInput.SetAutoAttack(FALSE);
					}
				}

				TempPVPInfo.KillCount++;
				this->m_PVPManager.ModifyPVPInfo( pRecvMsg->m_KillerKey, &TempPVPInfo);			
			}
			else
			{
				assert( pKillObject );
			}

			// ���� Ƚ�� ���
			Object * pDeadObject = g_ObjectManager.GetObject( pRecvMsg->m_DierKey );
			assert(pDeadObject);
			if ( pDeadObject )
			{
				TempPVPInfo.Init();
				TempPVPInfo.DeadCount++;
				this->m_PVPManager.ModifyPVPInfo(pRecvMsg->m_DierKey, &TempPVPInfo);
                if (pDeadObject->GetObjectKey() == g_pHero->GetObjectKey())
                {
                    this->showPVPInfoDialog();
                }
			}
			else
			{
				assert( pDeadObject );
			}

			if (pKillObject && pDeadObject)
			{
				//	[%s]�Բ��� [%s]�� ���Լ� ų���� ȹ�� �߽��ϴ�.
				const int	c_iGetPVPKillNumber = 5614;
				g_InterfaceManager.GetInterfaceString( c_iGetPVPKillNumber, szText, INTERFACE_STRING_LENGTH );
				Sprintf(szMessage, szText, pKillObject->GetName(), pDeadObject->GetName() );

				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
			}

			updatePVPInfo();
		}
		break;

	case CG_PVP_END_BRD:// PVP ������.
		{
			MSG_CG_PVP_END_BRD * pRecvMsg = (MSG_CG_PVP_END_BRD *)pMsg;

			updatePVPInfo();

			// ���� �Խ����� ��������.
			this->showPVPInfoDialog();

			// ��� ���� �������� ���� �Ҵ� ���·� ����� ���´�. 
			this->InvincibleToAllUser();

			//	���� ����!!!
			const int	c_iEndPVP = 5614;
			g_InterfaceManager.GetInterfaceString( c_iEndPVP, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.ShowCenterMessageBox( szMessage, 10000, NULL, FALSE );

            // 5�� �� ������ ���ư��� �ص���.
#ifdef _GS_GSP_TEAM_PVP_MSG
            m_PVPManager.ShowWinTeamMessage();
#endif//_GS_GSP_TEAM_PVP_MSG
			
		}
		break;
	}
}
