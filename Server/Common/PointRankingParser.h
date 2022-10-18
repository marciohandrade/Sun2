#ifdef _NA_008012_20150130_RANKING_SYSTEM

#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <hash_map>

struct st_PointRankingTable
{
	WORD		wIndex;				//인덱스

	DWORD		dwNomal_Per;		//일반 몬스터 드랍 확률
	DWORD		dwNomal_Min;		//일반 몬스터 포인트 최소
	DWORD		dwNomal_Max;		//일반 몬스터 포인트 최대

	DWORD		dwLeader_Per;		//리더 몬스터 드랍 확률
	DWORD		dwLeader_Min;		//리더 몬스터 포인트 최소
	DWORD		dwLeader_Max;		//리더 몬스터 포인트 최대

	DWORD		dwCaptain_Per;		//캡틴 몬스터 드랍 확률
	DWORD		dwCaptain_Min;		//캡틴 몬스터 포인트 최소
	DWORD		dwCaptain_Max;		//캡틴 몬스터 포인트 최대

	DWORD		dwVeteran_Per;		//정예 몬스터 드랍 확률
	DWORD		dwVeteran_Min;		//정예 몬스터 포인트 최소
	DWORD		dwVeteran_Max;		//정예 몬스터 포인트 최대

	DWORD		dwBoss_Per;			//보스 몬스터 드랍 확률
	DWORD		dwBoss_Min;			//보스 몬스터 포인트 최소
	DWORD		dwBoss_Max;			//보스 몬스터 포인트 최대

	LEVELTYPE	lvApplyMinLV;		//포인트 랭킹 적용 최소 레벨
	LEVELTYPE	lvApplyMaxLV;		//포인트 랭킹 적용 최대 레벨
};

class PointRankingParser : public util::Singleton<PointRankingParser>, public IParser
{
public:
	PointRankingParser();
	~PointRankingParser();

	VOID					Release();
	st_PointRankingTable*	GetPointTable( WORD Index );

public:
	BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	BOOL _Load( BOOL bReload );

private:
	STLX_HASH_MAP<WORD, st_PointRankingTable*>		m_PointTableHash;
	TCHAR											m_pszFileName[MAX_PATH];
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // _NA_008012_20150130_RANKING_SYSTEM