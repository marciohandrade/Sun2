#include "SunClientPrecompiledHeader.h"


#include "uiFateItemSystem.h"
#include "GameConst_Color.h"
#include "ItemManager.h"
#include "ItemFormula.h"
#include "SCSlotContainer.h"
#include "ItemInfoParser.h"
#include "SCItemSlot.h"
#ifdef _INTERNATIONAL_UI
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#endif//_INTERNATIONAL_UI

#define MODFLOAT(ratio) ((ratio)<0 ? (-0.5f):(0.5f))

//------------------------------------------------------------------------------
/**
*/

const WzColor wzRandomColor =  RGBA(255,45,45,255);
const WzColor wzNormalColor = RGBA(255,255,255,255);

void uiFateItemSystem::RenderMovie()
{
	if( !m_hBink)
		return;

	if(!m_hRenderTargetTexture)
		return;

	RECT rtPicture = m_pPictureMovie->GetSizeRect();
	g_pSunRenderer->RenderTexture(	m_hRenderTargetTexture,
		(float)rtPicture.left, (float)rtPicture.top,
		(float)rtPicture.right - rtPicture.left, 
		(float)rtPicture.bottom - rtPicture.top,
		0.0f,0.0f);
}

//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::UpdateMovie()
{
	if(!m_hBink)
		return;

	// Decompress the Bink frame.
	BinkDoFrame( m_hBink );

	// Lock the BinkBuffer so that we can copy the decompressed frame into it.
	long lPitch;
	int iComponent;
	VOID* pByte = g_pSunRenderer->x_pManagerTexture->LockTexture( m_hRenderTargetTexture, &lPitch, &iComponent );

	if( pByte )
	{
		BinkCopyToBuffer( m_hBink, pByte, lPitch, m_hBink->Height, 0,0, BINKSURFACE32 );
		g_pSunRenderer->x_pManagerTexture->UnlockTexture( m_hRenderTargetTexture );
	}
	else
	{
		m_iState = eFIS_END;
		return;
	}

	// Are we at the end of the movie?
	if ( m_hBink->FrameNum == m_hBink->Frames )
	{
		if( m_iState == eFIS_MOVIE_RUNNING)
		{
			NET_SEND_CG_ITEM_VALIDATION_OF_REIHALT_SYN();
			m_iState = eFIS_WAIT_RESPONSE;
			InitBink(FALSE);
			return;
		}
		else if( m_iState == eFIS_IDLE || m_iState == eFIS_END)
		{
			//기본 동영상 무한반복
			m_hBink->FrameNum = 0;
		}
	}
	else
	{
		BinkNextFrame( m_hBink );
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::UpdateShowNHideButton()
{
	switch( m_iState )
	{
	case eFIS_IDLE:
		{
			m_pButton[DLGPOS_BTN_EXIT]->EnableWindowWZ(TRUE);
			m_pButton[DLGPOS_BTN_OK]->EnableWindowWZ(TRUE);
			m_pButton[DLGPOS_BTN_RESULT_VIEW]->EnableWindowWZ(TRUE);
			m_pButton[DLGPOS_BTN_START]->EnableWindowWZ(TRUE);
		}
		break;

	case eFIS_MOVIE_RUNNING:
	case eFIS_WAIT_RESPONSE:
		{
			m_pButton[DLGPOS_BTN_EXIT]->EnableWindowWZ(FALSE);
			m_pButton[DLGPOS_BTN_OK]->EnableWindowWZ(FALSE);
			m_pButton[DLGPOS_BTN_RESULT_VIEW]->EnableWindowWZ(FALSE);
			m_pButton[DLGPOS_BTN_START]->EnableWindowWZ(FALSE);
		}
		break;

	case eFIS_END:
		{
			m_pButton[DLGPOS_BTN_EXIT]->EnableWindowWZ(TRUE);
			m_pButton[DLGPOS_BTN_OK]->EnableWindowWZ(TRUE);
			m_pButton[DLGPOS_BTN_RESULT_VIEW]->EnableWindowWZ(FALSE);
			m_pButton[DLGPOS_BTN_START]->EnableWindowWZ(FALSE);
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::ProcessCaptionEffect(DWORD dwTick)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	if( m_iState == eFIS_RESULT_SUCCESS )
	{
		m_dwAccmulateTick += dwTick;

		if( m_dwAccmulateTick > 100)
		{
			m_bPlayNotLoopEffect = TRUE;
		}
		if( m_dwAccmulateTick > BUTTON_EFFECT_DELAY)
		{
			//TODO : 생성된 아이템을 가지고 속성값을 얻어와서 SetCaption 해줘야 한다.

			BASE_ITEMINFO * pItemInfo =  ItemInfoParser::Instance()->GetItemInfo(m_ResultItem.GetCode());
			if( pItemInfo)
			{
				int nRatio = m_ResultItem.GetFateAtkDefRatio();

				if( m_iCaptionEffectCount == TXT_PHYSICAL)
				{
					Snprintf( szMessage, INTERFACE_STRING_LENGTH -1 , _T("%d") , 
						CalcPhyDef((DWORD)(pItemInfo->m_wPhyDef * nRatio /100.0f+MODFLOAT(nRatio)), m_ResultItem.GetEnchant(), pItemInfo->m_LV));
						
						
				}
				else if ( m_iCaptionEffectCount == TXT_DURATION)
				{
					Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"),  m_ResultItem.GetFateDuraMax() );
				}
				else if ( m_iCaptionEffectCount == TXT_DATE )
				{
					int iPeriod = m_ResultItem.GetFatePeriod();
					if( iPeriod == INFINITE_ITEM_DAY )
					{
						//1287 무제한
						g_InterfaceManager.GetInterfaceString(1287 , szMessage);
					}
					else
					{
						//1286	%d 일
						Snprintf( szMessage, INTERFACE_STRING_LENGTH-1, g_InterfaceManager.GetInterfaceString(1286), iPeriod);
					}

					
				}
				g_SoundEffect.PlayUI(UI_SOUND_ITEM_VALIDATION_NUMBER_SET);
				m_pStatic[m_iCaptionEffectCount]->SetTextColor(wzNormalColor);
				GlobalFunc::SetCaption(m_pStatic[m_iCaptionEffectCount], szMessage); 
			}
			m_dwAccmulateTick = 0;
			m_bPlayNotLoopEffect = FALSE;
			m_iCaptionEffectCount++;
			if( m_iCaptionEffectCount >= MAX_TXT)
			{
				//최종 결과 아이템을 인벤토리에 넣어준다.
				InsertResultItem();
			}
		}
	}
}

#ifdef _INTERNATIONAL_UI
void uiFateItemSystem::RenderTooltip()
{
	CCtrlStaticWZ* pStaticControl =  GetCtrlTargetName();

	if(!pStaticControl) return;

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	RECT rc = pStaticControl->GetSizeRect();

	if (rc.left <= iMouseX && rc.right >= iMouseX)
	{
		if (rc.top <= iMouseY && rc.bottom >= iMouseY)
		{
			SCSlotContainer * pInventory = ItemManager::Instance()->GetContainer(SI_INVENTORY);
			
			if( !pInventory ) return;

			SCItemSlot & rTargetItem = (SCItemSlot &)pInventory->GetSlot(m_iTargetItemPos);

			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if(pUIMan)
			{
				TCHAR szName[INTERFACE_STRING_LENGTH + 1] = {0,};
				g_InterfaceManager.GetItemName(rTargetItem.GetItemInfo()->m_Code , szName);
				pUIMan->RegisterTooltipSimpleText(szName);
				pUIMan->SetTooltipDialogID(GetDialogKey());

			}
		}
	}
}
#endif//_INTERNATIONAL_UI

void uiFateItemSystem::SetTargetItem(const BASE_ITEMINFO * pInfo, POSTYPE TargetItemPos)
{
	// 상단에서 NULL체크 하고 넘어온 데이타 이다.
    if (m_pIconImage)
    {
        ICONMGR()->RemoveIcon(m_pIconImage);
        m_pIconImage = NULL;
    }

	m_pIconImage = ICONMGR()->Add(pInfo->m_wVICode);
	if(!m_pIconImage)
	{
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("아이콘 리소스 코드 오류[%d]"), pInfo->m_wVICode);
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("아이콘 리소스 코드 오류[%d]"), pInfo->m_wVICode);
		assert(m_pIconImage);
	}

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	g_InterfaceManager.GetItemName(pInfo->m_Code , szMessage);

	CCtrlStaticWZ * pStaticControl = GetCtrlTargetName();
	if( pStaticControl )
	{
		pStaticControl->SetTextColor(  GlobalFunc::WzColorToRGBA(c_LuckyItemColor) );
	}

#ifdef _INTERNATIONAL_UI
	GlobalFunc::SetCaptionSimple(pStaticControl,szMessage);
#else
	SetCaptionTargetName(szMessage);
#endif//_INTERNATIONAL_UI
	

	if( g_InterfaceManager.GetItemClassName(pInfo->m_Code, szMessage) )
	{
		SetCaptionClassName(szMessage);
	}

	Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, _T("%d"), pInfo->m_LimitEqLevel);
	SetCaptionLevelLimit(szMessage);

	InitBaseAtributeCaption();
	m_iTargetItemPos = TargetItemPos;
}

void uiFateItemSystem::InitBaseAtributeCaption()
{	
	for( int i = 0; i< TXT_DATE; ++i)
	{
		GlobalFunc::SetCaption(m_pStatic[i] , _T("0"));
	}

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	//1286	%d 일
	Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(1286), 0);
	GlobalFunc::SetCaption(m_pStatic[TXT_DATE] , szMessage);
}

