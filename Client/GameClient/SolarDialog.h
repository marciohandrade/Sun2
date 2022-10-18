#ifndef __SOLAR_DIALOG_H__
#define __SOLAR_DIALOG_H__


#pragma once

//=======================================================================================================================
/// ��� �������̽� ���̾�α��� Base-class
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 12. 22
	@remarks
			- MessageProc()�� ���� ���� �޽��� ���ν������� ȣ��Ǿ�� �Ѵ�.
			- Ư�� ���̾�α׿� ������ �ִ� ��Ŷ�� NetworkProc()�� ������ ó��
*/
//=======================================================================================================================



#include "timer.h"

struct SI_MESSAGE;
enum eUI_SOUND_INDEX;

class CDrawBase;
class ItemUnitRenderer;

class SolarDialog
{
public:
	SolarDialog();
	virtual ~SolarDialog();

	VOID					SetDialogWZ( CDialogWZ * pDialogWZ );

	virtual VOID			Init(CDrawBase* pDrawBase){}
	virtual VOID			Release(){}
    virtual void			Reset(){}
	VOID					Render();
	VOID					Update();
	
	//--------------------- �������̽� ������ -------------------------------------
	virtual VOID			ShowWindow( BOOL val );
	virtual VOID			ShowDialog( BOOL val );
    virtual BOOL            IsVisible() { return m_bShow; } 

	bool					MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
	bool					MoveWindowWZ(float fx, float fy);
	bool					MoveWindowWZ(int x, int y);
	//bool					MoveWindowPos();
	virtual void			MoveDefaultWindowPos();
    void                    MoveCenter();

	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
    virtual VOID			NetworkProc( MSG_BASE * pMsg ) {}
	virtual VOID            Process(DWORD dwTick);
	virtual VOID			MouseOver_MsgProc( SI_MESSAGE * pMessage );

	virtual CDialogWZ*		    GetDialogWZ();
	virtual CControlWZ*			GetControlWZ(DWORD control_id);
    virtual CCtrlButtonCheckWZ* GetCheckButtonControlWZ(DWORD control_id);
    virtual CCtrlButtonWZ*      GetButtonControlWZ(DWORD control_id);
    virtual CCtrlListWZ*        GetListControlWZ(DWORD control_id);
    virtual CCtrlVScrollWZ*     GetVScrollControlWZ(DWORD control_id);
    virtual CCtrlStaticWZ*      GetStaticControlWZ(DWORD control_id);
    virtual CCtrlPictureWZ*     GetPictureControlWZ(DWORD control_id);
    virtual CCtrlImageStaticWZ* GetImageStaticControlWZ(DWORD control_id);
    virtual CCtrlDxEditBox*     GetDxEditBoxControlWZ(DWORD control_id);
    virtual CCtrlMultilineEditWZ* GetMultilineEditControlWZ(DWORD control_id);

    virtual void                SetEnable(DWORD control_index, bool enable);
    virtual void                SetVisible(DWORD control_index, ENUM_STATEWZ state);
    virtual bool                IsChecked(DWORD button_check_control_index);
    virtual void                SetChecked(DWORD button_check_control_index, bool is_check);
    virtual void                SetTextColor(DWORD text_control_index, COLORREF color);

    //------------------------------------------------------------------------------
    //! Returns:   void
    //! Qualifier:
    //! Parameter: DWORD control_index : static control �̳� DxEditBox control ���� ����
    //! Parameter: int val : ����
    //------------------------------------------------------------------------------

    virtual void                SetTextInt(DWORD control_index, int val);
    virtual int                 GetTextInt(DWORD control_index);

	virtual BOOL			IsVisibleWindow();
	virtual BOOL			HaveFocus();

    BOOL                    IsCheckDistance_NPCMEET();

    void                    Lock();
    void                    Unlock();
    BOOL                    IsLocked();

    void PlayOpenDialogSound();
    void PlayCloseDialogSound();

