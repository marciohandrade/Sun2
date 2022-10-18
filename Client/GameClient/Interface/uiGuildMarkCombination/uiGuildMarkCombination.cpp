#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMarkCombination.h"
#include "GuildMarkInfoParser.h"
#include "uiGuildMan/uiGuildMan.h"
#include "InterfaceManager.h"
#include "ItemManager.h"
#include "GlobalFunc.h"
#include "uiGuildMarkRegisteConfirm/uiGuildMarkRegisteConfirm.h"

WzID uiGuildMarkCombination::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("BT00"),		//DLGPOS_BTN_TARGET_ITEM,
	StrToWzID("ST00"),		//DLGPOS_TXT_BACKGROUND_PAGE,
	StrToWzID("BT61"),	//DLGPOS_BTN_BACKGROUND_LEFT,
	StrToWzID("BT62"),	//DLGPOS_BTN_BACKGROUND_RIGHT,
	StrToWzID("BT01"),	//DLGPOS_PIC_BACKGROUND1,
	StrToWzID("BT02"),	//DLGPOS_PIC_BACKGROUND2,
	StrToWzID("BT03"),	//DLGPOS_PIC_BACKGROUND3,
	StrToWzID("BT04"),	//DLGPOS_PIC_BACKGROUND4,
	StrToWzID("BT05"),	//DLGPOS_PIC_BACKGROUND5,
	StrToWzID("BT06"),	//DLGPOS_PIC_BACKGROUND6,
	StrToWzID("BT07"),	//DLGPOS_PIC_BACKGROUND7,
	StrToWzID("BT08"),	//DLGPOS_PIC_BACKGROUND8,
	StrToWzID("BT09"),	//DLGPOS_PIC_BACKGROUND9,
	StrToWzID("BT10"),	//DLGPOS_PIC_BACKGROUND10,
	StrToWzID("BT11"),	//DLGPOS_PIC_BACKGROUND11,
	StrToWzID("BT12"),	//DLGPOS_PIC_BACKGROUND12,
	StrToWzID("BT13"),	//DLGPOS_PIC_BACKGROUND13,
	StrToWzID("BT16"),	//DLGPOS_PIC_BACKGROUND14,
	StrToWzID("BT17"),	//DLGPOS_PIC_BACKGROUND15,
	StrToWzID("BT18"),	//DLGPOS_PIC_BACKGROUND16,
	StrToWzID("BT19"),	//DLGPOS_PIC_BACKGROUND17,
	StrToWzID("BT20"),	//DLGPOS_PIC_BACKGROUND18,
	StrToWzID("BT21"),	//DLGPOS_PIC_BACKGROUND19,
	StrToWzID("BT22"),	//DLGPOS_PIC_BACKGROUND20,
	StrToWzID("BT23"),	//DLGPOS_PIC_BACKGROUND21,
	StrToWzID("BT24"),	//DLGPOS_PIC_BACKGROUND22,
	StrToWzID("BT25"),	//DLGPOS_PIC_BACKGROUND23,
	StrToWzID("BT26"),	//DLGPOS_PIC_BACKGROUND24,
	StrToWzID("BT27"),	//DLGPOS_PIC_BACKGROUND25,
	StrToWzID("BT28"),	//DLGPOS_PIC_BACKGROUND26,
		
	StrToWzID("ST01"),	//DLGPOS_TXT_PATTERN_PAGE,
	StrToWzID("BT64"),	//DLGPOS_BTN_PATTERN_LEFT,
	StrToWzID("BT63"),	//DLGPOS_BTN_PATTERN_RIGHT,
	StrToWzID("BT31"),	//DLGPOS_PIC_PATTERN1,
	StrToWzID("BT32"),	//DLGPOS_PIC_PATTERN2,
	StrToWzID("BT33"),	//DLGPOS_PIC_PATTERN3,
	StrToWzID("BT34"),	//DLGPOS_PIC_PATTERN4,
	StrToWzID("BT35"),	//DLGPOS_PIC_PATTERN5,
	StrToWzID("BT36"),	//DLGPOS_PIC_PATTERN6,
	StrToWzID("BT37"),	//DLGPOS_PIC_PATTERN7,
	StrToWzID("BT38"),	//DLGPOS_PIC_PATTERN8,
	StrToWzID("BT39"),	//DLGPOS_PIC_PATTERN9,
	StrToWzID("BT40"),	//DLGPOS_PIC_PATTERN10,
	StrToWzID("BT41"),	//DLGPOS_PIC_PATTERN11,
	StrToWzID("BT42"),	//DLGPOS_PIC_PATTERN12,
	StrToWzID("BT43"),	//DLGPOS_PIC_PATTERN13,
	StrToWzID("BT46"),	//DLGPOS_PIC_PATTERN14,
	StrToWzID("BT47"),	//DLGPOS_PIC_PATTERN15,
	StrToWzID("BT48"),	//DLGPOS_PIC_PATTERN16,
	StrToWzID("BT49"),	//DLGPOS_PIC_PATTERN17,
	StrToWzID("BT50"),	//DLGPOS_PIC_PATTERN18,
	StrToWzID("BT51"),	//DLGPOS_PIC_PATTERN19,
	StrToWzID("BT52"),	//DLGPOS_PIC_PATTERN20,
	StrToWzID("BT53"),	//DLGPOS_PIC_PATTERN21,
	StrToWzID("BT54"),	//DLGPOS_PIC_PATTERN22,
	StrToWzID("BT55"),	//DLGPOS_PIC_PATTERN23,
	StrToWzID("BT56"),	//DLGPOS_PIC_PATTERN24,
	StrToWzID("BT57"),	//DLGPOS_PIC_PATTERN25,
	StrToWzID("BT58"),	//DLGPOS_PIC_PATTERN26,

	StrToWzID("C001"),	//DLGPOS_CHECK_BACKGROUND_CIRCLE,
	StrToWzID("C002"),	//DLGPOS_CHECK_BACKGROUND_SHIELD,
	StrToWzID("C003"),	//DLGPOS_CHECK_BACKGROUND_ETC,
	StrToWzID("C004"),	//DLGPOS_CHECK_PATTERN_FIGURE,
	StrToWzID("C005"),	//DLGPOS_CHECK_PATTERN_SYMBOL,
	StrToWzID("C006"),	//DLGPOS_CHECK_PATTERN_ETC,
		//
	StrToWzID("BT65"),	//DLGPOS_BTN_OK,
	StrToWzID("BT66"),	//DLGPOS_BTN_CANCLE,
	StrToWzID("B999"),	//DLGPOS_BTN_EXIT,

				
};
//------------------------------------------------------------------------------
/**
*/
uiGuildMarkCombination::uiGuildMarkCombination(InterfaceManager * pUIMan) : uiBase(pUIMan)
{

}
//------------------------------------------------------------------------------
/**
*/
uiGuildMarkCombination::~uiGuildMarkCombination()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkCombination::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);



	for( int i = 0; i < MAX_GUILD_MARK; i ++)
	{
		m_stBackGroundItem.pButton[i] = (CCtrlButtonWZ * ) getControl(DLGPOS_PIC_BACKGROUND1 +i);
		assert(m_stBackGroundItem.pButton[i]);
		m_stBackGroundItem.index[i] = INVALID_GUILD_MARK_INDEX;

		m_stPatternItem.pButton[i] = (CCtrlButtonWZ *) getControl(DLGPOS_PIC_PATTERN1 +i);
		assert(m_stPatternItem.pButton[i]);
		m_stPatternItem.index[i] = INVALID_GUILD_MARK_INDEX;
	}
	for(int i =0; i < MAX_FILTER_CHECK; ++i)
	{
		m_stPatternItem.pCheck[i] = ( CCtrlButtonCheckWZ *) getControl(DLGPOS_CHECK_PATTERN_FIGURE +i);
		m_stBackGroundItem.pCheck[i] = ( CCtrlButtonCheckWZ *) getControl(DLGPOS_CHECK_BACKGROUND_CIRCLE +i);
	}
	m_stBackGroundItem.pStaticPage = (CCtrlStaticWZ *) getControl(DLGPOS_TXT_BACKGROUND_PAGE);
	assert(m_stBackGroundItem.pStaticPage);
	m_stPatternItem.pStaticPage = (CCtrlStaticWZ *) getControl(DLGPOS_TXT_PATTERN_PAGE);
	assert(m_stPatternItem.pStaticPage);
	m_pTargetMark = (CCtrlButtonWZ *) getControl(DLGPOS_BTN_TARGET_ITEM);
	assert(m_pTargetMark);




	m_bShow = false;

	m_pGuildMan = (uiGuildMan * )g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
	assert(m_pGuildMan);

	InitstGuildMarkItem();
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkCombination::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkCombination::OnUpdateSolarDialog()
{
	if( !m_bShow )
	{
		ShowInterface(FALSE);
	}

}	
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkCombination::OnRenderSolarDialog()
{
	RenderGuildMark(&m_stPatternItem);		// 패턴 그리기
	RenderGuildMark(&m_stBackGroundItem);	// 백그라운드 그리기

	RenderTargetGuildMark(); // 타겟 결과 마크 그리기
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkCombination::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkCombination::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_bShow = true;
		InitstGuildMarkItem();
		MoveCenter();
	}
	else
	{
		m_bShow = false;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkCombination::MessageProc(SI_MESSAGE * pMessage)
{
	int iRecvMsgControPos = getControlIDToPos(pMessage->dwCtrlID);
	switch( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			if( iRecvMsgControPos >= DLGPOS_PIC_BACKGROUND1 && iRecvMsgControPos <=DLGPOS_PIC_BACKGROUND26 )
			{
				int arryIndex = iRecvMsgControPos - DLGPOS_PIC_BACKGROUND1;
				m_stBackGroundItem.iSelIndex = m_stBackGroundItem.index[arryIndex];
			}
			else if( iRecvMsgControPos >=DLGPOS_PIC_PATTERN1 && iRecvMsgControPos <=DLGPOS_PIC_PATTERN26)
			{
				int arryIndex = iRecvMsgControPos - DLGPOS_PIC_PATTERN1;
				m_stPatternItem.iSelIndex = m_stPatternItem.index[arryIndex];
			}
			else
			{
				switch( iRecvMsgControPos)
				{
				case DLGPOS_BTN_OK:
					{
						if( m_stBackGroundItem.iSelIndex <= 0 || m_stPatternItem.iSelIndex <= 0 )
						{
							//6162 배경과 문양을 각각 선택해야 길드 마크가 조합되며 길드 마크로 등록 및 변경할 수 있습니다.
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6162));
						}
						else
						{
							POSTYPE OutPos;
							BOOL foundGuildMarkItem = ItemManager::Instance()->FindFirstItem(eITEMTYPE_GUILDMARK_SEAL, OutPos );
							if( foundGuildMarkItem )
							{
								uiGuildMarkRegisteConfirm * pMarkRegisteConfirm = GET_CAST_DIALOG( uiGuildMarkRegisteConfirm, IM_GUILD_MANAGER::GUILD_MARK_REGISTE_CONFIRM );

								if( pMarkRegisteConfirm )
								{
									pMarkRegisteConfirm->SetMarkIndex(m_stBackGroundItem.iSelIndex, m_stPatternItem.iSelIndex);
									pMarkRegisteConfirm->ShowInterface(TRUE);
									m_bShow = false;
								}
							}
							else
							{
								//6161 길드 인장이 인벤토리에 존재하지 않습니다.
								GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,g_InterfaceManager.GetInterfaceString(6161));
							}
						}
					}
					break;
				case DLGPOS_BTN_CANCLE:
				case DLGPOS_BTN_EXIT:
					{
						m_bShow = false;
					}
					break;

				case DLGPOS_BTN_BACKGROUND_LEFT:
					{
						if( m_stBackGroundItem.iPage > 0)
						{
							m_stBackGroundItem.iPage--;
							RegisterGuildMarkData(&m_stBackGroundItem);
						}
					}
					break;

				case DLGPOS_BTN_BACKGROUND_RIGHT:
					{
						if( m_stBackGroundItem.iMaxPage > m_stBackGroundItem.iPage)
						{
							m_stBackGroundItem.iPage++;
							RegisterGuildMarkData(&m_stBackGroundItem);
						}	
					}
					break;

				case DLGPOS_BTN_PATTERN_LEFT:
					{
						if( m_stPatternItem.iPage > 0)
						{
							m_stPatternItem.iPage--;
							RegisterGuildMarkData(&m_stPatternItem);
						}
					}
					break;

				case DLGPOS_BTN_PATTERN_RIGHT:
					{
						if( m_stPatternItem.iMaxPage > m_stPatternItem.iPage)
						{
							m_stPatternItem.iPage++;
							RegisterGuildMarkData(&m_stPatternItem);
						}
					}
					break;
				case DLGPOS_CHECK_BACKGROUND_CIRCLE:
					{
						if( m_stBackGroundItem.iCurFiltter != eSUB_TYPE_BACKGROUND_CIRCUL)
						{
							m_stBackGroundItem.iCurFiltter = eSUB_TYPE_BACKGROUND_CIRCUL;
							m_stBackGroundItem.iPage =0;
							RegisterGuildMarkData(&m_stBackGroundItem);
						}
					}
					break;
				case DLGPOS_CHECK_BACKGROUND_SHIELD:
					{
						if( m_stBackGroundItem.iCurFiltter != eSUB_TYPE_BACKGROUND_SHIELD)
						{
							m_stBackGroundItem.iCurFiltter = eSUB_TYPE_BACKGROUND_SHIELD;
							m_stBackGroundItem.iPage =0;
							RegisterGuildMarkData(&m_stBackGroundItem);

						}
					}
					break;
				case DLGPOS_CHECK_BACKGROUND_ETC:
					{
						if( m_stBackGroundItem.iCurFiltter != eSUB_TYPE_BACKGROUND_ETC)
						{
							m_stBackGroundItem.iCurFiltter =eSUB_TYPE_BACKGROUND_ETC;
							m_stBackGroundItem.iPage =0;
							RegisterGuildMarkData(&m_stBackGroundItem);
						}
					}
					break;

				case DLGPOS_CHECK_PATTERN_FIGURE: 
					{
						if( m_stPatternItem.iCurFiltter != eSUB_TYPE_PATTERN_FIGURE)
						{
							m_stPatternItem.iCurFiltter =eSUB_TYPE_PATTERN_FIGURE;
							m_stPatternItem.iPage = 0;
							RegisterGuildMarkData(&m_stPatternItem);

						}
					}
					break;
				case DLGPOS_CHECK_PATTERN_SYMBOL:
					{
						if( m_stPatternItem.iCurFiltter != eSUB_TYPE_PATTERN_SYMBOL)
						{
							m_stPatternItem.iCurFiltter = eSUB_TYPE_PATTERN_SYMBOL;
							m_stPatternItem.iPage = 0;
							RegisterGuildMarkData(&m_stPatternItem);

						}
					}
					break;
				case DLGPOS_CHECK_PATTERN_ETC:
					{
						if( m_stPatternItem.iCurFiltter != eSUB_TYPE_PATTERN_ETC)
						{
							m_stPatternItem.iCurFiltter = eSUB_TYPE_PATTERN_ETC;
							m_stPatternItem.iPage = 0;
							RegisterGuildMarkData(&m_stPatternItem);

						}
					}
					break;
				}	 
			}
		}		 
		break;


	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkCombination::NetworkProc(MSG_BASE * pMsg)
{

}

