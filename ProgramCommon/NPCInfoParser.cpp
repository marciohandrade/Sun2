#include "stdafx.h"
#include "./NPCInfoParser.h"

#include <Struct.h>
#include "./MapInfoParser.h"
#include "./SkillInfoParser.h"

// ~09/12/23
// 09/12/23, waverix, code-arrangement
//  10.06.06|waverix|fit in a coding style and changes a data structure related to the BASE_NPCINFO
//                   that unidimentional array has an O(1) time complexity about fixed size base key.

//==================================================================================================

NPCInfoParser* NPCInfoParser::npc_info_parser_static_ = NULL;

NPCInfoParser* NPCInfoParser::CreateInstance()
{
    // (NOTE) thread-unsafe singleton
    npc_info_parser_static_ = new NPCInfoParser;
    return npc_info_parser_static_;
}

void NPCInfoParser::DestroyInstance()
{
    SAFE_DELETE(npc_info_parser_static_);
}

//==================================================================================================
// NOTE: f110520.2L
namespace util {
;
static NpcElementWeakness CalculateNpcElementWeakness(const BASE_NPCINFO* npc_info);

};
//==================================================================================================

NPCInfoParser::NPCInfoParser()
    : loaded_(false),
      extra_npc_table_(NULL),
      extra_npc_info_array_(NULL)
{
    ZeroMemory(&npc_table_, sizeof(npc_table_));
}

NPCInfoParser::~NPCInfoParser()
{
    Unload();
    UnLoadExtra();
}

void NPCInfoParser::Release()
{
    Unload();
    UnLoadExtra();
}

void NPCInfoParser::Init(DWORD unused_arg1, DWORD unused_arg2)
{
    __UNUSED((unused_arg1, unused_arg2));
}

void NPCInfoParser::Init(DWORD unused_arg1, char* pack_file_name, DWORD unused_arg2)
{
    Init(unused_arg1, unused_arg2);
    SetPackFileName(pack_file_name);
}

void NPCInfoParser::Unload()
{
    SetFirst();
    for (const BASE_NPCINFO* npc_info = GetNext(); npc_info; npc_info = GetNext())
    {
#ifdef _NA_007620_20140915_STATE_RESISTS
        if (npc_info->state_resistances_) {
            delete npc_info->state_resistances_;
        }
#endif //_NA_007620_20140915_STATE_RESISTS
    }

    if (npc_table_.seq_total_array_)
    {
        ::free((void*)npc_table_.seq_total_indexes_);
        ::free((void*)npc_table_.seq_total_array_);
        ZeroMemory(&npc_table_, sizeof(npc_table_));
    }
}

void NPCInfoParser::UnLoadExtra()
{
    if (extra_npc_table_ == NULL) {
        return;
    }
    extra_npc_table_->RemoveAll();
    SAFE_DELETE(extra_npc_table_);
    ::free(extra_npc_info_array_);
    extra_npc_info_array_ = NULL;
}

EXTRA_NPCINFO_SET* NPCInfoParser::GetExtraInfoSetFromFieldID(MAPCODE map_code,FIELDID field_id)
{
    extra_npc_info_set_.Clear();
    extra_npc_table_->SetFirst();
    while (EXTRA_NPCINFO* extra_info = extra_npc_table_->GetNext())
    {
        if (extra_info->m_MapCode == map_code && extra_info->m_FieldID == field_id) {
            extra_npc_info_set_.Insert(extra_info);
        }
    }
    return &extra_npc_info_set_;
}

bool NPCInfoParser::_BuildReference(int added_number_of_nodes, bool is_extra_loading)
{
    if (added_number_of_nodes <= 0) {
        return false;
    }
    //
    if (is_extra_loading == false)
    {   // load npc information
        Unload();
        const size_t indexes_chunk_size = sizeof(npc_table_.seq_total_indexes_[0]) *
                                          added_number_of_nodes;
        void* ptr = ::malloc(indexes_chunk_size);
        ZeroMemory(ptr, indexes_chunk_size);
        npc_table_.seq_total_indexes_ = reinterpret_cast<uint16_t*>(ptr);
        //
        const size_t npc_info_chunk_size = sizeof(npc_table_.seq_total_array_[0]) *
                                           added_number_of_nodes;
        ptr = ::malloc(npc_info_chunk_size);
        ZeroMemory(ptr, npc_info_chunk_size);
        npc_table_.seq_total_array_ = reinterpret_cast<BASE_NPCINFO*>(ptr);
        this->SetFirst();
    }
    else
    {   // load extra information
        UnLoadExtra();
        extra_npc_table_ = new util::SolarHashTable<EXTRA_NPCINFO*>;
        extra_npc_table_->Initialize(50);
        const size_t chunk_size = sizeof(extra_npc_info_array_[0]) * added_number_of_nodes;
        extra_npc_info_array_ = reinterpret_cast<EXTRA_NPCINFO*>(::malloc(chunk_size));
        ZeroMemory(extra_npc_info_array_, chunk_size);
    }
    return true;
}


