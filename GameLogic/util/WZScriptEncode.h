// WZScriptEncode.h: interface for the CWZScriptEncode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WZSCRIPTENCODE_H__80ABB03B_9F64_49C0_B541_2083B4522368__INCLUDED_)
#define AFX_WZSCRIPTENCODE_H__80ABB03B_9F64_49C0_B541_2083B4522368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_XORCODE_TABLE_COUNT		10
#define MAX_CODE					65000					// 최대 코드 갯수 

static enum WZSMDToken 
{
	T_NAME, 
	T_NUMBER, 
	T_END, 
	T_COMMAND = '#',
	T_LP = '{',
	T_RP = '}',
	T_COMMA = ',',
	T_SEMICOLON = ';',
	T_SMD_ERROR
};


class CWZScriptEncode  
{
	enum {
		ETC_ENCRYPT,	// 게임서버 옵션 파일
		ENCRYPT			// 아이템, 스킬에 대한 정보  
	};

	char*	m_pBuffer;	
	int		m_iBufferSize;
	int		m_iBufferCount;

	HANDLE	m_hFile;

	char*	m_Token;

	float    TokenNumber;
	char     TokenString[100];
	WZSMDToken CurrentToken;
	
	BYTE	m_btFileType;	// 파일 속성 
	BYTE	m_XorTable[MAX_XORCODE_TABLE_COUNT];
	BYTE	m_EtcXorTable[5];

public:
	CWZScriptEncode();
	virtual ~CWZScriptEncode();

	int			Open(char* filname, BYTE btFileType = ENCRYPT );
	void		Close();

	void		SetScriptParsingBuffer(char* buffer, int size);

	WZSMDToken	GetToken();
	WZSMDToken	TokenType(){return CurrentToken;}
	int			GetNumber();
	char*		GetString();

	void		Encode(char* buffer, int size );
protected:
	int			GetC();
	void		UnGetC(int ch);	
};

#endif // !defined(AFX_WZSCRIPTENCODE_H__80ABB03B_9F64_49C0_B541_2083B4522368__INCLUDED_)