//------------------------------------------------------------------------------
/**
 stGUILD_MARK_ITEM에 등록된 길드 마크를 인터페이스에 랜더링한다
*/
void uiGuildMarkCombination::RenderGuildMark(stGUILD_MARK_ITEM * pGuildMarkItem)
{
	if( !pGuildMarkItem)
		return;

	HANDLE hGuildMarkTexture = m_pGuildMan->GetGuildMarkTexture();
	if( hGuildMarkTexture == INVALID_HANDLE_VALUE )
		return;

	// stGUILD_MARK_ITEM에 채워진 정보를 기준으로 그린다.

	for( int i = 0; i < MAX_GUILD_MARK; ++i)
	{
		if(  pGuildMarkItem->index[i] != INVALID_GUILD_MARK_INDEX)
		{
			BASE_GuildMarkInfo * pInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(pGuildMarkItem->index[i]);
			if( pInfo)
			{
				RECT rtMark = pGuildMarkItem->pButton[i]->GetSizeRect();

				float fSrcX = (float)pInfo->iRow * 32.0f;
				float fSrcY = (float)pInfo->iColumn * 32.0f;
				g_pSunRenderer->RenderTexture(hGuildMarkTexture , (float)rtMark.left ,(float)rtMark.top , 32.0f, 32.0f , fSrcX, fSrcY, 32.0f,32.0f );
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
  필터링에 따라서 stGUILD_MARK_ITEM에 데이타를 채워 넣는다.
*/
void uiGuildMarkCombination::RegisterGuildMarkData(stGUILD_MARK_ITEM * pstGuildMarkItem)
{

	for( int i = 0; i < MAX_GUILD_MARK; ++i)
	{
		pstGuildMarkItem->index[i] = INVALID_GUILD_MARK_INDEX;
	}

	HASH_TABLE_GUILD_MARK * pHashTable = GuildMarkInfoParser::Instance()->GetHashTable();

	if( !pHashTable)
		return;

	HASH_TABLE_GUILD_MARK_ITR itr = pHashTable->begin();
	int iItemCount = 0;
	int iRegisteCount = 0;
	while( itr != pHashTable->end())
	{
		BASE_GuildMarkInfo * pGuildMarkInfo = (*itr);
		if( pGuildMarkInfo )
		{
			bool bAvalibleRegiste = pGuildMarkInfo->byMainCategory == pstGuildMarkItem->iType && pGuildMarkInfo->bySubCategory == pstGuildMarkItem->iCurFiltter;
			if(bAvalibleRegiste)
			{
				if( iItemCount >= pstGuildMarkItem->iPage * MAX_GUILD_MARK && iItemCount < (pstGuildMarkItem->iPage +1) * MAX_GUILD_MARK ) 
				{
					pstGuildMarkItem->index[iRegisteCount] = pGuildMarkInfo->dwIndex;
					iRegisteCount++;
				}
				iItemCount++;
			}
		}
		itr++;
	}

    TCHAR szMessge[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR szPage[INTERFACE_STRING_LENGTH] = {0, };
    //5796 %d페이지
    g_InterfaceManager.GetInterfaceString(5796, szPage);
    Snprintf(szMessge, INTERFACE_STRING_LENGTH -1, szPage , pstGuildMarkItem->iPage +1);
	GlobalFunc::SetCaption(pstGuildMarkItem->pStaticPage, szMessge);

	pstGuildMarkItem->iMaxPage = (iItemCount / MAX_GUILD_MARK);
	SetButtonCheck(pstGuildMarkItem);
}

//------------------------------------------------------------------------------
/**
  조합된 길드 마크를 결과 슬롯에 뿌려준다.
*/
void uiGuildMarkCombination::RenderTargetGuildMark()
{
	HANDLE hGuildMarkTexture = m_pGuildMan->GetGuildMarkTexture();
	if( hGuildMarkTexture == INVALID_HANDLE_VALUE)
		return;

	ENUM_ALPHABLEND eOldAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);


	RECT rtMark = m_pTargetMark->GetSizeRect();
	BASE_GuildMarkInfo * pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(m_stBackGroundItem.iSelIndex);
	if( pMarkInfo)
	{
		g_pSunRenderer->RenderTexture(hGuildMarkTexture , (float)rtMark.left ,(float)rtMark.top , 32.0f, 32.0f , (float)pMarkInfo->iRow * 32.0f, (float)pMarkInfo->iColumn * 32.0f, 32.0f,32.0f );
	}	

	pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(m_stPatternItem.iSelIndex);
	if( pMarkInfo )
	{
		g_pSunRenderer->RenderTexture(hGuildMarkTexture , (float)rtMark.left ,(float)rtMark.top , 32.0f, 32.0f , (float)pMarkInfo->iRow * 32.0f, (float)pMarkInfo->iColumn * 32.0f, 32.0f,32.0f );
	}
	g_pSunRenderer->SetAlphaBlendMode(eOldAlphablendMode);
}

void uiGuildMarkCombination::NET_SEND_CG_GUILD_GUILDMARK_REGISTER_SYN()
{
	POSTYPE OutPos;
	BOOL foundGuildMarkItem = ItemManager::Instance()->FindFirstItem(eITEMTYPE_GUILDMARK_SEAL, OutPos );
	if( foundGuildMarkItem )
	{
		MSG_CG_GUILD_GUILDMARK_REGISTER_SYN SendMsg;
		SendMsg.m_SlotIndex = SI_INVENTORY;
		SendMsg.m_SlotPos = OutPos;
		SendMsg.m_GuildIndex = m_pGuildMan->GetGuildInfo().m_GuildGuid;
		SendMsg.m_BackGroundIndex = m_stBackGroundItem.iSelIndex; 
		SendMsg.m_PatternIndex = m_stPatternItem.iSelIndex;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
	}
}

void uiGuildMarkCombination::InitstGuildMarkItem()
{
	m_stPatternItem.iPage = 0;
	m_stPatternItem.iSelIndex = 0;
	m_stPatternItem.iCurFiltter = eSUB_TYPE_PATTERN_FIGURE;
	m_stPatternItem.iType = eTYPE_PATTERN;

	m_stBackGroundItem.iPage = 0;
	m_stBackGroundItem.iSelIndex = 0;
	m_stBackGroundItem.iCurFiltter = eSUB_TYPE_BACKGROUND_CIRCUL;
	m_stBackGroundItem.iType = eTYPE_BACKGROUND;

	RegisterGuildMarkData(&m_stBackGroundItem);
	RegisterGuildMarkData(&m_stPatternItem);
}

void uiGuildMarkCombination::SetButtonCheck(stGUILD_MARK_ITEM * pstGuildMarkItem)
{
	int iSelCheck = 0;
	if( pstGuildMarkItem->iType == eTYPE_BACKGROUND)
	{
		iSelCheck = pstGuildMarkItem->iCurFiltter - eSUB_TYPE_BACKGROUND_CIRCUL;
	}

	if( pstGuildMarkItem->iType == eTYPE_PATTERN)
	{
		iSelCheck = pstGuildMarkItem->iCurFiltter - eSUB_TYPE_PATTERN_FIGURE;
	}


	for( int i = 0; i < MAX_FILTER_CHECK; ++i)
	{
		if (i == iSelCheck )
		{
			pstGuildMarkItem->pCheck[iSelCheck]->SetCheckState(true);
		}
		else
		{
			pstGuildMarkItem->pCheck[i]->SetCheckState(false);
		}
	}
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
