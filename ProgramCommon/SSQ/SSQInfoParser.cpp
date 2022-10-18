#include "stdafx.h"

#include "SSQInfoStruct.h"
#include "SSQInfoParser.h"

#include "NPCInfoParser.h"
#include "ItemInfoParser.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<DEFINITIONs>
using namespace nsSSQ;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<IMPLEMENTATIONs>

SSQInfoParser::SSQInfoParser()
{
#ifdef _SERVER
	m_pSSQTiles = 0;
#endif
};

SSQInfoParser::~SSQInfoParser()
{
#ifdef _SERVER
	SAFE_DELETE_ARRAY(m_pSSQTiles);
#endif
};

VOID
SSQInfoParser::Init(const TCHAR* pszPackFileName)
{
	SetPackFileName(pszPackFileName);
}

VOID
SSQInfoParser::Release()
{
	FOREACH_CONTAINER(SSQINFO_TABLE::value_type& rSSQInfo, m_SSQInfoTable, SSQINFO_TABLE)
	{
		SSQInfo* pSSQInfo = rSSQInfo.second;
        TAllocDelete(SSQInfo_Info::value_type, pSSQInfo);
	}

	FOREACH_CONTAINER(SSQPATTERN_TABLE::value_type& rSSQPattern, m_SSQPatternTable, SSQPATTERN_TABLE)
	{
		SSQPattern* pSSQPattern = rSSQPattern.second;
        TAllocDelete(SSQPattern_Info::value_type, pSSQPattern);
	}

	m_SSQInfoTable.clear();
	m_SSQPatternTable.clear();
#ifdef _SERVER
	m_SSQTileInfoTable.clear();
	SAFE_DELETE_ARRAY(m_pSSQTiles);
#endif
}


BOOL
SSQInfoParser::LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload)
{
    __UNUSED(bReload);
	switch(scriptCode)
	{
	case SSQInfo_Info::value:
		return _Load(SSQInfo_Info::category(), FALSE);
	case SSQPattern_Info::value:
		return _Load(SSQPattern_Info::category(), FALSE);
	}

	ASSERT(!"Invalid Type :Script Code");

	return FALSE;
}


//────────────────────────────────────────────────────────────────


