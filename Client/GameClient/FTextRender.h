//-------------------------------------------------------------------------------------------
/** 
	FTextRender	
	@Author : y2jinc ( y2inc@webzen.co.kr )
	@Desc  : FTextSentence �� ������ ����(RECT)�� �°� �˾Ƽ� ������ִ� Ŭ����
	@Since : 2006. 3. 13.
	

	@Function
		- ���ڷ��� ��.
		- ������ ���θ� �����Ҽ� �ִ�. 
		- ���ΰ��� ���� ���ϰ� �÷����� ���� ���� �� �ִ�.
		- �����¿� ������ ���� �� �ִ�.
		- ���� ������ �ְ� ���̸� ����ؼ� ũ�Ⱑ Ʋ�� ��Ʈ�� ������ �����Ѵ�.
		- �ְ� ���μ��� �����.
		- ���� ������ ����(RECT)�ȿ� ������ ������ �� �ִ� ��ġ���� �ϸ� 
		  ���̻� ������ ���� �� ���� ����.

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
		DEFAULT_CHARACTER_GAP	= 0, // �ڰ�
		DEFAULT_LINE_GAP		= 2, // �ణ
	};

	FTextRender(void);
	virtual ~FTextRender(void);

	void					Init( CDrawBase * pRender, RECT & rRcRender );
	void					Release();
	int						GetSentenceCount() const;
	void					Append(FTextSentence & rSentence);
	void					ClearSentence();
    	
	// render ����
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
	int								m_iCharGap;	// �ڰ�
	int								m_iLineGap;	// �ణ
	int								m_iMaxLine;
	BOOL							m_bGetValidMaxLine;	// ����� ����
	BOOL							m_bReadyForRender;
	BOOL							m_bCanPageUp;

    BOOL                            m_bAlphaModulate;
    BYTE                            m_byAlpha;
	int								m_iDrawLineInRect;
	
	int								m_ExtraMarginTop;
	bool							m_bVCenterAlign;
	bool							m_bHCenterAlign;
};
