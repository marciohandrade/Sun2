#pragma once

#include "CtrlEditWZ.h"
#include "CtrlVScrollWZ.h"
#include <usp10.h>
#include <dimm.h>
#include <vector>
#include <tchar.h>

class CCtrlDxEditBox :	public CCtrlBaseEdit
{
	friend class CExternalApiInitializer;
public:
	CCtrlDxEditBox(void);
	virtual ~CCtrlDxEditBox(void);

	virtual void			OnInitialize();

	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);
	virtual void			OnDestroy();
	virtual void			OnDoAction();                           // OnUpdate
	virtual bool			OnDraw(bool bl);
		
	//. �ε� / ���� ���� �Լ�.
	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);

	virtual void			SetStyle(DWORD dwStyle);
	virtual void			UpdateWindowWZ(HWND hParent);			//. �̹��� ���� ������ �Ҷ� ȣ��.
	virtual void			ReleaseImage();				
	
	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
	virtual bool			SendMessageWZ(SI_MESSAGE* pMsg);

public:
	//. ��Ʈ ���� �Լ�.
	virtual void			SetFontID(DWORD dwFontID);				//. Font ID �����Ѵ�.
	virtual DWORD			GetFontID();							//. Font ID ���´�.	
		
	//. ���� ���� �Լ�.
	virtual void			SetColorCaret(COLORREF color);			//. ĳ�� ���� �����Ѵ�.
	virtual void			SetColorText(COLORREF color);			//. ���ڻ��� �����Ѵ�.
	virtual void			SetColorSelBack(COLORREF color);		//. ���� ������ ���� �����Ѵ�.
	virtual void			SetColorSelText(COLORREF color);		//. ���� ������ ���ڻ��� �����Ѵ�.

	virtual COLORREF		GetFontColor();
	virtual COLORREF		GetCaretColor(){return m_CaretColor;}
	virtual COLORREF		GetSelBackColor(){return m_SelBackColor;}
	virtual COLORREF		GetSelTextColor(){return m_SelTextColor;}
	
	//. �ؽ�Ʈ ���� �Լ���.	
	virtual void			SetTextWZA( LPCSTR pszText,bool bSelected=false);
	virtual void			SetTextWZW( LPCWSTR pwszText,bool bSelected=false);
	virtual	void			GetTextWZA( OUT LPSTR pszText, int nLength);	
	virtual	void			GetTextWZW( OUT LPWSTR pwszText, int nLength);	
	virtual void			SetLimitText(DWORD dwLimit);			//. ������ ���̸� �����Ѵ�.
    virtual void			SetLimitText_MultibyteCheck(bool is_val); 
	virtual void			SetLimitLine(int iLimit);				 //. ������ �ٶ��θ� �����Ѵ�.
	virtual void			ClearText();

	//. ��Ŀ�� ���� �Լ���.
	virtual void			SetFocusWZ();
	virtual bool			CanHaveFocus();	
	
	virtual void			SetTextNCRect(int nTopThick, int nBottomThick, int nLeftThick, int nRightThick);	//. �۾��� ����� ����. �ش� �β� ��ŭ ��ü ũ�⿡�� ���ش�.

	//. �۾� ����� ���� ����.(�⺻, �׸���, �׵θ�)
	virtual void			SetTextKind(ENUM_TEXT_KIND eTK);
	virtual ENUM_TEXT_KIND  GetTextKind();

	virtual void			SetTextOutlineColor(COLORREF rgbColor){m_TextOutlineColor = rgbColor;}
	virtual COLORREF		GetTextOutlineColor(){return m_TextOutlineColor;}
    virtual bool            IsNumberic() const { return m_bNumberic; }
    virtual void            SetNumberic(bool val) { m_bNumberic = val; }
	
public:
	
	void					ParseFloatArray( float *pNumbers, int nCount );
    void					SetTextFloatArray( const float *pNumbers, int nCount );
	
protected:
	virtual BOOL			IsLimitLine();
	virtual BOOL			CheckLimitLine();
	void					PlaceCaret( int nCP );
	
    void					DeleteSelectionText();
    void					ResetCaretBlink();
    void					CopyToClipboard();
    virtual	void			PasteFromClipboard();
	int						GetFontHeight();
	
