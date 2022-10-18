#pragma once
#include "uibase.h"
struct BASE_HeroVariationInfo;
struct BASE_HeroHeightInfo;

//------------------------------------------------------------------------------
/**

    커스터마이징 Dialog

    @author		광수

    @since		10.06.14

    @remarks	
- 
*/
enum Customize_Type
{
    kHair,
    kFace,
    kHeight,
    KGender,
    kBase,
    kMax = kBase,
};

enum FaceViewState
{
    kEventNone=0,
    kRotateLeft,
    kRotateRight,
    kCameraZoomIn,
    kCameraZoomOut,
};

struct Display_Info
{
    FaceViewState state;
    float unit_scale;
    float rotate_angle;
    float camera_pos;
};

struct Customize_Info
{
    WORD customize_type;
    POSTYPE inven_pos;
    
    int max_id[kMax];
    int cur_id[kMax];
    int save_pos[kMax];
};

class uiCharCustomizeDlg : public uiBase
{
// Public Section
public:
    enum CharCustomizPos
    {
        kDlgTitle=0,    // 타이틀(ST00)
        kMainView,      // 캐릭터화면(P004)
        kSecondPic,     // 2번째PIC(P002)
        kThirdPic,      // 3번째PIC(P003)
        kEscBtn,        // x버튼(BT01)
        kZoonInBtn,     // +버튼(BT02)
        kZoomOutBtn,    // -버튼(BT03)
        kLeftRotateBtn, // 좌회전버튼(BT04)
        kRightRotateBtn,// 우회전버튼(BT05)
        kLeftHairBtn,   // 좌헤어버튼(BT06)
        kLeftFaceBtn,   // 좌얼굴버튼(BT08)
        kLeftHeightBtn, // 좌키버튼(BT10)
        kRigthHairBtn,  // 우헤어버튼(BT07)
        kRightFaceBtn,  // 우얼굴버튼(BT09)
        kRightHeightBtn,// 우키버튼(BT11)
        kChangeBtn,     // 변경버튼(BT12)
        kCancleBtn,     // 취소(BT13)
        kRollBackBtn,   // 원래대로(BT14)
        kHairTypeText,  // 머리텍스트(S000)
        kFaceTypeText,  // 헤어텍스트(S001)
        kHeightTypeText,// 키텍스트(S002)
        kCurHairText,   // 머리(S005)
        kCurFaceText,   // 헤어(S006)
        kCurHeightText, // 키(S003)
        kOnDescList,    // 설명(L000)
        kCustomDlgMax,  // max
    };

	uiCharCustomizeDlg(InterfaceManager *pUIMan);
	virtual ~uiCharCustomizeDlg(void);
	virtual VOID Init( CDrawBase * pDrawBase );
    
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();
	virtual VOID MessageProc( SI_MESSAGE * pMessage );
	virtual void OnShowWindow( BOOL val );
   
    // 커스터창열릴때 셋팅해줌
    void GenerateCustomInfo(WORD custom_type, POSTYPE slot_pos);
    // 서버로 변경패킷보내기
    void SendCharCustomizePacket();
    // 활성화 슬롯인지
    bool IsActiveSlot(POSTYPE slot_pos);

private:
    
    // 컨트롤러관련
    bool invalid_dlg_control() const { return invalid_dlg_control_; }
    void set_invalid_dlg_control(bool val) { invalid_dlg_control_ = val; }
    CControlWZ* getControl( POSTYPE AtPos );
    POSTYPE getCtrlIDToPos( WzID wzId );
    void InitControls();
    
    // 초기화(다이얼로그 열릴때 셋팅)
    DWORD GetCompositeID(WORD parts, WORD id);
    void InitCustomizeData();
    void SetListData(WORD type, WORD parts, bool is_cash);
    bool SetSingleListData(WORD type, WORD hero_part_id);
    bool InitTitleCaption(BYTE type, CCtrlStaticWZ* control);
        
