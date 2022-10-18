#pragma once

#include "uibase.h"
#include "uiQuestMan/uiQuestMan.h"

#define STRSAFE_NO_DEPRECATE
#include "../MemoryINI/AKMemoryIni/MemoryIni.h"

#define QUEST_NAME_CHK_BT_CNT 18
#define QUEST_CHK_PICTURE_CNT 72
#define QUEST_CHK_PICTURE_KIND 4
#define MAX_NAVI_CNT 4

class uiQuestListDlg : public uiBase
{
private:
    enum eQUEST_DISPLAY_TYPE
    {
        eQUEST_DISPLAY_TYPE_ALL = 0,
        eQUEST_DISPLAY_TYPE_PRG,
        eQUEST_DISPLAY_TYPE_END,
        eQUEST_DISPLAY_TYPE_SIZE
    };

    enum eDIALOG_QUEST_LIST
    {
        eDIALOG_QUEST_LIST_EXIT_BTN = 0,
        eDIALOG_QUEST_LIST_MISSION_BTN = 1,
        eDIALOG_QUEST_LIST_QUEST_BTN = 2,

        eDIALOG_QUEST_LIST_CHAPTER_LIST_BTN = 3,

#ifdef _SCJ_111202_UI_RENEWAL
        //eDIALOG_QUEST_LIST_CHAPTER_STRING = 4,
        eDIALOG_QUEST_LIST_CHAPTER_LIST = 4,

        eDIALOG_QUEST_LIST_DP_TYPE_LIST_BTN = 5,
        //eDIALOG_QUEST_LIST_DP_TYPE_STRING = 6,
        eDIALOG_QUEST_LIST_DP_TYPE_LIST = 6,

        // 퀘스트이름 버튼 7~24 (18개)
        eDIALOG_QUEST_LIST_QUEST_NAME_BT_START = 7,
        eDIALOG_QUEST_LIST_QUEST_NAME_BT_END = 24,

        // 체크버튼 4가지색 25~96 (72개)
        eDIALOG_QUEST_LIST_QUEST_CHK_PICTURE_START = 25,
        eDIALOG_QUEST_LIST_QUEST_CHK_PICTURE_END = 96,

        eDIALOG_QUEST_LIST_QUEST_NUM_STRING = 97,

        eDIALOG_QUEST_LIST_SCROLL = 98,

        // 네비게이션 버튼 99~116 (18개)
        eDIALOG_QUEST_LIST_NAVI_BTN_START = 99,
        eDIALOG_QUEST_LIST_NAVI_BTN_END   = 116,

        eDIALOG_QUEST_LIST_CHAPTER_LIST_BTN2 = 117,
        eDIALOG_QUEST_LIST_DP_TYPE_LIST_BTN2    = 118,

        // 체크버튼 배경 119~136 (18개)
        eDIALOG_QUEST_LIST_QUEST_CHK_BG_PICTURE_START = 119,
        eDIALOG_QUEST_LIST_QUEST_CHK_BG_PICTURE_END = 136,

        eDIALOG_QUEST_LIST_MAX = 137,
#else
        eDIALOG_QUEST_LIST_CHAPTER_STRING = 4,
        eDIALOG_QUEST_LIST_CHAPTER_LIST = 5,

        eDIALOG_QUEST_LIST_DP_TYPE_LIST_BTN = 6,
        eDIALOG_QUEST_LIST_DP_TYPE_STRING = 7,
        eDIALOG_QUEST_LIST_DP_TYPE_LIST = 8,

        // 퀘스트이름 버튼 9~26 (18개)
        eDIALOG_QUEST_LIST_QUEST_NAME_BT_START = 9,
        eDIALOG_QUEST_LIST_QUEST_NAME_BT_END = 26,

        // 체크버튼 4가지색 27~98 (72개)
        eDIALOG_QUEST_LIST_QUEST_CHK_PICTURE_START = 27,
        eDIALOG_QUEST_LIST_QUEST_CHK_PICTURE_END = 98,

