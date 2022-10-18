#pragma once
#include "solardialog.h"
//------------------------------------------------------------------------------
/**

    캐릭터 선택 다이얼 로그 
    @author

    @since

    @remarks
        -2005.06.22 캐릭터 선택 작업 추가-지완 
        - 
*/

class CharSelectDialog : public SolarDialog
{
public:
	enum CHARSELECT_POS
	{
		CharSelect_Create,
		CharSelect_Delete,
		CharSelect_Start,
        CharSelect_Exit,
#ifdef	_K2P_ADD_MOVE_TO_SERVER_BUTTON_001020
		CharSelect_ServerList,
#endif
        CharSelect_UnDelete,
        CharSelect_Max
	};

public:
	CharSelectDialog(void);
	virtual ~CharSelectDialog(void);
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );
	virtual VOID	OnUpdateSolarDialog();

	virtual VOID	OnShowWindow(BOOL val);
    void OnCreateCharacter();
    void OnDeleteCharacter();
    void EnableButton(bool val, CHARSELECT_POS type);
    void ShowDeleteButton(bool val);

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

    void            OnSelectStart();
    void            NoticeDeleteDate(Player* selected_character);

	BOOL			m_bSendPacket;
	
private:		
	static WzID		m_wzId[CharSelect_Max];
};
