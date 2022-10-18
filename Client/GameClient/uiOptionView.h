#pragma once

#include "uiBase.h"
#include "OptionItem.h"


class cOptionListControl
{
private:
    std::vector<cOptionCategory*> m_Categorys;
    std::vector<cOptionItem*> m_ItemArray;

public:
    cOptionListControl();
    ~cOptionListControl();

    void LoadPage( DWORD PageIndex );

    void UpdateVisibleItemArray( std::vector<cOptionItemBase*>& Array );
    void RestoreOptionValue();
    void ApplyOptionValue();

    cOptionCategory* FindCategory( const std::string& CategoryName );
};

class uiOptionView : public uiBase
{
public:
	//keymap only
	enum eOPTION_POS
	{
		eKEYMAP_CHECK_C004,//C004
		eOPTION_MAX,
	};

private:
    BOOL m_Initialized;
	
    std::vector<cOptionListControl*> m_PageList;
    DWORD m_PageIndex;


    class sOptionControlItem
    {
    public:
        sOptionControlItem( WzID Left, WzID Center, WzID Right ) : m_Left(Left), m_Center(Center), m_Right(Right) {}

        WzID m_Left;
        WzID m_Center;
        WzID m_Right;
    };
    std::vector<sOptionControlItem*> m_ControlArray;
    std::vector<cOptionItemBase*> m_VisibleItemArray;

    CCtrlVScrollWZ* m_pScroll;
    std::vector<CCtrlButtonCheckWZ*> m_CheckButtonArray;

    RECT m_ViewRect;

    cOptionComboItem* m_PopupItem;
    CControlWZ* m_pComboBackground;
    CCtrlVScrollWZ* m_pComboScroll;
	CTimerSimple			Update_Counter_timer_;
	int						m_iArrayIndex;

    struct sPopupItem
    {
        std::string m_Text;
        RECT m_Rect;
    };
    std::vector<sPopupItem> m_PopupItemRect;

public:
    uiOptionView(InterfaceManager* ui_man);
    virtual ~uiOptionView(void);
	
	virtual VOID            Release();

    void SetActivePageIndex( int ActiveTabIndex );
	DWORD GetPageIndex() { return m_PageIndex; }

protected:
	void Init( CDrawBase * pDrawBase );
    virtual void OnShowWindow( BOOL val );
	
private:
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();
    virtual void MessageProc( SI_MESSAGE * pMessage );
	
    //void UpdateScroll();

    cOptionListControl* GetPageList() { return m_PageIndex < m_PageList.size() ? m_PageList[m_PageIndex] : NULL; }
    void SetPageIndex( DWORD Index ) { m_PageIndex = Index; }

    void MoveComboScroll( int Move );
    void MoveScroll( int Move );

    void UpdateMouseWheel();

    void InitializeOptionView();
    void UpdateControlState();

    cOptionItemBase* PickItem();

    void SetPopupItem( cOptionComboItem* pItem );
	

    //static void DrawListImage(CControlWZ* control_ptr, void* parameter);\

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

private:
	static WzID     m_wzId[eOPTION_MAX];
};

