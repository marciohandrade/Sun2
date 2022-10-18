#include "SunClientPrecompiledHeader.h"
#include "herogaugedialog.h"
#include "Hero.h"
#include "CommonCharacterFormula.h"
#include "InterfaceManager.h"
#include "Mouse.h"
#include "HeroActionInput.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiPartyLeavePopup/uiPartyLeavePopup.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiCommunityDlg/uiCommunityDlg.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"

#include "StateInfoParser.h"
#include "GameOptionParser.h"
#include "StatusIconManager.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "uiConvenienceStore/CashShop.h"
#endif
#ifdef _DH_BATTLEZONE2_
  #include "uiBattlezone2Man/uiBattlezone2Man.h"
#endif //_DH_BATTLEZONE2_

static const int HIT_EFFECT_TICK_PER_FRAME = 40;
static const int HIT_EFFECT_FRAME_COUNT  = 3;
static const float HIT_EFFECT_SIZE = 64;

const float MAX_RATIO	= 0.9f;

#define	TOOLTIP_STRING_NOTE_CODE	5798	
#define BLINK_RANGE_TICK	500

#pragma warning ( push )
#pragma warning(disable : 4800)


//-si-
// 11_HP.iwz 의 컨트롤 설정
WzID HeroGaugeDialog::m_wzId[HGAUGE_MAX] = 
{ 
	StrToWzID("T012"),  // Text %d
	StrToWzID("T002"),  // 

    StrToWzID("P000"),  // 체력
	StrToWzID("P001"),  // 마나
    StrToWzID("P002"),  // cyan

	StrToWzID("T005"),	// HGAUGE_TEXT_CURRENT_HP,
	StrToWzID("T006"),	// HGAUGE_TEXT_TOATAL_HP,
	StrToWzID("T011"),	// HGAUGE_TEXT_CURRENT_MP,
	StrToWzID("T008"),	// HGAUGE_TEXT_TOTAL_MP,
	StrToWzID("T009"),	// HGAUGE_TEXT_CURRENT_EXP,
	StrToWzID("T010"),	// HGAUGE_TEXT_TOTAL_EXP,

	
	StrToWzID("P004"),  // cyan
	StrToWzID("P005"),  // cyan
	StrToWzID("P006"),  // cyan
	StrToWzID("P007"),  // cyan
	StrToWzID("P008"),  // cyan
	StrToWzID("P009"),  // cyan
	StrToWzID("P010"),  // cyan
	StrToWzID("P011"),  // cyan
	StrToWzID("P012"),  // cyan
	StrToWzID("P013"),  // cyan

	// 경험치 볼~
	StrToWzID("B001"),  // cyan
	StrToWzID("B002"),  // cyan
	StrToWzID("B003"),  // cyan
	StrToWzID("B004"),  // cyan
	StrToWzID("B005"),  // cyan
	StrToWzID("B006"),  // cyan
	StrToWzID("B007"),  // cyan
	StrToWzID("B008"),  // cyan
	StrToWzID("B009"),  // cyan
	StrToWzID("B010"),  // cyan


	// 대가리
	StrToWzID("PI03"),  // 
	StrToWzID("PI04"),  // 
    StrToWzID("PI05"),  // 
	StrToWzID("PI06"),  // 
	StrToWzID("PI07"),  // 
	 

    // TODO: 파티중인지 일단, PC 마크로 처리
    StrToWzID("PI00"),  //
	StrToWzID("B101"),  // HGAUGE_PARTY_LEAVE // 탈퇴 버튼

	StrToWzID("CT00"),	//HGAUGE_CHK_MIC,

	StrToWzID("C100"),		//HGAUGE_CHK_CHAT_MSG,
	StrToWzID("S300"),		//HGAUGE_TEXT_CHAT_MSG,
	StrToWzID("B011"),		//HGAUGE_BTN_CHAT_MSG

	StrToWzID("C200"),		//HGAUGE_CHK_PRESENT_MSG,

	StrToWzID("CT01"),		//HGAUGE_CHK_NOTE_MSG

	StrToWzID("PI01"),	// HGAUGE_RESERVE_HP

	StrToWzID("BT00"),//HGAUGE_SKILLEFFECT_OPEN_BTN
	StrToWzID("BT01"),//HGAUGE_SKILLEFFECT_CLOSE_BTN

	StrToWzID("PI13"),//HGAUGE_SD_GAUGE
	StrToWzID("ST00"),//HGAUGE_TEXT_CURRENT_SP
	StrToWzID("ST02"),//HGAUGE_TEXT_TOATAL_SP


    StrToWzID("PI02"),  // cyan2
};

//exp계산 함수(비멤버)
static float CalculateExp()
{
	float fExpPercent=0.0f;
	EXPTYPE destexp		= g_pHero->GetNextExp();
	EXPTYPE curaccumexp	= g_pHero->GetAccumExp(g_pHero->GetLevel());
	EXPTYPE curexp		= g_pHero->GetExp();

	__int64 iXpForNextLevel = (__int64)destexp - (__int64)curaccumexp;
	__int64 iCurrentXpForNextLevel = (__int64)curexp - (__int64)curaccumexp;

	if (iXpForNextLevel < 1)
	{
		iXpForNextLevel = 1;
	}

	if (iCurrentXpForNextLevel < 0)
	{
		iCurrentXpForNextLevel = 0;
	}


	fExpPercent = ((float)iCurrentXpForNextLevel / (float)iXpForNextLevel) * 100.0f;

	fExpPercent = min(100.0f,fExpPercent);

	return fExpPercent;

}

HeroGaugeDialog::HeroGaugeDialog()
:m_iExpBallCount(0)
,m_bBlinkHPWarning(FALSE)
,m_bHPShowFlag(TRUE)
,m_bChatState( false )
,m_dwChatShowTick( 0 )
,m_bPresentState( false )
,m_dwPresentShowTick( 0 )
,m_bNoteState( false )
,m_dwShowTick( 0 )
,hp_prev_percent_(100)
{
	memset( m_szCharName, 0, MAX_CHARNAME_LENGTH + 1 );

	m_hTextureHitEffect = INVALID_HANDLE_VALUE;
	m_hTextureHeadMask = INVALID_HANDLE_VALUE;

    m_pCtrlPartyMark = NULL;
	m_pMicButton =  NULL;
	m_pChkChatMsg = NULL;
	m_pChkPresentMsg = NULL;
	m_pCtrlTextChatMsg = NULL;
	m_UseTooltipCtrls.clear();
	m_iTooltipStringCode = 0;

	m_UseTooltipCtrls[HGAUGE_BTN_CHAT_MSG]	= 5921;

	// 선물이 도착하였습니다.
	m_UseTooltipCtrls[HGAUGE_CHK_PRESENT_MSG] = 5980;

	m_UseTooltipCtrls[HGAUGE_CHK_NOTE_MSG] = TOOLTIP_STRING_NOTE_CODE;

    m_blinkTick = 0;

	//6333 아이콘 최대 출력 개수를 확장 합니다.
	m_UseTooltipCtrls[HGAUGE_SKILLEFFECT_OPEN_BTN] = 6333;
	//6334 아이콘 최대 출력 개수를 원래 상태로 복구 합니다.
	m_UseTooltipCtrls[HGAUGE_SKILLEFFECT_CLOSE_BTN] = 6334;

	m_bAllSkillEffectView = FALSE;
	m_btSkillEffectCnt = 0;
	m_pCtrlSEOpenBtn = NULL;
	m_pCtrlSECloseBtn = NULL;
}

