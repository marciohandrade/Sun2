#pragma once
#include "testcase.h"
#include "FTextRender.h"
#include "FTextParser.h"
#include "FTextSentence.h"

class TestCaseFormatText :
	public TestCase
{
public:
	TestCaseFormatText(void);
	virtual ~TestCaseFormatText(void);

	virtual BOOL Init();							//	초기화, 로딩
	virtual BOOL Process( DWORD dwTick );			//	메인 계산
	virtual BOOL Render( DWORD dwTick );			//	메인 랜더링
	virtual void Release();							//	종료
	virtual void ParsePacket( MSG_BASE * pMsg ){};	//  네트워크 패킷 처리

	virtual void RenderText();
	virtual void ProcessKeyInput(DWORD dwTick);

	virtual BOOL EntMessage(WORD wMessage, int iParam1, int iParam2, int iParam3 = 0, int iParam4 = 0);

	void		SetRect(int left, int top, int right, int bottom);
	void		Append(DWORD dwStrID);
	void		Clear();

private:
	FTextParser		m_TextParser;
	FTextRender		m_TextRender;
	FTextSentence	m_TextSentence;
	RECT			m_rcRECT;
	TCHAR			m_szTest[512];
};
