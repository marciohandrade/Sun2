#pragma once

#include <ScriptParserManager.h>

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	<ICompressConverter>
//
class ICompressConverter : public IParser
{
protected:
	struct sCOMPRESS_INFO
	{
		sCOMPRESS_INFO() : IParserPtr(0), CompressSz(0), PlainSz(0) {}
		sCOMPRESS_INFO( const DWORD compressSz, const DWORD plainSz ) : IParserPtr(0), CompressSz(compressSz), PlainSz(plainSz) {}
		IParser*	IParserPtr;
		DWORD		CompressSz;
		DWORD		PlainSz;
	};

	typedef STLX_MAP<eSCRIPT_CODE, sCOMPRESS_INFO>	SCRIPT_COMPRESS_INFO_TABLE;
	typedef SCRIPT_COMPRESS_INFO_TABLE::iterator	SCRIPT_COMPRESS_INFO_TABLE_IT;

public:		ICompressConverter() {}
	virtual	~ICompressConverter() {}

	virtual	VOID	Release();

			//������ �ֶ�ε�� �ٸ� ��ũ��Ʈ�ļ������� �������� �ʴ´�. IParser���� �ᱹ LoadScript�� ȣ���ϱ� �����̴�.
			//�׷��� ���⼭�� SloarLoad�� ����� �����͸� Ǫ�� �ǹ̸� ���� �Լ��̴�. ���� ������ ���ش�.
			//������ �ϴ� ������ IParser�� SolarLoad������ �����͸� �ؼ��ؼ� ���̺� �������� ����� �����̴�. ���� ���� �ٸ� ������ �ʿ��ϴ�.
	virtual	BOOL	SolarLoad( eSCRIPT_CODE ScriptCode, BYTE* pSTREAM, DWORD dwSize, BOOL bBigData );
	virtual	BOOL	LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload ){ __UNUSED((ScriptCode, bReload)); return FALSE; }

public:		VOID	SetCompressInfo( const eSCRIPT_CODE scriptCode, const DWORD compressSz, const DWORD plainSz );

			//	<FIELDs>
protected:	SCRIPT_COMPRESS_INFO_TABLE		m_CompressInfos;
};


class IParserInMaster : public ICompressConverter
{
public:		IParserInMaster()  {}
	virtual	~IParserInMaster() {}
			VOID	SetParser( const eSCRIPT_CODE scriptCode, IParser* const pParser );

	virtual	BOOL	SolarLoad( eSCRIPT_CODE ScriptCode, BYTE* pSTREAM, DWORD dwSize, BOOL bBigData );
	virtual	BOOL	LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload ) { __UNUSED((ScriptCode, bReload)); return FALSE; }
};

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	<MasterScriptParserManager>
//
class MasterScriptParserManager : public ScriptParserManager
{
public:
	MasterScriptParserManager(void);
	~MasterScriptParserManager(void);

public:		// GAMESERVER�� Ưȭ��.. �� ������ Ưȭ�� �ļ� �ε� �Լ��� ���⼭ ����.
	virtual	VOID	InitParserPool( VOID* pPoolOption );
	virtual BOOL	RegisterParser();
	virtual	VOID	DestroyInstanceEx();

public:		inline VOID				SetCompressInfo( const eSCRIPT_CODE scriptCode, const DWORD compressSz, const DWORD plainSz )
			{	m_CompressConverter.SetCompressInfo( scriptCode, compressSz, plainSz ); }
private:	IParserInMaster			m_CompressConverter;
};