HeroGaugeDialog::~HeroGaugeDialog()
{

	if (m_hTextureHitEffect != INVALID_HANDLE_VALUE) {

		g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hTextureHitEffect);		
		m_hTextureHitEffect = INVALID_HANDLE_VALUE;
	}

	m_UseTooltipCtrls.clear();

}

VOID HeroGaugeDialog::Init( CDrawBase * /*pDrawBase*/ )
{
	//MoveWindowWZ( 0, 0 );

    memset ( (TCHAR *)m_szCharName, 0, sizeof (m_szCharName));

	m_fHPGaugeSize = (float)getControl(HGAUGE_RED)->GetSize().right;
	SetHeroHP( 75.f );
	m_fMPGaugeSize = (float)getControl(HGAUGE_BLUE)->GetSize().right;
	SetHeroMP( 75.f );
	m_fExpGaugeSize = (float)getControl(HGAUGE_EXP)->GetSize().right;
	m_fSDGaugeSize = (float)getControl(HGAUGE_SD_GAUGE)->GetSize().right;
	SetHeroSD( 75.f );
	m_pCtrlTextSDInfo[0] = (CCtrlStaticWZ *)getControl( HGAUGE_TEXT_CURRENT_SD);
	m_pCtrlTextSDInfo[1] = (CCtrlStaticWZ *)getControl( HGAUGE_TEXT_TOATAL_SD);
	m_pCtrlTextHPInfo[0] = (CCtrlStaticWZ *)getControl( HGAUGE_TEXT_CURRENT_HP);
	m_pCtrlTextHPInfo[1] = (CCtrlStaticWZ *)getControl( HGAUGE_TEXT_TOATAL_HP);
	m_pCtrlTextMPInfo[0] = (CCtrlStaticWZ *)getControl( HGAUGE_TEXT_CURRENT_MP);
	m_pCtrlTextMPInfo[1] = (CCtrlStaticWZ *)getControl( HGAUGE_TEXT_TOTAL_MP );
	m_pCtrlTextEXPInfo[0] =	(CCtrlStaticWZ *)getControl( HGAUGE_TEXT_CURRENT_EXP);
	m_pCtrlTextEXPInfo[1] =	(CCtrlStaticWZ *)getControl( HGAUGE_TEXT_TOTAL_EXP);

	for(int i=0;i<2;++i)
	{
		if(m_pCtrlTextHPInfo[i])
			m_pCtrlTextHPInfo[i]->ShowWindowWZ(WZ_HIDE);

		if(m_pCtrlTextMPInfo[i])
			m_pCtrlTextMPInfo[i]->ShowWindowWZ(WZ_HIDE);

		if(m_pCtrlTextEXPInfo[i])
			m_pCtrlTextEXPInfo[i]->ShowWindowWZ(WZ_HIDE);
		
 		if(m_pCtrlTextSDInfo[i])
			m_pCtrlTextSDInfo[i]->ShowWindowWZ(WZ_HIDE);
 	}

	m_pMicButton =  static_cast<CCtrlButtonCheckWZ*>(getControl(HGAUGE_CHK_MIC));

	//TODO: 음성채팅용 컨트롤이 UI에서 제거되면 이 코드도 제거해야 함
	if(m_pMicButton)
		m_pMicButton->ShowWindowWZ(WZ_HIDE);

	m_pChkChatMsg = static_cast<CCtrlButtonCheckWZ*>(getControl(HGAUGE_CHK_CHAT_MSG));
	if(m_pChkChatMsg)
		m_pChkChatMsg->SetCheckState(false);

	m_pChkPresentMsg = static_cast<CCtrlButtonCheckWZ*>(getControl(HGAUGE_CHK_PRESENT_MSG));
	if(m_pChkPresentMsg)
	{
		m_pChkPresentMsg->SetCheckState(false);
#ifdef __CHINA_BLOCK_ITEM_SHOP_PRESENT
		m_pChkPresentMsg->ShowWindowWZ( WZ_HIDE );
		m_pChkPresentMsg->EnableWindowWZ( FALSE );
#endif
	}

	m_pChkNoteMsg	= static_cast<CCtrlButtonCheckWZ*>( getControl( HGAUGE_CHK_NOTE_MSG ) );
	if( m_pChkNoteMsg != NULL )
		m_pChkNoteMsg->SetCheckState( false );


	m_pCtrlTextChatMsg = static_cast<CCtrlStaticWZ*>(getControl(HGAUGE_TEXT_CHAT_MSG));
	if(m_pCtrlTextChatMsg)
		GlobalFunc::SetCaption(m_pCtrlTextChatMsg, _T(""));

	m_iExpBallCount = 0;

	m_BlinkTimer.SetTimer( BLINK_GAP_TICK );

	m_iHitEffectDelay = 0;

	// 이펙트
	if (m_hTextureHitEffect == INVALID_HANDLE_VALUE) {
		m_hTextureHitEffect = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\HitEffect.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}

	if (m_hTextureHeadMask == INVALID_HANDLE_VALUE)
	{
		m_hTextureHeadMask = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\HP_AP00.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}

    m_pCtrlPartyMark =
        static_cast<CCtrlStaticWZ *>(this->getControl(HGAUGE_PARTY_MARK));
    assert (m_pCtrlPartyMark);


	
	m_rtDialog.left = 3;
	m_rtDialog.right = m_rtDialog.left + 20;
	m_rtDialog.top = 3;
	m_rtDialog.bottom = m_rtDialog.top + 20;

	CCtrlPictureWZ* pSkillEft = static_cast<CCtrlPictureWZ*>(getControl(HGAUGE_SKILLEFFECT0));
	if(pSkillEft)
	{
		m_rtSkillEffect.left = pSkillEft->GetSizeRect().left;
		m_rtSkillEffect.top = pSkillEft->GetSizeRect().top;
		m_rtSkillEffect.bottom = pSkillEft->GetSizeRect().bottom;
	}
	pSkillEft = NULL;
	pSkillEft = static_cast<CCtrlPictureWZ*>(getControl(HGAUGE_SKILLEFFECT9));
	if(pSkillEft)
	{
		m_rtSkillEffect.right = pSkillEft->GetSizeRect().right;
	}

	m_bAllSkillEffectView = FALSE;
	m_btSkillEffectCnt = 0;

	m_pCtrlSEOpenBtn  = static_cast<CCtrlButtonWZ*>(getControl(HGAUGE_SKILLEFFECT_OPEN_BTN));
	if(m_pCtrlSEOpenBtn)
	{
		m_pCtrlSEOpenBtn->ShowWindowWZ(WZ_SHOW);
	}
	m_pCtrlSECloseBtn = static_cast<CCtrlButtonWZ*>(getControl(HGAUGE_SKILLEFFECT_CLOSE_BTN));
	if(m_pCtrlSECloseBtn)
	{
		m_pCtrlSECloseBtn->ShowWindowWZ(WZ_HIDE);
	}

	m_pBattleZoneMan = (uiBattleZoneMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE );
	assert(m_pBattleZoneMan);

    m_pCtrlExpImage = static_cast<CControlWZ*>(getControl(HGAUGE_MAX_LV_EXP));
    assert(m_pCtrlExpImage);

    status_icon_container_.reserve(30);
}

