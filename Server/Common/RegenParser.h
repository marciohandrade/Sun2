#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <ProgramCommon/wzdatatype.h>
#include "ServerCommonDef.h"

#include <SolarFileReader.h>

#define MAX_REGEN_DESC_LEN			32
#define MAX_GROUP_NUM				3
#define MAX_REGEN_MONSTER_KIND		7

struct REGEN_INFO
{
	DWORD		dwNum;									// ��ȣ
	MAPCODE		MapCode;								// ���ڵ�
	DWORD		dwFieldIndex;							// �ʵ� �ε���
	DWORD		dwLocationID;							// ���� ID
	WORD		wMonsterType;							// ���� Ÿ��
	BOOL		bAllRegen;								// ��� ����
	BYTE		byMoveAttitude;							// �̵� ����(��Ʈ��, ���� ������, ���� ������...)
	DWORD		dwRegenTime;							// ���� �ð�
	float		fRegenRate;								// ���� Ȯ��
	WORD		wGroupID[MAX_GROUP_NUM];				// �׷� ID
	DWORD		dwMonCode[MAX_REGEN_MONSTER_KIND];		// ���� �ڵ�
	DWORD		dwMaxNum[MAX_REGEN_MONSTER_KIND];		// �ִ� ���ͼ�

	BOOL		bPosType;								// ��ġ ��� ���� : 0=�ٿ��ڽ����, 1=x,y,z ���
	WzVector	pvPos;									// ��ġ
	BYTE		bDistance;								// �Ÿ�
	BYTE		bPattern;								// ���� ����

	char		szDesc[MAX_REGEN_DESC_LEN+1];			// ����
};


//DWORD64 Ű�� ���ʿ�
typedef STLX_HASH_MAP<DWORD, REGEN_INFO*>						REGENINFO_HASH;
typedef REGENINFO_HASH::iterator								REGENINFO_HASH_ITR;

class RegenParser : public util::Singleton<RegenParser>, public IParser
{
public:
	RegenParser();
	~RegenParser();

	VOID						Init( DWORD dwPoolSize );
	VOID						Release();

	VOID						Reload();

	inline REGENINFO_HASH_ITR	BeginRegen()		{ return m_RegenHashTable.begin(); }
	inline REGENINFO_HASH_ITR	EndRegen()			{ return m_RegenHashTable.end(); }

	template<typename _FN, typename _OBJ>
	VOID						ForeachRegen( _FN _func, _OBJ& _left )
	{	for_each( m_RegenHashTable.begin(), m_RegenHashTable.end(), bind1st(mem_fun(_func), _left) );	}

	VOID						Unload();

	virtual	BOOL				LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
			BOOL				_LoadPos( BOOL bReload );
			BOOL				_LoadRegen( BOOL bReload );
private:
	BOOL						CheckValidity( REGEN_INFO* pRegenInfo );

	REGENINFO_HASH				m_RegenHashTable;
	TCHAR						m_pszFileName[MAX_PATH];


//////////////////////////////////////////////////////////////////////////
// UNUSED_SECTION
public:
	BOOL						AddMonster(REGEN_INFO* pInfo, DWORD dwID);
	BOOL						DelMonster(REGEN_INFO* pInfo, DWORD dwID);
	INT							GetHashSize() { return (INT)m_RegenHashTable.size(); };
//////////////////////////////////////////////////////////////////////////
};






