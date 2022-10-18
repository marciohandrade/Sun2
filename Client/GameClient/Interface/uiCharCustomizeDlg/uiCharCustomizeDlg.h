#pragma once
#include "uibase.h"
struct BASE_HeroVariationInfo;
struct BASE_HeroHeightInfo;

//------------------------------------------------------------------------------
/**

    Ŀ���͸���¡ Dialog

    @author		����

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
        kDlgTitle=0,    // Ÿ��Ʋ(ST00)
        kMainView,      // ĳ����ȭ��(P004)
        kSecondPic,     // 2��°PIC(P002)
        kThirdPic,      // 3��°PIC(P003)
        kEscBtn,        // x��ư(BT01)
        kZoonInBtn,     // +��ư(BT02)
        kZoomOutBtn,    // -��ư(BT03)
        kLeftRotateBtn, // ��ȸ����ư(BT04)
        kRightRotateBtn,// ��ȸ����ư(BT05)
        kLeftHairBtn,   // ������ư(BT06)
        kLeftFaceBtn,   // �¾󱼹�ư(BT08)
        kLeftHeightBtn, // ��Ű��ư(BT10)
        kRigthHairBtn,  // ������ư(BT07)
        kRightFaceBtn,  // ��󱼹�ư(BT09)
        kRightHeightBtn,// ��Ű��ư(BT11)
        kChangeBtn,     // �����ư(BT12)
        kCancleBtn,     // ���(BT13)
        kRollBackBtn,   // �������(BT14)
        kHairTypeText,  // �Ӹ��ؽ�Ʈ(S000)
        kFaceTypeText,  // ����ؽ�Ʈ(S001)
        kHeightTypeText,// Ű�ؽ�Ʈ(S002)
        kCurHairText,   // �Ӹ�(S005)
        kCurFaceText,   // ���(S006)
        kCurHeightText, // Ű(S003)
        kOnDescList,    // ����(L000)
        kCustomDlgMax,  // max
    };

	uiCharCustomizeDlg(InterfaceManager *pUIMan);
	virtual ~uiCharCustomizeDlg(void);
	virtual VOID Init( CDrawBase * pDrawBase );
    
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();
	virtual VOID MessageProc( SI_MESSAGE * pMessage );
	virtual void OnShowWindow( BOOL val );
   
    // Ŀ����â������ ��������
    void GenerateCustomInfo(WORD custom_type, POSTYPE slot_pos);
    // ������ ������Ŷ������
    void SendCharCustomizePacket();
    // Ȱ��ȭ ��������
    bool IsActiveSlot(POSTYPE slot_pos);

private:
    
    // ��Ʈ�ѷ�����
    bool invalid_dlg_control() const { return invalid_dlg_control_; }
    void set_invalid_dlg_control(bool val) { invalid_dlg_control_ = val; }
    CControlWZ* getControl( POSTYPE AtPos );
    POSTYPE getCtrlIDToPos( WzID wzId );
    void InitControls();
    
    // �ʱ�ȭ(���̾�α� ������ ����)
    DWORD GetCompositeID(WORD parts, WORD id);
    void InitCustomizeData();
    void SetListData(WORD type, WORD parts, bool is_cash);
    bool SetSingleListData(WORD type, WORD hero_part_id);
    bool InitTitleCaption(BYTE type, CCtrlStaticWZ* control);
        
    // ���Ű����Լ�(�÷��̾�,�ؽ�Ʈ,���̽���)
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
    
    // Ŀ���ҹ�ư ����(
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

    // ���̽� ����� (ȸ��, ī�޶� ��ġ, ĳ����ũ��)
    void reset_face_view(){ memset(&face_view_, 0, sizeof(face_view_));}
    FaceViewState face_view() const { return face_view_.state; }
    void set_face_view(FaceViewState val) { face_view_.state = val; }

    float roate_angel() const { return face_view_.rotate_angle; }
    void set_roate_angel(float val) { face_view_.rotate_angle = val; }

    float camera_pos() const { return face_view_.camera_pos; }
    void set_camera_pos(float val) { face_view_.camera_pos = val; }

    float unit_scale() const { return face_view_.unit_scale; }
    void set_unit_scale(float val) { face_view_.unit_scale = val; }

    // ��Ʈ�� ����
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
