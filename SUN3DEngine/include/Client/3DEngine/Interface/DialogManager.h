#pragma once


//! [2010-3-11 by i4u4me] 
//! ������ ���� ���� : control(static, button) �� ������ ǥ�� �ֱ�
//!                   control(static, button) �� money Ÿ�� �߰�
//! ���� ���� : m_bTextReducing
//#define UI_VERSION "ver.r2.0"	// ���� UI����
//#define UI_VERSION "ver.r2.1"	// Ÿ�ϸ� �̹��� �߰�����
//#define UI_VERSION "ver.r2.2"	// ���� �߰�����
#define UI_VERSION "ver.r2.3"   // �޼��� ���� �߰�����

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

	//. ���� ��ü. (���� ����Ʈ���� ��ü ����)
	virtual void				OnRelease();

	//. Create Dialog �Լ���
	virtual CDialogWZ*			CreateDialogWZ(DWORD dwDlgID, DWORD dwStyle);
	virtual CControlWZ*			CreateControlWZ(DWORD dwDlgID,DWORD dwCtrlID,DWORD dwCtrlKind, DWORD dwCtrlStyle,RECT* rtSize);	

	//. Remove Dialog �Լ���
	virtual bool				RemoveDialogWZ(DWORD dwDlgID);
	virtual bool				RemoveControlWZ(DWORD dwDlgID,DWORD dwCtrlID);
	
	//. �⺻ Process 
	virtual void				OnDoAction();
	virtual void				OnDraw();
	virtual void				OnDrawDialog(DWORD dwDlgID);

	//. �̹��� ���� ���� �Լ���
	virtual void				SetDialogImageFormat(DWORD dwDlgID,SI_TFORMAT* pFMT);
	virtual void				SetControlImageFormat(DWORD dwDlgID, DWORD dwCtrlID,SI_TFORMAT* pFMT);

	
	virtual bool				GetDialogWZSizeRect(DWORD dwDlgID, RECT* prtRect);						//. �Ƶ� �ش��ϴ� Dlg Rect�� ���´�.
	virtual bool				GetControlSizeRect(DWORD dwDlgID, DWORD dwCtrlID, RECT* prtRect);		//. �Ƶ� �ش��ϴ� Control Rect�� ���´�.


	virtual DWORD				GetDialogIDToIndex(int nIndex);											//. �ε��� ������ Dlg ID�� ���´�.
	virtual void				MoveDialogWZ(DWORD dwDlgID, DWORD dwCtrlID, RECT rtRect);				//. Dlg ID ��ġ ��ü

	virtual void				GetClientRectWZ(DWORD dwDlgID, RECT* prtClientRect);					//. Client ������ Dlg �����̶� ���� �ȴ�.

	//. �ܺ� ���μ����� ������ �޼����� ���� ���ش�. �߿��Լ�
	virtual bool				WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam, SI_MESSAGE* pResultMsg);

	virtual DWORD				GetDlgCnt();															//. Dlg ������ ���´�.
	virtual CDialogWZ*			GetDialogWZ(DWORD dwDlgID);												//. Dialog �����͸� ���´�.
	virtual void				ChangeDrawClassPtr(void* pDraw,HWND hParent);							//. �������� ����Ʈ�� ���� �� ��� �������ش�.

	virtual void				EnableImeSystem(bool blEnable);											//. IME Edit�� On/Off

	//. ����/�ε� ���� �Լ�.

	//. ���� �� �Ƶ� Manager�ȿ� ������� ���� �Ƶ� ������ Dialog�� ������� ���ο� �Ƶ� ����.
	virtual DWORD				OnLoad(HANDLE hHandle, DWORD* pdwDlgID = NULL, SYSTEMTIME* pSystemTime = NULL);

	//. DialogWZ ������ �����Ѵ�.
	virtual void				OnSave(HANDLE hHandle, SYSTEMTIME* pSystemTime = NULL);

	//. String �޴��� ���� �Լ�.
	//. nLength �� ���� �ܺο��� ������ ���� ũ�⸦ �־��ش�. ���ο��� �Ҵ� ũ�⸦ �� �ʿ� ����
	virtual	int					GetCountStringTable();													//. StringTable ������ ���´�.
	virtual	bool				RemoveStringTable(DWORD dwStrID);										//. �־��� �Ƶ��� �ش� String ����.

	virtual	bool				FindStringTableA(DWORD dwStrID,char* pszText = NULL, int nLength = 256);
	virtual	bool				FindStringTableW(DWORD dwStrID,WCHAR* pwszText = NULL, int nLength = 256);
	
	virtual	bool				AddStringTableA(DWORD dwStrID, char* pszText);
	virtual	bool				AddStringTableW(DWORD dwStrID, WCHAR* pwszText);
	
	virtual bool				GetStringTableIndexValueA(int iIndex, DWORD* pdwStrID, char* pszText  = NULL, int nLength = 256);	
	virtual bool				GetStringTableIndexValueW(int iIndex, DWORD* pdwStrID, WCHAR* pwszText  = NULL, int nLength = 256 );
	
	virtual bool				SaveStringTable(char* pszPath);											//. StringTable ����Ʈ Txt ���Ϸ� ����
	virtual bool				LoadStringTable(char* pszPath,ENUM_LOAD_UITYPE eLoadType = ELUIT_ADD);  //. StringTable �о� ����
	

	//. ��Ʈ ���� �Լ���.	
	virtual void				SetDefaultFontID(DWORD dwFontID);										//. ����Ʈ ��Ʈ �Ƶ� �����Ѵ�.
	virtual DWORD				GetDefaultFontID();														//. ����Ʈ ��Ʈ �Ƶ� ���´�.

	//. Default ��Ʈ
	virtual int					GetNumberOfDefaultFont();												//. ����Ʈ��Ʈ ���� ���
	virtual BOOL				GetDefaultFontInfo(int iIndex,DWORD* pdwFontID,LOGFONTA* pLogFont);		//. ����Ʈ��Ʈ ���� ���

	//. Ȯ�� ��Ʈ.
	virtual BOOL				AddExtendedFont(DWORD dwFontID, LOGFONTA* pLogFont);					//. Ȯ����Ʈ �߰�
	virtual void				DeleteExtendedFont( DWORD dwFontID);									//. Ȯ����Ʈ ����
	virtual void				DeleteAllExtendedFont();											//. Ȯ����Ʈ ��� ����
	virtual int					GetNumberOfExtendedFont();											//. Ȯ����Ʈ ���� ���
	virtual BOOL				GetExtendedFontInfo( int iIndex,DWORD* pdwFontID, LOGFONTA* pLogFont);	//. Ȯ����Ʈ ���� ���
	virtual BOOL				LoadExtendedFontInfo(char* pszPath);									//. wft ���� ���� ���ش�.				
	virtual BOOL				SaveExtendedFontInfo(char* pszPath);									//. wft ���� �а� ��� ���ش�.
	
	//. Disc Access
	virtual void				SetDiscAccess(VOID* pPtr);												//. File Stream ������ ����.

	//. ��Ÿ �Լ���.
	virtual void				RequestFocus(CDialogWZ* pDlg);											//. ��Ŀ���� üũ �Ѵ�.:	
	bool						IsUseDx(){return m_blUseDxEdit;}										//. DX�� ��� ���θ� �˻��Ѵ�.
	virtual void				FirstDialogID(DWORD dwDigID);											//. �ش� �Ƶ� �ֻ����� �����Ѵ�.- ���̵� ����Ʈ�� �߰��Ѵ�.
	virtual void				DeAttachFirstDialogID(DWORD dwDigID);									//. �ش� �Ƶ� �ֻ������� �����.
	virtual void				ApplyAttachDialog();													//. �ֻ��� ������ ����Ʈ�� ���̾�α׸� �ֻ����� �����Ų��.

	virtual void				LockMessageDialogID(DWORD dwDlgID);										//. Model ��� ���� Message�� �����Ѵ�.(��ü:NULL);
																										//. DialogID�� Lock �Ǿ� ������ �ٸ� DialogID�� ����. �ݵ�� ��� �� ��ü	
	virtual void				SetResourceFolderPath(char* pszPath);									//. ���ҽ� ���� ��θ� �����Ѵ�.
	virtual int					GetActiveEditCount();													//. ���� Ȱ��ȭ�� Edit ������ �ش�.

	virtual void				SetExePath( const char * szPath );										//. �н��� �ٽ� ����

	virtual CStringManager*	    GetStringManager() { return m_pcStringManager; }
    virtual CBaseManager*       GetDlgManager() { return m_pcDlgManager; }
    virtual DWORD               GetLockDlgID();

	virtual DWORD				PickDialog( int x, int y );
    virtual void                MoveTail( int Dialokey );
    virtual void                MoveHead( int Dialokey );

public:
	CDrawCtrl*					m_pcDraw;																//. Draw ���� �����͸� ������ Draw ���� ó���Լ����� �ִ� Ŭ����.....
	SI_MESSAGE					m_sReturnMsg;															//. Message ���� ���� ������.
	static	DWORD				m_sLockDlgID;															//. Dialog ���¸� �ᱺ��.

private:	
	CBaseManager*				m_pcDlgManager;															//. WzID ���� �޴���.(�Ƶ�, Ŭ���� ������)
	CStringManager*				m_pcStringManager;														//. Text ���� �޴���.
	char						m_szPath[MAX_PATH];														//. �� ��θ� ������ �ִ�.
	HWND						m_hParentWnd;															//. �θ� �ڵ�
	HINSTANCE					m_hInstance;															//. �ν��Ͻ�...
	CDialogWZ*					m_pDialogFocus;															//. ��Ŀ�� ������ �ִ� Dialog ������.
	bool						m_blUseDxEdit;															//. Dx Edit ���/����.

	STD_LIST					m_stdFisrtDrawList;														// �ֻ��� ������ ����Ʈ
};

