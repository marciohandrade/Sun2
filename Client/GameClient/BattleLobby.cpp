//------------------------------------------------------------------------------
//  BattleZoneRready.cpp
//  (C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "BattleLobby.h"
#include "InterfaceManager.h"
#include "BattleRoomEntry.h"
#include "hero.h"
#include "UserSlot.h"
#include "UserSlotSet.h"
#include "ObjectManager.h"
#include "MapInfoParser.h"
#include "LoadScene.h"
#include "Application.h"
#include "VillageScene.h"
#include "ChatDialog.h"
#include "RequireFieldLevelInfoParser.h"
#include "ResultCode.h"
#include "HeroActionInput.h"
#include "uiBattleZoneReadyPVPConfig/uiBattleZoneReadyPVPConfig.h"
#include "CloneManager.h"
#include "EventInfoParser.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "uiBattleZoneReadyMissionConfig/uiBattleZoneReadyMissionConfig.h"
#include "uiBattleZoneReadyCHuntingConfig/uiBattleZoneReadyCHuntingConfig.h"
#include "MissionScene.h"
#include "Map.h"
#include "uiPartyMan/uiPartyMan.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemFunction.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
    #include "uiGuildMenu/uiGuildMenu.h"
    #include "uiGuildMain/uiGuildMain.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "uiCommunityMan/uiCommunityMan.h"
#include "BattleRoomEntry.h"
#include "uibattlezoneman/uiBattleZoneMan_def.h"
#include "Mouse.h"
#include "uiChangeRoomName/uiChangeRoomName.h"
#include "uiChatMan/uiChatMan_def.h"

WzID BattleLobby::m_wzId[] = 
{
	StrToWzID("T001"), //BattleLobby_ST_ROOM_NUMBER = 0,			// >> �ؽ�Ʈ �������� ��Ʈ��
	StrToWzID("T002"), //BattleLobby_ST_ROOM_NAME,
	StrToWzID("T003"), //BattleLobby_ST_ROOM_PASSWORD,
	StrToWzID("T007"), //BattleLobby_ST_MAP_NAME,
	StrToWzID("T009"), //BattleLobby_ST_MAP_LIMIT_LEVEL,
	StrToWzID("T011"), //BattleLobby_ST_MAP_RECOMMEND_LEVEL,
	StrToWzID("T030"), //BattleLobby_ST_ZONE_TYPE,
	StrToWzID("T201"), //BattleLobby_ST_USER_INFO_LEVEL_1,
	StrToWzID("T202"), //BattleLobby_ST_USER_INFO_LEVEL_2,
	StrToWzID("T203"), //BattleLobby_ST_USER_INFO_LEVEL_3,
	StrToWzID("T204"), //BattleLobby_ST_USER_INFO_LEVEL_4,
	StrToWzID("T205"), //BattleLobby_ST_USER_INFO_LEVEL_5,
	StrToWzID("T206"), //BattleLobby_ST_USER_INFO_LEVEL_6,
	StrToWzID("T207"), //BattleLobby_ST_USER_INFO_LEVEL_7,
	StrToWzID("T208"), //BattleLobby_ST_USER_INFO_LEVEL_8,
	StrToWzID("T209"), //BattleLobby_ST_USER_INFO_LEVEL_9,
	StrToWzID("T210"), //BattleLobby_ST_USER_INFO_LEVEL_10,
	StrToWzID("T101"), //BattleLobby_ST_USER_INFO_NAME_1,
	StrToWzID("T102"), //BattleLobby_ST_USER_INFO_NAME_2,
	StrToWzID("T103"), //BattleLobby_ST_USER_INFO_NAME_3,
	StrToWzID("T104"), //BattleLobby_ST_USER_INFO_NAME_4,
	StrToWzID("T105"), //BattleLobby_ST_USER_INFO_NAME_5,
	StrToWzID("T106"), //BattleLobby_ST_USER_INFO_NAME_6,
	StrToWzID("T107"), //BattleLobby_ST_USER_INFO_NAME_7,
	StrToWzID("T108"), //BattleLobby_ST_USER_INFO_NAME_8,
	StrToWzID("T109"), //BattleLobby_ST_USER_INFO_NAME_9,
	StrToWzID("T110"), //BattleLobby_ST_USER_INFO_NAME_10,	// << �ؽ�Ʈ �������� ��Ʈ��
	StrToWzID("B001"), //BattleLobby_BT_VIEW_USER_1,
	StrToWzID("B002"), //BattleLobby_BT_VIEW_USER_2,
	StrToWzID("B003"), //BattleLobby_BT_VIEW_USER_3,
	StrToWzID("B004"), //BattleLobby_BT_VIEW_USER_4,
	StrToWzID("B005"), //BattleLobby_BT_VIEW_USER_5,
	StrToWzID("B006"), //BattleLobby_BT_VIEW_USER_6,
	StrToWzID("B007"), //BattleLobby_BT_VIEW_USER_7,
	StrToWzID("B008"), //BattleLobby_BT_VIEW_USER_8,
	StrToWzID("B009"), //BattleLobby_BT_VIEW_USER_9,
	StrToWzID("B010"), //BattleLobby_BT_VIEW_USER_10,
	StrToWzID("C001"), //BattleLobby_CHECK_USER_BLOCK_2,
	StrToWzID("C002"), //BattleLobby_CHECK_USER_BLOCK_3,
	StrToWzID("C003"), //BattleLobby_CHECK_USER_BLOCK_4,
	StrToWzID("C004"), //BattleLobby_CHECK_USER_BLOCK_5,
	StrToWzID("C005"), //BattleLobby_CHECK_USER_BLOCK_6,
	StrToWzID("C006"), //BattleLobby_CHECK_USER_BLOCK_7,
	StrToWzID("C007"), //BattleLobby_CHECK_USER_BLOCK_8,
	StrToWzID("C008"), //BattleLobby_CHECK_USER_BLOCK_9,
	StrToWzID("C009"), //BattleLobby_CHECK_USER_BLOCK_10,
	StrToWzID("TI02"), //BattleLobby_TI_START,				
	StrToWzID("TI03"), //BattleLobby_TI_LEAVE,	
	StrToWzID("TI01"), //BattleLobby_TI_TEAM_CHANGE,
	StrToWzID("B902"), //BattleLobby_BT_USER_EXIT_2,
	StrToWzID("B903"), //BattleLobby_BT_USER_EXIT_3,
	StrToWzID("B904"), //BattleLobby_BT_USER_EXIT_4,
	StrToWzID("B905"), //BattleLobby_BT_USER_EXIT_5,
	StrToWzID("B906"), //BattleLobby_BT_USER_EXIT_6,
	StrToWzID("B907"), //BattleLobby_BT_USER_EXIT_7,
	StrToWzID("B908"), //BattleLobby_BT_USER_EXIT_8,
	StrToWzID("B909"), //BattleLobby_BT_USER_EXIT_9,
	StrToWzID("B910"), //BattleLobby_BT_USER_EXIT_10,
	StrToWzID("B999"), //BattleLobby_BT_EXIT,					 // << �׼�
	StrToWzID("TI04"), //BattleLobby_BT_FRIEND_LIST
	StrToWzID("TI05"), //BattleLobby_BT_GUILD_LIST
	StrToWzID("TI00"), //BattleLobby_BT_ROOM_NAME_CHANGE
};


