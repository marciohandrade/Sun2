#pragma once
#include "testcase.h"

class TestCaseBattleZoneUI :
	public TestCase
{
public:
	TestCaseBattleZoneUI(void);
	virtual ~TestCaseBattleZoneUI(void);

	virtual BOOL Init(){return TRUE;}							//	�ʱ�ȭ, �ε�
	virtual BOOL Process( DWORD dwTick ){return TRUE;};			//	���� ���
	virtual BOOL Render( DWORD dwTick ){return TRUE;};			//	���� ������
	virtual void Release(){}						//	����
	virtual void ParsePacket( MSG_BASE * pMsg ){};	//  ��Ʈ��ũ ��Ŷ ó��

	virtual void RenderText(){}
	virtual void ProcessKeyInput(DWORD dwTick);

	virtual BOOL EntMessage(WORD wMessage, int iParam1, int iParam2, int iParam3 = 0, int iParam4 = 0);

};