void uiFateItemSystem::RenderEffectTexture(int id,  int PosX, int PosY, int width, int height, int numX, int numY, bool bLoop)
{
	if(  m_stAnimateSprite[id].hEffectTexture == INVALID_HANDLE_VALUE )
		return;

	int iTotalFrame = numX * numY;
	
	if( bLoop )
	{
		if( m_stAnimateSprite[id].iEffectFrame >= iTotalFrame)
			m_stAnimateSprite[id].iEffectFrame =0;
	}
	else
	{
		if( !m_bPlayNotLoopEffect || m_stAnimateSprite[id].iEffectFrame >= iTotalFrame )
		{
			m_bPlayNotLoopEffect = false;
			m_stAnimateSprite[id].iEffectFrame = 0;
			return;
		}
	}


	int Start_X = 0; 
	int Start_Y= 0;
	if( m_stAnimateSprite[id].iEffectFrame != 0 )
	{	
		Start_X = m_stAnimateSprite[id].iEffectFrame % numX;
		Start_Y = m_stAnimateSprite[id].iEffectFrame / numX;
	}

	g_pSunRenderer->RenderTexture(m_stAnimateSprite[id].hEffectTexture, (float)PosX , (float)PosY , (float) width ,(float) height , (float)Start_X * width , (float)Start_Y * height, (float)width, (float) height);	
}

