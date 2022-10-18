#include "SunClientPrecompiledHeader.h"
#include "GameMessage.h"
#include "Application.h"
#include "uiDominationTowerAreaDlg/uiDominationTowerAreaDlg.h"

GameMessage::GameMessage()
{
}

GameMessage::~GameMessage()
{
}

void GameMessage::Init()
{
    m_bShowMessageBox = FALSE;
    m_bShowCenterMessageBox = FALSE;
    m_byShowMessageBoxAlpha = 100;
    m_byShowCenterMessageBoxAlpha = 100;

    m_bShowMissionAreaMessageBox = FALSE;
    m_byShowMissionAreaMessageBoxAlpha = 100;
    m_bShowSkillResultMessage = FALSE;

    _InitializeMessageBox();
    _InitializeCenterMessageBox();
    _InitializeMissionAreaMessageBox();
    _InitializeSkillResultMessage();
}

void GameMessage::RenderMessage(WzSimpleMesh* pMessageMesh)
{
    _RenderMessageBox(pMessageMesh);
    _RenderCenterMessageBox(pMessageMesh);
    _RenderMissionAreaMessageBox();
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    _RenderMondSchienAreaOwnerMessage();
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

    if( m_bShowSkillResultMessage == TRUE )
    {
        _RenderSkillResultMessage();
    }
}

void GameMessage::Process(DWORD dwTick)
{
    //	메시지 박스
    if (m_bShowMessageBox)
    {
        if (m_iMessageDelay != -1)
        {
            if ((DWORD)m_iMessageDelay > dwTick)
            {
                m_iMessageDelay -= dwTick;
                if( m_iMessageDelay <= 1000 )
                {
                    m_byShowMessageBoxAlpha = (BYTE)( 100.0f * (float)m_iMessageDelay * 0.001f );
                }
            }
            else
            {
                HideMessageBox();
            }
        }
    }

    //	메시지 박스
    if (m_bShowCenterMessageBox)
    {
        if (m_iCenterMessageDelay != -1)
        {
            if ((DWORD)m_iCenterMessageDelay > dwTick)
            {
                m_iCenterMessageDelay  -= dwTick;
                if( m_iCenterMessageDelay <= 1000 )
                {
                    m_byShowCenterMessageBoxAlpha = (BYTE)( 100.0f * (float)m_iCenterMessageDelay  * 0.001f );
                }
            }
            else
            {
                HideCenterMessageBox();
            }
        }
    }


    if (m_bShowMissionAreaMessageBox)
    {
        if (m_iMissionAreaMessageDelay != -1)
        {
            if ((DWORD)m_iMissionAreaMessageDelay > dwTick)
            {
                m_iMissionAreaMessageDelay -= dwTick;
                if( m_iMissionAreaMessageDelay <= 1000 )
                {
                    m_byShowMissionAreaMessageBoxAlpha = (BYTE)( 100.0f * (float)m_iMissionAreaMessageDelay * 0.001f );
                }
            }
            else
            {
                m_bShowMissionAreaMessageBox = FALSE;
            }
        }
    }
}

void GameMessage::AddSkillResultMessage( LPCTSTR szMessage, WzColor MessageColor)
{
    SkillResultText text;
    text.m_Text = szMessage;
    text.m_TextColor = MessageColor;

    m_SkillResultMessage.push_front( text );

    if( m_SkillResultMessage.size() > 15 )
        m_SkillResultMessage.erase( m_SkillResultMessage.begin() + 15 );


    TCHAR path[MAX_PATH];
    GetCurrentDirectory( MAX_PATH, path);
    strcat(path, "\\skill_result_log.txt");
    if( FILE* fp = fopen(path, "a") )
    {
        fprintf(fp, "%s\n", szMessage );
        fclose(fp);
    }

}

