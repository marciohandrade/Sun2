#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

enum MATCH_ABUSE_TYPE
{
	MATCH_ABUSE_JOINTIMES		= 1,		// �ѹ濡 ������ ����ȸ�� ����
	MATCH_ABUSE_DISPENALTY		= 2,		// ��Ŀ�ؽ� ����
};

struct BASE_MatchInfo 
{
	DWORD		m_dwID;
	DWORD		m_dwMatchLimitTime;
	BYTE		m_byMatchLimitCount;
};

class MatchInfoParser : public util::Singleton<MatchInfoParser>, public IParser
{
public:
	MatchInfoParser();
	~MatchInfoParser();

	VOID				Init( DWORD dwPoolSize );
	VOID				Release();
	VOID				Reload();

	BASE_MatchInfo*		GetMatchInfo( DWORD dwKey );

private:
	BOOL				_Load( BOOL bReload );
	VOID				Unload();
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	util::SolarHashTable<BASE_MatchInfo *>*	m_pDataTable;
	TCHAR									m_pszFileName[MAX_PATH];
};