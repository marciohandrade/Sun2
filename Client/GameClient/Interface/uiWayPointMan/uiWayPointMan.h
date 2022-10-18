#pragma once

//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiWayPointMan class
//------------------------------------------------------------------------------

#include "uiBaseMan.h"


#pragma pack(push,1)
struct stWayPointData
{
	DWORD m_dwCategory;
	DWORD m_dwNCode;
	sMAP_ENTERENCE_PORTAL_BASE m_EnterencePortal;
};

#pragma pack(pop)

typedef std::multimap<DWORD, stWayPointData>	WAYPOINT_MULTIMAP;
typedef std::pair<DWORD, stWayPointData>		WAYPOINT_PAIR;
typedef WAYPOINT_MULTIMAP::iterator				WAYPOINT_ITER;

class uiWayPointWindow;

struct IM_WAYPOINT_MANAGER 
{
	static const DWORD WAYPOINT;
};

class uiWayPointMan : public uiBaseMan
{
public:
    uiWayPointMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
	virtual void            OnInitialize();
    void                    Clear();

    //--------------------------------------------------------------------------
    // functions
    //
private:
    BOOL                    _LoadUI();
	void					_addCategory(DWORD dwCode);
	void					_processErrorCode(DWORD dwErrorCode);
	bool					_isSaveWayPoint();

public:
    void					ShowWayPointWindow(bool bShow);
	void					SetWayPoint(WORD wIndex);
	void					SetWayPointbyMapCode(MAPCODE code);
	void					ClearPacketStatus()					{ m_bSendPacketToSave = false; m_bSendPacketToLoad = false; }
	
	std::vector<DWORD>		GetCategory()	{ return m_vecCategory; }
	WAYPOINT_MULTIMAP		GetWayPoint()	{ return m_mapWayPoint; }


	void					SEND_CG_WAYPOINT_INFO_LOAD_SYN();
	void					SEND_CG_WAYPOINT_INFO_SAVE_SYN(DWORD dwTargetID);

	DWORD					GetPendingWayPointID()	{ return m_dwPendingWayPointID; }
	void					ClearPendingWayPointID()	{ m_dwPendingWayPointID = 0; }

	void					RqstWayPointSave(DWORD dwTargetID);


    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );
    
    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiWayPointWindow*					m_pUIWayPoint;
	
	std::vector<DWORD>					m_vecCategory;
	WAYPOINT_MULTIMAP					m_mapWayPoint;
	WAYPOINT_TOTAL_INFO					m_waypointInfo;

	bool								m_bSendPacketToSave;
	bool								m_bSendPacketToLoad;

	DWORD								m_dwPendingWayPointID;
};