void GameMessage::ShowMessageBox( LPCTSTR szMessage,int iDelay, WzColor* pTextColor, WzID FontID)
{
	if ( !szMessage )
	{
		return;
	}

	int iMessageLen = _tcslen(szMessage);
	assert( iMessageLen < MAX_SHOWMESSAGE_LENGTH);
	if (iMessageLen > MAX_SHOWMESSAGE_LENGTH)
	{
		return;
	}

	_InitializeMessageBox();
	const TCHAR seps[] = _T("\\n#");
	
	TCHAR szBuffer[MAX_SHOWMESSAGE_LENGTH] = {0,};
	StrnCopy(szBuffer,szMessage,iMessageLen);
	
	int linecount = 0;
	if(FontID)
    {
		m_FontIdMessage = FontID;
    }

	ZeroMemory(m_szMessage , sizeof(m_szMessage) );
	
	int perLineLetterCount = 0; // 라인당 문자 카운트
	for( int i =0; i < iMessageLen; ++i )
	{
		BOOL FoundSingleLetterToken = (szBuffer[i] == _T('\n') || szBuffer[i] == _T('#') );
		BOOL FoundDoubleLetterToken =(szBuffer[i] == _T('\\') && szBuffer[i+1] == _T('n') );
		if( FoundSingleLetterToken || FoundDoubleLetterToken)
		{
			linecount++;
			perLineLetterCount = 0;
			if( linecount >= MAX_MESSAGE_LINE )
			{
				break;
			}
			if( FoundDoubleLetterToken )
			{
				i++;
			}
		}
		else
		{
			m_szMessage[linecount][perLineLetterCount++] = szBuffer[i];
		}
	}
	linecount++;

	m_iMessageLineCount = linecount;


	m_iMessageDelay = iDelay;
	m_bShowMessageBox = TRUE;
	m_byShowMessageBoxAlpha = 100;
	if( pTextColor == NULL )
	{
		m_TextColor = WzColor_RGBA( 255, 220, 50, 255 );
	}
	else
	{
		m_TextColor = *pTextColor;
	}
}

void GameMessage::ShowCenterMessageBox( LPCTSTR szMessage,int iDelay, WzColor *pTextColor, BOOL bDrawBox )
{
	_InitializeCenterMessageBox();

	if( szMessage == NULL )
		return;

	int iMessageLen = _tcslen(szMessage);
	assert( iMessageLen < MAX_SHOWMESSAGE_LENGTH);
	if (iMessageLen > MAX_SHOWMESSAGE_LENGTH)
	{
		return;
	}

#if	defined _JAPAN || defined _INTERNATIONAL_UI
	m_iCenterMessageLineCount = _ProcessWordWrapByLine( m_szCenterMessage, szMessage, MAX_MESSAGE_LINE );	
#else
	const TCHAR seps[] = _T("\\n#");
	
	TCHAR szBuffer[MAX_SHOWMESSAGE_LENGTH];
	ZeroMemory(szBuffer,MAX_SHOWMESSAGE_LENGTH);
	StrnCopy(szBuffer,szMessage,iMessageLen);

	
	TCHAR *token = _tcstok(szBuffer, seps);
	int linecount = 0;
	while( token != NULL )
	{      
		StrnCopy(m_szCenterMessage[linecount],token,MAX_SHOWMESSAGE_LENGTH);
		token = _tcstok( NULL, seps );
		linecount++;
		assert(linecount < MAX_MESSAGE_LINE);
		if (linecount >= MAX_MESSAGE_LINE)
		{
			break;
		}
	}

	m_iCenterMessageLineCount = linecount;
#endif

	m_iCenterMessageDelay = iDelay;
	m_bShowCenterMessageBox = TRUE;
	m_byShowCenterMessageBoxAlpha = 100;
	m_bDrawCenterMessageBox = bDrawBox;
	if( pTextColor == NULL )
	{
		m_CenterTextColor = WzColor_RGBA( 255, 220, 50, 255 );
	}
	else
	{
		m_CenterTextColor = *pTextColor;
	}
}

