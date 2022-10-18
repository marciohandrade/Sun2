#pragma once

#include "uiBaseMan.h"

class uiLottoWindow;
class uiLottoNumInput;

struct IM_LOTTO_MANAGER
{
	static const DWORD LOTTO_WINDOW_DLG;
	static const DWORD LOTTO_NUMINPUT_DLG;
};

//숫자오브젝트 정보구조체
typedef struct _LOTTO_INFO
{
	WORD	m_wLottoDrawNum;                // 현재회차
	INT64	m_i64TotalAccum;                // 현재 하임적립액
	BYTE	m_bNumTickets;					// 응모 티켓

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
	
	//로또오브젝트 키값(NPC셋팅시)
	DWORD					m_dwLottoObjKey;				

	bool					m_bWaitPacket;

};
