#pragma once
//------------------------------------------------------------------------------
//
//    �۾���: ������
//    uiCommunityInfo class
//------------------------------------------------------------------------------

#include "uibase.h"

class uiCommunityMan;

class uiCommunityInfo: public uiBase
{
    enum COMMUNITY_INFO_DIALOG_POS
    {
		COMMUNITY_INFO_DIALOG_EXIT = 0,

		COMMUNITY_INFO_DIALOG_LIST,

		COMMUNITY_INFO_DIALOG_BTN_GAME,		// ���� �����ϱ�
		COMMUNITY_INFO_DIALOG_BTN_MSG,		// �����ۼ�
		COMMUNITY_INFO_DIALOG_BTN_REFRESH,	// ���ΰ�ħ
		COMMUNITY_INFO_DIALOG_BTN_CHAT,		// ��ȭ�ϱ�

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
        COMMUNITY_INFO_DIALOG_EDIT_MEMO_INPUT,		// �Է� ����
        COMMUNITY_INFO_DIALOG_BTN_MEMO_ENABLE,		// �Է�
        COMMUNITY_INFO_DIALOG_BTN_MEMO_UPDATE,		// ������ ����
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO

        COMMUNITY_VSCROLL_V000,
        COMMUNITY_PICTURE_P003,

		COMMUNITY_INFO_DIALOG_MAX,
    };

	enum COMMUNITY_INFO_BTNS
	{
		COMMUNITY_INFO_BTN_GAME,		// ���� �����ϱ�
		COMMUNITY_INFO_BTN_MSG,			// �����ۼ�
		COMMUNITY_INFO_BTN_REFRESH,		// ���ΰ�ħ
		COMMUNITY_INFO_BTN_CHAT,		// ��ȭ�ϱ�

		COMMUNITY_INFO_BTN_MAX,
	};

public:
    enum FriendMemoButtonStatus
    {
        kFriendMemoButtonStatus_Disable = 0, 
        kFriendMemoButtonStatus_Modify, 
        kFriendMemoButtonStatus_Lock,   //!< ��Ŷ�� ��ٸ��� �������.
    };

public:
    uiCommunityInfo(InterfaceManager *pUIMan);
    virtual ~uiCommunityInfo();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

protected:
    void                    InitControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiCommunityMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();

public:

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
    static WzID		        m_wzId[COMMUNITY_INFO_DIALOG_MAX];

	void					initControls();
	void					RefreshList();
	void					_addListItem(int idx, COLORREF TextColor, TCHAR *pMsg);
	int						_getMapNameStringIndex(CODETYPE mapCode);
	int						_getMemberStringIndex(eGUILD_DUTY eDuty);
	void					_prepareShow();

    void      UpScroll();
    void      DownScroll();
    void      ThumbScroll();
    void      resetScroll();

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    LEVELTYPE				GetSelectPlayerLevel(TCHAR* character_name);
    BYTE					GetSelectPlayerClass(TCHAR* character_name);
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    GenderType::Value       GetSelectPlayerGender(TCHAR* character_name);
#endif //_NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

    UINT request_uimanager_id() const { return request_uimanager_id_; }
    void set_request_uimanager_id(UINT val) { request_uimanager_id_ = val; }

    void FriendMemo_InputButtonState(FriendMemoButtonStatus status);
    void OnButtonClick_UpdateMemo();
    CCtrlMultilineEditWZ* multilineedit_control() { return multilineedit_control_; }

    //--------------------------------------------------------------------------
    // member vars
    //
private:
	uiCommunityMan*				m_pCommunityMan;

    bool						m_bControlsInitialized;

    CCtrlMultilineEditWZ*       multilineedit_control_;
	CCtrlListWZ*				m_pCommunityInfoList;
	CCtrlButtonWZ*				m_pBtnCtrls[COMMUNITY_INFO_BTN_MAX];
	//bool						m_bShow;

    //! �ش� ���̾�α׸� ȣ���� ���̾�α�
    //! ģ��â, ���â���� ȣ�� ����
    UINT request_uimanager_id_;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCommunityInfo::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < COMMUNITY_INFO_DIALOG_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return COMMUNITY_INFO_DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCommunityInfo::getControl( POSTYPE AtPos )
{
    if (AtPos < COMMUNITY_INFO_DIALOG_MAX)
    {
        return GetControlWZ(m_wzId[AtPos]);
    }

    ASSERT( AtPos < COMMUNITY_INFO_DIALOG_MAX );
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCommunityMan*
uiCommunityInfo::GetManager()
{
    return this->m_pCommunityMan;
}

//------------------------------------------------------------------------------

