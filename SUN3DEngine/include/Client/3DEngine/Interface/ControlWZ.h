#pragma once

#include "typedef.h"
#include <map>

//. ������ ���� �ǳ� üũ ���ش�.
#define	DEF_IntersectRect(R, P) \
(\
	(R.left<=P.x && R.right>=P.x && R.top<=P.y&&R.bottom>=P.y)\
	   ?true:false\
)

#ifdef UNICODE
    #define GetTextWZ GetTextWZW
#else
    #define GetTextWZ GetTextWZA
#endif

#ifdef UNICODE
    #define SetTextWZ SetTextWZW
#else
    #define SetTextWZ SetTextWZA
#endif

//------------------------------------------------------------------------------ 
enum ControlSoundEventType
{
    kControlSoundEventControlShow = 0,  //!< ���̾�α�(CCtrlClientWZ) show �϶��� ���
    kControlSoundEventControlHide,      //!< ���̾�α�(CCtrlClientWZ) hide �϶��� ���
    kControlSoundEventLButtonClick,     //!< control���� ���(���̾�α׿� ������ ����
    kControlSoundEventLButtonDClick,
    kControlSoundEventRButtonClick, 
    kControlSoundEventRButtonDClick,
    kControlSoundEventMax,
};

class CControlWZ;
class CDrawCtrl;
class CIDManager;
class CDialogWZ;
class CBaseManager;
class CStringManager;

//------------------------------------------------------------------------------
//! Qualifier: OnDraw �Լ����� �ݹ����� ȣ��
//! Parameter: CControlWZ* control_ptr : ��Ʈ�� �ڽ�
//! Parameter: void* parameter : ���ý� ���ڷ� ���� �Ķ����
//------------------------------------------------------------------------------
typedef void (*fn_CallbackDraw)(CControlWZ* control_ptr, void* parameter);

//------------------------------------------------------------------------------------------------------
// CControlWZ
// - Control���� �ֻ��� Ŭ����
// - Image���� ó���� ���ش�.
// - Control�� �⺻ ���� �����ؼ� 
//------------------------------------------------------------------------------------------------------
class CControlWZ 
{
//.�⺻ �Լ���.
public:
    static bool             IsDigitA(LPCSTR message);
    static bool             IsDigitW(LPCWSTR message);
    static std::string      MakeMoneyString(ULONGLONG money);

	CControlWZ();
	virtual ~CControlWZ();
	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);
	virtual void			OnDestroy();
	virtual void			OnDoAction();
    virtual void			OnDoAction_ToolTip(const POINT& cursor_pos);
    virtual bool			IsShow_ToolTip();
	virtual bool			OnDraw(bool bl);

	virtual void			OnInitialize();
	virtual bool			SendMessageWZ(SI_MESSAGE* pMsg);
    virtual void            ResetDrawState();

	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);		

    virtual void			SetStyle(DWORD dwStyle) { m_sCtrlInfo.m_dwStyle = dwStyle; }
	virtual void			UpdateWindowWZ(HWND hParent);
	virtual void			ReleaseImage();
	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);

	virtual void			ShowWindowWZ(ENUM_STATEWZ eState);
	virtual void			EnableWindowWZ(bool bState);


    bool DrawImageData( const SI_TSUBINFO& ImageInfo, const SI_FRECT& DestRect );
    void DrawControlRectangle( const SI_TSUBINFO& ImageInfo, const SI_FRECT& DestRect );
    void DrawControlStyle( const SI_TSUBINFO& ImageInfo, const SI_FRECT& DestRect );
    void DrawCallbackProc();

