#pragma once

#include "ControlWZ.h"

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlListWZ Class
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef std::multimap <std::string,VOID*> STD_SORT_MULTIMAP;
typedef std::pair <std::string,VOID*> STD_SORT_PAIR_VAIUE;

#ifdef UNICODE
#define SetItemWZ SetItemWZW
#else
#define SetItemWZ SetItemWZA
#endif

#ifdef UNICODE
#define SetItemTextWZ SetItemTextWZW
#else
#define SetItemTextWZ SetItemTextWZA
#endif

#ifdef UNICODE
#define SetReportButtonInfo SetReportButtonInfoW
#else
#define SetReportButtonInfo SetReportButtonInfoA
#endif

#ifdef UNICODE
#define GetReportButtonInfo GetReportButtonInfoW
#else
#define GetReportButtonInfo GetReportButtonInfoA
#endif

class CCtrlListWZ : public CControlWZ  
{
public:
    struct CListSellData;

    typedef std::map<DWORD, CListSellData*> ListBoxSellMap;

    //------------------------------------------------------------------------------
    //! Qualifier: DrawListSell 함수에서 콜백으로 호출
    //! Parameter: CCtrlListWZ* list_control : 리스트 콘트롤
    //! Parameter: CListSellData* sell_data : 그려질 셀
    //! Parameter: int sell_index : 그려질 셀(레코드) 번호
    //! Parameter: RECT& render_area : 그려질 영역
    typedef void (*fn_CallbackListBoxSellDraw)(CCtrlListWZ* list_control, CListSellData* sell_data, int sell_index, RECT& render_area);

    //! List Sell 정보 구조체.
    struct CListSellData
    {
        CListSellData()
        {
            BKColor = RGBA(0,0,0,0);
            SelFontColor = RGBA(217,128,0,255);
            FontColor = RGBA(255,255,255,255);

            dwTextStyle = DT_LEFT | DT_VCENTER;
            dwFontID = 0;

            user_data = NULL;
            callback_draw = NULL;
        }

        COLORREF BKColor;
        COLORREF FontColor;
        COLORREF SelFontColor;

        STD_STRING stdStrText;

        DWORD	dwFontID;

        DWORD	dwTextStyle;

        //! 사용자 데이터를 저장할수 있는 공간
        void* user_data;

        //! DrawListSell 에서 콜백 함수호출
        //! callback_draw != NULL 이면 sell에 있는 모든 정보를 콜백에서 그림
        fn_CallbackListBoxSellDraw callback_draw;
    };

    struct CListData
    {//. 리스트 데이타 구조체.
        CListData()
        {
            m_nMapCnt = 0;
            m_pVoid = NULL;
        }
        ~CListData()
        {
            ListBoxSellMap::iterator it = m_stdMapSellData.begin();
            ListBoxSellMap::iterator end = m_stdMapSellData.end();

            for(; it != end; ++it)
            {
                CListSellData* pSell = (CListSellData*)it->second;
                if(pSell)
                {
                    delete pSell;
                    pSell = NULL;
                }
            }
            m_stdMapSellData.clear();
        }
        //. 리스트 정보를 추가 하는 함수.
        bool Add(LPCSTR pszSZ,
                DWORD dwFontID=0,
                DWORD dwStyle=DT_LEFT|DT_VCENTER,
                COLORREF rgbBK=RGBA(255,255,255,255),
                COLORREF rgbText=RGBA(0,0,0,255),
                COLORREF rgbSelText=RGBA(255,255,255,255))
        {		
            CListSellData* pSell = new CListSellData;

            pSell->stdStrText = pszSZ;	
            pSell->dwFontID = dwFontID;
            pSell->dwTextStyle = dwStyle;
            pSell->BKColor = rgbBK;
            pSell->FontColor = rgbText;
            pSell->SelFontColor = rgbSelText;

            m_stdMapSellData[m_nMapCnt++] = pSell;	
            return true;
        }
        //. 리스트 정보를 추가 하는 함수.
        bool Add(int nSubIndex, 
            LPCSTR pszSZ,
            DWORD dwFontID = 0,
            DWORD dwStyle = DT_LEFT|DT_VCENTER,
            COLORREF rgbBK = RGBA(0,0,0,0),
            COLORREF rgbText = RGBA(255,255,255,255),
            COLORREF rgbSelText = RGBA(217,128,0,255))
        {
            ListBoxSellMap::iterator it = m_stdMapSellData.find(nSubIndex);
            CListSellData* pSell = NULL;
            if(it == m_stdMapSellData.end())
            {
                pSell = new CListSellData;
            }
            else
            {
                pSell = (CListSellData*)it->second;
            }
            pSell->stdStrText = pszSZ;	
            pSell->dwFontID = dwFontID;
            pSell->dwTextStyle = dwStyle;
            pSell->BKColor = rgbBK;
            pSell->FontColor = rgbText;
            pSell->SelFontColor = rgbSelText;

            m_stdMapSellData[nSubIndex] = pSell;
            return true;
        }
        void    SetDataPtr(VOID* ptr) { m_pVoid = ptr; }
        VOID*   GetDataPtr() { return m_pVoid; }

