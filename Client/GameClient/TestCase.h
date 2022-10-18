#pragma once

#include "TestCaseMsg.h"

class TestCase
{
public:
	TestCase(void);
	virtual ~TestCase(void);

	virtual BOOL Init() = 0;							//	초기화, 로딩
	virtual BOOL Process( DWORD dwTick ) = 0;			//	메인 계산
	virtual BOOL Render( DWORD dwTick ) = 0;			//	메인 랜더링
	virtual void Release() = 0;							//	종료
	virtual void ParsePacket( MSG_BASE * pMsg ){};	//  네트워크 패킷 처리

	virtual void RenderText() = 0;
	virtual void ProcessKeyInput(DWORD dwTick) = 0;

	virtual BOOL EntMessage(WORD wMessage, int iParam1, int iParam2, int iParam3 = 0, int iParam4 = 0);


	void					SetName(const SUN_STRING & strName);
	void					SetName(TCHAR * pszName);
	const SUN_STRING &		GetName() const;

private:
	SUN_STRING		m_strName;
};