BOOLEAN
SSQInfoParser::_Load(const SSQInfo_Info::category&, BOOLEAN bReload)
{
	const INT nRowSize = GetRowSize();
	for(INT nRow = 0; nRow < nRowSize; ++nRow)
	{
        SSQInfo* const pSSQInfo = TAllocNew(SSQInfo_Info::value_type);
		ZeroMemory(pSSQInfo, sizeof(SSQInfo));

		pSSQInfo->m_Index			= GetDataWORD("Index", nRow);
		pSSQInfo->m_SSQName.CopyFrom(GetDataString("SSQName", nRow));

		const MAPCODE CodeOfSSQ		= GetDataWORD("MapCode", nRow);
		pSSQInfo->m_MapCode			= CodeOfSSQ;

		SSQINFO_TABLE::iterator itFind = m_SSQInfoTable.find(CodeOfSSQ);
		if(itFind == m_SSQInfoTable.end())
			m_SSQInfoTable.insert(std::make_pair(CodeOfSSQ, pSSQInfo));
		else if(bReload)
		{
			SSQINFO_TABLE::value_type& rPair = *itFind;
			SSQInfo* const pSSQInfoTmp = rPair.second;
            TAllocDelete(SSQInfo_Info::value_type, pSSQInfoTmp);
			rPair.second = pSSQInfo;
		}
		else
		{
			FASSERT(!"SSQInfo::Load-Duplicated ID");
			return FALSE;
		}

		SSQInfo::LEVEL_RANGE& rLevels	= pSSQInfo->m_LevelRange;
		rLevels[SSQInfo::LV_MIN]		= GetDataWORD("EnterableMinLevel", nRow);
		rLevels[SSQInfo::LV_MAX]		= GetDataWORD("EnterableMaxLevel", nRow);
		pSSQInfo->m_Cost				= GetDataInt("Cost", nRow);

		TCHAR string_buffer0[32];
		TCHAR string_buffer1[32];

		{
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			const INT str0_tail_index	= _sntprintf(string_buffer0, _countof(string_buffer0), "EnteranceTime");

			SSQInfo::TIME_RANGE& rTimeRange	= pSSQInfo->m_TimeRange;
			for(DWORD idx = 0; idx < SSQInfo::MI_ENTER_TIME_SECTION_MAX; ++idx)
			{
				string_buffer0[ str0_tail_index ] = (TCHAR)('1' + idx);
				SSQInfo::TimeRange& rNode = rTimeRange[idx];
				rNode.Start				= GetDataWORD(string_buffer0, nRow);
				// rNode.End <- 이것은 나중에 계산된다.
			}
		};
		pSSQInfo->m_WatingTime			= GetDataBYTE("WaitingTime", nRow);
		pSSQInfo->m_QuestPlayTime		= GetDataBYTE("QuestPlayTime", nRow);

		SSQInfo::PLAYER_COUNT_LIMIT& rCountLimit = pSSQInfo->m_PlayerCountInfo;
		rCountLimit[SSQInfo::PCL_MIN]	= GetDataBYTE("MinUserCount", nRow);
		rCountLimit[SSQInfo::PCL_MAX]	= GetDataBYTE("MaxUserCount", nRow);

		SSQInfo::GAMBLE_INFO& rGambleInfo = pSSQInfo->m_GamebleInfo;
		rGambleInfo.m_ChargeRatio		= GetDataBYTE("GambleCharge", nRow);

		{
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			ZeroMemory(string_buffer1, sizeof(string_buffer1));
			const INT str0_tail_index = _sntprintf(string_buffer0, _countof(string_buffer0), "EQCondTime");
			const INT str1_tail_index = _sntprintf(string_buffer1, _countof(string_buffer1), "EQCondUsers");

			SSQInfo::EARTHQUAKE_INFO& rEarth = pSSQInfo->m_EarthquakeInfo;
			for(DWORD idx = 0; idx < SSQInfo::MI_EARTHQUAKE_SECTION_MAX; ++idx)
			{
				string_buffer0[ str0_tail_index ] = (TCHAR)('1' + idx);
				string_buffer1[ str1_tail_index ] = (TCHAR)('1' + idx);

				SSQInfo::EARTHQUAKE_NODE& rNode = rEarth[idx];
				rNode.m_ElapsedTime		= GetDataBYTE(string_buffer0, nRow);
				rNode.m_RemainPlayers	= GetDataBYTE(string_buffer1, nRow);
			}
		};

		SSQInfo::THUNDERBOLT_NODE& rThunder = pSSQInfo->m_ThunderboltInfo;
		rThunder.m_PlayerDamage			= GetDataInt("ThunderboltDamagePC", nRow);
		rThunder.m_NpcDamage			= GetDataInt("ThunderboltDamageNPC", nRow);
		rThunder.m_RaisePerXsec			= GetDataBYTE("ThunderboltInterval", nRow);

		//SSQInfo::CLOSED_SPACE_NODE& rCSpace = pSSQInfo->m_ClosedSpaceInfo;
		//rCSpace.m_PlayerDamage			= GetDataWORD("ClosedSpaceDamagePC", nRow);
		//rCSpace.m_NpcDamage				= GetDataWORD("ClosedSpaceDamageNPC", nRow);
		//rCSpace.m_RaisePerXsec			= GetDataBYTE("ClosedSpaceDamageInterval", nRow);

		{
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			ZeroMemory(string_buffer1, sizeof(string_buffer1));
			const INT str0_tail_index = _sntprintf(string_buffer0, _countof(string_buffer0), "ClosedSpaceDamagePC");
			const INT str1_tail_index = _sntprintf(string_buffer1, _countof(string_buffer1), "ClosedSpaceDamageNPC");

			SSQInfo::CLOSED_SPACE_NODE& rCSpace = pSSQInfo->m_ClosedSpaceInfo;
			for(DWORD idx = 0; idx < SSQInfo::MI_EARTHQUAKE_SECTION_MAX; ++idx)
			{
				string_buffer0[ str0_tail_index   ] = (TCHAR)('1' + idx);
				string_buffer1[ str1_tail_index   ] = (TCHAR)('1' + idx);
				rCSpace.m_PlayerDamages[idx]= GetDataFloat(string_buffer0, nRow);
				rCSpace.m_NpcDamages[idx]	= GetDataFloat(string_buffer1, nRow);
			}
			rCSpace.m_RaisePerXsec			= GetDataBYTE("ClosedSpaceDamageInterval", nRow);
		};

		{
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			ZeroMemory(string_buffer1, sizeof(string_buffer1));
			const INT str0_tail_index = _sntprintf(string_buffer0, _countof(string_buffer0), "NpcCode");
			const INT str1_tail_index = _sntprintf(string_buffer1, _countof(string_buffer1), "NpcRegenRatio");

			SSQInfo::SUMMONDED_NPC_INFO& rMobInfo	= pSSQInfo->m_SummonedNpcInfo;
			for(DWORD idx = 0; idx < SSQInfo::MI_SUMMONED_NPC_SECTION_MAX; ++idx)
			{
				const BYTE remain	= (BYTE)((idx + 1) % 10);
				const BYTE carry	= (BYTE)((idx + 1) / 10);
				string_buffer0[ str0_tail_index   ] = (TCHAR)('0' + carry);
				string_buffer0[ str0_tail_index+1 ] = (TCHAR)('0' + remain);
				string_buffer1[ str1_tail_index   ] = (TCHAR)('0' + carry);
				string_buffer1[ str1_tail_index+1 ] = (TCHAR)('0' + remain);

				rMobInfo.List[idx].m_NPCCode	= GetDataWORD(string_buffer0, nRow);
				const FLOAT regen_ratio_f		= GetDataFloat(string_buffer1, nRow);
				rMobInfo.List[idx].m_SumRatio	= (SSQInfo::RATIO_SUM_T)(regen_ratio_f*100.f);
				if(rMobInfo.List[idx].m_NPCCode)	++rMobInfo.Count;
			}
		};

		{
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			_sntprintf(string_buffer0, _countof(string_buffer0), "HuntingPointPC");
			__ReadHuntingPoint(nRow, string_buffer0, pSSQInfo->m_PlayerHuntingInfo);
		}
		{
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			_sntprintf(string_buffer0, _countof(string_buffer0), "HuntingPointNPC");
			__ReadHuntingPoint(nRow, string_buffer0, pSSQInfo->m_NpcHuntingInfo);
		}

		{
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			const INT str0_tail_index = _sntprintf(string_buffer0, _countof(string_buffer0), "HuntineExpRatio");

			SSQInfo::EXP_RATIO_INFO& rExpRatio = pSSQInfo->m_ExpRatioInfo;
			for(DWORD idx = eCHAR_NONE+1; idx < eCHAR_TYPE_MAX; ++idx)
			{	// 해당 배열은 0을 포함 6개로 이루어져 있다.
				string_buffer0[ str0_tail_index ] = (TCHAR)('0' + idx);
				
                FLOAT exp_ratio = 0.0f;
                if (GetSafeNumericData(
                    __FUNCTION__, __LINE__, nRow, exp_ratio, string_buffer0) == false)
                {
                    continue;
                }
                rExpRatio[idx].m_FloatPoint = exp_ratio;
			}
		}

		{
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			const INT str0_tail_index = _sntprintf(string_buffer0, _countof(string_buffer0), "SuccessPoint");

			SSQInfo::SUCCESS_POINT_INFO& rSuccessInfo = pSSQInfo->m_SuccessPointInfo;
			for(DWORD idx = 0; idx < SSQInfo::MI_SUCCESS_POINT_BY_GRADE; ++idx)
			{
				string_buffer0[ str0_tail_index ] = (TCHAR)('1' + idx);
				rSuccessInfo[idx].m_FloatPoint	= GetDataFloat(string_buffer0, nRow);
			}
		};
    #ifdef _NA000000_100427_SSQ_DEFAULT_REWARD_
        ;{
            SSQInfo::REWARD_NODE* reward = &pSSQInfo->m_DefaultRewardInfo;
            reward->m_RewardCode = GetDataWORD("BasicItemCode", nRow);
            reward->m_RewardLimit = GetDataBYTE("BasicItemNum", nRow);
        };
    #endif
		{	// 경품 OnMonsterKill
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			ZeroMemory(string_buffer1, sizeof(string_buffer1));
			const INT str0_tail_index = _sntprintf(string_buffer0, _countof(string_buffer0), "DropItemCode");
			const INT str1_tail_index = _sntprintf(string_buffer1, _countof(string_buffer1), "DropItemLimit");

			SSQInfo::REWARD_DROP_INFO& rReward	= pSSQInfo->m_RewardDropInfo;
			for(DWORD idx = 0; idx < SSQInfo::MI_REWARD_DROP_INFO_SECTION_MAX; ++idx)
			{
				string_buffer0[ str0_tail_index   ] = (TCHAR)('1' + idx);
				string_buffer1[ str1_tail_index   ] = (TCHAR)('1' + idx);

				SSQInfo::REWARD_NODE& rNode		= rReward.List[idx];
				rNode.m_RewardCode				= GetDataWORD(string_buffer0, nRow);
				rNode.m_RewardLimit				= GetDataBYTE(string_buffer1, nRow);
				if(rNode.m_RewardCode)	++rReward.Count;
			}
		};

		{	// 최종 우승자
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			const INT str0_tail_index = _sntprintf(string_buffer0, _countof(string_buffer0), "RewardItemCode");

			for(DWORD idx = 0; idx < SSQInfo::MI_REWARD_WINNER_INFO_SECTION_MAX; ++idx)
			{
				string_buffer0[ str0_tail_index   ] = (TCHAR)('1' + idx);
				string_buffer0[ str0_tail_index+1 ] = (TCHAR)('_');
				__ReadWinnerRewardInfo(nRow, string_buffer0, pSSQInfo->m_RewardWinnerInfo[idx]);
			}
		};
		//{	// 최종 우승자
		//	ZeroMemory(string_buffer0, sizeof(string_buffer0));
		//	const INT str0_tail_index = _sntprintf(string_buffer0, _countof(string_buffer0), "RewardItemCode");

		//	SSQInfo::REWARD_WINNER_INFO& rReward = pSSQInfo->m_RewardWinnerInfo;
		//	for(DWORD idx=0 ; idx<SSQInfo::MI_REWARD_WINNER_INFO_SECTION_MAX ; ++idx)
		//	{
		//		string_buffer0[ str0_tail_index   ] = (TCHAR)('1' + idx);
		//		SSQInfo::REWARD_NODE& rNode		= rReward.List[idx];
		//		rNode.m_RewardCode				= GetDataWORD(string_buffer0, nRow);
		//		rNode.m_RewardLimit				= 1;
		//		if(rNode.m_RewardCode)	++rReward.Count;
		//	}
		//};

		{
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			const INT str0_tail_index = _sntprintf(string_buffer0, _countof(string_buffer0), "LimitSkillGroup");

			SSQInfo::RESTRICT_SKILL_GROUP_INFO& rSkill = pSSQInfo->m_RestrictSkillGroupInfo;
			for(DWORD idx=0 ; idx<SSQInfo::MI_RESTRICT_SKILL_SECTION_MAX ; ++idx)
			{
				const BYTE remain		= (BYTE)((idx + 1) % 10);
				const BYTE carry		= (BYTE)((idx + 1) / 10);
				string_buffer0[ str0_tail_index   ] = (TCHAR)('0' + carry);
				string_buffer0[ str0_tail_index+1 ] = (TCHAR)('0' + remain);

				rSkill.SkillGroupCodes[idx]		= GetDataWORD(string_buffer0, nRow);
				if(rSkill.SkillGroupCodes[idx]) ++rSkill.Count;
			}
		}

		{
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			const INT str0_tail_index = _sntprintf(string_buffer0, _countof(string_buffer0), "LimitItemType");

			SSQInfo::RESTRICT_ITEM_TYPE_INFO& rItems = pSSQInfo->m_RestrictItemTypeInfo;
			for(DWORD idx = 0; idx < SSQInfo::MI_RESTRICT_ITEM_SECTION_MAX; ++idx)
			{
				const BYTE remain		= (BYTE)((idx + 1) % 10);
				const BYTE carry		= (BYTE)((idx + 1) / 10);
				string_buffer0[ str0_tail_index   ] = (TCHAR)('0' + carry);
				string_buffer0[ str0_tail_index+1 ] = (TCHAR)('0' + remain);

				rItems.ItemTypes[idx]	= GetDataWORD(string_buffer0, nRow);
				if(rItems.ItemTypes[idx]) ++rItems.Count;
			}
		}

		//	LastSection	value calculate & validate
		_Validate(pSSQInfo);

	}

	return TRUE;
}