void GameMessage::ShowMissionAreaInfo( LPCTSTR szMessage,int iDelay, WzColor* pTextColor)
{
	_InitializeMissionAreaMessageBox();

#ifdef	_JAPAN
	_ProcessWordWrapByLine( m_szMissionAreaMessage, szMessage, MAX_MISSION_AREA_LINE );	
#else

	const TCHAR seps[] = _T("#");

	assert(_tcslen(szMessage) < MAX_MESSAGE_LENGTH);

	TCHAR szBuffer[MAX_MESSAGE_LENGTH];

	ZeroMemory(szBuffer,MAX_MESSAGE_LENGTH);

	StrnCopy(szBuffer,szMessage,MAX_MESSAGE_LENGTH);
	
	TCHAR *token = _tcstok(szBuffer, seps);
	int linecount = 0;
	while( token != NULL )
	{      
		assert(linecount < MAX_MISSION_AREA_LINE);
		if (linecount >= MAX_MISSION_AREA_LINE)
		{
			break;
		}

		StrnCopy(m_szMissionAreaMessage[linecount],token,MAX_MESSAGE_LENGTH);
		token = _tcstok( NULL, seps );
		linecount++;
	}
#endif

	m_iMissionAreaMessageDelay = iDelay;
	m_bShowMissionAreaMessageBox = TRUE;
	m_byShowMissionAreaMessageBoxAlpha = 100;

	if( pTextColor == NULL )
	{
		m_MissionTextColor = WzColor_RGBA( 255, 220, 50, 255 );
	}
	else
	{
		m_MissionTextColor = *pTextColor;
	}
}


int GameMessage::_ProcessWordWrapByLine( TCHAR (*szMessage)[MAX_MESSAGE_LENGTH], LPCTSTR szText, int MaxLine )
{
	if ((szText == NULL) ||
        (szText[0] == _T('\0')))
    {
		return 0;
    }

    TCHAR szTemp[MAX_MESSAGE_LENGTH];
    ZeroMemory( &szTemp, MAX_MESSAGE_LENGTH );

    int step1 = 0;  // szText Inc
    int step2 = 0;  // szTemp Inc
	int line = 0;

	while( 1 )
	{
		if( ( szText[step1] == '\n') || ( szText[step1] == '#' ) )
		{
			// lammy : 처음부터 개행이라면 패스
			if( step2 == 0 )
			{
				step1++;
				continue;
			}

			_tcsncpy( szMessage[line], szTemp, MAX_MESSAGE_LENGTH );
			ZeroMemory(&szTemp, MAX_MESSAGE_LENGTH);
			step1 += 1;
			step2 = 0;
			line++;
			continue;
		}

		if( !_ismbblead( szText[step1] ) )
		{
			if( (szText[step1] == '\\') )
			{
				if (szText[step1 + 1] == 'n')
				{
					_tcsncpy( szMessage[line], szTemp, MAX_MESSAGE_LENGTH );
					ZeroMemory( &szTemp, MAX_MESSAGE_LENGTH );
					step1 += 2;
					step2 = 0;
					line++;
					continue;
				}
			}
		}

        if( _ismbblead(szText[step1]) )
        {
            szTemp[step2] = szText[step1];
            step1++;
            step2++;
            szTemp[step2] = szText[step1];
            step1++;
            step2++;
        }
        else
        {
			// lammy : 젤 처음이 공백이라면 넣지 말자.
			if( step2 == 0 )				
			{
				if( szText[step1] == ' ' )
				{
					step1++;
					continue;
				}
			}

            szTemp[step2] = szText[step1];
            step1++;
            step2++;
        }

		if( line >= MaxLine || szText[step1] == '\0' )
		{
			_tcsncpy( szMessage[line], szTemp, MAX_MESSAGE_LENGTH );
			line++;
			break;
		}
	}

	return line;
}

void GameMessage::HideMessageBox()
{
	m_bShowMessageBox = FALSE;
	m_iMessageDelay = 0;
}

void GameMessage::HideCenterMessageBox()
{
	m_bShowCenterMessageBox = FALSE;
	m_bDrawCenterMessageBox = TRUE;
	m_iCenterMessageDelay = 0;
}