//. �ܺο��� ���� �Ҽ� �ִ� �Լ���.
public:
	//. �̹��� ���� ���� �Լ���.
	virtual void			SetCtrlImageFormat(SI_TFORMAT* pFormat);
	virtual void			SetCtrlDetailImageFormat(ENUM_DRAWSTATE eDrawState, SI_TSUBINFO* pFormat);

    virtual SCTRL_INFO*		GetCtrlInfo() { return &m_sCtrlInfo; }
    virtual SI_TFORMAT*		GetCtrlImageFormat() { return &m_sImageInfo; }



    virtual DWORD			GetCtrlID() { return m_sCtrlInfo.m_dwCtrlID; }
    virtual	bool			SetCtrlID(DWORD dwCtrlID);

	//. ���³� ��Ÿ�� �����Լ���.
    virtual ENUM_STATEWZ	GetState() { return m_eState; }
    bool					IsState(ENUM_STATEWZ eState) { return m_eState == eState; }	//. ���� ���� üũ

    virtual ENUM_DRAWSTATE	GetDrawState() { return m_eDrawState; }
    void					SetDrawState(ENUM_DRAWSTATE eState) { m_eDrawState = eState; }  //. Draw ���� ����.	

    bool					IsStyle(DWORD dwStyle ) { return m_sCtrlInfo.m_dwStyle & dwStyle ? true : false; }
    virtual void			AddStyle(DWORD dwStyle) { m_sCtrlInfo.m_dwStyle |= dwStyle; }
    virtual	DWORD			GetCtrlStyle() { return m_sCtrlInfo.m_dwStyle; }

    //. Control ���� ���� �Լ���
    virtual DWORD			GetKind() { return m_sCtrlInfo.m_dwKind; }
    void					ADDKind(DWORD dwKind) { m_sCtrlInfo.m_dwKind = dwKind; }        //. Control ����.
    virtual bool			IsKind(DWORD dwKind) { return m_sCtrlInfo.m_dwKind & dwKind ? true : false; }


	virtual RECT			GetSize();						//. (left, top, Widht, Height);
	virtual RECT			GetSizeRect();					//. (left, top, right, bottom);


	//. �ܰ� ���� �����Լ���.
    virtual void			SetIsOutline(bool bOutLine) { m_blOutline = bOutLine; }
    virtual bool			GetIsOutline() { return m_blOutline; }
    virtual void			SetOutlineColor(COLORREF OutlineColor) { m_rgbOutlineColor = OutlineColor; }
    virtual COLORREF		GetOutlineColor() { return m_rgbOutlineColor; }

	//. 3D ��� �����Լ���.
    virtual void			Set3DShapeState(bool blState) { m_bl3DShapeConvex = blState; }	//.true=raise, false=press
    virtual void			SetShadowState(bool blState) { m_blShadowConvex = blState; }	//.true=raise, false=press
    virtual bool			Get3DShapeState() { return m_bl3DShapeConvex; }
    virtual bool			GetShadowState() { return m_blShadowConvex; }

	//. ToolTip ���� �Լ���.
	virtual void			SetToolTipInfo(SI_ToolTip* pToolTip);
	virtual void			GetToolTipInfo(SI_ToolTip* pToolTip);

	virtual bool			CanHaveFocus() { return true; }			//. ��Ŀ���� ������ �ֳ� üũ �Ѵ�.	
	virtual void			OnFocusIn() {m_blHasFocus = true;	}		
    virtual void			OnFocusOut() {m_blHasFocus = false; OnInitialize(); }

    virtual DWORD			GetFontID() { return 0; }
	virtual void			SetFontID(DWORD dwFontID){};			//. 

    // [2010-3-10 by i4u4me]  
    virtual bool            IsTextReducing() const { return false; }
    virtual void            SetTextReducing(bool bReducing) {}
    virtual bool            IsMoneyType() const { return false; }
    virtual void            SetMoneyType(bool bMoneyType) {}
    virtual bool            IsUseDefault() const { return false; }
    virtual void            SetUseDefault(bool bDfault) {}

    virtual	void			GetTextWZA(OUT LPSTR pszText, int nLength){}
    virtual	void			GetTextWZW(OUT LPWSTR pwszText, int nLength){}

    virtual	void			SetTextWZA(LPCSTR pszText, bool bSelected=false){}
    virtual	void			SetTextWZW(LPCWSTR pwszText, bool bSelected=false){}
    virtual void			SetTextID(DWORD dwStrID){}
    virtual	DWORD			GetTextID(){ return 0; }

    virtual	void			SetMoney(ULONGLONG money);
    virtual	void			SetMoneyWithSymbol(LONGLONG money);

    virtual void            SetTransprency( float fTransperency ) { m_fTransparency = fTransperency; }

