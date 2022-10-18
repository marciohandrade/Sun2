//-------------------------------------------------------------------------------------------
/** 
	FTextParser	
	@Author : y2jinc ( y2inc@webzen.co.kr )
	@Desc  :				 
	@Since : 2006. 3. 13.
	
	@Function



	@TODO
*/
#pragma once

#include <stack>

enum eFORMAT_TEXT_TYPE
{
	eFORMAT_TEXT_TYPE_FONT = 0,
	eFORMAT_TEXT_TYPE_COLOR,
	eFORMAT_TEXT_TYPE_SHADOW,
	eFORMAT_TEXT_TYPE_SHADOW_COLOR,

    //! 2011.9.27 / i4u4me / 
    //ENUM_DRAWTEXTSTYLE 값 아래 3가지만 사용.
    //TP_HLEFT	= 0x0001,
    //TP_HRIGHT	= 0x0002,
    //TP_HCENTER	= 0x0004,
    eFORMAT_TEXT_TYPE_ALIGN, 

	eFORMAT_TEXT_TYPE_MAX,
	eFORMAT_TEXT_TYPE_NONE	// 이놈은 말그대로 없는놈. 
};

enum eFORMAT_TEXT_PARAM
{
	FORMAT_TEXT_PARAM_RETURN = 0,	// 줄 띄움
	FORMAT_TEXT_PARAM_HERO_NAME,	
	FORMAT_TEXT_PARAM_HERO_CLASS,
	FORMAT_TEXT_PARAM_NPC_NAME,
	FORMAT_TEXT_PARAM_ITEM_NAME,	
	FORMAT_TEXT_PARAM_MAP_NAME,	
	FORMAT_TEXT_PARAM_MAP_OBJECT_NAME,
    FORMAT_TEXT_PARAM_ICON,
#ifdef _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
    FORMAT_TEXT_PARAM_QUEST_START_NPC, // 퀘스트 시작(완료) NPC이름
#endif //_SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
	FORMAT_TEXT_PARAM_MAX,
	FORMAT_TEXT_PARAM_TEXT,			// 일반 텍스트
};

struct FT_TOKEN
{
	TCHAR szST_Token[4];	// 시작 토큰
	TCHAR szED_Token[4];	// 끝 토큰
};

typedef SUN_STRING						FT_ATTRIBUTE_TOKEN;
typedef SUN_STRING						FT_PARAM_TOKEN;		

struct FTAttrValue
{
	eFORMAT_TEXT_TYPE	eFormatType;
	DWORD				dwValue;
};

class FTextSentence;
class FTextParam;
                    

class FTextParser
{
public:
	FTextParser(void);
	virtual ~FTextParser(void);

	void		SetDefaultFont(WzID wzFont, DWORD dwColor);    
	BOOL		Parser( SUN_STRING strContents, FTextSentence & OUT rOutSentence );	

	void		SplitChar(FTextSentence & OUT rOutSentence);

private:
	BOOL		isValidFormatString( 
					SUN_STRING & strContents, 
					FTextSentence & OUT rOutSentence);

	// 속성 관련
	void		pushAttr(eFORMAT_TEXT_TYPE eType, FTAttrValue & OUT AttrValue);
	FTAttrValue	popAttr(eFORMAT_TEXT_TYPE eType);
	BOOL		topAttr(eFORMAT_TEXT_TYPE eType, FTAttrValue & OUT rOutAttrValue);
	int			sizeAttr(eFORMAT_TEXT_TYPE eType);

	// 파라미터 관련
	void		getTopParam( FTextParam & OUT rTextParam );		
	void		pushParam(SUN_STRING & IN strContents,
					FTextSentence & IN OUT rOutSentence, 
					int indexBegin, 
					int delta);
	void		clear();

	// parsing 관련 함수
	BOOL		parse(
					SUN_STRING & strContents, 
					FTextSentence & OUT rOutSentence);

    BOOL		parseStartToken( 
					SUN_STRING & strContents, 
					STR_INDEX & IN OUT indexBegin, 
					FTextSentence & OUT rOutSentence);

	BOOL		parseEndToken( 
					SUN_STRING & strContents, 
					STR_INDEX & IN OUT indexBegin, 
					FTextSentence & OUT rOutSentence);

	BOOL		parseAttrValue(
					SUN_STRING & strContents, 
					STR_INDEX & IN OUT indexBegin);

	BOOL		parseSpecialAttrValue(
					SUN_STRING & strContents, 
					STR_INDEX & IN OUT indexBegin, 
					FTextParam & OUT rOutParam);

	BOOL		parseGeneralText(
					SUN_STRING & IN strContents, 
					STR_INDEX & IN OUT indexBegin, 
					FTextSentence & OUT rOutSentence);
    
private:
	FT_TOKEN					m_StartToken;
	FT_TOKEN					m_EndToken;
	FT_TOKEN					m_SpecialToken;
	FT_ATTRIBUTE_TOKEN			m_AttributeToken[eFORMAT_TEXT_TYPE_MAX];
	FT_PARAM_TOKEN				m_ParamToken[FORMAT_TEXT_PARAM_MAX];

	WzID						m_wzDefaultFontID;
	DWORD						m_dwDefaultFontColor;

    
	std::stack<FTAttrValue>		m_AttributeStack[eFORMAT_TEXT_TYPE_MAX];
};