        DWORD			m_nMapCnt;			//!< Map 카운트
        ListBoxSellMap	m_stdMapSellData;   //!< CListSellData
        VOID*			m_pVoid;			//!< 임이의 값을 가지고 있는다.	
    };

public:
	CCtrlListWZ();
	virtual ~CCtrlListWZ();

	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);
	virtual void			OnDestroy();
	
	virtual bool			OnDraw(bool bl);
	
	virtual void			OnInitialize();

	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);	
	
	virtual void			UpdateWindowWZ(HWND hParent);
	virtual void			ReleaseImage();

	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);

	virtual void			EnableWindowWZ(bool bState);

public:
	virtual void			SetReportUseCnt(short nRowCnt);								//. Report 사용 갯수를 설정한다.
	virtual short			GetReportUseCnt();
	
	virtual void			SetSellHeight(int nHeight);									//. 샐의 높이값을 설정한다. 디폴트는 20 이다.
	virtual int				GetSellheight();											//. 셀 높이값을 리턴해준다.

	virtual int				InsertItemBack();											//. 뒤에 List Info를 만들어 삽입한다.
	virtual int				InsertItemFront();											//. 앞에 List Info를 만들어 삽입한다.

	virtual bool			SetItemWZA(int nIndex, LPCSTR pszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor); 
	virtual bool			SetItemWZA(int nIndex, int nSubIndex, LPCSTR pszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor); 

	virtual bool			SetItemWZW(int nIndex, LPCWSTR  pwszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor); 
	virtual bool			SetItemWZW(int nIndex, int nSubIndex, LPCWSTR pwszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor); 

	virtual	bool			SetItemTextWZA(int nIndex, LPCSTR pszSZ);						//. List Index, 맨뒤에 Text 삽입
	virtual	bool			SetItemTextWZA(int nIndex, int nSubIndex, LPCSTR pszSZ);		//. List의 Text 삽입

	virtual	bool			SetItemTextWZW(int nIndex, LPCWSTR  pwszSZ);					//. List Index, 맨뒤에 Text 삽입
	virtual	bool			SetItemTextWZW(int nIndex, int nSubIndex, LPCWSTR  pwszSZ);		//. List의 Text 삽입
			
	virtual bool			SetItemBKColor(int nIndex, int nSubIndex, COLORREF BKColor);
	virtual bool			SetItemTextColor(int nIndex, int nSubIndex, COLORREF TextColor);
	virtual bool			SetItemSelTextColor(int nIndex, int nSubIndex, COLORREF SelTextColor);

    virtual bool			SetItemUserData(int nIndex, int nSubIndex, void* user_data);
    virtual void*			GetItemUserData(int nIndex, int nSubIndex);
    virtual bool			SetItemCallbackDraw(int nIndex, int nSubIndex, fn_CallbackListBoxSellDraw fn);

	virtual bool			SetItemFontID(int nIndex, int nSubIndex, DWORD dwFontID);
	virtual bool			SetItemFontID(int nIndex, int nSubIndex, LPCSTR pszFontID){WzAssert(!"사용안함 : SetItemFontID(int nIndex, int nSubIndex, DWORD dwFontID)로 사용 바람!");return false;}	//.임시 코드 조만간 삭제

	virtual bool			SetItemTextStyle(int nIndex, int nSubIndex, DWORD dwTextStyle);

	virtual bool			SetItemData(int nIndex, VOID* pVoid);
	virtual VOID*			GetItemData(int nIndex);	
    virtual LPCTSTR		    GetItemText(int nIndex, int nSubIndex);

	virtual BOOL			DeleteItem(int nIndex);
	virtual void			DeleteItemAll();

	virtual	void			SetReportButtonInfoA(int nIndex, int nLength, LPCSTR pszText, const DWORD dwTextStyle = 0);
	virtual	void			SetReportButtonInfoW(int nIndex, int nLength, LPCWSTR pwszText, const DWORD dwTextStyle = 0);

	virtual bool			GetReportButtonInfoA(int nIndex, OUT int* pnLength, OUT LPSTR pszText, int nBufferLength, OUT DWORD* pdwTextStyle = NULL);	
	virtual bool			GetReportButtonInfoW(int nIndex, OUT int* pnLength, OUT LPWSTR pwszText, int nBufferLength, OUT DWORD* pdwTextStyle = NULL);
	
