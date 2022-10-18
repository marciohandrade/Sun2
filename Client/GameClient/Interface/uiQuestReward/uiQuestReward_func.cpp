//------------------------------------------------------------------------------
//  uiQuestReward_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan/uiQuestMan.h"
#include "uiQuestReward.h"
#include "ItemComposeListParser.h"
#include "ItemUnitRenderer.h"
#include "SlotKeyGenerator.h"
#include "SCItemSlot.h"
#include "ItemManager.h"
#include "QuestManager_Concrete.h"
#include "QuestTextInfoParser.h"
#include "RewardInfoList.h"
#include "ItemTypeList.h"
#include "ItemInfoParser.h"
#include "QuestInfoManagerEx.h"
#include "Hero.h"
#include "ObjectManager.h"
#include "NPC.h"

const int cMaxMaxQuestReward     = 10;

//------------------------------------------------------------------------------
/**
*/
uiQuestMan*
uiQuestReward::GetManager()
{
    uiQuestMan* puiQuestMan =
        static_cast<uiQuestMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_QUEST));
    assert (puiQuestMan);
    return puiQuestMan;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestReward::ClearInfo()
{
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if ( !IsEmpty(i) )
			DeleteSlot(i, NULL);
	}

	m_SelectRewardItemList.clear();

	if ( !this->CheckControls() )
		return;
}


//-------------------------------------------------------------------------------------------
/**
*/
ItemUnitRenderer *	
uiQuestReward::GetItemUnitRender() const
{
	return this->m_pItemUnitRenderer;
}

//-------------------------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT		uiQuestReward::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
	/// 슬롯에 넣음
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rSetSlot = GetSlot(AtPos);
	rSetSlot.SetSerial(dwSerial);


	/// 인벤토리
	CControlWZ * pCtrl = getControl( AtPos );
	RECT rect = pCtrl->GetSizeRect();

	if(m_pItemUnitRenderer)
	{
		ItemUnitRenderer::RenderUnit * pUnitRender = m_pItemUnitRenderer->AddItemUnit( dwSerial, rSlot.GetCode(), &rect, &GetSlot(AtPos), pCtrl );	

		if(pUnitRender && m_eQuestGainState!=QUEST_REWARD)
		{
			pUnitRender->SetState(SLOT_RENDER_STATE_REWARDLIST);
		}

	}
	else
	{
		assert(!"m_pItemUnitRenderer == NULL");
	}

	return eInsertSlotResult;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL				
uiQuestReward::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	SCSlot & rSlot = GetSlot(AtPos);
	g_SlotKeyGenerator.Restore((DWORD)rSlot.GetSerial());

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->RemoveItemUnit( rSlot.GetSerial() );
	}
	else
	{
		assert(!"m_pItemUnitRenderer == NULL");
	}

	return SCSlotContainer::DeleteSlot(AtPos, pSlotOut);
}

//-------------------------------------------------------------------------------------------
/**
*/
void                    
uiQuestReward::SetCurQuestCode(QuestInfo * pQuestInfo, eQUEST_GAIN_STATE QuestState )
{ 
    m_pCurQuestInfo = pQuestInfo; 
    m_eQuestGainState = QuestState;
}


