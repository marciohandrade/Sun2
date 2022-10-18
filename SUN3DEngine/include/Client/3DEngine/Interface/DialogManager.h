#pragma once


//! [2010-3-11 by i4u4me] 
//! 수정된 파일 포멧 : control(static, button) 에 글줄임 표시 넣기
//!                   control(static, button) 에 money 타입 추가
//! 관련 변수 : m_bTextReducing
//#define UI_VERSION "ver.r2.0"	// 현제 UI버젼
//#define UI_VERSION "ver.r2.1"	// 타일링 이미지 추가버전
//#define UI_VERSION "ver.r2.2"	// 사운드 추가버전
#define UI_VERSION "ver.r2.3"   // 메세지 무시 추가버전

#ifdef UNICODE
#define FindStringTable FindStringTableW
#else
#define FindStringTable FindStringTableA
#endif

#ifdef UNICODE
#define AddStringTable AddStringTableW
#else
#define AddStringTable AddStringTableA
#endif


#ifdef UNICODE
#define GetStringTableIndexValue GetStringTableIndexValueW
#else
#define GetStringTableIndexValue GetStringTableIndexValueA
#endif

class CStringManager;
class CBaseManager;
class CDrawCtrl;
class CDialogWZ;
class CControlWZ;
class CDialogManager  
{
public:
	CDialogManager();
	virtual ~CDialogManager();

	//. Create
	virtual bool				OnCreate(char* pszPath, HWND hParentWnd, HINSTANCE hInstance, void* pDraw,bool blDxUse = true);	
	
	//. Destroy
	virtual void				OnDestroy();

	//. 정보 해체. (메인 포인트들은 해체 안함)
	virtual void				OnRelease();

	//. Create Dialog 함수들
	virtual CDialogWZ*			CreateDialogWZ(DWORD dwDlgID, DWORD dwStyle);
	virtual CControlWZ*			CreateControlWZ(DWORD dwDlgID,DWORD dwCtrlID,DWORD dwCtrlKind, DWORD dwCtrlStyle,RECT* rtSize);	

	//. Remove Dialog 함수들
	virtual bool				RemoveDialogWZ(DWORD dwDlgID);
	virtual bool				RemoveControlWZ(DWORD dwDlgID,DWORD dwCtrlID);
	
	//. 기본 Process 
	virtual void				OnDoAction();
	virtual void				OnDraw();
	virtual void				OnDrawDialog(DWORD dwDlgID);

	//. 이미지 포멧 과련 함수들
	virtual void				SetDialogImageFormat(DWORD dwDlgID,SI_TFORMAT* pFMT);
	virtual void				SetControlImageFormat(DWORD dwDlgID, DWORD dwCtrlID,SI_TFORMAT* pFMT);

	
	virtual bool				GetDialogWZSizeRect(DWORD dwDlgID, RECT* prtRect);						//. 아디에 해당하는 Dlg Rect을 얻어온다.
	virtual bool				GetControlSizeRect(DWORD dwDlgID, DWORD dwCtrlID, RECT* prtRect);		//. 아디에 해당하는 Control Rect을 얻어온다.


	virtual DWORD				GetDialogIDToIndex(int nIndex);											//. 인덱스 순서의 Dlg ID를 얻어온다.
	virtual void				MoveDialogWZ(DWORD dwDlgID, DWORD dwCtrlID, RECT rtRect);				//. Dlg ID 위치 교체

	virtual void				GetClientRectWZ(DWORD dwDlgID, RECT* prtClientRect);					//. Client 영역이 Dlg 영역이라 보면 된다.

