#pragma once

#include "typedef.h"
#include <map>

//. 영역에 포함 되나 체크 해준다.
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
    kControlSoundEventControlShow = 0,  //!< 다이얼로그(CCtrlClientWZ) show 일때만 사용
    kControlSoundEventControlHide,      //!< 다이얼로그(CCtrlClientWZ) hide 일때만 사용
    kControlSoundEventLButtonClick,     //!< control에만 사용(다이얼로그에 사용되지 않음
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
//! Qualifier: OnDraw 함수에서 콜백으로 호출
//! Parameter: CControlWZ* control_ptr : 콘트롤 자신
//! Parameter: void* parameter : 셋팅시 인자로 받은 파라메터
//------------------------------------------------------------------------------
typedef void (*fn_CallbackDraw)(CControlWZ* control_ptr, void* parameter);

//------------------------------------------------------------------------------------------------------
// CControlWZ
// - Control들의 최상의 클래스
// - Image관련 처리를 해준다.
// - Control의 기본 정보 관련해서 
//------------------------------------------------------------------------------------------------------
class CControlWZ 
{
//.기본 함수들.
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

//. 외부에서 접근 할수 있는 함수들.
public:
	//. 이미지 정보 관련 함수들.
	virtual void			SetCtrlImageFormat(SI_TFORMAT* pFormat);
	virtual void			SetCtrlDetailImageFormat(ENUM_DRAWSTATE eDrawState, SI_TSUBINFO* pFormat);

    virtual SCTRL_INFO*		GetCtrlInfo() { return &m_sCtrlInfo; }
    virtual SI_TFORMAT*		GetCtrlImageFormat() { return &m_sImageInfo; }



    virtual DWORD			GetCtrlID() { return m_sCtrlInfo.m_dwCtrlID; }
    virtual	bool			SetCtrlID(DWORD dwCtrlID);

	//. 상태나 스타일 관련함수들.
    virtual ENUM_STATEWZ	GetState() { return m_eState; }
    bool					IsState(ENUM_STATEWZ eState) { return m_eState == eState; }	//. 현재 상태 체크

    virtual ENUM_DRAWSTATE	GetDrawState() { return m_eDrawState; }
    void					SetDrawState(ENUM_DRAWSTATE eState) { m_eDrawState = eState; }  //. Draw 관련 상태.	

    bool					IsStyle(DWORD dwStyle ) { return m_sCtrlInfo.m_dwStyle & dwStyle ? true : false; }
    virtual void			AddStyle(DWORD dwStyle) { m_sCtrlInfo.m_dwStyle |= dwStyle; }
    virtual	DWORD			GetCtrlStyle() { return m_sCtrlInfo.m_dwStyle; }

    //. Control 종류 관련 함수들
    virtual DWORD			GetKind() { return m_sCtrlInfo.m_dwKind; }
    void					ADDKind(DWORD dwKind) { m_sCtrlInfo.m_dwKind = dwKind; }        //. Control 종류.
    virtual bool			IsKind(DWORD dwKind) { return m_sCtrlInfo.m_dwKind & dwKind ? true : false; }


	virtual RECT			GetSize();						//. (left, top, Widht, Height);
	virtual RECT			GetSizeRect();					//. (left, top, right, bottom);


	//. 외곽 라인 관련함수들.
    virtual void			SetIsOutline(bool bOutLine) { m_blOutline = bOutLine; }
    virtual bool			GetIsOutline() { return m_blOutline; }
    virtual void			SetOutlineColor(COLORREF OutlineColor) { m_rgbOutlineColor = OutlineColor; }
    virtual COLORREF		GetOutlineColor() { return m_rgbOutlineColor; }

	//. 3D 모양 관련함수들.
    virtual void			Set3DShapeState(bool blState) { m_bl3DShapeConvex = blState; }	//.true=raise, false=press
    virtual void			SetShadowState(bool blState) { m_blShadowConvex = blState; }	//.true=raise, false=press
    virtual bool			Get3DShapeState() { return m_bl3DShapeConvex; }
    virtual bool			GetShadowState() { return m_blShadowConvex; }

	//. ToolTip 관련 함수들.
	virtual void			SetToolTipInfo(SI_ToolTip* pToolTip);
	virtual void			GetToolTipInfo(SI_ToolTip* pToolTip);

