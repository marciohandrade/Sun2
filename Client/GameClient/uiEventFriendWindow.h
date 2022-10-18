#pragma once
//------------------------------------------------------------------------------
//
//    �۾���: �̵���
//    uiEventItemSelect class
//------------------------------------------------------------------------------
#include "uibase.h"
class uiEventMan;

#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"

struct FRIENDEVENT_INFO_LIST
{
	CHARGUID FriendGUID;
	char ptszFriendName[MAX_CHARNAME_LENGTH+1];
	WORD RewardType;//����Ÿ�� 20,40,60,80 ����
	BYTE IsReward;// ���󿩺�
};

class uiEventFriend: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_EXIT,		//�����ư
		DLGPOS_TXT_ITEM,		//��� (��ǰ�����)	
		DLGPOS_TXT_COUNT,		//����Ƚ�� (��ǰ�����)
		DLGPOS_TXT_ITEM2,		//��� (���������)
		DLGPOS_BTN_OK,			//Ȯ��
		DLGPOS_BTN_CANCEL,		//���

		DLGPOS_TEXT_TITLE,		//����
        DLGPOS_TXT_MISSION,     //�̼�
		DLGPOS_LIST,			//����Ʈ(������)
		DLGPOS_TXT,				//����
        DLGPOS_VSCROLL,         //��ũ�ѹ�
        DLGPOS_VSCROLL_COMMENT,     //���� ��ũ�ѹ�
#ifdef _NA_000000_20130114_RENEWER_UI
		eControl_Picture_P001,
		eControl_Picture_P004,
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
        DIALOG_MAX,
    };

	enum
    {
		TXT_ITEM,
		TXT_COUNT,
		TXT_ITEM2,
		POS_TXT_SIZE = DLGPOS_TXT_ITEM2 - DLGPOS_TXT_ITEM + 1,
    };
	
	const int MAX_VISIBLE_LIST_NUMBER;
public:
    uiEventFriend(InterfaceManager *pUIMan);
    virtual ~uiEventFriend();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
	void					InsertList(InvitateFriendRewardInfo *pRewardInfo);

protected:
    void                    InitControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiEventMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();
	void					_refreshList();

public:
    void					_initList();
	void					UpdateList();

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
    virtual void            OnRenderSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getControlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:

    void    UpdateMouseWheel_Comment();

    FTextParser&    ftext_parser()  { return ftext_parser_; }
    FTextSentence&  ftext_sentence(){ return ftext_sentence_; }
    FTextParam&     ftext_param()   { return ftext_param_; }
    FTextRender&    ftext_render()  { return ftext_render_; }

    FTextParser     ftext_parser_;
    FTextSentence   ftext_sentence_;
    FTextParam      ftext_param_;
    FTextRender     ftext_render_;

    uiEventMan*				m_pEventMan;
    CCtrlListWZ*            m_pListControl;
	CCtrlStaticWZ*          m_pTxtControl[POS_TXT_SIZE];
    CCtrlVScrollWZ* 		list_scroll_;
    int                     search_index_;
	bool                    m_bControlsInitialized;
	int						scroll_current_value_;
	int						scroll_value_max_;
	std::vector<FRIENDEVENT_INFO_LIST>		m_vecEventList;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiEventFriend::getControlIDToPos( WzID wzId )
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
uiEventFriend::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiEventMan*
uiEventFriend::GetManager()
{
    return this->m_pEventMan;
}

//------------------------------------------------------------------------------