void uiFateItemSystem::ProcessRollingNumber()
{
	// 숫자 랜덤으로 돌기기
	if( m_iState ==  eFIS_MOVIE_RUNNING ||m_iState == eFIS_WAIT_RESPONSE || m_iState == eFIS_RESULT_SUCCESS)
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
		int iValue[MAX_TXT];
		for( int i =0; i < MAX_TXT; ++i)
		{
			iValue[i] = rand() % 999;

			if( m_iCaptionEffectCount <= i)
			{
				Snprintf( szMessage, INTERFACE_STRING_LENGTH -1, _T("%d"), iValue[i]);
				m_pStatic[i]->SetTextColor(wzRandomColor);
				GlobalFunc::SetCaption(m_pStatic[i] , szMessage);
			}
		}
	}
}

void uiFateItemSystem::SetFateItemProcessState(eFIS eState)
{
	m_iState = eState;
	if( m_iState == eFIS_RESULT_SUCCESS )
	{
		g_SoundEffect.PlayUI(UI_SOUND_ITEM_VALIDATION_NUMBER_ROLLING);
	}
}

void uiFateItemSystem::SetTargetItemLock(bool bLock)
{
	SCSlotContainer * pInventory = ItemManager::Instance()->GetContainer(SI_INVENTORY);
	if( !pInventory )
		return;

	SCItemSlot & rTargetItem = (SCItemSlot &)pInventory->GetSlot(m_iTargetItemPos);
	rTargetItem.SetLock(bLock);
}

void uiFateItemSystem::InsertResultItem()
{
	g_SoundEffect.PlayUI(UI_SOUND_ITEM_VALIDATION_COMPLEATE);
	
	SCSlotContainer * pInventory = ItemManager::Instance()->GetContainer(SI_INVENTORY);
	if( pInventory )
	{
		// 타겟 아이템 슬롯 락을 풀어준다.
		SetTargetItemLock(FALSE);
		// 서버에서 받은아이템을 겜블 UI가 완료된후 비로서 넣어준다.
		pInventory->DeleteSlot(m_iTargetItemPos , NULL);
		pInventory->InsertSlot(m_iTargetItemPos , m_ResultItem);
	}
	//6326	럭키아이템 인증 의식이 완료되었습니다.
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6326));
	m_iState = eFIS_END;
}