public:	
	virtual void			SelectReportButton(int nSel);	
	virtual void			SetBeginDrawIndex(int nBegin);
    virtual int             GetBeginDrawIndex() { return m_nBegin; }
	virtual void			SetRoportButtonImageFormat(SI_TFORMAT* pFormat);			//. Roport 버튼의 이미지 정보를 셋팅 해준다.
	virtual SI_TFORMAT*		GetRoportButtonImageFormat();								//. Report 버튼의 미미지 정보를 넘겨주낟.

	virtual void			SetCurSel(int nSel);										//. 셀 선택
	virtual int				GetCurSel();
    virtual void			SetOverSel(int nSel) { m_nOverSel = nSel; }
    virtual int				GetOverSel() { return m_nOverSel; }

	virtual void			SetReportIsOutline(bool blIs);
	virtual bool			GetReportIsOutline();
	virtual void			SetReportOutlineColor(COLORREF rgb);
	virtual COLORREF		GetReportOutlineColor();

	virtual bool			GetUseReport();
	virtual void			SetUseReport(bool blUse);
	virtual int				GetListRowCnt();									
    virtual int				GetDrawRowCount();

	//. color를 변경 해주는 함수들.
	virtual void			SetColorText(COLORREF rgb);
	COLORREF                GetColorText(){return m_rgbTextColor;}

	virtual void			SetColorSelBar(COLORREF rgb);
	virtual void			SetColorSelBarOnText(COLORREF rgb);

	//. FontID 관련 함수들.
	virtual void			SetFontID(DWORD dwFontID);
	virtual DWORD			GetFontID();

	virtual void			SetUnuseSelBar(bool bl);

	//. 글씨 모양의 종류 설정.(기본, 그림자, 테두리)
	virtual void			SetTextKind(ENUM_TEXT_KIND eTK);
	virtual ENUM_TEXT_KIND  GetTextKind();

	void					SetTextOutlineColor(COLORREF rgbColor){m_rgbTextOutlineColor = rgbColor;}
	COLORREF				GetTextOutlineColor(){return m_rgbTextOutlineColor;}

	//. Report 관련 함수들.
	void					SetHideReportRender(bool blFlag){m_blHideReportRender=blFlag;}
	bool					GetHideReportRender(){return m_blHideReportRender;}

	virtual void			SetReportHeight(int nHeight);
	int						GetReportHeight(){return m_nReportHeight;}

	void					SetReportSortOn(bool bOn){ m_bResportSortOn = bOn; }
	bool					GetReportSortOn(){ return m_bResportSortOn; }

	virtual	void			SetStartLineGapTop( int gap )		{ m_nStartLineGapTop = gap; }
	virtual void			SetStartLineGapLeft( int gap )		{ m_nStartLineGapLeft = gap; }
	virtual void			SetStartLineGapRight( int gap )		{ m_nStartLineGapRight = gap; }

	void					SetUseRButtonSelect(bool bUse)		{	m_bUseRButtonSelect = bUse; }
    void                    SetDrawTextUnderLineOverSell(bool draw_underline) { draw_underline_ = draw_underline;}
    bool                    GetDrawTextUnderLineOverSell()      { return draw_underline_; }
    
    virtual bool            IsTextReducing() const { return m_bTextReducing; }
    virtual void            SetTextReducing(bool bReducing) { m_bTextReducing = bReducing; }