protected:
	
	COLORREF				m_TextColor;			//.�⺻ �ؽ�Ʈ �� ����.	
	COLORREF				m_CaretColor;			//.ĳ���� �� ����.
	COLORREF				m_SelBackColor;			//.���õ� ������ �� ����.
	COLORREF				m_SelTextColor;			//.���õ� ������ �ؽ�Ʈ ������.


	DWORD					m_dwFontID;
	RECT					m_rcText;				// Bounding rectangle for the text
	RECT					m_rcTextBorder;			// Text Rect Border Thickness
	bool					m_bCaretOn;				// Flag to indicate whether caret is currently visible
    int						m_nCaret;				// Caret position, in characters
    bool					m_bInsertMode;			// If true, control is in insert mode. Else, overwrite mode.
    int						m_nSelStart;			// Starting position of the selection. The caret marks the end.
    int						m_nFirstVisible;		// First visible character in the edit control

	 // Mouse-specific
    bool					m_bMouseDrag;			// True to indicate drag in progress	
	LOGFONTA				m_lplf;					// LogFont 
	long					m_lnLimitTextLength;	//. �Է� TextLength�� �����Ѵ�.
    //! �Է� ���ڿ� ������ ������ ��Ƽ����Ʈ ���·� ��ȯ�ؼ� üũ �Ұ��� ����
    //! Ŭ��(���ø����̼�)�� ��Ƽ ����Ʈ���϶��� ����Ѵ�.(���� ��Ƽ����Ʈ ������Ʈ)
    //! �ִ� 2048 ���ڿ������� üũ �Ѵ�.
    bool                    m_bLimitTextLength_Multibyte_Check; //! 

	int						m_iLimitLine;			// �ٶ������Ѽ�
	BOOL					m_bUseLimitLine;		// �ٶ��λ���Ұ���
	DWORD					m_dwTextSytle;
	ENUM_TEXT_KIND			m_eTextKind;
	COLORREF				m_TextOutlineColor;	

    bool                    m_bNumberic;