void GameMessage::_RenderMessageBox(WzSimpleMesh* pMessageMesh)
{
	if (GENERALPARAM->GetScreenShotMode())
	{
		return;
	}

	WzColor		backcolor = WzColor_RGBA( 0, 0, 0, m_byShowMessageBoxAlpha );
	BYTE		Alpha = BYTE( 255.0f * (float)m_byShowMessageBoxAlpha * 0.01f );
	WzColor		color = ( WzColor) ( ( (m_TextColor) & 0x00FFFFFF ) | ( (Alpha & 0xff) << 24 ) );

	if( m_iMessageLineCount <= 0 )
		return; 

	if( m_bShowMessageBox )
	{
		g_pSunRenderer->Set2DMode( TRUE );

		if(m_FontIdMessage)
			g_pSunRenderer->x_pManagerTextOut->StartToUseFont(m_FontIdMessage );
		else
			g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mi17") );

		RECT rc;
		SIZE sz;

		int maxxsize  = 0;
		int ysize = 0;

		for (int a = 0; a < m_iMessageLineCount; a++)
		{

			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( m_szMessage[a], lstrlen(m_szMessage[a]), &sz );
			maxxsize = max(maxxsize,sz.cx);
			ysize = max( ysize ,sz.cy+1);
		}


		// 한줄 사이즈 설정
		POINT pt = g_pApplication->Get2DModeMapping();
		rc.left = (pt.x/2) - (maxxsize / 2);
		// 점령지역길드창 (uiDominationTowerAreaDlg) 와 상하 위치 바꿈
		rc.top = 226 - (ysize / 2);
		rc.right = rc.left + maxxsize;
		rc.bottom = rc.top + ysize;
	
		BYTE alpha=Alpha_WzColor(color);

		for (int a = 0; a < m_iMessageLineCount; a++)
		{
			g_pSunRenderer->x_pManagerTextOut->DrawText(m_szMessage[a],
				&rc,
				color,
				WzColor_RGBA(0,0,0,0),
				TP_HLEFT,
                ETS_OUTLINE,
				WzColor_RGBA(0,0,0,alpha)
				);

			rc.top += ysize;
			rc.bottom += ysize;

		}
		
		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
		g_pSunRenderer->Set2DMode( FALSE );
	}
}

void GameMessage::_RenderCenterMessageBox(WzSimpleMesh* pMessageMesh)
{
	if (GENERALPARAM->GetScreenShotMode())
	{
		return;
	}

	
	ENUM_CMP_FUNC		eZFunc;
	ENUM_ALPHABLEND		eAlphablendMode;
	
	BYTE		Alpha = BYTE( 255.0f * (float)m_byShowCenterMessageBoxAlpha * 0.01f );
	WzColor		backcolor = WzColor_RGBA( 0, 0, 0, Alpha );
	WzColor		color = ( WzColor) ( ( (m_CenterTextColor) & 0x00FFFFFF ) | ( (Alpha & 0xff) << 24 ) );
	WzColor		oldcolor = g_pSunRenderer->GetDiffuseColor();
								  
	if (m_iCenterMessageLineCount <= 0) return; 


	if (m_bShowCenterMessageBox)
	{
		assert(pMessageMesh);

		g_pSunRenderer->Set2DMode( TRUE );
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mi17") );

		RECT rc,rc2;
		SIZE sz;

	
		int maxxsize  = 0;
		int ysize = 0;

		for (int a = 0; a < m_iCenterMessageLineCount; a++)
		{
			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( m_szCenterMessage[a], lstrlen(m_szCenterMessage[a]), &sz );
			maxxsize = max(maxxsize,sz.cx);
			ysize = sz.cy;
		}

		// 한줄 사이즈 설정
		POINT pt = g_pApplication->Get2DModeMapping();
		rc.left = (pt.x/2) - (maxxsize / 2);
		rc.top = (pt.y/2-14) - (ysize / 2);
		rc.right = rc.left + maxxsize;
		rc.bottom = rc.top + ysize;			

		ENUM_CULL_FACE      eCullFaceMode;
		eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
		eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
		g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
		g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
		eZFunc = g_pSunRenderer->GetZFunc();
		BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
		BOOL bZTest = g_pSunRenderer->IsEnableZTest();

		g_pSunRenderer->SetZBufferWrite( FALSE );	
		g_pSunRenderer->SetZBufferTest(FALSE);


		if ( m_bDrawCenterMessageBox == TRUE )
		{
			// 밑바탕 그리기

			rc2.left = rc.left - 15;
			rc2.right = rc.right + 15;
			rc2.top = rc.top - 10;
			rc2.bottom = rc.top + ysize * m_iCenterMessageLineCount + 10;

			//괴이한 좌표계(Y가 Z로 바뀌고 범위가 0~ -(해상도))에따라 Mesh를 만든다 -.-

			pMessageMesh->m_pVertices[0].x = (float)rc2.left;
			pMessageMesh->m_pVertices[0].y = 0.f;
			pMessageMesh->m_pVertices[0].z = (float)-rc2.top;

			pMessageMesh->m_pVertices[2].x = (float)rc2.right;
			pMessageMesh->m_pVertices[2].y = 0.f;
			pMessageMesh->m_pVertices[2].z = (float)-rc2.top;

			pMessageMesh->m_pVertices[1].x = (float)rc2.left;
			pMessageMesh->m_pVertices[1].y = 0.f;
			pMessageMesh->m_pVertices[1].z = (float)-rc2.bottom;

			pMessageMesh->m_pVertices[3].x = (float)rc2.right;
			pMessageMesh->m_pVertices[3].y = 0.f;
			pMessageMesh->m_pVertices[3].z = (float)-rc2.top;

			pMessageMesh->m_pVertices[4].x = (float)rc2.left;
			pMessageMesh->m_pVertices[4].y = 0.f;
			pMessageMesh->m_pVertices[4].z = (float)-rc2.bottom;

			pMessageMesh->m_pVertices[5].x = (float)rc2.right;
			pMessageMesh->m_pVertices[5].y = 0.f;
			pMessageMesh->m_pVertices[5].z = (float)-rc2.bottom;

			g_pSunRenderer->SetColor(backcolor);
			g_pSunRenderer->RealizeRenderSimpleMesh(pMessageMesh);
			g_pSunRenderer->SetColor(oldcolor);
		}			

		for (int a = 0; a < m_iCenterMessageLineCount; a++)
		{
			g_pSunRenderer->x_pManagerTextOut->DrawText(this->m_szCenterMessage[a],
				&rc,
				color,
				WzColor_RGBA(0,0,0,0),
				TP_HLEFT,
				ETS_OUTLINE,
				backcolor
				);

			rc.top += ysize;
			rc.bottom += ysize;
		}

		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
		g_pSunRenderer->Set2DMode( FALSE );

		g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
		g_pSunRenderer->SetZFunc(eZFunc);
		g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
		g_pSunRenderer->SetZBufferTest( bZTest );
		g_pSunRenderer->SetZBufferWrite( bZWrite );		

	}
}

