#pragma once

#define		MAX_GMCMD_STRING	128
#define		GMSTRING_FILE		"data\\GMCommand.ini"


class GMCmdString
{
public:
	GMCmdString(void);
	~GMCmdString(void);

private:
//메인명령어
	TCHAR	m_szWorldNotice[MAX_GMCMD_STRING];			// 전체 월드 공지
	TCHAR	m_szChannelNotice[MAX_GMCMD_STRING];		// 채널 공지
	TCHAR	m_szZoneNotice[MAX_GMCMD_STRING];			// 지역 공지

public:
	bool		Init();


public:
	TCHAR*	GetWorldNotice(){ return m_szWorldNotice; }
	TCHAR*	GetChannelNotice(){ return m_szChannelNotice; }
	TCHAR*	GetZoneNotice(){ return m_szZoneNotice; }
};