VOID HeroGaugeDialog::MessageProc( SI_MESSAGE * pMessage )
{
	if( g_pHero )
	{
		if( g_pHero->IsShowFullMap() )
		{
			return;
		}
	}

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			int iMouseY = Mouse::Instance()->GetMouseY();
			if(iMouseY < m_rtSkillEffect.top)
			{
				if (g_pHero)
				{
					g_HeroInput.SetCurrentTarget(g_pHero->GetObjectKey());
				}
			}

			switch( pos )
			{
			case HGAUGE_PARTY_LEAVE:
				{
                    uiPartyMan * pPartyMan = (uiPartyMan *)g_InterfaceManager.GetUserInterfaceManager( UIMAN_PARTY );
                    if ((pPartyMan != NULL) && (pPartyMan->IsParty() == false) && (g_pHero != NULL))
                    {
                        g_pHero->SendZONE_EX_QUEST_PORTAL_ENTER_SYN(); 
                    }
                    else
                    {
					    LeaveParty();
                    }
				}
				break;


			case HGAUGE_CHK_CHAT_MSG:
			case HGAUGE_TEXT_CHAT_MSG:
			case HGAUGE_BTN_CHAT_MSG:
				{
                    if (GameFramework::GetCurScene() == SCENE_TYPE_SSQ)
                    {
                        break;
                    }
                    uiCommunityMan* pCommMan = 
                        static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
                    if(pCommMan)
                    {
                        pCommMan->ShowFriendChatDlg(NULL);
                    }
				}
				break;

			case HGAUGE_CHK_PRESENT_MSG:
				{
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL

    #if defined(_SUNGAME_VER)
                    if (PROGRAMPARAM->is_block_cash_shop() == false)
    #endif //defined(_SUNGAME_VER)
                    {
                        ConvenienceStore::CashShop::instance()->OpenCashShopWithGiftMenu();

                    }
#else
					uiCashItemShopMan *pMan = static_cast<uiCashItemShopMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
					if(pMan && pMan->GetNewPresentRcvCount())
					{
						pMan->SetRqstState(eRqstState_Present_List);
						pMan->Send_CG_SHOP_CASH_REQ_SYN();
					}
#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
				}
				break;

			case HGAUGE_CHK_NOTE_MSG:
				{
                    if (GameFramework::GetCurScene() == SCENE_TYPE_SSQ)
                    {
                        break;
                    }

					uiCommunityMan* pCommMan = GET_DIALOG_MANAGER( uiCommunityMan, UIMAN_COMMUNITY );

					if( pCommMan )
					{
						pCommMan->ShowCommunityDlg();
															
						uiCommunityDlg *pCommunityDlg =(uiCommunityDlg*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_COMMUNITY_DLG);

						if(pCommunityDlg)
						{
							pCommunityDlg->ChangeMode(eCM_MAIL);
						}
					}
				}
				break;

			case HGAUGE_SKILLEFFECT_OPEN_BTN:
				{
					m_bAllSkillEffectView = TRUE;
					m_pCtrlSEOpenBtn->ShowWindowWZ(WZ_HIDE);
					m_pCtrlSECloseBtn->ShowWindowWZ(WZ_SHOW);
				}
				break;
			case HGAUGE_SKILLEFFECT_CLOSE_BTN:
				{
					m_bAllSkillEffectView = FALSE;
					m_pCtrlSEOpenBtn->ShowWindowWZ(WZ_SHOW);
					m_pCtrlSECloseBtn->ShowWindowWZ(WZ_HIDE);
				}
				break;
			default:
				break;
			}
		}
		break;

		case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			m_iTooltipStringCode = getTooltipString( pos );
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}
//------------------------------------------------------------------------------ 
void HeroGaugeDialog::hp_prev_percent_increase(float val)
{
    hp_prev_percent_ += val;
    hp_prev_percent_ = min(100.0f, hp_prev_percent_);
    hp_prev_percent_ = max(0.0f, hp_prev_percent_);
}
//------------------------------------------------------------------------------ 
void HeroGaugeDialog::set_hp_prev_percent(float val) 
{ 
    hp_prev_percent_ = val; 
    hp_prev_percent_ = min(100.0f, hp_prev_percent_);
    hp_prev_percent_ = max(0.0f, hp_prev_percent_);
}
//------------------------------------------------------------------------------ 
VOID HeroGaugeDialog::SetHeroHP( float fHPPercent )
{
    CControlWZ* ctl = getControl( HGAUGE_RED );
    assert( ctl );

    if( !ctl )
    {
        return;
    }

    fHPPercent = max( 0.0f, min( 100.0f, fHPPercent ) );

	RECT rect;
	rect = ctl->GetSizeRect();

	float width, height;
	height = (float)ctl->GetSize().bottom;

	m_fHitEffectPosY = rect.top + (height - HIT_EFFECT_SIZE) / 2;

	if (m_fHPGaugeSize >= 0.0f )
	{
		width = (m_fHPGaugeSize/100.f) * fHPPercent;

		ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fHeight =height;
		if(width>0.f)
		{
			ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =width;
		}
		else
		{
			ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =0.1f;
		}
		ctl->MoveWindowWZ( (float)rect.left, (float)rect.top, width, height );

		m_fHitEffectPosX = rect.left + width - 32;
	}
	else
	{
		m_fHitEffectPosX = (float)rect.left;
	}
}