//────────────────────────────────────────────────────────────────


BOOLEAN
SSQInfoParser::_Load(const SSQPattern_Info::category&, BOOLEAN bReload)
{
	const INT nRowSize = GetRowSize();
	for(INT nRow=0; nRow<nRowSize ; ++nRow)
	{
        SSQPattern* const pSSQPattern = TAllocNew(SSQPattern_Info::value_type);
		ZeroMemory(pSSQPattern, sizeof(SSQPattern));

		const SSQPattern::INDEX_T PatternIDofSSQ	= GetDataWORD("Index", nRow);

		SSQPATTERN_TABLE::iterator itFind = m_SSQPatternTable.find(PatternIDofSSQ);
		if(itFind == m_SSQPatternTable.end())
			m_SSQPatternTable.insert(std::make_pair(PatternIDofSSQ, pSSQPattern));
		else if(bReload)
		{
			SSQPATTERN_TABLE::value_type& rPair = *itFind;
			SSQPattern* const pSSQPatternTmp = rPair.second;
            TAllocDelete(SSQPattern_Info::value_type, pSSQPatternTmp);
			rPair.second = pSSQPattern;
		}
		else
		{
			FASSERT(!"SSQInfo::Load-Duplicated ID");
			return FALSE;
		}

		pSSQPattern->m_Index = PatternIDofSSQ;

		//SSQ_NAME sTmpName;	<- 이름은 스킵한다.
		//sTmpName.CopyFrom(sr.GetTokenString());
		pSSQPattern->m_BanLiftStep	= GetDataBYTE("BanLiftStep", nRow);

		TCHAR string_buffer0[32];
		TCHAR string_buffer1[32];

		{
			ZeroMemory(string_buffer0, sizeof(string_buffer0));
			ZeroMemory(string_buffer1, sizeof(string_buffer1));
			const INT str0_tail_index = _sntprintf(string_buffer0, _countof(string_buffer0), "PatternStep");
			const INT str1_tail_index = _sntprintf(string_buffer1, _countof(string_buffer1), "Step");
			const INT str2_tail_index = _sntprintf(&string_buffer1[str1_tail_index+1], _countof(string_buffer1), "_ID_") + str1_tail_index + 1;

			SSQPattern::STEP_GROUP& rStepGroup = pSSQPattern->m_StepGroup;
			for(DWORD step_idx = 0; step_idx < SSQPattern::MI_STEP_MAX; ++step_idx)
			{
				string_buffer0[ str0_tail_index ] = (TCHAR)('1' + step_idx);

				SSQPattern::sSINGLE_STEP& rSingle = rStepGroup[step_idx];
				rSingle.m_Step		= GetDataBYTE(string_buffer0, nRow);

				rSingle.m_ListSize = 0;
				for(DWORD obj_idx = 0; obj_idx < SSQPattern::MI_OBJECT_COUNT_MAX; ++obj_idx)
				{
					const BYTE remain	= (BYTE)((obj_idx + 1) % 10);
					const BYTE carry	= (BYTE)((obj_idx + 1) / 10);
					string_buffer1[ str1_tail_index   ] = (TCHAR)('1' + step_idx);
					string_buffer1[ str2_tail_index   ] = (TCHAR)('0' + carry);
					string_buffer1[ str2_tail_index+1 ] = (TCHAR)('0' + remain);

					rSingle.m_List[obj_idx]		= GetDataInt(string_buffer1, nRow);
					if(rSingle.m_List[obj_idx]) ++rSingle.m_ListSize;
				}
			}
		};
		//
	}
	return TRUE;
}


