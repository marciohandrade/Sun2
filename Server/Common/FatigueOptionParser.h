#pragma once

#include <Singleton.h>
#include <map>

struct _FATIGUE_COMMON
{
	BOOL		m_bUse;
	BYTE		m_byTypeCount;
	BYTE		m_byInitType;
	WORD		m_wCheckCycle;
};

struct _FATIGUE_APPLY
{
	BOOL		m_bVillageTime;
	BOOL		m_bLobbyTime;
	BOOL		m_bSuperMaster;
	BOOL		m_bDeadTime;
	BOOLEAN		m_bInSSQField;
};

struct _FATIGUE_TYPE
{
	BYTE		m_byType;
	int			m_nStartTime;
	int			m_nEndTime;
	WORD		m_wMsgInterval;
	BYTE		m_byItemDropRatio;
	BYTE		m_byExpRatio;
	BOOL		m_bItemPickUp;
	BYTE		m_bySharedItemPickupRatio;
	BYTE		m_bySharedHeimPickupRatio;
	BOOL		m_bItemReward;
	BYTE		m_byHeimRewardRatio;
	BOOL		m_byExpRewardRatio;
};

typedef STLX_MAP<BYTE, _FATIGUE_TYPE*>		FATIGUE_TYPE_MAP;
typedef	FATIGUE_TYPE_MAP::iterator			FATIGUE_TYPE_MAP_ITER;

class FatigueOptionParser : public util::Singleton<FatigueOptionParser>, public IParser
{
public:
	FatigueOptionParser(void);
	~FatigueOptionParser(void);

private:
	_FATIGUE_COMMON			m_FatigueCommon;
	_FATIGUE_APPLY			m_FatigueApply;
	FATIGUE_TYPE_MAP		m_mapFatigueType;

public:
	BOOL					IsUse(){ return m_FatigueCommon.m_bUse; }
	WORD					GetCheckCycle(){ return m_FatigueCommon.m_wCheckCycle; }
	BYTE					GetInitType(){ return m_FatigueCommon.m_byInitType; }
	BOOL					ApplyVillageTime(){ return m_FatigueApply.m_bVillageTime; }
	BOOL					ApplyLobbyTime(){ return m_FatigueApply.m_bLobbyTime; }
	BOOL					ApplySuperMaster(){ return m_FatigueApply.m_bSuperMaster; }
	BOOL					ApplyDeadTime(){ return m_FatigueApply.m_bDeadTime; }
	BOOLEAN					ApplyInSSQField()	{ return m_FatigueApply.m_bInSSQField; }
	int						GetMsgInterval( int nPlayTime );
	_FATIGUE_TYPE*			GetFatigueTypeFromTime( int nPlayTime );
	_FATIGUE_TYPE*			GetFatigueType( BYTE byType );

public:
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
			BOOL			_Load( BOOL bReload );

	virtual VOID			Release();

};
