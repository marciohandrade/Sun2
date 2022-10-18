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
    //! Qualifier: DrawListSell �Լ����� �ݹ����� ȣ��
    //! Parameter: CCtrlListWZ* list_control : ����Ʈ ��Ʈ��
    //! Parameter: CListSellData* sell_data : �׷��� ��
    //! Parameter: int sell_index : �׷��� ��(���ڵ�) ��ȣ
    //! Parameter: RECT& render_area : �׷��� ����
    typedef void (*fn_CallbackListBoxSellDraw)(CCtrlListWZ* list_control, CListSellData* sell_data, int sell_index, RECT& render_area);

    //! List Sell ���� ����ü.
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

        //! ����� �����͸� �����Ҽ� �ִ� ����
        void* user_data;

        //! DrawListSell ���� �ݹ� �Լ�ȣ��
        //! callback_draw != NULL �̸� sell�� �ִ� ��� ������ �ݹ鿡�� �׸�
        fn_CallbackListBoxSellDraw callback_draw;
    };

    struct CListData
    {//. ����Ʈ ����Ÿ ����ü.
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
        //. ����Ʈ ������ �߰� �ϴ� �Լ�.
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
        //. ����Ʈ ������ �߰� �ϴ� �Լ�.
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

        DWORD			m_nMapCnt;			//!< Map ī��Ʈ
        ListBoxSellMap	m_stdMapSellData;   //!< CListSellData
        VOID*			m_pVoid;			//!< ������ ���� ������ �ִ´�.	
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
	virtual void			SetReportUseCnt(short nRowCnt);								//. Report ��� ������ �����Ѵ�.
	virtual short			GetReportUseCnt();
	
	virtual void			SetSellHeight(int nHeight);									//. ���� ���̰��� �����Ѵ�. ����Ʈ�� 20 �̴�.
	virtual int				GetSellheight();											//. �� ���̰��� �������ش�.

	virtual int				InsertItemBack();											//. �ڿ� List Info�� ����� �����Ѵ�.
	virtual int				InsertItemFront();											//. �տ� List Info�� ����� �����Ѵ�.

	virtual bool			SetItemWZA(int nIndex, LPCSTR pszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor); 
	virtual bool			SetItemWZA(int nIndex, int nSubIndex, LPCSTR pszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor); 

	virtual bool			SetItemWZW(int nIndex, LPCWSTR  pwszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor); 
	virtual bool			SetItemWZW(int nIndex, int nSubIndex, LPCWSTR pwszText, DWORD dwFontID, DWORD dwTextStyle, COLORREF BKColor, COLORREF TextColor, COLORREF SelTextColor); 

	virtual	bool			SetItemTextWZA(int nIndex, LPCSTR pszSZ);						//. List Index, �ǵڿ� Text ����
	virtual	bool			SetItemTextWZA(int nIndex, int nSubIndex, LPCSTR pszSZ);		//. List�� Text ����

	virtual	bool			SetItemTextWZW(int nIndex, LPCWSTR  pwszSZ);					//. List Index, �ǵڿ� Text ����
	virtual	bool			SetItemTextWZW(int nIndex, int nSubIndex, LPCWSTR  pwszSZ);		//. List�� Text ����
			
	virtual bool			SetItemBKColor(int nIndex, int nSubIndex, COLORREF BKColor);
	virtual bool			SetItemTextColor(int nIndex, int nSubIndex, COLORREF TextColor);
	virtual bool			SetItemSelTextColor(int nIndex, int nSubIndex, COLORREF SelTextColor);

    virtual bool			SetItemUserData(int nIndex, int nSubIndex, void* user_data);
    virtual void*			GetItemUserData(int nIndex, int nSubIndex);
    virtual bool			SetItemCallbackDraw(int nIndex, int nSubIndex, fn_CallbackListBoxSellDraw fn);

	virtual bool			SetItemFontID(int nIndex, int nSubIndex, DWORD dwFontID);
	virtual bool			SetItemFontID(int nIndex, int nSubIndex, LPCSTR pszFontID){WzAssert(!"������ : SetItemFontID(int nIndex, int nSubIndex, DWORD dwFontID)�� ��� �ٶ�!");return false;}	//.�ӽ� �ڵ� ������ ����

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
	virtual void			SetRoportButtonImageFormat(SI_TFORMAT* pFormat);			//. Roport ��ư�� �̹��� ������ ���� ���ش�.
	virtual SI_TFORMAT*		GetRoportButtonImageFormat();								//. Report ��ư�� �̹��� ������ �Ѱ��ֳ�.

	virtual void			SetCurSel(int nSel);										//. �� ����
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

	//. color�� ���� ���ִ� �Լ���.
	virtual void			SetColorText(COLORREF rgb);
	COLORREF                GetColorText(){return m_rgbTextColor;}

	virtual void			SetColorSelBar(COLORREF rgb);
	virtual void			SetColorSelBarOnText(COLORREF rgb);

	//. FontID ���� �Լ���.
	virtual void			SetFontID(DWORD dwFontID);
	virtual DWORD			GetFontID();

	virtual void			SetUnuseSelBar(bool bl);

	//. �۾� ����� ���� ����.(�⺻, �׸���, �׵θ�)
	virtual void			SetTextKind(ENUM_TEXT_KIND eTK);
	virtual ENUM_TEXT_KIND  GetTextKind();

	void					SetTextOutlineColor(COLORREF rgbColor){m_rgbTextOutlineColor = rgbColor;}
	COLORREF				GetTextOutlineColor(){return m_rgbTextOutlineColor;}

	//. Report ���� �Լ���.
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
	//. Mouse ���� �Լ���.
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
	int						m_nReportUseCnt;		//.Report�ǰ���.
	//int						m_nLDataCnt;			//.List data ����.
	int						m_nCurSelBT;			//.Select�� Report ��ư.
	//int						m_nDrawSelCnt;			//.�׸��� �ִ� �ټ�.
	int						m_nStartLineGapTop;		//.ó�� ��ο�� �δ� ����
	int						m_nStartLineGapLeft;	// ���� gap
	int						m_nStartLineGapRight;	// ������ gap

	int						m_nCurSel;				//.���� ���� �� ��.
    int						m_nOverSel;				//.���� ���콺 �� ��.
	std::map<DWORD,VOID*>	m_StdButtonMap;			//.Button Map.
	STD_SORT_MULTIMAP		m_StdSortMap;			//.�������ֱ� ���� ��.
	std::list<VOID*>		m_StdDataList;			//.���� ����ü�� ������ �ִ´�.
	
	DWORD					m_dwFontID;
	BOOL					m_blGreater;			//. true: ��������, false: �ø�����.
	BOOL					m_blSelChange;			//. �� ���� ������ Ŭ�� ������ True;

	POINT					m_poLBDown;				//. ���콺 LButtonDown Point

	HCURSOR					m_hOldCursor;	
	SI_FRECT				m_sSrcRect;

	bool					m_blUseReport;			//. Report button�� ����ϳ� OR �� �ϳ�.
	COLORREF				m_rgbTextColor;			//. ��¥ ����.
	COLORREF				m_rgbSelBarColor;		//. ���ù� ����.
	COLORREF				m_rgbSelBarOnTextColor;	//. ���ùٰ� ���� ������ �۾� ����.
	COLORREF				m_rgbTextOutlineColor;	
	
	bool					m_blUnuseSelBar;
	bool					m_blHideReportRender;
	int						m_nSellHeight;
	int						m_nReportHeight;		//. ����Ʈ ��ư ����.

	ENUM_TEXT_KIND			m_eTextKind;

	bool					m_bResportSortOn;
	bool					m_bUseRButtonSelect;

    bool                    draw_underline_;

    //! ���ڿ��� ĸ�� ������ �Ѿ� ������ "..." ���� ǥ�� �Ұ�������
    bool                    m_bTextReducing;

};