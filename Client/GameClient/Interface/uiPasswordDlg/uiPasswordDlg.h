#pragma once
//------------------------------------------------------------------------------
//
//    �۾���: ���¿�
//    uiPasswordDlg class
//------------------------------------------------------------------------------

#include "uibase.h"
#include "uiPasswordMan/uiPassWordMan.h"


class uiPasswordMan;

typedef std::vector<int>		RANDOM_VEC;

class Randomize
{
public:
	Randomize() { m_iRandMax = 0; m_iGetCount = 0; }
	~Randomize() { Destroy(); }

	void Create( int iRandMax )
	{
		m_iRandMax = iRandMax;
		for( int i = 0; i < iRandMax; ++i )
		{
			m_vecRandom.push_back( i );
		}
		RandomShuffule();
	}

	void Destroy()
	{
		m_vecRandom.clear();
	}

	inline int Rand()
	{
		int retVal = m_vecRandom[m_iGetCount];
		if( ++m_iGetCount >= m_iRandMax ) {	m_iGetCount = 0; RandomShuffule(); }
		return retVal;
	}

	inline int Rand( int min, int max )
	{
		return ( Rand() % ( max + 1 - min ) ) + min;
	}

	inline void RandomShuffule()
	{
		random_shuffle( m_vecRandom.begin(), m_vecRandom.end() );
	}
	RANDOM_VEC		m_vecRandom;
private:

	int				m_iRandMax;
	int				m_iGetCount;
};

class uiPasswordDlg: public uiBase
{
	enum eDIALOG_PASSWORD_DLG
	{
		eDIALOG_PASSWORD_DLG_BTN_EXIT = 0,			//	X ��ư

		eDIALOG_PASSWORD_DLG_TXT_MESSAGE,			//	�޽��� �ؽ�Ʈ
		eDIALOG_PASSWORD_DLG_TXT,					//	�н����� �ؽ�Ʈ

		eDIALOG_PASSWORD_DLG_BTN_1,					//	1�� ��ư
		eDIALOG_PASSWORD_DLG_BTN_2,					//	2�� ��ư
		eDIALOG_PASSWORD_DLG_BTN_3,					//	3�� ��ư
		eDIALOG_PASSWORD_DLG_BTN_4,					//	4�� ��ư
		eDIALOG_PASSWORD_DLG_BTN_5,					//	5�� ��ư
		eDIALOG_PASSWORD_DLG_BTN_6,					//	6�� ��ư
		eDIALOG_PASSWORD_DLG_BTN_7,					//	7�� ��ư
		eDIALOG_PASSWORD_DLG_BTN_8,					//	8�� ��ư
		eDIALOG_PASSWORD_DLG_BTN_9,					//	9�� ��ư
		eDIALOG_PASSWORD_DLG_BTN_0,					//	0�� ��ư	// ��Ʈ�����̺����� 0�� �� �ڷΰ��� �ϴ�... ����.. �ٸ����� ��ġ �Ἥ ���ٲ۴ٳ�;;
#ifdef _GS_ADD_PWD_X_BTN_1143
		eDIALOG_PASSWORD_DLG_BTN_X,					//	X�� ��ư
#endif//_GS_ADD_PWD_X_BTN_1143:�߱�X��ư

#ifdef _GS_ADD_PWD_X_BTN_1245
		eDIALOG_PASSWORD_DLG_BTN_X2,				//	X�� ��ư(�ҹ���)
#endif//_GS_ADD_PWD_X_BTN_1245
        
		eDIALOG_PASSWORD_DLG_BTN_CHGPASSWORD,		//	�н����� �ʱ�ȭ

		eDIALOG_PASSWORD_DLG_BTN_BACKSPACE,			//	�齺���̽� ��ư
		eDIALOG_PASSWORD_DLG_BTN_CLEAR,				//	Ŭ���� ��ư

		eDIALOG_PASSWORD_DLG_BTN_OK,				//	Ȯ�� ��ư
		eDIALOG_PASSWORD_DLG_BTN_CANCLE,			//	��� ��ư

