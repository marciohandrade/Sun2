#pragma once

#include <ScriptParserManager.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

			//원래의 솔라로드는 다른 스크립트파서에서는 정의하지 않는다. IParser에서 결국 LoadScript를 호출하기 때문이다.
			//그런데 여기서의 SloarLoad의 압축된 데이터를 푸는 의미를 가진 함수이다. 따라서 재정의 해준다.
			//재정의 하는 이유는 IParser의 SolarLoad에서는 데이터를 해석해서 테이블 구조까지 만들기 때문이다. 따라서 전혀 다른 로직이 필요하다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<MasterScriptParserManager>
//
class MasterScriptParserManager : public ScriptParserManager
{
public:
	MasterScriptParserManager(void);
	~MasterScriptParserManager(void);

public:		// GAMESERVER에 특화됨.. 각 서버에 특화된 파서 로드 함수를 여기서 선언.
	virtual	VOID	InitParserPool( VOID* pPoolOption );
	virtual BOOL	RegisterParser();
	virtual	VOID	DestroyInstanceEx();

public:		inline VOID				SetCompressInfo( const eSCRIPT_CODE scriptCode, const DWORD compressSz, const DWORD plainSz )
			{	m_CompressConverter.SetCompressInfo( scriptCode, compressSz, plainSz ); }
private:	IParserInMaster			m_CompressConverter;
};