VOID HeroGaugeDialog::SetHeroMP( float fMPPercent )
{
	CControlWZ* ctl = getControl( HGAUGE_BLUE );
	assert( ctl );

	if( !ctl )
	{
		return;
	}

	if (fMPPercent >= 100.0f)
	{
		fMPPercent = 100.0f;
	}

	if (fMPPercent < 0.0f)
	{
		fMPPercent = 0.0f;

	}

	RECT rect;
	rect = ctl->GetSizeRect();
	float width;
	float height = (float)ctl->GetSize().bottom;

	if (m_fMPGaugeSize >= 0.0f )
	{
		width = (m_fMPGaugeSize/100.f) * fMPPercent;

		ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fHeight =height;
		if(width>0.f)
		{
			ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =width;
		}
		else
		{
			ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =0.1f;
		}
		ctl->MoveWindowWZ( (float)rect.left, (float)rect.top, width, height );
	}
	
}
VOID HeroGaugeDialog::SetHeroExp(float fPercent)
{
    CControlWZ* ctl = getControl(HGAUGE_EXP);
    assert( ctl );
    if (m_pCtrlExpImage)
    {
	    if ((g_pHero != NULL) &&
            (GameOptionParser::Instance()->GetMaxLevelChangeOfClass1st() == g_pHero->GetLevel()))
        {
            m_pCtrlExpImage->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
            m_pCtrlExpImage->ShowWindowWZ(WZ_HIDE);
        }
    }
	if( !ctl )
	{
		return;
	}
	assert( fPercent <= 100 );

	if (fPercent >= 100.0f)
	{
		fPercent = 100.0f;
	}

	if (fPercent < 0.0f)
	{
		fPercent = 0.0f;
	}

	RECT rect;
	rect = ctl->GetSizeRect();
	float width ;
	float height = (float)ctl->GetSize().bottom;

	if (m_fExpGaugeSize >= 0.0f )
	{
		width = (m_fExpGaugeSize/100.f) * fPercent;
		ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fHeight =height;
		if(width>0.f)
		{
			ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =width;
		}
		else
		{
			ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =0.1f;
		}
		ctl->MoveWindowWZ( (float)rect.left, (float)rect.top, width, height );
	}

}

VOID HeroGaugeDialog::SetHeroSD( float fPercent)
{
	CControlWZ* ctl = getControl( HGAUGE_SD_GAUGE );
	assert( ctl );

	if( !ctl )
	{
		return;
	}

	if (fPercent >= 100.0f)
	{
		fPercent = 100.0f;
	}

	if (fPercent < 0.0f)
	{
		fPercent = 0.0f;

	}

	RECT rect;
	rect = ctl->GetSizeRect();
	float width ;
	float height = (float)ctl->GetSize().bottom;

	if (m_fSDGaugeSize >= 0.0f )
	{
		width = (m_fSDGaugeSize/100.f) * fPercent;
		ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fHeight =height;
		if(width>0.f)
		{
			ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =width;
		}
		else
		{
			ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =0.1f;
		}
		ctl->MoveWindowWZ( (float)rect.left, (float)rect.top, width, height );
	}
}


VOID HeroGaugeDialog::SetHeroReserveHP( float val )
{
	val = max( 0.0f, min( 100.0f, val ) );

	CControlWZ* hpCtl = getControl( HGAUGE_RED );
	assert( hpCtl );

	CControlWZ* rcvCtl = getControl( HGAUGE_RESERVE_HP );
	assert( rcvCtl );

	RECT hpRect, rcvRect;
	float width, height;

	hpRect = hpCtl->GetSizeRect();
	rcvRect = rcvCtl->GetSize();

    // reserveHP바를 HP바의 왼쪽에 맞추는 경우
    //rcvRect.left = hpRect.left;
    width = ( m_fHPGaugeSize * ( val / 100.0f ) );

    // reserveHP바를 HP바의 오른쪽에 맞추는 경우
	// rcvRect.left = hpRect.right + 1;
	// width = m_fHPGaugeSize * ( val / 100.0f );

	height = (float)rcvRect.bottom;

	if( rcvRect.left + width > hpRect.left + m_fHPGaugeSize )
	{
		width = ( hpRect.left + m_fHPGaugeSize ) - rcvRect.left;
	}

	rcvCtl->MoveWindowWZ( (float)rcvRect.left, (float)rcvRect.top, width, height );

}


VOID HeroGaugeDialog::SetHeroName( char * /*pszHeroName*/ )
{
}


VOID HeroGaugeDialog::SetLevel( DWORD dwLevel )
{
	Sprintf( m_szTemp, _T("%d"), dwLevel );

	CCtrlStaticWZ * pControl = (CCtrlStaticWZ*)getControl(HGAUGE_TEXT_LEVEL);

	GlobalFunc::SetCaption(pControl, m_szTemp);
}

VOID HeroGaugeDialog::NetworkProc( MSG_BASE * /*pMsg*/ )
{

}

POSTYPE HeroGaugeDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < HGAUGE_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return HGAUGE_MAX;
}