        eDIALOG_QUEST_LIST_QUEST_NUM_STRING = 99,

        eDIALOG_QUEST_LIST_SCROLL = 100,

        // 네비게이션 버튼 101~118 (18개)
        eDIALOG_QUEST_LIST_NAVI_BTN_START = 101,
        eDIALOG_QUEST_LIST_NAVI_BTN_END   = 118,

        eDIALOG_QUEST_LIST_CHAPTER_LIST_BTN2 = 119,
        eDIALOG_QUEST_LIST_DP_TYPE_LIST_BTN2    = 120,

        eDIALOG_QUEST_LIST_MAX = 121,
#endif //_SCJ_111202_UI_RENEWAL
    };

    struct ScrollbarPosition
    {
        ScrollbarPosition(eQUEST_CATEGORY quest_category, 
                            DWORD qeust_chapter_groupcode, 
                            eQUEST_DISPLAY_TYPE quest_display_type,
                            int scrollbar_position)
        {
            quest_category_ = quest_category;
            qeust_chapter_groupcode_ = qeust_chapter_groupcode;
            quest_display_type_ = quest_display_type;
            scrollbar_position_ = scrollbar_position;
        }
        eQUEST_CATEGORY quest_category() const { return quest_category_; }
        DWORD qeust_chapter_groupcode() const { return qeust_chapter_groupcode_; }
        eQUEST_DISPLAY_TYPE quest_display_type() const { return quest_display_type_; }
        int scrollbar_position() const { return scrollbar_position_; }
        void set_scrollbar_position(int scrollbar_position) { scrollbar_position_ = scrollbar_position; }

    private:
        eQUEST_CATEGORY quest_category_;            //!< 벡터 키값으로 사용
        DWORD qeust_chapter_groupcode_;             //!< 벡터 키값으로 사용
        eQUEST_DISPLAY_TYPE quest_display_type_;    //!< 벡터 키값으로 사용
        int scrollbar_position_; 
    };

//------------------------------------------------------------------------------
public:
	uiQuestListDlg(InterfaceManager* pUIMan);
	~uiQuestListDlg();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
    virtual void		Reset();
	virtual void		OnUpdateSolarDialog();
	virtual void		OnRenderSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);
	virtual void		MessageProc(SI_MESSAGE * pMessage);
#ifdef _INTERNATIONAL_UI
	virtual VOID		MouseOver_MsgProc( SI_MESSAGE * pMessage );
	void				renderToolTip();
#endif//_INTERNATIONAL_UI

	void				SetDlgState(eQUEST_CATEGORY eState){m_eDlgState = eState;}
	eQUEST_CATEGORY		GetDlgState(){return m_eDlgState;}

	uiQuestMan*			GetManager();

	void				RefreshQuestList();

	void				LoadNaviQCode();
	void				SaveNaviQCode();

	void				SetNaviQuest(Quest* pQ);

	DWORD				GetNaviQCodes(BYTE btNaviIdx);

	std::vector<DWORD>& GetMainQuestCodes(){ return m_vecMainQuest; };
	std::vector<DWORD>& GetSubQuestCodes(){ return m_vecSubQuest; };

    void OpenQuest(QCODE quest_code);

    int QuestListCount(const eQUEST_CATEGORY quest_category, 
                        const DWORD quest_chapter_groupCode, 
                        const eQUEST_DISPLAY_TYPE quest_display_type);

    int active_chapter_combocontrol_position(const eQUEST_CATEGORY quest_category) const { return active_chapter_combocontrol_position_[quest_category]; }
    void set_active_chapter_combocontrol_position(const eQUEST_CATEGORY quest_category, const int val) { active_chapter_combocontrol_position_[quest_category] = val; }
    eQUEST_DISPLAY_TYPE active_display_combocontrol_position(const eQUEST_CATEGORY quest_category) const { return active_display_combocontrol_position_[quest_category]; }
    void set_active_display_combocontrol_position(const eQUEST_CATEGORY quest_category, const eQUEST_DISPLAY_TYPE quest_display_type) { active_display_combocontrol_position_[quest_category] = quest_display_type; }
    int last_scrollbar_position(const eQUEST_CATEGORY quest_category,
                                const DWORD qeust_chapter_groupcode,
                                const eQUEST_DISPLAY_TYPE quest_display_type);
    void set_last_scrollbar_position(const eQUEST_CATEGORY quest_category, 
                                    const DWORD qeust_chapter_groupcode, 
                                    const eQUEST_DISPLAY_TYPE quest_display_type, 
                                    const int scrollbar_position);