//────────────────────────────────────────────────────────────────
BOOLEAN SSQInfoParser::__ReadHuntingPoint(const DWORD nRow, TCHAR INOUT string_buffer[32],
                                  SSQInfo::HUNTING_POINT_NODE OUT pNODE[eCHAR_TYPE_MAX])
{
	const INT str0_tail_index = (INT)_tcslen(string_buffer);

	for (int idx = eCHAR_BERSERKER; idx < eCHAR_TYPE_MAX; ++idx)
    {
        string_buffer[ str0_tail_index ] = (TCHAR)('0' + idx);	// idx == eCHAR_TYPE
        pNODE[idx].m_HuntingPoint	= GetDataBYTE(string_buffer, nRow);
    }

	return TRUE;
}

BOOLEAN SSQInfoParser::__ReadWinnerRewardInfo(const DWORD nRow, TCHAR INOUT string_buffer[32],
                                      SSQInfo::CHAR_REWARD_NODE& OUT rNODE)
{
	const INT str0_tail_index = (INT)_tcslen(string_buffer);

	for (int idx = eCHAR_BERSERKER; idx < eCHAR_TYPE_MAX; ++idx)
    {
        string_buffer[ str0_tail_index ] = (TCHAR)('0' + idx);	// idx == eCHAR_TYPE
        rNODE[idx].m_RewardCode		= GetDataWORD(string_buffer, nRow);
        rNODE[idx].m_RewardLimit	= 1;
    }

    string_buffer[ str0_tail_index ] = '\0';
	return TRUE;
}
//───────────────────────────────────────────────────────────


