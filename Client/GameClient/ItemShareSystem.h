
#pragma once
#include "uibase.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "../PartyLib/PartyDefine.h"

class ItemShareSystem : public uiBase
{
	enum eDIALOG_POS
    {	
		eDIALOG_LIST,
		eDIALOG_BTN,
        eDIVIDE_ITEM_BACK_IMAGE,
        eDIVIDE_ITEM_TEXT,
		ePTBACK_IMG,
		ePTWIN_VIEW_OPT_BTN,
		eCHK_ALL,
		eCHK_FACE,
		eCHK_HP_MP,
		eCHK_BUF,
		eTXT_ALL,
		eTXT_FACE,
		eTXT_HP_MP,
		eTXT_BUF,
#ifdef _NA_000000_20130114_RENEWER_UI
		ePICTURE_P002,
#endif
		DLGPOS_MAX,
    };

	enum OPTCHK_LIST
	{
		OPTCHK_ALL = 0,
		OPTCHK_FACE,
		OPTCHK_HP_MP,
		OPTCHK_BUF,
		PTVIEWOPT_CHECK_MAX,
	};

public:

static DWORD distribution_type_string_code[eITEM_DISTRIBUTION_MAX];
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
static UINT distribution_type_list[eITEM_DISTRIBUTION_MAX];
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

		ItemShareSystem(InterfaceManager * pUIMan );
		~ItemShareSystem();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);

		void				UpdateDialogSize();

		void				SetPartyLeader(BOOL val) { m_bPartyLeader = val;}
		void				SetToggleName(int SelType);
		void				NET_SEND_CG_PARTY_ITEM_DISTIRIBUTE_CHANGE_SYN();
		int					GetItemShareType() {return m_iShareType;}

		void				ShowViewOptControls(bool bSetVal);
		bool				GetShowViewOpt(){ return m_bIsShowPtViewOption; }
		void				SetShowViewOpt(bool val){ m_bIsShowPtViewOption = val; }
		bool				ToggleShowViewOpt(){ return (m_bIsShowPtViewOption = !m_bIsShowPtViewOption); }
		bool				GetAllCheckBtn();
		void				SetAllCheckBtn(bool val);
		void				DoAllCheckBtn(bool val);
        void                ShowDivideItemControls(ENUM_STATEWZ visibility);

		void				CloseItemShareSystemWindow();
		bool				GetDisplayDivOpt(){ return m_bIsDisplayDivOpt; }
		void				SetDisplayDivOpt(bool val){ m_bIsDisplayDivOpt = val; }
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        void                SetPartyObtainRoom(bool val);
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

protected:
		CControlWZ *		getControl(POSTYPE AtPos);
		POSTYPE				getControlIDToPos(WzID wzID);

		void				InitList();

private:
		static WzID	m_wzID[DLGPOS_MAX];

		UI_CONTROL_STATIC_PROPERTY(SelectName , StrToWzID("ST00"));
		CCtrlListWZ *		m_pList;
		BOOL				m_bPartyLeader;
		int					m_iShareType;
		bool				m_bIsShowPtViewOption;
		CCtrlStaticWZ*		m_pTxtCtrls[PTVIEWOPT_CHECK_MAX];
		CCtrlButtonCheckWZ* m_pBtnChecks[PTVIEWOPT_CHECK_MAX];
		CCtrlPictureWZ*	    m_pPicture;
		CCtrlButtonWZ*		m_pBtnOptView;
        CCtrlButtonWZ*		m_pItemDivideButton;
        CCtrlListWZ*		m_pItemDivideList;
        CCtrlPictureWZ*	    m_pItemDivideBack;
        CCtrlStaticWZ*		m_pItemDivideText;
        int				    m_bIsVisibilityDivOpt;

		bool				m_bIsDisplayDivOpt;
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        bool party_obtain_room_;  // ¸ðµÎ È¹µæ ¹æÀÎÁö
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
};

inline CControlWZ * ItemShareSystem::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE ItemShareSystem::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; i++)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}