//------------------------------------------------------------------------------
private:
    void				RefreshNaviBtn();
    void				SetNaviBtn(POSTYPE pos);

    void				SelectDPType();
    void				SelectChapter();

    void				RePosQuestList();

    void				SetQuestCntAndChapterCnt();

    void				updateMouseWheel();

    void				AllHideQuestName();

    CControlWZ *		getControl(POSTYPE AtPos);
    POSTYPE				getControlIDToPos(WzID wzID);
    void				InitControl();

    //------------------------------------------------------------------------------
    static WzID		m_wzID[eDIALOG_QUEST_LIST_MAX];

    BOOL			m_bInit;

    eQUEST_CATEGORY		m_eDlgState;
    eQUEST_DISPLAY_TYPE m_eQDpType;

	CCtrlButtonCheckWZ*	m_pCtrlMissionTabBtn;
	CCtrlButtonCheckWZ*	m_pCtrlQuestTabBtn;

	CCtrlListWZ*		m_pCtrlChapterList;
	CCtrlButtonWZ*		m_pCtrlChapterBtn;
	CCtrlButtonWZ*		m_pCtrlChapterBtn2;
#ifdef _SCJ_111202_UI_RENEWAL
    CCtrlPictureWZ*		quset_check_bg_picture[QUEST_NAME_CHK_BT_CNT];
#else
	CCtrlStaticWZ*		m_pCtrlChapterName;
    CCtrlStaticWZ*		m_pCtrlDPTypeName;
#endif //_SCJ_111202_UI_RENEWAL

	CCtrlListWZ*		m_pCtrlDPTypeList;
	CCtrlButtonWZ*		m_pCtrlDPTypeBtn;
	CCtrlButtonWZ*		m_pCtrlDPTypeBtn2;
	CCtrlButtonWZ*      m_pCtrlQuestNameBtn[QUEST_NAME_CHK_BT_CNT];
	CCtrlPictureWZ*		m_pCtrlQusetChkPicture[QUEST_NAME_CHK_BT_CNT][QUEST_CHK_PICTURE_KIND];
	CCtrlButtonWZ*		m_pCtrlQuestNaviBtn[QUEST_NAME_CHK_BT_CNT];

	CCtrlStaticWZ*		m_pCtrlQuestNum;

	CCtrlVScrollWZ*		m_pCtrlScroll;

	std::vector<DWORD>  m_vecMainQuest;
	std::vector<DWORD>  m_vecSubQuest;

    std::vector<DWORD>  m_MainQChapterCodes;
    std::vector<DWORD>  m_SubQChapterCodes;

	std::vector<int>    m_vecVisCurChapterQuestIdx;

	int					m_iCurChapterIdx;
	DWORD				m_dwChapterCode;

	DWORD				m_dwNaviQCode[MAX_NAVI_CNT];
	int					m_iOldSelNaviIdx;

	TCHAR				m_szFileName[512];
	MemoryIni			m_MemoryINI;
    QCODE open_quest_code_;
    int active_chapter_combocontrol_position_[QUEST_CATEGORY_SIZE];
    eQUEST_DISPLAY_TYPE active_display_combocontrol_position_[QUEST_CATEGORY_SIZE];
    std::vector<ScrollbarPosition> last_scrollbar_position_;
};

inline CControlWZ * uiQuestListDlg::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_QUEST_LIST_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiQuestListDlg::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_QUEST_LIST_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_QUEST_LIST_MAX;
}