//----------------------------------------------------------------------------
/** 
*/
BattleLobby::BattleLobby(void)
{
	 
}

//----------------------------------------------------------------------------
/** 
*/
BattleLobby::~BattleLobby(void)
{
}


//----------------------------------------------------------------------------
/** 
*/
VOID	
BattleLobby::Init( CDrawBase * pDrawBase )
{
	m_pBattleZoneMan = static_cast<uiBattleZoneMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE));
	assert(m_pBattleZoneMan);

	if(m_pBattleZoneMan)
		m_pBattleZoneMan->InitializeUserSlotSet();

	m_hTexture[USE_TEXTURE_READY]		= g_pSunRenderer->x_pManagerTexture->LoadTexture( "Data\\Interface\\63_ready.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
	m_pTextureInfo[USE_TEXTURE_READY]	= g_pSunRenderer->x_pManagerTexture->GetTextureInfo( m_hTexture[USE_TEXTURE_READY] );

	assert(m_pTextureInfo[USE_TEXTURE_READY]);


	//UpdatePosition();


	// ��Ʈ�� �����͸� �޾� ���´�. 
	int i = 0;

	// ���� ������ ����
	for (i = 0; i < MAX_VIEW_USER_SLOT; ++i)
	{
		m_pCtrlBtnUserSlotView[i] = (CCtrlButtonWZ *)getControl(BattleLobby_BT_VIEW_USER_1+i);
		assert(m_pCtrlBtnUserSlotView[i]);
	}

	// ���� ����
	for (i = 0; i < MAX_VIEW_USER_SLOT; ++i)
	{
		m_pCtrlStaticUserLevel[i] = (CCtrlStaticWZ *)getControl(BattleLobby_ST_USER_INFO_LEVEL_1+i);
		assert(m_pCtrlStaticUserLevel[i]);
	}
	// ���� ����
	for (i = 0; i < MAX_VIEW_USER_SLOT; ++i)
	{
		m_pCtrlStaticUserName[i] = (CCtrlStaticWZ *)getControl(BattleLobby_ST_USER_INFO_NAME_1+i);
		assert(m_pCtrlStaticUserName[i]);
	}

	// ���� ��ư 
	for (i = 0; i < MAX_VIEW_USER_SLOT-1; ++i)
	{
		m_pCtrlBtnBan[i] = (CCtrlButtonWZ *)getControl(BattleLobby_BT_USER_EXIT_2+i);
		assert(m_pCtrlBtnBan[i]);
	}

	// ���� ���� �� CHECK
	for (i = 0; i < MAX_VIEW_USER_SLOT-1; ++i)
	{
		 m_pCtrlCheckBtnUserSlotBlock[i] = (CCtrlButtonCheckWZ *)getControl(BattleLobby_CHECK_USER_BLOCK_2+i);
		 assert(m_pCtrlCheckBtnUserSlotBlock[i]);
	}
	
	// ��Ÿ
	// ���ȣ
	m_pCtrlStaticRoomNo		= (CCtrlStaticWZ *)getControl(BattleLobby_ST_ROOM_NUMBER);
	assert(m_pCtrlStaticRoomNo);
	// ������
	m_pCtrlStaticRoomTitle	= (CCtrlStaticWZ *)getControl(BattleLobby_ST_ROOM_NAME);			
	assert(m_pCtrlStaticRoomTitle);
	// �н����� 
	m_pCtrlStaticRoomPW		= (CCtrlStaticWZ *)getControl(BattleLobby_ST_ROOM_PASSWORD);			
	assert(m_pCtrlStaticRoomPW);
	// �� TYPE
	m_pCtrlStaticZoneType	= (CCtrlStaticWZ *)getControl(BattleLobby_ST_ZONE_TYPE);			
	assert(m_pCtrlStaticZoneType);

	m_pCtrlImageStaticTeamChange = (CCtrlImageStaticWZ *)getControl(BattleLobby_TI_TEAM_CHANGE);
	assert(m_pCtrlImageStaticTeamChange);
	// �� �̸�
	m_pCtrlStaticMapName	= (CCtrlStaticWZ *)getControl(BattleLobby_ST_MAP_NAME);			
	assert(m_pCtrlStaticMapName);
	// ���� ����
	m_pCtrlStaticLimitLevel	= (CCtrlStaticWZ *)getControl(BattleLobby_ST_MAP_LIMIT_LEVEL);			
	assert(m_pCtrlStaticLimitLevel);
	// ���� ���� ����
	m_pCtrlStaticRecommendLevel	= (CCtrlStaticWZ *)getControl(BattleLobby_ST_MAP_RECOMMEND_LEVEL);
	assert(m_pCtrlStaticRecommendLevel);

	m_pCtrlTIStartNReady = (CCtrlImageStaticWZ *)getControl( BattleLobby_TI_START );
	assert(m_pCtrlTIStartNReady);

	m_pCtrlRoomNameChangeBtn = (CCtrlButtonWZ*)getControl(BattleLobby_BT_ROOM_NAME_CHANGE);
	assert(m_pCtrlRoomNameChangeBtn);
}

