#ifndef _uiLoginServerChannelList_h_
#define _uiLoginServerChannelList_h_

#include "uibase.h"

class uiLoginMan;
struct AuthSetverData;
class AuthGroupData;

//------------------------------------------------------------------------------ 
class uiLoginServerChannelList : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B000 = 0,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B005,

        kControl_Text_T001,
        kControl_Text_T002,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Text_S003,
        kControl_Text_S004,

        kControl_List_L001,
        kControl_VScroll_V000,
        kControl_VScroll_V003,

        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P002,
        kControl_Picture_P003,
        kControl_Picture_P004,
        kControl_Picture_P005,
        kControl_Picture_P006,
        kControl_Picture_P007,
        kControl_Picture_P008,
        kControl_Picture_P009,

        kControl_Picture_P010,
        kControl_Picture_P019,
        kControl_Picture_P011,
        kControl_Picture_P018,
        kControl_Picture_P012,
        kControl_Picture_P017,
        kControl_Picture_P013,
        kControl_Picture_P016,
        kControl_Picture_P014,
        kControl_Picture_P015,

        kControl_Picture_P021,
        kControl_Picture_P030,
        kControl_Picture_P022,
        kControl_Picture_P032,
        kControl_Picture_P024,
        kControl_Picture_P034,
        kControl_Picture_P026,
        kControl_Picture_P036,
        kControl_Picture_P028,
        kControl_Picture_P038,

        kControl_Picture_P020,
        kControl_Picture_P031,
        kControl_Picture_P023,
        kControl_Picture_P033,
        kControl_Picture_P025,
        kControl_Picture_P035,
        kControl_Picture_P027,
        kControl_Picture_P037,
        kControl_Picture_P029,
        kControl_Picture_P039,

        kControl_Picture_P040,
        kControl_Picture_P041,
        kControl_Picture_P042,
        kControl_Picture_P043,
        kControl_Picture_P044,
        kControl_Picture_P045,
        kControl_Picture_P046,
        kControl_Picture_P047,
        kControl_Picture_P048,
        kControl_Picture_P049,

        kControl_Size,
    };

//public:
//    static const DWORD kDialog_ID;

    const static int kServerNumPerList = 10;    //!< 리스트 박스에 한번에 나올수 있는 리스트갯수
    const static int kCrowdGradePictureNum = 10; //!< 혼잡도 게이지 갯수
    const static int kChannelNumPerList = 5;
    

public:
    uiLoginServerChannelList(InterfaceManager* ui_man);
    virtual ~uiLoginServerChannelList(void);

    virtual void    Init(CDrawBase* drawbase);
    virtual void    Release(void);
    virtual void    Process(DWORD tick);
    virtual void	OnShowWindow(BOOL val);
    virtual void    OnRenderSolarDialog();
    virtual void    MessageProc(SI_MESSAGE* message);

    uiLoginMan* GetManager();
    AuthGroupData* GetCurrentSelectedAuthGroup();
    void UpdateServerList();
	void					hideCheckonShow(bool hide);
	float					GetTransTime() { return m_fTrans; }

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnVScrollUp(SI_MESSAGE* message);
    bool OnVScrollDown(SI_MESSAGE* message);
    bool OnVScrollThumb(SI_MESSAGE* message);

    void UpdateChannelList();
    void UpdateChannel(int index, AuthSetverData* auth_setver_data);
    int  GetPictureImageCount(BYTE crowd_grade);

    IconImage* m_pNewServerIcon;

	float                   m_fTrans;
	DWORD                   m_dwTransAcc;
	DWORD                   m_dwTransEnd;
	bool					hide_check_mouse_;
};

#endif // _uiLoginServerChannelList_h_