BOOL NPCInfoParser::_LoadExtra(BOOL reloaded)
{
    const int max_rows = GetRowSize();
    if (_BuildReference(max_rows, true) == false) {
        return false;
    }
    //
    static const char __NullUnitID[4] = { '0', '0', '0', '0', };
    const WzID kNullUnitID = StrToWzID(__NullUnitID);
    //
    EXTRA_NPCINFO* extra_npcinfo_it = this->extra_npc_info_array_;
    for (int row = 0; row < max_rows; ++row)
    {
        MAPCODE  MapCode     = GetDataWORD("MapCode", row);
        FIELDID  FieldID     = GetDataInt("FieldID", row);
        BYTE     byNPCTYPE   = GetDataBYTE("NpcType", row);
        CODETYPE MonsterCode = GetDataDWORD("MonsterCode", row);
        EXTRA_NPCINFO* extra_npc_info = extra_npc_table_->GetData(MonsterCode);
        if (extra_npc_info == NULL) 
        {
            extra_npc_info = extra_npcinfo_it++;
            extra_npc_table_->Add(extra_npc_info, MonsterCode);
        }
        else
        {
            if (reloaded == false)
            {
                SUNLOG(eCRITICAL_LOG, "NPCExtraInfo ID(%ld)가 중복됩니다", MonsterCode);
                FASSERT(!"데이터에 오류가 있습니다.");
            }
        }
        //
        extra_npc_info->m_MapCode  = MapCode;
        extra_npc_info->m_FieldID  = FieldID;
        extra_npc_info->m_eNPCTYPE = (eNPC_TYPE)byNPCTYPE;
        extra_npc_info->m_NPCCODE  = MonsterCode;

        extra_npc_info->m_eRotateType   = \
            (EXTRA_NPCINFO::eROTATE_TYPE)GetDataInt("RotateType", row);
        extra_npc_info->m_eDisplayType  = \
            (EXTRA_NPCINFO::eDISPLAY_TYPE)GetDataInt("DisplayType", row);

        extra_npc_info->m_fNPCPos[0] = GetDataFloat("NPCPos_X", row);
        extra_npc_info->m_fNPCPos[1] = GetDataFloat("NPCPos_Y", row);
        extra_npc_info->m_fNPCPos[2] = GetDataFloat("NPCPos_Z", row);

        extra_npc_info->m_fNPCDir[0] = GetDataFloat("NPCDir_X", row);
        extra_npc_info->m_fNPCDir[1] = GetDataFloat("NPCDir_Y", row);
        extra_npc_info->m_fNPCDir[2] = GetDataFloat("NPCDir_Z", row);

        extra_npc_info->m_fNPCDir[0] = extra_npc_info->m_fNPCDir[0] - extra_npc_info->m_fNPCPos[0];
        extra_npc_info->m_fNPCDir[1] = extra_npc_info->m_fNPCDir[1] - extra_npc_info->m_fNPCPos[1];
        extra_npc_info->m_fNPCDir[2] = extra_npc_info->m_fNPCDir[2] - extra_npc_info->m_fNPCPos[2];

        extra_npc_info->m_iMoveType = GetDataInt("MoveType", row);

        extra_npc_info->m_fMovePos[0] = GetDataFloat("MovePos_X", row);
        extra_npc_info->m_fMovePos[1] = GetDataFloat("MovePos_Y", row);
        extra_npc_info->m_fMovePos[2] = GetDataFloat("MovePos_Z", row);

        extra_npc_info->m_fRange      = GetDataFloat("Range", row);
        extra_npc_info->m_dwJobStrID  = GetDataDWORD("JobStrID", row);
        extra_npc_info->m_iNPC_IconID = GetDataInt("NPCIconID", row);
        ;{  //_NA_0_20100222_UNIT_TRIGGERS_REGEN
            // CHANGES: f110118.5L, change WzID reading method
            // to support excel converting format... 6L, difficult, make to easy
            char unit_id_string[sizeof(WzID)];
            ulong id_value = GetDataDWORD("UnitID", row);
            _snprintf(unit_id_string, _countof(unit_id_string), "%04u", id_value);
            WzID wz_id = StrToWzID(unit_id_string);
            extra_npc_info->m_unitID = 0;
            if (kNullUnitID != wz_id) {
                extra_npc_info->m_unitID = wz_id;
            }
        };
    }

    return true;
}

bool NPCInfoParser::_AddNpcInfo(CODETYPE npc_code, BASE_NPCINFO* npc_info)
{
    if (npc_code == 0 || npc_code > USHRT_MAX || npc_info == NULL) {
        __debugbreak();
        return false;
    }
    --npc_code;
    ++npc_table_.number_of_total_npcs_;
    *const_cast<uint16_t*>(npc_table_.seq_total_it_) = static_cast<uint16_t>(npc_code);
    ++npc_table_.seq_total_it_;
    npc_table_.array_[npc_code] = npc_info;
    return true;
}

