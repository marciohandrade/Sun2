#pragma once

#ifdef _NA_008405_20150716_MINIGAME_BINGO

#include "uiBase.h"
#include "uiMiniGameMan/uiMiniGameMan.h"

struct BingoData
{
	BYTE    value;
	POSTYPE pos;
	bool    check;

	BingoData()
	{
		value = 99;
		pos = INVALID_POSTYPE_VALUE;
		check = false;
	}
};

typedef BYTE BINGO_VALUE;

class uiMiniGameMan;
class uiBingoGameDialog : public uiBase
{
	enum UIControlPos
	{
		kPicture_0 = 0,        // 0
		kPicture_1,            // 1
		kPicture_2,            // 2
		kPicture_3,            // 3
		kPicture_4,            // 4
		kPicture_5,            // 5
		kPicture_6,            // 6
		kPicture_7,            // 7
		kPicture_8,            // 8
		kPicture_9,            // 9
		kPicture_10,           // 10
		kPicture_11,           // 11
		kPicture_12,           // 12
		kPicture_13,           // 13
		kPicture_14,           // 14
		kPicture_15,           // 15
		kPicture_16,           // 16
		kPicture_17,           // 17
		kPicture_18,           // 18
		kPicture_19,           // 19
		kPicture_20,           // 20
		kPicture_21,           // 21
		kPicture_22,           // 22
		kPicture_23,           // 23
		kPicture_24,           // 24

		kControl_Text_S000, 

		kControl_Button_BT01,             // 보상 받기

		kControl_CheckButton_C000, 

		kControl_Button_B999,             // x

		kControl_Size, 
	};

public:
	uiBingoGameDialog(InterfaceManager* ui_manager);
	~uiBingoGameDialog(void);

public:
	virtual void Init(CDrawBase* draw_base);
	virtual void Release();

	virtual void OnShowWindow(BOOL is_show);
	virtual void MessageProc(SI_MESSAGE* message);

private:
	bool         OnLButtonClick(SI_MESSAGE* message);

protected:
	POSTYPE      getCtrlIDToPos( WzID wzId );

private:
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();

public:
	void BingoGameInfoSetting(MiniGameBingoInfos& minigame_bingo_infos);

public:
	bool get_is_enable_() { return is_enable_; }
	void set_is_enable(const bool& is_enable) { is_enable_ = is_enable; }

private:
	void DrawBingoLine();

private:
	void ResetBingoCheck();
	void BingoCheck();

private:
	int MakeDigitArray(int value, BYTE* digitarray);

private:
	uiMiniGameMan* mini_game_manager_;

private:
	std::map<POSTYPE, BingoData> bingo_map_;
	bool bingo_check_[12];

	HANDLE number_texture_handle_;

	CCtrlButtonCheckWZ* review_check_button_;

	bool is_enable_;

private:
	static WzID wz_id_[kControl_Size];
};

inline
POSTYPE uiBingoGameDialog::getCtrlIDToPos( WzID wzId )
{
	for ( POSTYPE i = 0; i < kControl_Size; ++i )
	{
		if ( wz_id_[i] == wzId )
		{
			return i;
		}
	}
	return kControl_Size;
}

#endif // _NA_008405_20150716_MINIGAME_BINGO
