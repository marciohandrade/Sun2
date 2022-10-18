#ifndef     _SKILL_QUEUE_DIALOG_H
#define     _SKILL_QUEUE_DIALOG_H
//------------------------------------------------------------------------------
/**
@class SkillQueueDialog
*/
#include "uibase.h"

class InterfaceManager;

class SkillQueueDialog :public uiBase
{
    enum eDIALOG_POS
    {
        DIALOG_PICTURE0 = 0,
        DIALOG_PICTURE1,
        DIALOG_PICTURE2,
        DIALOG_MAX,
    };


public:
    SkillQueueDialog(InterfaceManager * pUIman);
    virtual ~SkillQueueDialog();

    virtual void            Init( CDrawBase * pDrawBase );
    virtual void            Release();
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
    virtual void	        NetworkProc( MSG_BASE * pMsg ) {};      // Pure

	RECT                    GetSkillRect(int index);
    virtual void MoveDefaultWindowPos();
protected:
    virtual void	        OnShowWindow( BOOL val );
   
protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];
private:
    DWORD					m_dwSkillId[DIALOG_MAX];
    bool is_once_position_init_;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
SkillQueueDialog::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
SkillQueueDialog::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
#endif  //  _SKILL_QUEUE_DIALOG_H
