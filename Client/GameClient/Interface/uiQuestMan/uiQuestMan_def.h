#ifndef     _UI_QUEST_MAN_DEF_H_
#define     _UI_QUEST_MAN_DEF_H_

struct IM_QUEST_MANAGER
{
	static const DWORD QUEST_COMMON_FORWARD;
	static const DWORD QUEST_REWARD;
	static const DWORD QUEST_LIST_DLG;
	static const DWORD QUEST_LIST_MISSION;
	static const DWORD QUEST_TIME_INFO;
	static const DWORD QUEST_AUTO_ACCETP; 
	static const DWORD QUEST_TIME_QUEST;
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
    static const DWORD QUEST_LIST;
#endif//_NA_004644_20110506_QUEST_NAVIGATION
};


//
//  Packet I/O Boolean
//
enum
{
    ePKBOOL_QUEST_BASE = 0,
	ePKBOOL_QUEST_ACCEPT,
	ePKBOOL_QUEST_INCOMPLETE,
	ePKBOOL_QUEST_COMPLETE,
	ePKBOOL_QUEST_REWARD,
	ePKBOOL_QUEST_FAILED,
	ePKBOOL_QUEST_ABANDON,
	ePKBOOL_QUEST_EVENT_AREA,
	ePKBOOL_QUEST_AUTO_ACEEPT,
	
    ePKBOOL_QUEST_MAX,
};

// color define
#define JOB_TEXT_COLOR								RGBA(255, 255, 255, 255)
#define TITLE_TEXT_COLOR							RGBA(0,0,255,255)
#define TEXT_COLOR_QUEST_LIST_CATEGORY				RGBA(166, 207, 136, 255)
#define TEXT_COLOR_QUEST_LIST_GENERAL				RGBA(255,255,255,255)
#define TEXT_COLOR_QUEST_LIST_COMPLETE				RGBA(255,255,255,255)
#define TEXT_COLOR_QUEST_LIST_MISSION_GENERAL		RGBA(255,255,255,255)
#define TEXT_COLOR_QUEST_LIST_MISSION_COMPLETE		RGBA(255,255,255,255)
#define TEXT_COLOR_MISSION_LIST_GENERAL				RGBA(255,255,255,255)
#define TEXT_BG_COLOR_QUEST_LIST_CATEGORY			RGBA( 200, 200, 200, 0 )
#define TEXT_BG_COLOR_QUEST_LIST_GENERAL			RGBA( 200, 200, 200, 0 )
#define TEXT_BG_COLOR_MISSION_LIST_GENERAL			RGBA(255,255,255,0)
#define TEXT_COLOR_ON_SELECT_LIST					RGBA( 192, 178, 137, 255 )
#define BAR_COLOR_SELECT_LIST						RGBA( 123, 100, 50, 100 )
#define BAR_COLOR_SELECT_LIST_UNUSE					RGBA( 0, 0, 255, 0 )
#define TEXT_COLOR_QUEST_LIST_DEACTIVE				RGBA( 150, 150, 150, 255 )

#define TEXT_COLOR_QUEST_LIST_FAILED				WzColor_RGBA(255,0,0,255)
#define TEXT_COLOR_QUEST_FOR_PREVIEW_NAME			WzColor_RGBA(0,204,0,255)
#define TEXT_COLOR_QUEST_FOR_PREVIEW_CONTENT		WzColor_RGBA(255,255,255,255)
#define TEXT_COLOR_QUEST_FOR_PREVIEW_COMPLETE		WzColor_RGBA(204,102,255,255)
#define	BG_SHADOW_BOARD								WzColor_RGBA(0,0,0,102)


#ifdef _INTERNATIONAL_UI
	#define FONT_MISSION_LIST_GENERAL				StrToWzID("st10")
	#define FONT_QUEST_LIST_GENERAL					StrToWzID("st10")
	#define FONT_QUEST_LIST_CATEGORY				StrToWzID("st10")	//퀘스트 카테고리 폰트의 가독성이 너무 떨어져서 변경함 2008.3.17 by kiki
#else
	#define FONT_QUEST_LIST_GENERAL					StrToWzID("mn12")
	#define FONT_QUEST_LIST_CATEGORY				StrToWzID("mn12")	//퀘스트 카테고리 폰트의 가독성이 너무 떨어져서 변경함 2008.3.17 by kiki
	#define FONT_MISSION_LIST_GENERAL				StrToWzID("mn12")
#endif//_INTERNATIONAL_UI

#define TEXT_COLOR_CONDITION_QUEST_GENERAL			RGBA(150, 150, 150, 255)
#define TEXT_COLOR_CONDITION_QUEST_COMPLETE			RGBA(255, 255, 255, 255)

#define TEXT_HEIGHT_GAP_FOR_PREVIEW					4	// 행간
#define TEXT_MARGIN_FOR_PREVIEW						3	

#define MAX_PROGRESSQUEST_LIST_PAGE_PER_LINE        4

#define MAX_SHOW_QUEST_LEVEL_GAP					15

class uiQuestWindowPosition
{
public:
	uiQuestWindowPosition()
	{
		this->Clear();
	}
	~uiQuestWindowPosition()
	{
	}

	inline
		void    Clear()
	{
		m_bActive = false;
		m_pt.x = 0;
		m_pt.y = 0;
	}

	inline
		bool    IsActivated() const
	{
		return m_bActive;
	}

	inline
		void    SetPoint(const POINT& pt)
	{
		if (m_bActive)
			return;

		m_bActive = true;
		m_pt.x = pt.x;
		m_pt.y = pt.y;
	}

	inline
		const POINT&    GetPoint() const
	{
		return m_pt;
	}

	inline
		void    DeActivated()
	{
		m_bActive = false;
	}

private:
	bool    m_bActive;
	POINT   m_pt;
};

enum eQuestPreviewType
{
	eQUEST_PREVIEW_NONE = -1,
	eQUEST_PREVIEW_TITLE = 0,	// 퀘스트 이름.
	eQUEST_PREVIEW_MISSION,	// 퀘스트 임무.
};

struct sQuestPreview
{
	eQuestPreviewType	m_eType;
	WzColor				m_wcColor;
	SUN_STRING			m_strContent;
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
    BOOL state;
    WORD quest_code;
#endif//_NA_004644_20110506_QUEST_NAVIGATION

};



#endif