	//. 외부 프로세스와 윈도우 메세지로 연결 해준다. 중요함수
	virtual bool				WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, SI_MESSAGE* pResultMsg);

	virtual DWORD				GetDlgCnt();															//. Dlg 개수를 얻어온다.
	virtual CDialogWZ*			GetDialogWZ(DWORD dwDlgID);												//. Dialog 포인터를 얻어온다.
	virtual void				ChangeDrawClassPtr(void* pDraw,HWND hParent);							//. 랜더러의 포인트가 변경 될 경우 설정해준다.

	virtual void				EnableImeSystem(bool blEnable);											//. IME Edit의 On/Off

	//. 저장/로드 관련 함수.

	//. 저장 된 아디가 Manager안에 있을경우 랜덤 아디 생성해 Dialog를 만든다음 새로운 아디 리턴.
	virtual DWORD				OnLoad(HANDLE hHandle, DWORD* pdwDlgID = NULL, SYSTEMTIME* pSystemTime = NULL);

	//. DialogWZ 정보를 저장한다.
	virtual void				OnSave(HANDLE hHandle, SYSTEMTIME* pSystemTime = NULL);

	//. String 메니져 접근 함수.
	//. nLength 의 경우는 외부에서 설정한 버터 크기를 넣어준다. 내부에서 할당 크기를 알 필요 있음
	virtual	int					GetCountStringTable();													//. StringTable 개수를 얻어온다.
	virtual	bool				RemoveStringTable(DWORD dwStrID);										//. 주어진 아디의 해당 String 제거.

	virtual	bool				FindStringTableA(DWORD dwStrID,char* pszText = NULL, int nLength = 256);
	virtual	bool				FindStringTableW(DWORD dwStrID,WCHAR* pwszText = NULL, int nLength = 256);
	
	virtual	bool				AddStringTableA(DWORD dwStrID, char* pszText);
	virtual	bool				AddStringTableW(DWORD dwStrID, WCHAR* pwszText);
	
	virtual bool				GetStringTableIndexValueA(int iIndex, DWORD* pdwStrID, char* pszText  = NULL, int nLength = 256);	
	virtual bool				GetStringTableIndexValueW(int iIndex, DWORD* pdwStrID, WCHAR* pwszText  = NULL, int nLength = 256 );
	
	virtual bool				SaveStringTable(char* pszPath);											//. StringTable 리스트 Txt 파일로 저장
	virtual bool				LoadStringTable(char* pszPath,ENUM_LOAD_UITYPE eLoadType = ELUIT_ADD);  //. StringTable 읽어 오기
	

	//. 폰트 관련 함수들.	
	virtual void				SetDefaultFontID(DWORD dwFontID);										//. 디폴트 폰트 아디 설정한다.
	virtual DWORD				GetDefaultFontID();														//. 디폴트 폰트 아디를 얻어온다.

	//. Default 폰트
	virtual int					GetNumberOfDefaultFont();												//. 디폴트폰트 개수 얻기
	virtual BOOL				GetDefaultFontInfo(int iIndex,DWORD* pdwFontID,LOGFONTA* pLogFont);		//. 디폴트폰트 정보 얻기

	//. 확장 폰트.
	virtual BOOL				AddExtendedFont(DWORD dwFontID, LOGFONTA* pLogFont);					//. 확장폰트 추가
	virtual void				DeleteExtendedFont( DWORD dwFontID);									//. 확장폰트 삭제
	virtual void				DeleteAllExtendedFont();											//. 확장폰트 모두 삭제
	virtual int					GetNumberOfExtendedFont();											//. 확장폰트 개수 얻기
	virtual BOOL				GetExtendedFontInfo( int iIndex,DWORD* pdwFontID, LOGFONTA* pLogFont);	//. 확장폰트 정보 얻기
	virtual BOOL				LoadExtendedFontInfo(char* pszPath);									//. wft 파일 저장 해준다.				
	virtual BOOL				SaveExtendedFontInfo(char* pszPath);									//. wft 파일 읽고 등록 해준다.
	
	//. Disc Access
	virtual void				SetDiscAccess(VOID* pPtr);												//. File Stream 관리자 설정.

	//. 기타 함수들.
	virtual void				RequestFocus(CDialogWZ* pDlg);											//. 포커스를 체크 한다.:	
	bool						IsUseDx(){return m_blUseDxEdit;}										//. DX의 사용 여부를 검사한다.
	virtual void				FirstDialogID(DWORD dwDigID);											//. 해당 아디를 최상위에 적용한다.- 아이디를 리스트에 추가한다.
	virtual void				DeAttachFirstDialogID(DWORD dwDigID);									//. 해당 아디를 최상위에서 떼어낸다.
	virtual void				ApplyAttachDialog();													//. 최상위 윈도우 리스트의 다이얼로그를 최상위로 적용시킨다.

	virtual void				LockMessageDialogID(DWORD dwDlgID);										//. Model 기능 같이 Message을 독점한다.(해체:NULL);
																										//. DialogID가 Lock 되어 있으면 다른 DialogID는 무시. 반드시 사용 후 해체	
	virtual void				SetResourceFolderPath(char* pszPath);									//. 리소스 폴더 경로를 설정한다.
	virtual int					GetActiveEditCount();													//. 현재 활성화된 Edit 개수를 준다.

	virtual void				SetExePath( const char * szPath );										//. 패스를 다시 설정

	virtual CStringManager*	    GetStringManager() { return m_pcStringManager; }
    virtual CBaseManager*       GetDlgManager() { return m_pcDlgManager; }
    virtual DWORD               GetLockDlgID();

	virtual DWORD				PickDialog( int x, int y );
    virtual void                MoveTail( int Dialokey );
    virtual void                MoveHead( int Dialokey );

public:
	CDrawCtrl*					m_pcDraw;																//. Draw 엔진 포인터를 가지고 Draw 관련 처리함수들이 있는 클래스.....
	SI_MESSAGE					m_sReturnMsg;															//. Message 정보 관련 포인터.
	static	DWORD				m_sLockDlgID;															//. Dialog 상태를 잠군다.

private:	
	CBaseManager*				m_pcDlgManager;															//. WzID 관리 메니저.(아디, 클래스 포인터)
	CStringManager*				m_pcStringManager;														//. Text 관리 메니져.
	char						m_szPath[MAX_PATH];														//. 겜 경로를 가지고 있다.
	HWND						m_hParentWnd;															//. 부모 핸들
	HINSTANCE					m_hInstance;															//. 인스턴스...
	CDialogWZ*					m_pDialogFocus;															//. 포커스 가지고 있는 Dialog 포인터.
	bool						m_blUseDxEdit;															//. Dx Edit 사용/비사용.

	STD_LIST					m_stdFisrtDrawList;														// 최상위 윈도우 리스트
};

