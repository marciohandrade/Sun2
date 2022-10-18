#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "uiBase.h"
#include "ItemAttrCalculator.h"

class uiCharacterStatusSockecInfo : public uiBase
{
    enum UIControlPos
    {
        // 탭 컨트롤
        kControl_CheckBT_C000 = 0,
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		kControl_CheckBT_C004, 
#endif // _NA_008472_20150903_IMPROVED_BALANCE
        kControl_CheckBT_C001,
        kControl_CheckBT_C002,
        kControl_CheckBT_C003,

        // 소켓 옵션명
        kControl_Text_S006,
        kControl_Text_S007,
        kControl_Text_S008,
        kControl_Text_S009,
        kControl_Text_S010,
        kControl_Text_S011,
        kControl_Text_S012,
        kControl_Text_S013,
        kControl_Text_S014,
        kControl_Text_S015,
        kControl_Text_S016,
        kControl_Text_S017,
        kControl_Text_S018,
        kControl_Text_S019,
        kControl_Text_S020,
        kControl_Text_S000,

        // 옵션 수치
        // 장비1
        kControl_Text_S021,
        kControl_Text_S031,
        kControl_Text_S041,
        kControl_Text_S051,
        kControl_Text_S061,
        kControl_Text_S071,
        kControl_Text_S081,
        kControl_Text_S091,
        kControl_Text_S101,
        kControl_Text_S111,
        kControl_Text_S121,
        kControl_Text_S131,
        kControl_Text_S141,
        kControl_Text_S151,
        kControl_Text_S161,
        kControl_Text_S171,

        // 장비2
        kControl_Text_S022,
        kControl_Text_S032,
        kControl_Text_S042,
        kControl_Text_S052,
        kControl_Text_S062,
        kControl_Text_S072,
        kControl_Text_S082,
        kControl_Text_S092,
        kControl_Text_S102,
        kControl_Text_S112,
        kControl_Text_S122,
        kControl_Text_S132,
        kControl_Text_S142,
        kControl_Text_S152,
        kControl_Text_S162,
        kControl_Text_S172,

        // 장비3
        kControl_Text_S023,
        kControl_Text_S033,
        kControl_Text_S043,
        kControl_Text_S053,
        kControl_Text_S063,
        kControl_Text_S073,
        kControl_Text_S083,
        kControl_Text_S093,
        kControl_Text_S103,
        kControl_Text_S113,
        kControl_Text_S123,
        kControl_Text_S133,
        kControl_Text_S143,
        kControl_Text_S153,
        kControl_Text_S163,
        kControl_Text_S173,

        // 장비4
        kControl_Text_S024,
        kControl_Text_S034,
        kControl_Text_S044,
        kControl_Text_S054,
        kControl_Text_S064,
        kControl_Text_S074,
        kControl_Text_S084,
        kControl_Text_S094,
        kControl_Text_S104,
        kControl_Text_S114,
        kControl_Text_S124,
        kControl_Text_S134,
        kControl_Text_S144,
        kControl_Text_S154,
        kControl_Text_S164,
        kControl_Text_S174,

        // 장비5
        kControl_Text_S025,
        kControl_Text_S035,
        kControl_Text_S045,
        kControl_Text_S055,
        kControl_Text_S065,
        kControl_Text_S075,
        kControl_Text_S085,
        kControl_Text_S095,
        kControl_Text_S105,
        kControl_Text_S115,
        kControl_Text_S125,
        kControl_Text_S135,
        kControl_Text_S145,
        kControl_Text_S155,
        kControl_Text_S165,
        kControl_Text_S175,

        // 장비6
        kControl_Text_S026,
        kControl_Text_S036,
        kControl_Text_S046,
        kControl_Text_S056,
        kControl_Text_S066,
        kControl_Text_S076,
        kControl_Text_S086,
        kControl_Text_S096,
        kControl_Text_S106,
        kControl_Text_S116,
        kControl_Text_S126,
        kControl_Text_S136,
        kControl_Text_S146,
        kControl_Text_S156,
        kControl_Text_S166,
        kControl_Text_S176,

        // 장비7
        kControl_Text_S027,
        kControl_Text_S037,
        kControl_Text_S047,
        kControl_Text_S057,
        kControl_Text_S067,
        kControl_Text_S077,
        kControl_Text_S087,
        kControl_Text_S097,
        kControl_Text_S107,
        kControl_Text_S117,
        kControl_Text_S127,
        kControl_Text_S137,
        kControl_Text_S147,
        kControl_Text_S157,
        kControl_Text_S167,
        kControl_Text_S177,

