#pragma once
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "uiBase.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "FTextRender.h"

class uiSUNRankingStatueInfo : public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_PICTURE_PI00,    // ��� ��ũ
		DLGPOS_PICTURE_PI02,    // ��� ��ũ ���
		DLGPOS_BUTTON_BT00,     // ��� ���� ��ư
		DLGPOS_PICTURE_P007,    // ��� �̸�, �̸�
		DLGPOS_TEXT_ST01,       // ���� �ؽ�Ʈ
		DLGPOS_BUTTON_B999,     // x ��ư

		DIALOG_MAX,
	};
public:
	uiSUNRankingStatueInfo(InterfaceManager* ui_manager);
	~uiSUNRankingStatueInfo(void);

public:
	virtual void Init(CDrawBase* draw_base);
	virtual void OnShowWindow(BOOL is_show);
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();
	virtual void MessageProc( SI_MESSAGE* message );

private:
	bool         OnLButtonClick(SI_MESSAGE* message);

public:
	void UpdateStatuePlayerInfo(const DWORD& statue_player_key);

	// getter
public:
	DWORD get_statue_player_key_() const { return statue_player_key_; }

	// setter
public:
	void set_statue_player_key_(const DWORD& statue_player_key) { statue_player_key_ = statue_player_key; }

private:
	POSTYPE getCtrlIDToPos( WzID wzId );

private:
	CCtrlPictureWZ*   guild_mark_control_;
	CCtrlPictureWZ*   guild_mark_background_control_;
	CCtrlPictureWZ*   statue_name_picture_control_;
	CCtrlButtonWZ*    equip_show_button_control_;
	CCtrlStaticWZ*    statue_description_control_;
	FTextParser       statue_name_ftext_parser_;
	FTextSentence     statue_name_ftext_sentence_;
	FTextParam        statue_name_ftext_param_;
	FTextRender       statue_name_ftext_render_;
	DWORD             statue_player_key_;
	FTextRender       statue_description_ftext_render_;

private:
	static WzID m_wzId[DIALOG_MAX];
};

inline
POSTYPE uiSUNRankingStatueInfo::getCtrlIDToPos( WzID wzId )
{
	for ( POSTYPE i = 0; i < DIALOG_MAX; ++i )
	{
		if ( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return DIALOG_MAX;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
