#include "SunClientPrecompiledHeader.h"
#include "uiNoticeMan/uiNoticeMan.h"
#include "uiGuildNoticeDialog/uiGuildNoticeDialog.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemData.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "GuildMarkInfoParser.h"
#include "Hero.h"


const DWORD FadeTime = 1500;

//------------------------------------------------------------------------------
WzID uiGuildNoticeDialog::m_wzId[DIALOG_MAX] = 
{
	StrToWzID("ST00"), 
	StrToWzID("PI01"), 
	StrToWzID("BT00"), 
};

//------------------------------------------------------------------------------
/**
*/
uiGuildNoticeDialog::uiGuildNoticeDialog(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{
	memset(m_szNoticeMessage, 0, sizeof(m_szNoticeMessage));
	m_pNoticeTextControl = NULL;
	m_fFadeValue = 1.0f;
	m_bCloseForNextMessage = false;
}

//------------------------------------------------------------------------------
/**
*/
uiGuildNoticeDialog::~uiGuildNoticeDialog()
{
	Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildNoticeDialog::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	m_pNoticeTextControl = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_NOTICE_TEXT]));
	assert(m_pNoticeTextControl);
	m_pNoticeTextControl->ShowWindowWZ(WZ_HIDE);
	m_pGuildMarkPicControl = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_GUILDMARK_PIC]));
	assert(m_pGuildMarkPicControl);
	m_pCloseButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_CLOSE_BTN]));
	assert(m_pCloseButton);

	m_pNoticeMan = (uiNoticeMan*)(GetMan());	
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildNoticeDialog::Release()
{
    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
        intersect_list.clear();
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildNoticeDialog::OnShowWindow( BOOL val )
{

	if (val)
	{
		g_InterfaceManager.AttachFirstDialog(IM_NOTICE_MANAGER::GUILD_NOTICE_DIALOG);

        // UI Inetersect 영역 설정 - 성정한 영역에서만 UI가 동작한다.
        RECT EnableRect = this->m_pCloseButton->GetSizeRect();
        STD_UI_INTERSECT_RECT& rect_list = GetDialogWZ()->GetIntersectRectList();
        rect_list.clear();
        rect_list.push_back(EnableRect);
	}
	else
	{
		// 창닫힘.
		m_Timer.Clear();
		m_bCloseForNextMessage = false;

		g_InterfaceManager.DeAttachFirstDialog(IM_NOTICE_MANAGER::GUILD_NOTICE_DIALOG);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildNoticeDialog::OnRenderSolarDialog()
{
    if (g_pHero == NULL)
    {
        return;
    }

	if (IsVisibleWindow())
	{
		// 텍스트 렌더
		m_FtextRender.SetAlphaModulate(TRUE);
		m_FtextRender.SetAlphaValue((BYTE)(m_fFadeValue*255.0f));
		m_FtextRender.Render();

        HANDLE hGuildMarkTexture = INVALID_HANDLE_VALUE;
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        hGuildMarkTexture = GuildSystem::GetGuildSystemData().GetGuildMarkTexture();
#else
        // 길드마크 세팅
        uiGuildMan* pUIMan = GET_DIALOG_MANAGER(uiGuildMan, UIMAN_GUILD);
        if (pUIMan != NULL)
        {
            hGuildMarkTexture = pUIMan->GetGuildMarkTexture();
        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

        if (hGuildMarkTexture != INVALID_HANDLE_VALUE)
        {
            ENUM_ALPHABLEND eOldAlphaMode =	g_pSunRenderer->GetAlphaBlendMode();
            BASE_GuildMarkInfo* pMarkInfo = NULL;

            RECT rtSize = m_pGuildMarkPicControl->GetSizeRect();

            float fWidth = (float) rtSize.right - rtSize.left;
            float fHeight = (float) rtSize.bottom - rtSize.top;
            g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);
            BYTE ALPHA = (BYTE)(m_fFadeValue*255.0f);			

            WzColor Color[4] = {WzColor_RGBA(255,255,255,ALPHA)
                , WzColor_RGBA(255,255,255,ALPHA)
                , WzColor_RGBA(255,255,255,ALPHA)
                , WzColor_RGBA(255,255,255,ALPHA)};

            for(int i =0; i < 2; ++i)
            {
                if( i == 0)
                    pMarkInfo =  GuildMarkInfoParser::Instance()->GetGuildMarkInfo(g_pHero->GetGuildRenderInfo().m_BackGroundIndex);
                else
                    pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(g_pHero->GetGuildRenderInfo().m_PatternIndex);

                if( pMarkInfo )
                {
                    g_pSunRenderer->RenderTexture(hGuildMarkTexture, (float)rtSize.left , (float)rtSize.top, fWidth , fHeight,
                        (float)pMarkInfo->iRow * 32.0f , (float) pMarkInfo->iColumn * 32.0f, 32.0f, 32.0f, Color);
                }
            }
            g_pSunRenderer->SetAlphaBlendMode(eOldAlphaMode);
		}
	}	
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildNoticeDialog::OnUpdateSolarDialog()
{
	// 페이드 인/아웃 처리
	DWORD ProgressTime = m_Timer.GetProcessTime();
	m_fFadeValue = 1.0f;
	if (ProgressTime <= FadeTime)
	{
		m_fFadeValue = (float)(ProgressTime)/FadeTime;		
	}
	if (ProgressTime >= m_dwNoticeTime - FadeTime)
	{
		m_fFadeValue = 1.0f - (float)(ProgressTime-(m_dwNoticeTime - FadeTime))/FadeTime;
	}
	GetDialogWZ()->SetTransparency(m_fFadeValue);

	// 시간이 지났거나 버튼클릭했을 경우 창닫음 & 다음 메시지 출력
	if (m_Timer.IsExpired() || m_bCloseForNextMessage)
	{
		ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool uiGuildNoticeDialog::ShowNoticeMessage(const TCHAR* pNoticeMessage, int DisplayTime)
{
	// 공지 보여주기	
	ShowInterface(TRUE);
	m_dwNoticeTime = DisplayTime;

	// 시스템창에 메시지 추가
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, pNoticeMessage);

	// 투명도
	GetDialogWZ()->SetTransparency(0.0f);	

	// 정규화 텍스트 세팅
	FTextParser parser;
	FTextSentence Sentence;
	RECT rcCtr = m_pNoticeTextControl->GetSizeRect();

	m_FtextRender.Init(GetDrawBase(), rcCtr);
	m_FtextRender.SetVCenterAlign(true);
	m_FtextRender.ClearSentence();
	parser.SetDefaultFont(m_pNoticeTextControl->GetFontID(), m_pNoticeTextControl->GetTextColor());
	
	std::vector<util::_tstring> vstr;
	GlobalFunc::ProcessWordWrapByFontSize( &vstr, const_cast<TCHAR *>(pNoticeMessage) , m_pNoticeTextControl->GetFontID() , _tcslen(pNoticeMessage), (rcCtr.right - rcCtr.left - 10), false );
    
	BOOL bRet = FALSE;
	size_t count = vstr.size();
	for (size_t i = 0; i < count; ++i)
	{
		bRet = parser.Parser(vstr[i], Sentence);
		if(bRet)
		{
			m_FtextRender.Append(Sentence);
		}
	}	
	m_FtextRender.SetBeginLineIndex(0);

	m_Timer.SetTimer(m_dwNoticeTime);

	return true;
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildNoticeDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			if (m_wzId[DLGPOS_CLOSE_BTN] == pMessage->dwCtrlID)
			{
				m_bCloseForNextMessage = true;
			}
		}
		break;
	default:
		{
		}
	}
}