//----------------------------------------------------------------------------
/** 
*/
VOID	
BattleLobby::Release()
{
	for (int i = 0; i < MAX_USE_TEXTURE; ++i)
	{
        if (this->m_hTexture[i] != INVALID_HANDLE_VALUE)
        {
		    g_pSunRenderer->x_pManagerTexture->DestroyTexture(this->m_hTexture[i]);
		    this->m_hTexture[i] = INVALID_HANDLE_VALUE;
        }
	}
}


//----------------------------------------------------------------------------
/** 
*/
VOID	
BattleLobby::MessageProc( SI_MESSAGE * pMessage )
{
	if(!m_pBattleZoneMan)
		return;

	POSTYPE CurPos = 255;
	switch( pMessage->eResultMsg  ) 
	{
		case RT_MSG_LBUTTONCLICK:
		{
			showZoneStatusWindow();

			CurPos = getCtrlIDToPos( pMessage->dwCtrlID );
            switch( CurPos )
            {
			case BattleLobby_BT_EXIT:
				{
					m_pBattleZoneMan->LeaveLobby();
				}
				break;

			case BattleLobby_BT_FRIEND_LIST:
				{
					uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
					if(pCommMan)
					{
						pCommMan->ShowCommunityDlg();
						pCommMan->SetWindowFocus();
					}
				}
				break;

			case BattleLobby_BT_GUILD_LIST:
				{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                    GuildSystem::Function::ToggleGuildInfoDialog();
#else
                    // ��� �޴�
                    uiGuildMan* guildMan =
                        static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                    if (guildMan)
                    {

                        if(guildMan->IsShowWindow())
                        {
                            uiBase * guildMain = GET_DIALOG( IM_GUILD_MANAGER::GUILD_MAIN );

                            assert (guildMain);

                            guildMain->ShowInterface(TRUE);

                        }
                        else
                        {
                            guildMan->SetShowGuildMain();
                            guildMan->SEND_CG_GUILD_TOTALINFO_REQ_SYN();
                        }

                        guildMan->SetWindowFocus();
                    }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
				}
				break;

			//case BattleLobby_TI_INVITE_LIST:	
			//	{
			//		// ���� �߰��ɰ�
			//		// �ʴ��� ���� ����Ʈ�� ���´�.
			//		// ���� ������ �ִ� �༮���� ������ �����õ��ϴ�.
			//	}
			//	break;

			case BattleLobby_TI_START:
				{
					// ������ Start
					// @ ���� ��� Ready�� �ؾ��Ѵ�!! 
					// Ÿ�ɹ����� Ready ��ư ó���� ���ش�.
					if(m_pBattleZoneMan->IsMaster())
					{
						// Start!!
						m_pBattleZoneMan->StartBattle();
					}
					else
					{
						m_pBattleZoneMan->ReadyBattle();
					}					
				}
				break;

			case BattleLobby_TI_LEAVE:		
				{
					m_pBattleZoneMan->LeaveLobby();
				}
				break;

			case BattleLobby_TI_TEAM_CHANGE:
				{
					m_pBattleZoneMan->ChangeTeam();
				}
				break;

			case BattleLobby_BT_VIEW_USER_2:
			case BattleLobby_BT_VIEW_USER_3:
			case BattleLobby_BT_VIEW_USER_4:
			case BattleLobby_BT_VIEW_USER_5:
			case BattleLobby_BT_VIEW_USER_6:
			case BattleLobby_BT_VIEW_USER_7:
			case BattleLobby_BT_VIEW_USER_8:
			case BattleLobby_BT_VIEW_USER_9:
			case BattleLobby_BT_VIEW_USER_10:
				{
					// ���� �������� ����
					if(!m_pBattleZoneMan->IsMaster())
					{
						return;
					}

					// ��
					int BlockIndex = CurPos - BattleLobby_BT_VIEW_USER_2 + 1 ; 
					this->BlockPlayer(BlockIndex);
				}
				break;

			case BattleLobby_BT_USER_EXIT_2:
			case BattleLobby_BT_USER_EXIT_3:
			case BattleLobby_BT_USER_EXIT_4:
			case BattleLobby_BT_USER_EXIT_5:
			case BattleLobby_BT_USER_EXIT_6:
			case BattleLobby_BT_USER_EXIT_7:
			case BattleLobby_BT_USER_EXIT_8:
			case BattleLobby_BT_USER_EXIT_9:
				{
					int ExitIndex = CurPos - BattleLobby_BT_USER_EXIT_2 + 1; // 0 ���� �����̴�. ������ ��ŵ

					m_pBattleZoneMan->BanPlayerByIndex(ExitIndex);
				}
				break;

			case BattleLobby_BT_ROOM_NAME_CHANGE:
				{
					uiChangeRoomName * pChangeRoomName = 
						(uiChangeRoomName *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHANGE_ROOM_NAME);
					pChangeRoomName->ShowDialog(TRUE);
				}
				break;
			}
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}



//----------------------------------------------------------------------------
/** 
*/
VOID	
BattleLobby::NetworkProc( MSG_BASE * pMsg )
{
}


//----------------------------------------------------------------------------
/** 
*/
VOID	
BattleLobby::OnUpdateSolarDialog()
{
	updateUserSlotUI();
	updateMapInfoUI();

	updateZoneType();
	updatePVPUI();

	if(!m_pBattleZoneMan)
		return;

	if(g_pHero)
	{
		UserSlot * pUserSlot = m_pBattleZoneMan->GetUserSlotByKey(g_pHero->GetObjectKey());

		CCtrlButtonWZ* pLeaveButton = static_cast<CCtrlButtonWZ *>(getControl(BattleLobby_TI_LEAVE));
		CCtrlButtonWZ* pExitButton = static_cast<CCtrlButtonWZ *>(getControl(BattleLobby_BT_EXIT));

		if (pUserSlot && pLeaveButton && pExitButton)
		{
			if (pUserSlot->GetSlotState() == UserSlot::READY)
			{
				// �泪���� ��ư hide
				pLeaveButton->ShowWindowWZ(WZ_HIDE);	
				pExitButton->ShowWindowWZ(WZ_HIDE);
				
			}
			else
			{
				// �泪���� ��ư show
				pLeaveButton->ShowWindowWZ(WZ_SHOW);
				pExitButton->ShowWindowWZ(WZ_SHOW);
			}
		}
	}

	
}


//----------------------------------------------------------------------------
/** 
*/
VOID	
BattleLobby::OnRenderSolarDialog()
{
	// TODO : ������ �Լ��� ���°��� ���ʹ�. �ϴ� �̰� �ϴ����� ���θ�
	// �״�� �ΰ� �ƴϸ� �Լ��� ����.
	float fMeshHeight	= 1.f;
	float fMeshWidth	= 1.f;
	RECT rcSize;			// ������ ��ġ ����
	RECT rcViewport;		// view port rect

	// ī�޶� ���� �԰� �����°Ͷ����� �̺κ��� �־��ش�.
	if ( g_pHero )
	{
		g_pHero->SetCameraAlpha(FALSE);
	}

	// ���� BoundingBox�� ���Ҽ����� ĳ������ Ű�� ���Ҽ�����.
	// ĳ������ Ű�� �ϵ��ڵ��Ѵ�.

	if(!m_pBattleZoneMan)
		return;

	for (int i = 0; i < m_pBattleZoneMan->GetMaxUserSlot(); ++i)
	{
		UserSlot * pUserSlot = m_pBattleZoneMan->GetUserSlotByIndex(i);
		
		if(!pUserSlot)
		{
			assert(0);
			continue;
		}

		Player * pPlayer = static_cast<Player *>(g_ObjectManager.GetObject( pUserSlot->GetPlayerKey() ));
		if ( !pPlayer )
		{
			continue;
		}

		switch(pPlayer->GetClass())
		{
		case eCHAR_BERSERKER:
			fMeshHeight = 2.0f;			
			break;
		case eCHAR_DRAGON:
			fMeshHeight = 1.8f;			
			break;
		case eCHAR_VALKYRIE:
			fMeshHeight = 1.7f;			
			break;
		case eCHAR_SHADOW:
			fMeshHeight = 1.8f;			
			break;
		case eCHAR_MAGICIAN:
			fMeshHeight = 1.7f;			
			break;

		}

		if(pPlayer->IsTransForm())//���� ���϶�
		{
			fMeshHeight = 2.2f;
			fMeshWidth = 1.2f;
		}

		// Calc ViewPortWithRect
		CControlWZ * pControl = (CControlWZ *)getControl(BattleLobby_BT_VIEW_USER_1+i);
		if (!pControl)
		{
			assert(!"CHECK THE 63_BattleLobby.IWZ");
			continue;
		}

		rcSize = pControl->GetSizeRect();
		rcViewport = g_InterfaceManager.CalcuateViewPortWithRect(rcSize);

		if ( 0 == pUserSlot->GetTeam() )
		{
			// ���� ����.
			g_InterfaceManager.RenderObjectOnRect(pPlayer,rcViewport,fMeshWidth,fMeshHeight);
		}
		else
		{
			WzColor wzColor = 0xFFFF6FFF;	//	�Ӱ�

			switch (pUserSlot->GetTeam())
			{
			case 1:
				{
					wzColor = WzColor_RGBA(0, 50, 121, 100);	//	û
				}
				break;
			case 2:
				{
					wzColor = WzColor_RGBA(137, 9, 0, 100);	//	��
				}
				break;
			case 3:
				{
					wzColor = 0xFF8F8F8F;	//	���
				}
				break;
			default:
				break;
			}

			g_InterfaceManager.RenderRect(rcSize, wzColor);

			g_InterfaceManager.RenderObjectOnRect(pPlayer, rcViewport, fMeshWidth, fMeshHeight);
		}


		switch (pUserSlot->GetSlotState())
		{
		case UserSlot::FILL:
			{
			}
			break;

		case UserSlot::READY:
			{
				// ����ٰ� Ready Texture�� ����� ��´�.

				if (m_hTexture[USE_TEXTURE_READY] != INVALID_HANDLE_VALUE) 
				{
					float x = (float)((rcSize.left+rcSize.right) - m_pTextureInfo[USE_TEXTURE_READY]->m_nImageWidth)/2;
					float y = (float)(rcSize.top + rcSize.bottom + m_pTextureInfo[USE_TEXTURE_READY]->m_nImageHeight) /2.f;

					g_pSunRenderer->RenderTexture( 
						m_hTexture[USE_TEXTURE_READY],
						x,
						y,
						(float)m_pTextureInfo[USE_TEXTURE_READY]->m_nImageWidth,
						(float)m_pTextureInfo[USE_TEXTURE_READY]->m_nImageHeight,
						0.0f,
						0.0f,
						0.0f,
						0.0f,
						NULL,
						0.0f
						);
				}
			}
			break;

		case UserSlot::OPENED:
		case UserSlot::BLOCK:
			{
			}
			break;

		default:
			{	
				assert(0);
			}
			break;
		}
	}

	// ī�޶� ���� �԰� �����°Ͷ����� �̺κ��� �־��ش�.
	if ( g_pHero )
	{
		g_pHero->SetCameraAlpha(TRUE);
	}

	BattleRoomEntry & rEntry =  m_pBattleZoneMan->GetBattleRoomEntry();
	
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    RECT rect_guild_pvp = m_rtSuperRoomIcon;
#ifndef  __CHINA_PCBANG_SUPER_ROOM_BLOCK
	if( rEntry.m_RoomInfo.IsSpecialType(eROOM_SPECIAL_PCBANG) )
	{
        g_InterfaceManager.RenderBattleRoomIcon(
            InterfaceManager::eBATTLE_ICON_PCBANG_SUPERMASTER_ROOM,
            &m_rtSuperRoomIcon);
        rect_guild_pvp.left += 20;
        rect_guild_pvp.right += 20;
	}
	else
#endif
	if ( rEntry.m_RoomInfo.IsSpecialType(eROOM_SPECIAL_ITEM) )
	{
        g_InterfaceManager.RenderBattleRoomIcon(
            InterfaceManager::eBATTLE_ICON_ITEM_SUPERMASTER_ROOM,
            &m_rtSuperRoomIcon);
        rect_guild_pvp.left += 20;
        rect_guild_pvp.right += 20;
	}

	if( rEntry.m_RoomInfo.IsSpecialType(eROOM_SPECIAL_GUILD_PVP) )
	{
        g_InterfaceManager.RenderBattleRoomIcon(
            InterfaceManager::eBATTLE_ICON_GUILD_PVP_ROOM,
            &rect_guild_pvp);
	}

#else
#ifndef  __CHINA_PCBANG_SUPER_ROOM_BLOCK
	if( rEntry.m_RoomInfo.m_SpecialType == eROOM_SPECIAL_PCBANG_SUPER_ROOM)
	{
        g_InterfaceManager.RenderBattleRoomIcon(
            InterfaceManager::eBATTLE_ICON_PCBANG_SUPERMASTER_ROOM,
            &m_rtSuperRoomIcon);
	}
	else
#endif
	if( rEntry.m_RoomInfo.m_SpecialType == eROOM_SPECIAL_ITEM1_SUPER_ROOM )
	{
        g_InterfaceManager.RenderBattleRoomIcon(
            InterfaceManager::eBATTLE_ICON_ITEM_SUPERMASTER_ROOM,
            &m_rtSuperRoomIcon);
	}
#endif

}

//----------------------------------------------------------------------------
/** 
	@remark
	 -[prev] : 1. ����Ű�� �޾Ƴ��� ���¿����Ѵ�.		   
			   2. BattleRoomEntry �� �޾Ƴ��� ���¿����Ѵ�. 
*/
VOID	
BattleLobby::OnShowWindow(BOOL val)
{
    if (val)
	{
		GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST )->ShowInterface(FALSE);
		GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE )->ShowInterface(FALSE);
        GET_DIALOG(IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE_EP2)->ShowInterface(FALSE);

        CCtrlStaticWZ* titleJunk = (CCtrlStaticWZ *)GetControlWZ( StrToWzID("T999") );
        if (titleJunk)
        {
            titleJunk->ShowWindowWZ(WZ_HIDE);
        }

		g_HeroInput.ClearPickingTarget();

		if (g_pMap)
		{
			g_pMap->SetRenderFlag(FALSE);
		}
		
		initSetUIForBasicInfo();
        
		if(m_pBattleZoneMan && m_pBattleZoneMan->IsMaster())
		{
			this->initSetUIForMaster();
		}
		else
		{
			this->initSetUIForMember();
		}

		switch ( m_pBattleZoneMan->GetBattleRoomEntry().GetType() )
		{
		case eZONETYPE_PVP:
			{
				this->initPVPUI();
			}
			break;

		default:
			break;
		}

		if (g_pMap)
		{
			g_pMap->SetRenderFlag(FALSE);
		}

		switch ( m_pBattleZoneMan->GetBattleRoomEntry().GetType() )
		{
		case eZONETYPE_MISSION:
			{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
				uiBattleZoneReadyMissionConfig * missionCfg = GET_CAST_DIALOG( uiBattleZoneReadyMissionConfig,
															  IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_MISSION_CFG );
                if (missionCfg)
                {
                    missionCfg->Clear();
                    missionCfg->ShowInterface(TRUE);
                }
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
			}
			break;

		case eZONETYPE_PVP:
			{
				uiBattleZoneReadyPVPConfig * pvpCfg = GET_CAST_DIALOG( uiBattleZoneReadyPVPConfig, IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_PVP_CFG );
				if (pvpCfg)
				{
					pvpCfg->Clear();
					pvpCfg->ShowInterface( TRUE );
				}
			}
			break;

        case eZONETYPE_CHUNTING:
            {
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
				uiBattleZoneReadyCHuntingConfig * chunting_config_ptr = GET_CAST_DIALOG( uiBattleZoneReadyCHuntingConfig,
													IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_CHUNTING_CFG );

                if (chunting_config_ptr)
                {
                    if (chunting_config_ptr->Clear() == true)
                    {
                        chunting_config_ptr->ShowInterface(TRUE);
                    }
                }
#endif
            }
            break;
		}

		g_InterfaceManager.DeAttachFirstDialog(IM_CHAT_MANAGER::CHAT_DIALOG);
		g_InterfaceManager.ShowWindow(IM_CHAT_MANAGER::CHAT_DIALOG, FALSE);
		g_InterfaceManager.AttachFirstDialog(IM_CHAT_MANAGER::CHAT_DIALOG);

        //
        // setup almost top to chat dialog
        //
        AttachChatDialog();
	
        uiPartyMan* partyMan =
            static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
        if (partyMan)
        {
            partyMan->uiClearAll();
        }

		m_rtSuperRoomIcon.left		= m_pCtrlStaticRoomNo->GetSizeRect().left - 25;
		m_rtSuperRoomIcon.top		=  m_pCtrlStaticRoomNo->GetSizeRect().top;
		m_rtSuperRoomIcon.right		= m_rtSuperRoomIcon.left + 20;
		m_rtSuperRoomIcon.bottom	= m_rtSuperRoomIcon.top + 20;
    }
	else
	{
		this->initInfo();
        m_packetStatus = 0;

		m_pCtrlImageStaticTeamChange->ShowWindowWZ( WZ_HIDE );

		// �ֻ��� ������ ����
		g_InterfaceManager.DeAttachFirstDialog(IM_CHAT_MANAGER::CHAT_DIALOG);
		ChatDialog * pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
		if (pChatDlg) 
		{
			pChatDlg->MoveDefaultPosition();
			pChatDlg->ShowWindow(FALSE);
		}
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
        GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_CHUNTING_CFG )->ShowInterface( FALSE );
  
        GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_MISSION_CFG )->ShowInterface( FALSE );