BOOLEAN
SSQInfoParser::_Validate(SSQInfo* const pSSQInfo)
{
	//	추가할 내역
	//	- 몬스터 및 아이템 의존도
	//	- 인원 관련
	//	- MIN<MAX
	//	- 입장제어 시간

	//────────────────────────────────────────────
	//	-	MIN<MAX, 인원 관련
	{
		ASSERT(pSSQInfo->m_GamebleInfo.m_ChargeRatio <= 100);	// %단위로 MAX(100)
		SSQInfo::PLAYER_COUNT_LIMIT& rNumLimits = pSSQInfo->m_PlayerCountInfo;
		ASSERT(rNumLimits[SSQInfo::PCL_MIN] && rNumLimits[SSQInfo::PCL_MAX] &&
               rNumLimits[SSQInfo::PCL_MIN] &&
               rNumLimits[SSQInfo::PCL_MIN] < rNumLimits[SSQInfo::PCL_MAX]);
	};
	//────────────────────────────────────────────
	//	-	Monster Integrity
	{
		SSQInfo::SUMMONDED_NPC_INFO& rNpcInfo = pSSQInfo->m_SummonedNpcInfo;
		ASSERT(rNpcInfo.Count);

		NPCInfoParser* const pParser = NPCInfoParser::Instance();
		SSQInfo::SUMMONED_NPC_NODE* pIt = rNpcInfo.List;
		SSQInfo::SUMMONED_NPC_NODE* const pItEnd = &rNpcInfo.List[rNpcInfo.Count];

		BOOLEAN bRatioIntegrity = FALSE;
		DWORD ratio_sum = 0;
		for( ; pIt != pItEnd; ++pIt)
		{
			SSQInfo::SUMMONED_NPC_NODE& rNode = *pIt;
			const BASE_NPCINFO* const pNpcInfo = pParser->GetNPCInfo(rNode.m_NPCCode);
			ASSERT(pNpcInfo && (pNpcInfo->m_byGrade == eNPC_SSQ_MONSTER));	// Ratio는 0일 수 있다...
			if(!bRatioIntegrity) bRatioIntegrity = !!rNode.m_SumRatio;
			rNode.m_SumRatio += (WORD)ratio_sum;	// 증분확률로 전환
			ratio_sum = rNode.m_SumRatio;
		}
		ASSERT(bRatioIntegrity && ratio_sum < 60000);	// 최소한 하나라도 확률이 있어야 한다.
		rNpcInfo.MaxRegenValue = (SSQInfo::RATIO_SUM_T)ratio_sum;
	}
	////────────────────────────────────────────────
	////	-	Item Integrity << - type이 너무 많다. 조사할 수 없다.
	//FLOWCONTROL_START() {
	//	SSQInfo::RESTRICT_ITEM_TYPE_INFO& rInfo = pSSQInfo->m_RestrictItemTypeInfo;
	//	const SSQInfo::COUNT_T upperbound = rInfo.Count;
	//	if(!upperbound)
	//		break;
	//	typedef WORD	ITEMTYPE;
	//	const ITEMTYPE* pIt				= rInfo.ItemTypes;
	//	const ITEMTYPE* const pItEnd	= &rInfo.ItemTypes[upperbound];
	//	for(; pIt!=pItEnd ; ++pIt)
	//	{
	//	}
	//} }
	//────────────────────────────────────────────
	//	-	Skill Integrity << - type이 너무 많다. 조사할 수 없다.
	//	-	캐릭터별 스킬 그룹이 될 것이다. 분리할 수는 없는가?

	//────────────────────────────────────────────
	//	-	입장 제어 시간 검사 및 변환
	SSQInfo::TIME_RANGE& INOUT rScriptT = pSSQInfo->m_TimeRange;
	const SSQInfo::MINUTE_T waiting = pSSQInfo->m_WatingTime;
	const SSQInfo::MINUTE_T playing = pSSQInfo->m_QuestPlayTime;

	SSQInfo::TIME_RANGE& rNotice	= pSSQInfo->m_CTimeNotice;
	SSQInfo::TIME_RANGE& rWaiting	= pSSQInfo->m_CTimeWaiting;
	SSQInfo::TIME_RANGE& rBattle	= pSSQInfo->m_CTimeBattle;

    BOOLEAN acceptTimeField[SSQInfo::MI_ENTER_TIME_SECTION_MAX] = { FALSE, };
    for(DWORD idx = 0; idx < SSQInfo::MI_ENTER_TIME_SECTION_MAX; ++idx)
    {
        // +10은 관련 시간 이벤트(공지관련) 시작 시간을 감안한 여유있는 수치, 0'10"
        acceptTimeField[idx] = !!rScriptT[idx].Start;
        if(acceptTimeField[idx])
        {
            ASSERT(10 < rScriptT[idx].Start);
        }
        _ConvertWzTmFmt2AcmTime(rScriptT[idx]);
    }

	//	Acm Time Check
	ASSERT(waiting && (waiting == 7) && playing);	// 현재 시간 이벤트는 waiting시간이 틀리면 꼬인다.
	const WORD acmTime24 = 24 * 60;
	const WORD safe_range = waiting + playing + 10;
	for(DWORD idx = 0; idx < (SSQInfo::MI_ENTER_TIME_SECTION_MAX - 1); ++idx)
	{
        if(acceptTimeField[idx] == FALSE)
            continue;
		const WORD check_time = rScriptT[idx].Start + safe_range;
		ASSERT(check_time < acmTime24);
        if(acceptTimeField[idx + 1])
        {
		    ASSERT(check_time < rScriptT[idx + 1].Start);
        }
	}

    {
        util::AssertCompileTimeVariableTypeEqual(&rScriptT[0], &rNotice[0]);
        util::AssertCompileTimeVariableTypeEqual(&rScriptT[0], &rWaiting[0]);
        util::AssertCompileTimeVariableTypeEqual(&rScriptT[0], &rBattle[0]);
        BOOST_STATIC_ASSERT(_countof(rScriptT) == _countof(rNotice) &&
                            _countof(rScriptT) == _countof(rWaiting) &&
                            _countof(rScriptT) == _countof(rBattle));
        CopyMemory(&rNotice[0], &rScriptT[0], sizeof(rScriptT));
        CopyMemory(&rWaiting[0], &rScriptT[0], sizeof(rScriptT));
        CopyMemory(&rBattle[0], &rScriptT[0], sizeof(rScriptT));
    };


	for(DWORD idx=0 ; idx<SSQInfo::MI_ENTER_TIME_SECTION_MAX ; ++idx)
	{
        if(acceptTimeField[idx] == FALSE)
            continue;

		{	// 스크립트 시작 시간 기준	-5'~ 0'("포탈 생성할 수 있는 시간까지 5분 남았어용~")
			SSQInfo::TimeRange& rNode = rNotice[idx];
			rNode.End = rNode.Start;
			rNode.Start -= SSQInfo::TIME_NOTICE_START;
		};
		{	// 스크립트 시작 시간 기준	 0'~ 7'(waiting) (스크립트 정보: 7')
			SSQInfo::TimeRange& rNode = rWaiting[idx];
			rNode.End = rNode.Start + waiting;
		};
		{	// 시작시간 (스크립트 시작 시간 기준 + 대기 시간)
			//							 7'~22'(playing) (스크립트 정보:15')
			SSQInfo::TimeRange& rNode = rBattle[idx];
			rNode.Start += waiting;
			rNode.End = rNode.Start + playing;
		};
		{
			rScriptT[idx].End = rBattle[idx].End;
		};
	}

	for(DWORD idx = 0; idx < SSQInfo::MI_ENTER_TIME_SECTION_MAX; ++idx)
	{
        if(acceptTimeField[idx] == FALSE)
            continue;
		ASSERT(rScriptT[idx].End + 10 < acmTime24);	// +10은 관련 시간 사후 처리 시간을 감안한 여유있는 수치
	}

	for(DWORD idx = 0; idx < SSQInfo::MI_ENTER_TIME_SECTION_MAX; ++idx)
	{
		_ConvertAcmTime2WzTmFmt(rScriptT[idx]);
		_ConvertAcmTime2WzTmFmt(rNotice[idx]);
		_ConvertAcmTime2WzTmFmt(rWaiting[idx]);
		_ConvertAcmTime2WzTmFmt(rBattle[idx]);
	}
	//────────────────────────────────────────────

	return TRUE;
}


