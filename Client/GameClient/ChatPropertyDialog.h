#include "uiBase.h"

#pragma once

class InterfaceManager;
class uiChatMan;

class ChatPropertyDialog : public uiBase
{
	enum eDIALOG_POS
    {
		DLGPOS_CHECK_SYSTEM,
		DLGPOS_CHECK_SHOUT,
		DLGPOS_CHECK_GENERAL,
		DLGPOS_CHECK_WHISPER,
		DLGPOS_CHECK_PARTY,
		DLGPOS_CHECK_GUILD,
		DLGPOS_CHECK_CHANNEL,
		DLGPOS_BTN_EXIT,
		DLGPOS_MAX,
    };

	enum
	{
		DLGPOS_CHECK_MAX = DLGPOS_BTN_EXIT,
	};

public:
		ChatPropertyDialog(InterfaceManager* pUIMan);
		~ChatPropertyDialog();
		virtual void			Init(CDrawBase * pDrawBase);
		virtual void			Release();
		virtual void			Process(DWORD dwTick);
		virtual void			OnShowWindow(BOOL val);
		virtual void			MessageProc(SI_MESSAGE * pMessage);
		virtual void			NetworkProc(MSG_BASE * pMsg);
        //virtual void            UpdatePosition();
public:
		void					SetPropertyDlgFilter(int filterType , int filterValue);
        void                    MoveDefaultPosition();
protected:
		CControlWZ *			getControl(POSTYPE AtPos);
		POSTYPE					getControlIDToPos(WzID wzID);
		CCtrlButtonCheckWZ *	m_pCheck[DLGPOS_CHECK_MAX];

		// 체크 버튼 업데이트
		void					updateAllCheckButton();
		void					ComposeBitFilter();
		void					_setEachBitFilter(int id , int bit);
		void					_updatEachCheckButton(int id, int bit);
		void					ForceSettingCheckButton();

private:
		static WzID	m_wzID[DLGPOS_MAX];
		uiChatMan *				m_pChatMan;
		int						m_iFilterType;
		int						m_iFilterValue;
		int						m_iGapOfDialogYPosition;

};

inline CControlWZ * ChatPropertyDialog::getControl(POSTYPE AtPos)
{
	ASSERT(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE ChatPropertyDialog::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}
