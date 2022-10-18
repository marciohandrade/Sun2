#pragma once

#define		MAX_GMCMD_STRING	128
#define		GMSTRING_FILE		"data\\GMCommand.ini"


class GMCmdString
{
public:
	GMCmdString(void);
	~GMCmdString(void);

private:
//���θ�ɾ�
	TCHAR	m_szWorldNotice[MAX_GMCMD_STRING];			// ��ü ���� ����
	TCHAR	m_szChannelNotice[MAX_GMCMD_STRING];		// ä�� ����
	TCHAR	m_szZoneNotice[MAX_GMCMD_STRING];			// ���� ����

public:
	bool		Init();


public:
	TCHAR*	GetWorldNotice(){ return m_szWorldNotice; }
	TCHAR*	GetChannelNotice(){ return m_szChannelNotice; }
	TCHAR*	GetZoneNotice(){ return m_szZoneNotice; }
};