BOOL NPCInfoParser::_Load(BOOL reloaded)
{
    // (WARNING) this usage case has a problem about an overrun, changes to an array block
    static const char __wzid_nullstream[4] = { '0', '\0', '\0', '\0' };
    const WzID WzID_NullCode = StrToWzID(__wzid_nullstream); 

    const int max_rows = GetRowSize();
    if (_BuildReference(max_rows, false) == false) {
        return false;
    }
    // (WARNING) in building table, don't controlled by SetFirst, GetNext
    //uint16_t* npc_info_index_it = const_cast<uint16_t*>(npc_table_.seq_total_indexes_);
    BASE_NPCINFO* npc_info_data_it = const_cast<BASE_NPCINFO*>(npc_table_.seq_total_array_);
    for (int row = 0; row < max_rows; ++row)
    {
        CODETYPE MonsterCode = GetDataDWORD("MonsterCode", row);

        BASE_NPCINFO* npc_info = this->GetNPCInfo(MonsterCode);
        if (npc_info == NULL) 
        {
            npc_info = npc_info_data_it++;
            //*npc_info_index_it++ = static_cast<uint16_t>(MonsterCode);
            this->_AddNpcInfo(MonsterCode, npc_info);
        }
        else
        {
            if (false == reloaded)
            {
                SUNLOG(eCRITICAL_LOG, "NPCInfo ID(%ld)가 중복됩니다", MonsterCode);
                FASSERT(!"데이터에 오류가 있습니다.");
            }
        }

        static CODETYPE testCode = MonsterCode;

        npc_info->m_MonsterCode = static_cast<MONSTERCODE>(MonsterCode);
        strncpy(npc_info->m_pszName, GetDataString("Name", row), _countof(npc_info->m_pszName));
        npc_info->m_pszName[_countof(npc_info->m_pszName) - 1] = '\0';
#ifdef _DH_NPC_GROUP_NAME
        npc_info->group_name = GetDataWORD("GName", row);
#endif//_DH_NPC_GROUP_NAME
        npc_info->m_Level        = GetDataWORD("Level", row);
        npc_info->m_DisplayLevel = GetDataWORD("DisplayLevel", row);
        npc_info->m_dwMaxHP      = GetDataDWORD("MaxHP", row);
        npc_info->m_dwMaxMP      = GetDataDWORD("MaxMP", row);
        npc_info->m_dwMaxSD      = GetDataDWORD("MaxSD", row);
        npc_info->m_NCode   = GetDataDWORD("NCode", row);
        npc_info->m_dwICode = GetDataDWORD("ICode", row);
        npc_info->m_wNpcImageIndex = GetDataWORD("NPCImageIndex", row);

        npc_info->m_byGrade   = GetDataBYTE("Grade", row);
        npc_info->m_fGradeExpRatio = GetDataFloat("GradeExpRatio", row); // 2006.3.24 추가
        npc_info->m_wSize     = GetDataWORD("Size", row);
        npc_info->m_wAICode   = GetDataWORD("AICode", row);
        npc_info->m_byAttType = GetDataBYTE("AttType", row);

        npc_info->m_byCriticalRatio  = GetDataBYTE("CriticalRatio", row);
        npc_info->m_dwMinAttackPower = GetDataDWORD("MinAttackPower", row);
        npc_info->m_dwMaxAttackPower = GetDataDWORD("MaxAttackPower", row);
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        npc_info->skill_power = GetDataDWORD("SkillPower", row);
    #endif
        npc_info->m_dwPhyDef     = GetDataDWORD("PhyDef", row);
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        npc_info->m_dwMagicDef   = GetDataDWORD("MagicDef", row);
    #endif
        npc_info->m_wClass       = GetDataWORD("Class", row);
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        npc_info->element_fire_attack = GetDataWORD("FireAttack", row);
        npc_info->element_water_attack = GetDataWORD("WaterAttack", row);
        npc_info->element_wind_attack = GetDataWORD("WindAttack", row);
        npc_info->element_earth_attack = GetDataWORD("EarthAttack", row);
        npc_info->element_dark_attack = GetDataWORD("DarkAttack", row);
        // script ranges = [0, 2.2]
        // reduction ratio against AttackPower
        // Resist = 1.0, Conversion Resist = 0%
        //  AttackPower = 100, Damage = 100
        // Resist = 2.0, Conversion Resist = 100%
        //  AttackPower = 100, Damage = 0
        // Resist = 0.0, Conversion Resist = -100%
        //  AttackPower = 100, Damage = 200
        npc_info->element_fire_resist = static_cast<int16_t>(
            util::LL_Round(GetDataNumber("FireResist", row) * 100.0f - 100.0f));
        npc_info->element_water_resist = static_cast<int16_t>(
            util::LL_Round(GetDataNumber("WaterResist", row) * 100.0f - 100.0f));
        npc_info->element_wind_resist  = static_cast<int16_t>(
            util::LL_Round(GetDataNumber("WindResist", row) * 100.0f - 100.0f));
        npc_info->element_earth_resist = static_cast<int16_t>(
            util::LL_Round(GetDataNumber("EarthResist", row) * 100.0f - 100.0f));
        npc_info->element_dark_resist = static_cast<int16_t>(
            util::LL_Round(GetDataNumber("DarkResist", row) * 100.0f - 100.0f));
        // CHANGES: f110520.2L, the element weakness of the npc based on the user view calculate here.
        npc_info->element_weakness = util::CalculateNpcElementWeakness(npc_info);
        //
        npc_info->m_wPhyAttRate = static_cast<uint16_t>(
            util::LL_Round(GetDataNumber("PhyAttRate", row) * 100.0f));
        npc_info->m_wPhyAttAvoid = static_cast<uint16_t>(
            util::LL_Round(GetDataNumber("PhyAttAvoid", row) * 100.0f));
    #else
        npc_info->element_fire_resist  = GetDataWORD("FireResist", row);
        npc_info->element_water_resist = GetDataWORD("WaterResist", row);
        npc_info->element_wind_resist  = GetDataWORD("WindResist", row);
        npc_info->element_earth_resist = GetDataWORD("EarthResist", row);
        npc_info->m_wPhyAttRate  = GetDataWORD("PhyAttRate", row);
        npc_info->m_wPhyAttAvoid = GetDataWORD("PhyAttAvoid", row);
    #endif
        npc_info->m_byMeleeType  = GetDataBYTE("MeleeType", row);
        npc_info->m_byArmorType  = GetDataBYTE("ArmorType", row);
        npc_info->m_fBodyRadius  = GetDataFloat("BodyRadius", row); // 2006.4.12 추가
        npc_info->m_fAttRange    = GetDataFloat("AttRange", row);
        npc_info->m_fViewRange   = GetDataFloat("ViewRange", row);
        npc_info->m_fMoveRange   = GetDataFloat("MoveRange", row);

        npc_info->m_fWalkSpeed  = GetDataFloat("WalkSpeed", row);
        npc_info->m_fRunSpeed   = GetDataFloat("RunSpeed", row);
        npc_info->m_wAttSpeed   = GetDataWORD("AttSpeed", row);
        npc_info->m_wAttSpeed2  = GetDataWORD("AttSpeed2", row);
        npc_info->m_wProjectileCode  = GetDataWORD("ProjectileCode", row);
        npc_info->m_wProjectileCode2 = GetDataWORD("ProjectileCode2", row);
        npc_info->m_byJumpRatio = GetDataBYTE("JumpRatio", row);

        //_YJ_ADD_DAMAGE_EFFECT_NPCINFO
        TCHAR szDamageEffectID[4] = {0,};
        strncpy(szDamageEffectID, GetDataString("AttackEffectCode", row), 4);
        DWORD dwDamageEffect = StrToWzID(szDamageEffectID);
        if (WzID_NullCode != dwDamageEffect) {
            npc_info->m_wiDamageEffectID = dwDamageEffect;
        }
        else {
            npc_info->m_wiDamageEffectID = INVALID_WZID;
        }
        //
        TCHAR szCDamageEffectID[4] = {0,};
        strncpy(szCDamageEffectID, GetDataString("AttackCriticalEffectCode", row), 4);
        DWORD dwCDamageEffect = StrToWzID(szCDamageEffectID);
        if (WzID_NullCode != dwCDamageEffect) {
            npc_info->m_wiCDamageEffectID = dwCDamageEffect;
        }
        else {
            npc_info->m_wiCDamageEffectID = INVALID_WZID;
        }
    
        TCHAR szSpawnEffectID[4] = {0,};
        strncpy(szSpawnEffectID, GetDataString("SpawnEffectCode", row), 4);
        DWORD dwEffect = StrToWzID(szSpawnEffectID);
        if (WzID_NullCode != dwEffect) {
            npc_info->m_dwSpawnEffectCode = dwEffect;
        }
        else {
            npc_info->m_dwSpawnEffectCode = INVALID_WZID;
        }
    
        strncpy(npc_info->m_wzidSpawnAniCode.code, GetDataString("SpawnAniCode", row), 4);
        npc_info->m_dwSpawnTime    = GetDataDWORD("SpawnAniTime", row);
        npc_info->m_dwDeadWaitingTime = GetDataDWORD("DeadWaitingTime", row);
        npc_info->m_byAttitude = GetDataBYTE("Attitude", row);
        npc_info->m_byMoveAttitude = GetDataBYTE("MoveAttitude", row);
        TCHAR szLocationID[MAX_AREA_ID_LENGTH];
        strncpy(szLocationID, GetDataString("MoveAreaID", row), MAX_AREA_ID_LENGTH);
        npc_info->m_dwMoveAreaID = StrToWzID(szLocationID);

        // 특수행동
        TCHAR szTemp[MAX_FIELD_NAME_SIZE];
        for (int i = 0; i < BASE_NPCINFO::MAX_SPECIAL_NUM; ++i)
        {
            _snprintf(szTemp, _countof(szTemp), "MonsterCondition%d\0", i+1);
            npc_info->m_SpecialCondition[i].byCondition = GetDataBYTE(szTemp, row);
            _snprintf(szTemp, _countof(szTemp), "MonsterConditionParam%d\0", i+1);
            npc_info->m_SpecialCondition[i].wConditionParam = GetDataWORD(szTemp, row);
            _snprintf(szTemp, _countof(szTemp), "ActionType%d\0", i+1);
            npc_info->m_SpecialCondition[i].wActionType = GetDataWORD(szTemp, row);
            _snprintf(szTemp, _countof(szTemp), "ActionParam%d\0", i+1);
            npc_info->m_SpecialCondition[i].wActionParam = GetDataWORD(szTemp, row);
            _snprintf(szTemp, _countof(szTemp), "ActionRate%d\0", i+1);
            npc_info->m_SpecialCondition[i].byActionRate = GetDataBYTE(szTemp, row);
        }

        // 상태저항
        for (int i = 0; i < BASE_NPCINFO::MAX_RESISTANCE_NUM; ++i)
        {
            _snprintf(szTemp, _countof(szTemp), "StateCode%d\0", i+1);
            npc_info->m_ResistanceCondition[i].m_wStateCode = GetDataWORD(szTemp, row);
            _snprintf(szTemp, _countof(szTemp), "ConditionRatio%d\0", i+1);
            npc_info->m_ResistanceCondition[i].m_byRatio = GetDataBYTE(szTemp, row);
            _snprintf(szTemp, _countof(szTemp), "ConditionEffectCode%d\0", i+1);
            strncpy(npc_info->m_ResistanceCondition[i].m_wzidEffectCode.code,
                    GetDataString(szTemp, row), 4);
        }
        npc_info->m_byChangeTargetRatio = GetDataBYTE("ChangeTargetRatio", row);

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        //SkillUpdateTime의 랜덤성을 결정
        npc_info->skill_update_condition = GetDataBYTE("SkillUpdateCondition", row);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
        // 스킬조건을 체크하는 주기시간
        npc_info->m_wSkillUpdateTime = GetDataWORD("SkillUpdateTime", row);
        // 부활스킬
        npc_info->m_byReviveCondition = GetDataBYTE("ReviveCondition", row);
        npc_info->m_wReviveConditionParam = GetDataWORD("ReviveConditionParam", row);
        npc_info->m_byReviveRate = GetDataBYTE("ReviveRate", row);
        npc_info->m_wReviveCode = GetDataWORD("ReviveCode", row);
        npc_info->m_wReviveDelay = GetDataWORD("ReviveDelay", row);
        // 힐 스킬
        npc_info->m_byHealCondition = GetDataBYTE("HealCondition", row);
        npc_info->m_wHealConditionParam = GetDataWORD("HealConditionParam", row);
        npc_info->m_byHealRate = GetDataBYTE("HealRate", row);
        npc_info->m_wHealCode = GetDataWORD("HealCode", row);
        npc_info->m_wHealDelay = GetDataWORD("HealDelay", row);

        // 소환 스킬
        npc_info->m_bySummonCondition = GetDataBYTE("SummonCondition", row);
        npc_info->m_wSummonConditionParam = GetDataWORD("SummonConditionParam", row);
        npc_info->m_bySummonRate = GetDataBYTE("SummonRate", row);
        npc_info->m_wSummonCode= GetDataWORD("SummonCode", row);
        npc_info->m_wSummonDelay = GetDataWORD("SummonDelay", row);

        // 기타 스킬
        npc_info->m_bySkillUsePossible = GetDataBYTE("SkillUsePossible", row);
        // (CHANGES) (f100709.2L)
        BOOST_STATIC_ASSERT(BASE_NPCINFO::MAX_SKILL_NUM == _countof(npc_info->m_wSkillCode) &&
                            BASE_NPCINFO::MAX_SKILL_NUM == _countof(npc_info->m_bySkillRate) &&
                            BASE_NPCINFO::MAX_SKILL_NUM == _countof(npc_info->m_wSkillDelay));
        static const char* kSkillCode_string[_countof(npc_info->m_wSkillCode)] = {
            "SkillCode1", "SkillCode2", "SkillCode3", "SkillCode4", "SkillCode5",
            "SkillCode6", "SkillCode7", "SkillCode8", "SkillCode9", "SkillCode10"
        };
        static const char* kSkillRate_string[_countof(npc_info->m_bySkillRate)] = {
            "SkillRate1", "SkillRate2", "SkillRate3", "SkillRate4", "SkillRate5",
            "SkillRate6", "SkillRate7", "SkillRate8", "SkillRate9", "SkillRate10"
        };
        static const char* kSkillDelay_string[_countof(npc_info->m_wSkillDelay)] = {
            "SkillDelay1", "SkillDelay2", "SkillDelay3", "SkillDelay4", "SkillDelay5",
            "SkillDelay6", "SkillDelay7", "SkillDelay8", "SkillDelay9", "SkillDelay10"
        };
        for (int i = 0; i < _countof(npc_info->m_wSkillCode); ++i)
        {
            npc_info->m_wSkillCode[i] = GetDataWORD(kSkillCode_string[i], row);
            npc_info->m_bySkillRate[i] = GetDataBYTE(kSkillRate_string[i], row);
            npc_info->m_wSkillDelay[i] = GetDataWORD(kSkillDelay_string[i], row);
        }
        //
        npc_info->m_wHateSkill    = GetDataWORD("HateSkill", row);
        npc_info->m_wRevengeSkill = GetDataWORD("RevengeSkill", row);
        npc_info->m_byRevengeRate = GetDataBYTE("RevengeRate", row);

        DWORD dwDropOwnerShip = GetDataDWORD("DropOwnership", row);
        npc_info->m_bRegion = (GetDataDWORD("Region", row) != false);
        // (CHANGES) (f100604.3L) fix up a compatible error on the vc++ 1600
        // pseudo : set   = { (dwDropOwnerShip & (1 << n)) != 0 }
        //          unset = { (dwDropOwnerShip & (1 << n)) == 0 }
        //std::bitset<BASE_NPCINFO::MAX_DROPRATIO_INDEX_NUM> bitOwnerShip(dwDropOwnerShip);
        // (CHANGES) (f100709.2L)
        const int kDropRatioIndexNum = _countof(npc_info->m_bOwnerShipForDropIndex);
        BOOST_STATIC_ASSERT(_countof(npc_info->m_bOwnerShipForDropIndex) ==
                            _countof(npc_info->m_dwDropIndex) &&
                            _countof(npc_info->m_bOwnerShipForDropIndex) ==
                            _countof(npc_info->m_dwFieldDropIndex) &&
                            _countof(npc_info->m_bOwnerShipForDropIndex) ==
                            _countof(npc_info->m_dwBattleDropIndex));
        static const char* kDropIndex_strings[kDropRatioIndexNum] = {
            "DropIndex1", "DropIndex2", "DropIndex3", "DropIndex4", "DropIndex5",
            "DropIndex6", "DropIndex7", "DropIndex8", "DropIndex9", "DropIndex10"
        };
        static const char* kFieldDropIndex_strings[kDropRatioIndexNum] = {
            "FieldDropIndex1", "FieldDropIndex2", "FieldDropIndex3",
            "FieldDropIndex4", "FieldDropIndex5", "FieldDropIndex6",
            "FieldDropIndex7", "FieldDropIndex8", "FieldDropIndex9", "FieldDropIndex10"
        };
        static const char* kBattleDropIndex_strings[kDropRatioIndexNum] = {
            "BattleDropIndex1", "BattleDropIndex2", "BattleDropIndex3",
            "BattleDropIndex4", "BattleDropIndex5", "BattleDropIndex6",
            "BattleDropIndex7", "BattleDropIndex8", "BattleDropIndex9", "BattleDropIndex10"
        };

        for (int i = 0; i < _countof(npc_info->m_dwDropIndex); ++i)
        {   //                                   (f100604.3L)
            npc_info->m_bOwnerShipForDropIndex[i] = ((dwDropOwnerShip & (1 << i)) != 0);
            npc_info->m_dwDropIndex[i] = GetDataDWORD(kDropIndex_strings[i], row);
            //필드존에 있는 몬스터 드랍
            npc_info->m_dwFieldDropIndex[i] = GetDataDWORD(kFieldDropIndex_strings[i], row);
            //배틀존에 있는 몬스터 드랍
            npc_info->m_dwBattleDropIndex[i] = GetDataDWORD(kBattleDropIndex_strings[i], row);
        }
        //
        for (DWORD idx=0 ; idx < BASE_NPCINFO::MAX_QUEST_ITEM_DROP_NUM; ++idx)
        {
            _snprintf(szTemp, _countof(szTemp), "QuestCode%d\0", idx+1);
            npc_info->m_QuestCode[idx] = GetDataWORD(szTemp, row);
            _snprintf(szTemp, _countof(szTemp), "QuestDropIndex%d\0", idx+1);
            npc_info->m_dwQuestDropIndex[idx] = GetDataDWORD(szTemp, row);
        }
        // 'm_wACCode' changes to smalltype after the ep2 AC system
        npc_info->m_wACCode = GetDataBYTE("ACCode", row);
        npc_info->m_byACReferenceID = GetDataBYTE("ACReferenceID", row);
        //
        for (int i = 0; i < 2; ++i)
        {
            _snprintf(szTemp, _countof(szTemp), "OptionID%d\0", i+1);
            npc_info->m_dwOptionID[i] = GetDataDWORD(szTemp, row);
            _snprintf(szTemp, _countof(szTemp), "OptionParam%d\0", i+1);
            npc_info->m_dwOptionParam[i] = GetDataDWORD(szTemp, row);
        }
        npc_info->m_wDest = GetDataWORD("DEST", row);
        for (int i = 0; i < 5; ++i)
        {
            _snprintf(szTemp, _countof(szTemp), "ACT%d\0", i+1);
            npc_info->m_wAct[i] = GetDataWORD(szTemp, row);
            _snprintf(szTemp, _countof(szTemp), "P%d\0", i+1);
            npc_info->m_wP[i] = GetDataWORD(szTemp, row);
        }
        testCode = MonsterCode;

#ifdef _NA_20100307_BETAKEY_SYSTEM
        npc_info->m_BeytaKey        = GetDataInt("BetaKeyCode", row);
#endif
#ifdef _NA_007620_20140915_STATE_RESISTS
        npc_info->state_resistances_ = NULL;
        CHAR state_resists[2048] = {0,};
        const char* char_state_regists = GetDataString("StateRegists", row);
        strncpy(state_resists, char_state_regists, sizeof(state_resists));
        if (state_resists[0] != '0')
        {
            npc_info->state_resistances_ = new STLX_MAP<WORD, RESISTANCE_CONDITION>();
            //char* state_resists = "1200,1100%80,1000";
            std::istringstream iss(state_resists);
            std::string token;
            while(std::getline(iss, token, '.'))
            {
                //std::cout << token << std::endl;
                std::istringstream iss2(token);
                std::string token_state_id;
                std::string token_ratio;
                WORD state_regist_code = 0;
                WORD state_regist_ratio = 100;

                if (std::getline(iss2, token_state_id, '%'))
                    state_regist_code = atoi(token_state_id.c_str());
                //std::cout << token_state_id << std::endl;

                if (std::getline(iss2, token_ratio, ' '))
                    state_regist_ratio = atoi(token_ratio.c_str());
                //std::cout << token_ratio << std::endl;

                RESISTANCE_CONDITION resist_state;
                resist_state.m_wStateCode = state_regist_code;
                resist_state.m_byRatio    = state_regist_ratio;
                npc_info->state_resistances_->insert( \
                    STLX_MAP<WORD, RESISTANCE_CONDITION>::value_type( \
                        state_regist_code, resist_state));
            }
        }
#endif //_NA_007620_20140915_STATE_RESISTS

        // 12096 버전에서 주석처리 됨, 이유 알 수 없음. 
        // 유효성 검증은 필요한 부분이니 풀어 두고, 이후 발생 문제를 해결 해야함.
        //  [5/19/2010 lst1024]

        // 유효성을 검증한다.
        CheckValidity(npc_info);
    }

    return true;
}

