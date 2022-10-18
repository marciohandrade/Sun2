#pragma once

#include "uiBaseMan.h"

class uiLottoWindow;
class uiLottoNumInput;

struct IM_LOTTO_MANAGER
{
	static const DWORD LOTTO_WINDOW_DLG;
	static const DWORD LOTTO_NUMINPUT_DLG;
};

//���ڿ�����Ʈ ��������ü
typedef struct _LOTTO_INFO
{
	WORD	m_wLottoDrawNum;                // ����ȸ��
	INT64	m_i64TotalAccum;                // ���� ����������
	BYTE	m_bNumTickets;					// ���� Ƽ��

}LOTTO_INFO;


class uiLottoMan : public uiBaseMan
{
public:
	uiLottoMan(InterfaceManager * pUIMAN);

public:
	virtual void            OnInitialize();
		
	virtual void	        NetworkProc( MSG_BASE * pMsg );
	virtual void            NetworkProcWorld(MSG_BASE * pMsg );

	void					SEND_LOTTO_INFO_SYN(BOOL bFlag=TRUE);

	void					LottoMessage(LPCTSTR szMessage,BOOL bCenterMSG=TRUE);

	void					RegisterLottoKey(DWORD dwObjKey){m_dwLottoObjKey=dwObjKey;}

	void					SetWaitPacket(bool bFlag){m_bWaitPacket =bFlag; }
	bool					GetWaitPacket(){return m_bWaitPacket; }

	void					SetLottoNumber(INT64 Number);

	LOTTO_INFO*				GetLottoInfo(){return &m_LottoInfo;}

	BOOL					CheckDistanceNPC();
	
	void					SetLottoActive(BOOL bActive);

private:	
	void					closeAllLottoDlg();

private:
	uiLottoWindow*			m_pUI_LottoWindow;
	uiLottoNumInput*		m_pUI_LottoNumInput;

	LOTTO_INFO				m_LottoInfo;
	
	//�ζǿ�����Ʈ Ű��(NPC���ý�)
	DWORD					m_dwLottoObjKey;				

	bool					m_bWaitPacket;

};
