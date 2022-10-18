

#ifndef     __uiQuestAutoAcceptAlram_H__
#define     __uiQuestAutoAcceptAlram_H__
//------------------------------------------------------------------------------
/**
    @class uiQuestAutoAcceptAlram

    퀘스트 자동 수락 알림.

    (C) 2005 y2jinc 
*/


#include "uiBase.h"
#include "uiQuestAutoAcceptAlram_def.h"
#include "ItemComposeListParser.h"
#include "SCItemSlotContainer.h"
#include "Quest.h"
#include "QuestManager_Concrete.h"

class uiQuestMan;


class uiQuestAutoAcceptAlram :
    public uiBase
{
    enum eDIALOG_POS
    {
		eDLGPOS_ALRAM_BTN, 
	
        DIALOG_MAX,
    };

    enum
    {
	};

public:
    uiQuestAutoAcceptAlram(InterfaceManager *pUIMan);
    virtual ~uiQuestAutoAcceptAlram();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    void                    InitControls();
    bool                    CheckControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiQuestMan*				GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
public:
	void					ClearInfo();

protected:
	void					initInfo();
	void					updateInfo();

	//--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();

    
protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
	virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
	virtual VOID			MouseOver_MsgProc( SI_MESSAGE * pMessage );
    //virtual void            UpdatePosition();


    //--------------------------------------------------------------------------
    // member vars
    //
private:
	BOOL					m_bInitControls;

	util::Timer 			m_BlinkTimer;
	// controls
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiQuestAutoAcceptAlram::getCtrlIDToPos( WzID wzId )
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
uiQuestAutoAcceptAlram::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiQuestAutoAcceptAlram::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
#endif  //  __uiQuestAutoAcceptAlram_H__