BOOL NPCInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch (script_code)
    {
    case SCRIPT_NPCINFO:
        return _Load(reloaded);
    case SCRIPT_NPCEXTRAINFO:
        return _LoadExtra(reloaded);
    }
    return false;
}

BOOL NPCInfoParser::CheckValidity(BASE_NPCINFO* const npc_info)
{
    BOOL success = true;
    //
    if (npc_info->m_byGrade == eNPC_MERCHANT_NPC) {
        return true;
    }
    const MONSTERCODE npc_code = npc_info->m_MonsterCode;
    const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();
    const SkillScriptInfo* base_skill_info = NULL;
    // NPC 스킬코드 유효성 체크
    ;{
        int ratioSum = 0;
        int count = 0;
        for (int i = 0; i < npc_info->MAX_SKILL_NUM; ++i)
        {
            const SLOTCODE skill_code = npc_info->m_wSkillCode[i];
            if (skill_code == 0) {
                continue;
            }
            base_skill_info = skill_parser->GetSkillInfo(skill_code);
            if (base_skill_info == NULL)
            {
                SUNLOG(eCRITICAL_LOG,
                       _T("|[%s]|the monster(%d)'s m_wSkillCode(%d) can't find a skill table|"),
                       __FUNCTION__, npc_code, skill_code);
                npc_info->m_wSkillCode[i] = 0;
                success = false;
            }
            else if (npc_info->m_bySkillRate == 0)
            {
                SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=Invalid skill configuration")
                       _T("|Npc=%d, %dth={SkillCode=%d, SkillRate=0}")
                       _T("|Expected=SkillRate should not be zero|"),
                       __FUNCTION__, npc_code, i+1, skill_code);
            }
            ratioSum += npc_info->m_bySkillRate[i];
            ++count;
        }

        if (count > 0 && ratioSum == 0)
        {
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|the monster(%d)'s m_bySkillRate is 0|"),
                   __FUNCTION__, npc_code);
            success = false;
        }
    };
    //
    if (npc_info->m_wReviveCode)
    {
        base_skill_info = skill_parser->GetSkillInfo(npc_info->m_wReviveCode);
        if (base_skill_info == NULL)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|[%s]|the monster(%d)'s m_wReviveCode(%d) can't find in a skill table|"),
                   __FUNCTION__, npc_code, npc_info->m_wReviveCode);
            npc_info->m_wReviveCode = 0;
            success = false;
        }
    };
    //
    if (npc_info->m_wHealCode)
    {
        base_skill_info = skill_parser->GetSkillInfo(npc_info->m_wHealCode);
        if (base_skill_info == NULL)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|[%s]|the monster(%d)'s m_wHealCode(%d) can't find in a skill table|"),
                   __FUNCTION__, npc_code, npc_info->m_wHealCode);
            npc_info->m_wHealCode = 0;
            success = false;
        }
    };
    //
    if (npc_info->m_wSummonCode)
    {
        base_skill_info = skill_parser->GetSkillInfo(npc_info->m_wSummonCode);
        if (base_skill_info == NULL) {
            SUNLOG(eCRITICAL_LOG,
                   _T("|[%s]|the monster(%d)'s m_wSummonCode(%d) can't find in a skill table|"),
                   __FUNCTION__, npc_code, npc_info->m_wSummonCode);
            npc_info->m_wSummonCode = 0;
            success = false;
        }
    };
    //----------------------------------------------------------------------------------------------
    // 스킬 업데이트 타임 유효성 체크
    if (npc_info->m_wSkillUpdateTime == 0)
    {
        bool need_skill_update_time = false;
        for (int i = 0; i < npc_info->MAX_SKILL_NUM; ++i)
        {
            if (npc_info->m_wSkillCode[i])
            {
                need_skill_update_time = true;
                break;
            }
        }

        if (npc_info->m_wReviveCode || npc_info->m_wHealCode || npc_info->m_wSummonCode)
        {
            need_skill_update_time = true;
        }

        for (int i = 0; i < npc_info->MAX_SPECIAL_NUM; ++i)
        {
            if (npc_info->m_SpecialCondition[i].wActionType) {
                need_skill_update_time = true;
                break;
            }
        }

        for (int i = 0; i < BASE_NPCINFO::MAX_RESISTANCE_NUM; ++i)
        {
            if (npc_info->m_ResistanceCondition[i].m_wStateCode) {
                need_skill_update_time = true;
                break;
            }
        }

        if (need_skill_update_time)
        {
            //스킬입력시스킬사용주기시간값입력필요
            SUNLOG(eCRITICAL_LOG,
                   _T("|[%s]|the monster(%d)'s m_wSkillUpdateTime is 0!|"),
                   __FUNCTION__, npc_code);
            npc_info->m_wSkillUpdateTime = 2000;
            success = false;
        }
    }
    else if (npc_info->m_wSkillUpdateTime < 1000)
    {
        SUNLOG(eCRITICAL_LOG,
               "|[%s]|the monster(%d)'s m_wSkillUpdateTime(%d) is too short!|"
               "|Min value is 1000|",
               __FUNCTION__, npc_code, npc_info->m_wSkillUpdateTime);
        npc_info->m_wSkillUpdateTime = 1000;
        success = false;
    }

    // HP, MP 체크
    if (!npc_info->m_dwMaxHP || !npc_info->m_dwMaxMP)
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|the monster(%d)'s MaxHP or MaxMP is 0!|"),
               __FUNCTION__, npc_code);
        npc_info->m_dwMaxHP = 100;
        success = false;
    }

    // 주석처리 [6/15/2010 lst1024]
    // 경험치를 증가 하기 위한 값이나, 기획에서 0값을 사용 경험치 없는 몬스터를 사용.

    // 경험치 증가 획득 비율