public:	
    //-----------------------------------------------------------------------------
    // CUniBuffer class for the edit control
    //-----------------------------------------------------------------------------
    class CUniBuffer
    {
        friend class CExternalApiInitializer;

        // static member

        // Empty implementation of the Uniscribe API
        static HRESULT WINAPI Dummy_ScriptApplyDigitSubstitution( const SCRIPT_DIGITSUBSTITUTE*, SCRIPT_CONTROL*, SCRIPT_STATE* ) { return E_NOTIMPL; }
        static HRESULT WINAPI Dummy_ScriptStringAnalyse( HDC, const void *, int, int, int, DWORD, int, SCRIPT_CONTROL*, SCRIPT_STATE*, const int*, SCRIPT_TABDEF*, const BYTE*, SCRIPT_STRING_ANALYSIS* ) { return E_NOTIMPL; }
        static HRESULT WINAPI Dummy_ScriptStringCPtoX( SCRIPT_STRING_ANALYSIS, int, BOOL, int* ) { return E_NOTIMPL; }
        static HRESULT WINAPI Dummy_ScriptStringXtoCP( SCRIPT_STRING_ANALYSIS, int, int*, int* ) { return E_NOTIMPL; }
        static HRESULT WINAPI Dummy_ScriptStringFree( SCRIPT_STRING_ANALYSIS* ) { return E_NOTIMPL; }
        static const SCRIPT_LOGATTR* WINAPI Dummy_ScriptString_pLogAttr( SCRIPT_STRING_ANALYSIS ) { return NULL; }
        static const int* WINAPI Dummy_ScriptString_pcOutChars( SCRIPT_STRING_ANALYSIS ) { return NULL; }

        // Function pointers
        static HRESULT (WINAPI *_ScriptApplyDigitSubstitution)( const SCRIPT_DIGITSUBSTITUTE*, SCRIPT_CONTROL*, SCRIPT_STATE* );
        static HRESULT (WINAPI *_ScriptStringAnalyse)( HDC, const void *, int, int, int, DWORD, int, SCRIPT_CONTROL*, SCRIPT_STATE*, const int*, SCRIPT_TABDEF*, const BYTE*, SCRIPT_STRING_ANALYSIS* );
        static HRESULT (WINAPI *_ScriptStringCPtoX)( SCRIPT_STRING_ANALYSIS, int, BOOL, int* );
        static HRESULT (WINAPI *_ScriptStringXtoCP)( SCRIPT_STRING_ANALYSIS, int, int*, int* );
        static HRESULT (WINAPI *_ScriptStringFree)( SCRIPT_STRING_ANALYSIS* );
        static const SCRIPT_LOGATTR* (WINAPI *_ScriptString_pLogAttr)( SCRIPT_STRING_ANALYSIS );
        static const int* (WINAPI *_ScriptString_pcOutChars)( SCRIPT_STRING_ANALYSIS );

        static void InitializeUniscribe();
        static void UninitializeUniscribe();

        static HINSTANCE s_hDll;  // Uniscribe DLL handle

    // Instance data
    private:
        WCHAR *m_pwszBuffer;    // Buffer to hold text
        int    m_nBufferSize;   // Size of the buffer allocated, in characters
        int    m_nTextSize;     // Size of the text in buffer excluding
                                //   NULL terminator, in characters
        // Uniscribe-specific
        int  m_iFont;           // Font index of this buffer
        bool m_bAnalyseRequired;// True if the string has changed since last analysis.
        SCRIPT_STRING_ANALYSIS m_Analysis;  // Analysis for the current string
		
		VOID*	m_pParentPtr ;
    private:
        bool Grow( int nNewSize = -1 );

        // Uniscribe -- Analyse() analyses the string in the buffer
        HRESULT Analyse();

    public:
        CUniBuffer( int nInitialSize = 1 );
        ~CUniBuffer();
        int GetBufferSize() { return m_nBufferSize; }
        bool SetBufferSize( int nSize );
        int GetTextSize() { return m_nTextSize; }
        const WCHAR *GetBuffer() { return m_pwszBuffer; }
        const WCHAR &operator[]( int n ) const { return m_pwszBuffer[n]; }
        WCHAR &operator[]( int n );
        int GetFontIndex() { return m_iFont; }
        void SetFontIndex( int iFont ) { m_iFont = iFont; }
        void Clear();

		void SetParentPtr(VOID* pPtr){m_pParentPtr = pPtr;};
        // Inserts the char at specified index.
        // If nIndex == -1, insert to the end.
        bool InsertChar( int nIndex, WCHAR wChar );

        // Removes the char at specified index.
        // If nIndex == -1, remove the last char.
        bool RemoveChar( int nIndex );

        // Inserts the first nCount characters of the string pStr at specified index.
        // If nCount == -1, the entire string is inserted.
        // If nIndex == -1, insert to the end.
        bool InsertString( int nIndex, const WCHAR *pStr, int nCount = -1 );

        bool SetText( LPCWSTR wszText );

        // Uniscribe
        HRESULT CPtoX( int nCP, BOOL bTrail, int *pX );
        HRESULT XtoCP( int nX, int *pCP, int *pnTrail );
        void GetPriorItemPos( int nCP, int *pPrior );
        void GetNextItemPos( int nCP, int *pPrior );

		void AnalyseRequired(){m_bAnalyseRequired = true;}
    };

protected:
	static bool s_bHideCaret;   // If true, we don't render the caret.
	CUniBuffer m_Buffer;     // Buffer to hold text
	std::string		m_stdStringPassBuffer;
};

#define MAX_CANDLIST 10
#define MAX_COMPSTRING_SIZE 256

//--------------------------------------------------------------------------------------
// A growable array
//--------------------------------------------------------------------------------------
template< typename TYPE >
class CGrowableArray
{
public:
    CGrowableArray()  { m_pData = NULL; m_nSize = 0; m_nMaxSize = 0; }
    CGrowableArray( const CGrowableArray<TYPE>& a ) { for( int i=0; i < a.m_nSize; i++ ) Add( a.m_pData[i] ); }
    ~CGrowableArray() { RemoveAll(); }

    const TYPE& operator[]( int nIndex ) const { return GetAt( nIndex ); }
    TYPE& operator[]( int nIndex ) { return GetAt( nIndex ); }
   
    CGrowableArray& operator=( const CGrowableArray<TYPE>& a ) { if( this == &a ) return *this; RemoveAll(); for( int i=0; i < a.m_nSize; i++ ) Add( a.m_pData[i] ); return *this; }

