#pragma once

#include <hash_map>

enum eSTRINGTABLE_TOKEN_TYPE
{
	STR_TOKEN_ANYTYPE			= -1,
	STR_TOKEN_ERROR				= 0,
	STR_TOKEN_END,
	STR_TOKEN_STRING,

	STR_TOKEN_SLASH				= '/',
	STR_TOKEN_ASTERISK			= '*',

	// white-space
	STR_TOKEN_SPACE				= ' ',
	STR_TOKEN_NEWLINE			= '\n',
	STR_TOKEN_CARRIAGERETURN	= '\r', 
	STR_TOKEN_TAB				= '\t',
};

typedef stdext::hash_map<DWORD, std::string>	STRING_LIST_TYPE;
typedef	STRING_LIST_TYPE::iterator				STRING_LIST_ITR_TYPE;

class CDrawCtrl;
class CStringManager
{
public:
	CStringManager(void);
	~CStringManager(void);

	void	OnCreate();
	void	OnDestory();
	void	OnRelease();
	bool	OnLoad(LPCSTR pszFileName, ENUM_LOAD_UITYPE eLoadType = ELUIT_ADD);
	bool    OnSave(LPCSTR pszFileName);

	int		GetCount(){return m_stdHmStringMG.size();}
	bool	FindString(DWORD dwStrID, char* pszText );
	const	STD_STRING& FindString( DWORD dwStrID );

	bool	AddString(DWORD dwStrID, char* pszText);
	bool	RemoveString(DWORD dwStrID);

	bool	GetIndexValue(int iIndex, DWORD* pdwStrID, char* pszText = NULL);
	void	SetDrawPtr(CDrawCtrl*  pPtr){m_pDraw = pPtr;};

	void	CloseBuffer();
	char *	GetTokenString() { return m_pszTokenString;	}

private:
	void	_clearToken();
	char	_getchar( DWORD * pPos = NULL );
	char	_getcurchar( DWORD * pPos = NULL );
	char	_getcurchar( DWORD pos );	
	void	_rewind( int i , DWORD * pPos = NULL );
	int		_isspace( char c );
	int		_isnewline( char c );
	char	_skipComment( DWORD * pPos  = NULL );
	bool	_isnumber( char c );
	eSTRINGTABLE_TOKEN_TYPE			_getNextNewLine();
	eSTRINGTABLE_TOKEN_TYPE			_getNextWhiteSpace();

public:
	STRING_LIST_TYPE m_stdHmStringMG;

protected:	
	CDrawCtrl*  m_pDraw;	

	DWORD		m_dwBufferPos;
	DWORD		m_dwBufferSize;
	char *		m_pszBuffer;
	char		m_pszTokenString[_MAX_UI_STRING_LENGTH];
};