#endif//#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
     
        GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_PVP_CFG )->ShowInterface( FALSE );
        uiPartyMan* partyMan =
            static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
        if (partyMan)
        {
            partyMan->uiShowAll();
        }
    }
}

//----------------------------------------------------------------------------
/** 
*/
void 
BattleLobby::AttachChatDialog()
{
	g_InterfaceManager.AttachFirstDialog(IM_CHAT_MANAGER::CHAT_DIALOG);
	g_InterfaceManager.ShowWindow(IM_CHAT_MANAGER::CHAT_DIALOG, TRUE);

    ChatDialog * pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
    if (pChatDlg)
    {
        RECT rcClient;
        this->GetDialogWZ()->GetClientRect(&rcClient);
        pChatDlg->MoveBattleZonePosition( rcClient );
    }

    //ä�ð��� �Ӽ�â�� ���� ǥ��
    g_InterfaceManager.AttachFirstDialog(IM_CHAT_MANAGER::CHAT_MENU_DIALOG);
    g_InterfaceManager.AttachFirstDialog(IM_CHAT_MANAGER::CHAT_PROPERTY_DIALOG);
    g_InterfaceManager.AttachFirstDialog(IM_CHAT_MANAGER::CHAT_PERSONAL_OPTION);
}

//----------------------------------------------------------------------------
/** 
*/
CControlWZ *	
BattleLobby::getControl( POSTYPE AtPos )
{
	if( AtPos < Dialog_Max )
	{
		return GetControlWZ( m_wzId[AtPos] );
	}
	else
	{
		return NULL;
	}
}


