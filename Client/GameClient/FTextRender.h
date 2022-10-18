//-------------------------------------------------------------------------------------------
/** 
	FTextRender	
	@Author : y2jinc ( y2inc@webzen.co.kr )
	@Desc  : FTextSentence 를 일정한 영역(RECT)에 맞게 알아서 출력해주는 클래스
	@Since : 2006. 3. 13.
	

	@Function
		- 문자랩이 됨.
		- 렌더할 라인를 지정할수 있다. 
		- 라인간의 갭을 정하고 컬럼간에 갭을 정할 수 있다.
		- 상하좌우 여백을 정할 수 있다.
		- 현재 라인의 최고 높이를 계산해서 크기가 틀린 폰트를 적절히 렌더한다.
		- 최고 라인수를 계산함.
		- 현재 보여줄 영역(RECT)안에 마지막 보여줄 수 있는 위치까지 하면 
		  더이상 라인을 내릴 수 없게 했음.

	@TODO		
*/

#pragma once
#include "FTextParam.h"

class FTextSentence;
class FTextParam;

class FTextRender
{
public:
	struct LineInfo
	{
		LineInfo()
			:iLineWidth(0)
		{}
		~LineInfo()
		{
			for ( size_t i = 0; i < vecParams.size(); ++i)
			{
				delete (FTextParam *)vecParams[i];
			}

			vecParams.clear();
		}

		BOOL Pushback( const FTextParam & rParam )
		{
			FTextParam * pParam = new FTextParam(rParam);
			assert(pParam);
			if (pParam) 
			{
				vecParams.push_back(pParam);
			}

			return BOOL(pParam);
		}

		int							iMaxHeight;
		int							iLineWidth;
		std::vector<FTextParam *>	vecParams;
	};

	enum
	{
		DEFAULT_MARGIN_LEFT		= 3,
		DEFAULT_MARGIN_TOP		= 3,
		DEFAULT_MARGIN_RIGHT	= 3,
        DEFAULT_MARGIN_BOTTOM	= 3,
		DEFAULT_CHARACTER_GAP	= 0, // 자간
		DEFAULT_LINE_GAP		= 2, // 행간
	};

	FTextRender(void);
	virtual ~FTextRender(void);

	void					Init( CDrawBase * pRender, RECT & rRcRender );
	void					Release();
	int						GetSentenceCount() const;
	void					Append(FTextSentence & rSentence);
	void					ClearSentence();
    	
	// render 관련
	void					SetRect(RECT & rRenderRect);
	const RECT				GetRect() const { return this->m_rcRender; }
	void					SetBeginLineIndex(size_t iIndex );
	size_t					GetBeginLineIndex() const;
	size_t					GetMaxLine() const	{	return m_vecLineInfos.size(); }
	size_t					GetDrawLineInRect()	{	if (m_bReadyForRender)	{	return m_iDrawLineInRect; }	return 0; }
    size_t					GetDrawLineHeight();

	void					convertLineInfo(); 

	void					Render();

	void					SetMargin(int left, int top, int right, int bottom);
	void					SetGap(int CharacterGap, int LineGap);

    inline void             SetAlphaModulate(BOOL modulate)
    {
        m_bAlphaModulate = modulate;
    }

    inline void             SetAlphaValue(BYTE byValue)
    {
        m_byAlpha = byValue;
    }

	void					SetReadyForRender( BOOL b ) { m_bReadyForRender = b; }
	void					SetVCenterAlign(bool bAlign)	{	m_bVCenterAlign = bAlign; }
	void					SetHCenterAlign(bool bAlign)	{	m_bHCenterAlign = bAlign; }

	int						getGapLine() { return m_iLineGap; }
	int						getGapChar() { return m_iCharGap; }
    int						getLineCount() const;
protected:
	void					clearLineInfos();


	const FTextSentence &	getSentence( int iIndex );
	BOOL					calcCurMaxLineHeight( 
									int iCurX,
									int iSentence, 
									int iParam, 
									int & OUT riCurLineWidth,
									int & OUT riCurLineMaxHeight);
	BOOL					calcParam(
								const FTextParam & Param, 
								int & OUT riWidth, 
								int & OUT riHeight);
	BOOL					renderParam( 
								int x, 
								int y, 
								const FTextParam & Param, 
								int iLineMaxHeight); 

	void					renderDefault();
	void					renderLineInfos();

	int						getBaseRenderLeft();
	int						getBaseRenderTop();
	int						getBaseRenderRight();
	int						getBaseRenderBottom();

	BOOL					isOverRightRect(int iCurX, int iWidth);

	LineInfo *				allocLineInfo()	{ return new LineInfo; }
	BOOL					appendLineInfo(LineInfo * pLineInfo);



private:
	std::vector<FTextSentence *>	m_vecSentences;
	std::vector<LineInfo *>			m_vecLineInfos;
	size_t							m_iStartLineIndex;
	CDrawBase *						m_pRender;
	RECT							m_rcRender;	
	POINT							m_CurRenderPoint;
	int								m_iMarginTop;
	int								m_iMarginLeft;
	int								m_iMarginBottom;
	int								m_iMarginRight;
	int								m_iCharGap;	// 자간
	int								m_iLineGap;	// 행간
	int								m_iMaxLine;
	BOOL							m_bGetValidMaxLine;	// 사라질 예정
	BOOL							m_bReadyForRender;
	BOOL							m_bCanPageUp;

    BOOL                            m_bAlphaModulate;
    BYTE                            m_byAlpha;
	int								m_iDrawLineInRect;
	
	int								m_ExtraMarginTop;
	bool							m_bVCenterAlign;
	bool							m_bHCenterAlign;
};