		eDIALOG_PASSWORD_DLG_MAX,
	};

	enum ePASSWORD_NUM_DLG_NUM
	{
		ePASSWORD_NUM_DLG_NUM_1 = 0,
		ePASSWORD_NUM_DLG_NUM_2,
		ePASSWORD_NUM_DLG_NUM_3,
		ePASSWORD_NUM_DLG_NUM_4,
		ePASSWORD_NUM_DLG_NUM_5,
		ePASSWORD_NUM_DLG_NUM_6,
		ePASSWORD_NUM_DLG_NUM_7,
		ePASSWORD_NUM_DLG_NUM_8,
		ePASSWORD_NUM_DLG_NUM_9,
		ePASSWORD_NUM_DLG_NUM_0,

#ifdef _GS_ADD_PWD_X_BTN_1143
		ePASSWORD_NUM_DLG_NUM_X,
#endif//_GS_ADD_PWD_X_BTN_1143:�߱�X��ư

#ifdef _GS_ADD_PWD_X_BTN_1245
		ePASSWORD_NUM_DLG_NUM_X2,	//	X�� ��ư(�ҹ���)
#endif//_GS_ADD_PWD_X_BTN_1245

		ePASSWORD_NUM_DLG_NUM_MAX,  
	};

public:
	uiPasswordDlg(InterfaceManager *pUIMan);
	virtual ~uiPasswordDlg();

	//--------------------------------------------------------------------------
	// init/release
	//
public:
	virtual void            Init(CDrawBase* pDrawBase);
	virtual void            Release();

protected:
	void                    InitControls();
	//--------------------------------------------------------------------------
	// helper/comms.
	//
public:
	uiPasswordMan*			GetManager();

	//--------------------------------------------------------------------------
	// functions
	//
private:
	BOOL					_isEnable();

	void					_clearAllItem();
	void					_setItem(int index, CODETYPE itemCode);
	void					_SetPasswordKey();
	void					_SetChgPassBtn( BOOL bShowChgPassBtn );

	//--------------------------------------------------------------------------
	// dialog functions
	//
public:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();

protected:
	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );
	void					updateToolTip( void );
	int						getTooltipString(POSTYPE pos);
	int						m_iTooltipStringCode;
	map<int,int>			m_UseTooltipCtrls;

	static WzID		        m_wzId[eDIALOG_PASSWORD_DLG_MAX];

	//--------------------------------------------------------------------------
	// procedure
	//
public:
	virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

	void					SetPassword(TCHAR* szMessage, int iMinCount, int iMaxCount, BOOL bShowPassChgBtn, int iCalled);
	TCHAR*					GetStrPassword()		{return m_strPassword;}
	void					_SetPsaawordMinMaxCount(int iMinCount, int iMaxCount)
	{m_iPasswordMinCount = iMinCount; m_iPasswordMaxCount = iMaxCount;}
	void					_SetPasswordMessage( TCHAR* szMessage );

	//--------------------------------------------------------------------------
	// member vars
	//
private:
	uiPasswordMan*			m_pPasswordMan;

	CCtrlButtonWZ*			m_pBtnPasswordNum[ePASSWORD_NUM_DLG_NUM_MAX];

	CCtrlStaticWZ*			m_pTxtCtrlMessage;

	CCtrlStaticWZ*			m_pTxtCtrlPassword;

	CCtrlButtonWZ*			m_pBtnPasswordChg;

	bool					m_bShow;
	bool                    m_bControlsInitialized;

	Randomize				m_randomizer;

	TCHAR					m_strPassword[INTERFACE_STRING_LENGTH];

	int						m_iPasswordMinCount;
	int						m_iPasswordMaxCount;
	int						m_iCalledState;

public:
	fn_Password				m_fnPassword;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiPasswordDlg::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_PASSWORD_DLG_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_PASSWORD_DLG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiPasswordDlg::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_PASSWORD_DLG_MAX);

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiPasswordMan*
uiPasswordDlg::GetManager()
{
	return this->m_pPasswordMan;
}

//------------------------------------------------------------------------------