	virtual bool			CanHaveFocus() { return true; }			//. 포커스를 가지고 있나 체크 한다.	
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
	//. Mouse 관련 함수들.
	virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonDoubleClick(SI_MESSAGE* pMsg);

	virtual bool			Message_MouseRButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseRButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseRButtonDoubleClick(SI_MESSAGE* pMsg);

	virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);	

	//. KeyDown 관련 메세지.	
	virtual bool			Message_KeyDown(SI_MESSAGE* pMsg);
	virtual bool			Message_KeyUp(SI_MESSAGE* pMsg);

//. 내부사용 함수들.
public:	
	void					Release();

    CBaseManager*			GetManager() { return m_pControlMG; }
    void					SetParentDlgWZ(CDialogWZ* pParentDialog) { m_pParentWZ = pParentDialog; }   //. 부모 즉 DialogWZ 포인터 설정.
    void					SetManager(CBaseManager* pManager) { m_pControlMG = pManager; }             //. CtrlID Manager의 포인터 설정
    void					SetStringManager(CStringManager* pManager) { m_pStringMG = pManager; }      //. StringManager 의 포인터 설정

	
	//. 사이즈 관련 함수들.
	void					ChangeSize(float fPosX, float fPosY, float fWidth, float fHeight);

    //. flaot Rect 정보를 주는 함수
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

	//. 임시 할당 정보 설정 함수
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
	CDialogWZ*				m_pParentWZ;			//. 부모 클래스의 포인터
	SCTRL_INFO				m_sCtrlInfo;			//. control의 주요 정보 구조체
	SI_TFORMAT				m_sImageInfo;			//. Image 관련 정보 구조체

	ENUM_STATEWZ			m_eState;				//. Ctrl들의 상태
	ENUM_DRAWSTATE			m_eDrawState;			//. Draw State
	SI_FRECT				m_sSrcBaseRect;			

	CDrawCtrl*				m_pDraw;				//. Draw 관련 클래스의 포인터를 가지고 그림을 그린다.
	bool					m_blButtonPress;
	
	CBaseManager*			m_pControlMG;			//.자기가 포한된 매니져 클래스 포인터
	CStringManager*			m_pStringMG;			//.String 메니져 포인트를 가지고 있는다.
	HWND					m_hParentWnd;			//.부모 핸드을 가지고 있는다. 뭐 어디다 쓰겠지 뭐
	HINSTANCE				m_hInstance;
	
	COLORREF				m_rgbOutlineColor;		//. 외각 라인 Color
	bool					m_blOutline;			//. 외각 테두리 그리기 사용 여부
	
	bool					m_bl3DShapeConvex;		//. 스타일에서 CTRL_3DSHADE 설정해야 쓸수있다. 솟은 모양이냐 아니냐.
	bool					m_blShadowConvex;		//. 스타일에서 CTRL_SHADOW 설정해야 쓸수있다. 솟은 모양이냐 아니냐.

	SI_ToolTip				m_sToolTipInfo;			//. 툴팁 정보
	int						m_nTTipEnableCnt;

	bool					m_blHasFocus;			//. 포커스를 가지나 안 가지나 체크.
	DWORD					m_dwTempAllo[4];		//. 임의 할당자. 사용자가 설정한다.

	bool					m_blEnable;				//. 활성화 비활성화.
    bool                    m_bNotScale;            //  스케일링 처리 비처리


    float                   m_fTransparency;        //. 투명도

	std::string				m_CaptionString;			//  컨트롤 각자의 caption string

    //! 깊이가 있는 다이얼로그 사용시
    float m_fDepth;

    //! 해당 콘트롤의 메세지를 무시할때 사용
    bool m_bDiscardMessage;

    //! 사용자 데이터를 저장할수 있는 공간
    DWORD m_dwUserData;

    //! OnDraw 에서 콜백 함수호출
    fn_CallbackDraw m_fnCallbackDraw;
    void* m_CallbackParameter;

    RECT m_OriginPosition;

    //------------------------------------------------------------------------------ 
    //! 0은 처리 하지 않음
    int m_nSoundIndex[kControlSoundEventMax];
};

