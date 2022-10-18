#pragma once

#include "PacketStruct_CG.h"
#include "PacketControl/WarMessages_Sync.h"

class SkillScriptInfo;

namespace SkillFacade
{
    struct TargetResult
    {
        struct ResultCodeData
        {
            SKILL_RESULT_CODE* m_pResultCode;
            BASE_ABILITYINFO* m_pAbillityInfo;
        };

        SKILL_RESULT_BASE* m_pResult;
        DWORD m_AttackSerial;
        DWORD m_TimeSerial;

        std::vector<ResultCodeData> m_ResultCodeDataArray;

    public:
        DWORD GetTargetKey() { return m_pResult->m_dwTargetKey; }
        BYTE GetSkillEffect() { return m_pResult->m_bySkillEffect; }
        size_t GetResultCount() { return m_ResultCodeDataArray.size(); }
        SKILL_RESULT_CODE* GetResultCode(int index) { return ((size_t)index) >= m_ResultCodeDataArray.size() ? NULL : m_ResultCodeDataArray[index].m_pResultCode; }
        BASE_ABILITYINFO* GetResultAbillityInfo(int index) { return ((size_t)index) >= m_ResultCodeDataArray.size() ? NULL : m_ResultCodeDataArray[index].m_pAbillityInfo; }
    };

    struct EffectResult
    {
        SKILL_RESULT_EFFECT* m_pResult;
    };


    class ResultData
    {
    public:
        ResultData() {}

    public:
        SLOTCODE GetSkillCode() { return (SLOTCODE)GetSkillActionBase()->m_SkillCode; }
        void SetClientSerial( DWORD Serial ) { GetSkillActionBase()->m_dwClientSerial = Serial; }
        DWORD GetPrimaryTargetKey() { return GetSkillActionBase()->m_dwPrimaryTargetKey; }

        DWORD GetAttackerKey() { return GetSkillActionBase()->m_dwAttackerKey; }
        DWORD GetClientSerial() { return GetSkillActionBase()->m_dwClientSerial; }

        virtual SkillActionBase* GetSkillActionBase() PURE;
        virtual int GetTargetResultCount() PURE;
        virtual int GetEffectResultCount() PURE;
        virtual DWORD GetAttackerHP() PURE;
        virtual DWORD GetAttackerMP() PURE;

        SkillScriptInfo* GetSkillScriptInfo() const { return m_pSkillScriptInfo; }

        TargetResult* GetTargetResult( int index ) { return ((size_t)index) >= m_TargetResult.size() ? NULL : &m_TargetResult[index]; }
        EffectResult* GetEffectResult( int index ) { return ((size_t)index) >= m_EffectResult.size() ? NULL : &m_EffectResult[index]; }

        const std::vector< TargetResult >& GetTargetResult() const { return m_TargetResult; }
        const std::vector< EffectResult >& GetEffectResult() const { return m_EffectResult; }

    protected:
        void _ParserCommonResult( BYTE* pMessage );

        std::vector< TargetResult > m_TargetResult;
        std::vector< EffectResult > m_EffectResult;


        SkillScriptInfo* m_pSkillScriptInfo;
    };

    class InstantResultData : public ResultData
    {
    public:
        InstantResultData() {}

        void ParserInstantResult( MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD* pPacket, BYTE* pPacket_Additions );

        DWORD GetAttackerHP() { return m_SkillActionInstantResult.m_dwAttackerHP; }
        DWORD GetAttackerMP() { return m_SkillActionInstantResult.m_dwAttackerMP; }

    private:
        SkillActionInstantResult m_SkillActionInstantResult;

        SkillActionBase* GetSkillActionBase() { return &m_SkillActionInstantResult; }
        int GetTargetResultCount() { return m_SkillActionInstantResult.m_byNumberOfTargets; }
        int GetEffectResultCount() { return m_SkillActionInstantResult.m_byNumberOfFieldEffect; };

    };


    class DelayResultData : public ResultData
    {
    public:
        DelayResultData() {}

    public:
        void ParserDelayResult( MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD* pPacket, BYTE* pPacket_Additions );

        DWORD GetAttackerHP() { return m_SkillActionDelayResult.m_dwAttackerHP; }
        DWORD GetAttackerMP() { return m_SkillActionDelayResult.m_dwAttackerMP; }

    private:
        SkillActionDelayResult m_SkillActionDelayResult;

        SkillActionBase* GetSkillActionBase() { return &m_SkillActionDelayResult; }
        int GetTargetResultCount() { return m_SkillActionDelayResult.m_byNumberOfTargets; }
        int GetEffectResultCount() { return m_SkillActionDelayResult.m_byNumberOfFieldEffect; };

    };
};