protected:
	//. Mouse ���� �Լ���.
	virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonDoubleClick(SI_MESSAGE* pMsg);

	virtual bool			Message_MouseRButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseRButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseRButtonDoubleClick(SI_MESSAGE* pMsg);

	virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);	

	//. KeyDown ���� �޼���.	
	virtual bool			Message_KeyDown(SI_MESSAGE* pMsg);
	virtual bool			Message_KeyUp(SI_MESSAGE* pMsg);

//. ���λ�� �Լ���.
public:	
	void					Release();

    CBaseManager*			GetManager() { return m_pControlMG; }
    void					SetParentDlgWZ(CDialogWZ* pParentDialog) { m_pParentWZ = pParentDialog; }   //. �θ� �� DialogWZ ������ ����.
    void					SetManager(CBaseManager* pManager) { m_pControlMG = pManager; }             //. CtrlID Manager�� ������ ����
    void					SetStringManager(CStringManager* pManager) { m_pStringMG = pManager; }      //. StringManager �� ������ ����

	
	//. ������ ���� �Լ���.
	void					ChangeSize(float fPosX, float fPosY, float fWidth, float fHeight);

    //. flaot Rect ������ �ִ� �Լ�
    const SI_FRECT&         GetFSize() const { return m_sCtrlInfo.m_sSizeRect; }
    virtual float           GetPosX() { return m_sCtrlInfo.m_sSizeRect.fTopX; }
    virtual float           GetPosY() { return m_sCtrlInfo.m_sSizeRect.fTopY; }
    virtual float			GetSizeWidth() { return m_sCtrlInfo.m_sSizeRect.fWidth; }
    virtual float			GetSizeHeight() { return m_sCtrlInfo.m_sSizeRect.fHeight; }
    virtual RECT            GetOriginRect() { return m_OriginPosition; }
    virtual void            SetNotScaling(bool set) { m_bNotScale = set;}
    virtual bool            GetNotScaling()    { return m_bNotScale; }

    void					SetPosX(float fx) { m_sCtrlInfo.m_sSizeRect.fTopX = fx; }
    void					SetPosY(float fy) { m_sCtrlInfo.m_sSizeRect.fTopY = fy; }
    virtual void			SetSizeWidth(long lnWidth) { m_sCtrlInfo.m_sSizeRect.fWidth = (float)lnWidth; }
    virtual void			SetSizeHeight(long lnHeight) { m_sCtrlInfo.m_sSizeRect.fHeight = (float)lnHeight; }

    virtual bool            IsDiscardMessage() const { return m_bDiscardMessage; }
    virtual void            SetDiscardMessage(bool val) { m_bDiscardMessage = val; }

    float                   GetDepth() const { return m_fDepth; }
    void                    SetDepth(float val) { m_fDepth = val; }

	//. �ӽ� �Ҵ� ���� ���� �Լ�
	DWORD					GetTempAlloc(int nIndex){if(nIndex < 4)return m_dwTempAllo[nIndex];return 0xffffffff;}
	void					SetTempAlloc(int nIndex, DWORD dwTemp ){if(nIndex < 4) m_dwTempAllo[nIndex]=dwTemp;}

	virtual void			ChangeCaptionString();

    virtual DWORD           GetUserData() const { return m_dwUserData; }
    virtual void            SetUserData(DWORD user_data) { m_dwUserData = user_data; }
    virtual void            SetCallbackDraw(fn_CallbackDraw fn, void* parameter) { m_fnCallbackDraw = fn; m_CallbackParameter = parameter; }
    virtual int             GetSoundIndex(ENUM_RESULT_MSG result_msg) const;
    virtual int             GetSoundIndex(ControlSoundEventType soundevent_type) const;
    virtual void            SetSoundIndex(ControlSoundEventType soundevent_type, int sound_index);