CControlWZ * HeroGaugeDialog::getControl( POSTYPE AtPos )
{
	
	assert( AtPos < HGAUGE_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

void HeroGaugeDialog::CheckNoteMessage( DWORD dwTick )
{
	uiCommunityMan* pCommMan = GET_DIALOG_MANAGER( uiCommunityMan, UIMAN_COMMUNITY );
	
	if( pCommMan == NULL || m_pChkNoteMsg == NULL )
		return;

	if( pCommMan->HaveNewNote() )
	{
		m_pChkNoteMsg->ShowWindowWZ( WZ_SHOW );

		m_dwShowTick += dwTick;

		if( m_dwShowTick >= BLINK_RANGE_TICK )
		{
			m_dwShowTick = 0;
			m_bNoteState = !m_bNoteState;
		}

		m_pChkNoteMsg->SetCheckState( m_bNoteState );
		
	}
	else
	{
		m_pChkNoteMsg->ShowWindowWZ( WZ_HIDE );
	}
}

void HeroGaugeDialog::CheckChatMessage( DWORD dwTick )
{
	uiCommunityMan* pCommMan = GET_DIALOG_MANAGER( uiCommunityMan, UIMAN_COMMUNITY );

	if( pCommMan == NULL || m_pChkChatMsg == NULL )
		return;

	if( pCommMan->GetNewFriendChatCount() && !pCommMan->GetRejectChat() )
	{
		TCHAR szCnt[64] = {0,};
		Sprintf(szCnt, _T("%d"), pCommMan->GetNewFriendChatCount());
		GlobalFunc::SetCaption(m_pCtrlTextChatMsg, szCnt);

		m_pChkChatMsg->ShowWindowWZ( WZ_SHOW );
		
		m_dwChatShowTick += dwTick;

		if( m_dwChatShowTick >= BLINK_RANGE_TICK )
		{
			m_dwChatShowTick = 0;
			m_bChatState = !m_bChatState;
		}

		m_pChkChatMsg->SetCheckState( m_bChatState );

	}
	else
	{
		GlobalFunc::SetCaption(m_pCtrlTextChatMsg, _T(""));
		m_pChkChatMsg->ShowWindowWZ( WZ_HIDE );
	}
}

void HeroGaugeDialog::CheckPresentMessage( DWORD dwTick )
{
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    //! 2011.7.22 / i4u4me / 캐시샵 작업 

    if( m_pChkPresentMsg == NULL )
        return;

    ConvenienceStore::CashShop* pInstance = ConvenienceStore::CashShop::instance();
    if( pInstance == NULL )
        return;

    if( pInstance->HasNewPresent() )
    {
        m_pChkPresentMsg->ShowWindowWZ( WZ_SHOW );

        m_dwPresentShowTick += dwTick;

        if( m_dwPresentShowTick >= BLINK_RANGE_TICK )
        {
            m_dwPresentShowTick = 0;
            m_bPresentState = !m_bPresentState;
        }

        m_pChkPresentMsg->SetCheckState(m_bPresentState);
    }
    else
    {
        m_pChkPresentMsg->ShowWindowWZ( WZ_HIDE );
    }


#else

	uiCashItemShopMan *pMan = GET_DIALOG_MANAGER( uiCashItemShopMan, UIMAN_CASH_ITEM_SHOP );

	if( pMan == NULL ||  m_pChkPresentMsg == NULL )
		return;

	if( pMan->GetNewPresentRcvCount() )
	{
		m_pChkPresentMsg->ShowWindowWZ( WZ_SHOW );

		m_dwPresentShowTick += dwTick;

		if( m_dwPresentShowTick >= BLINK_RANGE_TICK )
		{
			m_dwPresentShowTick = 0;
			m_bPresentState = !m_bPresentState;
		}

		m_pChkPresentMsg->SetCheckState( m_bPresentState );
	}
	else
	{
		m_pChkPresentMsg->ShowWindowWZ( WZ_HIDE );
	}

#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL

}

VOID HeroGaugeDialog::OnUpdateSolarDialog()
{
	if (GENERALPARAM->IsNet())
	{
		if (!g_pHero)	return;

		updateTooltip();

		CCtrlStaticWZ * pCtrlStatic =  (CCtrlStaticWZ * )getControl(HGAUGE_TEXT_MP_OR_SP);

		if (pCtrlStatic)
		{
			// sp/mp 를 정하자.
			if (eCHAR_BERSERKER == g_pHero->GetClass())
			{
				g_InterfaceManager.GetInterfaceString(eST_SP, m_szTemp, 512);
				GlobalFunc::SetCaption( pCtrlStatic, m_szTemp );
			}
			else
			{
				g_InterfaceManager.GetInterfaceString(eST_MP, m_szTemp, 512);
				GlobalFunc::SetCaption( pCtrlStatic, m_szTemp );
			}
		}

		DWORD maxhp = max(g_pHero->GetMaxHP(),1);
		DWORD curhp = g_pHero->GetHP();

		//safecode:현재hp가 맥스hp보다 클수는없다
		if(curhp>=maxhp)
			curhp=maxhp;

		DWORD maxmp = max(g_pHero->GetMaxMP(),1);
		DWORD curmp = g_pHero->GetMP();
		LEVELTYPE CurLevel = g_pHero->GetLevel();
		

		SetHeroMP( ((float)curmp / (float)maxmp) * 100.0f );

		DWORD cursd	= g_pHero->GetSD();
		DWORD curmaxsd = g_pHero->GetMaxSD();
		SetHeroSD( ( (float)cursd /  (float)curmaxsd ) * 100.0f);

		DWORD reserveHP = g_pHero->GetReserveHP();
		//assert( reserveHP <= maxhp );
        if (reserveHP > maxhp)
        {
            reserveHP = maxhp;
        }
 
        DWORD dest_hp = curhp + reserveHP;
        if (dest_hp > maxhp)
        {
            dest_hp = maxhp;
        }
        float dest_hp_percent = (float)dest_hp / (float)maxhp * 100.0f;
        {
            static DWORD cur_time = timeGetTime();
            static DWORD prev_time = timeGetTime();
            cur_time = timeGetTime();
            DWORD gab = cur_time - prev_time;

            float vel_per_second = CGraphicParam::Instance()->hero_gauge_velocity(maxhp, g_pHero->GetSitState() == TRUE);
            float vel_inc = vel_per_second * gab / 1000.0f;
 
            hp_prev_percent_increase(vel_inc); 
            if (hp_prev_percent() > dest_hp_percent)
            {
                set_hp_prev_percent(dest_hp_percent);
            }
            prev_time = cur_time;
        }
 
        SetHeroHP(hp_prev_percent());
        SetHeroReserveHP(dest_hp_percent);

        uiGuideSystemMan* guide_manager =
            static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

        if (guide_manager)
        {
            guide_manager->CheckHelpMessage(TotalGuideNotice::kUnderHP, 
                TotalGuideNotice::kNumbericType, dest_hp);
            guide_manager->CheckHelpMessage(TotalGuideNotice::kUnderHP, 
                TotalGuideNotice::kPercentageType, static_cast<DWORD>(dest_hp_percent));
        }

	    float fExpPercent = CalculateExp();
	
		int iBallCount = 0;

		while (fExpPercent >= 10.0f)
		{
			fExpPercent -= 10.0f;
			iBallCount++;
		}

		// 경험치 공 갯수를 세팅
		SetExpBallCount(iBallCount);
		// 10% 이하 부분을 경험치 바에 세팅
		SetHeroExp(fExpPercent * 10.0f);
		SetLevel( CurLevel );

		uiPartyMan * pPartyMan = (uiPartyMan *)g_InterfaceManager.GetUserInterfaceManager( UIMAN_PARTY );
		if ( pPartyMan )
		{
			if ( GetCtrlLeavePartyBtn() )
			{
                if ((g_pHero != NULL) && (g_pHero->FixedPortalState.has_portal_info() == true))
                {
                    ShowLeavePartyBtn(TRUE);
                }
                else
                {
				    ShowLeavePartyBtn( pPartyMan->IsParty() );
                }
			}			
		}

		if ( GetCtrlPCModeBtn() )
		{
			ShowPCModeBtn( g_pHero->GetPCBangMode() );
		}
	}
	else
	{
		SetHeroExp(99.0f);
	}

	// 대가리보이기
	if (g_pHero)
	{
		// 3d 얼굴로 교체한다.
		// 기존의 얼굴은 안보이게한다.
		// lammy waring 수정
		for (int a = eCHAR_BERSERKER; a <=  eCHAR_ELEMENTALIST; a++) 
		{
			int index = HGAUGE_HEADIMAGE1 + a - 1;

			CControlWZ *pControl = getControl(index);

			if (pControl)
			{
				pControl->ShowWindowWZ(WZ_HIDE);
			}


		}
	}
    // 우클릭(삭제가능 여부 체크하여 처리)
    if (mouseover_status_icon_index_ >= 0 && mouseover_status_icon_index_ < 30)
    {   
        SKILL_EFFECT_IMAGE* status_icon = &(status_icon_container_[mouseover_status_icon_index_]);
        if ((status_icon != NULL) &&
            ((g_Input.GetState(MOUSE_RBUTTON, IKS_MOUSE_DOWN) == TRUE) ||
            (g_Input.GetState(MOUSE_RBUTTON, KS_MOUSE_DOWN) == TRUE)))
        {
            if(status_icon->pStateInfo->m_bDelType == TRUE)
            {
                MSG_CG_STATUS_DELETE_CHAR_STATE_SYN send_packet;
                send_packet.m_eCharStateType = 
                    static_cast<eCHAR_STATE_TYPE>(status_icon->pStateInfo->m_wStateID);
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

//#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
//                // 디펜스 모션을 사용해서 자신에게 피격스턴 상태가 부여되고 이 상태를 삭제하면 모션을 스킵하는 하드코딩 ㅋ
//                static int state_code = 121;    // 피격스턴의 상태코드
//                if( status_icon->pStateInfo->m_wStateID == state_code )
//                {
//                    if( StateBase* current_state = g_pHero->GetCurStateInstance() )
//                    {
//                        // Skip DefenceMotionState 
//                        current_state->ProcessExtraCommand();
//                    }
//                }
//#endif
            }
            else
            {
                TCHAR notice_message[INTERFACE_STRING_LENGTH + 1] = {0,};
                TCHAR temp_string[INTERFACE_STRING_LENGTH + 1] = {0,};
                TCHAR status_name[INTERFACE_STRING_LENGTH + 1] = {0,};
                //6332 %s 상태 아이콘을 삭제 할 수 없습니다.
                g_InterfaceManager.GetInterfaceString(6332, temp_string);
                g_InterfaceManager.GetInterfaceString(status_icon->pStateInfo->m_dwNamecode, status_name);
                Snprintf(notice_message, INTERFACE_STRING_LENGTH, temp_string, status_name);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, notice_message);
            }
        } 
    }
}

VOID HeroGaugeDialog::OnRenderSolarDialog()
{
	if (IsVisibleWindow())
	{
		// 대갈통 렌더
        CControlWZ *pHeadControl = getControl(HGAUGE_HEADIMAGE1);

		if (pHeadControl) 
		{
			RECT rc = pHeadControl->GetSizeRect();

			HANDLE hTexture = g_InterfaceManager.GetMyHeadTexture();

			if (hTexture != INVALID_HANDLE_VALUE)
			{
				g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA);

				g_pSunRenderer->x_pManagerTexture->SetColorArg1(0,CManagerTexture::ETARG_TEXTURE);				
				g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(0,CManagerTexture::ETARG_TEXTURE);
				g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(0,CManagerTexture::ETOP_SELECTARG1);
				g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(0,CManagerTexture::ETOP_SELECTARG1);

				g_pSunRenderer->x_pManagerTexture->SetColorArg1(1,CManagerTexture::ETARG_CURRENT);				
				g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(1,CManagerTexture::ETARG_CURRENT);
				g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(1,CManagerTexture::ETOP_SELECTARG1);
				g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(1,CManagerTexture::ETOP_MODULATE);

				g_pSunRenderer->x_pManagerTexture->SetAlphaArg2(1,CManagerTexture::ETARG_TEXTURE);

                g_pSunRenderer->RenderTextureBlending(
					hTexture,
					m_hTextureHeadMask,
					(float)rc.left - 10,
					(float)rc.top - 10, 
					(float)100.0f, 
					(float)100.0f);
				
				g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(0);
				g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(1);
				g_pSunRenderer->x_pManagerTexture->SetColorArg1(0);
				g_pSunRenderer->x_pManagerTexture->SetColorArg1(1);				

				g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(0);
				g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(1);
				g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(0);
				g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(1);
				g_pSunRenderer->x_pManagerTexture->SetAlphaArg2(1);
			}

		}

		if(m_pCtrlSEOpenBtn && m_pCtrlSECloseBtn)
		{
			if(10 >= m_btSkillEffectCnt) //10 한열의 최대 상태(버프) 표시 갯수
			{
				m_pCtrlSEOpenBtn->ShowWindowWZ(WZ_HIDE);
				m_pCtrlSECloseBtn->ShowWindowWZ(WZ_HIDE);
			}
			else
			{
				if(m_bAllSkillEffectView) //모든 상태(버프) 보일거냐,말거냐
				{
					m_pCtrlSEOpenBtn->ShowWindowWZ(WZ_HIDE);
					m_pCtrlSECloseBtn->ShowWindowWZ(WZ_SHOW);
				}
				else
				{
					m_pCtrlSEOpenBtn->ShowWindowWZ(WZ_SHOW);
					m_pCtrlSECloseBtn->ShowWindowWZ(WZ_HIDE);
				}
			}
		}

		if (m_iHitEffectDelay) 
		{
			int iHitEffectFrame = HIT_EFFECT_FRAME_COUNT - (m_iHitEffectDelay / HIT_EFFECT_TICK_PER_FRAME);

			float fSrcX;
			float fSrcY;

			switch (iHitEffectFrame)
			{
			default:
			case 0:
			case 1:
				fSrcX = 0;
				fSrcY = 0;
				break;
			case 2:

				fSrcX = HIT_EFFECT_SIZE;
				fSrcY = 0;

				break;
			case 3:
				fSrcX = 0;
				fSrcY = HIT_EFFECT_SIZE;
				break;
			}



			
			g_pSunRenderer->RenderTexture(m_hTextureHitEffect,m_fHitEffectPosX,m_fHitEffectPosY,HIT_EFFECT_SIZE,HIT_EFFECT_SIZE,fSrcX,fSrcY,HIT_EFFECT_SIZE,HIT_EFFECT_SIZE);
		}

		RenderGaugeNumber();
		RenderSuperRoomIcon();
	}
}