    HRESULT SetSize( int nNewMaxSize );
    HRESULT Add( const TYPE& value );
    HRESULT Insert( int nIndex, const TYPE& value );
    HRESULT SetAt( int nIndex, const TYPE& value );
    TYPE&   GetAt( int nIndex ) { WzAssert( nIndex >= 0 && nIndex < m_nSize ); return m_pData[nIndex]; }
    int     GetSize() const { return m_nSize; }
    TYPE*   GetData() { return m_pData; }
    bool    Contains( const TYPE& value ){ return ( -1 != IndexOf( value ) ); }

    int     IndexOf( const TYPE& value ) { return ( m_nSize > 0 ) ? IndexOf( value, 0, m_nSize ) : -1; }
    int     IndexOf( const TYPE& value, int iStart ) { return IndexOf( value, iStart, m_nSize - iStart ); }
    int     IndexOf( const TYPE& value, int nIndex, int nNumElements );

    int     LastIndexOf( const TYPE& value ) { return ( m_nSize > 0 ) ? LastIndexOf( value, m_nSize-1, m_nSize ) : -1; }
    int     LastIndexOf( const TYPE& value, int nIndex ) { return LastIndexOf( value, nIndex, nIndex+1 ); }
    int     LastIndexOf( const TYPE& value, int nIndex, int nNumElements );

    HRESULT Remove( int nIndex );
    void    RemoveAll() { SetSize(0); }

protected:
    TYPE* m_pData;      // the actual array of data
    int m_nSize;        // # of elements (upperBound - 1)
    int m_nMaxSize;     // max allocated

    HRESULT SetSizeInternal( int nNewMaxSize );  // This version doesn't call ctor or dtor.
};

//-----------------------------------------------------------------------------
// IME-enabled EditBox control
//-----------------------------------------------------------------------------
class CCtrlDxIMEEditBox : public CCtrlDxEditBox
{
    friend class CExternalApiInitializer;

    static void InitializeImm();
    static void UninitializeImm();

    static HINSTANCE s_hDllImm32;  // IMM32 DLL handle
    static HINSTANCE s_hDllVer;    // Version DLL handle
    static HIMC      s_hImcDef;    // Default input context	

public:
	virtual bool	OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);
	virtual void	OnDestroy();
	virtual void	OnDoAction();
	virtual bool	OnDraw(bool bl);
	virtual bool	MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
	virtual bool	SendMessageWZ(SI_MESSAGE* pMsg);
	virtual void	SetHeight(DWORD nHeight);

	//. �ε� / ���� ���� �Լ�.
	virtual void	OnLoad(HANDLE hHandle);
	virtual void	OnSave(HANDLE hHandle);

