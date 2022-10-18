#pragma once

//#include <Singleton.h>
//
//class SolarDialog;
//
//struct UISuspendFocus
//{
//	UISuspendFocus(WzID DialogID, WzID ControlID) : 
//		wzDialogID(DialogID), wzControlID(ControlID) {}
//
//	~UISuspendFocus() {}
//
//	WzID		wzDialogID;
//	WzID		wzControlID;
//};
//
//typedef std::deque<UISuspendFocus *>::iterator	FOCUS_ITR;
//
//class UISuspendFocusMgr : public Singleton< UISuspendFocusMgr >
//{
//public:
//	UISuspendFocusMgr(void);
//	virtual	~UISuspendFocusMgr(void);
//
//	BOOL		ChangeFocusTransact(SolarDialog * pDlg, WzID wzControlID);
//	void		RequestFocusTransact(SolarDialog * pDlg);
//
//	void		RemoveFocus(SolarDialog * pDlg);
//	void		ForceChangeFocus(SolarDialog * pDlg, WzID wzControlID); 
//	void		ClearFocus();
//
//	int			GetSize() const { return m_SuspendFocusDeque.size(); }
//
//protected:
//	BOOL		isValidFocus(SolarDialog * pDlg, WzID ControlID);
//
//	FOCUS_ITR	find( WzID DialogID);
//	BOOL		add(WzID DialogID, WzID ControlID );
//	void		remove( WzID DialogID);
//	void		update( WzID DialogID, WzID ControlID );
//
//	void		returnPendingFocus();
//
//private:
//	std::deque<UISuspendFocus *>			m_SuspendFocusDeque;
};