void GameMessage::_RenderMissionAreaMessageBox()
{
	
	ENUM_CMP_FUNC		eZFunc;
	ENUM_ALPHABLEND		eAlphablendMode;
	ENUM_CULL_FACE      eCullFaceMode;

	//렌더러가 여기서 없다면 출력도 되면 안되는대.. 멀티로딩 문제가 있을듯?
	if (g_pSunRenderer == NULL )
	{
		return;
	}
	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
	
	WzColor		backcolor = WzColor_RGBA( 0, 0, 0, m_byShowMissionAreaMessageBoxAlpha );
	BYTE		Alpha = BYTE( 255.0f * (float)m_byShowMissionAreaMessageBoxAlpha * 0.01f );
	WzColor		color = ( WzColor) ( ( (m_MissionTextColor) & 0x00FFFFFF ) | ( (Alpha & 0xff) << 24 ) );
	WzColor		oldcolor = g_pSunRenderer->GetDiffuseColor();

	uiDominationTowerAreaDlg* pDlg = GET_CAST_DIALOG(uiDominationTowerAreaDlg, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_AREA_DIALOG);

	//안전장치 하나더?
	if (m_bShowMissionAreaMessageBox && g_pSunRenderer)
	{
		g_pSunRenderer->Set2DMode( TRUE );

		eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
		g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
		g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
		eZFunc = g_pSunRenderer->GetZFunc();
		BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
		BOOL bZTest = g_pSunRenderer->IsEnableZTest();
	
	
		g_pSunRenderer->SetZBufferWrite( FALSE );	
		g_pSunRenderer->SetZBufferTest(FALSE);
	
		

		g_pSunRenderer->SetColor(backcolor);
		
		

		RECT rc;
		SIZE sz;
		POINT	pt = g_pApplication->Get2DModeMapping();
	
		//rc.top = 100;
		rc.top = 60;
		

		for (int a = 0; a < MAX_MISSION_AREA_LINE; a++) {

			if ( a == 0)
			{
				g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("ti35") );
				color = WzColor_RGBA(235,192,38,255);
			}
			else
			{
				g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("sn27")  );
				
				color = WzColor_RGBA(152,206,142,255);
			}

			TCHAR	szMBString[INTERFACE_STRING_LENGTH];
			StrCopy( szMBString, m_szMissionAreaMessage[a] );

			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( szMBString, lstrlen(m_szMissionAreaMessage[a]), &sz );
#ifdef _RUSSIA
            // 지역명이 짤리는 문제로 인하여 임의적으로 영역을 두배로 늘린다.
            // 밑에 부분에서 중앙으로 보정해 주기 때문에 두배의 영역으로 늘려도 상관없다.
            // GetTextExtentPoint함수에서 제대로 사이즈를 찾아주지 못하는 것 같음.
            sz.cx *= 2;
#endif // _RUSSIA
			rc.left = pt.x /2 - (sz.cx / 2);			
			rc.right = rc.left + sz.cx;
			rc.bottom  = rc.top + sz.cy;

			g_pSunRenderer->x_pManagerTextOut->DrawText(szMBString,
				&rc,
				color,
				WzColor_RGBA(0,0,0,0),
				TP_HCENTER,
				ETS_SHADOW
				);

			g_pSunRenderer->x_pManagerTextOut->EndUseFont();


			rc.top += sz.cy;
		}
		if (pDlg)
		{
			if (FALSE == pDlg->IsVisible() && pDlg->CheckShowDialog())
			{
				pDlg->ShowInterface(TRUE);
			}			
			pDlg->SetTransparency(Alpha);						
		}

		g_pSunRenderer->Set2DMode( FALSE );
		g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
		g_pSunRenderer->SetZFunc(eZFunc);
		g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
		g_pSunRenderer->SetZBufferTest( bZTest );
		g_pSunRenderer->SetZBufferWrite( bZWrite);
		g_pSunRenderer->SetColor(oldcolor);

	}
	else
	{
		if (pDlg && pDlg->IsVisible())
		{
			pDlg->ShowInterface(FALSE);
		}
	}
}

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
void GameMessage::_RenderMondSchienAreaOwnerMessage()
{
    uiDominationTowerMan* domination_manager_ptr = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (NULL == domination_manager_ptr)
        return;

    ENUM_CMP_FUNC		eZFunc;
    ENUM_ALPHABLEND		eAlphablendMode;
    ENUM_CULL_FACE      eCullFaceMode;

    //렌더러가 여기서 없다면 출력도 되면 안되는대.. 멀티로딩 문제가 있을듯?
    if (g_pSunRenderer == NULL )
    {
        return;
    }
    eCullFaceMode = g_pSunRenderer->GetCullFaceMode();

    WzColor		backcolor = WzColor_RGBA( 0, 0, 0, m_byShowMissionAreaMessageBoxAlpha );
    BYTE		Alpha = BYTE( 255.0f * (float)m_byShowMissionAreaMessageBoxAlpha * 0.01f );
    WzColor		color = ( WzColor) ( ( (m_MissionTextColor) & 0x00FFFFFF ) | ( (Alpha & 0xff) << 24 ) );
    WzColor		oldcolor = g_pSunRenderer->GetDiffuseColor();
    
    if (m_bShowMissionAreaMessageBox && g_pSunRenderer)
    {
        g_pSunRenderer->Set2DMode( TRUE );

        eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
        g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
        g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
        eZFunc = g_pSunRenderer->GetZFunc();
        BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
        BOOL bZTest = g_pSunRenderer->IsEnableZTest();

        g_pSunRenderer->SetZBufferWrite( FALSE );
        g_pSunRenderer->SetZBufferTest(FALSE);

        g_pSunRenderer->SetColor(backcolor);

        RECT rc;
        SIZE sz;
        POINT	pt = g_pApplication->Get2DModeMapping();
        
        rc.top = 25;
        
        g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("sn27")  );
        color = WzColor_RGBA(235,192,38,255);
        
        TCHAR total_message[INTERFACE_STRING_LENGTH] = {0, };
        ZeroMemory(total_message, INTERFACE_STRING_LENGTH);
        TCHAR*   mondschien_text = domination_manager_ptr->get_mondschien_guild_name();
        
        if (_tcscmp(mondschien_text, _T("")) != 0)
        {
            // 3498 대영주 %s 길드의 영토
            TCHAR string_format[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(3498, string_format, INTERFACE_STRING_LENGTH);
            Snprintf(total_message, INTERFACE_STRING_LENGTH, string_format, mondschien_text);
        }
        
        g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( total_message, lstrlen(total_message), &sz );

        rc.left = pt.x /2 - (sz.cx / 2);
        rc.right = rc.left + sz.cx;
        rc.bottom  = rc.top + sz.cy;

        g_pSunRenderer->x_pManagerTextOut->DrawText(total_message,
            &rc,
            color,
            WzColor_RGBA(0,0,0,0),
            TP_HCENTER,
            ETS_SHADOW
            );

        g_pSunRenderer->x_pManagerTextOut->EndUseFont();

        rc.top += sz.cy;

        g_pSunRenderer->Set2DMode( FALSE );
        g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
        g_pSunRenderer->SetZFunc(eZFunc);
        g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
        g_pSunRenderer->SetZBufferTest( bZTest );
        g_pSunRenderer->SetZBufferWrite( bZWrite);
        g_pSunRenderer->SetColor(oldcolor);
    }
}
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

