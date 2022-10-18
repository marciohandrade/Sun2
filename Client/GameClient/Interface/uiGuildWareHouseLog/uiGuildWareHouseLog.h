#pragma once
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"

class uiGuildWareHouseLog : public uiBase
{
public:
	enum eDIALOG_GUILD_WAREHOUSE_LOG
	{
		eDIALOG_GUILD_WAREHOUSE_LOG_EXIT_BTN,
		eDIALOG_GUILD_WAREHOUSE_LOG_OK_BTN,
		eDIALOG_GUILD_WAREHOUSE_LOG_MAIN_LIST,
		eDIALOG_GUILD_WAREHOUSE_LOG_ID_TEXT,   //나중에 버튼으로 변경
		eDIALOG_GUILD_WAREHOUSE_LOG_USED_TEXT, //나중에 버튼으로 변경
		eDIALOG_GUILD_WAREHOUSE_LOG_TIME_TEXT, //나중에 버튼으로 변경
		eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_TEXT,
		eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_RIGHT_BTN,
		eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_LEFT_BTN,
		eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_KEEP_CHECK_BTN,
		eDIALOG_GUILD_WAREHOUSE_LOG_PAGE_TAKE_CHECK_BTN,
		eDIALOG_GUILD_WAREHOUSE_LOG_MAX,
	};

	enum
	{
		eLISTCNT_ID,
		eLISTCNT_USED,
		eLISTCNT_TIME
	};

	enum
	{
		eSTATE_IN,
		eSTATE_OUT,
	};

	uiGuildWareHouseLog(InterfaceManager * pUIMan );
	~uiGuildWareHouseLog();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);
	virtual void		MessageProc(SI_MESSAGE * pMessage);

	void				SetLogList(STRUCT_GUILDWAREHOUSE_LOGINFO sLogInfo);
	void				ClearLogList();

protected:
	CControlWZ *		getControl(POSTYPE AtPos);
	POSTYPE				getControlIDToPos(WzID wzID);
	void				InitControl();

	void				RefreshCurTab(BYTE btState);

public:

private:
	static WzID		m_wzID[eDIALOG_GUILD_WAREHOUSE_LOG_MAX];
	BOOL			m_bShow;

	CCtrlStaticWZ*		m_pctrlPageText;
	CCtrlButtonWZ*		m_pctrlLeftBtn;
	CCtrlButtonWZ*		m_pctrlRightBtn;
	CCtrlButtonCheckWZ* m_pctrlInCheckBtn;
	CCtrlButtonCheckWZ* m_pctrlOutCheckBtn;
	CCtrlListWZ*		m_pctrlList;

	std::list<STRUCT_GUILDWAREHOUSE_LOGINFO> m_listLogListIn;
	std::list<STRUCT_GUILDWAREHOUSE_LOGINFO> m_listLogListOut;
	int				m_iCurInPage;
	int				m_iCurOutPage;

	BYTE			m_btCurState;
};

inline CControlWZ * uiGuildWareHouseLog::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_GUILD_WAREHOUSE_LOG_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildWareHouseLog::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_GUILD_WAREHOUSE_LOG_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_GUILD_WAREHOUSE_LOG_MAX;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
