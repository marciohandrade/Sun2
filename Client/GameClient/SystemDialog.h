//-----------------------------------------------------------------------
/// SystemDialog
/**
	@author
			유재영 < y2jinc@webzen.co.kr >
	@since

	@remarks
			- 		
	@history
		- 리소스 수정 2005. 9. 21
*/

#pragma once
#include "uiBase.h"

class SystemDialog : public uiBase
{
public:
	enum SYSTEM_POS
	{
		System_BACK_TO_THE_VILLAGE = 0,
		System_BACK_TO_THE_CHARACTER_SELECT,
#ifdef _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING
#else
		System_BACK_TO_THE_SERVER_SELECT,
#endif        
		System_OPTION_SETTING,
		System_CREDIT,
		System_GAME_EXIT,        
		System_EXIT,
		System_GO_OUT_DOMINATION,
		System_MAX
	};

	enum SYSTEM_PACKET_STATUS
	{
		SYSTEM_PACKET_NONE = 0,
		BACK_TO_THE_VILLAGE = 1,
		BACK_TO_THE_CHARACTER_SELECT = (2 << 1),
	};

public:
	SystemDialog(void);
	virtual ~SystemDialog(void);

public:
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

public:
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );
			VOID	ServerSelectStandingBy( VOID );
			VOID	ProcessServerSelect( VOID );
			VOID	CharacterSelectStandingBy( VOID );
			VOID	ProcessCharacterSelect( VOID );

public:
	virtual VOID	Update();
	//void			UpdatePosition();

	void			ResetPacketStatus() { this->m_packetStatus = SYSTEM_PACKET_NONE; }
	void			AddPacketStatus(SYSTEM_PACKET_STATUS ePacketStatus);
	void			RemovePacketStatus(SYSTEM_PACKET_STATUS ePacketStatus);
	DWORD			GetPacketStatus() { return this->m_packetStatus; }
	void			CheckModifyOption();
protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
    VOID			OnShowWindow(BOOL val);

private:		
    static WzID     m_wzId[System_MAX];
};
