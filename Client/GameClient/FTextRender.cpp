#include "SunClientPrecompiledHeader.h"
#include "ftextrender.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "Application.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ItemInfoParser.h"
#include "NPCInfoParser.h"
#include "GraphicResourceList.h"

//-------------------------------------------------------------------------------------------
/**
*/
FTextRender::FTextRender(void) : m_pRender(NULL), 
m_iStartLineIndex(0),
m_iMarginTop(DEFAULT_MARGIN_TOP),
m_iMarginLeft(DEFAULT_MARGIN_LEFT),
m_iMarginBottom(DEFAULT_MARGIN_BOTTOM),
m_iMarginRight(DEFAULT_MARGIN_RIGHT),
m_iCharGap(DEFAULT_CHARACTER_GAP),	
m_iLineGap(DEFAULT_LINE_GAP),
m_iMaxLine(0),
m_bReadyForRender(FALSE),
m_bCanPageUp(FALSE),
m_bVCenterAlign(false),
m_bHCenterAlign(false),
m_ExtraMarginTop(0)
{
    ZeroMemory(&m_rcRender, sizeof(m_rcRender));

    m_bAlphaModulate = FALSE;
    m_byAlpha = 0;
}

//-------------------------------------------------------------------------------------------
/**
*/
FTextRender::~FTextRender(void)
{
    Release();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
FTextRender::Init( CDrawBase * pRender, RECT & rRcRender )
{
    m_bReadyForRender = FALSE;

    m_pRender = pRender;

    SetRect( rRcRender );
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
FTextRender::Release()
{
    ClearSentence();

    m_iMaxLine = 0;
    m_iStartLineIndex = 0;

    m_pRender = NULL;
}

//-------------------------------------------------------------------------------------------
/**
*/
int						
FTextRender::GetSentenceCount() const
{
    return m_vecSentences.size();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
FTextRender::Append(FTextSentence & rSentence)
{
    FTextSentence * pNewSentence = new FTextSentence(rSentence);

    m_vecSentences.push_back( pNewSentence );

    m_bReadyForRender = FALSE;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
FTextRender::ClearSentence()
{
    int iSize = m_vecSentences.size();

    for ( int i = 0; i < iSize; ++i )
    {
        SAFE_RELEASENDELETE( (FTextSentence *)m_vecSentences[i] );
    }

    m_iStartLineIndex = 0;
    m_vecSentences.clear();

    clearLineInfos();
}

//-------------------------------------------------------------------------------------------
/**
*/
const FTextSentence &	
FTextRender::getSentence( int iIndex )
{
    return *m_vecSentences[iIndex];
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL					
FTextRender::calcCurMaxLineHeight(
                                  int iCurX,
                                  int iSentence, 
                                  int iParam, 
                                  int & OUT riCurLineWidth,
                                  int & OUT riCurLineMaxHeight)
{
    int iCurLine = m_iStartLineIndex;
    int iWidth	= 0;
    int iHeight = 0;
    int x	= getBaseRenderLeft();
    int y	= getBaseRenderTop();

    int	iMaxLineHeight = 0;					// 현재까지 최고의 높이 값
    int	iLineWidth = 0;					// 현재 라인의 넓이 값			


    int iSize = GetSentenceCount();

    if (iSize <= iSentence)
    {
        return FALSE;
    }

    FTextSentence & rSentence = 
        (FTextSentence &)(const FTextSentence &)getSentence(iSentence);
    int ParamSize = rSentence.Size();
    for ( int j = iParam; j < ParamSize; ++j )
    {	
        const FTextParam & rParam = rSentence.GetParam(j);
        if (rParam.eType == FORMAT_TEXT_PARAM_RETURN)
        {
            break;
        }
        else
        {
            calcParam(rParam, iWidth, iHeight);
        }	

        iMaxLineHeight = max(iHeight, iMaxLineHeight);		
        if ( isOverRightRect(iCurX, iWidth) )
        {
            break;
        }	

        iLineWidth += iWidth + m_iCharGap;
        iCurX += iWidth + m_iCharGap;
    }

    riCurLineMaxHeight = iMaxLineHeight;
    riCurLineWidth = iLineWidth;

    return TRUE;	
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL					
FTextRender::calcParam(const FTextParam & Param, int & OUT riWidth, int & OUT riHeight)
{
    if (Param.eType == FORMAT_TEXT_PARAM_ICON)
    {
        riWidth = Param.draw_width;
        riHeight = Param.draw_height;
    }
    else
    {
        SIZE FontSize;
        m_pRender->x_pManagerTextOut->StartToUseFont(Param.wzFont);
        m_pRender->x_pManagerTextOut->GetTextExtentPoint( 
            (TCHAR *)(const TCHAR *)Param.m_StrContents.c_str(), 
            _tcslen(Param.m_StrContents.c_str()),
            &FontSize);

        riWidth = FontSize.cx;
        riHeight = FontSize.cy;

        m_pRender->x_pManagerTextOut->EndUseFont();	
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL					
FTextRender::renderParam( int x, int y, const FTextParam & Param, int iLineMaxHeight )
{
    if (Param.m_StrContents == "")
        return FALSE;

    RECT rc;

    int iWidth = 0;
    int iHeight = 0;

    if ( !calcParam(Param, iWidth, iHeight) )
    {
        return FALSE;
    }

    //////////////////////////////////////////////////////////////////////////
    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();
    BYTE btMouseOn = 0;

    if ((x <= iMouseX) &&
        ((x + iWidth) >= iMouseX) &&
        (y <= iMouseY) &&
        ((y + iHeight) >= iMouseY)
#ifdef _NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT
        && (getBaseRenderBottom() > iMouseY)
#endif //_NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT
        )
    {
        btMouseOn = 15;

        switch(Param.eType)
        {
        case FORMAT_TEXT_PARAM_ITEM_NAME:
            {
                uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
                if(NULL ==pUIMan)
                    break;

                BASE_ITEMINFO* pBII = ItemInfoParser::Instance()->GetItemInfo(Param.dwValue);
                if(NULL == pBII)
                    break;

                pUIMan->RegisterTooltipItem_OnText(pBII);
                pUIMan->SetTooltipDialogID(0);
            }
            break;

        case FORMAT_TEXT_PARAM_NPC_NAME:
#ifdef _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
        case FORMAT_TEXT_PARAM_QUEST_START_NPC:
#endif //_SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
            {
                uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
                if(NULL ==pUIMan)
                    break;

                BASE_NPCINFO* pBNI = NPCInfoParser::Instance()->GetNPCInfo(Param.dwValue);
                if(NULL == pBNI)
                    break;

                pUIMan->RegisterTooltipNPC_OnText(pBNI);
                pUIMan->SetTooltipDialogID(0);
            }
            break;

        case FORMAT_TEXT_PARAM_MAP_NAME:
        case FORMAT_TEXT_PARAM_MAP_OBJECT_NAME:
        case FORMAT_TEXT_PARAM_HERO_NAME:
        case FORMAT_TEXT_PARAM_HERO_CLASS:
        default:
            {
            }
            break;
        }
    }

    //////////////////////////////////////////////////////////////////////////

    //! 2011.9.27 / i4u4me / 중앙정렬 탭이 들어가면서 오른쪽 영역을 실제 그려질 영역으로 대체 
    //rc.right = 10000;
    rc.right = m_rcRender.right;
#ifdef _NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT
    //! 2012.11.13 / naklu / 밑부분이 짤리는 텍스트를 출력하게 하면서 밑부분 영역을 실제 그려질 영역으로 대체 
    rc.bottom = m_rcRender.bottom;
#else
    rc.bottom = 10000;
#endif //_NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT
    
    rc.left = x;
    rc.top  = y + (iLineMaxHeight - iHeight);

    DWORD _dwColor = Param.dwColor;

#ifdef _SCJ_111202_UI_RENEWAL
    if(FORMAT_TEXT_PARAM_ITEM_NAME == Param.eType)
    {
        _dwColor = WzColor_RGBA(
            min(255, 40+btMouseOn),
            min(255, 200+btMouseOn),
            min(255, 230+btMouseOn),
            255);
    }
    else if(FORMAT_TEXT_PARAM_MAP_NAME == Param.eType)
    {
        _dwColor = WzColor_RGBA(
            min(255, 250+btMouseOn),
            min(255, 120+btMouseOn),
            min(255, 170+btMouseOn),
            255);
    }
    else if(FORMAT_TEXT_PARAM_NPC_NAME == Param.eType)
    {
        _dwColor = WzColor_RGBA(
            min(255, 220+btMouseOn),
            min(255, 220+btMouseOn),
            min(255, 20+btMouseOn),
            255);
    }
#ifdef _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
    else if (FORMAT_TEXT_PARAM_QUEST_START_NPC == Param.eType)
    {
        _dwColor = WzColor_RGBA(
            min(255, 250+btMouseOn),
            min(255, 140+btMouseOn),
            min(255, 0+btMouseOn),
            255);
    }
#endif //_SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
#else //#else _SCJ_111202_UI_RENEWAL
    if(FORMAT_TEXT_PARAM_ITEM_NAME == Param.eType)
    {
        _dwColor = WzColor_RGBA(34 + btMouseOn, 105 + btMouseOn, 122 + btMouseOn, 255);
    }
    else if(FORMAT_TEXT_PARAM_MAP_NAME == Param.eType)
    {
        _dwColor = WzColor_RGBA(139 + btMouseOn, 87 + btMouseOn, 109 + btMouseOn, 255);
    }
    else if(FORMAT_TEXT_PARAM_NPC_NAME == Param.eType)
    {
        _dwColor = WzColor_RGBA(169 + btMouseOn, 164 + btMouseOn, 9 + btMouseOn, 255);
    }
#ifdef _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
    else if (FORMAT_TEXT_PARAM_QUEST_START_NPC == Param.eType)
    {
        _dwColor = WzColor_RGBA(180 + btMouseOn, 104 + btMouseOn, 9 + btMouseOn, 255);
    }
#endif //_SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
#endif //_SCJ_111202_UI_RENEWAL

    if (m_bAlphaModulate)
    {
        _dwColor = WzColor_RGBA(Red_WzColor(Param.dwColor),
            Green_WzColor(Param.dwColor),
            Blue_WzColor(Param.dwColor),
            m_byAlpha);
    }

    if (Param.eType == FORMAT_TEXT_PARAM_ICON)
    {
        RESOURCE_INFO* resource_info = GraphicResourceList::Instance()->GetItemInfo(Param.dwValue);
        if (resource_info == NULL)
        {
            return FALSE;
        }

        HANDLE texture_handle = m_pRender->x_pManagerTexture->LoadTexture(resource_info->szFilename, TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
        if (texture_handle == INVALID_HANDLE_VALUE)
        {
            return FALSE;
        }

#ifdef _NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT
        int bottom_pos = Param.draw_height;
        if (rc.top + bottom_pos > getBaseRenderBottom())
        {
            bottom_pos = getBaseRenderBottom() - rc.top;
        }
#endif //_NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT

        m_pRender->RenderTexture(texture_handle,
            static_cast<float>(rc.left),
            static_cast<float>(rc.top),
            static_cast<float>(Param.draw_width),
#ifdef _NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT
            static_cast<float>(bottom_pos),
#else
            static_cast<float>(Param.draw_height),
#endif //_NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT
            static_cast<float>(Param.texture_x),
            static_cast<float>(Param.texture_y),
            static_cast<float>(Param.texture_width),
            static_cast<float>(Param.texture_height));
    }
    else
    {
        m_pRender->x_pManagerTextOut->StartToUseFont(Param.wzFont);

        m_pRender->x_pManagerTextOut->DrawText((TCHAR *)(const TCHAR *)Param.m_StrContents.c_str(), 
                                                &rc, 
                                                _dwColor,
                                                WzColor_RGBA(0,0,0,0),
                                                Param.align_text_style/*TP_HLEFT*/,
                                                Param.eShapeType,
                                                Param.dwShapeColor);

        m_pRender->x_pManagerTextOut->EndUseFont();
    }

    return TRUE;
}

void					
FTextRender::renderDefault()
{
    int iCurLine = m_iStartLineIndex;
    int iWidth	= 0;
    int iHeight = 0;
    int iLine	= 0;
    int x		= getBaseRenderLeft();
    int y		= getBaseRenderTop();

    int	iMaxLineHeight = 0;						// 현재까지 최고의 높이 값
    int iMaxLineWidth = 0;						// 현재라인의 넓이 값 
    int	iPrevMaxLineHeight = 0;					// 현재까지 최고의 높이 값
    int iPrevLine = -1;
    FTextParam TempParam;

    int iSize	= GetSentenceCount();

    // 한번 돌아서 계산한 값이 있다.
    for ( int i = 0; i < iSize; ++i )
    {
        FTextSentence * pSentence = (FTextSentence *)m_vecSentences[i];
        int ParamSize = pSentence->Size();

        if (m_bGetValidMaxLine && y + iMaxLineHeight >= getBaseRenderBottom())
        {
            // 아래쪽 경계선에 걸렸다.
            break;
        }

        for ( int j = 0; j < ParamSize; ++j )
        {		
            if (m_bGetValidMaxLine && y + iMaxLineHeight >= getBaseRenderBottom())
            {
                // 아래쪽 경계선에 걸렸다.
                break;
            }

            switch( pSentence->GetParam(j).eType )
            {
            case FORMAT_TEXT_PARAM_RETURN:
                {
                    for ( size_t iReturn = 0; iReturn < pSentence->GetParam(j).dwValue; ++iReturn)
                    {
                        x = getBaseRenderLeft();
                        if ( iLine >= (int)m_iStartLineIndex )
                        {							
                            TempParam = pSentence->GetParam(j);
                            TempParam.m_StrContents = _T("N");	// 임시 값을 넣고 높이를 계산한다.
                            calcParam(TempParam, iWidth, iHeight);
                            iMaxLineHeight = iHeight;
                            y += iHeight + m_iLineGap;						
                        }
                        iPrevLine = iLine++;						
                    }
                }
                break;

            default:
                {				
                    calcParam( pSentence->GetParam(j), iWidth, iHeight );			

                    // 오른쪽 경계선에 걸렸나?
                    if ( isOverRightRect(x, iWidth) )
                    {
                        if ( iLine >= (int)m_iStartLineIndex )
                        {
                            y += iMaxLineHeight + m_iLineGap;								
                        }

                        x = getBaseRenderLeft();
                        iPrevLine = iLine++;				
                    }

                    if ( iPrevLine != iLine)
                    {
                        iPrevLine = iLine;
                        calcCurMaxLineHeight(x, i, j, iMaxLineWidth, iMaxLineHeight);
                    }

                    if ( iLine >= (int)m_iStartLineIndex )
                    {
                        renderParam(x, y + m_ExtraMarginTop, pSentence->GetParam(j), iMaxLineHeight );  
                    }

                    x += iWidth + m_iCharGap;
                }
                break;
            }					
        }	


        m_iMaxLine = max(m_iMaxLine, iLine);
    }

    m_bGetValidMaxLine = TRUE;	
}

void					
FTextRender::renderLineInfos()
{
    register size_t i = 0;
    register size_t j = 0;
    register int iWidth = 0;
    register int iHeight = 0;
    register int x = getBaseRenderLeft();
    register int y = getBaseRenderTop();
    int RenderWidth = getBaseRenderRight() - x;
    int ExtraMarginLeft = 0;

    for ( i = m_iStartLineIndex; i < GetMaxLine(); ++i)
    {
        LineInfo * pInfo = m_vecLineInfos[i];
        x = getBaseRenderLeft();
        if (m_bHCenterAlign)
        {
            ExtraMarginLeft = (RenderWidth - pInfo->iLineWidth) / 2;
        }
        else
        {
            ExtraMarginLeft = 0;
        }


#ifdef _NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT
        if (y > getBaseRenderBottom())
#else
        if (y + pInfo->iMaxHeight > getBaseRenderBottom())
#endif //_NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT
        {
            // 범위가 넘어갔다
            break;
        }

        for (j = 0; j < pInfo->vecParams.size(); ++j)
        {
            FTextParam * pParam = (FTextParam *)pInfo->vecParams[j];

            if (pParam->eType == FORMAT_TEXT_PARAM_RETURN) 
            {
                break;
            }
            else
            {
                calcParam(*pParam, iWidth, iHeight );
                renderParam(x + ExtraMarginLeft, y + m_ExtraMarginTop, *pParam, pInfo->iMaxHeight);
                x += iWidth;
            }
        }

        y += pInfo->iMaxHeight + m_iLineGap;
    }
}

//-------------------------------------------------------------------------------------------
/**
*/
int						
FTextRender::getBaseRenderLeft()
{
    return m_rcRender.left + m_iMarginLeft;
}

//-------------------------------------------------------------------------------------------
/**
*/
int						
FTextRender::getBaseRenderTop()
{
    return m_rcRender.top + m_iMarginTop;
}

//-------------------------------------------------------------------------------------------
/**
*/
int				
FTextRender::getBaseRenderRight()
{
    return m_rcRender.right - m_iMarginRight;	
}

//-------------------------------------------------------------------------------------------
/**
*/
int						
FTextRender::getBaseRenderBottom()
{
    return m_rcRender.bottom - m_iMarginBottom;	
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL					
FTextRender::isOverRightRect(int iCurX, int iWidth)
{
    return BOOL( getBaseRenderRight() <= iCurX + iWidth + m_iCharGap );
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
FTextRender::convertLineInfo()
{
    clearLineInfos();

    int iWidth	= 0;
    int iHeight = 0;
    int iLine	= 0;
    int x		= getBaseRenderLeft();
    int y		= getBaseRenderTop();

    int	iMaxLineHeight = 0;						// 현재까지 최고의 높이 값
    int iMaxLineWidth = 0;						// 현재라인의 넓이 값 
    int	iPrevMaxLineHeight = 0;					// 현재까지 최고의 높이 값
    int iPrevLine = -1;
    FTextParam TempParam;
    FTextParam PrevParam;

    int iSize	= GetSentenceCount();
    LineInfo * pDefaultLineInfo = NULL;

    // 한번 돌아서 계산한 값이 있다.
    for ( int i = 0; i < iSize; ++i )
    {
        FTextSentence * pSentence = (FTextSentence *)m_vecSentences[i];
        int ParamSize = pSentence->Size();

        // 2008.04.02 / lammy / 개행되고 한글자가 남았을때 나오지 않는 버그가 있어서 while문으로 수정
        int j = 0;
        while( 1 )
        {
            if ( NULL == pDefaultLineInfo )
            {
                pDefaultLineInfo = allocLineInfo();
            }

            if( j > ParamSize - 1 )
                break;

            switch( pSentence->GetParam(j).eType )
            {
            case FORMAT_TEXT_PARAM_RETURN:
                {
                    for ( size_t iReturn = 0; iReturn < pSentence->GetParam(j).dwValue; ++iReturn)
                    {
                        x = getBaseRenderLeft();   						

                        if ( pDefaultLineInfo && !TempParam.IsEmpty() )
                        {
                            pDefaultLineInfo->Pushback( TempParam );
                            TempParam.Clear();

                            pDefaultLineInfo->iMaxHeight = iPrevMaxLineHeight;
                            appendLineInfo( pDefaultLineInfo );
                            pDefaultLineInfo = NULL;
                        }
                        else
                        {
                            LineInfo * pLineInfo = allocLineInfo();

                            TempParam = pSentence->GetParam(j);
                            TempParam.m_StrContents = _T("N");	// 임시 값을 넣고 높이를 계산한다.					
                            calcParam(TempParam, iWidth, iHeight);
                            pLineInfo->iMaxHeight = iMaxLineHeight = iHeight;						
                            iPrevMaxLineHeight = iMaxLineHeight;
                            pLineInfo->Pushback( TempParam );
                            appendLineInfo(pLineInfo);
                            TempParam.Clear();
                        }

                        iPrevLine = iLine++;						
                    }
                }
                break;

            default:
                {				
                    calcParam( pSentence->GetParam(j), iWidth, iHeight );			

                    if ( iPrevLine != iLine)
                    {
                        iPrevLine = iLine;
                        calcCurMaxLineHeight(x, i, j, iMaxLineWidth, iMaxLineHeight);
                        pDefaultLineInfo->iMaxHeight = iMaxLineHeight;
                        iPrevMaxLineHeight = iMaxLineHeight;
                    }

                    // 오른쪽 경계선에 걸렸나?
                    if ( isOverRightRect(x, iWidth) )
                    {
                        y += iMaxLineHeight + m_iLineGap;								
                        x = getBaseRenderLeft();

                        pDefaultLineInfo->iMaxHeight = iMaxLineHeight;
                        iPrevMaxLineHeight = iMaxLineHeight;

                        if ( !TempParam.IsEmpty() )
                        {
#ifdef	_INTERNATIONAL_UI
                            char * tok = " ";
                            SUN_STRING temp( TempParam.m_StrContents.c_str() );

                            size_t len = temp.length();
                            size_t pos = len;
                            int	backcount = 0;

                            if( temp.compare( len - 1, 1, tok ) != 0 )
                            {
                                pos = temp.find_last_of( tok, pos );
                                backcount = len - pos - 1;
                                temp.erase( pos + 1, len );
                            }

                            // j가 0이 되어버리면 무한루프에 빠진다
                            if( backcount < j )
                            {
                                TempParam.m_StrContents = temp;

                                j -= backcount;
                            }

#endif//_INTERNATIONAL_UI
                            pDefaultLineInfo->Pushback( TempParam );
                            // 、。일경우는 개행하지 않고 끝에 붙여준다.
                            TCHAR szTemp[3];
                            _tcsncpy( szTemp, pSentence->GetParam(j).m_StrContents.c_str(), 2 );
                            szTemp[2] = NULL;

                            if( _ismbblead( szTemp[0] ) )
                            {
                                if( szTemp[1] == 0x41 || szTemp[1] == 0x42 )
                                {
                                    pDefaultLineInfo->Pushback( pSentence->GetParam(j) );									
                                    ++j;
                                }
                            }
                            else
                            {
                                if( szTemp[0] == '.' || szTemp[0] == ',' )
                                {
                                    pDefaultLineInfo->Pushback( pSentence->GetParam(j) );									
                                    ++j;
                                }
                            }							
                            TempParam.Clear();
                        }

                        if( j < ParamSize )
                        {
                            TempParam = pSentence->GetParam(j);
                        }

                        appendLineInfo( pDefaultLineInfo );
                        pDefaultLineInfo = NULL;
                        iPrevLine = iLine++;				
                    }
                    else
                    {
                        if ( TempParam == pSentence->GetParam(j))
                        {
                            TempParam = TempParam + pSentence->GetParam(j);
                        }
                        else
                        {
                            if ( !TempParam.IsEmpty() )
                            {
                                pDefaultLineInfo->Pushback(TempParam);								
                                TempParam.Clear();
                            }

                            TempParam = pSentence->GetParam(j);							
                        }
                    }

                    x += iWidth + m_iCharGap;
                }
                break;
            }					

            j++;
        }

        if ( pDefaultLineInfo )
        {	
            if ( !TempParam.IsEmpty() )
            {
                calcCurMaxLineHeight(x, i, j-1, iMaxLineWidth, iMaxLineHeight);
                pDefaultLineInfo->iMaxHeight = iMaxLineHeight;
                iPrevMaxLineHeight = iMaxLineHeight;

                pDefaultLineInfo->Pushback(TempParam);
                appendLineInfo( pDefaultLineInfo );
                pDefaultLineInfo = NULL;
                TempParam.Clear();
                x = getBaseRenderLeft();
            }
            else if ( 0 == pDefaultLineInfo->vecParams.size() )
            {
                delete pDefaultLineInfo;
                pDefaultLineInfo = NULL;
            }
        }
    }

    m_bReadyForRender =	TRUE;

    if ( GetBeginLineIndex() > GetMaxLine() )
    {
        SetBeginLineIndex(GetMaxLine()-1);
    }


    y = getBaseRenderTop();
    int RenderHeight = 0;
    size_t Line = 0;
    for ( ; Line < GetMaxLine(); ++Line)
    {
        LineInfo * pInfo = m_vecLineInfos[Line];

        if ( y + pInfo->iMaxHeight > getBaseRenderBottom() )
        {
            // 범위가 넘어갔다
            RenderHeight -= m_iLineGap;
            break;
        }
        y += pInfo->iMaxHeight + m_iLineGap;
        RenderHeight = y;
    }

    m_ExtraMarginTop = 0;
    if (m_bVCenterAlign)
    {
        m_ExtraMarginTop = (getBaseRenderBottom() - RenderHeight) / 2 ;
    }	

    m_iDrawLineInRect = Line;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL					
FTextRender::appendLineInfo(LineInfo * pLineInfo)
{
    assert( pLineInfo );
    if (pLineInfo) 
    {
        if (m_bHCenterAlign)
        {
            int width = 0;
            int height = 0;
            size_t count = pLineInfo->vecParams.size();
            for (size_t index = 0; index < count; ++index)
            {
                calcParam(*(pLineInfo->vecParams[index]), width, height);
                pLineInfo->iLineWidth += width;
            }
        }

        m_vecLineInfos.push_back(pLineInfo);
    }

    return BOOL(pLineInfo);
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
FTextRender::SetRect(RECT & rRenderRect)
{
    if ( (m_rcRender.right - m_rcRender.left) != (rRenderRect.right - rRenderRect.left))
    {
        m_bReadyForRender = FALSE;
    }

    memcpy( &m_rcRender, &rRenderRect, sizeof(m_rcRender) );
}

//-------------------------------------------------------------------------------------------
/** 
*/
void					
FTextRender::SetBeginLineIndex(size_t iIndex )
{
    if ( iIndex < 0)
    {
        iIndex = 0;
    }

    iIndex = min(GetMaxLine(), iIndex);

    m_iStartLineIndex = iIndex;
}

//-------------------------------------------------------------------------------------------
/** 
*/
size_t					
FTextRender::GetBeginLineIndex() const
{
    return m_iStartLineIndex;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
FTextRender::Render()
{
    if ( !m_pRender )
    {
        return;
    }

    BOOL bOld2dMode, bOldRealSize;
    m_pRender->Check2DMode(&bOld2dMode,&bOldRealSize);

    BOOL bIsEnableAlphaTest			= m_pRender->IsEnableAlphaTest();
    ENUM_ALPHABLEND eAlphablendMode = m_pRender->GetAlphaBlendMode();
    BOOL bZWrite					= m_pRender->IsEnableZWrite();
    BOOL bZTest						= m_pRender->IsEnableZTest();
    ENUM_CMP_FUNC eZFunc			= m_pRender->GetZFunc();
    ENUM_CULL_FACE eCullFaceMode	= g_pSunRenderer->GetCullFaceMode();

    g_pApplication->Set2DMode(TRUE);

    m_pRender->SetCullFaceMode( CF_DRAW_ALL );			
    m_pRender->SetZBufferWrite( FALSE );	
    m_pRender->SetZBufferTest(FALSE);
    m_pRender->SetAlphaBlendMode( AB_NEW_ALPHA, FALSE );
    m_pRender->SetAlphaBlendMode( AB_ALPHA, FALSE );
    m_pRender->EnableAlphaTest( TRUE );	

    if (!m_bReadyForRender) 
    {
        convertLineInfo();
    }
    else
    {
        renderLineInfos();
    }

    g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
    g_pSunRenderer->SetZFunc(eZFunc);
    g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
    g_pSunRenderer->SetZBufferTest( bZTest );

    //	랜더 스테이트 복구
    m_pRender->SetZBufferWrite( bZWrite );
    m_pRender->SetAlphaBlendMode( eAlphablendMode, FALSE );
    m_pRender->EnableAlphaTest( bIsEnableAlphaTest );

    g_pApplication->Set2DMode( bOld2dMode);	
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
FTextRender::SetMargin(int left, int top, int right, int bottom)
{
    m_iMarginLeft	  = left;
    m_iMarginTop	  = top;
    m_iMarginRight  = right;
    m_iMarginBottom = bottom;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
FTextRender::SetGap(int CharacterGap, int LineGap)
{
    m_iCharGap = CharacterGap;
    m_iLineGap = LineGap;
}

int						
FTextRender::getLineCount() const
{
    return m_vecLineInfos.size();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
FTextRender::clearLineInfos()
{
    for ( size_t i = 0; i < m_vecLineInfos.size(); ++i )
    {
        LineInfo * pInfo = m_vecLineInfos[i];
        SAFE_DELETE(pInfo);
    }	

    m_vecLineInfos.clear();
}

size_t FTextRender::GetDrawLineHeight()
{
    int render_height = m_iLineGap;
    for (size_t line = 0; line < GetMaxLine(); ++line)
    {
        LineInfo* line_info = m_vecLineInfos[line];
        render_height += (line_info->iMaxHeight + m_iLineGap);
    }

    return render_height;
}