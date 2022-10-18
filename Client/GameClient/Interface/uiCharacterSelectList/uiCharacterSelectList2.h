#pragma once
#include "uiBase.h"
#include "FTextRender.h"

#ifdef _NA_000000_20130114_RENEWER_UI 

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
const int MAXCHARLIST = 8;
const int MAXKIND = 8;
#else
const int MAXCHARLIST = 7;
const int MAXKIND = 7;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

struct stSlotCharacterList
{
	//총캐릭터 + 빈슬롯
	CControlWZ* charater_Image_[MAXCHARLIST+1];
	//총 족족 + 빈슬롯
	CControlWZ* kind_Image_[MAXKIND+1];
	CControlWZ* lock_Image;
	CControlWZ*	Back_Check_;

	FTextRender character_info_text_;

	bool		lock_slot_;

	int			save_char_iamge_index_;
	int			save_kind_image_index_;

	//클래스, KIND
	DWORD			save_character_index_[MAXCHARLIST+1];
	//클래스, KIND
	DWORD			save_Kind_index_[MAXKIND+1];

	DWORD			save_back_check_;

	stSlotCharacterList()
	{
		for (int index = 0 ; index < MAXCHARLIST+1 ; ++index)
		{
			charater_Image_[index] = NULL;
			kind_Image_[index] = NULL;
		}

		kind_Image_[MAXKIND] = NULL;
		lock_Image = NULL;
		Back_Check_ = NULL;
		lock_slot_ = false;

		save_char_iamge_index_ = -1;
		save_kind_image_index_ = -1;

		memset(save_character_index_,0,sizeof(int)*(MAXCHARLIST+1));
		memset(save_Kind_index_,0,sizeof(int)*(MAXKIND+1));



	}

	void all_show_character_Image(ENUM_STATEWZ state)
	{
		for (int index = 0 ; index < MAXCHARLIST ; ++index)
		{
			charater_Image_[index]->ShowWindowWZ(state);
		}
	}

	void all_show_Kind_Image(ENUM_STATEWZ state)
	{
		for (int index = 0 ; index < MAXKIND ; ++index)
		{
			kind_Image_[index]->ShowWindowWZ(state);
		}
	}

	void show_character_Image(ENUM_STATEWZ state, int index)
	{
		charater_Image_[index]->ShowWindowWZ(state);

		save_char_iamge_index_ = index;
	}

	void show_Kind_Image(ENUM_STATEWZ state, int index)
	{
		kind_Image_[index]->ShowWindowWZ(state);

		save_kind_image_index_ = index;
	}

	void show_control_check(bool isState)
	{
		//현재 활성화된 이미지만 체크 표시

		if(save_char_iamge_index_ != -1)
		{
			CCtrlButtonCheckWZ* CheckBox = static_cast<CCtrlButtonCheckWZ*>(charater_Image_[save_char_iamge_index_]);
			if(CheckBox)
			{	
				CheckBox->SetCheckState(isState);
			}
		}

		if(save_kind_image_index_ != -1)
		{
			CCtrlButtonCheckWZ* CheckBox = static_cast<CCtrlButtonCheckWZ*>(kind_Image_[save_kind_image_index_]);
			if(CheckBox)
			{
				CheckBox->SetCheckState(isState);
			}
		}

		if(Back_Check_)
		{
			CCtrlButtonCheckWZ* CheckBox = static_cast<CCtrlButtonCheckWZ*>(Back_Check_);
			if(CheckBox)
			{	
				CheckBox->SetCheckState(isState);
			}
		}
	}

	bool isRangeKind(DWORD Value)
	{
		for(int i = 0 ; i < MAXKIND+1 ; ++i)
		{
			if (Value == save_Kind_index_[i])
			{
				return true;
			}
		}

		return false;
	}

	bool isRangeCharacter(DWORD Value)
	{
		for(int i = 0 ; i < MAXCHARLIST+1 ; ++i)
		{
			if (Value == save_character_index_[i])
			{
				return true;
			}
		}

		return false;
	}