//     if (npc_info->m_fGradeExpRatio < 1)
//     {
//         SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] m_fGradeExpRatio[%f] is under 1.0!",
//                npc_code, npc_info->m_fGradeExpRatio);
//         npc_info->m_fGradeExpRatio = 1.0f;
//         success = false;
//     }

    // CHANGES: f101223.2L, fixed an invalid usage, use m_byAttitude, not m_byAttType
    // 공격타입
    if (!npc_info->m_byAttitude)
    {
        SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] m_byAttType is 0!", npc_code);
        npc_info->m_byAttitude = 1;
        success = false;
    }

    if (npc_info->m_byMoveAttitude == eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK &&
        npc_info->m_byAttitude != ATTACK_ATTITUDE_NO_ATTACK)
    {
        SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] m_byMoveAttitude is 7  m_byAttType not 7", npc_code);
        npc_info->m_byAttitude = ATTACK_ATTITUDE_NO_ATTACK;
        success = false;
    }
    // UNDONE: check related codes
    if (npc_info->m_byMeleeType == eMELEE_TYPE_NOT && 
        npc_info->m_byAttitude != ATTACK_ATTITUDE_NO_ATTACK)
    {
        SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] m_byAttType not 7 m_byMeleeType is 0", npc_code);
        success = false;        
    }

    if (npc_info->m_byMeleeType >= eMELEE_TYPE_MAX)
    {
        SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] Invalid m_byMeleeType[%d]!",
               npc_code, npc_info->m_byMeleeType);
        npc_info->m_byMeleeType = eMELEE_TYPE_NOT;
        success = false;
    }

    // 크리티컬 확률
    if (npc_info->m_byCriticalRatio >= 100)
    {
        SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] m_byCriticalRatio[%d] is very great!",
               npc_code, npc_info->m_byCriticalRatio);
        success = false;
    }

    // 최소, 최대 데미지
    if (npc_info->m_dwMinAttackPower >= 100000 || npc_info->m_dwMaxAttackPower >= 100000)
    {
        SUNLOG(eCRITICAL_LOG,
               "[NPCInfoParser] MonsterCode[%d] m_dwMinAttackPower[%d] m_dwMaxAttackPower[%d] is "
               "very great!",
            npc_code, npc_info->m_dwMinAttackPower, npc_info->m_dwMaxAttackPower);
        success = false;
    }

    if (npc_info->m_dwMinAttackPower > npc_info->m_dwMaxAttackPower)
    {
        SUNLOG(eCRITICAL_LOG,
               "[NPCInfoParser] MonsterCode[%d] m_dwMinAttackPower[%d] is "
               "more great than m_dwMaxAttackPower[%d]!",
               npc_code, npc_info->m_dwMinAttackPower, npc_info->m_dwMaxAttackPower);
        npc_info->m_dwMinAttackPower = npc_info->m_dwMaxAttackPower;
        success = false;
    }

    // 물리공격 성공률

    if (npc_info->m_wPhyAttRate == 0 || npc_info->m_wPhyAttAvoid == 0)
    {
        // 특정 상황에서 이용되지는 않지만, 로직상 문제가 발생할 수 있으므로 로그만 띄우지 않는다.
        if (npc_info->m_byAttitude != ATTACK_ATTITUDE_NO_ATTACK &&
            npc_info->m_byMoveAttitude != eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK)
        {
            SUNLOG(eCRITICAL_LOG,
                "[NPCInfoParser] MonsterCode[%d] m_wPhyAttRate or m_wPhyAttAvoid is 0!", npc_code);
        }

        if (!npc_info->m_wPhyAttRate)    npc_info->m_wPhyAttRate = 1;
        if (!npc_info->m_wPhyAttAvoid)   npc_info->m_wPhyAttAvoid = 1;
        success = false;
    }
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    npc_info->m_byMeleeType = eMELEE_TYPE_SLASH;
    npc_info->m_byArmorType = eARMOR_HARD;