void HeroGaugeDialog::GetSkillEffectArea(int status_index, RECT* icon_rect)
{
    if (icon_rect == NULL)
    {
        return;
    }

    const int KAreaSize = StatusIconManager::STATUS_ICON_SIZE + 1;
    icon_rect->left = m_rtSkillEffect.left + ((status_index%10) * KAreaSize);
    icon_rect->right = icon_rect->left + KAreaSize;
    icon_rect->top = m_rtSkillEffect.top + ((status_index/10) * KAreaSize);
    icon_rect->bottom = icon_rect->top + KAreaSize;
}

void HeroGaugeDialog::UpdateStatusIcon()
{
    status_icon_container_.clear();
    if (g_pHero == NULL)
    {
        return;
    }

    int mouse_x_position = Mouse::Instance()->GetMouseX();
    int mouse_y_position = Mouse::Instance()->GetMouseY();
    mouseover_status_icon_index_ = 255;

    // Hero 의 SKILL_EFFECT 를 순회
    BASE_STATEINFO* state_info = NULL;
    SKILL_EFFECT_IMAGE status_icon;
    DWORD hero_object_key = g_pHero->GetObjectKey();
    int status_Index = 0;
    RECT draw_rect;

    std::vector<SKILL_EFFECT*>& array = g_pHero->GetSkillEffectArray();
    for( size_t i=0;i!=array.size();++i)
    {
        SKILL_EFFECT* skill_effect = array[i];

        state_info = StateInfoParser::Instance()->GetStateInfo(skill_effect->dwStatusID);
        status_icon.iEffectID = skill_effect->iEffectID;
        status_icon.dwStatusID = skill_effect->dwStatusID;
        status_icon.dwOwnerID = hero_object_key;
        status_icon.pSkillEffect = skill_effect;
        status_icon.pStateInfo = state_info;
        status_icon.m_byAlpha = 0xFF;
        // draw_rect 계산
        GetSkillEffectArea(status_Index, &draw_rect);
        status_icon.rcDrawRect = draw_rect;
        
        // 마우스 오버체크
        if ((draw_rect.left <= mouse_x_position) && 
            (draw_rect.right >= mouse_x_position) &&
            (draw_rect.top <= mouse_y_position) && 
            (draw_rect.bottom >= mouse_y_position))
        {
            // 툴팁 출력
            uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
            if (tooltip_manager)
            {
                tooltip_manager->RegisterSkillEffectInfoTooltip(&status_icon, TRUE);
				tooltip_manager->SetTooltipDialogID(GetDialogKey());

            }

            // 마우스 오버된 아이콘은 반투명
            status_icon.m_byAlpha = StatusIconManager::ALPHA_VALUE_MOUSE_OVER_ICON;
            mouseover_status_icon_index_ = status_Index;
        }
        status_icon_container_.push_back(status_icon);

        ++status_Index;
        if (status_Index >= StatusIconManager::MAX_HERO_ICON)
        {
            break;
        }
    }
    
    // status_icon_manager에 삽입
    StatusIconManager* status_icon_manager = g_InterfaceManager.GetStatusIconManager();
    if (status_icon_manager)
    {
        status_icon_manager->RegisterStatusIconInfo(status_icon_container_);
    }
}