#ifdef _SERVER
BOOLEAN
SSQInfoParser::_LoadWat(const TCHAR* pszWatFilePath, const BOOLEAN isPack)
{
    __UNUSED(isPack);
	CAreaTileHelper* const pLoader = TAllocNew(CAreaTileHelper);
	pLoader->LoadAreaTile(const_cast<TCHAR*>(pszWatFilePath));

	const DWORD totalTiles = pLoader->GetTotalTileCount();
	SAFE_DELETE_ARRAY(m_pSSQTiles);
	m_pSSQTiles = new INT[totalTiles];
	memset(m_pSSQTiles, -1, sizeof(INT)*totalTiles);
	INT* pTileIT = m_pSSQTiles;

	AREATILEINFO_ITERATOR it=pLoader->Begin(), itend=pLoader->End();
	for(; it!=itend ; ++it)
	{
		AREA_TILE_INFO* const pINFO = *it;
		const INT id = pINFO->iAreaID;
		_AREA_TILE_INFO& rDest = m_SSQTileInfoTable[id];
		rDest.iAreaID = id;
		rDest.wNumberOfTiles = pINFO->wNumberOfTiles;
		rDest.piTileIndex = pTileIT;

		INT* pSrcIt = pINFO->piTileIndex;
		INT* pTileEnd = pTileIT+rDest.wNumberOfTiles;
		for(; pTileIT != pTileEnd ; ++pTileIT)
			*pTileIT = *pSrcIt++;
	}

    TAllocDelete(CAreaTileHelper, pLoader);

	return TRUE;
}