#else //if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    // 공격타입, 아머타입
    if (npc_info->m_byMeleeType >= eMELEE_TYPE_MAX)
    {
        SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] Invalid m_byMeleeType[%d]!",
               npc_code, npc_info->m_byMeleeType);
        npc_info->m_byMeleeType = eMELEE_TYPE_SLASH;
        success = false;
    }
    if (npc_info->m_byArmorType >= eARMOR_TYPE_MAX)
    {
        SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] Invalid m_byArmorType[%d]!",
               npc_code, npc_info->m_byArmorType);
        npc_info->m_byArmorType = eARMOR_HARD;
        success = false;
    }
#endif
    // 몬스터 바디 사이즈
    if (npc_info->m_fBodyRadius > 3.0f)
    {
        SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] m_fBodyRadius[%f] is very great!",
               npc_code, npc_info->m_fBodyRadius);
        success = false;
    }


    // 공격 사거리
    if (npc_info->m_fAttRange == 0 || npc_info->m_fAttRange > 50.f)
    {
        if (npc_info->m_byAttitude != ATTACK_ATTITUDE_NO_ATTACK &&
            npc_info->m_byMoveAttitude != eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK)
        {
            SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] Invalid m_fAttRange[%f]!",
                npc_code, npc_info->m_fAttRange);
        }

        if (npc_info->m_fAttRange)
            npc_info->m_fAttRange = 50.f;   // 저탑에서 사용하면 꼬이는 수치이긴 한데...
        else
            npc_info->m_fAttRange = 1.f;
        
        success = false;
    }

    // 시야 범위
    if (npc_info->m_fViewRange == 0 || npc_info->m_fViewRange > 50.f)
    {
        if (npc_info->m_byAttitude != ATTACK_ATTITUDE_NO_ATTACK &&
            npc_info->m_byMoveAttitude != eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK)
        {
            SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] Invalid m_fViewRange[%f]!",
                npc_code, npc_info->m_fViewRange);
        }

        if (npc_info->m_fViewRange)
            npc_info->m_fViewRange = 50.f;  // 저탑에서 사용하면 꼬이는 수치이긴 한데...
        else
            npc_info->m_fViewRange = 1.f;

        success = false;
    }

    // 이동속도
    if (npc_info->m_byMoveAttitude == eMOVE_ATTITUDE_ETERNAL_STOP ||
        npc_info->m_byMoveAttitude == eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK)
    {
        if (npc_info->m_fWalkSpeed != 0 || npc_info->m_fRunSpeed != 0)
        {
            SUNLOG(eCRITICAL_LOG, 
                   "[NPCInfoParser] MonsterCode[%d] MoveAttitude[%d]는 이동할 수 없는 속성이다."
                   "m_fWalkSpeed, m_fRunSpeed는 0 이외의 값을 가질 수 없다.",
                   npc_code, npc_info->m_byMoveAttitude);

            npc_info->m_fWalkSpeed = 0;
            npc_info->m_fRunSpeed = 0;
            success = false;
        }
    }
    else
    {
        if (npc_info->m_fWalkSpeed < 0.2f || npc_info->m_fWalkSpeed > 20.f)
        {
            SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] Invalid m_fWalkSpeed[%f]!",
                   npc_code, npc_info->m_fWalkSpeed);
            if (npc_info->m_fWalkSpeed > 20.f) {
                npc_info->m_fWalkSpeed = 20.f;
            }
            else {
                npc_info->m_fWalkSpeed = 0.2f;
            }
            success = false;
        }

        if (npc_info->m_fRunSpeed < 0.2f || npc_info->m_fRunSpeed > 20.f)
        {
            SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] Invalid m_fRunSpeed[%f]!",
                   npc_code, npc_info->m_fRunSpeed);
            if (npc_info->m_fRunSpeed > 20.f) {
                npc_info->m_fRunSpeed = 20.f;
            }
            else {
                npc_info->m_fRunSpeed = 0.2f;
            }
            success = false;
        }

        if (npc_info->m_fWalkSpeed > npc_info->m_fRunSpeed)
        {
            SUNLOG(eCRITICAL_LOG,
                   "[NPCInfoParser] MonsterCode[%d] m_fWalkSpeed[%f] is"
                   " more great than m_fRunSpeed[%f]!",
                   npc_code, npc_info->m_fWalkSpeed, npc_info->m_fRunSpeed);
            npc_info->m_fWalkSpeed = npc_info->m_fRunSpeed;
            success = false;
        }
    }

    if (npc_info->m_wAttSpeed == 0 || npc_info->m_wAttSpeed2 == 0)
    {
        if (npc_info->m_byAttitude != ATTACK_ATTITUDE_NO_ATTACK &&
            npc_info->m_byMoveAttitude != eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK)
        {
            SUNLOG(eCRITICAL_LOG, 
                "[NPCInfoParser] MonsterCode[%d] m_wAttSpeed or m_wAttSpeed2 is 0!", npc_code);
        }

        if (!npc_info->m_wAttSpeed) 
            npc_info->m_wAttSpeed = 1000;

        if (!npc_info->m_wAttSpeed2)
            npc_info->m_wAttSpeed2 = 1000;

        success = false;
    }

    // 적대성향
    if (npc_info->m_byAttitude >= ATTACK_ATTITUDE_TYPE_MAX)
    {
        SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] Invalid m_byAttitude[%d]!",
               npc_code, npc_info->m_byAttitude);
        npc_info->m_byAttitude = ATTACK_ATTITUDE_NEARES_FIRST;
        success = false;
    }

    // 이동성향
    if (npc_info->m_byMoveAttitude >= eMOVE_ATTITUDE_MAX)
    {
        SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] Invalid m_byMoveAttitude[%d]!",
               npc_code, npc_info->m_byMoveAttitude);
        npc_info->m_byMoveAttitude = eMOVE_ATTITUDE_WANDER;
        success = false;
    }

    // 현재 툴로 찍는 방식으로 사용되지 않는다. 
    // 주석처리 : 리젠이 정리 될때 m_dwMoveAreaID 도 같이 정리 되어야한다. [6/15/2010 lst1024]