void  HeroGaugeDialog::SetExpBallCount(int iCount)
{
	m_iExpBallCount = iCount;
	if (m_iExpBallCount >= 10)
	{
		m_iExpBallCount = 9;
	}

	if (m_iExpBallCount < 0)
	{
		m_iExpBallCount = 0;
	}

	for (int a = 0; a < 10; a++)
	{
		CControlWZ *pControl = getControl(HGAUGE_EXPBALL0 + a);
		if (pControl) {
			if (a < m_iExpBallCount)
			{
				pControl->ShowWindowWZ(WZ_SHOW);
			}
			else
			{
				pControl->ShowWindowWZ(WZ_HIDE);

			}
		}
	}
}

void HeroGaugeDialog::MakeHitEffect()
{
	m_iHitEffectDelay = HIT_EFFECT_TICK_PER_FRAME * HIT_EFFECT_FRAME_COUNT;
}

void			
HeroGaugeDialog::LeaveParty()
{
	// 파티 Leave
	if ( (GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE)
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        || (GameFramework::GetCurScene() == SCENE_TYPE_BATTLEGROUND)
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
        || (GameFramework::GetCurScene() == SCENE_TYPE_FREEPVP)
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        || (GameFramework::GetCurScene() == SCENE_TYPE_GOLDRUSH)
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        )
	{
		// 마을 
        uiPartyMan* partyMan =
            static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
        if (partyMan)
        {
            partyMan->LeaveParty();
        }
	}
	else 
	{
		// 기타 베틀
		uiBase *partyLeavePopup = GET_DIALOG( IM_PARTY_MANAGER::PARTY_LEAVE_POPUP );

        if (partyLeavePopup)
        {
            partyLeavePopup->ShowInterface(TRUE);
        }

	}	
}

void HeroGaugeDialog::Process(DWORD dwTick)
{
	if (m_iHitEffectDelay > 0)
	{
		m_iHitEffectDelay -= dwTick;
		if (m_iHitEffectDelay < 0)
		{
			m_iHitEffectDelay = 0;
		}
	}

	CheckNoteMessage( dwTick );
	CheckChatMessage( dwTick );
	CheckPresentMessage( dwTick );
}

//------------------------------------------------------------------------------
int	HeroGaugeDialog::getTooltipString(POSTYPE pos)
{
	map<int,int>::iterator iter;

	iter = m_UseTooltipCtrls.find(pos);
	
	if (iter != m_UseTooltipCtrls.end())
	{
		return iter->second;
	}
	else
	{
		return 0;
	}
}