	inline void				RemovePacketBit(DWORD packetType)
	{
		BIT_REMOVE(m_packetStatus, packetType);
	}
	//////////////////////////////////////////////////////////////////////////
	// TODO : base Dialog�� ��ӹ��� ������ �̵�ó���� �ϴ� Ŭ������ ������..
	//		  �� �޼ҵ���� �׸��� ������.
	virtual ItemUnitRenderer * GetItemUnitRender() const { return NULL; }

	//////////////////////////////////////////////////////////////////////////
	//virtual void            UpdatePosition();

	void                    SetOwnerKey(DWORD dwKey);
	DWORD                   GetOwnerKey();

	BOOL                    IsDynamicWindow();
	void                    SetDynamicWindow(BOOL bDynamic);

	void                    SetDestroyWindow(BOOL bDestroy);
	BOOL					GetDestroyWindow(); 

	void					SetSkipCheckSuspendFocus(BOOL bFlag ) { m_bSkipSuspendFocusCheck = bFlag; }

	BOOL					IsSkipCheckSuspendFocus() const { return m_bSkipSuspendFocusCheck; }

	BOOL					IsLoaded() { return m_bLoaded; }
	void					SetLoaded( BOOL bLoad) { m_bLoaded = bLoad; }

	//TODO: Dialog���� ���콺�̺�Ʈ(��ŷ,Ŭ��)������ �޸��ϰ����Ҷ�
	//		�������̵��ؼ� ���� �������� �� ����
	void					DisableInterSectRect(const RECT& rRT){ m_pDialogWZ->DisableInterSectRect(rRT); }

    virtual	BOOL			InterSectRect( POINT point );			
    bool IsInRect(float left, float top, float width, float height);

	void					ReleaseDialogTexture();

    void RegisterControlPosition(WzID control_id, DWORD position);
    DWORD GetControlPosition_byID(WzID control_id);
    WzID GetControlID_byPosition(DWORD position);
    CControlWZ* GetControlWZ_byPosition(DWORD position);

    // ��Ʈ�ѿ� �̹��� �����Լ�
    // image_code : GraphicResourceList�� ���ҽ��ڵ�(0�̸� �̹��������� ������)
    void SetControlImage(WzID control_id, 
                         CODETYPE image_code, 
                         int image_src_x, 
                         int image_src_y, 
                         int image_height, 
                         int image_width, 
                         BYTE image_alpha = 255);
    void SetControlImageFilename(WzID control_id, 
                                 char* image_filename, 
                                 int image_src_x, 
                                 int image_src_y, 
                                 int image_height, 
                                 int image_width, 
                                 BYTE image_alpha = 255);

    void SetBlinkGrid(DWORD control_id, bool is_blink);

protected:
    virtual void OnShowWindow( BOOL val );

private:

    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

protected:

	virtual BOOL UpdateData(BOOL /*bTake = TRUE*/) {return TRUE;}
#if _DEV_VER//#ifdef _DH_SHOW_UI_NAME
    virtual char* GetFileName();
    void ShowFileName();
#endif//_DH_SHOW_UI_NAME
	// ���� ����
protected:
	virtual DWORD			getOpenDlgSoundIndex();
	virtual DWORD			getCloseDlgSoundIndex();

    bool    is_ani_playing() const { return is_ani_playing_; }
    void    set_is_ani_playing(bool val) { is_ani_playing_ = val; }
    DWORD   ani_start_tick() const { return ani_start_tick_; }
    void    set_ani_start_tick(DWORD val) { ani_start_tick_ = val; }
    float GetFadeValue(util::Timer* timer, DWORD fade_max_time, bool is_fade_in);

    void ControlSize(RECT control_rect);

//------------------------------------------------------------------------------
private:
	CDialogWZ*				m_pDialogWZ;
    BOOL					m_bShow;
    BOOL					m_bLoaded;	// ���ҽ� �ε尡 �Ǿ��°�?

    std::map<WzID, DWORD>   control_position_map_;
    std::map<DWORD, WzID>   control_id_map_;

