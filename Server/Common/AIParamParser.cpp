#include "stdafx.h"
#include "AIParamParser.h"
#include <SolarFileReader.h>
#include <NPCInfoParser.h>
#include <iostream>

//==================================================================================================
// (CHANGES) (WAVERIX) add const reference for property i/o
namespace internal {

static BYTE AICommonInfoStream[sizeof(AI_COMMON_INFO)] = { 0, };

}; //end of namespace

const AI_COMMON_INFO&
AIParamParser::AICommonInfo = reinterpret_cast<AI_COMMON_INFO&>(::internal::AICommonInfoStream);

//==================================================================================================
AIParamParser::AIParamParser()
{
}

AIParamParser::~AIParamParser()
{
}

void
AIParamParser::Release()
{
    m_AITypeTable.clear();
}

void
AIParamParser::Init(DWORD dwPoolSize)
{
    __UNUSED(dwPoolSize);
    m_AITypeTable.clear();
}

void
AIParamParser::Reload()
{
    //  _LoadCommon(m_pszCommonFileName, true);
    //  _LoadType(m_pszTypeFileName, true);
}

BOOL
AIParamParser::CheckNPCInfoParser()
{
    bool success = true;
    // (f100606.2L) changed interfaces to support begin and end interface
    const NPCInfoParser* const npc_parser = NPCInfoParser::Instance();
    npc_parser->SetFirst();
    while (const BASE_NPCINFO* npc_info = npc_parser->GetNext())
    {
        const AI_TYPE_INFO* ai_type = GetTypeInfo(npc_info->m_wAICode);
        if (ai_type == NULL)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|[%s]|the monster(%d)'s m_wAICode(%d) can't find in ai table"),
                   __FUNCTION__, npc_info->m_MonsterCode, npc_info->m_wAICode);
            success = false;
        }
    }
    return success;
}

/*
AI_TYPE_INFO*
AIParamParser::GetTypeInfo(AICODE_T wAICode)
{
    AI_CODE_TABLE_IT itFIND = m_AITypeTable.find(wAICode);
    if(itFIND == m_AITypeTable.end())
        return NULL;

    AI_TYPE_INFO& rINFO = itFIND->second;
    return &rINFO;
}
*/