//------------------------------------------------------------------------------
void HeroGaugeDialog::updateTooltip()
{
	uiToolTipMan * pUIMan = GET_DIALOG_MANAGER( uiToolTipMan, UIMAN_TOOLTIP );

	if( pUIMan == NULL )
		return;

	int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

    // 스킵처리 
    // 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
    BOOL bMouseOver = FALSE;
    for (POSTYPE a = 0; a < HGAUGE_MAX; a++)
    {
        CControlWZ * pControl = getControl(a);

		if(pControl)
		{
			RECT rc = pControl->GetSizeRect();

			if (rc.left <= iMouseX && rc.right >= iMouseX)
			{
				if (rc.top <= iMouseY && rc.bottom >= iMouseY)
				{
					bMouseOver = TRUE;
				}
			}
		}
    }

    if ( !bMouseOver )
    {
		m_iTooltipStringCode = 0;
        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
		return;
    }
	
	if(m_iTooltipStringCode)
	{
		if(5921 == m_iTooltipStringCode)	// 친구메시지 누적정보 툴팁의 경우는 따로 처리됨
		{
			uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
			if(pCommMan)
			{
				if(pCommMan->GetFriendNameForTooltip())
				{
					TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
					TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
					
					g_InterfaceManager.GetInterfaceString(5921, szTemp, INTERFACE_STRING_LENGTH);
					Sprintf(szMessage, szTemp, pCommMan->GetFriendNameForTooltip());

					pUIMan->RegisterTooltipSimpleText(szMessage);
					pUIMan->SetTooltipDialogID(GetDialogKey());

				}
			}
		}
		else if(m_iTooltipStringCode == 5980)
		{
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
            //! 2011.7.22 / i4u4me / 캐시샵 작업 
            if( ConvenienceStore::CashShop* pInstance = ConvenienceStore::CashShop::instance() )
            {
                if( pInstance->HasNewPresent() )
				{
                    pUIMan->RegisterTooltipWithStringCode(m_iTooltipStringCode);
					pUIMan->SetTooltipDialogID(GetDialogKey());
				}
            }

#else
			uiCashItemShopMan *pMan = static_cast<uiCashItemShopMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
			if(pMan && pMan->GetNewPresentRcvCount())
			{
				pUIMan->RegisterTooltipWithStringCode(m_iTooltipStringCode);
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}

#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
		}
		else if( m_iTooltipStringCode == TOOLTIP_STRING_NOTE_CODE )
		{
			if( m_pChkNoteMsg != NULL )
			{
				if( m_pChkNoteMsg->GetState() == WZ_SHOW )
				{
					pUIMan->RegisterTooltipWithStringCode( m_iTooltipStringCode );
					pUIMan->SetTooltipDialogID(GetDialogKey());
				}
			}
		}
		else
		{
			pUIMan->RegisterTooltipWithStringCode(m_iTooltipStringCode);
			pUIMan->SetTooltipDialogID(GetDialogKey());
		}
	}
}
//------------------------------------------------------------------------------
/** 수치랜더링(텍스쳐로)
*/
void HeroGaugeDialog::RenderGaugeNumber()
{
    if (!g_pHero)
    {
        return;
    }

	CCtrlStaticWZ * pCurCtrl=NULL;
	CCtrlStaticWZ * pMaxCtrl=NULL;

	DWORD dwCurVal=0;
	DWORD dwMaxVal=0;
	RECT  rc;
	POINT pt;
	for(int i=0;i<3;++i)
	{
		pCurCtrl=pMaxCtrl=NULL;
		dwMaxVal=dwCurVal=0;
		
		switch(i)
		{
		//========================================
		//	HP
		//========================================
		case 0:
			{
				pCurCtrl = m_pCtrlTextHPInfo[0];
				pMaxCtrl = m_pCtrlTextHPInfo[1];
				dwMaxVal = max(g_pHero->GetMaxHP(),1);
				dwCurVal = g_pHero->GetHP();
			}
			break;
		//========================================
		//	MP
		//========================================
		case 1:
			{
				pCurCtrl = m_pCtrlTextMPInfo[0];
				pMaxCtrl = m_pCtrlTextMPInfo[1];
				dwMaxVal = max(g_pHero->GetMaxMP(),1);
				dwCurVal = g_pHero->GetMP();
			}
			break;
		//========================================
		//	SD
		//========================================
		case 2:
			{
				pCurCtrl = m_pCtrlTextSDInfo[0];
				pMaxCtrl = m_pCtrlTextSDInfo[1];
				dwMaxVal = max(g_pHero->GetMaxSD(),1);
				dwCurVal = g_pHero->GetSD();
			}
			break;

		}

		if(pCurCtrl && pMaxCtrl)
		{
			rc =  pCurCtrl->GetSizeRect();
			//현재수치
			pt =g_InterfaceManager.RenderDrawNumber(rc,dwCurVal,pCurCtrl->GetCtrlStyle());	
			//슬래쉬
			pt = g_InterfaceManager.RenderDrawSign(pt,eIMG_SLASH);										
			rc =  pMaxCtrl->GetSizeRect();
			rc.left=pt.x;
			//맥스수치	
			g_InterfaceManager.RenderDrawNumber(rc,dwMaxVal,pMaxCtrl->GetCtrlStyle());				
		}		
	}
	//========================================
	//	EXP : 현재 cur만찍고있음
	//========================================
	pCurCtrl = m_pCtrlTextEXPInfo[0];	

	if(pCurCtrl)
	{
		float fExpPercent = CalculateExp();
				
		//exp소수점 앞자리 그리기
		rc = pCurCtrl->GetSizeRect();
		pt =g_InterfaceManager.RenderDrawNumber(rc,(int)fExpPercent,pCurCtrl->GetCtrlStyle());
		pt.x-=1;
		pt = g_InterfaceManager.RenderDrawSign(pt,eIMG_DOT);
		rc.left=pt.x;
		rc.top=pt.y;
		int  arFraction[2];
		char szTemp[2]={0,};
		
		float fMinor = (fExpPercent-(int)(fExpPercent))*100.f;
		Sprintf(m_szRenderInfo, _T("%02f"), fMinor);

		//소수점 뒷자리 두개값얻기
		szTemp[0] = m_szRenderInfo[0];

		
		if(fMinor<10.f)
		{
			arFraction[0]=0;
			arFraction[1]=atoi(&szTemp[0]);
		}
		else
		{
			arFraction[0]=atoi(&szTemp[0]);
			arFraction[1]=atoi(&m_szRenderInfo[1]);
		}
 		
		//exp소수점 뒷자리 랜더링(2자리만 그림)
		for(int i=0;i<2;++i)
		{
			pt = g_InterfaceManager.RenderDrawNumber(rc,arFraction[i],DT_LEFT | DT_TOP);
			rc.left=pt.x;
			rc.top=pt.y;
		}
		
		g_InterfaceManager.RenderDrawSign(pt,eIMG_PERCENT);
	}
}

//------------------------------------------------------------------------------
/** 수치랜더링(텍스쳐로)
*/
void HeroGaugeDialog::RenderSuperRoomIcon()
{
	if( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_MISSION ||
		GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHUNTING )
	{

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        RECT rect_guild_pvp = m_rtDialog;
		BASE_ROOMINFO * pRoomInfo = m_pBattleZoneMan->GetBaseRoomInfo();
		if (!pRoomInfo)
			return;

#ifndef __CHINA_PCBANG_SUPER_ROOM_BLOCK
		if (pRoomInfo->IsSpecialType(eROOM_SPECIAL_PCBANG))
		{
            g_InterfaceManager.RenderBattleRoomIcon(
                InterfaceManager::eBATTLE_ICON_PCBANG_SUPERMASTER_ROOM,
                &m_rtDialog);
            rect_guild_pvp.top += 22;
            rect_guild_pvp.bottom += 22;
		}
		else
#endif
		if (pRoomInfo->IsSpecialType(eROOM_SPECIAL_ITEM))
		{
            g_InterfaceManager.RenderBattleRoomIcon(
                    InterfaceManager::eBATTLE_ICON_ITEM_SUPERMASTER_ROOM,
                    &m_rtDialog);
            rect_guild_pvp.top += 22;
            rect_guild_pvp.bottom += 22;
		}

		if (pRoomInfo->IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
		{
            g_InterfaceManager.RenderBattleRoomIcon(
                InterfaceManager::eBATTLE_ICON_GUILD_PVP_ROOM,
                &rect_guild_pvp);
		}
#else //#else __NA_000000_20070528_TAIWAN_GUILD_PVP

        eROOM_SPECIAL_TYPE room_special_type = eROOM_SPECIAL_NORMAL;
#ifdef _DH_BATTLEZONE2_
        uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
        if (ui_battlezone2_manager_)
        {
            room_special_type = 
                static_cast<eROOM_SPECIAL_TYPE>(ui_battlezone2_manager_->current_room_info()->base_party_room_info.special_type_);
        }
#else
        room_special_type = m_pBattleZoneMan->GetSuperMasterType();
#endif //_DH_BATTLEZONE2_
        
#ifndef __CHINA_PCBANG_SUPER_ROOM_BLOCK
		if (room_special_type == eROOM_SPECIAL_PCBANG_SUPER_ROOM)
		{
            g_InterfaceManager.RenderBattleRoomIcon(
                InterfaceManager::eBATTLE_ICON_PCBANG_SUPERMASTER_ROOM,
                &m_rtDialog);
		}
		else
#endif
		if (room_special_type == eROOM_SPECIAL_ITEM1_SUPER_ROOM)
		{
            g_InterfaceManager.RenderBattleRoomIcon(
                InterfaceManager::eBATTLE_ICON_ITEM_SUPERMASTER_ROOM,
                &m_rtDialog);
		}
#endif
	}
}
#pragma warning ( pop )