//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestReward::initInfo()
{
	ClearInfo();

	// 현재 퀘스트의 상태가 수락 / 진행인지 판다.
	if ( !g_pHero || !m_pCurQuestInfo )
	{
		return;
	}

	initBtnInfo();
	initQuestInfo();
	
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestReward::initBtnInfo()
{
	DWORD dwStrIDOK			= 100;
	DWORD dwStrIDCancel		= 101;

	ShowRewardOK(TRUE);
	ShowRewardCancel(TRUE);

	switch ( m_eQuestGainState )
	{
	case QUEST_ACCEPT:
		{
			dwStrIDOK		= 9010;
			dwStrIDCancel	= 9011;
			GetCtrlRewardOK()->SetTextColor(ACTIVE_TEXT_COLOR);
			GetCtrlRewardCancel()->SetTextColor(ACTIVE_TEXT_COLOR);
		}
		break;

	case QUEST_PROGRESS:
		{
			dwStrIDOK		= 100;
			dwStrIDCancel	= 101;
			GetCtrlRewardOK()->SetTextColor(ACTIVE_TEXT_COLOR);
			GetCtrlRewardCancel()->SetTextColor(ACTIVE_TEXT_COLOR);
		}
		break;

	case QUEST_REWARD:
		{
			// 완료 보상
			dwStrIDOK		= 100;
			dwStrIDCancel	= 101;

			GetCtrlRewardOK()->SetTextColor(ACTIVE_TEXT_COLOR);
			GetCtrlRewardCancel()->SetTextColor(ACTIVE_TEXT_COLOR);
		}
		break;

	case QUEST_INPERFECT_REWARD:
		{
			// 미완료 
			dwStrIDOK		= 100;
			dwStrIDCancel	= 101;
			GetCtrlRewardOK()->SetTextColor(DEACTIVE_TEXT_COLOR);
			GetCtrlRewardCancel()->SetTextColor(ACTIVE_TEXT_COLOR);
		}
		break;


	case QUEST_AUTO_ACCEPT:
		{
			// 자동 수락
			dwStrIDCancel	= 100;	// 확인
			ShowRewardOK(FALSE);
			GetCtrlRewardCancel()->SetTextColor(ACTIVE_TEXT_COLOR);
		}
		break;


	default:
		assert(!"여기 기어 들어오면 어케!!!");
		return;
		break;
	}

	SetCaptionRewardOK(dwStrIDOK);
	SetCaptionRewardCancel(dwStrIDCancel);
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestReward::initQuestInfo()
{
	if ( !m_pCurQuestInfo )
	{
		assert(!"퀘스트정보가 없네~!");
		return;
	}

	// 타이틀 설정.
	SetCaptionTitle( m_pCurQuestInfo->GetQNCode() );

	initQuestInfoConversation();
	initQuestInfoReward();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestReward::initQuestInfoConversation()
{
	// 퀘스트 설명 내용.
	DWORD dwConverationID = 0;

	switch ( this->m_eQuestGainState )
	{
	case QUEST_AUTO_ACCEPT:
	case QUEST_ACCEPT:		
		{
			dwConverationID = m_pCurQuestInfo->GetSCode_Start();
		}
		break;
	case QUEST_PROGRESS:
		{
			dwConverationID = m_pCurQuestInfo->GetSCode_Progr1();
		}
		break;
	case QUEST_REWARD:			
		{
			dwConverationID = m_pCurQuestInfo->GetSCode_Compl();
		}
		break;
	case QUEST_INPERFECT_REWARD:
		{
			dwConverationID = m_pCurQuestInfo->GetSCode_Progr1();
		}
		break;
	}

	QuestTextInfo * pInfo =
		QuestTextInfoParser::Instance()->GetQuestTextInfo(dwConverationID);

	m_FTextRender.ClearSentence();

	m_FTextRender.SetBeginLineIndex(0);
	GetCtrlQuestInfoVscr()->SetScrollPosWZ(0);

	if (pInfo) 
	{
		SUN_STRING strTemp = pInfo->m_pszText;
		if ( m_FTextParser.Parser(strTemp, m_FTextSentence) )
		{
			m_FTextRender.Append(m_FTextSentence);
		}
	}	

	GetCtrlQuestInfoVscr()->SetScrollRangeWZ( 0, m_FTextRender.GetMaxLine() );
}

//-------------------------------------------------------------------------------------------
/** 보상받을아이템중 자신의클래스에 해당하는아이템선택하게끔하는함수
*/
int uiQuestReward::RewardItemActiveForClass(RewardInfo * pRewardInfo)
{
	// 선택 아이템
	for ( int a = 0; a < RewardInfo::MAX_DROP_INDEX_NUM; ++a)
	{
		//없으면 제끼고
		if ( 0 == pRewardInfo->GetTypeCode(a) ) continue;
		
		BASE_ITEMINFO *pInfo=  ItemInfoParser::Instance()->GetItemInfo( pRewardInfo->GetTypeCode(a) );

		if (!pInfo) continue;
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
        bool is_wing_item = ((pInfo->item_rule_info.is_accessory_wings == true) &&
            (pInfo->item_rule_info.is_accessory_special == true));
        //무기나 장비아이템에 한에서만 체크한다
        if(pInfo && (pInfo->IsArmor() || pInfo->IsWeapon() || is_wing_item == true))
#else
		//무기나 장비아이템에 한에서만 체크한다
		if(pInfo && (pInfo->IsArmor() || pInfo->IsWeapon()))
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
		{
			eCHAR_TYPE eCharEqClass[eCHAR_TYPE_MAX] = {	
				(pInfo->m_wEqClass1)?(eCHAR_BERSERKER):(eCHAR_TYPE_MAX),		//(B)버서커
				(pInfo->m_wEqClass2)?(eCHAR_DRAGON):(eCHAR_TYPE_MAX),			//(D)드래곤
				(pInfo->m_wEqClass3)?(eCHAR_SHADOW):(eCHAR_TYPE_MAX),			//(S)쉐도우
				(pInfo->m_wEqClass4)?(eCHAR_VALKYRIE):(eCHAR_TYPE_MAX),			//(V)발키리
				(pInfo->m_wEqClass5)?(eCHAR_ELEMENTALIST):(eCHAR_TYPE_MAX),     //(E)엘리멘탈리스트
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
                (pInfo->m_wEqClass6)?(eCHAR_MYSTIC):(eCHAR_TYPE_MAX),           //(M)미스틱//_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                (pInfo->m_wEqClass7)?(eCHAR_HELLROID):(eCHAR_TYPE_MAX),         //(H)
#else //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                (pInfo->m_wEqClass6)?(eCHAR_MYSTIC):(eCHAR_TYPE_MAX),           //(M)미스틱//_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                (pInfo->m_wEqClass8)?(eCHAR_WITCHBLADE):(eCHAR_TYPE_MAX),       //(W)위치블레이드//_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };
			//자신의클래스와 일치하는지 체크한다
			for( int i = 0 ; i < eCHAR_TYPE_MAX ; ++i )
			{
				if(eCharEqClass[i]==g_pHero->GetClass())
				{
					//현재 슬롯위치를 리턴
					return a;
				}
			}	
		}
	}

	return -1;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestReward::initQuestInfoReward()
{
	// 퀘스트 보상 관련
	assert(g_pHero);
	if( !g_pHero )
    {
		return;
    }
	DWORD dwRewardCode = m_pCurQuestInfo->GetRewardCode(g_pHero->GetClass());
	RewardInfo * pRewardInfo = RewardInfoList::Instance()->FindRewardInfo( dwRewardCode );
	assert(pRewardInfo);
	if (!pRewardInfo) 
	{
		GetUIMan()->GetInterfaceString(999, m_szText, INTERFACE_STRING_LENGTH);
		SetCaptionRewardChao( m_szText );
		SetCaptionRewardHeim(_T("0"));
		SetCaptionRewardExp(_T("0"));
        if (reward_point_button_)
        {
            reward_point_button_->SetTextWZ(_T("0"));
        }
#ifdef _DEV_VER
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, "Not Found RewardInfoList.txt Script Data : index(%d)", dwRewardCode);
#endif			
		return;
	}	

	// 하임 or 카오 타임
	if ( pRewardInfo->GetChaoTime() )
	{
		// 카오시간 : 9043
		GetUIMan()->GetInterfaceString(9043, m_szText, INTERFACE_STRING_LENGTH);
		SetCaptionRewardChao( m_szText );
		Sprintf(m_szText, _T("%ld"), pRewardInfo->GetChaoTime());
	}
	else
	{
		// 하임 : 999
		GetUIMan()->GetInterfaceString(999, m_szText, INTERFACE_STRING_LENGTH);
		TCHAR szMoney[INTERFACE_STRING_LENGTH + 1] = {0,};
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		SetCaptionRewardChao( m_szText );
		Sprintf(szMoney, _T("%I64u"), pRewardInfo->GetHime());
		::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoney, &nFmt, m_szText, sizeof(m_szText));
	}

	
	SetCaptionRewardHeim(m_szText);

	// 경험치
	{
		TCHAR exp_string[INTERFACE_STRING_LENGTH + 1] = {0, };
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		Sprintf(exp_string, _T("%I64d"), pRewardInfo->GetExp() );
		::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, exp_string, &nFmt, m_szText, sizeof(m_szText));
	}
	SetCaptionRewardExp(m_szText);

    Snprintf(m_szText, INTERFACE_STRING_LENGTH, _T("%d"), pRewardInfo->GetCoin());
    if (reward_point_button_)
    {
        reward_point_button_->SetTextWZ(m_szText);
    }

	ItemType * pItemType = NULL;    
    SCItemSlot TempSlot;

	// 픽스 아이템
	BASE_ITEMINFO * pItemInfo = 
		ItemInfoParser::Instance()->GetItemInfo( pRewardInfo->GetItemCode() );

	if (pItemInfo)
	{
		TempSlot.SetCode( pRewardInfo->GetItemCode() );

		if ( pItemInfo->m_byDupNum > 1 )
		{			
			TempSlot.SetNum( (DURATYPE)pRewardInfo->GetItemTypeIndex() );          
		}
		else
		{
			pItemType = 
				ItemTypeList::Instance()->FindItemType( pRewardInfo->GetItemTypeIndex() );

			if (pItemType)
			{
				
				TempSlot.SetEnchant( pItemType->GetEnchant() );
				TempSlot.SetRank((eRANK)pItemType->GetRank() );

                nsSlot::ItemTypeChanger::ChangeItemByType(&TempSlot, pItemType->GetItemType());
			}			
		}	

		InsertSlot( SLOT_FIX_ITEM, TempSlot);
	}	

	switch ( pRewardInfo->GetRefType() )
	{
	case RewardInfo::REF_TYPE_SELECT:
		{
			// 선택 아이템
			for ( int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)
			{
				if ( 0 == pRewardInfo->GetTypeCode(i) )
				{
					continue;
				}

				pItemInfo = 
					ItemInfoParser::Instance()->GetItemInfo( pRewardInfo->GetTypeCode(i) );

				if (pItemInfo)
				{
					TempSlot.SetCode( (SLOTCODE)pRewardInfo->GetTypeCode(i) );

					if (pItemInfo->m_byDupNum > 1) 
					{					    
						TempSlot.SetNum( (DURATYPE)pRewardInfo->GetTypeIndex(i) );
					}
					else
					{
						pItemType = 
							ItemTypeList::Instance()->FindItemType( pRewardInfo->GetTypeIndex(i) );
						if ( pItemType )
						{
							TempSlot.SetEnchant( pItemType->GetEnchant() );
							TempSlot.SetRank( (eRANK)pItemType->GetRank() );		
                            nsSlot::ItemTypeChanger::ChangeItemByType(&TempSlot, pItemType->GetItemType());
						}
					}

					InsertSlot( SLOT_SELECT_BEGIN+i, TempSlot );
				}
			}

			// 기본 선택 아이템을 추가한다.
			m_SelectRewardItemList.clear();

			if ( QUEST_REWARD == m_eQuestGainState )
			{
				
				for ( int i = 0; i < pRewardInfo->GetSelectNum(); ++i)
				{
					sSelectRewardSlotIdx sSelect;
					sSelect.pos = i;
					m_SelectRewardItemList.push_back(sSelect);
				}

				int nPos = RewardItemActiveForClass(pRewardInfo);
				
				//자신클래스 선택장비가있을경우(무기,아머)
				if(nPos!=-1 && pRewardInfo->GetSelectNum()>0)
				{
					m_SelectRewardItemList.clear();
					sSelectRewardSlotIdx sSelect;
					sSelect.pos = nPos;
					m_SelectRewardItemList.push_back(sSelect);

				}
			}
		}
		break;
	case RewardInfo::REF_TYPE_RANDOM:
		{
			// 랜덤 아이템
			for ( int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)
			{
				if ( pRewardInfo->GetTypeCode(i) )
				{
					TempSlot.SetCode( RANDOM_ITEM_CODE );	// 물음표 아이템
					InsertSlot( SLOT_SELECT_BEGIN+i, TempSlot );
				}
			}	

			// 기본 선택 아이템을 추가한다.
			m_SelectRewardItemList.clear();

			if ( QUEST_REWARD == m_eQuestGainState )
			{
				for ( int i = 0; i < pRewardInfo->GetSelectNum(); ++i)
				{
					sSelectRewardSlotIdx sSelect;
					sSelect.pos = i;
					m_SelectRewardItemList.push_back(sSelect);
				}
			}
		}
		break;
	case RewardInfo::REF_TYPE_EVENT_ITEMCODE:
	case RewardInfo::REF_TYPE_DATETIME:
		{
			// 선택 아이템
			for ( int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)
			{
				if ( 0 == pRewardInfo->GetTypeCode(i) )
				{
					continue;
				}

				pItemInfo = 
					ItemInfoParser::Instance()->GetItemInfo( pRewardInfo->GetTypeCode(i) );

				if (pItemInfo)
				{
					TempSlot.SetCode( (SLOTCODE)pRewardInfo->GetTypeCode(i) );

					if (pItemInfo->m_byDupNum > 1) 
					{					    
						TempSlot.SetNum( (DURATYPE)pRewardInfo->GetTypeIndex(i) );
					}
					else
					{
						pItemType = 
							ItemTypeList::Instance()->FindItemType( pRewardInfo->GetTypeIndex(i) );
						if ( pItemType )
						{
							TempSlot.SetEnchant( pItemType->GetEnchant() );
							TempSlot.SetRank( (eRANK)pItemType->GetRank() );	
                            nsSlot::ItemTypeChanger::ChangeItemByType(&TempSlot, pItemType->GetItemType());

						}
					}

					InsertSlot( SLOT_SELECT_BEGIN+i, TempSlot );
				}
			}

			m_SelectRewardItemList.clear();
		}
		break;
	case RewardInfo::REF_TYPE_EVENT_DROPINFO:
		{
			// 랜덤 아이템
			for ( int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)
			{
				if ( pRewardInfo->GetTypeCode(i) )
				{
					TempSlot.SetCode( RANDOM_ITEM_CODE );	// 물음표 아이템
					InsertSlot( SLOT_SELECT_BEGIN+i, TempSlot );
				}
			}	

			m_SelectRewardItemList.clear();
		}
		break;
	default:
		assert(0);
		break;
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestReward::updateInfo()
{
	// ..
	m_FTextRender.SetRect(GetCtrlQuestInfo()->GetSizeRect());
	
	int iMaxLine = 0;

	if(m_FTextRender.GetMaxLine()>cMaxMaxQuestReward)
	{
		iMaxLine = (m_FTextRender.GetMaxLine())-cMaxMaxQuestReward;
	}

	GetCtrlQuestInfoVscr()->SetScrollRangeWZ( 0, iMaxLine );

	RECT rc;
	GetDialogWZ()->GetClientRect(&rc);
	switch (this->GetMouseWheelStatus(&rc))
	{
	default:
		break;
	case MOUSE_WHEEL_UP:
		{
			//.윗쪽 화살표를 눌렀다.
			int pos = GetCtrlQuestInfoVscr()->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
			int mmin = 0 , mmax = 0;
			GetCtrlQuestInfoVscr()->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

			if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
			{
				pos--;      // 한줄 감소 시키고 
				GetCtrlQuestInfoVscr()->SetScrollPosWZ(pos);
				m_FTextRender.SetBeginLineIndex(pos);

			}
		}
		break;

	case MOUSE_WHEEL_DOWN:
		{
			//.아랫쪽 화살표를 눌렀다
			int pos = GetCtrlQuestInfoVscr()->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
			int mmin = 0 , mmax = 0;
			GetCtrlQuestInfoVscr()->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

			if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
			{
				pos++;      // 한줄 증가 시키고 
				GetCtrlQuestInfoVscr()->SetScrollPosWZ(pos);
				m_FTextRender.SetBeginLineIndex(pos);
			}
		}
		break;
	}
}

void					
uiQuestReward::clickBtn(POSTYPE pos)
{
	if ( !m_pCurQuestInfo )
	{
		return;
	}

	switch (pos)
	{
	case eDLGPOS_BTN_OK:
		{
			switch ( m_eQuestGainState )
			{
			case QUEST_ACCEPT:
				{
					GetManager()->NET_SEND_CG_QUEST_ACCEPT_SYN(
						m_pCurQuestInfo->GetQuestCode() );
                    DWORD npc_object_key = GetManager()->GetPendingNPCKey();
                    NPC* npc_object = static_cast<NPC*>(g_ObjectManager.GetObject(npc_object_key));
                    if (npc_object != NULL)
                    {
                        WzID animation_id = npc_object->GetCurrentQuestAcceptAnimation();
                        npc_object->SetAnimation(animation_id, FALSE);
                    }
				}
				break;
			case QUEST_PROGRESS:
				{
					ShowInterfaceWithSound(FALSE);
				}
				break;

			case QUEST_REWARD:
				{	
					assert(g_pHero);
					if(!g_pHero)
                    {
						return;
                    }

					DWORD dwRewardCode = m_pCurQuestInfo->GetRewardCode(g_pHero->GetClass());
					RewardInfo * pRewardInfo = RewardInfoList::Instance()->FindRewardInfo( dwRewardCode );
					assert(pRewardInfo);
					if (!pRewardInfo) 
					{
						GlobalFunc::DebugOutputMsg(
							eOUTPUT_MSG_TOP_SHOW, 
							_T("[기획자확인]보상 정보가 없습니다. (QuestCode : %d RewardCode : %d)"), 
							m_pCurQuestInfo->GetQuestCode(), dwRewardCode);
						return;
					}

					m_bSelectItem = 0;

					switch (pRewardInfo->GetRefType())
					{
					case RewardInfo::REF_TYPE_SELECT:
					case RewardInfo::REF_TYPE_RANDOM:
						{
							// 이놈 정보를 가지고 선택 아이템을 보내야할 경우라면 정보를 채워서 보내주자.
							if ( m_SelectRewardItemList.size() < pRewardInfo->GetSelectNum() )
							{
								// 선택 아이템을 모두 선택하지 않았다.!!
								return;
							}

							std::list<sSelectRewardSlotIdx>::iterator itr =	
								m_SelectRewardItemList.begin();

							for (; itr != m_SelectRewardItemList.end(); ++itr)
							{
								sSelectRewardSlotIdx sSelect = (*itr);
								switch ( sSelect.pos )
								{
								case 0:
									{
										BIT_ADD(m_bSelectItem, MSG_CG_QUEST_REWARD_SYN::ITEM1_SELECTED);
									}
									break;

								case 1:
									{
										BIT_ADD(m_bSelectItem, MSG_CG_QUEST_REWARD_SYN::ITEM2_SELECTED);
									}
									break;

								case 2:
									{
										BIT_ADD(m_bSelectItem, MSG_CG_QUEST_REWARD_SYN::ITEM3_SELECTED);
									}
									break;

								case 3:
									{
										BIT_ADD(m_bSelectItem, MSG_CG_QUEST_REWARD_SYN::ITEM4_SELECTED);
									}
									break;
								
								case 4:
									{
										BIT_ADD(m_bSelectItem, MSG_CG_QUEST_REWARD_SYN::ITEM5_SELECTED);
									}
									break;
								}							

							}		
						}
						break;

						// 선택 필요없는 아이템
					case RewardInfo::REF_TYPE_EVENT_DROPINFO:
					case RewardInfo::REF_TYPE_EVENT_ITEMCODE:
					case RewardInfo::REF_TYPE_DATETIME:
						{
							for (int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i )
							{
								if ( 0 == pRewardInfo->GetTypeCode(i) )
								{
									break;
								}

								switch ( i )
								{
								case 0:
									{
										BIT_ADD(m_bSelectItem, MSG_CG_QUEST_REWARD_SYN::ITEM1_SELECTED);
									}
									break;

								case 1:
									{
										BIT_ADD(m_bSelectItem, MSG_CG_QUEST_REWARD_SYN::ITEM2_SELECTED);
									}
									break;

								case 2:
									{
										BIT_ADD(m_bSelectItem, MSG_CG_QUEST_REWARD_SYN::ITEM3_SELECTED);
									}
									break;

								case 3:
									{
										BIT_ADD(m_bSelectItem, MSG_CG_QUEST_REWARD_SYN::ITEM4_SELECTED);
									}
									break;
								case 4:
									{
										BIT_ADD(m_bSelectItem, MSG_CG_QUEST_REWARD_SYN::ITEM5_SELECTED);
									}
									break;
								}															
							}
						}
						break;

					default:
						assert(0);
						break;
					}

					GetManager()->NET_SEND_CG_QUEST_REWARD_SYN(
						m_pCurQuestInfo->GetQuestCode(),
						m_bSelectItem
						);
                    DWORD npc_object_key = GetManager()->GetPendingNPCKey();
                    NPC* npc_object = static_cast<NPC*>(g_ObjectManager.GetObject(npc_object_key));
                    if (npc_object != NULL)
                    {
                        WzID animation_id = npc_object->GetCurrentQuestCompleteAnimation();
                        npc_object->SetAnimation(animation_id, FALSE);
                    }
				}
				break;
			case QUEST_INPERFECT_REWARD:
				{
					// 완료 조건을 만족 못한 퀘스트 입니다.
					GetUIMan()->GetInterfaceString(5901, m_szResult, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, m_szResult);
				}
				break;

			default:
				break;
			}
		}
		break;

	case eDLGPOS_BTN_CANCEL:
		{
			switch ( m_eQuestGainState )
			{
			case QUEST_AUTO_ACCEPT:
				{
					// 퀘스트 리스트 창을 열어준다.
					if ( !GetManager()->IsOpenQuestList() )
					{
						GetManager()->OpenForceQuestList();
					}

					ShowInterface(FALSE);
				}
				break;

			case QUEST_ACCEPT:
			case QUEST_PROGRESS:
			case QUEST_REWARD:
			case QUEST_INPERFECT_REWARD:
			default:
				{
					ShowInterfaceWithSound(FALSE);
				}
				break;
			}
		}
		break;

	default:
		break;
	}
}
//#endif

//-------------------------------------------------------------------------------------------
/**
*/
BOOL					
uiQuestReward::clickSelectItemSlot(POSTYPE pos)
{
	assert(g_pHero);
	if(!g_pHero)
    {
		return FALSE;
    }

	DWORD dwRewardCode = m_pCurQuestInfo->GetRewardCode(g_pHero->GetClass());
	RewardInfo * pRewardInfo = RewardInfoList::Instance()->FindRewardInfo( dwRewardCode );
	assert(pRewardInfo);
	if (!pRewardInfo) 
	{
		return FALSE;
	}

	if ( RewardInfo::REF_TYPE_EVENT_ITEMCODE == pRewardInfo->GetRefType() || 
		 RewardInfo::REF_TYPE_EVENT_DROPINFO == pRewardInfo->GetRefType() ||
		 RewardInfo::REF_TYPE_DATETIME       == pRewardInfo->GetRefType() || 
		 0 == pRewardInfo->GetSelectNum())
	{
		// 선택 불필요 아이템 타입이거나
		// 선택 개수가 0 이면 선택할 수 없다.		
		return FALSE;
	}

	switch ( m_eQuestGainState )
	{
	case QUEST_REWARD:
		{
			if ( pos >= SLOT_SELECT_BEGIN && pos < SLOT_SELECT_BEGIN+SLOT_SELECT_SIZE )
			{
				// 이전에 채워져 있는 위치면 무시 하자.
				std::list<sSelectRewardSlotIdx>::iterator itr = m_SelectRewardItemList.begin();
				for (itr; itr != m_SelectRewardItemList.end(); ++itr)
				{
					sSelectRewardSlotIdx SelectIndex = (*itr);
					if ( SelectIndex.pos == pos )
					{
						return FALSE;
					}
				}				
			
				// 선택 아이템이다.
				if ( !IsEmpty(pos) )
				{
					if ( pRewardInfo->GetSelectNum() > m_SelectRewardItemList.size() )
					{
						// 선택을 해야하는 상태이다.
						sSelectRewardSlotIdx SelectIndex;
						SelectIndex.pos = pos;
						m_SelectRewardItemList.push_back(SelectIndex);
					}
					else
					{
						// 이전 선택된것을 하나 지우고 새로운 위치를 지정한다.
						sSelectRewardSlotIdx SelectIndex;
						SelectIndex.pos = pos;
						m_SelectRewardItemList.erase( m_SelectRewardItemList.begin() );
						m_SelectRewardItemList.push_back(SelectIndex);

					}
				}
			}			
		}
		break;
	case QUEST_INPERFECT_REWARD:
	case QUEST_ACCEPT:
	case QUEST_PROGRESS:
	default:
		{
		}
		break;
	}

	return TRUE;
}




//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