	bool isCheckBack(DWORD Value)
	{
		return (save_back_check_ == Value);
	}

	void allClearData()
	{

	}
};

class uiCharacterSelectList2 : public uiBase
{
public:
	enum
	{
		//한슬롯에 붙는 컨트롤 리스트 아이디
		KControl_0Slot_Max,

		kControl_Check_C002,
		kControl_Check_C042,
		kControl_Check_C041,
		kControl_Check_C031,
		kControl_Check_C021,
		kControl_Check_C000,
        kControl_Check_C141,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C054,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P000,
		kControl_Max_Step_1,

		kControl_Check_C101,
		kControl_Check_C091,
		kControl_Check_C063,
		kControl_Check_C062,
		kControl_Check_C061,
		kControl_Check_C007,
        kControl_Check_C151,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C124,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P001,
		
		kControl_Check_C006,
		KControl_1Slot_Max, //한슬롯에 들어가는 최대치

		kControl_Check_C003,
		kControl_Check_C070,
		kControl_Check_C057,
		kControl_Check_C036,
		kControl_Check_C026,
		kControl_Check_C016,
        kControl_Check_C142,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C055,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P002,
		kControl_Max_Step_2,

		kControl_Check_C102,
		kControl_Check_C092,
		kControl_Check_C082,
		kControl_Check_C073,
		kControl_Check_C064,
		kControl_Check_C052,
        kControl_Check_C152,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C161,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P023,

		kControl_Check_C001,
		KControl_2Slot_Max, //한슬롯에 들어가는 최대치
		
		kControl_Check_C004,
		kControl_Check_C071,
		kControl_Check_C058,
		kControl_Check_C037,
		kControl_Check_C027,
		kControl_Check_C017,
        kControl_Check_C143,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C056,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P003,
		kControl_Max_Step_3,

		kControl_Check_C133,
		kControl_Check_C125,
		kControl_Check_C109,
		kControl_Check_C099,
		kControl_Check_C089,
		kControl_Check_C080,
        kControl_Check_C153,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C162,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P012,

		kControl_Check_C008,
		KControl_3Slot_Max, //한슬롯에 들어가는 최대치

		kControl_Check_C005,
		kControl_Check_C072,
		kControl_Check_C059,
		kControl_Check_C038,
		kControl_Check_C028,
		kControl_Check_C018,
        kControl_Check_C144,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C117,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P004,
		kControl_Max_Step_4,

		kControl_Check_C134,
		kControl_Check_C126,
		kControl_Check_C110,
		kControl_Check_C100,
		kControl_Check_C090,
		kControl_Check_C081,
        kControl_Check_C154,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C163,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P013,

		kControl_Check_C009,
		KControl_4Slot_Max, //한슬롯에 들어가는 최대치

		kControl_Check_C047,
		kControl_Check_C074,
		kControl_Check_C060,
		kControl_Check_C039,
		kControl_Check_C029,
		kControl_Check_C019,
        kControl_Check_C145,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C118,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P006,
		kControl_Max_Step_5,

		kControl_Check_C135,
		kControl_Check_C127,
		kControl_Check_C111,
		kControl_Check_C103,
		kControl_Check_C093,
		kControl_Check_C083,
        kControl_Check_C155,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C168,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P014,

		kControl_Check_C010,
		KControl_5Slot_Max, //한슬롯에 들어가는 최대치

		kControl_Check_C048,
		kControl_Check_C075,
		kControl_Check_C065,
		kControl_Check_C040,
		kControl_Check_C030,
		kControl_Check_C020,
        kControl_Check_C146,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C119,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P007,
		kControl_Max_Step_6,

		kControl_Check_C136,
		kControl_Check_C128,
		kControl_Check_C112,
		kControl_Check_C104,
		kControl_Check_C094,
		kControl_Check_C084,
        kControl_Check_C156,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C169,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P015,

		kControl_Check_C011,
		KControl_6Slot_Max, //한슬롯에 들어가는 최대치