protected:
	//. Mouse 관련 함수들.
	virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);

	virtual bool			Message_MouseRButtonDown(SI_MESSAGE* pMsg);

	VOID*					GetReportButtonInfoPtr(int nIndex);
	VOID*					GetListInfoDataToIndex(int nIndex);	
	int						UpdateListDate(VOID* pLD);
	int						GetIndexToListDataPtr(VOID* pLD);
	void					DrawListSell(int nSellCnt, CListSellData* pSell, RECT rtRect);
	void					DrawListData(int nSellCnt, VOID* pVoid, int nHeight);
	
	//int						ComputeCurSel(POINT point);
    int						ComputeSel(POINT point);

protected:
	int						m_nBegin;				//.Draw Begin.
	int						m_nEnd;					//.Draw End.
	int						m_nReportUseCnt;		//.Report의갯수.
	//int						m_nLDataCnt;			//.List data 갯수.
	int						m_nCurSelBT;			//.Select된 Report 버튼.
	//int						m_nDrawSelCnt;			//.그릴수 있는 줄수.
	int						m_nStartLineGapTop;		//.처음 드로우시 두는 간격
	int						m_nStartLineGapLeft;	// 왼쪽 gap
	int						m_nStartLineGapRight;	// 오른쪽 gap

	int						m_nCurSel;				//.현재 선택 된 셀.
    int						m_nOverSel;				//.현재 마우스 된 셀.
	std::map<DWORD,VOID*>	m_StdButtonMap;			//.Button Map.
	STD_SORT_MULTIMAP		m_StdSortMap;			//.정렬해주기 위한 맵.
	std::list<VOID*>		m_StdDataList;			//.정보 구조체를 가지고 있는다.
	
	DWORD					m_dwFontID;
	BOOL					m_blGreater;			//. true: 내림차순, false: 올림차순.
	BOOL					m_blSelChange;			//. 셀 변경 역역을 클릭 했을때 True;

	POINT					m_poLBDown;				//. 마우스 LButtonDown Point

	HCURSOR					m_hOldCursor;	
	SI_FRECT				m_sSrcRect;

	bool					m_blUseReport;			//. Report button을 사용하냐 OR 안 하나.
	COLORREF				m_rgbTextColor;			//. 글짜 색깔.
	COLORREF				m_rgbSelBarColor;		//. 선택바 색깔.
	COLORREF				m_rgbSelBarOnTextColor;	//. 선택바가 위에 있을때 글씨 색깔.
	COLORREF				m_rgbTextOutlineColor;	
	
	bool					m_blUnuseSelBar;
	bool					m_blHideReportRender;
	int						m_nSellHeight;
	int						m_nReportHeight;		//. 리포트 버튼 높이.

	ENUM_TEXT_KIND			m_eTextKind;

	bool					m_bResportSortOn;
	bool					m_bUseRButtonSelect;

    bool                    draw_underline_;

    //! 문자열이 캡션 영역을 넘어 갔을대 "..." 으로 표시 할건지여부
    bool                    m_bTextReducing;

};