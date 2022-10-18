#ifndef _WORLDSERVERINFOPARSER_H_
#define _WORLDRSERVERINFOPARSER_H_

#pragma once

#include <Singleton.h>
#include <Define.h>
#include <ServerStruct.h>
class WorldServerInfoParser : public util::Singleton<WorldServerInfoParser>
{
public:
	WorldServerInfoParser(void);
	~WorldServerInfoParser(void);

	BOOL				ParseINI( char *szFileName );

	inline BOOL			IsServerInfoFromFile()	{ return m_bServerInfoFromFile; }
	inline DWORD		GetWorldID() { return m_dwWorldID; }
	inline char*		GetMasterServerIP()		{ return m_szMasterServerIP; }
	inline WORD			GetMasterServerPort()	{ return m_wMasterServerPort; }
	inline char*		GetGameDBProxyIP() { return m_szGameDBProxyIP; }
	inline WORD			GetGameDBProxyPort() { return m_wGameDBProxyPort; }

	INI_IOHANDLER_DESC* GetClientIoDesc()	{ return &m_ClientIoDesc; }
	INI_IOHANDLER_DESC* GetServerIoDesc()	{ return &m_ServerIoDesc; }

	SUNLOG_INFO			*GetSunLogInfo()		{ return &m_SunLogInfo; }
	inline char			*GetLogFilePath()		{ return m_szLogFilePath; }
	BYTE				GetLogFileOption()		{ return m_byLogOption; }
	DWORD				GetStatisticsInterval() { return m_dwStatisticsInterval; }

	int					IsServiceMode()			{ return m_nServiceMode; }

	int					IsCryptable()			{ return m_nCryptable; }
	VOID				ReloadShoutInterval();
	DWORD&				GetShoutInterval()		{ return m_dwShoutInterval; }
	VOID				ReloadFileNotice();
	BYTE&				GetFileNotice()			{ return m_byFileNotice; }
	BYTE&				GetServerStatus()		{ return m_byServerStatus; }

private:
	BOOL				m_bServerInfoFromFile;
	DWORD				m_dwWorldID;
	INI_IOHANDLER_DESC	m_ClientIoDesc;
	INI_IOHANDLER_DESC	m_ServerIoDesc;

	char				m_szMasterServerIP[MAX_IPADDRESS_SIZE + 1];
	WORD				m_wMasterServerPort;
	char				m_szGameDBProxyIP[MAX_IPADDRESS_SIZE + 1];
	WORD				m_wGameDBProxyPort;
	SUNLOG_INFO			m_SunLogInfo;
	char				m_szLogFilePath[MAX_LOGPATH_LEN];
	BYTE				m_byLogOption;
	DWORD				m_dwStatisticsInterval;							// ��� ���ۿ� �ð� �ֱ�
	int					m_nServiceMode;									// ���񽺸�� ų�ų�?(0:�ܼ� 1:����)
	int					m_nCryptable;									// ��ȣȭ ���� �Ⱦ���(0: ������ 1: ����)
	DWORD				m_dwShoutInterval;								// ��ġ��� �ð� �ֱ�
	char				m_szFullPath[512];
	BYTE				m_byFileNotice;									// ���� ������ ����� ������ üũ(0: �̻�� 1: ���)
	BYTE				m_byServerStatus;								// ���� ����(1: ����, 2:�系, 3:����)
};


#endif // _MASTERSERVERINFOPARSER_H_