protected:
    // Empty implementation of the IMM32 API
    static INPUTCONTEXT* WINAPI Dummy_ImmLockIMC( HIMC ) { return NULL; }
    static BOOL WINAPI Dummy_ImmUnlockIMC( HIMC ) { return FALSE; }
    static LPVOID WINAPI Dummy_ImmLockIMCC( HIMCC ) { return NULL; }
    static BOOL WINAPI Dummy_ImmUnlockIMCC( HIMCC ) { return FALSE; }
    static BOOL WINAPI Dummy_ImmDisableTextFrameService( DWORD ) { return TRUE; }
    static LONG WINAPI Dummy_ImmGetCompositionStringW( HIMC, DWORD, LPVOID, DWORD ) { return IMM_ERROR_GENERAL; }
    static DWORD WINAPI Dummy_ImmGetCandidateListW( HIMC, DWORD, LPCANDIDATELIST, DWORD ) { return 0; }
    static HIMC WINAPI Dummy_ImmGetContext( HWND ) { return NULL; }
    static BOOL WINAPI Dummy_ImmReleaseContext( HWND, HIMC ) { return FALSE; }
    static HIMC WINAPI Dummy_ImmAssociateContext( HWND, HIMC ) { return NULL; }
    static BOOL WINAPI Dummy_ImmGetOpenStatus( HIMC ) { return 0; }
    static BOOL WINAPI Dummy_ImmSetOpenStatus( HIMC, BOOL ) { return 0; }
    static BOOL WINAPI Dummy_ImmGetConversionStatus( HIMC, LPDWORD, LPDWORD ) { return 0; }
    static HWND WINAPI Dummy_ImmGetDefaultIMEWnd( HWND ) { return NULL; }
    static UINT WINAPI Dummy_ImmGetIMEFileNameA( HKL, LPSTR, UINT ) { return 0; }
    static UINT WINAPI Dummy_ImmGetVirtualKey( HWND ) { return 0; }
    static BOOL WINAPI Dummy_ImmNotifyIME( HIMC, DWORD, DWORD, DWORD ) { return FALSE; }
    static BOOL WINAPI Dummy_ImmSetConversionStatus( HIMC, DWORD, DWORD ) { return FALSE; }
    static BOOL WINAPI Dummy_ImmSimulateHotKey( HWND, DWORD ) { return FALSE; }
    static BOOL WINAPI Dummy_ImmIsIME( HKL ) { return FALSE; }
	static DWORD WINAPI Dummy_ImmGetProperty( HKL, DWORD ) { return 0; }
	static BOOL WINAPI Dummy_ImmSetCompositionWindow(  HIMC, LPCOMPOSITIONFORM ) { return 0;}
    // Traditional Chinese IME
    static UINT WINAPI Dummy_GetReadingString( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT ) { return 0; }
    static BOOL WINAPI Dummy_ShowReadingWindow( HIMC, BOOL ) { return FALSE; }

    // Verion library imports
    static BOOL APIENTRY Dummy_VerQueryValueA( const LPVOID, LPSTR, LPVOID *, PUINT ) { return 0; }
    static BOOL APIENTRY Dummy_GetFileVersionInfoA( LPSTR, DWORD, DWORD, LPVOID ) { return 0; }
    static DWORD APIENTRY Dummy_GetFileVersionInfoSizeA( LPSTR, LPDWORD ) { return 0; }

    // Function pointers: IMM32
    static INPUTCONTEXT* (WINAPI * _ImmLockIMC)( HIMC );
    static BOOL (WINAPI * _ImmUnlockIMC)( HIMC );
    static LPVOID (WINAPI * _ImmLockIMCC)( HIMCC );
    static BOOL (WINAPI * _ImmUnlockIMCC)( HIMCC );
    static BOOL (WINAPI * _ImmDisableTextFrameService)( DWORD );
    static LONG (WINAPI * _ImmGetCompositionStringW)( HIMC, DWORD, LPVOID, DWORD );
    static DWORD (WINAPI * _ImmGetCandidateListW)( HIMC, DWORD, LPCANDIDATELIST, DWORD );
    static HIMC (WINAPI * _ImmGetContext)( HWND );
    static BOOL (WINAPI * _ImmReleaseContext)( HWND, HIMC );
    static HIMC (WINAPI * _ImmAssociateContext)( HWND, HIMC );
    static BOOL (WINAPI * _ImmGetOpenStatus)( HIMC );
    static BOOL (WINAPI * _ImmSetOpenStatus)( HIMC, BOOL );
    static BOOL (WINAPI * _ImmGetConversionStatus)( HIMC, LPDWORD, LPDWORD );
    static HWND (WINAPI * _ImmGetDefaultIMEWnd)( HWND );
    static UINT (WINAPI * _ImmGetIMEFileNameA)( HKL, LPSTR, UINT );
    static UINT (WINAPI * _ImmGetVirtualKey)( HWND );
    static BOOL (WINAPI * _ImmNotifyIME)( HIMC, DWORD, DWORD, DWORD );
    static BOOL (WINAPI * _ImmSetConversionStatus)( HIMC, DWORD, DWORD );
    static BOOL (WINAPI * _ImmSimulateHotKey)( HWND, DWORD );
    static BOOL (WINAPI * _ImmIsIME)( HKL );
	static DWORD (WINAPI * _ImmGetProperty )( HKL, DWORD );
	static BOOL (WINAPI *  _ImmSetCompositionWindow)( HIMC, LPCOMPOSITIONFORM);

    // Function pointers: Traditional Chinese IME
    static UINT (WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
    static BOOL (WINAPI * _ShowReadingWindow)( HIMC, BOOL );

    // Function pointers: Verion library imports
    static BOOL (APIENTRY * _VerQueryValueA)( const LPVOID, LPSTR, LPVOID *, PUINT );
    static BOOL (APIENTRY * _GetFileVersionInfoA)( LPSTR, DWORD, DWORD, LPVOID );
    static DWORD (APIENTRY * _GetFileVersionInfoSizeA)( LPSTR, LPDWORD );

public:
    CCtrlDxIMEEditBox();
    virtual ~CCtrlDxIMEEditBox();

    static  HRESULT		StaticOnCreateDevice(HWND hWnd);										//. Application�� ���� �ɶ� �ѹ��� ������.
//    static  bool StaticMsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

    static  void		EnableImeSystem( bool bEnable );										//. IME ������ ���� �ϴ� �Լ� (On/off)
	
	static HWND			s_hWnd;	

	virtual void		OnFocusIn();															//. ��Ŀ���� IN/Out���� �������ش�.
    virtual void		OnFocusOut();
	virtual bool		HaveFocus(){return m_blHasFocus;}										//. ��Ŀ�� ���� üũ
    void				PumpMessage();															//. ?
public:
	//. �� ���� �Լ���.
	virtual void		SetColorCompText(COLORREF color){m_CompColor = color;}
	virtual void		SetColorCompBack(COLORREF color){m_CompWinColor = color;}
	virtual void		SetColorCompCaret(COLORREF color){m_CompCaretColor = color;}
	
	virtual void		SetColorIndicatorIme(COLORREF color){m_IndicatorImeColor = color;}
	virtual void		SetColorIndicatorEng(COLORREF color){m_IndicatorEngColor = color;}
	virtual void		SetColorIndicatorBK(COLORREF color){m_IndicatorBkColor = color;}

	virtual COLORREF	GetCompTextColor(){return m_CompColor;}
	virtual COLORREF	GetCompBackColor(){return m_CompWinColor;}
	virtual COLORREF	GetCompCaretColor(){return m_CompCaretColor;}

	virtual COLORREF	GetIndicatorImeColor(){return m_IndicatorImeColor;}
	virtual COLORREF	GetIndicatorEngColor(){return m_IndicatorEngColor;}
	virtual COLORREF	GetIndicatorBKColor(){return m_IndicatorBkColor;}

	virtual void		SetUseIndicator(bool bl){m_blUseIndicator = bl;}						//. ��/�� ǥ�ñ� �׸����� �� �׸����� ���� �Լ�.
	virtual bool		GetUseIndicator(){return m_blUseIndicator;}

protected:
	virtual void		RenderCandidateReadingWindow( bool bReading );							//. ����Ʈ�� ����Ʈ ����
    virtual void		RenderComposition();													//. ���� â ����
    virtual void		RenderIndicator();														//. Ime ���� â ����

protected:
	
    static WORD			GetLanguage() { return LOWORD( s_hklCurrent ); }						//. ��� ����
    static WORD			GetPrimaryLanguage() { return PRIMARYLANGID( LOWORD( s_hklCurrent ) ); }//. �߱���, �ѱ���.....	
    static WORD			GetSubLanguage() { return SUBLANGID( LOWORD( s_hklCurrent ) ); }		//. �߱��� ������ ........
    static void			SendKey( BYTE nVirtKey );
    static DWORD		GetImeId( UINT uIndex = 0 );
    static void			CheckInputLocale();
    static void			CheckToggleState();
    static void			SetupImeApi();
    
    void				TruncateCompString( bool bUseBackSpace = true, int iNewStrLen = 0 );
    void				FinalizeString( bool bSend );
    static void			GetReadingWindowOrientation( DWORD dwId );
    static void			GetPrivateReadingString();

    void				SendCompString();															//. ����â 
	void				ResetCompositionString();													//. ����â String Table ����.

protected:
    enum { INDICATOR_NON_IME, INDICATOR_CHS, INDICATOR_CHT, INDICATOR_KOREAN, INDICATOR_JAPANESE }; // Indicator ����
    enum IMESTATE { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };							//. IME ����

    struct CCandList
    {
        WCHAR awszCandidate[MAX_CANDLIST][256];
        CUniBuffer HoriCand;			// Candidate list string (for horizontal candidate window)
        int   nFirstSelected;			// First character position of the selected string in HoriCand
        int   nHoriSelectedLen;			// Length of the selected string in HoriCand
        DWORD dwCount;					// Number of valid entries in the candidate list
        DWORD dwSelection;				// Currently selected candidate entry relative to page top
        DWORD dwPageSize;
        int   nReadingError;			// Index of the error character
        bool  bShowWindow;				// Whether the candidate list window is visible
        RECT  rcCandidate;				// Candidate rectangle computed and filled each time before rendered
    };

    struct CInputLocale
    {
        HKL   m_hKL;					// Keyboard layout
        WCHAR m_wszLangAbb[3];			// Language abbreviation
        WCHAR m_wszLang[64];			// Localized language name
    };

    // Application-wide data
    static HKL     s_hklCurrent;          // Current keyboard layout of the process
    static bool    s_bVerticalCand;       // Indicates that the candidates are listed vertically
    static LPWSTR  s_wszCurrIndicator;    // Points to an indicator string that corresponds to current input locale
    static WCHAR	s_aszIndicator[5][3]; // String to draw to indicate current input locale
    static bool    s_bInsertOnType;       // Insert the character as soon as a key is pressed (Korean behavior)
    static HINSTANCE s_hDllIme;           // Instance handle of the current IME module
    static IMESTATE  s_ImeState;          // IME global state
	static bool    s_bEnableImeSystem;    // Whether the IME system is active
    static POINT   s_ptCompString;        // Composition string position. Updated every frame.
    static int     s_nCompCaret;          // Caret position of the composition string
    static int     s_nFirstTargetConv;    // Index of the first target converted char in comp string.  If none, -1.
    static BYTE    s_abCompStringAttr[MAX_COMPSTRING_SIZE];
    static DWORD   s_adwCompStringClause[MAX_COMPSTRING_SIZE];
    static WCHAR   s_wszReadingString[32];// Used only with horizontal reading window (why?)
    static CCandList s_CandList;          // Data relevant to the candidate list
    static bool    s_bShowReadingWindow;  // Indicates whether reading window is visible
    static bool    s_bHorizontalReading;  // Indicates whether the reading window is vertical or horizontal
    static bool    s_bChineseIME;
    static CGrowableArray< CInputLocale > s_Locale; // Array of loaded keyboard layout on system
	static DWORD   s_dwProperty;	
	static bool	   s_bUseDefWindowCandidate;
    // Color of various IME elements
    COLORREF       m_ReadingColor;        // Reading string color
    COLORREF       m_ReadingWinColor;     // Reading window color
    COLORREF       m_ReadingSelColor;     // Selected character in reading string
    COLORREF       m_ReadingSelBkColor;   // Background color for selected char in reading str
    COLORREF       m_CandidateColor;      // Candidate string color
    COLORREF       m_CandidateWinColor;   // Candidate window color
    COLORREF       m_CandidateSelColor;   // Selected candidate string color
    COLORREF       m_CandidateSelBkColor; // Selected candidate background color
    COLORREF       m_CompColor;           // Composition string color
    COLORREF       m_CompWinColor;        // Composition string window color
    COLORREF       m_CompCaretColor;      // Composition string caret color
    COLORREF       m_CompTargetColor;     // Composition string target converted color
    COLORREF       m_CompTargetBkColor;   // Composition string target converted background
    COLORREF       m_CompTargetNonColor;  // Composition string target non-converted color
    COLORREF       m_CompTargetNonBkColor;// Composition string target non-converted background
    COLORREF       m_IndicatorImeColor;   // Indicator text color for IME
    COLORREF       m_IndicatorEngColor;   // Indicator text color for English
    COLORREF       m_IndicatorBkColor;    // Indicator text background color

    // Edit-control-specific data
    int            m_nIndicatorWidth;     // Width of the indicator symbol
    RECT           m_rcIndicator;         // Rectangle for drawing the indicator button
	bool		   m_blUseIndicator;
	
	CUniBuffer		s_CompString;			// Buffer to hold the composition string (we fix its length)
	
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlMultilineEditWZ Class
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class CCtrlMultilineEditWZ : public CCtrlDxIMEEditBox
{
public:
	CCtrlMultilineEditWZ();
	virtual ~CCtrlMultilineEditWZ();

	virtual void			SetHeight(DWORD nHeight);
	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);		
	virtual void			OnDestroy();	
	virtual bool			OnDraw(bool bl);	
	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
	virtual bool			SendMessageWZ(SI_MESSAGE* pMsg);

	//. �ε� / ���� ���� �Լ�.
	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);
	
	virtual void			UpdateWindowWZ(HWND hParent);			//. �̹��� ���� ������ �Ҷ� ȣ��.
	virtual void			ReleaseImage();	

