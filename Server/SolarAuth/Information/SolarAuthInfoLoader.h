#pragma once

//=============================================================================================================================
/// SolarAuthInfoLoader class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 11
	@remark
	- SolarAuthServer
	@note
	- SolarAuthServer ������ ȯ�� ���� �δ�
	@history 
	-  
*/
//=============================================================================================================================
#include "FileListLoader.h"

typedef DWORD	SOLAR_AUTH_SERVER_INDEX_T;


typedef struct sSOLAR_AUTH_SERVER_INFO
{
	SOLAR_AUTH_SERVER_INDEX_T	m_dwMasterServerIndex;
	struct
	{
		CODE_NATION_T			m_byNationCode;
		CODE_REGION_T			m_byRegionCode;
		MODE_SERVER_T			m_byServerMode;
		PADDING_T				m_byPaddingType;
	};
	CRYPT_KEY_T					m_CryptKey;
	CHAR						m_strIPAddress[16];	// 16 -> ��ü�� ���� ���� �ִ��� Ȯ���� ��

	// Addin Attribute
	DWORD						m_dwReloadInterval;

	CHAR						m_strDataPath[32];
} SOLAR_AUTH_SERVER_INFO;

typedef Delegator<SOLAR_AUTH_SERVER_INFO>		SOLAR_AUTH_SERVER_INFO_DELEGATOR;
//
//
//
class SolarAuthInfoLoader
{
public:
	SolarAuthInfoLoader();
	~SolarAuthInfoLoader();

public:
	VOID			Release();
	BOOL			Load( const CHAR* pszFileName, BOOL bReload = FALSE );

	DWORD			GetReloadTimeInterval() { return m_ServerInfo.m_dwReloadInterval; }

	SOLAR_AUTH_SERVER_INFO_DELEGATOR&
		GetSolarAuthServerInfo() { return m_ServerInfoDelegator; }

private:
	VOID			_Init();
	static INT		ParseInfoSection( PVOID pPassingPtr, AttrTag tagName );
	static BOOL		ParseInfoSectionData( PVOID pPassingPtr, AttrValueName tagValueName, AttrValue tagValue );

private:
	// <FIELDs>
	BOOL								m_bInited;
	DWORD								m_dwCurrentTick;
	SOLAR_AUTH_SERVER_INFO_DELEGATOR	m_ServerInfoDelegator;
	SOLAR_AUTH_SERVER_INFO				m_ServerInfo;	// simple timer�� ������ ã�ƺ� ��
};


