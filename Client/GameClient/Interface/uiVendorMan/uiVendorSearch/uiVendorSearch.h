#pragma once
#include "uiBase.h"

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
#define VENDOR_DATA_MAX 12
#include <SCItemSlotContainer.h>
#include <SCItemSlot.h>

#define ANIMATION_FRAME 12
#define ANIMATION_FRAME_WIDTH 40.0f
#define ANIMATION_FRAME_HEIGHT 40.0f

class uiVendorMan;

class uiVendorSearch : public uiBase, public SCItemSlotContainer
{
public:
	enum eSTRINGPRICE
	{

		VECTOR_STRING_PRICE_LOW1 = 1,
		VECTOR_STRING_PRICE_LOW2,
		VECTOR_STRING_PRICE_LOW3,
		VECTOR_STRING_PRICE_LOW4,
		VECTOR_STRING_PRICE_LOW5,

		VECTOR_STRING_PRICE_MID = 6,

		VECTOR_STRING_PRICE_HIGH = 7,
		
		VECTOR_STRING_PRICE_VERYHIGH = 8,
		VECTOR_STRING_PRICE_VERYHIGH2 = 9,
		VECTOR_STRING_PRICE_VERYHIGH3 = 10,
		VECTOR_STRING_PRICE_VERYHIGH4 = 11,
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
		VECTOR_STRING_PRICE_VERYHIGH5 = 12, 
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	};

    enum eVENDOR_LIST
    {
        VENDOR_ITEM_NAME = 0,
        VENDOR_NAME,
        VENDOR_PRICE,
        VENDOR_COUNT,
        VENDOR_TOTAL_PRICE,
        VENDOR_PLACE,
    };

    struct VENDOR_DATA
    {
        CODETYPE item_code;
        TCHAR vendor_name[ID_MAX_LEN + 1];
		TCHAR Item_Name_[INTERFACE_STRING_LENGTH];
        MONEY price;
        BYTE count;
        MONEY total_price;
        MAPCODE place;
		DWORD PlayerKeyValue_;
		POSTYPE BuySlotPos_;
		RENDER_ITEMSTREAMEX RenderInfo;
		SCItemSlot SlotInfo;

		VENDOR_DATA() : 
		item_code(0),
		price(0),
		count(0),
		total_price(0),
		place(0),
		PlayerKeyValue_(0),
		BuySlotPos_(0)
		{
		}
    };

private:


    enum UIControlPos
    {
        kControl_Picture_P029 = 0,
        kControl_Picture_P030,
        kControl_Picture_P031,
        kControl_Picture_P032,
        kControl_Picture_P033,
        kControl_Picture_P034,
        kControl_Picture_P035,
        kControl_Picture_P036,
        kControl_Picture_P037,
        kControl_Picture_P038,
        kControl_Picture_P039,
        kControl_Picture_P040,

        kControl_Button_B999,
        kControl_Button_BT01,
        kControl_Button_BT00,
        kControl_Button_B003,
        kControl_Button_BT02,
        kControl_Button_B000,
        kControl_Button_B005,
        kControl_Button_B002,
        kControl_Button_B004,
        kControl_Button_B006,
        kControl_Button_B007,
        kControl_Button_B008,
        kControl_Button_B009,
        kControl_Text_B001,
        kControl_Text_ST00,
        kControl_List_L000,
        kControl_List_L001,
        kControl_Edit_E000,
        kControl_Size,
    };

    enum
    {
        kSlotTotalCount = kControl_Picture_P040 - kControl_Picture_P029 + 1,
    };

public:
    uiVendorSearch(InterfaceManager* ui_manager);
    virtual ~uiVendorSearch();

    virtual void Init(CDrawBase* drawbase);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
	bool OnLButtonDClick( SI_MESSAGE* message );
	void OnKeyEnterInput(SI_MESSAGE* message );
	void OnBuyItem();

	const VENDOR_DATA& GetVendorSelectItem() { return isNowButClick_; }

    virtual void OnShowWindow(BOOL is_show);
    virtual void NetworkProc(MSG_BASE* message_ptr);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();
    virtual void MouseOver_MsgProc(SI_MESSAGE* message);

    void set_search_type(BYTE search_type){ search_type_ = search_type; }
    void set_max_pate(int max_page){ max_page_ = max_page; }
    void SetPage(int page);
    nArray<VENDOR_DATA> vendor_array_;

	void PlayAnimation();
	void InitTexture();
	void ShowAnimation(bool is_value);

	const TCHAR* GetSearchItemName() { return EnterSearchInput_; }
	void RewindSearchPacket();

private:
    void SetItemSlot(int index_display, BASE_ITEMINFO* item_info);
    RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, RENDER_ITEMSTREAMEX& RenderInfo,CODETYPE ItemCodes);
    BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_out);
    void DeleteAllSlot();
    void ReFreshRenderSlot();
    void UpdateToolTip();

    void SetListData(int nIndex,int nSubIndex,TCHAR* pszText);
    void RefreshListData(nArray<VENDOR_DATA> &ArrayType);
    bool OnLButtonClick(SI_MESSAGE* message);
    void RegisterControl();
    void SetSearchType(BYTE type);
    void SortList();
    //bool SortPrice(const VENDOR_DATA& member_info_ref_1, VENDOR_DATA& member_info_ref2);
	__forceinline int GetItemPriceColorLevel(int stringLen);
	__forceinline void SendPackSearchItem(int page, const TCHAR* SearchName,bool inForce = false);
	__forceinline int  CheckByte(const char* charset);

    ItemUnitRenderer* item_unit_renderer_;
    CCtrlListWZ* vendor_search_list_ptr_;
    BYTE search_type_;
    int page_;
    int max_page_;
    int pre_page_;
    bool order_value_;

	bool open_shop_;
	DWORD open_shopOwner_Key_;

	FRECT texture_rectangles_[ANIMATION_FRAME];
	CTimerSimple list_refresh_timer_;
	CTimerSimple animation_refresh_timer_;

	bool animatino_show_;
	HANDLE effect_texture_;

	// 지금 클릭한 아이템의 정보
	VENDOR_DATA isNowButClick_;

	//내가 입력한 검색어 저장
	TCHAR EnterSearchInput_[INTERFACE_STRING_LENGTH];

	
};

const int c_iListHeight = 24;
const int c_iListWidth[uiVendorSearch::VENDOR_PLACE+1] =
{
    191,	//VENDOR_ITEM_NAME = 0,
        191,	//VENDOR_NAME,
        127,//VENDOR_PRICE,
        68,	//VENDOR_COUNT,
        127,	//VENDOR_TATAL_PRICE,
        150,//VENDOR_PLACE,
};
#endif//_NA_005988_20120901_VENDOR_SEARCH_SYSTEM