void GameMessage::ToggleSkillResultMessage()
{
    m_bShowSkillResultMessage = !m_bShowSkillResultMessage;
}

void GameMessage::_RenderSkillResultMessage()
{
#ifndef _DEV_VER
    return;
#endif

    if (GENERALPARAM->GetScreenShotMode())
    {
        return;
    }

    g_pSunRenderer->Set2DMode( TRUE );
    g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID( "SMAL") );

    RECT rc;
    int max_width = 300;
    int line_height = 0;

    SIZE sz;
    for( size_t i=0;i!=m_SkillResultMessage.size();++i)
    {
        m_SkillResultMessage[i].m_Text.c_str();

        g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( m_SkillResultMessage[i].m_Text.c_str(), m_SkillResultMessage[i].m_Text.size(), &sz );
        max_width = max(max_width,sz.cx);
        line_height = max( line_height ,sz.cy+1);
    }

    // 한줄 사이즈 설정
    POINT pt = g_pApplication->Get2DModeMapping();
    rc.left = pt.x /2;
    rc.top = (line_height * 15) + 150;
    rc.right = rc.left + max_width;
    rc.bottom = rc.top + line_height;

    for( size_t i=0;i!=m_SkillResultMessage.size();++i)
    {
        g_pSunRenderer->x_pManagerTextOut->DrawText(
            m_SkillResultMessage[i].m_Text.c_str(),
            &rc,
            m_SkillResultMessage[i].m_TextColor,
            WzColor_RGBA(0,0,0,0),
            TP_HLEFT,
            ETS_OUTLINE,
            WzColor_RGBA(0,0,0,255)
            );

        rc.top -= line_height;
        rc.bottom -= line_height;

    }

    g_pSunRenderer->x_pManagerTextOut->EndUseFont();
    g_pSunRenderer->Set2DMode( FALSE );
}


