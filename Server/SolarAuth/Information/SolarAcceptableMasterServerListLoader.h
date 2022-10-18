#pragma once

//=============================================================================================================================
/// SolarAcceptableMasterServerListLoader class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 11
	@remark
	- SolarAuthServer
	@note
	- SolarAuthServer ������ ȯ�� ���� �δ� (���������� ������ ���� ��� ����)
	@history 
	-  
*/
//=============================================================================================================================
#include "FileListLoader.h"
#include "ScriptLoader.h"
#include "ServerList/IServerInfo.h"

#pragma pack(push, 1)

//
// <DEFINITION><DATATYPE>
//

typedef DWORD	MASTER_SERVER_INDEX_T;

typedef struct sACCEPTABLE_MASTER_SERVER_INFO
{
	MASTER_SERVER_INDEX_T	m_dwMasterServerIndex;
	union
	{
		struct
		{
			CODE_NATION_T	m_byNationCode;
			CODE_REGION_T	m_byRegionCode;
			MODE_SERVER_T	m_byServerMode;
			PADDING_T		m_byPaddingType;
		};

		DWORD				m_dwGlobalKey;
	};
	CRYPT_KEY_T				m_CryptKey;
	DWORD					m_dwNetworkIP_Public;
	DWORD					m_dwNetworkIP_Server;
	SCRIPT_LOADER			m_ScriptLoaderHandle;
	ServerTreeINFO*			m_pServerTreeInfo;
	//
	//CHAR					m_strIPAddress[16];	// 16 -> ��ü�� ���� ���� �ִ��� Ȯ���� ��
	CHAR					m_strDataPath[64];

	GLOBAL_SERVER_KEY		GetGlobalMasterKey()
	{
		return GLOBAL_SERVER_KEY(m_dwGlobalKey);
	}

	inline DWORD			GetIPAddress() { return m_dwNetworkIP_Public; }
	inline CRYPT_KEY_T		GetCryptKey() { return m_CryptKey; }

	// �����ͺ� ��ũ��Ʈ �δ� �Ҵ�. 
	SCRIPT_LOADER			GetScriptLoader() { return m_ScriptLoaderHandle; }
	ServerTreeINFO*			GetServerTree() { return m_pServerTreeInfo; }

	VOID					ConvertKeys()
	{
		// <1> GlobalKey�� �������� m_CryptKey�� �����Ѵ�.
		//   ��������:
		//     ( 19381 * ( ServerMode + (11 * (NationCode * RegionCode)) ) ) % 99999 = [n]
		//     BYTE* pStream = (BYTE*)ToString([n])
		//     DWORD cryptKey = *(DWORD*)pStream;
		//
		DWORD valueKey = 11 * m_byNationCode * m_byRegionCode ;
		valueKey += m_byServerMode;
		valueKey *= 19381;
		valueKey %= 99999;
		//DWORD valueKey = ( 19381 * ( 11 * ((DWORD)m_byNationCode * m_byRegionCode) ) + m_byServerMode ) % 99999;

		CHAR strValueKey[32];
		_snprintf( strValueKey, sizeof(strValueKey), "%d" /* +,- */, valueKey );
		valueKey = *(DWORD*)strValueKey;

		m_CryptKey = valueKey;

		// <2> m_strIPAddress�� DWORD���� ��ȯ �� m_dwNetworkIP_Public�� ��ȯ
		//m_dwNetworkIP_Public = (DWORD)inet_addr( m_strIPAddress );
	}

	VOID					ConvertIPAddress( BOOL bIsPublic, CHAR* strIPAddress )
	{
		//m_strIPAddress�� DWORD���� ��ȯ �� m_dwNetworkIP_Public�� ��ȯ
		if( bIsPublic )
			m_dwNetworkIP_Public = (DWORD)inet_addr( strIPAddress );
		else
			m_dwNetworkIP_Server = (DWORD)inet_addr( strIPAddress );
	}

} ACCEPTABLE_MASTER_SERVER_INFO;

#pragma pack(pop)

//
// <DEFINITION><DATATYPE>
//


typedef Delegator<ACCEPTABLE_MASTER_SERVER_INFO>					ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR;
typedef STLX_MAP<DWORD, ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR>	ACCEPTABLE_MASTER_SERVER_HASH;
typedef ACCEPTABLE_MASTER_SERVER_HASH::iterator						ACCEPTABLE_MASTER_SERVER_HASH_IT;
typedef std::pair<DWORD, ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR>	ACCEPTABLE_MASTER_SERVER_PAIR;

typedef STLX_LIST<ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR>			ACCEPTABLE_MASTER_SERVER_LIST;
typedef ACCEPTABLE_MASTER_SERVER_LIST::iterator						ACCEPTABLE_MASTER_SERVER_LIST_IT;

//
//
//

class SolarAcceptableMasterServerListLoader
{
public:
	SolarAcceptableMasterServerListLoader();
	~SolarAcceptableMasterServerListLoader();

public:
	VOID			Release();
	BOOL			Load( const CHAR* pszFileName, BOOL bReload = FALSE );

	ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR&		GetMasterServerByIP( DWORD dwIPAddress );
	ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR&		GetMasterServerByKey( CRYPT_KEY_T cryptKey );

	SCRIPT_LOADER	GetMasterServerDataByIP( DWORD dwIPAddress );
	SCRIPT_LOADER	GetMasterServerDataByKey( CRYPT_KEY_T cryptKey );

	template<typename _FN, typename _OBJ>
	VOID			ForeachServerList( _FN _func, _OBJ& _left )
	{
		std::for_each( m_MasterServerList.begin(), m_MasterServerList.end(), std::bind1st(std::mem_fun(_func), _left) );
	}

private:
	VOID			_Init();
	static INT		ParseInfoSection( PVOID pPassingPtr, AttrTag tagName );
	//static BOOL		ParseInfoSectionData( PVOID pPassingPtr, const CHAR* pszSectionName, const DWORD dwBufSize, const DWORD dwValue );

private:
	//for foreach
	VOID			_ReleaseNode( ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR node );
private:
	// <FIELDs>
	BOOL										m_bInited;
	ACCEPTABLE_MASTER_SERVER_HASH				m_MasterServerIPs;
	ACCEPTABLE_MASTER_SERVER_HASH				m_MasterServerKeys;
	ACCEPTABLE_MASTER_SERVER_LIST				m_MasterServerList;
	ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR		m_EmptyServerInfo;
};