    bool is_blink_grid_;
    DWORD grid_control_id_;
    util::Timer grid_timer_;
    FRECT control_rectangles_[8];//8���� ��Ʈ��

    //! �����̴� ���̾�α� �ϰ�� Ȯ�� ����
    //! dialoginfo.txt�� ���� ��.
    //! OnShowWindow���� �ð��� �����ϰ�
    //! update���� movewindow �Լ��� ����ؼ� �̵���
    bool                    is_ani_playing_;
    DWORD                   ani_start_tick_;

public:
    bool IsActivated() { return m_bActivate; }
    void SetActivated( bool bActived ) { m_bActivate = bActived; }

private:
    bool					m_bActivate;

protected:
    InterfaceManager*       m_pUIMan;
    DWORD                   m_packetStatus;//��Ŷ ���� ����-���� �߰� 
    BOOL                    m_bLocked;
	DWORD                   m_dwOwnerKey;

	MouseOverObject			m_MouseOverObject;
	BOOL                    m_bDynamicWindow;
	BOOL                    m_bDestory;
    BOOL                    m_bCheckDistance_NPCMEET;
	BOOL					m_bSkipSuspendFocusCheck; // ��Ŀ���� ������� �ʾƵ� �Ǵ� �������� �Ǵ�.
};

inline
BOOL
SolarDialog::IsDynamicWindow()
{
	return m_bDynamicWindow;
}


inline
void 
SolarDialog::SetDynamicWindow(BOOL bDynamic)
{
	m_bDynamicWindow=bDynamic;
}


inline
void 
SolarDialog::SetDestroyWindow(BOOL bDestroy)
{
	SUN_ASSERT_MSG(m_bDynamicWindow , "SetDestroyWindow() -Use Only DynamicWindow");
	m_bDestory=bDestroy;
}

inline
BOOL 
SolarDialog::GetDestroyWindow()
{
	return m_bDestory;
}

inline
void
SolarDialog::Lock()
{
    this->m_bLocked = true;

    if (GetDialogWZ() != NULL)
    {
        CControlWZ* client_control = GetDialogWZ()->GetClientWZ();
        if (client_control != NULL)
        {
            client_control->SetDiscardMessage(true);
        }
    }

}


inline
void
SolarDialog::Unlock()
{
    this->m_bLocked = false;

    if (GetDialogWZ() != NULL)
    {
        CControlWZ* client_control = GetDialogWZ()->GetClientWZ();
        if (client_control != NULL)
        {
            client_control->SetDiscardMessage(false);
        }
    }

}

inline
BOOL
SolarDialog::IsLocked()
{
    return this->m_bLocked;
}

inline
CDialogWZ*
SolarDialog::GetDialogWZ()
{ 
	return m_pDialogWZ; 
}

inline
void 
SolarDialog::SetOwnerKey(DWORD dwKey)
{
	m_dwOwnerKey = dwKey;
}

inline
DWORD 
SolarDialog::GetOwnerKey()
{
	return m_dwOwnerKey;
}

inline
BOOL
SolarDialog::IsCheckDistance_NPCMEET()
{
    return m_bCheckDistance_NPCMEET;
}


//------------------------------------------------------------------------------ 
// Ÿ�̸ӿ� ���� �ð��� �޾� Fade���� �����Ѵ�.
inline float SolarDialog::GetFadeValue(util::Timer* timer, DWORD fade_max_time, bool is_fade_in)
{
    float fade_value = 1.0f;

    DWORD ProgressTime = timer->GetProcessTime();

    if (timer->GetRemainedTime() > 0)
    {
        fade_value = static_cast<float>(ProgressTime) / fade_max_time;
        if (fade_value > 1.0f)
        {
            fade_value = 1.0f;
        }
    }

    if (is_fade_in == false)
    {
        fade_value = 1.0f - fade_value;
    }

    return fade_value;
}
#endif // __SOLAR_DIALOG_H__