#include "SunClientPrecompiledHeader.h"
#include "ftextparser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "NPCInfoParser.h"
#include "ItemInfoParser.h"
#include "MapInfoParser.h"
#include "hero.h"
#include "GlobalFunc.h"
#include "interfacemanager.h"
//-------------------------------------------------------------------------------------------
/**
*/
FTextParser::FTextParser(void)
{
	// default font info
	this->m_wzDefaultFontID = StrToWzID("mn12");
	this->m_dwDefaultFontColor = 0xFFFFFFFF;

   	StrCopy(m_StartToken.szST_Token, _T("<"));
	StrCopy(m_StartToken.szED_Token, _T(">"));
	StrCopy(m_EndToken.szST_Token, ("</"));
	StrCopy(m_EndToken.szED_Token, _T(">"));
	StrCopy(m_SpecialToken.szST_Token, _T("<"));
	StrCopy(m_SpecialToken.szED_Token, _T("/>"));

	m_AttributeToken[eFORMAT_TEXT_TYPE_FONT] = _T("font");
	m_AttributeToken[eFORMAT_TEXT_TYPE_COLOR] = _T("color");
	m_AttributeToken[eFORMAT_TEXT_TYPE_SHADOW]		 = _T("shadow");
	m_AttributeToken[eFORMAT_TEXT_TYPE_SHADOW_COLOR] = _T("sdwcolor");
    m_AttributeToken[eFORMAT_TEXT_TYPE_ALIGN] = _T("align");

	m_ParamToken[0]	= _T("RETURN");			// FORMAT_TEXT_PARAM_RETURN = 0,
	m_ParamToken[1]	= _T("HERONAME");		// FORMAT_TEXT_PARAM_HERO_NAME,	
	m_ParamToken[2]	= _T("HEROCLASS");		// FORMAT_TEXT_PARAM_HERO_CLASS,
	m_ParamToken[3]	= _T("NPCNAME");		// FORMAT_TEXT_PARAM_NPC_NAME,	
	m_ParamToken[4]	= _T("ITEMNAME");		// FORMAT_TEXT_PARAM_ITEM_NAME,	
	m_ParamToken[5]	= _T("MAPNAME");		// FORMAT_TEXT_PARAM_MAP_NAME,	
	m_ParamToken[6]	= _T("MAPOBJECTNAME");	// FORMAT_TEXT_PARAM_MAP_OBJECT_NAME,
    m_ParamToken[7]	= _T("ICON_CODE");           // FORMAT_TEXT_PARAM_ICON,
#ifdef _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
    m_ParamToken[8]	= _T("QUESTSTARTNPC");      // FORMAT_TEXT_PARAM_QUEST_START_NPC
#endif //_SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC

}

//-------------------------------------------------------------------------------------------
/**
*/
FTextParser::~FTextParser(void)
{
	clear();
}