BOOL
AIParamParser::_LoadCommon(BOOL bReload)
{
    __UNUSED(bReload);
    int nRowSize = GetRowSize();

    for(int nRow = 0; nRow < nRowSize; ++nRow)
    {
        ZeroMemory(&m_AICommonInfo, sizeof(m_AICommonInfo));

        // Battle Record 관련
        m_AICommonInfo.m_byDAMAGE_POINT_REDUCE_RATIO    = GetDataBYTE("DamagePointReduceRatio", nRow);
        m_AICommonInfo.m_wMIN_DAMAGE_POINT              = GetDataWORD("MinDamagePoint", nRow);
        m_AICommonInfo.m_fSUMMON_FOLLOW_DISTANCE        = GetDataFloat("SummonFollowDist", nRow);
        m_AICommonInfo.m_fSUMMON_MAX_FOLLOW_DISTANCE    = GetDataFloat("SummonTeleportDist", nRow);
        m_AICommonInfo.m_wKNOCKDOWN_TIME                = GetDataWORD("KnockDownTime", nRow);
        m_AICommonInfo.m_wTHRUST_TIME                   = GetDataWORD("ThrustTime", nRow);
        m_AICommonInfo.m_fMIN_MOVE_DISTANCE             = GetDataFloat("MinMoveDist", nRow);
        m_AICommonInfo.m_fMAX_MOVE_DISTANCE             = GetDataFloat("MaxMoveDist", nRow);
        m_AICommonInfo.m_fMIN_MOVABLE_DISTANCE          = GetDataFloat("MinMovableDist", nRow);
        m_AICommonInfo.m_fGROUP_FOLLOWER_MIN_RADIUS     = GetDataFloat("GroupFollowerMinRadius", nRow);
        m_AICommonInfo.m_fGROUP_FOLLOWER_MAX_RADIUS     = GetDataFloat("GroupFollowerMaxRadius", nRow);

#if !defined(_NA_003649_20111115_BATTLE_ZONE_HARDMODE)
        m_AICommonInfo.m_fMOVE_HEIGHT_LIMIT             = GetDataFloat("MoveHeightLimit", nRow);  // Ver 1.1
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE
        m_AICommonInfo.m_fSIGHT_HEIGHT_LIMIT            = GetDataFloat("SightHeightLimit", nRow);  // Ver 1.1
        m_AICommonInfo.m_fATTACK_RANGE_HEIGHT_RATIO     = GetDataFloat("AttackRangeHeightRatio", nRow); // Ver 1.2

        // 점프 상태
        m_AICommonInfo.m_fJUMP_MIN_HEIGHT               = GetDataFloat("JumpMinHeight", nRow);
        m_AICommonInfo.m_fJUMP_MAX_HEIGHT               = GetDataFloat("JumpMaxHeight", nRow);
        m_AICommonInfo.m_fJUMP_MIN_DISTANCE             = GetDataFloat("JumpMinDist", nRow);
        m_AICommonInfo.m_fJUMP_MAX_DISTANCE             = GetDataFloat("JumpMaxDist", nRow);

        m_AICommonInfo.m_fJUMP_ATTACK_MIN_DISTANCE      = GetDataFloat("JumpAttackMinDist", nRow);
        m_AICommonInfo.m_fJUMP_ATTACK_MAX_DISTANCE      = GetDataFloat("JumpAttackMaxDist", nRow);
        m_AICommonInfo.m_fJUMP_ATTACK_RANGE_RATIO       = GetDataFloat("JumpAttackRangeRatio", nRow);

        // FallApart 상태
        m_AICommonInfo.m_fFALLAPART_MIN_ATTACK_RANGE_LIMIT = GetDataFloat("FallapartMinAttackRangeLimit", nRow);;

        // Retreat 상태
        m_AICommonInfo.m_fRETREAT_MIN_ATTACKRANGE_LIMIT     = GetDataFloat("RetreatMinAttackRangeLimit", nRow);;
        m_AICommonInfo.m_fRETREAT_MIN_DISTANCE_LIMIT_RATIO  = GetDataFloat("RetreatMinDistanceRangeLimitRatio", nRow);;

        // Help 상태
        m_AICommonInfo.m_fHELP_SIGHTRANGE_RATIO         = GetDataFloat("HelpSightRangeRatio", nRow);;

        // 기타
        m_AICommonInfo.m_fRANGE_TOLERANCE               = GetDataFloat("RangeTolerance", nRow);;
        m_AICommonInfo.m_wSEARCH_ROTATE_ANGLE           = GetDataWORD("SearchRotateAngle", nRow);;
        m_AICommonInfo.m_byMAX_OBSERVERS_PER_PLAYER     = GetDataBYTE("MaxObserversPerPlayer", nRow);;
        m_AICommonInfo.m_wGROUP_AIMESSAGE_MIN_DELAY     = GetDataWORD("GroupAIMessageMinDelay", nRow);;
        m_AICommonInfo.m_wGROUP_AIMESSAGE_MAX_DELAY     = GetDataWORD("GroupAIMessageMaxDelay", nRow);;
        m_AICommonInfo.m_wTRACK_INNER_ANGLE             = GetDataWORD("TrackInnerAngle", nRow);;

        // (CHANGES) (WAVERIX) add const reference for property i/o
        if(nRow != 0) {
            __debugbreak();
        }
        BOOST_STATIC_ASSERT(sizeof(::internal::AICommonInfoStream) == sizeof(m_AICommonInfo));
        CopyMemory(&::internal::AICommonInfoStream, &m_AICommonInfo,
                   sizeof(::internal::AICommonInfoStream));
    }

    return true;
}