    // 갱신관련함수(플레이어,텍스트,페이스뷰)
    Player* GetCustomizePlayer();
    void RenderPlayer(RECT viewport);
    void OnClickChangeButton(WORD index, bool is_next);
    void SetRotateAngle(float angle);
    void SetDiplayCamera(bool is_zoomin);
    
    void RefreshData();
    void RefreshAppearance();
    void RefreshSelectText();
    void RefreshDescText();

    void RelesePlayer();
    void DataClear();
    
    // 커스텀버튼 관련(
    void reset_customize_data(){ memset(&customize_info_, 0, sizeof(customize_info_));}
    WORD customize_type() { return customize_info_.customize_type; }
    void set_customize_type(WORD type){ customize_info_.customize_type = type; }

    int cur_pos(WORD type) { return customize_info_.cur_id[type]; }
    void set_cur_pos(WORD type, int index){ customize_info_.cur_id[type] = index; }

    int save_pos(WORD type){ return customize_info_.save_pos[type]; }
    void set_save_pos(WORD type, int index){ customize_info_.save_pos[type] = index; }

    int max_pos(WORD type) { return customize_info_.max_id[type]; }
    void set_max_pos(WORD type, int index){ customize_info_.max_id[type] = index; }

    POSTYPE inven_pos() const {return customize_info_.inven_pos;}
    void set_inven_pos(POSTYPE pos) {customize_info_.inven_pos = pos;}
    
    BASE_HeroHeightInfo* height_info() const { return height_info_; }
    BASE_HeroVariationInfo* get_custom_info(WORD type, WORD pos);
    void set_height_info(BASE_HeroHeightInfo* val) { height_info_ = val; }
    
    void custom_list_clear(WORD type);
    void custom_list_insert(WORD type, BASE_HeroVariationInfo* info);

    // 페이스 뷰관련 (회전, 카메라 위치, 캐릭터크기)
    void reset_face_view(){ memset(&face_view_, 0, sizeof(face_view_));}
    FaceViewState face_view() const { return face_view_.state; }
    void set_face_view(FaceViewState val) { face_view_.state = val; }

    float roate_angel() const { return face_view_.rotate_angle; }
    void set_roate_angel(float val) { face_view_.rotate_angle = val; }

    float camera_pos() const { return face_view_.camera_pos; }
    void set_camera_pos(float val) { face_view_.camera_pos = val; }

    float unit_scale() const { return face_view_.unit_scale; }
    void set_unit_scale(float val) { face_view_.unit_scale = val; }

    // 컨트롤 변수
    bool invalid_dlg_control_;
	Customize_Info customize_info_; 
    Display_Info face_view_;
    std::vector<BASE_HeroVariationInfo*> custom_list_[kBase-1];
    BASE_HeroHeightInfo* height_info_;
    static WzID control_id[kCustomDlgMax];
};
//--------------------------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiCharCustomizeDlg::getCtrlIDToPos( WzID wz_id )
{
	for (POSTYPE index = 0; index < kCustomDlgMax; ++index)
	{
		if (control_id[index] == wz_id)
		{
			return index;
		}
	}
	return kCustomDlgMax;
}
//--------------------------------------------------------------------------------------------------
/**
*/
inline CControlWZ* uiCharCustomizeDlg::getControl(POSTYPE control_pos)
{
	ASSERT(control_pos < kCustomDlgMax);

	return GetControlWZ(control_id[control_pos]);
}
//--------------------------------------------------------------------------------------------------
/**
*/
inline void uiCharCustomizeDlg::custom_list_insert(WORD type, BASE_HeroVariationInfo* info)
{
    custom_list_[type].push_back(info);
}
//--------------------------------------------------------------------------------------------------
/**
*/
inline void uiCharCustomizeDlg::custom_list_clear(WORD type)
{
    custom_list_[type].clear();
}
//--------------------------------------------------------------------------------------------------
/**
*/
inline  BASE_HeroVariationInfo* uiCharCustomizeDlg::get_custom_info(WORD type, WORD pos)
{
    if (custom_list_[type].empty() ||
        (pos >= custom_list_[type].size()))
    {
        return NULL;
    }

    return custom_list_[type].at(pos);
}
