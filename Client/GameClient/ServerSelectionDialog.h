#ifndef		_SERVERSELECTIONDIALOG_H
#define		_SERVERSELECTIONDIALOG_H
//------------------------------------------------------------------------------
/**
    @class ServerSelectionDialog

    (C) 2005 ckbang
*/
#include "solardialog.h"

class _ServerInfo
{
public:
    _ServerInfo()
    {
        strName.clear();
        Index = 0;
        altIndex = -1;
        curPos = -1;
    }
    
    std::string strName;
    int Index;
    int altIndex;
    int groupNo;
    int curPos;
};

struct _ServerGroup
{
    std::string strName;
    int Index;
};

class ServerSelectionDialog : public SolarDialog
{
    enum eDIALOG_POS
    {	
        DIALOG_SERVER_SELECTED = 0,
        DIALOG_SERVER_LIST,
        DIALOG_MAX,
    };	
public:
    ServerSelectionDialog();
    virtual ~ServerSelectionDialog();

    VOID			Init( CDrawBase * pDrawBase );
    virtual VOID	MessageProc( SI_MESSAGE * pMessage );
    virtual VOID	NetworkProc( MSG_BASE * pMsg ){}

    virtual VOID	update();

protected:
    virtual VOID	showWindow( BOOL val );
    void            ParseServerListFile();
    char            m_szFullPath[MAX_PATH];
    std::vector<_ServerGroup>   vSvGroups;
    std::vector<_ServerInfo>    vSvLists;
    int             m_curServerCounts;
    void            ServerSelected();

protected:
    CControlWZ *	getControl( POSTYPE AtPos );
    POSTYPE			getCtrlIDToPos( WzID wzId );
    static WzID		m_wzId[DIALOG_MAX];
};

//------------------------------------------------------------------------------
#endif	//	_SERVERSELECTIONDIALOG_H
