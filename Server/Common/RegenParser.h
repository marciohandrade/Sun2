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
	DWORD		dwNum;									// 번호
	MAPCODE		MapCode;								// 맵코드
	DWORD		dwFieldIndex;							// 필드 인덱스
	DWORD		dwLocationID;							// 영역 ID
	WORD		wMonsterType;							// 몬스터 타입
	BOOL		bAllRegen;								// 모두 리젠
	BYTE		byMoveAttitude;							// 이동 성향(패트롤, 스폰 정지형, 완전 정지형...)
	DWORD		dwRegenTime;							// 리젠 시간
	float		fRegenRate;								// 리젠 확률
	WORD		wGroupID[MAX_GROUP_NUM];				// 그룹 ID
	DWORD		dwMonCode[MAX_REGEN_MONSTER_KIND];		// 몬스터 코드
	DWORD		dwMaxNum[MAX_REGEN_MONSTER_KIND];		// 최대 몬스터수

	BOOL		bPosType;								// 위치 출력 형식 : 0=바운드박스방식, 1=x,y,z 방식
	WzVector	pvPos;									// 위치
	BYTE		bDistance;								// 거리
	BYTE		bPattern;								// 리젠 패턴

	char		szDesc[MAX_REGEN_DESC_LEN+1];			// 설명
};


//DWORD64 키가 불필요
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