//----------------------------------------------------------------------------
/** 
*/
POSTYPE			
BattleLobby::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_Max;
}


//----------------------------------------------------------------------------
/** 
*/
VOID			
BattleLobby::initInfo()
{
	if(m_pBattleZoneMan)
		m_pBattleZoneMan->FlushUserSlotSet();

	this->m_packetStatus = 0;
}




//----------------------------------------------------------------------------
/**
	@desc 
	 - �� ��ȣ, Ÿ��Ʋ ����, ��й�ȣ ǥ��
	@remark
	 - BattleRoomEntry ����
*/
VOID			
BattleLobby::initSetUIForBasicInfo()
{
	if(!m_pBattleZoneMan)
		return;

	_itot( m_pBattleZoneMan->GetBattleRoomEntry().GetSerial(), m_szResult, 10);
	GlobalFunc::SetCaption( m_pCtrlStaticRoomNo, m_szResult );

	GlobalFunc::SetCaption( m_pCtrlStaticRoomTitle, (TCHAR *)m_pBattleZoneMan->GetBattleRoomEntry().GetTitle() );

	GlobalFunc::SetCaption( m_pCtrlStaticRoomPW, (TCHAR *)m_pBattleZoneMan->GetBattleRoomEntry().GetPassword() );

	// ��ư ǥ�� ����
	int iStringID = 0;

	if(m_pBattleZoneMan->IsMaster())
	{
		iStringID = 115;
	}
	else
	{
		iStringID = 110;
	}
	g_InterfaceManager.GetInterfaceString(iStringID, this->m_szResult, 512);
	GlobalFunc::SetCaption( m_pCtrlTIStartNReady, m_szResult );

	// �ִ� ���������� UserSlotSet�� �⺻������ �������ش�.
	m_pBattleZoneMan->SetDefaultUserSlot();

	if ( this->m_pCtrlImageStaticTeamChange )
	{
		this->m_pCtrlImageStaticTeamChange->ShowWindowWZ( WZ_HIDE );
	}

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void BattleLobby::ChangeRoomTitle(TCHAR* szMessage)
{
	GlobalFunc::SetCaption(m_pCtrlStaticRoomTitle, szMessage);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
/**
	@desc
	 - ���常�� ���� �������̽� �ʱ� ó��
*/
VOID			
BattleLobby:: initSetUIForMaster()
{
	int i = 0;
	for (i; i < MAX_VIEW_USER_SLOT; ++i)
	{
		m_pCtrlBtnBan[i]->ShowWindowWZ( WZ_SHOW );
	}
}


//----------------------------------------------------------------------------
/**
	@desc
	 - ������� ���� �������̽� �ʱ� ó��
*/
VOID
BattleLobby::initSetUIForMember()
{
	int i = 0;
	for (i; i < MAX_VIEW_USER_SLOT; ++i)
	{
		m_pCtrlBtnBan[i]->ShowWindowWZ( WZ_HIDE );
	}
}


//----------------------------------------------------------------------------
/** 
	@desc 
	 - ���� ����, ���� ǥ��, �� ���� check

	@remark 
	 - UserSlotSet ����
*/
VOID			
BattleLobby::updateUserSlotUI()
{

	if(!m_pBattleZoneMan)
		return;

	DWORD dwPlayerKey = 0;
	UserSlot * pUserSlot = NULL;

	for (int i = 0; i < m_pBattleZoneMan->GetMaxUserSlot(); ++i)	
	{
		pUserSlot	= m_pBattleZoneMan->GetUserSlotByIndex(i);

		assert(pUserSlot);

		if(!pUserSlot)
			continue;

		if (i != 0)
		{
			// ������ ���� �������� �ʴ´�.
			// i - 1�� ������ ������ ���ϰ� ��Ʈ���� �����ϱ� ����.
			m_pCtrlCheckBtnUserSlotBlock[i-1]->SetCheckState( (pUserSlot->GetSlotState() == UserSlot::BLOCK)? TRUE : FALSE );

			if(m_pBattleZoneMan && m_pBattleZoneMan->IsMaster())
			{
				if ( pUserSlot->GetSlotState() == UserSlot::FILL ||
					 pUserSlot->GetSlotState() == UserSlot::READY )
				{
					m_pCtrlBtnBan[i-1]->ShowWindowWZ( WZ_SHOW );
				}
				else
				{ 
					m_pCtrlBtnBan[i-1]->ShowWindowWZ( WZ_HIDE );
				}
			}
		}

		dwPlayerKey = pUserSlot->GetPlayerKey();
	
		Player * pPlayer = static_cast<Player *>( g_ObjectManager.GetObject( dwPlayerKey ) );

		if (pPlayer)
		{
			_itot(pPlayer->GetLevel(), m_szResult, 10);
			GlobalFunc::SetCaption( m_pCtrlStaticUserLevel[i], m_szResult );
			GlobalFunc::SetCaptionSimple(m_pCtrlStaticUserName[i], const_cast<TCHAR*>(pPlayer->GetName()));
		}
		else
		{
			GlobalFunc::SetCaption( m_pCtrlStaticUserLevel[i], _T("") );
			GlobalFunc::SetCaption( m_pCtrlStaticUserName[i], _T("") );
		}
	}

	if(m_pBattleZoneMan && m_pBattleZoneMan->IsMaster())
	{
		// ��ư ǥ�� ����
		int iStringID = 115;

		g_InterfaceManager.GetInterfaceString(iStringID, this->m_szResult, 512);
		GlobalFunc::SetCaption( m_pCtrlTIStartNReady, m_szResult );

		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString( 70242, szMessage, INTERFACE_STRING_LENGTH );

		GlobalFunc::SetCaption( this->m_pCtrlTIStartNReady, (m_pBattleZoneMan->IsReadyForStart())? m_szResult : szMessage );
	}
}

//----------------------------------------------------------------------------
/**
	@desc 
	 - �� Ÿ��

	@remark 
	 - m_BattleRoomEntry ����
*/
VOID			
BattleLobby::updateZoneType()
{
	// �� Ÿ��
	if ( m_pCtrlStaticZoneType )
	{
		DWORD dwStringID = 604;

		switch (m_pBattleZoneMan->GetBattleRoomEntry().GetType())
		{
		case eZONETYPE_MISSION:
			{
				dwStringID = 640;
			}
			break;

		case eZONETYPE_PVP:
			{
				dwStringID = 685;
			}
			break;


        case eZONETYPE_CHUNTING:
            {
                dwStringID = 604;
            }
            break;

		default:
			{
				dwStringID = 604;
			}
			break;
		}

		m_pCtrlStaticZoneType->SetTextID( dwStringID );
	}
}


//----------------------------------------------------------------------------
/**
	@desc 
	 - �ʳ���, ���� ����, ���巹�� ǥ�� 

	@remark 
	 - m_BattleRoomEntry ����
*/
VOID			
BattleLobby::updateMapInfoUI()
{
	const sMAPINFO * pMapInfo = (sMAPINFO *)m_pBattleZoneMan->GetBattleRoomEntry().GetCurMapInfo();

	if (pMapInfo)
	{
		// �� ����
		g_InterfaceManager.GetInterfaceString(pMapInfo->dwNCode, m_szResult, 512);
		GlobalFunc::SetCaption( m_pCtrlStaticMapName, m_szResult );

		BASE_RequireFieldLevelInfo * pInfo = NULL;

		if (eZONETYPE_MISSION == m_pBattleZoneMan->GetBattleRoomEntry().GetType())
		{
			pInfo = RequireFieldLevelInfoParser::Instance()->GetRequireFieldLevelInfo(
						(WORD)m_pBattleZoneMan->GetBattleRoomEntry().GetMapCode(),
						INVALID_DIFFICULT);
		}
		else
		{
			pInfo = RequireFieldLevelInfoParser::Instance()->GetRequireFieldLevelInfo(
						(WORD)m_pBattleZoneMan->GetBattleRoomEntry().GetMapCode(),
						(BYTE)m_pBattleZoneMan->GetBattleRoomEntry().GetDifficult());
		}

		// ���� ���� 
		int iRequireMinLV = 0, iRequireMaxLV = 0;
		if (pInfo)
		{
			iRequireMinLV = pInfo->m_wMinlv;
			iRequireMaxLV = pInfo->m_wMaxlv;
		}
		else
		{
			iRequireMinLV = pMapInfo->minLV;
			iRequireMaxLV = pMapInfo->maxLV;
		}

		ZeroMemory(m_szResult, sizeof(m_szResult));
		Snprintf( m_szResult, MAX_RESULT_STRING_LENGTH-1,  _T("%3d / %3d"), iRequireMinLV, iRequireMaxLV );
		GlobalFunc::SetCaption( m_pCtrlStaticRecommendLevel, m_szResult );
	}

	// ���� ����
	ZeroMemory(m_szResult, sizeof(m_szResult));
	Snprintf( m_szResult, MAX_RESULT_STRING_LENGTH-1, _T("%3d / %3d"), m_pBattleZoneMan->GetBattleRoomEntry().GetMinLevel(), m_pBattleZoneMan->GetBattleRoomEntry().GetMaxLevel() );
	GlobalFunc::SetCaption( m_pCtrlStaticLimitLevel, m_szResult );
}


//----------------------------------------------------------------------------
/**
	@desc
	 - ���Ѽ��� ǥ��(������ �ʵ�, ���̵�, ���ʽ� Ÿ��)

	@remark 
	 - m_BattleRoomEntry.m_AdditionalInfo ����
*/


//----------------------------------------------------------------------------
/**
*/
VOID			
BattleLobby::updatePVPUI()
{
	if ( m_pBattleZoneMan->GetBattleRoomEntry().GetType() != eZONETYPE_PVP )
	{
		if ( m_pCtrlImageStaticTeamChange )
		{
			this->m_pCtrlImageStaticTeamChange->ShowWindowWZ(WZ_HIDE);
		}

		return;
	}

	if (ePVP_TEAM_MODE == m_pBattleZoneMan->GetBattleRoomEntry().GetPVPMode())
	{
		if ( m_pCtrlImageStaticTeamChange )
		{
			this->m_pCtrlImageStaticTeamChange->ShowWindowWZ(WZ_SHOW);
		}
	}
	else
	{
		if ( m_pCtrlImageStaticTeamChange )
		{
			this->m_pCtrlImageStaticTeamChange->ShowWindowWZ(WZ_HIDE);
		}
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID			
BattleLobby::initPVPUI()
{
	if ( eZONETYPE_PVP != m_pBattleZoneMan->GetBattleRoomEntry().GetType() )
	{
		return;
	}

	// �� ��ü UI Ŵ.
	if ( this->m_pCtrlImageStaticTeamChange )
	{
		this->m_pCtrlImageStaticTeamChange->ShowWindowWZ( WZ_SHOW );
	}
}

//----------------------------------------------------------------------------
/**
*/
BOOL			
BattleLobby::BlockPlayer(int index)
{
	if(!m_pBattleZoneMan)
		return FALSE;

	assert(index >= 1 && index < MAX_VIEW_USER_SLOT);		
	
	// ���常 ���� �Ҽ��ִ�
	if(m_pBattleZoneMan && !m_pBattleZoneMan->IsMaster())
	{
		return FALSE;
	}

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	int iCurUser = m_pBattleZoneMan->GetNumUser();	
	m_pBattleZoneMan->m_iPendingPlayerIndex = index;

	UserSlot * pUserSlot = m_pBattleZoneMan->GetUserSlotByIndex(index);

	if (!pUserSlot)
	{
		assert(0);
		return FALSE;
	}

	// �ִ� ������ �������ش�. �ּ� �ο��� ���� �¸��ϰ� �ѻ����̴�.
	switch (pUserSlot->GetSlotState())
	{
	case UserSlot::BLOCK:
		{
			// ���� ������ �����������
			// �ִ� ���� ���ѿ� ������� �ʴ���?
			int iOpenedSlot = m_pBattleZoneMan->GetNonBlockUserSlotNum();		

			if ( iOpenedSlot >= m_pBattleZoneMan->GetBattleRoomEntry().GetCurLimitMaxUser() ) 
			{
				// �ο��� ���ѿ� ���� �˴ϴ�.
				const int	c_iNotChangeMinNumber = 70245;
				g_InterfaceManager.GetInterfaceString( c_iNotChangeMinNumber, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, szMessage);
					g_InterfaceManager.ShowMessageBox(szMessage, 10000, NULL, 0);
				return FALSE;
			}

			++iOpenedSlot;

			assert(	iOpenedSlot <= MAX_VIEW_USER_SLOT );	

			m_pBattleZoneMan->ModifyPlayerNums( iOpenedSlot);
		}
		break;

	case UserSlot::OPENED:
		{
			// ���µ� ������ ���������

			int iBlockedNum = m_pBattleZoneMan->GetBlockUserSlotNum();	
			int iCanBlockNum = m_pBattleZoneMan->GetMaxUserSlot() - iBlockedNum;

			int iMaxLimitUser= m_pBattleZoneMan->GetBattleRoomEntry().GetCurLimitMinUser();

			// TODO : �ϴ� Min User�� �����ϱ� 1������.
			// Min User ���� �۰� ���Ҽ� ����.
			// �ּ� ���� ���ѿ� ������� �ʴ���?
			if (iCanBlockNum <= iMaxLimitUser ) 
			{
				// �ο��� ���ѿ� ���� �˴ϴ�.
				const int	c_iNotChangeMinNumber = 70244;
				g_InterfaceManager.GetInterfaceString( c_iNotChangeMinNumber, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, szMessage);
				return FALSE;
			}

			int iChangeNum = m_pBattleZoneMan->GetMaxUserSlot() - (iBlockedNum + 1);	
			m_pBattleZoneMan->ModifyPlayerNums( iChangeNum );
			
		}
		break;

	case UserSlot::FILL:
	case UserSlot::READY:
		{
			// ����.
		}
		break;

	default:
		assert(0);
		return FALSE;
		break;
	}

	return TRUE;
}


//----------------------------------------------------------------------------
/** 
*/
//void
//BattleLobby::UpdatePosition()
//{
//	//RECT rcDlgSize;
//	//GetDialogWZ()->GetDlgSize(&rcDlgSize);
//
//	//POINT pt = g_pApplication->Get2DModeMapping();
//
//	//RECT rcInterface;
//	//rcInterface.left = rcInterface.top = 0;
//	//rcInterface.right = pt.x;
//	//rcInterface.bottom = pt.y;
//	//
//	//int x = (rcInterface.right / 2) - (rcDlgSize.right / 2);
//	//int y = (rcInterface.bottom / 2) - (rcDlgSize.bottom / 2);
//
//	//MoveWindowWZ( (float)x, (float)y );
//
//}

void BattleLobby::showZoneStatusWindow()
{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	uiBase * pBattleZoneReadyChuntingCfg = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_CHUNTING_CFG );

	if( pBattleZoneReadyChuntingCfg && pBattleZoneReadyChuntingCfg->IsVisible() && !pBattleZoneReadyChuntingCfg->HaveFocus() )
	{
		pBattleZoneReadyChuntingCfg->ShowWindow( TRUE );						
	}

	uiBase * pBattleZoneReadyPVPConfig = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_PVP_CFG );
	uiBase * pBattleZoneReadyMissioncfg = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_MISSION_CFG );

	if( pBattleZoneReadyPVPConfig && pBattleZoneReadyPVPConfig->IsVisible() && !pBattleZoneReadyPVPConfig->HaveFocus() )
	{
		pBattleZoneReadyPVPConfig->ShowWindow( TRUE );
	}
	else if( pBattleZoneReadyMissioncfg && pBattleZoneReadyMissioncfg->IsVisible() && !pBattleZoneReadyMissioncfg->HaveFocus() )
	{
		pBattleZoneReadyMissioncfg->ShowWindow( TRUE );						
	}
#endif//#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)

}