public:
	//. ��Ʈ ���� �Լ�.
	virtual void			SetFontID(DWORD dwFontID);				//. Font ID �����Ѵ�.		
	
	//. �ؽ�Ʈ ���� �Լ���.	
	virtual void			SetTextWZA( LPCSTR pszText,bool bSelected=false);
	virtual void			SetTextWZW( LPCWSTR pwszText,bool bSelected=false);
		
	virtual void			ClearText();

	//. ��Ŀ�� ���� �Լ���.
	virtual void			SetFocusWZ();

	//. VScroll ��Ʈ�� ���� ���� �Լ���.

	virtual void			SetVScrollBaseImageFormat(SI_TFORMAT* pFormat);		
	virtual void			SetVScrollArrowUPBTImageFormat(SI_TFORMAT* pFormat);		
	virtual void			SetVScrollArrowDownBTImageFormat(SI_TFORMAT* pFormat);
	virtual void			SetVScrollArrowThumbBTImageFormat(SI_TFORMAT* pFormat);

	virtual SI_TFORMAT*		GetVScrollBaseImageFormat();		
	virtual SI_TFORMAT*		GetVScrollArrowUPBTImageFormat();		
	virtual SI_TFORMAT*		GetVScrollArrowDownBTImageFormat();
	virtual SI_TFORMAT*		GetVScrollArrowThumbBTImageFormat();	

	virtual void			SetVScrollBaseIsOutline(bool blIs);
	virtual bool			GetVScrollBaseIsOutline();
	virtual void			SetVScrollBaseOutlineColor(COLORREF rgb);
	virtual COLORREF		GetVScrollBaseOutlineColor();

	virtual void			SetVScrollArrowUPBTIsOutline(bool blIs);
	virtual bool			GetVScrollArrowUPBTIsOutline();
	virtual void			SetVScrollArrowUPBTOutlineColor(COLORREF rgb);
	virtual COLORREF		GetVScrollArrowUPBTOutlineColor();

	virtual void			SetVScrollArrowDownBTIsOutline(bool blIs);
	virtual bool			GetVScrollArrowDownBTIsOutline();
	virtual void			SetVScrollArrowDownBTOutlineColor(COLORREF rgb);
	virtual COLORREF		GetVScrollArrowDownBTOutlineColor();

	virtual void			SetVScrollArrowThumbBTIsOutline(bool blIs);
	virtual bool			GetVScrollArrowThumbBTIsOutline();
	virtual void			SetVScrollArrowThumbBTOutlineColor(COLORREF rgb);
	virtual COLORREF		GetVScrollArrowThumbBTOutlineColor();	
    virtual	CCtrlVScrollWZ* GetVScroll() { return &m_cVScroll; }
	bool					IsUseInputEnterKey() const { return m_bUseInputEnterKey; }
	void					SetUseInputEnterKey(bool val) { m_bUseInputEnterKey = val; }

protected:	
	virtual BOOL			IsLimitLine();
	virtual BOOL			CheckLimitLine();
	virtual void			RenderComposition();
	virtual void			RenderCandidateReadingWindow(bool bReading);
	virtual	void			PasteFromClipboard();
	void					ResetLineInfo(int nRow);
	
protected:
	std::vector<DWORD>		m_stdVecterLineInfo;			//. �� ��ȣ ����
	
	CCtrlVScrollWZ			m_cVScroll;						//. VScroll�� ������ �ִ�.
	int						m_nHeight;						//. ������ ����
	int						m_nCurrRow;						//. ���� �� ��ȣ
	int						m_nSelRow;						//. ������ �� ��ȣ
	int						m_nDrawLineCnt;					//. ũ��ȿ� �׸��� �ִ� ���� ��.
	bool					m_bUseInputEnterKey;					//!< ����Ű ��뿩��
};
