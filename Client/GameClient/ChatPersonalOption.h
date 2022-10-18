#include "uiBase.h"

#pragma once

class InterfaceManager;
class uiChatMan;

class ChatPersonalOption : public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_LIST,   
        DLGPOS_CANCLE,
		DLGPOS_MAX,
	};

    enum eDIALOG_CONTROLS
    {
        CONTROL_NAME = 0,    // 이름
        CONTROL_WISPER, //귓속말
        CONTROL_ADD_FRIEND,//친구추가
        CONTROL_TRADE,//거래요청
        CONTROL_ADD_PARTY,//파티초대
        CONTROL_ADD_BLOCK,//차단
        
        CONTROL_MAX,
    };

public:
	ChatPersonalOption(InterfaceManager* pUIMan);
	~ChatPersonalOption();
	virtual void			Init(CDrawBase * pDrawBase);
	virtual void			Release();
	virtual void			OnUpdateSolarDialog();
	virtual void			Process(DWORD dwTick);
	virtual void			OnShowWindow(BOOL val);
	virtual void			MessageProc(SI_MESSAGE * pMessage);
	virtual void			NetworkProc(MSG_BASE * pMsg);
    //virtual void            UpdatePosition();
    void                    MoveDefaultPosition();
protected:
	CControlWZ *			getControl(POSTYPE AtPos);
	POSTYPE					getControlIDToPos(WzID wzID);

private:
	BOOL					m_bShow;
	static WzID				m_wzID[DLGPOS_MAX];
	TCHAR					m_szClickedCharName[INTERFACE_STRING_LENGTH+1];

	CCtrlListWZ*			m_pListCtrl;
public:
	void					SetClickedCharName(TCHAR* szVal){StrnCopy(m_szClickedCharName, szVal, INTERFACE_STRING_LENGTH);}
	void					GetClickedCharName(TCHAR* szVal){StrnCopy(szVal, m_szClickedCharName, INTERFACE_STRING_LENGTH);}
    

    static void             CallbackRender(CCtrlListWZ* list_control, CCtrlListWZ::CListSellData* sell_data, int sell_index, RECT& render_area);
};

inline CControlWZ * ChatPersonalOption::getControl(POSTYPE AtPos)
{
	ASSERT(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE ChatPersonalOption::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return DLGPOS_MAX;
}