		kControl_Check_C049,
		kControl_Check_C076,
		kControl_Check_C066,
		kControl_Check_C043,
		kControl_Check_C032,
		kControl_Check_C022,
        kControl_Check_C147,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C120,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P008,
		kControl_Max_Step_7,

		kControl_Check_C137,
		kControl_Check_C129,
		kControl_Check_C113,
		kControl_Check_C105,
		kControl_Check_C095,
		kControl_Check_C085,
        kControl_Check_C157,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C166,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P016,

		kControl_Check_C012,
		KControl_7Slot_Max, //한슬롯에 들어가는 최대치

		kControl_Check_C050,
		kControl_Check_C077,
		kControl_Check_C067,
		kControl_Check_C044,
		kControl_Check_C033,
		kControl_Check_C023,
        kControl_Check_C148,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C121,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P009,
		kControl_Max_Step_8,

		kControl_Check_C138,
		kControl_Check_C130,
		kControl_Check_C114,
		kControl_Check_C106,
		kControl_Check_C096,
		kControl_Check_C086,
        kControl_Check_C158,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C167,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P017,

		kControl_Check_C013,
		KControl_8Slot_Max, //한슬롯에 들어가는 최대치

		kControl_Check_C051,
		kControl_Check_C078,
		kControl_Check_C068,
		kControl_Check_C045,
		kControl_Check_C034,
		kControl_Check_C024,
        kControl_Check_C149,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C122,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P010,
		kControl_Max_Step_9,

		kControl_Check_C139,
		kControl_Check_C131,
		kControl_Check_C115,
		kControl_Check_C107,
		kControl_Check_C097,
		kControl_Check_C087,
        kControl_Check_C159,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C165,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P018,

		kControl_Check_C014,
		KControl_9Slot_Max, //한슬롯에 들어가는 최대치

		kControl_Check_C053,
		kControl_Check_C079,
		kControl_Check_C069,
		kControl_Check_C046,
		kControl_Check_C035,
		kControl_Check_C025,
        kControl_Check_C150,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C123,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P011,
		kControl_Max_Step_10,

		kControl_Check_C140,
		kControl_Check_C132,
		kControl_Check_C116,
		kControl_Check_C108,
		kControl_Check_C098,
		kControl_Check_C088,
        kControl_Check_C160,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Check_C164,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		kControl_Picture_P019,

		kControl_Check_C015,
		KControl_10Slot_Max, //한슬롯에 들어가는 최대치

		//disable slot
		kControl_Picture_P021,
		kControl_Picture_P022,
		kControl_Picture_P024,
		kControl_Picture_P025,

		kControl_Max,
	};

	enum
	{
		kMaxListSlot = 10,
	};

public:
    uiCharacterSelectList2(InterfaceManager* ui_manager);
    virtual ~uiCharacterSelectList2();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE * message);

    void SetServerName();
    void SetSelectedCharacterSlotIndex(int slot_index);
    void SetSelectedCharacterListIndex(int slot_index);
    void CharacterListUp();
    int GetSelectedCharacterSlotIndex();

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

private:
    void RefreshSelectList();
    void RegisterControl();
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnLButtonDClick(SI_MESSAGE* message);

    void ShowListSlotByCount(int slot_count);
    void SetCharacterSlotInfo(int list_index, 
                              TCHAR* character_name, 
                              BYTE delete_check,
                              int level, 
                              BYTE class_type, 
                              MAPCODE current_mapcode,
                              TCHAR* day_for_delete,
							  BYTE Gender);

    void SetCharacterSlot(int list_index);
    void OnSelectPrevCharacter();
    void OnSelectNextCharacter();
    int GetListIndexFromSlotIndex(int slot_index);
	void CheckModifyDoubleClick(DWORD PosCheck);

private:
    std::vector<int> character_slot_list_;
    FTextRender character_info_text_[kMaxListSlot];

	stSlotCharacterList character_info_[kMaxListSlot];
    
	int selected_list_index_;

	int	save_post_select_;
	int save_now_select_;

	void onShowCheckUICharacterandKind(byte classidx, byte gender, int& charIndex, int& KindIndex);
	void onCheckUIList(int NowClickList);
};
#endif
