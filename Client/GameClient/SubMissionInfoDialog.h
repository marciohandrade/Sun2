#pragma once
#include "solardialog.h"

class BattleLobby;
class BattleRoomEntry;

class SubMissionInfoDialog :
	public SolarDialog
{
public:
	SubMissionInfoDialog(void);
	virtual ~SubMissionInfoDialog(void);

	// 기본 enum
	enum eSUBMISSION_POS
	{
        eSUBMISSION_CHECK_FIELD_1,
        eSUBMISSION_CHECK_FIELD_2,
        eSUBMISSION_CHECK_FIELD_3,
		Dialog_Max
	};

	enum PacketStatus
	{
		PACKET_STATUS_NONE				= 0,
		PACKET_STATUS_SEND_CONFIG		= (2 << 1),	// 제한사항 변경 패킷
	};

	// 기본 virtual methods
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual VOID	update();
	virtual VOID	render();

	virtual VOID	showWindow(BOOL val);


	virtual void     UpdatePosition();

public:
	VOID			ModifyMapCode( CODETYPE MapCode, int index );

protected:
	VOID			updateMapConfig();

	// 기본 proected methods
protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

protected:
	BattleLobby *			m_pBattleLobby;
	BattleRoomEntry	*		m_pBattleRoomEntry;
	CCtrlButtonCheckWZ *	m_pCtrlCheckField[3];

	// 기본 attributes
private:
	static WzID	    m_wzId[Dialog_Max];
	DWORD			m_ValidMapTextColor;
	DWORD			m_InvalidMapTextColor;
};