//-------------------------------------------------------------------------------------------
/**
*/
void	
FTextParser::SetDefaultFont(WzID wzFont, DWORD dwColor)
{
	this->m_wzDefaultFontID = wzFont;
	this->m_dwDefaultFontColor = dwColor;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
FTextParser::Parser( SUN_STRING strContents, FTextSentence & OUT rOutSentence )
{	
	rOutSentence.Release();

	this->clear();

	if ( !strContents.size() )
	{
		return FALSE;
	}

	if ( !isValidFormatString(strContents, rOutSentence) ) 	
	{
		return FALSE;
	}
	
	BOOL bRet = this->parse(strContents, rOutSentence);

	SplitChar(rOutSentence);

	return bRet; 
}

//-------------------------------------------------------------------------------------------
/**
*/
void		
FTextParser::SplitChar(FTextSentence & OUT rOutSentence)
{
	FTextSentence	TempSentence;
	FTextParam		TempParam;

	int iSize = rOutSentence.Size();
	for ( int i = 0; i < iSize; ++i)
	{
		const FTextParam & rParam = rOutSentence.GetParam(i);
		TempParam = rParam;
		switch(rParam.eType)
		{
		case FORMAT_TEXT_PARAM_RETURN:
        case FORMAT_TEXT_PARAM_ICON:
			{ // return은 그냥 skip
				TempSentence.Pushback( rParam );
			}
			break;
		case FORMAT_TEXT_PARAM_HERO_NAME:
        case FORMAT_TEXT_PARAM_HERO_CLASS:
		case FORMAT_TEXT_PARAM_NPC_NAME:
		case FORMAT_TEXT_PARAM_ITEM_NAME:	
		case FORMAT_TEXT_PARAM_MAP_NAME:	
		case FORMAT_TEXT_PARAM_MAP_OBJECT_NAME:
		case FORMAT_TEXT_PARAM_TEXT:
#ifdef _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
        case FORMAT_TEXT_PARAM_QUEST_START_NPC:
#endif //_SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
			{
				// 한자씩 파람으로 만든다.
				TCHAR * pszString = (TCHAR *)(const TCHAR *)rParam.m_StrContents.c_str();
				TCHAR * pszCur = pszString; 
				TCHAR * pszPrev = pszString; 
				int		iCharSize = 0;

				TCHAR	szTemp[4];
				
				while ( pszCur = CharNext( pszCur ) )
				{ 
					ZeroMemory(szTemp, sizeof(szTemp));
					iCharSize = pszCur - pszPrev;
					if ( 0 == iCharSize )
					{
						break;
					}
					strncpy(szTemp, pszPrev, iCharSize);
					TempParam.m_StrContents = szTemp;
					TempSentence.Pushback(TempParam);
					pszPrev = pszCur;		
				}
					
			}
			break;
		}
	}

	rOutSentence = TempSentence;	
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
FTextParser::isValidFormatString( SUN_STRING & strContents, FTextSentence & OUT rOutSentence )
{
	BOOL bRet = TRUE;
	bRet = this->parse( strContents, rOutSentence );

	for ( int i = 0; i < eFORMAT_TEXT_TYPE_MAX; ++i)
	{
		if ( this->m_AttributeStack[i].size() )
		{
			bRet = FALSE;
			break;
		}
	}

	this->clear();
	rOutSentence.Release();

	return bRet;
}

//-------------------------------------------------------------------------------------------
/**
*/
void		
FTextParser::pushAttr(eFORMAT_TEXT_TYPE eType, FTAttrValue & AttrValue)
{
	if ( eFORMAT_TEXT_TYPE_NONE == eType)
	{
		return;
	}

	this->m_AttributeStack[eType].push(AttrValue);
}

//-------------------------------------------------------------------------------------------
/**
*/
FTAttrValue	
FTextParser::popAttr(eFORMAT_TEXT_TYPE eType)
{
	assert( this->m_AttributeStack[eType].size() );

	FTAttrValue Attr = m_AttributeStack[eType].top();
	this->m_AttributeStack[eType].pop();
	return Attr;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL
FTextParser::topAttr(eFORMAT_TEXT_TYPE eType, FTAttrValue & OUT rOutAttrValue)
{
	if (this->m_AttributeStack[eType].size())
	{
		rOutAttrValue = this->m_AttributeStack[eType].top();
		return TRUE;
	}
	
	return FALSE;	
}


//-------------------------------------------------------------------------------------------
/**
*/
int			
FTextParser::sizeAttr(eFORMAT_TEXT_TYPE eType)
{
	return this->m_AttributeStack[eType].size();
}

void		
FTextParser::getTopParam( FTextParam & OUT rTextParam )
{
	FTAttrValue TempAttr;

	rTextParam.wzFont	  = m_wzDefaultFontID;
	rTextParam.dwColor	  = m_dwDefaultFontColor;
	rTextParam.eShapeType = ETS_BASE;
	rTextParam.dwShapeColor = 0xffffffff;
    rTextParam.align_text_style = TP_HLEFT;

	if ( topAttr(eFORMAT_TEXT_TYPE_FONT, TempAttr) )
	{
		rTextParam.wzFont = (WzID)TempAttr.dwValue;
	}

	if ( topAttr(eFORMAT_TEXT_TYPE_COLOR, TempAttr) )
	{
		rTextParam.dwColor = TempAttr.dwValue;
	}	

	if(topAttr(eFORMAT_TEXT_TYPE_SHADOW, TempAttr))
	{
		rTextParam.eShapeType = (ENUM_TEXT_SHAPE)TempAttr.dwValue;
	}

	if(topAttr(eFORMAT_TEXT_TYPE_SHADOW_COLOR, TempAttr))
	{
		rTextParam.dwShapeColor = TempAttr.dwValue;
	}
    if(topAttr(eFORMAT_TEXT_TYPE_ALIGN, TempAttr))
    {
        rTextParam.align_text_style = static_cast<ENUM_DRAWTEXTSTYLE>(TempAttr.dwValue);
    }

}


//-------------------------------------------------------------------------------------------
/**
*/
void		
FTextParser::pushParam(SUN_STRING & IN strContents, FTextSentence & IN OUT rOutSentence, int indexBegin, int delta)
{
	FTextParam	TempTextParam;
	TempTextParam.m_StrContents = strContents.substr(indexBegin, delta);
	this->getTopParam( TempTextParam );
	rOutSentence.Pushback(TempTextParam);	
}


//-------------------------------------------------------------------------------------------
/**
*/
void		
FTextParser::clear()
{
	for ( int i = 0; i < eFORMAT_TEXT_TYPE_MAX; ++i)
	{
		while ( !m_AttributeStack[i].empty() )
		{
			m_AttributeStack[i].pop();
		}
	}
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL		
FTextParser::parse(SUN_STRING & strContents, FTextSentence & OUT rOutSentence)
{
	STR_INDEX iEndofString = strContents.size();
	STR_INDEX indexBegin = 0;

	do {
		// 토큰 끝 파스
		if ( !parseEndToken( strContents, indexBegin, rOutSentence) )
		{
			// 토큰 시작 파스
			if ( !parseStartToken( strContents, indexBegin, rOutSentence) )
			{
				// 일반 텍스트 파스
				parseGeneralText( strContents, indexBegin, rOutSentence );
			}
		}
	} while( iEndofString > indexBegin );

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL		
FTextParser::parseStartToken( SUN_STRING & strContents, STR_INDEX & IN OUT indexBegin, FTextSentence & OUT rOutSentence )
{
	TCHAR * pszString =	(TCHAR *)(const TCHAR *)strContents.c_str();
	BOOL bRet = FALSE;

	basic_string<TCHAR>::size_type	indexEnd = 0;
	basic_string<TCHAR>::size_type	notPos = -1;
	FTextParam TempTextParam;
	int	iDelta = 0;
	int iInvalidStartTokenCnt = 0; 

	int indexBeginBak = indexBegin;
	int iEndofString = strContents.size();
	int iSTTokenSize = _tcslen(this->m_StartToken.szST_Token);
	
	do 
	{
		if(_Strncmp(pszString+indexBegin, 1, _T("/"), 1))
		{
			if(_Strncmp(pszString+indexBegin+1, 1, _T("*"), 1))
			{
				while(1)
				{
					if(iEndofString <= (int)indexBegin)
					{
						indexBegin = indexBeginBak;
						break;
					}
					if(_Strncmp(pszString+indexBegin, 1, _T("*"), 1))
					{
						if(_Strncmp(pszString+indexBegin+1, 1, _T("/"), 1))
						{
							indexBegin += 2;
							break;
						}
					}

					indexBegin += 1;
				}
			}
		}

		if ( _Strncmp(pszString+indexBegin, iSTTokenSize, this->m_StartToken.szST_Token, iSTTokenSize) )
		{		
			indexBegin += _tcslen(this->m_StartToken.szST_Token);
			// 속성 파스
			if ( !parseAttrValue(strContents, indexBegin) )
			{
				// 스페셜 속성 파스
				if ( !parseSpecialAttrValue(strContents, indexBegin, TempTextParam) )
				{
					// 스폐셜 속성도 아닌데 시작 토큰이면 그냥 문자다.
					iInvalidStartTokenCnt++;
				}			
				else
				{
   					if (iInvalidStartTokenCnt)
					{
						pushParam(strContents, rOutSentence, indexBeginBak, iInvalidStartTokenCnt);
					}

					rOutSentence.Pushback( TempTextParam );
					
					bRet = TRUE;
					break;
				}
			}	
			else
			{
   				if (iInvalidStartTokenCnt)
				{
					pushParam(strContents, rOutSentence, indexBeginBak, iInvalidStartTokenCnt);
				}
				bRet = TRUE;
				break;
			}  	
		}
		else
		{
   			if (iInvalidStartTokenCnt)
			{
				pushParam(strContents, rOutSentence, indexBeginBak, iInvalidStartTokenCnt);
			}
			break;
		}
	} while ( iEndofString > (int)indexBegin);


 	return bRet;	
}

BOOL		
FTextParser::parseEndToken( SUN_STRING & strContents, STR_INDEX & IN OUT indexBegin, FTextSentence & OUT rOutSentence )
{
	BOOL bRet = FALSE;
	TCHAR * pszString =	(TCHAR *)(const TCHAR *)strContents.c_str();
	
	STR_INDEX	indexEnd = 0;
	STR_INDEX	notPos = -1;
	int	iDelta = 0;
	int iIndexBeginBak = indexBegin;

	int iEDTokenSize = _tcslen(this->m_EndToken.szST_Token);

	if ( _Strncmp(pszString+indexBegin, iEDTokenSize, this->m_EndToken.szST_Token, iEDTokenSize) )
	{
		// 하나의 속성을 닫았다.
		indexBegin += iEDTokenSize;

		// 어떤 속성이냐?
		for ( int i =0; i < eFORMAT_TEXT_TYPE_MAX; ++i)
		{
			TCHAR * pszTokenString = (TCHAR *)(const TCHAR *)m_AttributeToken[i].c_str();
			int iLen = _tcslen( pszTokenString );
			if ( _Strincmp(pszString + indexBegin, iLen, pszTokenString, iLen ) )
			{
				indexBegin += iLen;
			}

	   		// 끝 토큰이냐?
			int iEndTokenLen = _tcslen( m_EndToken.szED_Token );
			if ( _Strncmp(pszString + indexBegin, iEndTokenLen, m_EndToken.szED_Token, iEndTokenLen ) )
			{
				// 토큰 하나가 닫혔다.
				// 내용하나를 비운다.
				indexBegin += iEndTokenLen;
				if ( this->sizeAttr((eFORMAT_TEXT_TYPE)i) )
				{
					this->popAttr((eFORMAT_TEXT_TYPE)i);
					bRet = TRUE;
				}
				else
				{
					bRet = FALSE;
				}		
				break;
			}
		}

		if (!bRet)
		{
			// 아무속성이 없었다면
			// 인덱스를 뒤로 돌린후
			// 일반 Text로 보고 텍스트를 얻어온다.
			this->pushParam(strContents, rOutSentence, iIndexBeginBak, iEDTokenSize);
//			bRet = this->parseGeneralText(strContents, indexBegin, rOutSentence);
		}
	}
             
	return bRet;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL		
FTextParser::parseAttrValue(SUN_STRING & strContents, STR_INDEX & IN OUT indexBegin )
{
	BOOL bRet = FALSE;
	STR_INDEX	indexEnd = 0;
	STR_INDEX	notPos = -1;
	SUN_STRING	strValue;
	DWORD		dwValue;
	int			iDelta = 0;

	TCHAR * pszString =	(TCHAR *)(const TCHAR *)strContents.c_str();

	for ( int i =0; i < eFORMAT_TEXT_TYPE_MAX; ++i)
	{
		TCHAR * pszTokenString = (TCHAR *)(const TCHAR *)m_AttributeToken[i].c_str();
		int iLen = _tcslen( pszTokenString );
		if ( _Strincmp(pszString + indexBegin, iLen, pszTokenString, iLen ) )
		{
			indexBegin += iLen;

			FTAttrValue Attr;
			Attr.eFormatType = (eFORMAT_TEXT_TYPE)i;
			indexEnd = strContents.find_first_of(_T("="), indexBegin);
			indexBegin = indexEnd+1;
			
			// 값을 얻는다.
			indexEnd = strContents.find_first_of(m_StartToken.szED_Token, indexBegin);
			iDelta = indexEnd - indexBegin;
			strValue = strContents.substr(indexBegin, iDelta);
			indexBegin = indexEnd+1;

			switch ((eFORMAT_TEXT_TYPE)i) 
			{
			case eFORMAT_TEXT_TYPE_FONT:
				{
					dwValue = StrToWzID((TCHAR *)(const TCHAR *)strValue.c_str());
				}
				break;

			case eFORMAT_TEXT_TYPE_COLOR:
				{
					_stscanf(	strValue.c_str(), _T("%X"), &dwValue);
				}
				break;

			case eFORMAT_TEXT_TYPE_SHADOW:
				{
					dwValue = atoi(strValue.c_str());
					if(2 < dwValue)
					{
						dwValue = 2;
					}
				}
				break;

			case eFORMAT_TEXT_TYPE_SHADOW_COLOR:
				{
					_stscanf(	strValue.c_str(), _T("%X"), &dwValue);
				}
				break;

            case eFORMAT_TEXT_TYPE_ALIGN:
                {
                    _stscanf(	strValue.c_str(), _T("%d"), &dwValue);
                }
                break;
			}

			Attr.dwValue = dwValue;

			this->pushAttr((eFORMAT_TEXT_TYPE)i, Attr);
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL		
FTextParser::parseSpecialAttrValue(SUN_STRING & strContents, STR_INDEX & IN OUT indexBegin, FTextParam & OUT rOutParam)
{
	BOOL		bRet = FALSE;
	SUN_STRING	strValue;
	TCHAR		szValue[INTERFACE_STRING_LENGTH];
	int			iDelta = 0;
	STR_INDEX	indexEnd = 0;
	STR_INDEX	notPos = -1;
	FTextParam TempTextParam;
	TCHAR * pszString =	(TCHAR *)(const TCHAR *)strContents.c_str();

	for (int i =0; i < FORMAT_TEXT_PARAM_MAX; ++i)
	{
 		TCHAR * pszTokenString = (TCHAR *)(const TCHAR *)this->m_ParamToken[i].c_str();
		int iLen = _tcslen( pszTokenString );
		if ( _Strincmp(pszString + indexBegin, iLen, pszTokenString, iLen ) )
		{
			indexBegin += iLen;

			if (((eFORMAT_TEXT_PARAM)i == FORMAT_TEXT_PARAM_HERO_NAME) || 
                ((eFORMAT_TEXT_PARAM)i == FORMAT_TEXT_PARAM_HERO_CLASS))
			{
				// 값이 없어서 건너뛴다.
				indexEnd   = strContents.find_first_of(this->m_SpecialToken.szED_Token, indexBegin);
				indexBegin = indexEnd+_tcslen(this->m_SpecialToken.szED_Token);
			}
			else
			{
				// 값을 얻는다.
				indexEnd   = strContents.find_first_of(_T("="), indexBegin);
				indexBegin = indexEnd+1;
				indexEnd   = strContents.find_first_of(this->m_SpecialToken.szED_Token, indexBegin);
				iDelta     = indexEnd - indexBegin;
				strValue   = strContents.substr(indexBegin, iDelta);
				indexBegin = indexEnd+_tcslen(this->m_SpecialToken.szED_Token);
			}
		
			// 최신 파라미터 속성을 받아온다.
			this->getTopParam( TempTextParam );

			TempTextParam.eType = (eFORMAT_TEXT_PARAM)i;
			switch (TempTextParam.eType) 
			{
			case FORMAT_TEXT_PARAM_RETURN:
				_stscanf(	strValue.c_str(), _T("%d"), &TempTextParam.dwValue);	// 몇 칸 띄울지 얻어온다.
				break;

			case FORMAT_TEXT_PARAM_HERO_NAME:
				{
					if (g_pHero) 
					{
						TempTextParam.m_StrContents = g_pHero->GetName();
					}
					else
					{
						TempTextParam.m_StrContents = _T("HERONAME");
					}
				}
				break;

			case FORMAT_TEXT_PARAM_HERO_CLASS:
				{
					if (g_pHero) 
					{
						int iStrID = eST_LIMITCLASS_BER_STRING_INDEX;
						switch (g_pHero->GetClass()) 
						{
						case eCHAR_BERSERKER:
							iStrID = eST_LIMITCLASS_BER_STRING_INDEX;
							break;
						case eCHAR_DRAGON:
							iStrID = eST_LIMITCLASS_DRA_STRING_INDEX;
							break;
						case eCHAR_SHADOW:
							iStrID = eST_LIMITCLASS_SHA_STRING_INDEX;
							break;
						case eCHAR_VALKYRIE:
							iStrID = eST_LIMITCLASS_VAL_STRING_INDEX;
							break;
						case eCHAR_ELEMENTALIST:
							iStrID = eST_LIMITCLASS_ELM_STRING_INDEX;
							break;
                        case eCHAR_MYSTIC://_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                            iStrID = eST_LIMITCLASS_MYS_STRING_INDEX;
                            break;
                        case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                            iStrID = eST_LIMITCLASS_HEL_STRING_INDEX;
                            break;
                        case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                            iStrID = eST_LIMITCLASS_WIT_STRING_INDEX;
                            break;
						}

						g_InterfaceManager.GetInterfaceString(iStrID, szValue, INTERFACE_STRING_LENGTH);

						TempTextParam.m_StrContents = szValue;
					}
					else
					{
						TempTextParam.m_StrContents = _T("HEROCLASS");
					}
				}
				break;

			case FORMAT_TEXT_PARAM_NPC_NAME:
#ifdef _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
            case FORMAT_TEXT_PARAM_QUEST_START_NPC:
#endif //_SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
				{
					_stscanf(	strValue.c_str(), _T("%d"), &TempTextParam.dwValue);
					BASE_NPCINFO *pInfo = NPCInfoParser::Instance()->GetNPCInfo( TempTextParam.dwValue );
					if ( pInfo )
					{
#ifdef _DH_NPC_GROUP_NAME
                        TCHAR group_name[INTERFACE_STRING_LENGTH] = {0,};
                        TCHAR npc_name[INTERFACE_STRING_LENGTH] = {0,};
                        if (pInfo->group_name != 0)
                        {
                            g_InterfaceManager.GetInterfaceString(pInfo->group_name, group_name);
                            g_InterfaceManager.GetInterfaceString(pInfo->m_NCode, npc_name);
                            Snprintf(szValue, INTERFACE_STRING_LENGTH, "<%s> %s", group_name, npc_name);
                        }
                        else
#endif//_DH_NPC_GROUP_NAME
                        {
						    g_InterfaceManager.GetInterfaceString( pInfo->m_NCode, szValue, INTERFACE_STRING_LENGTH );
                        }
					}
					else
					{
						Snprintf(szValue, INTERFACE_STRING_LENGTH, _T("INVALID NPC NAME(%d)"), TempTextParam.dwValue);								
					}

#ifdef _DEV_VER
                    //#ifdef _SCJ_SHOW_NPC_ID
                    if (CGeneralGameParam::Instance()->IsShowNpcId())
                    {
                        TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
                        StrnCopy(buffer_string, szValue, INTERFACE_STRING_LENGTH-1); 
                        Snprintf(szValue, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                            buffer_string, pInfo->m_MonsterCode);
                    }
                    //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER

					TempTextParam.m_StrContents = szValue;
				}
				break;

			case FORMAT_TEXT_PARAM_ITEM_NAME:	
				{
					_stscanf(	strValue.c_str(), _T("%d"), &TempTextParam.dwValue);
					BASE_ITEMINFO *pInfo = ItemInfoParser::Instance()->GetItemInfo( TempTextParam.dwValue );
					if ( pInfo )
					{
						g_InterfaceManager.GetInterfaceString( pInfo->m_NCode, szValue, INTERFACE_STRING_LENGTH );
					}
					else
					{
						Snprintf(szValue, INTERFACE_STRING_LENGTH, _T("INVALID ITEM NAME(%d)"), TempTextParam.dwValue);								
					}

					TempTextParam.m_StrContents = szValue;
				}
				break;

			case FORMAT_TEXT_PARAM_MAP_NAME:	
				{
					_stscanf(	strValue.c_str(), _T("%d"), &TempTextParam.dwValue);
					const sMAPINFO * pInfo = MapInfoParser::Instance()->FindMapInfo( (MAPCODE)TempTextParam.dwValue );
					if ( pInfo )
					{
						g_InterfaceManager.GetInterfaceString( pInfo->dwNCode, szValue, INTERFACE_STRING_LENGTH );
					}
					else
					{
						Snprintf(szValue, INTERFACE_STRING_LENGTH, _T("INVALID MAP NAME(%d)"), TempTextParam.dwValue);								
					}

					TempTextParam.m_StrContents = szValue;
				}
				break;

			case FORMAT_TEXT_PARAM_MAP_OBJECT_NAME:
				{
					_stscanf(	strValue.c_str(), _T("%d"), &TempTextParam.dwValue);
					Snprintf(szValue, INTERFACE_STRING_LENGTH, _T("INVALID MAP OBJECT NAME(%d)"), TempTextParam.dwValue);
					TempTextParam.m_StrContents = szValue;
				}
				break;

            case FORMAT_TEXT_PARAM_ICON:
                {
                    _stscanf(strValue.c_str(), _T("%d,%d,%d,%d,%d,%d,%d"), 
                        &TempTextParam.dwValue, 
                        &TempTextParam.texture_x, 
                        &TempTextParam.texture_y, 
                        &TempTextParam.texture_width, 
                        &TempTextParam.texture_height, 
                        &TempTextParam.draw_width, 
                        &TempTextParam.draw_height);
                    TempTextParam.m_StrContents = m_ParamToken[FORMAT_TEXT_PARAM_ICON];

                    if ((TempTextParam.dwValue == 0) ||
                        (TempTextParam.texture_width) == 0 || 
                        (TempTextParam.texture_height) == 0 || 
                        (TempTextParam.draw_width) == 0 || 
                        (TempTextParam.draw_height) == 0)
                    {
                        assert(!"FORMAT_TEXT_PARAM_ICON - INVALID PARAM");
                    }                    
                }
                break;
			default: assert(0);
				break;
			}

			rOutParam = TempTextParam;
			bRet = TRUE;			
			break;
		}
	}

	return bRet;
}

BOOL		
FTextParser::parseGeneralText(SUN_STRING & IN strContents, STR_INDEX & IN OUT indexBegin, FTextSentence & OUT rOutSentence)
{
	STR_INDEX	indexEnd = 0;
	STR_INDEX	notPos = -1;
	int			iDelta = 0;
	int			iEndofString = strContents.size();

	FTextParam TempTextParam;

	indexEnd = strContents.find_first_of(m_StartToken.szST_Token, indexBegin);
	if ( notPos == indexEnd )
	{
		// 아무런 토큰이 없구만.
		iDelta = iEndofString - indexBegin;							
	}
	else
	{
		iDelta = indexEnd - indexBegin;
	}

	TempTextParam.m_StrContents = strContents.substr(indexBegin, iDelta);

	this->getTopParam( TempTextParam );

	rOutSentence.Pushback(TempTextParam);

	indexBegin = indexEnd;
    
	return TRUE;
}