private:
    virtual void            OnDraw_Tile_XY(const SI_FRECT& source_rect, const SI_FRECT& destination_rect, void* texture_handle);
    virtual void            OnDraw_Tile_X(const SI_FRECT& source_rect, const SI_FRECT& destination_rect, void* texture_handle);
    virtual void            OnDraw_Tile_Y(const SI_FRECT& source_rect, const SI_FRECT& destination_rect, void* texture_handle);


protected:
	CDialogWZ*				m_pParentWZ;			//. �θ� Ŭ������ ������
	SCTRL_INFO				m_sCtrlInfo;			//. control�� �ֿ� ���� ����ü
	SI_TFORMAT				m_sImageInfo;			//. Image ���� ���� ����ü

	ENUM_STATEWZ			m_eState;				//. Ctrl���� ����
	ENUM_DRAWSTATE			m_eDrawState;			//. Draw State
	SI_FRECT				m_sSrcBaseRect;			

	CDrawCtrl*				m_pDraw;				//. Draw ���� Ŭ������ �����͸� ������ �׸��� �׸���.
	bool					m_blButtonPress;
	
	CBaseManager*			m_pControlMG;			//.�ڱⰡ ���ѵ� �Ŵ��� Ŭ���� ������
	CStringManager*			m_pStringMG;			//.String �޴��� ����Ʈ�� ������ �ִ´�.
	HWND					m_hParentWnd;			//.�θ� �ڵ��� ������ �ִ´�. �� ���� ������ ��
	HINSTANCE				m_hInstance;
	
	COLORREF				m_rgbOutlineColor;		//. �ܰ� ���� Color
	bool					m_blOutline;			//. �ܰ� �׵θ� �׸��� ��� ����
	
	bool					m_bl3DShapeConvex;		//. ��Ÿ�Ͽ��� CTRL_3DSHADE �����ؾ� �����ִ�. ���� ����̳� �ƴϳ�.
	bool					m_blShadowConvex;		//. ��Ÿ�Ͽ��� CTRL_SHADOW �����ؾ� �����ִ�. ���� ����̳� �ƴϳ�.

	SI_ToolTip				m_sToolTipInfo;			//. ���� ����
	int						m_nTTipEnableCnt;

	bool					m_blHasFocus;			//. ��Ŀ���� ������ �� ������ üũ.
	DWORD					m_dwTempAllo[4];		//. ���� �Ҵ���. ����ڰ� �����Ѵ�.

	bool					m_blEnable;				//. Ȱ��ȭ ��Ȱ��ȭ.
    bool                    m_bNotScale;            //  �����ϸ� ó�� ��ó��


    float                   m_fTransparency;        //. ����

	std::string				m_CaptionString;			//  ��Ʈ�� ������ caption string

    //! ���̰� �ִ� ���̾�α� ����
    float m_fDepth;

    //! �ش� ��Ʈ���� �޼����� �����Ҷ� ���
    bool m_bDiscardMessage;

    //! ����� �����͸� �����Ҽ� �ִ� ����
    DWORD m_dwUserData;

    //! OnDraw ���� �ݹ� �Լ�ȣ��
    fn_CallbackDraw m_fnCallbackDraw;
    void* m_CallbackParameter;

    RECT m_OriginPosition;

    //------------------------------------------------------------------------------ 
    //! 0�� ó�� ���� ����
    int m_nSoundIndex[kControlSoundEventMax];
};

