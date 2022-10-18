#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiDominationStamp/uiDominationStamp.h"

#include "GlobalFunc.h"
#include "Hero.h"
#include "ItemManager.h"
#include "DominationInfoParser.h"
#include "CollectionParser.h"
#include "Map.h"
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
#include "Application.h"
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiDominationStamp::m_wzID[eDIALOG_DOMINATION_STAMP_MAX] = 
{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    StrToWzID("S000"),
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	StrToWzID("PI00"),//eDIALOG_DOMINATION_STAMP_TEXTURE,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiDominationStamp::uiDominationStamp(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
	m_pctrlPicture    = NULL;
	m_hStampTex       = INVALID_HANDLE_VALUE;
	m_wiStampID       = INVALID_WZID;
	m_dwTick		  = 0;
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    string_control_ptr_ = NULL;
    string_tick_      = 0;
    string_rate_alpha_color_ = 0;
    alpha_color_state_ = eCOLOR_STATE_DEC;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiDominationStamp::~uiDominationStamp()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationStamp::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationStamp::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationStamp::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationStamp::Process(DWORD dwTick)
{
	if(FALSE == m_bShow)
		return;

	m_dwTick += dwTick;
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    ProcessStampGuildString(dwTick);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	const CollectionInfo* pCI = CollectionParser::Instance()->FindCollectionInfo(m_wiStampID);
	if(pCI)
	{
        if (m_dwTick > pCI->m_dwCollectingTime + 2000) // 시간이 완료시간+3초가 지났으면 닫는다.
        {
            m_fRate = 0.f;
            ShowInterface(FALSE);
        }
		else if(m_dwTick < pCI->m_dwCollectingTime)
        {
			m_fRate = ((float)m_dwTick/(float)pCI->m_dwCollectingTime) * 100.f;
        }
		else
        {
			m_fRate = 100.f;
        }
	}
}

void uiDominationStamp::OnRenderSolarDialog()
{
	if(INVALID_HANDLE_VALUE == m_hStampTex)
		return;
	if(NULL == m_pctrlPicture)
		return;

	{
		g_pSunRenderer->RenderTexture4SpinGage(m_hStampTex, (float)m_pctrlPicture->GetSizeRect().left,
															(float)m_pctrlPicture->GetSizeRect().top, 
															(float)(m_pctrlPicture->GetSizeRect().right - m_pctrlPicture->GetSizeRect().left),
															(float)(m_pctrlPicture->GetSizeRect().bottom - m_pctrlPicture->GetSizeRect().top),
															m_sRect.fTopX,
															m_sRect.fTopY,
															m_sRect.fWidth,
															m_sRect.fHeight,
															m_fRate);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationStamp::OnShowWindow(BOOL val)
{
	if( val )
	{
		this->MoveDefaultWindowPos();

		RECT rc;
		GetDialogWZ()->GetClientRect(&rc);
		DisableInterSectRect(rc);

		m_dwTick  = 0;
		m_fRate   = 0.f;
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        string_rate_alpha_color_ = 0;
        alpha_color_state_ = eCOLOR_STATE_DEC;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

		m_bShow = TRUE;
	}
	else
	{
		m_bShow   = FALSE;
		m_dwTick  = 0;
		m_fRate   = 0.f;
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        string_rate_alpha_color_ = 0;
        alpha_color_state_ = eCOLOR_STATE_DEC;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationStamp::InitControl()
{
	m_pctrlPicture = (CCtrlPictureWZ*)getControl(eDIALOG_DOMINATION_STAMP_TEXTURE);
	assert(m_pctrlPicture);
	if(m_pctrlPicture)
	{
		m_pctrlPicture->ShowWindowWZ(WZ_HIDE);
		m_hStampTex = (HANDLE)m_pctrlPicture->GetCtrlImageFormat()->m_sImageState[E_BASE].m_pVoid;
		m_sRect		= m_pctrlPicture->GetCtrlImageFormat()->m_sImageState[E_BASE].m_sFRect;
	}

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    string_control_ptr_ = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_DOMINATION_STAMP_STRING));
    assert(string_control_ptr_);
    if (string_control_ptr_)
    {
        string_tick_ = 0;
        string_rate_alpha_color_ = 0;
        alpha_color_state_ = eCOLOR_STATE_DEC;

        string_control_ptr_->ShowWindowWZ(WZ_SHOW);
    }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

}

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
void uiDominationStamp::set_stamp_guild_name( TCHAR* stamp_guild_name )
{
    if (!stamp_guild_name)
        return;

    ZeroMemory(stamp_guild_name_, MAX_GUILDNAME_LENGTH+1);
    _tcsncpy(stamp_guild_name_, stamp_guild_name, MAX_GUILDNAME_LENGTH);
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
    g_InterfaceManager.GetInterfaceString(3503, temp_string);
    Snprintf(result_string, INTERFACE_STRING_LENGTH, temp_string, stamp_guild_name_);
    if (string_control_ptr_)
    {
        string_control_ptr_->SetTextWZ("");
    }
}

void uiDominationStamp::ProcessStampGuildString( DWORD tick )
{
    string_tick_ += tick;

    if (string_tick_ > 100)
    {
        string_tick_ = 0;

        if (alpha_color_state_ == eCOLOR_STATE_INC)
        {
            string_rate_alpha_color_ += 30;

            if (string_rate_alpha_color_ >= 255)
            {
                string_rate_alpha_color_ = 255;

                alpha_color_state_ = eCOLOR_STATE_DEC;
            }
        }
        if (alpha_color_state_ == eCOLOR_STATE_DEC)
        {
            string_rate_alpha_color_ -= 30;

            if (string_rate_alpha_color_ <= 0)
            {
                string_rate_alpha_color_ = 0;

                alpha_color_state_ = eCOLOR_STATE_INC;
            }
        }

        if (string_control_ptr_)
        {
            WzColor color = string_control_ptr_->GetTextColor();
            color = WzColor_ChangeAlpha(color, string_rate_alpha_color_);
            string_control_ptr_->SetTextColor(color);
        }
    }
}

void uiDominationStamp::RenderStampGuildText()
{
    if (IsVisible() == FALSE)
        return;

    ENUM_CMP_FUNC eZFunc;
    ENUM_ALPHABLEND eAlphablendMode;
    ENUM_CULL_FACE eCullFaceMode;

    // 렌더러가 없으면 return;
    if (g_pSunRenderer == NULL)
    {
        return;
    }
    eCullFaceMode = g_pSunRenderer->GetCullFaceMode();

    WzColor guild_name_color = WzColor_RGBA(255, 0, 0, 255);
    WzColor backcolor = WzColor_RGBA(0, 0, 0, string_rate_alpha_color_);
    WzColor guild_string_color = WzColor_RGBA(255, 255, 0, 255);
    WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();    

    g_pSunRenderer->Set2DMode( TRUE );

    eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
    g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
    g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
    eZFunc = g_pSunRenderer->GetZFunc();
    BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
    BOOL bZTest = g_pSunRenderer->IsEnableZTest();

    g_pSunRenderer->SetZBufferWrite( FALSE );
    g_pSunRenderer->SetZBufferTest( FALSE );

    g_pSunRenderer->SetColor(backcolor);

    RECT rc;
    SIZE sz;
    POINT pt = g_pApplication->Get2DModeMapping();

    if (string_control_ptr_)
    {        
        RECT control_rect = string_control_ptr_->GetSizeRect();        
        rc.top = control_rect.top;
        rc.top -= 28; // 위로 조금 올려준다.        
    }
    else
    {
        // 기본
        rc.top = 25;
    }    

    g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("sn27") );
    guild_name_color = WzColor_RGBA(255, 0, 0, 255);
    guild_name_color = WzColor_ChangeAlpha(guild_name_color, string_rate_alpha_color_);
    WzColor format_color = WzColor_RGBA(255, 255, 0, 255);
    format_color = WzColor_ChangeAlpha(format_color, string_rate_alpha_color_);
    guild_string_color = WzColor_ChangeAlpha(guild_string_color, string_rate_alpha_color_);
    WzColor out_line_color = WzColor_RGBA(0, 0, 0, 255);
    out_line_color = WzColor_ChangeAlpha(out_line_color, string_rate_alpha_color_);

    // %s길드가 각인 중 입니다.
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
    g_InterfaceManager.GetInterfaceString(3503, temp_string);    
    TCHAR guild_format_string[INTERFACE_STRING_LENGTH] = {0, };
    ZeroMemory(guild_format_string, INTERFACE_STRING_LENGTH);
    TCHAR guild_string[INTERFACE_STRING_LENGTH] = {0, };

    TCHAR* token;
    token = strtok(temp_string ,_T("\\n"));
    if (token)
    {
        _tcsncpy(guild_format_string, temp_string, _tcslen(temp_string));
        Snprintf(guild_format_string, INTERFACE_STRING_LENGTH, temp_string, stamp_guild_name_);

        token = strtok(NULL,_T("\\n"));
        if (token != NULL)
        {
            _tcsncpy(result_string, token, _tcslen(token));
        }
    }

    g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( guild_format_string, _tcslen(guild_format_string), &sz);
    SIZE guild_name_size;
    g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( stamp_guild_name_, _tcslen(stamp_guild_name_), &guild_name_size);

    rc.left = pt.x /2 - (sz.cx / 2);
    rc.right = rc.left + guild_name_size.cx;
    rc.bottom = rc.top + sz.cy;

    g_pSunRenderer->x_pManagerTextOut->DrawText(stamp_guild_name_,
        &rc,
        guild_name_color,
        WzColor_RGBA(0,0,0,0),
        TP_HRIGHT,
        ETS_OUTLINE,
        out_line_color
        );

    Sprintf(guild_string, temp_string, _T(""));
    rc.left += guild_name_size.cx;
    rc.right = rc.left + sz.cx;
    g_pSunRenderer->x_pManagerTextOut->DrawText(guild_string,
        &rc,
        guild_string_color,
        WzColor_RGBA(0,0,0,0),
        TP_HLEFT,
        ETS_OUTLINE,
        out_line_color
        );    
    
    rc.top += sz.cy;    
    g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( result_string, _tcslen(result_string), &sz);    
    rc.left = pt.x /2 - (sz.cx / 2);    
    rc.right = rc.left + sz.cx;
    rc.bottom = rc.top + sz.cy;    

    g_pSunRenderer->x_pManagerTextOut->DrawText(result_string,
        &rc,
        format_color,
        WzColor_RGBA(0,0,0,0),
        TP_HCENTER,
        ETS_OUTLINE,
        out_line_color
        );

    g_pSunRenderer->x_pManagerTextOut->EndUseFont();    

    g_pSunRenderer->Set2DMode( FALSE );
    g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
    g_pSunRenderer->SetZFunc(eZFunc);
    g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
    g_pSunRenderer->SetZBufferTest( bZTest );
    g_pSunRenderer->SetZBufferWrite( bZWrite );
    g_pSunRenderer->SetColor(oldcolor);
}
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

