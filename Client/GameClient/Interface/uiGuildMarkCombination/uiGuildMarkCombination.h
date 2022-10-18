#ifndef _uiGuildMarkCombination_h_
#define _uiGuildMarkCombination_h_
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uibase.h"

class uiGuildMan;

class uiGuildMarkCombination : public uiBase
{

#define MAX_GUILD_MARK 26
#define INVALID_GUILD_MARK_INDEX -1
#define MAX_FILTER_CHECK 3

	struct stGUILD_MARK_ITEM
	{
		CCtrlButtonWZ * pButton[MAX_GUILD_MARK];		// 길드 마크 Image 버튼
		CCtrlStaticWZ * pStaticPage;					// 현재 페이지
		CCtrlButtonCheckWZ * pCheck[MAX_FILTER_CHECK];	// 필터링 Check
		int				index[MAX_GUILD_MARK];			// 길드 마크 Image에 할당된 GuildMark.txt 인덱스
		int				iSelIndex;						// 현재 선택된 길드 마크 index
		int				iPage;							// 현재 페이지
		int				iMaxPage;						// Max 페이지
		int				iCurFiltter;					// 현재 적용된 필터
		int				iType;							// 문양 or 패턴?
	};

	enum eDIALOG_POS
    {
		DLGPOS_BTN_TARGET_ITEM,
		
		DLGPOS_TXT_BACKGROUND_PAGE,
		DLGPOS_BTN_BACKGROUND_LEFT,
		DLGPOS_BTN_BACKGROUND_RIGHT,

		DLGPOS_PIC_BACKGROUND1,
		DLGPOS_PIC_BACKGROUND2,
		DLGPOS_PIC_BACKGROUND3,
		DLGPOS_PIC_BACKGROUND4,
		DLGPOS_PIC_BACKGROUND5,
		DLGPOS_PIC_BACKGROUND6,
		DLGPOS_PIC_BACKGROUND7,
		DLGPOS_PIC_BACKGROUND8,
		DLGPOS_PIC_BACKGROUND9,
		DLGPOS_PIC_BACKGROUND10,
		DLGPOS_PIC_BACKGROUND11,
		DLGPOS_PIC_BACKGROUND12,
		DLGPOS_PIC_BACKGROUND13,
		DLGPOS_PIC_BACKGROUND14,
		DLGPOS_PIC_BACKGROUND15,
		DLGPOS_PIC_BACKGROUND16,
		DLGPOS_PIC_BACKGROUND17,
		DLGPOS_PIC_BACKGROUND18,
		DLGPOS_PIC_BACKGROUND19,
		DLGPOS_PIC_BACKGROUND20,
		DLGPOS_PIC_BACKGROUND21,
		DLGPOS_PIC_BACKGROUND22,
		DLGPOS_PIC_BACKGROUND23,
		DLGPOS_PIC_BACKGROUND24,
		DLGPOS_PIC_BACKGROUND25,
		DLGPOS_PIC_BACKGROUND26,


		DLGPOS_TXT_PATTERN_PAGE,
		DLGPOS_BTN_PATTERN_LEFT,
		DLGPOS_BTN_PATTERN_RIGHT,
		DLGPOS_PIC_PATTERN1,
		DLGPOS_PIC_PATTERN2,
		DLGPOS_PIC_PATTERN3,
		DLGPOS_PIC_PATTERN4,
		DLGPOS_PIC_PATTERN5,
		DLGPOS_PIC_PATTERN6,
		DLGPOS_PIC_PATTERN7,
		DLGPOS_PIC_PATTERN8,
		DLGPOS_PIC_PATTERN9,
		DLGPOS_PIC_PATTERN10,
		DLGPOS_PIC_PATTERN11,
		DLGPOS_PIC_PATTERN12,
		DLGPOS_PIC_PATTERN13,
		DLGPOS_PIC_PATTERN14,
		DLGPOS_PIC_PATTERN15,
		DLGPOS_PIC_PATTERN16,
		DLGPOS_PIC_PATTERN17,
		DLGPOS_PIC_PATTERN18,
		DLGPOS_PIC_PATTERN19,
		DLGPOS_PIC_PATTERN20,
		DLGPOS_PIC_PATTERN21,
		DLGPOS_PIC_PATTERN22,
		DLGPOS_PIC_PATTERN23,
		DLGPOS_PIC_PATTERN24,
		DLGPOS_PIC_PATTERN25,
		DLGPOS_PIC_PATTERN26,

		DLGPOS_CHECK_BACKGROUND_CIRCLE,
		DLGPOS_CHECK_BACKGROUND_SHIELD,
		DLGPOS_CHECK_BACKGROUND_ETC,
		DLGPOS_CHECK_PATTERN_FIGURE,
		DLGPOS_CHECK_PATTERN_SYMBOL,
		DLGPOS_CHECK_PATTERN_ETC,

		DLGPOS_BTN_OK,
		DLGPOS_BTN_CANCLE,
		DLGPOS_BTN_EXIT,

		DLGPOS_MAX,
    };

public:
		uiGuildMarkCombination(InterfaceManager * pUIMan );
		~uiGuildMarkCombination();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		OnUpdateSolarDialog();
		virtual void		OnRenderSolarDialog();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);
		void				NET_SEND_CG_GUILD_GUILDMARK_REGISTER_SYN();

protected:
		CControlWZ *		getControl(POSTYPE AtPos);
		POSTYPE				getControlIDToPos(WzID wzID);
		void				RenderGuildMark(stGUILD_MARK_ITEM * pGuildMarkItem);
		void				RenderTargetGuildMark();
		uiGuildMan *		GetGuildManager(){ return m_pGuildMan;}

		void				RegisterGuildMarkData(stGUILD_MARK_ITEM * pstGuildMarkItem);
		void				InitstGuildMarkItem();
		void				SetButtonCheck(stGUILD_MARK_ITEM * pstGuildMarkItem);

private:
		static WzID	m_wzID[DLGPOS_MAX];
		bool		m_bShow;
		stGUILD_MARK_ITEM	m_stBackGroundItem;
		stGUILD_MARK_ITEM	m_stPatternItem;
		uiGuildMan *		m_pGuildMan;
		CCtrlButtonWZ *		m_pTargetMark;
};

inline CControlWZ * uiGuildMarkCombination::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiGuildMarkCombination::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#endif // _uiGuildMarkCombination_h_
