#ifdef _NA_008012_20150130_RANKING_SYSTEM

#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <hash_map>

struct st_PointRankingTable
{
	WORD		wIndex;				//�ε���

	DWORD		dwNomal_Per;		//�Ϲ� ���� ��� Ȯ��
	DWORD		dwNomal_Min;		//�Ϲ� ���� ����Ʈ �ּ�
	DWORD		dwNomal_Max;		//�Ϲ� ���� ����Ʈ �ִ�

	DWORD		dwLeader_Per;		//���� ���� ��� Ȯ��
	DWORD		dwLeader_Min;		//���� ���� ����Ʈ �ּ�
	DWORD		dwLeader_Max;		//���� ���� ����Ʈ �ִ�

	DWORD		dwCaptain_Per;		//ĸƾ ���� ��� Ȯ��
	DWORD		dwCaptain_Min;		//ĸƾ ���� ����Ʈ �ּ�
	DWORD		dwCaptain_Max;		//ĸƾ ���� ����Ʈ �ִ�

	DWORD		dwVeteran_Per;		//���� ���� ��� Ȯ��
	DWORD		dwVeteran_Min;		//���� ���� ����Ʈ �ּ�
	DWORD		dwVeteran_Max;		//���� ���� ����Ʈ �ִ�

	DWORD		dwBoss_Per;			//���� ���� ��� Ȯ��
	DWORD		dwBoss_Min;			//���� ���� ����Ʈ �ּ�
	DWORD		dwBoss_Max;			//���� ���� ����Ʈ �ִ�

	LEVELTYPE	lvApplyMinLV;		//����Ʈ ��ŷ ���� �ּ� ����
	LEVELTYPE	lvApplyMaxLV;		//����Ʈ ��ŷ ���� �ִ� ����
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