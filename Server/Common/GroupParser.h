#ifndef _GROUPPARSER_H_
#define _GROUPPARSER_H_


#pragma once


#include <SolarHashTable.h>
#include <Singleton.h>

#include <SolarFileReader.h>

#define MAX_FOLLOWER_NUM		5

struct FOLLOWERINFO
{
	DWORD			dwMonCode;
	WORD			wNum;
};

struct BASE_GROUPINFO
{
	WORD			wGroupID;							// ��ȣ
	DWORD			dwFormationType;					// ���� Ÿ��
	float			fLinkRange;							// ����
	DWORD			dwLeaderCode;						// ���� �ڵ�
	FOLLOWERINFO	FollowerInfo[MAX_FOLLOWER_NUM];		// ���� ����
};

class GroupParser : public util::Singleton<GroupParser>, public IParser
{
public:
	GroupParser();
	~GroupParser();

	VOID					Init( DWORD dwPoolSize );
	VOID					Release();

	VOID					Reload();

	BASE_GROUPINFO *		GetGroupInfo( WORD wGroupID ) { return m_pGroupHashTable->GetData( wGroupID ); }

private:
	BOOL					_Load( BOOL bReload );
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
	BOOL					CheckValidity( BASE_GROUPINFO *pGroupInfo );
	VOID					Unload();


private:
	util::SolarHashTable<BASE_GROUPINFO *>*		m_pGroupHashTable;
	char										m_pszFileName[MAX_PATH];
};

#endif // _GROUPPARSER_H_