BOOLEAN
SSQInfoParser::GM_SpecialControl_OpenStart(const MAPCODE CodeOfSSQ, const SSQInfo::WzTmFmt retape,
                                           BOOLEAN isFastTrack)
{
    SSQINFO_TABLE::iterator itFind = m_SSQInfoTable.find(CodeOfSSQ);
    SSQInfo* const pSSQInfo = itFind != m_SSQInfoTable.end() ? itFind->second : NULL;
    if(!pSSQInfo)
        return FALSE;

    const SSQInfo::MINUTE_T waiting = isFastTrack == 0 ? pSSQInfo->m_WatingTime : 1;
    const SSQInfo::MINUTE_T playing = pSSQInfo->m_QuestPlayTime;

    const WORD acmTime24 = 24 * 60;
    const WORD safe_range = waiting + playing + 20;

    if(retape + safe_range >= acmTime24)
        return FALSE;

    SSQInfo::TIME_RANGE& rNotice	= pSSQInfo->m_CTimeNotice;
    SSQInfo::TIME_RANGE& rWaiting	= pSSQInfo->m_CTimeWaiting;
    SSQInfo::TIME_RANGE& rBattle	= pSSQInfo->m_CTimeBattle;

    ZeroMemory(&rNotice, sizeof(rNotice));
    ZeroMemory(&rWaiting, sizeof(rWaiting));
    ZeroMemory(&rBattle, sizeof(rBattle));

    rNotice[0].Start	= retape;
    rNotice[0].End		= retape + (isFastTrack == 0 ? SSQInfo::TIME_NOTICE_START : 0);

    rWaiting[0].Start	= rNotice[0].End;
    rWaiting[0].End		= rWaiting[0].Start + waiting;

    rBattle[0].Start	= rWaiting[0].End;
    rBattle[0].End		= rBattle[0].Start += playing;

    _ConvertAcmTime2WzTmFmt(rNotice[0]);
    _ConvertAcmTime2WzTmFmt(rWaiting[0]);
    _ConvertAcmTime2WzTmFmt(rBattle[0]);

    return TRUE;
}

#endif



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<IMPLEMENTATION><UTILITY>
//

const SSQInfo* const
SSQInfoParser::FindSSQInfoEnterableLevel(const LEVELTYPE level) const
{
	SSQINFO_TABLE::const_iterator itFind = 
		std::find_if(m_SSQInfoTable.begin(), m_SSQInfoTable.end()
		, boost::bind(&SSQInfo::IsAcceptLevel
			, boost::bind(select2nd<SSQINFO_TABLE::value_type>(), _1)
			, level
			)
		);

	return itFind != m_SSQInfoTable.end() ? itFind->second : NULL;
}