//     if (npc_info->m_byMoveAttitude == eMOVE_ATTITUDE_PATROL ||
//         npc_info->m_byMoveAttitude == eMOVE_ATTITUDE_SEARCH_AREA)
//     {
//         if (npc_info->m_dwMoveAreaID == 48)
//         {
//             SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] m_dwMoveAreaID is 0!", npc_code);
//             success = false;
//         }
//     }

    int exist_trans    = -1;
    int exist_selfdest = -1;

    // 특수행동
    for (int i = 0; i < BASE_NPCINFO::MAX_SPECIAL_NUM; ++i)
    {
        if (npc_info->m_SpecialCondition[i].wActionType >= eNPC_SPECIAL_ACTION_MAX)
        {
            SUNLOG(eCRITICAL_LOG,
                   "[NPCInfoParser] MonsterCode[%d] Invalid m_SpecialCondition wActionType[%d]!",
                   npc_code, npc_info->m_SpecialCondition[i].wActionType);
            npc_info->m_SpecialCondition[i].wActionType = 0;
            success = false;
        }

        if (npc_info->m_SpecialCondition[i].byCondition &&
            npc_info->m_SpecialCondition[i].byCondition != 5)
        {
            if (npc_info->m_SpecialCondition[i].wConditionParam == 0 ||
                npc_info->m_SpecialCondition[i].wConditionParam > 100)
            {
                SUNLOG(eCRITICAL_LOG,
                       "[NPCInfoParser] MonsterCode[%d] Invalid m_SpecialCondition wConditionParam[%d]!",
                       npc_code, npc_info->m_SpecialCondition[i].wConditionParam);
                success = false;
            }
        }

        if (npc_info->m_SpecialCondition[i].wActionType == eNPC_SPECIAL_ACTION_TRANSFORMATION) {
            exist_trans = i;
        }
        if (npc_info->m_SpecialCondition[i].wActionType == eNPC_SPECIAL_ACTION_SELP_DESTRUCTION) {
            exist_selfdest = i;
        }
    }

    // _NA_0_20100514_NPC_DEAD_STATE_TIME
    // 죽은 상태에서의 액션 2가지는 같이 존재 할 수 없다.
    if (exist_trans != -1 && 
        exist_selfdest != -1)
    {
        SUNLOG(eCRITICAL_LOG, "[NPCInfoParser] MonsterCode[%d] Invalid SpecialCondition"
               " TRANSFORMATION[%d] && DESTRUCTION[%d]"
               " 같이 존재 할 수 없는 액션이다",
               npc_code, exist_trans, exist_selfdest);
        success = false;
    }
    return success;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