void GameMessage::_InitializeMessageBox()
{
	m_iMessageLineCount = 0;
	for (int a = 0; a< MAX_MESSAGE_LINE; a++)
	{
		ZeroMemory(m_szMessage[a],MAX_SHOWMESSAGE_LENGTH * sizeof(TCHAR));
	}

	m_FontIdMessage=0;
}

void GameMessage::_InitializeCenterMessageBox()
{
	m_iCenterMessageLineCount = 0;
	for (int a = 0; a< MAX_MESSAGE_LINE; a++)
	{
		ZeroMemory(m_szCenterMessage[a],MAX_SHOWMESSAGE_LENGTH * sizeof(TCHAR));

	}
}

void GameMessage::_InitializeMissionAreaMessageBox()
{
	for (int a = 0; a< MAX_MISSION_AREA_LINE; a++)
	{
		ZeroMemory(m_szMissionAreaMessage[a],MAX_MESSAGE_LENGTH * sizeof(TCHAR));
	}
}

void GameMessage::_InitializeSkillResultMessage()
{
#ifdef _DEV_VER

    TCHAR path[MAX_PATH];
    GetCurrentDirectory( MAX_PATH, path);
    strcat(path, "\\skill_result_log.txt");
    if( FILE* fp = fopen(path, "w") )
    {
        fclose(fp);
    }

    m_SkillResultMessage.clear();

#endif
}