BOOL
AIParamParser::_LoadType(BOOL bReload)
{
    int nRowSize = GetRowSize();
    char szTemp[MAX_FIELD_NAME_SIZE];

    for(int nRow = 0; nRow < nRowSize; ++nRow)
    {
        AICODE_T wAICode = GetDataWORD("Code", nRow);

        AI_CODE_TABLE::iterator itFind = m_AITypeTable.find(wAICode);
        if(itFind != m_AITypeTable.end())
        {
            if(false == bReload)
            {
                SUNLOG(eCRITICAL_LOG, "Script File Data Error, File = %s, Code = %d",  GetCurFileName(), wAICode);
                ASSERT(!"데이터에 오류가 있습니다.");
            }
        }

        AI_TYPE_INFO& rINFO = m_AITypeTable[wAICode];
        ZeroMemory(&rINFO, sizeof(rINFO));
        rINFO.m_wCode = wAICode;

        // Battle Record 관련
        rINFO.m_wSEARCH_TARGET_TIME = GetDataWORD("SearchTargetTime", nRow);
        rINFO.m_wAGGRO_TIME = GetDataWORD("AggroTime", nRow);
        rINFO.m_wBATTLE_RECORD_UPDATE_TIME = GetDataWORD("BattleUpdateTime", nRow);
        rINFO.m_byPOINT_INIT_RANDOM_RATIO = GetDataBYTE("PointInitRandomRatio", nRow);
        rINFO.m_byTARGET_CHANGE_RATIO = GetDataBYTE("TargetChangeRatio", nRow);

#ifdef _NA_0_20100819_AGGRO_RENEWAL
		for(int i = 0; i < AGGRO_CHANGE_RATIO_MAX; ++i)
        {	
            _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "AggroChangeRange%d", i+1);
			rINFO.m_Distance_Info[i].change_range_ = GetDataWORD(szTemp, nRow);
			_snprintf(szTemp, MAX_FIELD_NAME_SIZE, "AggroChangePoint%d", i+1);
			rINFO.m_Distance_Info[i].change_point_ = GetDataWORD(szTemp, nRow);            
        }
#endif

        for(int i = 1; i < eCHAR_TYPE_MAX; ++i)
        {
            WORD class_base_point = 0;
            if (GetSafeNumericData(
                __FUNCTION__, __LINE__, nRow, class_base_point, "ClassBasePoint%d", i) == false)
            {
                continue;
            }
            rINFO.m_wCLASS_BASE_POINT[i] = class_base_point;
        }

        rINFO.m_wFIRST_ATTACK_POINT = GetDataWORD("FirstAttPoint", nRow);
        rINFO.m_wNEAR_DISTANCE_POINT_INC = GetDataWORD("NearDistPointInc", nRow);
        rINFO.m_wNEAR_DISTANCE_POINT_MAX = GetDataWORD("NearDistPointMax", nRow);
        rINFO.m_wLOW_LEVEL_POINT_INC = GetDataWORD("LowLevelPointInc", nRow);
        rINFO.m_wLOW_LEVEL_POINT_MAX = GetDataWORD("LowLevelPointMax", nRow);
        rINFO.m_wLOW_HP_POINT_INC = GetDataWORD("LowHPPointInc", nRow);
        rINFO.m_wLOW_HP_POINT_MAX = GetDataWORD("LowHPPointMax", nRow);
#if !defined(_NA_0_20100819_AGGRO_RENEWAL)
        rINFO.m_wDAMAGE_POINT_INC = GetDataWORD("DamagePointInc", nRow);
        rINFO.m_wDAMAGE_POINT_MAX = GetDataWORD("DamagePointMax", nRow);
#endif
        // 상태 주기 시간
        rINFO.m_wSEARCH_PERIOD = GetDataWORD("SearchPeriod", nRow);
        rINFO.m_wRETREAT_PERIOD = GetDataWORD("RetreatPeriod", nRow);
        rINFO.m_wTRACK_PERIOD = GetDataWORD("TrackPeriod", nRow);
        rINFO.m_wIDLE_MIN_TIME = GetDataWORD("IdleMinTime", nRow);
        rINFO.m_wIDLE_MAX_TIME = GetDataWORD("IdleMaxTime", nRow);
        rINFO.m_wRUNAWAY_TIME = GetDataWORD("RunAwayTime", nRow);

        // 거리 관련
        rINFO.m_fREGEN_LOCATION_LIMIT = GetDataFloat("RegenLocationLimit", nRow);
        rINFO.m_fWANDER_RADIUS_FROM_REGENAREA = GetDataFloat("WanderRadiusFromRegenArea", nRow);
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
        rINFO.m_fMOVE_HEIGHT_LIMIT = GetDataFloat("MoveHeightLimit", nRow);
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

        // Help 상태
        rINFO.m_fHELP_REQUEST_HP_PERCENT = GetDataFloat("HelpRequestHPPercent", nRow);

        // HP, MP 리젠 주기 관련
        rINFO.m_wNPC_HPMP_REGEN_PERIOD = GetDataWORD("NPCHpMpRegenPeriod", nRow);
        //__NA_001290_20090525_SHIELD_SYSTEM
        rINFO.m_wNPC_SD_REGEN_PERIOD = GetDataWORD("NPCSDRecoveryPeriod", nRow);
        // 럭키몬스터
        rINFO.m_dwLUCKY_MONSTER_RUNAWAY_TIME_MIN = GetDataDWORD("LuckyMonsterRunawayTimeMin", nRow);
        rINFO.m_dwLUCKY_MONSTER_RUNAWAY_TIME_MAX = GetDataDWORD("LuckyMonsterRunawayTimeMax", nRow);
        rINFO.m_dwLUCKY_MONSTER_REGEN_TIME = GetDataDWORD("LuckyMonsterRegenTime", nRow);
    }

    // NPC와의 AICode 링크부분을 검증
    return CheckNPCInfoParser();
}


BOOL
AIParamParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    switch(ScriptCode)
    {
    case SCRIPT_AICOMMONINFO:
        return _LoadCommon(bReload);
    case SCRIPT_AITYPEINFO:
        return _LoadType(bReload);
    };

    return false;
}


