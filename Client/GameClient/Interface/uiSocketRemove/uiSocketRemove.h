#ifndef _uiSocketRemove_h_
#define _uiSocketRemove_h_

#include "uibase.h"

class uiSocketRemove : public uiBase
{
	enum eDIALOG_POS
    {
		eDIALOG_BTN_EXIT,
		eDIALOG_BTN_OK,
		eDIALOG_BTN_CANCLE,
		eDIALOG_TXT_CAPTION,
		eDIALOG_LIST,
		DLGPOS_MAX,
    };

	enum
	{
		BTN_MAX = eDIALOG_TXT_CAPTION,
	};

	enum
	{
		SOCKET_LOW = 0,
		SOCKET_HIGH,
	};

    enum
    {
        SOCKET_STATE_MAX = 5,
    };

	struct SocketState
	{
		int socketType;
		int socketPos;
	};

public:
		uiSocketRemove(InterfaceManager *pUIMan);
		~uiSocketRemove();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		OnUpdateSolarDialog();
        virtual void		OnRenderSolarDialog();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);
		BOOL				IsAvaliableRemoveSocket(SLOTIDX SlotIdx, POSTYPE TargetPos);
		BOOL				InsertSocketOption(SLOTIDX SlotIdx, POSTYPE TargetPos);
		void				NET_SEND_CG_ITEM_SOCKET_EXTRACT_SYN();
	
protected:
		CControlWZ *		getControl(POSTYPE AtPos);
		POSTYPE				getControlIDToPos(WzID wzID);

private:
		static WzID	m_wzID[DLGPOS_MAX];
		CCtrlButtonWZ *		m_pButton[BTN_MAX];
		CCtrlListWZ	*		m_pList;
		CCtrlStaticWZ *		m_pStatic;
		SocketState			m_iSocketState[SOCKET_STATE_MAX];
		BOOL				m_bShow;
		POSTYPE				m_TargetSocketItemPos;
		BOOL				m_bSendPacket;
        BYTE zard_grades_[SOCKET_MAX];
        TCHAR socket_names[SOCKET_STATE_MAX][INTERFACE_STRING_LENGTH];
};

inline CControlWZ * uiSocketRemove::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiSocketRemove::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}
#endif // _uiSocketRemove_h_