        // 장비8
        kControl_Text_S028,
        kControl_Text_S038,
        kControl_Text_S048,
        kControl_Text_S058,
        kControl_Text_S068,
        kControl_Text_S078,
        kControl_Text_S088,
        kControl_Text_S098,
        kControl_Text_S108,
        kControl_Text_S118,
        kControl_Text_S128,
        kControl_Text_S138,
        kControl_Text_S148,
        kControl_Text_S158,
        kControl_Text_S168,
        kControl_Text_S178,

        // 장비9
        kControl_Text_S029,
        kControl_Text_S039,
        kControl_Text_S049,
        kControl_Text_S059,
        kControl_Text_S069,
        kControl_Text_S079,
        kControl_Text_S089,
        kControl_Text_S099,
        kControl_Text_S109,
        kControl_Text_S119,
        kControl_Text_S129,
        kControl_Text_S139,
        kControl_Text_S149,
        kControl_Text_S159,
        kControl_Text_S169,
        kControl_Text_S179,

        // 옵션수치 합계
        kControl_Text_S030,
        kControl_Text_S040,
        kControl_Text_S050,
        kControl_Text_S060,
        kControl_Text_S070,
        kControl_Text_S080,
        kControl_Text_S090,
        kControl_Text_S100,
        kControl_Text_S110,
        kControl_Text_S120,
        kControl_Text_S130,
        kControl_Text_S140,
        kControl_Text_S150,
        kControl_Text_S160,
        kControl_Text_S170,
        kControl_Text_S180,

        // 옵션효과
        kControl_Text_S201,
        kControl_Text_S202,
        kControl_Text_S203,
        kControl_Text_S204,
        kControl_Text_S205,
        kControl_Text_S206,
        kControl_Text_S207,
        kControl_Text_S208,
        kControl_Text_S209,
        kControl_Text_S210,
        kControl_Text_S211,
        kControl_Text_S212,
        kControl_Text_S213,
        kControl_Text_S214,
        kControl_Text_S215,
        kControl_Text_S216,

        // 장비 슬롯
        kControl_Picture_P035,
        kControl_Picture_P043,
        kControl_Picture_P044,
        kControl_Picture_P045,
        kControl_Picture_P046,
        kControl_Picture_P047,
        kControl_Picture_P048,
        kControl_Picture_P049,
        kControl_Picture_P050,

        // 스크롤
        kControl_VScroll_V000,

        kControl_Size,
    };

    enum
    {
        kSocketOptionShowRowCount = 16,
        kSocketOptionShowColCount = 9,
    };

public:
    static const DWORD kDialog_ID;

    typedef std::vector<ItemAttrCalculator::SocketOptionCalculator::CalculationResult> SocketOptionVector;

public:
    uiCharacterStatusSockecInfo(InterfaceManager* interface_manager);
    ~uiCharacterStatusSockecInfo();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);

private:
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    void RefreshTab();
    void UpdateMouseWheel();

    //------------------------------------------------------------------------------
    //! Returns:   void
    //! Qualifier: 해당 아이템의 소켓정보를 화면에 출력
    //! Parameter: SCItemSlot * item_slot : 아이템 정보
    //! Parameter: int col : 각각의 소켓정보를 보여줄 ui 배열 칼럼(eEQUIPCONTAINER_POS 와 같음)
    //! Parameter: socketcount_vector : 소켓정보 합계(포인트)
    //------------------------------------------------------------------------------
    void UpdateSocketOptionValue(SCItemSlot* item_slot, 
                                 int col, 
                                 IN uiCharacterStatusSockecInfo::SocketOptionVector& socketcount_vector);
    void UpdateSocketOptionName(IN uiCharacterStatusSockecInfo::SocketOptionVector& socketcount_vector);

    void SetSocketOptionValue(int col, int row, int val, WzColor text_color);
    void SetSocketOptionValue(UIControlPos control_position, TCHAR* text_string, WzColor text_color);
    void SetSocketOptionValue(UIControlPos control_position, int val, WzColor text_color);
    void CalcTotalSocketOption(OUT uiCharacterStatusSockecInfo::SocketOptionVector& socketcount_vector);
    void RenderTooltip_SocketOption(IN uiCharacterStatusSockecInfo::SocketOptionVector& socketcount_vector);

    uiCharacterStatusSockecInfo::SocketOptionVector& socketoption_vector() { return socketoption_vector_; }
    int socket_option_show_begin_index() const { return socket_option_show_begin_index_; }
    void set_socket_option_show_begin_index(int val) { socket_option_show_begin_index_ = val; }

    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnVScrollUp(CCtrlVScrollWZ* scroll_control);
    bool OnVScrollDown(CCtrlVScrollWZ* scroll_control);
    bool OnVScrollThumb(CCtrlVScrollWZ* scroll_control);

private:
    SocketOptionVector socketoption_vector_;
    int socket_option_show_begin_index_;
};
#endif //_NA_003027_20111013_HONOR_SYSTEM