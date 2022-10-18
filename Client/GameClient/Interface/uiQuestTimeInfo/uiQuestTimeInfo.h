#ifndef     __uiQuestTimeInfo_H__
#define     __uiQuestTimeInfo_H__
//------------------------------------------------------------------------------
/**
    @class uiQuestTimeInfo

    퀘스트 내용

	@todo
		- 스크롤 작업해야 함.

    (C) 2005 y2jinc 
*/


#include "uiBase.h"
#include "uiQuestTimeInfo_def.h"
#include "ItemComposeListParser.h"
#include "SCItemSlotContainer.h"
#include "Quest.h"
#include "QuestManager_Concrete.h"
#include "interfacemanager.h"
#include "globalfunc.h"
class uiQuestMan;


class uiQuestTimeInfo :
    public uiBase
{
    enum eDIALOG_POS
    {
		DUMMY,
	
        DIALOG_MAX,
    };

    enum
    {
	};

public:
    uiQuestTimeInfo(InterfaceManager *pUIMan);
    virtual ~uiQuestTimeInfo();

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

	void					TimeLimitQuest(QCODE QuestCode);

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
    virtual void            UpdatePosition();


    //--------------------------------------------------------------------------
    // member vars
    //
private:
	BOOL					m_bInitControls;
	TCHAR					m_szText[INTERFACE_STRING_LENGTH];
	TCHAR					m_szResult[INTERFACE_STRING_LENGTH];

	// controls
	UI_CONTROL_STATIC_PROPERTY(QuestName, StrToWzID("T101"));
	UI_CONTROL_STATIC_PROPERTY(QuestTimeInfo, StrToWzID("T001"));

	Quest *					m_pQuest;

};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiQuestTimeInfo::getCtrlIDToPos( WzID wzId )
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
uiQuestTimeInfo::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiQuestTimeInfo::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
#endif  //  __uiQuestTimeInfo_H__

