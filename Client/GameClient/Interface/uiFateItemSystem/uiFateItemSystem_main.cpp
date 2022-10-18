#include "SunClientPrecompiledHeader.h"

#include "uiFateItemSystem.h"
#include "BattleScene.h"

#include "MouseHandler.h"
#include "uiFateItemSystemMan/uiFateItemSystemMan.h"

WzID uiFateItemSystem::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("B999"),	//DLGPOS_BTN_EXIT,
	StrToWzID("B001"),	//DLGPOS_BTN_START,
	StrToWzID("B002"),	//DLGPOS_BTN_RESULT_VIEW,
	StrToWzID("B003"),	//DLGPOS_BTN_OK,
	StrToWzID("BT00"),	//DLGPOS_BTN_TARGET_ITEM,
	StrToWzID("PI00"),	//DLGPOS_PIC_MOVIE,
	StrToWzID("ST11"),	//DLGPOS_TXT_PHYSICAL
	StrToWzID("ST13"),	//DLGPOS_TXT_DURATION
	StrToWzID("ST14"),	//DLGPOS_TXT_DATE,
};
//------------------------------------------------------------------------------
/**
*/
uiFateItemSystem::uiFateItemSystem(InterfaceManager * pUIMan) 
: uiBase(pUIMan),
  m_pPictureMovie(NULL),
  m_hBink(NULL),
  m_hRenderTargetTexture(INVALID_HANDLE_VALUE),
  m_dwAccmulateTick(0),
  m_iCaptionEffectCount(0),
  m_iState(eFIS_IDLE),
  m_pIconImage(NULL),
  m_iTargetItemPos(0)
{

}
//------------------------------------------------------------------------------
/**
*/
uiFateItemSystem::~uiFateItemSystem()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	
	for( int i =0; i < MAX_BUTTON ; ++i)
	{
		m_pButton[i]= (CCtrlButtonWZ *) getControl(DLGPOS_BTN_EXIT +i);
		assert(m_pButton[i]);
	}

	for ( int i =0; i <MAX_TXT; ++i )
	{
		m_pStatic[i] = (CCtrlStaticWZ *) getControl(START_TXT +i);
		assert(m_pStatic[i]);
	}

	m_pPictureMovie		= ( CCtrlPictureWZ *) getControl(DLGPOS_PIC_MOVIE);
	assert(m_pPictureMovie);	

	m_stAnimateSprite[EFFECT_POS_TARGET_SLOT].hEffectTexture = 
		g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Icon\\Commercial\\activate_fate1.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD | TLS_NEAREST);

	m_stAnimateSprite[EFFECT_POS_STATIC].hEffectTexture =
		g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Icon\\Commercial\\activate_fate2.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD | TLS_NEAREST);


	m_stAnimateSprite[EFFECT_POS_TARGET_SLOT].iEffectFrame = 0;
	m_stAnimateSprite[EFFECT_POS_STATIC].iEffectFrame = 0;
}
//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::Release()
{
	ReleaseBink();
	if( m_hRenderTargetTexture )
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hRenderTargetTexture);
		m_hRenderTargetTexture = INVALID_HANDLE_VALUE;	
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::InitBink(bool bConsciousness)
{
	if( m_hBink )
	{
		ReleaseBink();
	}

	if( m_iState == eFIS_MOVIE_RUNNING)
	{
		if(bConsciousness )
		{
			m_hBink = BinkOpen( "Data\\Movie\\Effect2.bik",0);
		}
		else
		{
			m_hBink = BinkOpen( "Data\\Movie\\Effect.bik", 0 );
		}
	}
	else
		m_hBink = BinkOpen("Data\\movie\\idle.bik",0);

	assert(m_hBink);
	if( m_hBink  && m_hRenderTargetTexture == INVALID_HANDLE_VALUE )
	{
		//	3 - RGB     4 - RGBA
		m_hRenderTargetTexture = g_pSunRenderer->x_pManagerTexture->GenerateEmptyTexture(
			m_hBink->Width, m_hBink->Height, 3,
			TLS_NO_MIPMAP | TLS_EMPTY_TEXTURE |TLS_NO_MULTITHREAD );
		BinkSoundUseDirectSound( 0 );
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::ReleaseBink()
{
	if( m_hBink )
	{
		BinkClose(m_hBink);
		m_hBink =NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::OnUpdateSolarDialog()
{
	UpdateShowNHideButton();
}
//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::OnRenderSolarDialog()
{
	RenderMovie();

	if( m_pIconImage )
	{
		m_pIconImage->Render(&m_pButton[DLGPOS_BTN_TARGET_ITEM]->GetSizeRect());
	}
#ifdef _NA_000000_20130114_RENEWER_UI
	RenderEffectTexture(EFFECT_POS_TARGET_SLOT, (int)m_pButton[DLGPOS_BTN_TARGET_ITEM]->GetSizeRect().left - (42 + (42/5)), 
						(int)m_pButton[DLGPOS_BTN_TARGET_ITEM]->GetSizeRect().top - (28+ (28/5)),133,87,4,3,TRUE);
#else
		RenderEffectTexture(EFFECT_POS_TARGET_SLOT, (int)m_pButton[DLGPOS_BTN_TARGET_ITEM]->GetSizeRect().left - 42, 
						(int)m_pButton[DLGPOS_BTN_TARGET_ITEM]->GetSizeRect().top - 28,133,87,4,3,TRUE);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

	if( m_iState == eFIS_RESULT_SUCCESS)
	{
		RenderEffectTexture(EFFECT_POS_STATIC, (int) m_pStatic[m_iCaptionEffectCount]->GetSizeRect().left - 55, 
			(int) m_pStatic[m_iCaptionEffectCount]->GetSizeRect().top-33, 185,75,3,5, FALSE);
	}

#ifdef _INTERNATIONAL_UI
	RenderTooltip();
#endif//_INTERNATIONAL_UI
}
//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::Process(DWORD dwTick)
{
	ProcessCaptionEffect(dwTick);

	static DWORD dwAccmulateTick = 0;
	dwAccmulateTick += dwTick;
	if( dwAccmulateTick > EFFECT_FRAME_UPDATE_TIME)
	{
		ProcessRollingNumber();
		m_stAnimateSprite[0].iEffectFrame += 1;
		m_stAnimateSprite[1].iEffectFrame += 1;
		dwAccmulateTick = 0;
	}	

	static DWORD dwSecondAccmulateTick = 0;
	dwSecondAccmulateTick += dwTick;
	if( dwSecondAccmulateTick > MOVIE_FRAME_UPDATE_TIME)
	{
		UpdateMovie();
		dwSecondAccmulateTick =dwSecondAccmulateTick - MOVIE_FRAME_UPDATE_TIME;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_iState = eFIS_IDLE;
		m_dwAccmulateTick = 0;
		m_iCaptionEffectCount = 0;
		m_bPlayNotLoopEffect = false;
		m_iTargetItemPos = 0;
		InitBaseAtributeCaption();
		InitBink(false);
		MoveCenter();
		g_SoundEffect.PlayUI(UI_SOUND_ITEM_CLICK_VALIDATION);

		MouseHandler::Instance()->RollbackItemAtHand();
		g_InterfaceManager.LockDlgs(IM_FATEITEM_SYSTEM::FATEITEM_DLG);
		
	}
	else
	{
		g_InterfaceManager.UnLockDlgs();

		if (m_pIconImage)
        {
			ICONMGR()->RemoveIcon(m_pIconImage);
            m_pIconImage = NULL;
        }
		ReleaseBink();

		if( m_iState == eFIS_RESULT_SUCCESS )
		{
			// eFIS_RESULT_SUCCESS에서 창을 닫는경우가 생기면(강제로 존이동시) ResultItem을 넣고 닫자!!
			InsertResultItem();

			//InsertResultItem을 호출한 시점은 이미 SaveHero를 호출한 상태이고, Insert를 해봐야 Save가 안됐으므로
			// 다시한번 SaveHero를 호출한다. 효율은 안좋지만, 워낙 예외적인 상황에서 발생하므로 이렇게 하자.
			if( g_pHero )
			{
				BattleScene::SaveHero(g_pHero);
			}
		}
		SetTargetItemLock(FALSE);
	}

}
//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystem::MessageProc(SI_MESSAGE * pMessage)
{
	switch( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getControlIDToPos(pMessage->dwCtrlID) )
			{

			case DLGPOS_BTN_OK:
			case DLGPOS_BTN_EXIT:
				{
					ShowInterface(FALSE);
				}
				break;

			case DLGPOS_BTN_START:
				{
					//6325 럭키 아이템 인증 의식이 시작 되었습니다.
					g_SoundEffect.PlayUI(UI_SOUND_ITEM_VALIDATION_START);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6325));
					m_iState = eFIS_MOVIE_RUNNING;
					InitBink(true);
				}
				break;

			case DLGPOS_BTN_RESULT_VIEW:
				{
#ifdef __CN_000948_20080305_VALIDATE_REIHALT_IMMEDIATELY
					NET_SEND_CG_ITEM_VALIDATION_OF_REIHALT_SYN();
					m_iState = eFIS_WAIT_RESPONSE;
					InitBink(false);
#else
					//6325 럭키 아이템 인증 의식이 시작 되었습니다.
					g_SoundEffect.PlayUI(UI_SOUND_ITEM_VALIDATION_START);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6325));
					m_iState = eFIS_MOVIE_RUNNING;
					InitBink(false);
#endif
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
void uiFateItemSystem::NetworkProc(MSG_BASE * pMsg)
{

}

void uiFateItemSystem::NET_SEND_CG_ITEM_VALIDATION_OF_REIHALT_SYN()
{
	// 타겟 아이템을 LOCK이 성공하면 SYN 보냄.
	MSG_CG_ITEM_VALIDATION_OF_REIHALT_SYN SendMsg;
	SendMsg.m_TargetItemPos = m_iTargetItemPos;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
}