// NOTE: f110520.2L, this logic that is used by _NA_20100322_AGGRO_DISPLAY is migrated from
// sun:r53711 written by i4u4me.
// To complete this logic, loaded the element resist category before calling this function.
static NpcElementWeakness util::CalculateNpcElementWeakness(const BASE_NPCINFO* npc_info)
{
    // NOTE: to add magic value of original regist value is caused by minus value existence
#define COMPOSED_RESIST_RELATION(__resist__, __weakness__) \
    ulong((((__resist__) + 10000) << 16) | (__weakness__))
    NpcElementWeakness weakness = { weakness.eElementWeakness_Default };
    ulong resist_list[weakness.eElementWeakness_Counts] = { 0, };
    //
    resist_list[weakness.eElementWeakness_Fire] = \
        COMPOSED_RESIST_RELATION(npc_info->element_fire_resist, weakness.eElementWeakness_Dark);
    resist_list[weakness.eElementWeakness_Water] = \
        COMPOSED_RESIST_RELATION(npc_info->element_water_resist, weakness.eElementWeakness_Fire);
    resist_list[weakness.eElementWeakness_Wind] = \
        COMPOSED_RESIST_RELATION(npc_info->element_wind_resist, weakness.eElementWeakness_Earth);
    resist_list[weakness.eElementWeakness_Earth] = \
        COMPOSED_RESIST_RELATION(npc_info->element_earth_resist, weakness.eElementWeakness_Water);
    resist_list[weakness.eElementWeakness_Dark] = \
        COMPOSED_RESIST_RELATION(npc_info->element_dark_resist, weakness.eElementWeakness_Wind);
    //
    std::sort(resist_list, &resist_list[weakness.eElementWeakness_Counts]);
    //
    if ((resist_list[0] & 0xFFFF0000) != (resist_list[1] & 0xFFFF0000)) {
        weakness.value = uint8(resist_list[0] & 0x000000FF);
    };
    return weakness;
#undef COMPOSED_RESIST_RELATION
}

