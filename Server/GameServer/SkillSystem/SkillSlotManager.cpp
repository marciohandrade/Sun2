#include "stdafx.h"
#include "SkillSlotManager.h"

#include <SkillInfoParser.h>
#include <StyleQuickRegistInfoParser.h>
#include <SkillAttrCalculator.h>
#include <SCSkillSlotContainer.h>
#include <SCQuickStyleSlot.h>
#include <SCQuickStyleSlotContainer.h>

//==================================================================================================

#include "Player.h"
#include "SkillManager.h"
#include "ItemManager.h"
#include "SlotManager.h"
#include "QuickManager.h"
#include "QuickStyleManager.h"
#include "StatusManager.h"
#include "SummonManager.h"
#include "PassiveSkill.h"
#include "SkillFactory.h"
#include "CommonCharacterFormula.h"

#ifdef _NA_001990_ACSYSTEM_ADD
#include "AchievementManager.h"
#endif //_NA_001990_ACSYSTEM_ADD

SkillSlotManager::SkillSlotManager()
{
}

SkillSlotManager::~SkillSlotManager()
{
}

void SkillSlotManager::Init(SlotManager* const slot_manager)
{
    player_ = slot_manager->player_;
    slot_manager_ = slot_manager;
    skill_container_ = \
        static_cast<SCSkillSlotContainer*>(slot_manager->GetSlotContainer(SI_SKILL));
    quick_container_ = \
        static_cast<SCQuickSlotContainer*>(slot_manager->GetSlotContainer(SI_QUICK));
    style_container_ = \
        static_cast<SCQuickStyleSlotContainer*>(slot_manager->GetSlotContainer(SI_STYLE));
    pre_style_code_ = 0;

    passive_skills_.clear();
}

void SkillSlotManager::Release()
{
    // old... (WAVERIX)(080603)(change logic) - decrease instance count
    const SkillFactory* const skill_factory = SkillFactory::Instance();
    FOREACH_CONTAINER(const SKILL_HASH::value_type& node, passive_skills_, SKILL_HASH)
    {
        Skill* const skill = node.second;
        skill->Release();
        skill_factory->FreeSkill(skill);
    }
    passive_skills_.clear();
}

bool SkillSlotManager::IsEmpty(POSTYPE pos)
{
    return (slot_manager_->IsEmpty(SI_SKILL, pos) != false);
}

bool SkillSlotManager::ValidState()
{
    return (slot_manager_->ValidState() != false);
}

// 무기 아이템 교환에 따른 스킬 적용 변경
void SkillSlotManager::AddWeapon(CODETYPE new_weapon_code)
{
    const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(new_weapon_code);
    if (item_info == NULL)    
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't find a weapon (%d)"),
               new_weapon_code);
        return;
    }

    // 패시브 스킬들의 기능이 살아난다.
    UpdatePassiveSkills();

    // 이전 스타일의 무기타입과 삽입하려는 무기의 타입이 같으면 이전 스타일을 복원해 준다.
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    const BASE_STYLEINFO* prev_style_info = skill_parser->GetStyleInfo(pre_style_code_);
    if (prev_style_info && prev_style_info->m_iWeaponDefine == item_info->m_wType)
    {
        SetSelectStyle(pre_style_code_);
    }
    // 이전 스타일이 새로운 무기를 사용할 수 없다면 기본 스타일로 교체한다.
    else
    {
        SetSelectStyle(player_->GetDefaultStyle(new_weapon_code));
    };

    // 퀵 스타일 슬롯의 정보들이 모두 바뀐다.
    UpdateStyleSlots();
}

void SkillSlotManager::DelWeapon()
{
    // 패시브 스킬들의 기능이 해제된다.
    UpdatePassiveSkills();

    // 이전 스타일 정보를 기억한다.
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    SLOTCODE style_code = player_->GetBaseSelectedStyle();
    const BASE_STYLEINFO* base_style_info = skill_parser->GetStyleInfo(style_code);
    if (base_style_info) {
        pre_style_code_ = style_code;
    }

    // 주먹질 스타일로 교체한다.
    StatusManager* const status_manager = player_->GetStatusManager();
    style_code = player_->GetDefaultStyle(0);
    if (status_manager->Status.IsDragonTransforming())
    {
        player_->SetBaseSelectedStyle(style_code);
    }
    else
    {
        SetSelectStyle(style_code);
    }
    // 퀵 스타일 슬롯의 정보들이 모두 바뀐다.
    UpdateStyleSlots();
}

// CHANGES f100825.2L, change a routine to a total serializing skill stream converter
void SkillSlotManager::SerializeSkillStreamTotal(SKILL_TOTAL_INFO* const total_stream,
                                                 eSERIALIZE serialize_type)
{
    const POSTYPE kMaxNumberOfSlots = skill_container_->GetMaxSlotNum();
    BOOST_STATIC_ASSERT(MAX_SKILL_SLOT_NUM == _countof(total_stream->m_Slot));
    //MAX_SKILL_SLOT_NUM == kMaxNumberOfSlots &&
    //

    if (serialize_type == SERIALIZE_LOAD)
    {
        //_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
        // 캐릭터에게 스킬 정보를 보낼때 스킬 트리의 첫행의 스킬을 익히기 위해 호출된다.(Default)
        // 스킬 컨테이너에 스킬이 존재하지 않을 경우만 호출된다.
        UnLockFirstSkillLine();

        POSTYPE pos_index = 0;
        SKILLSLOT* it = total_stream->m_Slot;
        const SKILLSLOT* const end = &total_stream->m_Slot[kMaxNumberOfSlots];
        for ( ; pos_index < kMaxNumberOfSlots && it != end; ++pos_index)
        {
            if (skill_container_->IsEmpty(pos_index)) {
                continue;
            }
            it->m_Pos = pos_index;
            skill_container_->SerializeSkillStream(pos_index, &it->m_Stream, serialize_type);
            ++it;
        };
        //
        total_stream->m_Count = static_cast<POSTYPE>(it - total_stream->m_Slot);
    }
    else if (serialize_type == SERIALIZE_STORE)
    {
        const DWORD cur_max_hp = player_->GetMaxHP();
        const DWORD cur_max_mp = player_->GetMaxMP();
        const DWORD cur_max_sd = player_->GetMaxSD();
        //
        int number_of_stream_slots = total_stream->m_Count;
        if (FlowControl::FCAssert(number_of_stream_slots <= kMaxNumberOfSlots) == false) {
            return;
        }
        //
        SKILLSLOT* it = total_stream->m_Slot;
        const SKILLSLOT* const end = total_stream->m_Slot + number_of_stream_slots;
        for ( ; it != end; ++it)
        {
            const SLOTCODE skill_code = it->m_Stream.Part.wCode;
            WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_logskillcode", skill_code)));
            skill_container_->SerializeSkillStream(it->m_Pos, &it->m_Stream, serialize_type);
            // r100825.2L, regard the complexity of AddAttrForPassiveSkill.
            if (AddAttrForPassiveSkill(skill_code) == false) {
                // register a cool-time information if this skill is not a passive type.
                player_->SetSkillCoolTime(skill_code, SKILL_COOLTIME_MARGIN);
            }
        }
        //
        if (cur_max_hp != player_->GetMaxHP()) {
            player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedHP);
        };
        if (cur_max_mp != player_->GetMaxMP()) {
            player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedMP);
        };
        if (cur_max_sd != player_->GetMaxSD()) {
            player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedSD);
        };
    }
}

// CHANGES f100825.2L, removed. change a routine to a total serializing skill stream converter
/*
// 스킬 슬롯 갱신
void SkillSlotManager::SerializeSkillStream(POSTYPE pos, SKILLSTREAM* IN pStream, eSERIALIZE eType)
{
    skill_container_->SerializeSkillStream(pos, pStream, eType);

    if (eType == SERIALIZE_STORE)
    {
        DWORD maxHP = player_->GetMaxHP();
        DWORD maxMP = player_->GetMaxMP();
        DWORD maxSD = player_->GetMaxSD();

        if (!AddAttrForPassiveSkill(pStream->Part.wCode))
        {
            // 패시브 스킬이 아닌경우 쿨타임 등록
            player_->SetCoolTime(pStream->Part.wCode, SKILL_COOLTIME_MARGIN);
        }
        //
        if (maxHP != player_->GetMaxHP())
        {
            player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedHP);
        };
        if (maxMP != player_->GetMaxMP())
        {
            player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedMP);
        };
        if (maxSD != player_->GetMaxSD())
        {
            player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedSD);
        };
    }
}
*/

//==================================================================================================
/*
// CHANGES, f100826.7L, removed
class QuickSkillLevelUp
{
    BOOL        complete_;
    SLOTCODE    cur_skill_code_;
    SLOTCODE    new_skill_code_;
public:
    QuickSkillLevelUp(SLOTCODE cur_skill_code, SLOTCODE new_skill_code) :
      cur_skill_code_(cur_skill_code), new_skill_code_(new_skill_code), complete_(0) {}

      void operator() (SCSlot* slot)
      {
          SCQuickSlot* quick_slot = static_cast<SCQuickSlot*>(slot);

          if (complete_) {
              return;
          }
          if (quick_slot->GetOrgCode() != cur_skill_code_) {
              return;
          }

          quick_slot->SetOrgCode(new_skill_code_);
          complete_ = true;
      }
};
*/
namespace nsSlot {
;
// CHANGES, f100826.7L, change a functor class to regard a locality.
struct QuickSlotOriginalCodeChanger
{
    QuickSlotOriginalCodeChanger(SCQuickSlotContainer* quick_container,
                                 SLOTCODE from, SLOTCODE to)
    {
        typedef SCQuickSlotContainer::SLOT_LINEAR_LIST SLOT_LINEAR_LIST;
        const SLOT_LINEAR_LIST& slot_list = quick_container->GetSlotList();
        FOREACH_CONTAINER(const SLOT_LINEAR_LIST::value_type& node, slot_list,
                          SLOT_LINEAR_LIST)
        {
            SCQuickSlot* const quick_slot = static_cast<SCQuickSlot*>(node.slot);
            if (quick_slot->IsEmptySlot()) {
                continue;
            };
            if (quick_slot->GetOrgCode() != from) {
                continue;
            }
            quick_slot->SetOrgCode(to);
        }
    }
};

}; //end of namespace

//==================================================================================================

class SearchSkillAllLevelDown
{
    SkillSlotManager* skill_slot_manager_;  
public:
    SearchSkillAllLevelDown(SkillSlotManager* skill_slot_manager)
        : skill_slot_manager_(skill_slot_manager) {} 

    void operator()(SCSlot* slot)
    {
        SCSkillSlot* pSkillSlot = static_cast<SCSkillSlot*>(slot);
        if (skill_slot_manager_ == NULL) {
            return;
        }
        skill_slot_manager_->LevelDownSkillUntilSatisfaction(pSkillSlot->GetCode());
    }
};

//--------------------------------------------------------------------------------------------------
//_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
class SkillLevelDown
{
    SkillSlotManager* skill_slot_manager_;

public:
    SkillLevelDown(SkillSlotManager* skill_slot_manager) 
        : skill_slot_manager_(skill_slot_manager)  
    {
    }

    void operator()(const SCSlot* const slot)
    {
        if (skill_slot_manager_ == NULL)
        {
            return;
        }
        const SCSkillSlot* const skill_slot = static_cast<const SCSkillSlot* const>(slot);
        const SkillScriptInfo* const skill_info = skill_slot->GetSkillInfo();
        if (skill_info != NULL)
        {
            UnlearnSkill(skill_info->m_SkillClassCode);
        }            
    }

    void UnlearnSkill(const SLOTCODE skill_class)
    {
        Player* const player = skill_slot_manager_->player_;
        SCSkillSlotContainer* const skill_container_ = skill_slot_manager_->skill_container_;
        
        const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();
        const SkillJobTree::Desc* const job_desc = 
            skill_parser->FindAllJobDesc(player->GetCharInfo()->m_byClassCode, skill_class);
        if (job_desc == NULL)
        {
            return;
        }
        
        // 현재 존재하는 스킬이어야 한다.
        const SCSkillSlot* skill_slot = skill_container_->GetSkillSlotByClass(skill_class);
        if (skill_slot == NULL)
        {
            return;
        }
        const SkillScriptInfo* const skill_info = skill_slot->GetSkillInfo();
        if (skill_info == NULL)
        {
            return;
        }
        
        // 스킬 레벨을 낮추면서 각 레벨에 사용된 스킬포인트를 반납한다.
        // 사용된 스킬 포인트를 반납한다.
        SLOTCODE skill_code = skill_info->m_SkillCode;
        for (int level = skill_info->m_wSkillLv; level >= 0; --level, --skill_code)
        {
            const SkillScriptInfo* const temp_skill_info = skill_parser->GetSkillInfo(skill_code);
            if (temp_skill_info == NULL)
            {
                continue;
            }
            player->GetCharInfo()->m_iRemainSkill += temp_skill_info->m_byRequireSkillPoint;
        }
        // 위 루프에서 변경된 값을 새로 초기화한다.
        skill_code = skill_info->m_SkillCode;

        // 현재 스킬슬롯을 삭제한다.
        skill_container_->DeleteSlot(skill_slot->GetPos(), NULL);

        // QuickSlot에 스킬이 등록되어 있다면 삭제 시켜준다.
        // 단, 스킬 트리의 1번째줄의 스킬인경우, 0레벨의 스킬로 교체한다.
        QuickManager* const quick_manager = player->GetQuickManager();
        if (quick_manager != NULL)
        {
#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
            quick_manager->UnlinkByCode(SI_SKILL, skill_code);
#else
            if (job_desc->line_index == 0)
            {
                quick_manager->ChangeSkill(skill_code, job_desc->first_skill_code);
            }
            else
            {
                quick_manager->UnlinkByCode(SI_SKILL, skill_code);
            }
#endif // _NA_004530_20120315_1202_SKILL_RENEWAL
        }

        // 패시브 스킬이라면 이전스킬을 삭제한다. <- (080120, WAVERIX) 이거 항상 성공하는 것 아닌감?
        if (skill_slot_manager_->RemovePassiveSkill(skill_code) == false)
        {
            // 패시브 스킬이 아닌경우 쿨타임 제거
            player->FreeSkillCoolTime(skill_code);
        }

        // 소환체 스킬이라면 이전스킬을 제거
        SummonManager::Instance()->DelSummonedSkill(player->GetObjectKey(), skill_code);

        GAMELOG->WriteSkillLevelDown(player, skill_code, false);
    }
};

void SkillSlotManager::InitSkill()
{
    if (ValidState() == false) 
        return;

    SkillLevelDown Opr(this);
    skill_container_->ForEachSlot(Opr);

#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
    // 남아 있는 스킬 포인트를 계산한다.
    int remain_skill_point = 0;
    const LEVELTYPE cur_level = player_->GetLevel();
    for (LEVELTYPE lv = 1; lv <= cur_level; ++lv)
    {
        remain_skill_point += ns_formula::GetSkillPoint(lv);
    }
    if (player_->GetRemainSkillPoint() != remain_skill_point)
    {
        player_->GetCharInfo()->m_iRemainSkill = remain_skill_point;
    }
#endif
    
    UnLockFirstSkillLine();

    ;{
        MSG_CG_SKILL_REFRESH_CMD msg_cmd;
        player_->SerializeSkillTotalInfo(msg_cmd.m_SkillInfo, SERIALIZE_LOAD);
        player_->SendPacket(&msg_cmd, msg_cmd.GetSize());
    };
    ;{
        MSG_CG_CHARINFO_QUICK_CMD msg_cmd;
        player_->GetQuickManager()->SerializeQuickTotalInfo(msg_cmd.m_QuickInfo, SERIALIZE_LOAD);
        player_->SendPacket(&msg_cmd, msg_cmd.GetSize());
    };
    ;{
        MSG_CG_SKILL_POINT_REMAIN_CMD msg_cmd;
        msg_cmd.m_iRemainSkill = player_->GetRemainSkillPoint();
        player_->SendPacket(&msg_cmd, sizeof(msg_cmd));
    };
}

void SkillSlotManager::LevelDownSkillUntilSatisfaction(SLOTCODE cur_skill_code)
{
    SLOTCODE target_skill_code = cur_skill_code;
    while (true)
    {
        //레벨을 다운시켜야 하나?
        if (CanLevelDownSkill(target_skill_code))
        {
            const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
            const SkillScriptInfo* base_skill_info = skill_parser->GetSkillInfo(target_skill_code);
            if (base_skill_info == NULL) {
                return;
            }
            //스킬 레벨이 1이면...
            if (1 == base_skill_info->m_wSkillLv)
            {
                if (CanUnLearnSkill((target_skill_code)))
                {
                    UnLearnSkill(cur_skill_code);
                }
                else
                {
                    LevelDownSkill(cur_skill_code, target_skill_code);              
                }               
                break;
            }
            else
            {
                target_skill_code--;
            }
        }
        else
        {
            LevelDownSkill(cur_skill_code, target_skill_code);
            break;
        }
    }
}

bool SkillSlotManager::CanLevelDownSkill(SLOTCODE cur_skill_code)
{
    // cur_skill_code에 해당하는 스킬이 스킬스크립트에 존재하는지 체크
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    const SkillScriptInfo* base_skill_info = skill_parser->GetSkillInfo(cur_skill_code);
    if (base_skill_info == NULL) {
        return false;
    }

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    return true;
#else
    // 배운 스킬의 요구레벨을 만족못하면
    if (player_->GetLevel() < base_skill_info->m_wRequireLv) {
        return true;
    }
    // 배운 스킬의 요구숙련도를 만족못하면
    if (player_->GetExperty1() < base_skill_info->m_wRequireSkillStat[0]) {
        return true;
    }
    if (player_->GetExperty2() < base_skill_info->m_wRequireSkillStat[1]) {
        return true;
    }
    return false;
#endif //_NA_0_20100901_SKILLTREE_SYSTEM
}

void SkillSlotManager::LevelDownSkill(SLOTCODE cur_skill_code, SLOTCODE lower_skill_code)
{
    if (cur_skill_code == lower_skill_code) {
        return;
    }
    // 현재 스킬슬롯
    SCSkillSlot* cur_skill_slot = skill_container_->GetSkillSlotByCode(cur_skill_code);
    if (cur_skill_slot == NULL) {
        return; 
    }
    //
    const POSTYPE cur_slot_pos = cur_skill_slot->GetPos();
    // 새롭게 스킬슬롯 정보를 셋팅한다.
    SCSkillSlot making_skill_slot;
    making_skill_slot.SetCode(lower_skill_code);
    making_skill_slot.SetPos(cur_slot_pos);
    //
    const SkillScriptInfo* cur_skill_info = cur_skill_slot->GetSkillInfo();
    const SkillScriptInfo* lower_skill_info = making_skill_slot.GetSkillInfo();
    assert(lower_skill_info);
    if (cur_skill_info->m_SkillClassCode != lower_skill_info->m_SkillClassCode) {
        assert(cur_skill_info->m_SkillClassCode == lower_skill_info->m_SkillClassCode);
        return;
    }
    //
    skill_container_->DeleteSlot(cur_slot_pos, NULL);
    skill_container_->InsertSlot(cur_slot_pos, making_skill_slot);

    ;{
        // QuickSlot에 스킬이 등록되어 있다면 레벨다운 시켜준다.
        // CHANGES, f100826.7L, change a functor class to regard a locality.
        nsSlot::QuickSlotOriginalCodeChanger do_change_event(\
            quick_container_,cur_skill_code, lower_skill_code);
    };

    if (lower_skill_info->m_bySkillType == SKILL_TYPE_PASSIVE)
    {
        // 패시브 스킬이라면 이전스킬을 제거하고 새로운 스킬을 추가한다.
        RemovePassiveSkill(cur_skill_code);
        AddAttrForPassiveSkill(lower_skill_code);
    }
    else
    {   // 이전 스킬 쿨타임 제거 및 새스킬 쿨타임 등록
        player_->FreeSkillCoolTime(cur_skill_code);
        player_->SetSkillCoolTime(lower_skill_code, SKILL_COOLTIME_MARGIN);
    }
    // 소환체 스킬이라면 이전스킬을 제거하고 새로운 스킬을 추가한다.
    if (cur_skill_info->m_bySkillUserType == eSKILL_USER_SUMMONED)
    {
        SummonManager* summon_manager = SummonManager::Instance();
        DWORD summoner_key = player_->GetObjectKey(); // ... player key?
        summon_manager->DelSummonedSkill(summoner_key, cur_skill_code);
        summon_manager->AddSummonedSkill(summoner_key, lower_skill_code);
    }
    // NOTE, a current logic has only 1 level downgrade operation.
    // need change a logic to apply multi-level downgrade.
    //요구했던 스킬 포인트를 돌려주자.
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    int* const remain_skill_point = &player_->GetCharInfo()->m_iRemainSkill;
    for (SLOTCODE skill_code = cur_skill_code; skill_code > lower_skill_code; --skill_code)
    {
        const SkillScriptInfo* base_skill_info = skill_parser->GetSkillInfo(skill_code);
        if (base_skill_info == NULL) {
            // WARNING : ... problem point ...
            return;
        }
        (*remain_skill_point) += base_skill_info->m_byRequireSkillPoint;
    }
    //
    GAMELOG->WriteSkillLevelDown(player_, lower_skill_code, true);
}

bool SkillSlotManager::CanUnLearnSkill(SLOTCODE cur_skill_code)
{
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    const SkillScriptInfo* base_skill_info = skill_parser->GetSkillInfo(cur_skill_code);
    if (base_skill_info == NULL) {
        return false;
    }
    //아이템으로 배우는 아이템은 UnLearn 시키지 않는다.
    if (base_skill_info->m_bySkillAcquire) {
        return false;
    }
    return true;
}

void SkillSlotManager::UnLearnSkill(SLOTCODE cur_skill_code)
{
    // 현재 스킬슬롯
    SCSkillSlot* cur_skill_slot = skill_container_->GetSkillSlotByCode(cur_skill_code);
    if (!cur_skill_slot) {
        return; 
    }
    // 현재 스킬슬롯을 삭제한다.
    skill_container_->DeleteSlot(cur_skill_slot->GetPos(), NULL);

    // QuickSlot에 스킬이 등록되어 있다면 삭제 시켜준다.
    QuickManager* quick_manager = player_->GetQuickManager();
    if (!quick_manager) {
        return;
    }
    quick_manager->UnlinkByCode(SI_SKILL, cur_skill_code);

    // 패시브 스킬이라면 이전스킬을 삭제한다. <- (080120, WAVERIX) 이거 항상 성공하는 것 아닌감?
    if (!RemovePassiveSkill(cur_skill_code))
    {
        // 패시브 스킬이 아닌경우 쿨타임 제거
        player_->FreeSkillCoolTime(cur_skill_code);
    }

    // 소환체 스킬이라면 이전스킬을 제거
    SummonManager::Instance()->DelSummonedSkill(player_->GetObjectKey(), cur_skill_code);

    //요구했던 스킬 포인트를 돌려주자.
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    int* const remain_skill = &player_->GetCharInfo()->m_iRemainSkill;
    SLOTCODE calc_skill_code = cur_skill_code;
    while (const SkillScriptInfo* base_skill_info = skill_parser->GetSkillInfo(calc_skill_code))
    {
        (*remain_skill) += base_skill_info->m_byRequireSkillPoint;
        // TODO: need a validation test.
        // a downgrade calculation of the 1 level is changed to empty skill.
        if (base_skill_info->m_wSkillLv == 1) {
            break;
        }
        --calc_skill_code;
    };
    if (calc_skill_code == cur_skill_code) {
        return;
    }
    //
    GAMELOG->WriteSkillLevelDown(player_, cur_skill_code, false);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Passive Skills
////////////////////////////////////////////////////////////////////////////////////////////////////

void SkillSlotManager::UpdatePassiveSkills()
{
    FOREACH_CONTAINER(const SKILL_HASH::value_type& node, passive_skills_, SKILL_HASH)
    {
        Skill* const skill = node.second;
        if (skill->GetSkillType() != SKILL_TYPE_PASSIVE) 
        {
            continue;
        };
        if (player_->CanApplyPassiveSkill(skill->GetSkillCode()))
        {
            skill->StartExecute();
        }
        else
        {
            skill->EndExecute();
        }
    }
}

const Skill* SkillSlotManager::FindPassiveSkillfromSkillClass(const eSKILL skill_class) const
{
    FOREACH_CONTAINER(const SKILL_HASH::value_type& node, passive_skills_, SKILL_HASH)
    {
        Skill* const skill = node.second;
        if (skill->GetSkillClassCode() == skill_class) 
        {
            return skill;
        }
    }
    return NULL;
}

void SkillSlotManager::AddPassiveSkill(Skill* const skill)
{
    if (skill == NULL) 
    {
        return;
    }
    const SLOTCODE skill_code = skill->GetSkillCode();
    if (FindPassiveSkill(skill_code) != NULL)
    {
        RemovePassiveSkill(skill_code);
    }
    passive_skills_[skill_code] = skill;
    skill->StartExecute();
}

bool SkillSlotManager::AddAttrForPassiveSkill(const SLOTCODE skill_code)
{
    const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();
    const SkillScriptInfo* const skill_script_info = skill_parser->GetSkillInfo(skill_code);
    if (skill_script_info == NULL) 
    {
        return false;
    }
    if (skill_script_info->m_bySkillType != SKILL_TYPE_PASSIVE) 
    {
        return false;
    }
    const SkillFactory* const skill_factory = SkillFactory::Instance();
    Skill* const new_skill = skill_factory->AllocSkill(SKILL_TYPE_PASSIVE, skill_script_info);
    if (new_skill == NULL)
    {
        return false;
    }

    SkillInfo skill_info;
    skill_info.skill_code_ = skill_code;
    new_skill->Init(player_, &skill_info, skill_script_info);

    AddPassiveSkill(new_skill);

    return true;
}

bool SkillSlotManager::RemovePassiveSkill(const SLOTCODE skill_code)
{
    Skill* const skill = FindPassiveSkill(skill_code);
    if (skill == NULL)   
    {
        return false;
    }
    passive_skills_.erase(skill_code);
    skill->Release();
    SkillFactory::Instance()->FreeSkill(skill);
    return true;
}


//==================================================================================================
//==================================================================================================
// NOTE: The Episode I Passive skills & Style skills
//==================================================================================================
//==================================================================================================

#if SKILL_SYSTEM_VERSION < 2

namespace nsSlot {
;

struct QuickStyleUpdate
{
    QuickStyleUpdate(IQuickStyleManager* manager, const BASE_StyleQuickRegistInfo* style_quick_info)
        : manager_(manager), style_quick_info_(style_quick_info)
    {}

    void operator()(SCSlot* slot)
    {
        const SCSkillSlot* skill_slot = static_cast<SCSkillSlot*>(slot);
        const SLOTCODE style_code = skill_slot->GetCode();
        const BASE_STYLEINFO* base_style_info = skill_slot->GetStyleInfo();

        if (base_style_info == NULL) {
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't find a style info (%d)|"),
                   style_code);
            return;
        }
        assert(base_style_info->IsStyle());
        //
        BOOST_STATIC_ASSERT(_countof(style_quick_info_->m_wStyleClassCode) == MAX_SKILL_REGIST_COUNT);
        const uint16_t* it = style_quick_info_->m_wStyleClassCode,
                      * const end = &style_quick_info_->m_wStyleClassCode[\
                          _countof(style_quick_info_->m_wStyleClassCode)];
        for ( ; it != end; ++it)
        {
            if (*it == 0) { // stop loop
                return;
            }
            if (*it != base_style_info->m_SkillClassCode) {
                continue;
            }
            // register a style to a quick style slot.
            RC::eSTYLE_RESULT result = manager_->LinkStyle(style_code,
                static_cast<POSTYPE>(it - style_quick_info_->m_wStyleClassCode));
            __UNUSED(result);
            assert(result == RC::RC_STYLE_SUCCESS);
            break;
        }
    }
    // data fields ...
    IQuickStyleManager* const manager_;
    const BASE_StyleQuickRegistInfo* const style_quick_info_;
    //
    __DISABLE_COPY(QuickStyleUpdate);
};

}; //end of namespace

//==================================================================================================
// IQuickStyleManager interface
bool SkillSlotManager::UpdateStyleSlots()
{
    const eCHAR_TYPE char_type = player_->GetCharType();
    const int weapon_kind = player_->GetWeaponKind();

    StyleQuickRegistInfoParser* const style_regist_parser = StyleQuickRegistInfoParser::Instance();
    const BASE_StyleQuickRegistInfo* style_quick_info = \
        style_regist_parser->GetStyleQuickRegistInfo(char_type, weapon_kind);
    // TODO : need more a case study.
    style_container_->ClearAll();
    if (style_quick_info == NULL) {
        return false;
    }
    //
    nsSlot::QuickStyleUpdate op(this, style_quick_info);
    skill_container_->ForEachSlot(op);

    return true;
}

//
// IQuickStyleManager interface
RC::eSTYLE_RESULT SkillSlotManager::LinkStyle(SLOTCODE style_code, POSTYPE pos)
{
    if (ValidState() == false) {
        return RC::RC_STYLE_INVALIDSTATE;
    }
    if (slot_manager_->ValidPos(SI_STYLE, pos, false) == false) {
        return RC::RC_STYLE_INVALIDPOS;
    }
    //
    if (style_container_->IsEmpty(pos) == false) {
        return RC::RC_STYLE_INVALIDPOS;
    }

    // 어떤 것을 올릴 수 있는지에 대한 체크 필요!
    SCQuickStyleSlot slot;
    slot.SetOrgCode(style_code);
    static_cast<SCSlotContainer*>(style_container_)->InsertSlot(pos, slot);

    return RC::RC_STYLE_SUCCESS;
}

/*
// NOTE, a style quick slot container of a player has only link style operation.
// how to do an unlink style quick slot container? it can solve by clearing a slot container
// IQuickStyleManager interface
RC::eSTYLE_RESULT SkillSlotManager::UnlinkStyle(POSTYPE pos)
{
    if (ValidState() == false) {
        return RC::RC_STYLE_INVALIDSTATE;
    }
    if (slot_manager_->ValidPos(SI_STYLE, pos, true) == false) {
        return RC::RC_STYLE_INVALIDPOS;
    }
    //
    style_container_->DeleteSlot(pos, NULL);

    return RC::RC_STYLE_SUCCESS;
}

// IQuickStyleManager interface
RC::eSTYLE_RESULT SkillSlotManager::MoveStyle(POSTYPE from_pos, POSTYPE to_pos)
{
    if (ValidState() == false) {
        return RC::RC_STYLE_INVALIDSTATE;
    }
    if (slot_manager_->ValidPos(SI_STYLE, from_pos, true) == false) {
        return RC::RC_STYLE_INVALIDPOS;
    }
    if (slot_manager_->ValidPos(SI_STYLE, to_pos, false) == false) {
        return RC::RC_STYLE_INVALIDPOS;
    }

    SCQuickStyleSlot from_slot;
    style_container_->DeleteSlot(from_pos, &from_slot);
    if (false != style_container_->IsEmpty(to_pos))
    {
        // 한 방향 이동
        RC::eSLOT_INSERT_RESULT result = \
            static_cast<SCSlotContainer*>(style_container_)->InsertSlot(to_pos, from_slot);
        if (RC::RC_INSERT_SLOT_SUCCESS != result)
        {
            static_cast<SCSlotContainer*>(style_container_)->InsertSlot(from_pos, from_slot);
            return SlotInsertResultConverter::ConvertToeStyleResult(result);
        }
    }
    else
    {
        // 교환
        SCQuickStyleSlot to_slot;
        static_cast<SCSlotContainer*>(style_container_)->DeleteSlot(to_pos, &to_slot);

        RC::eSLOT_INSERT_RESULT result = \
            static_cast<SCSlotContainer*>(style_container_)->InsertSlot(from_pos, to_slot);
        if (RC::RC_INSERT_SLOT_SUCCESS != result)
        {
            //rollback
            static_cast<SCSlotContainer*>(style_container_)->InsertSlot(from_pos, from_slot);
            static_cast<SCSlotContainer*>(style_container_)->InsertSlot(to_pos, to_slot);
            return SlotInsertResultConverter::ConvertToeStyleResult(result);
        }
        //
        result = static_cast<SCSlotContainer*>(style_container_)->InsertSlot(to_pos, from_slot);
        if (RC::RC_INSERT_SLOT_SUCCESS != result)
        {
            //rollback
            style_container_->DeleteSlot(from_pos, NULL);
            static_cast<SCSlotContainer*>(style_container_)->InsertSlot(from_pos, from_slot);
            static_cast<SCSlotContainer*>(style_container_)->InsertSlot(to_pos, to_slot);
            return SlotInsertResultConverter::ConvertToeStyleResult(result);
        }
    }

    return RC::RC_STYLE_SUCCESS;
}
*/
//
// IQuickStyleManager interface
void SkillSlotManager::SerializeStyleTotalInfo(STYLE_TOTAL_INFO& style_total_info,
                                               eSERIALIZE serialize_type)
{
    BOOST_STATIC_ASSERT(_countof(style_total_info.m_Slot) == STYLE_TOTAL_INFO::MAX_SLOT_NUM);
    //
    const int max_number_of_slots = style_container_->GetMaxSlotNum();
    if (!FlowControl::FCAssert(max_number_of_slots == _countof(style_total_info.m_Slot))) {
        __asm { int 3 };
        return;
    }
    //
    if (serialize_type == SERIALIZE_LOAD)
    {
        style_total_info.m_Count = 0;
        STYLE_TOTAL_INFO::SLOT_TYPE* it = style_total_info.m_Slot,
            * const end = &style_total_info.m_Slot[_countof(style_total_info.m_Slot)];
        for (POSTYPE pos = 0; pos < _countof(style_total_info.m_Slot) && it != end; ++pos)
        {
            if (style_container_->IsEmpty(pos)) {
                continue;
            }
            it->m_Pos = pos;
            //
            SCQuickStyleSlot& slot = static_cast<SCQuickStyleSlot&>(style_container_->GetSlot(pos));
            slot.CopyOut(it->m_Stream);
            //
            ++it;
        }
        style_total_info.m_Count = static_cast<POSTYPE>(it - style_total_info.m_Slot);
    }
    else if (serialize_type == SERIALIZE_STORE)
    {
        if (FlowControl::FCAssert(style_container_->GetSlotNum() == 0) == false) {
            __asm { int 3 };
            return;
        };
        const int number_of_slots = style_total_info.m_Count;
        STYLE_TOTAL_INFO::SLOT_TYPE* it = style_total_info.m_Slot,
                                   * const end = style_total_info.m_Slot + number_of_slots;
        for ( ; it != end; ++it)
        {
            style_container_->InsertSlot(it->m_Pos, it->m_Stream);
        }
    }
}

//// IQuickStyleManager interface
//void SkillSlotManager::SerializeStyleStream(POSTYPE pos,
//                                            STYLESTREAM* IN stream, eSERIALIZE serialize_type)
//{
//    if (serialize_type == SERIALIZE_LOAD)
//    {
//        SCQuickStyleSlot& slot = static_cast<SCQuickStyleSlot&>(style_container_->GetSlot(pos));
//        slot.CopyOut(*stream);
//    }
//    else if (serialize_type == SERIALIZE_STORE) {
//        style_container_->InsertSlot(pos, *stream);
//    }
//}

// 스타일 습득 관련
RC::eSKILL_RESULT SkillSlotManager::CanLevelUpStyle(SLOTCODE cur_style_code, SLOTCODE new_style_code)
{
    if (!ValidState()) {
        return RC::RC_SKILL_FAILED;
    }
    // 1렙씩 증가하도록 체크
    if ((cur_style_code+1) != new_style_code) {
        return RC::RC_SKILL_FAILED;
    }
    // cur_style_code에 해당하는 스타일을 습득하고 있는지 체크
    SCSkillSlot* cur_skill_slot = skill_container_->GetSkillSlotByCode(cur_style_code);
    if (cur_skill_slot == NULL) {
        return RC::RC_SKILL_DOES_NOT_HAVE;
    }
    const BASE_STYLEINFO* skill_info = cur_skill_slot->GetStyleInfo();
    if (skill_info == NULL) {
        return RC::RC_SKILL_BASEINFO_NOTEXIST;
    }

    // 새로운 스킬의 요구조건을 만족하는지 체크
    return CanLearnStyle(false, new_style_code);
}

//==================================================================================================
//=== Passive Skill ================================================================================
//==================================================================================================

class SearchPassiveSkillStyleAllUnLean
{
    SkillSlotManager* skill_slot_manager_;    
public:
    SearchPassiveSkillStyleAllUnLean(SkillSlotManager* skill_slot_manager) : skill_slot_manager_(skill_slot_manager) {} 

    void operator () (SCSlot* pSlot)
    {
        SCSkillSlot* pSkillSlot = (SCSkillSlot*)pSlot;
        if (!skill_slot_manager_)
            return;

        const SLOTCODE skillcode = pSkillSlot->GetCode();
        if (pSkillSlot->IsStyle())
        {
            if (pSkillSlot->GetStyleInfo()->m_wRequireLV > 0)
                skill_slot_manager_->UnLearnStyle(skillcode);
        }
        else
        {
            if (skill_slot_manager_->FindPassiveSkill(skillcode))
                skill_slot_manager_->UnLearnSkill(skillcode);
        }

    }
};

void SkillSlotManager::InitPassiveSkillStyle()
{
    if (!skill_container_)
        return;    

    //스타일을 초기화 한다.
    SearchPassiveSkillStyleAllUnLean Opr(this);
    skill_container_->ForEachSlot(Opr);    

    //2010-01-15 버그수정:5690 문상현 
    //스타일을 초기화 하고 이전에 스타일이 초기화 되어서 사용할수 없다면 기본 스타일을 지정한다.
    BASE_STYLEINFO* pPreBaseStylenfo = SkillInfoParser::Instance()->GetStyleInfo(pre_style_code_);
    if (pPreBaseStylenfo && RC::RC_SKILL_SUCCESS != CanSelectStyle(pre_style_code_))
    {
        //패시브 초기화 할때 무기를 모두 벗는다. 
        //무기를 새로 장착할때 적용될수 있도록 이전 스타일 코드를 세팅한다.
        pre_style_code_ = player_->GetDefaultStyle(pPreBaseStylenfo->m_iWeaponDefine);
    }

    //클라에게 스킬을 보내자
    MSG_CG_SKILL_REFRESH_CMD SkillMsg;
    player_->SerializeSkillTotalInfo(SkillMsg.m_SkillInfo, SERIALIZE_LOAD);
    player_->SendPacket(&SkillMsg, SkillMsg.GetSize());

    //클라에게 남은 스킬 포인트를 보내자
    MSG_CG_SKILL_POINT_REMAIN_CMD CmdMsg;
    CmdMsg.m_iRemainSkill = player_->GetRemainSkillPoint();
    player_->SendPacket(&CmdMsg, sizeof(CmdMsg));


}

void SkillSlotManager::UnLearnStyle(SLOTCODE style_code)
{
    // 현재 스타일 슬롯 정보를 얻는다.
    SCSkillSlot* cur_skill_slot = skill_container_->GetSkillSlotByCode(style_code);        

    // 스타일의 위치를 찾는다.
    POSTYPE StylePos = 0;
    StylePos = cur_skill_slot->GetPos();

    //(shogen)(090813)
    //if (!StylePos)   return;

    // 스킬슬롯을 삭제한다.
    if (!skill_container_->DeleteSlot(StylePos, NULL))  
        return;     

    // 퀵 스타일 슬롯의 정보들이 모두 바뀐다.
    UpdateStyleSlots();

    //요구했던 스킬 포인트를 돌려주자.
    BASE_STYLEINFO* pBaseStyleInfo = NULL;
    SLOTCODE NowSkillCode = style_code;
    while (NowSkillCode)
    {        
        pBaseStyleInfo = SkillInfoParser::Instance()->GetStyleInfo(NowSkillCode);
        if (!pBaseStyleInfo)   return;

        player_->GetCharInfo()->m_iRemainSkill += pBaseStyleInfo->m_byRequireSkillPoint;
        if (1 == pBaseStyleInfo->m_wStyleLV)
        {
            //요구 포인트가 0로면 생성시 배우는 스킬이므로 다시 배워라.
            if (pBaseStyleInfo->m_byRequireSkillPoint == 0)
            {
                SKILLSLOT SkillSlot;
                LearnStyle(pBaseStyleInfo->m_SkillCode, SkillSlot);
            }
            break;
        }

        NowSkillCode--;
    }

    // 스타일 Unlearn 로그 남기기
    GAMELOG->WriteSkillLevelDown(player_, style_code, false);
    // 스타일 획득 로그 남기기 끝
}


//==================================================================================================
//=== Quick Style ==================================================================================
//==================================================================================================

class QuickStyleLevelUp
{
    BOOL        m_bComplete;
    SLOTCODE    m_CurStyleCode;
    SLOTCODE    m_NewStyleCode;
public:
    QuickStyleLevelUp(SLOTCODE cur_style_code, SLOTCODE new_style_code) : m_CurStyleCode(cur_style_code), 
        m_NewStyleCode(new_style_code), m_bComplete(0) {}

    void operator () (SCSlot* pSlot)
    {
        SCQuickStyleSlot* pStyleSlot = (SCQuickStyleSlot*)pSlot;

        if (m_bComplete)                                   return;
        if (pStyleSlot->GetOrgCode() != m_CurStyleCode)    return;

        pStyleSlot->SetOrgCode(m_NewStyleCode);
        m_bComplete = true;
    }
};

//--------------------------------------------------------------------------------------------------

void SkillSlotManager::LevelUpStyle(SLOTCODE cur_style_code, SLOTCODE new_style_code,
                                    SKILLSLOT& OUT new_skill_slot)
{
    if (cur_style_code == new_style_code) {
        return;
    }
    // 현재 스킬슬롯
    const SCSkillSlot* cur_style_slot = skill_container_->GetSkillSlotByCode(cur_style_code);
    if (cur_style_slot == NULL) {
        return;
    };
    const BASE_STYLEINFO* cur_style_info = cur_style_slot->GetStyleInfo();
    assert(cur_style_info);
    //
    const POSTYPE cur_style_slot_pos = cur_style_slot->GetPos();
    //
    SCSkillSlot making_skill_slot;
    making_skill_slot.SetCode(new_style_code);
    making_skill_slot.SetPos(cur_style_slot_pos);
    //
    const BASE_STYLEINFO* making_style_info = making_skill_slot.GetStyleInfo();
    if (making_style_info == NULL) {
        return; // critical
    }
    if (cur_style_info->m_SkillClassCode != making_style_info->m_SkillClassCode) {
        return; // critical
    }
    //
    SCSkillSlot rollback_cur_slot = *cur_style_slot;

    // 현재 스킬슬롯을 삭제한다.
    if (!skill_container_->DeleteSlot(cur_style_slot_pos, NULL))
    {
        int* remain_skill_point = &player_->GetCharInfo()->m_iRemainSkill;
        (*remain_skill_point) += making_style_info->m_byRequireSkillPoint;
        return;
    }

    // 새롭게 스킬슬롯을 추가한다.
    if (RC::RC_INSERT_SLOT_SUCCESS !=
        skill_container_->InsertSlot(cur_style_slot_pos, making_skill_slot))
    {
        int* remain_skill_point = &player_->GetCharInfo()->m_iRemainSkill;
        (*remain_skill_point) += making_style_info->m_byRequireSkillPoint;

        // 이전 스타일을 복구해준다.
        skill_container_->InsertSlot(cur_style_slot_pos, rollback_cur_slot);
        return;
    }

    // new_skill_slot에 값을 셋팅한다. f100825.2L, fix up a value containing an invalid position.
    new_skill_slot.m_Pos = cur_style_slot_pos;
    making_skill_slot.CopyOut(new_skill_slot.m_Stream);
    // QuickSlot에 스킬이 등록되어 있다면 레벨업 시켜준다.
    QuickStyleLevelUp Opr(cur_style_code, new_style_code);
    style_container_->ForEachSlot(Opr);

    // 레벨업한 스타일이 내가 선택하고 있던 스타일이라면
    // 새롭게 버프를 적용하고, 선택 스타일 코드를 새걸로 바꿔준다.
    if (player_->GetBaseSelectedStyle() == cur_style_code)
    {
        StatusManager* const status_manager = player_->GetStatusManager();
        if (status_manager->Status.IsDragonTransforming()) {
            player_->SetBaseSelectedStyle(new_style_code);
        }
        else {
            SetSelectStyle(new_style_code);
        }
    }
    //
    GAMELOG->WriteSkillLevelUp(player_, new_style_code, true);
}

RC::eSKILL_RESULT SkillSlotManager::CanLearnStyle(BOOL is_first_learn, SLOTCODE new_style_code)
{
    if (ValidState() == false) {
        return RC::RC_SKILL_FAILED;
    }
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();

    // new_style_code에 해당하는 스타일이 스타일스크립트에 존재하는지 체크
    const BASE_STYLEINFO* new_style_info = skill_parser->GetStyleInfo(new_style_code);
    if (new_style_info == NULL) {
        return RC::RC_SKILL_BASEINFO_NOTEXIST;
    }
    // 최초 습득일 경우 1렙인지 체크
    if (is_first_learn && new_style_info->m_wStyleLV != 1)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=request an invalid style learn process ")
               _T("which is requested %d skill over than 1 level style on first learn, ")
               _T("hack? bug?|CharName=%s|"),
               new_style_code, player_->GetCharName());
        return RC::RC_SKILL_FAILED;
    }
    // 스타일레벨이 만렙(LV10)인 경우, 오버가능 레벨 체크 필요!!!
    if (new_style_info->m_wStyleLV > MAX_SKILL_LEVEL) {
        return RC::RC_SKILL_MAX_LEVEL_LIMIT;
    }
    // 캐릭터 클래스 조건이 맞는지 체크
    if (player_->GetCharType() != new_style_info->m_byClassDefine) {
        return RC::RC_SKILL_CHAR_CLASS_LIMIT;
    }
    // 해당 클래스의 스타일을 이미 습득하고 있으며, 기존의 레벨이 새로운 레벨보다 같거나 높을 경우
    SCSkillSlot* cur_skill_slot = \
        skill_container_->GetSkillSlotByClass(new_style_info->m_SkillClassCode);
    const BASE_STYLEINFO* cur_style_info = \
        cur_skill_slot == NULL ? NULL : cur_skill_slot->GetStyleInfo();
    if (cur_skill_slot)
    {
        if (cur_style_info->m_wStyleLV >= new_style_info->m_wStyleLV) {
            return RC::RC_SKILL_ALREADY_EXIST_SKILL;
        }
    }
    //
    const int remains_skill_point = player_->GetCharInfo()->m_iRemainSkill;
    // 남은 스킬포인트가 요구스킬포인트보다 적은 경우
    if (remains_skill_point < new_style_info->m_byRequireSkillPoint) {
        return RC::RC_SKILL_REMAIN_SKILLPOINT_LACK;
    }
    // 새로운 스타일의 요구레벨을 만족하는지 체크
    if (player_->GetLevel() < new_style_info->m_wRequireLV) {
        return RC::RC_SKILL_REQUIRE_LEVEL_LIMIT;
    }

    return RC::RC_SKILL_SUCCESS;
}

void SkillSlotManager::LearnStyle(SLOTCODE new_style_code, SKILLSLOT& OUT new_skill_slot)
{
    // 새롭게 스킬슬롯 정보를 셋팅한다.
    SCSkillSlot making_slot;
    making_slot.SetCode(new_style_code);
    //
    const BASE_STYLEINFO* new_style_info = making_slot.GetStyleInfo();
    if (new_style_info == NULL) {
        assert(new_style_info != NULL);
        return;
    }
    //
    POSTYPE empty_pos = 0;
    // f100930.6L, expected a NOT condition check, but it did occur a NOT-NOT condition check. 
    if (skill_container_->GetEmptyPos(empty_pos) == false) {
        return;
    }
    // 스킬슬롯을 추가한다.
    if (RC::RC_INSERT_SLOT_SUCCESS != skill_container_->InsertSlot(empty_pos, making_slot))
    {
        int* remain_skill_point = &player_->GetCharInfo()->m_iRemainSkill;
        (*remain_skill_point) += new_style_info->m_byRequireSkillPoint;
        return;
    }

    // new_skill_slot에 값을 셋팅한다. f100825.2L, fix up... find a related reference above section
    new_skill_slot.m_Pos = empty_pos;
    making_slot.CopyOut(new_skill_slot.m_Stream);

    // 퀵 스타일 슬롯의 정보들이 모두 바뀐다.
    UpdateStyleSlots();
    //
    GAMELOG->WriteSkillLevelUp(player_, new_style_code, false);
}

#endif //SKILL_SYSTEM_VERSION < 2

//==================================================================================================
//==================================================================================================
//==================================================================================================
// NOTE: Default Styles, this section is a generic style routines
//==================================================================================================

// 스타일 지정
RC::eSKILL_RESULT SkillSlotManager::CanSelectStyle(SLOTCODE new_style_code)
{
    const SLOTCODE cur_style_code = player_->GetSelectedStyle();
    // NOTE, f100825.1L, add a rule
    if (cur_style_code == new_style_code) {
        return RC::RC_SKILL_INVLIDPOS;
    }
    // 변신상태이면 스타일 지정 실패
    StatusManager* const status_manager = player_->GetStatusManager();
    if (status_manager->Status.IsDragonTransforming()) {
        return RC::RC_SKILL_UNABLE_FUNCTION_FOR_TRANSFORMATION;
    }
    if (!ValidState()) {
        return RC::RC_SKILL_FAILED;
    }
    // 해당하는 스타일을 습득하고 있는지 체크한다.
    SCSkillSlot* new_skill_slot = skill_container_->GetSkillSlotByCode(new_style_code);
    if (!new_skill_slot) {
        return RC::RC_SKILL_DOES_NOT_HAVE;
    }
    // 해당하는 스타일이 스타일스크립트에 존재하는지 체크
    const BASE_STYLEINFO* base_style_info = new_skill_slot->GetStyleInfo();
    if (!base_style_info) {
        return RC::RC_SKILL_BASEINFO_NOTEXIST;
    }
    // 캐릭터 클래스 체크
    if (player_->GetCharType() != base_style_info->m_byClassDefine)
    {
        SUNLOG(eFULL_LOG,  "[SkillManager::CanSelectStyle] The Type Of Character is Invalid! ");
        return RC::RC_SKILL_CHAR_CLASS_LIMIT;
    }
    // 무기 제한
    if (base_style_info->m_iWeaponDefine != -1)
    {
        int cur_weapon_kind = player_->GetWeaponKind();

        if (cur_weapon_kind != base_style_info->m_iWeaponDefine)
        {
            SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg=the weapon type is invalid|"));
            return RC::RC_SKILL_WEAPON_LIMIT;
        }
    }
    // 요구레벨을 체크한다.
    if (player_->GetLevel() < base_style_info->m_wRequireLV)
    {
        SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|")
               _T("Msg=the level of player not meet a requirement about a level|"));
        return RC::RC_SKILL_REQUIRE_LEVEL_LIMIT;
    }

    return RC::RC_SKILL_SUCCESS;
}

//--------------------------------------------------------------------------------------------------

void SkillSlotManager::SetSelectStyle(SLOTCODE new_style_code)
{
    const SLOTCODE cur_style_code = player_->GetSelectedStyle();
    // NOTE, f100825.1L, add a rule
    if (cur_style_code == new_style_code) {
        return;
    }
    //
    MSG_CG_STYLE_SELECT_STYLE_BRD msg_brd;
    msg_brd.m_CurStyleCode = cur_style_code;

    // 이전 버프를 해제한다.
    ReleaseStyleBuff(cur_style_code);

    // 스타일을 지정한다.
    player_->SetBaseSelectedStyle(new_style_code);

    // 새로 지정한 스타일의 버프를 적용한다.
    ApplyStyleBuff(new_style_code); 

    // 주변에 패킷을 날린다.
    msg_brd.m_NewStyleCode = new_style_code;
    msg_brd.m_dwObjectKey = player_->GetObjectKey();
    player_->SendPacketAround(&msg_brd, sizeof(msg_brd));
}

//--------------------------------------------------------------------------------------------------
#if SKILL_SYSTEM_VERSION < 2

void SkillSlotManager::ApplyStyleBuff(SLOTCODE style_code)
{
    // CHANGES f100825.2L, change a skill attributes calculator process to a delayed update concept
    const BASE_STYLEINFO* base_style_info = SkillInfoParser::Instance()->GetStyleInfo(style_code);
    if (base_style_info == NULL) {
        return;
    };
    // f100920.2L, add an attributes calculator skip filter
    if (base_style_info->m_iAttackRate == 0 &&
        base_style_info->m_iAvoidRate == 0 &&
        base_style_info->m_iAttackSpeed == 0 &&
        base_style_info->m_iBonusDefence == 0 &&
        base_style_info->m_wCriticalBonus == 0)
    {
        return;
    }
    //
    Attributes& attributes = player_->GetAttr();
    SkillAttrCalculator skill_calculator(attributes);

    // 공격 성공 증가
    if (base_style_info->m_iAttackRate) {
        skill_calculator.AddAttr(eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO, VALUE_TYPE_VALUE,
                                 base_style_info->m_iAttackRate, true);
    }
    // 블록 성공 증가
    if (base_style_info->m_iAvoidRate) {
        skill_calculator.AddAttr(eATTR_PHYSICAL_ATTACK_BLOCK_RATIO, VALUE_TYPE_VALUE,
                                 base_style_info->m_iAvoidRate, true);
    }
    // 공속 증가
    if (base_style_info->m_iAttackSpeed) {
        skill_calculator.AddAttr(eATTR_ATTACK_SPEED, VALUE_TYPE_VALUE,
                                 base_style_info->m_iAttackSpeed, true);
    }
    // 방어력 증가
    if (base_style_info->m_iBonusDefence) {
        skill_calculator.AddAttr(eATTR_OPTION_ALL_DEFENSE_POWER, VALUE_TYPE_VALUE,
                                 base_style_info->m_iBonusDefence, true);
    }
    // 크리티컬 확률 증가
    if (base_style_info->m_wCriticalBonus) {
        skill_calculator.AddAttr(eATTR_ADD_ALL_CRITICAL_RATIO, VALUE_TYPE_VALUE,
                                 base_style_info->m_wCriticalBonus, true);
    }
    //
    attributes.UpdateEx();
}

//--------------------------------------------------------------------------------------------------

void SkillSlotManager::ReleaseStyleBuff(SLOTCODE style_code)
{
    // CHANGES f100825.2L, change a skill attributes calculator process to a delayed update concept
    const BASE_STYLEINFO* base_style_info = SkillInfoParser::Instance()->GetStyleInfo(style_code);
    if (base_style_info == NULL) {
        return;
    };
    // f100920.2L, add an attributes calculator skip filter
    if (base_style_info->m_iAttackRate == 0 &&
        base_style_info->m_iAvoidRate == 0 &&
        base_style_info->m_iAttackSpeed == 0 &&
        base_style_info->m_iBonusDefence == 0 &&
        base_style_info->m_wCriticalBonus == 0)
    {
        return;
    }
    //
    Attributes& attributes = player_->GetAttr();
    SkillAttrCalculator skill_calculator(attributes);

    // 공격 성공 감소
    if (base_style_info->m_iAttackRate) {
        skill_calculator.DelAttr(eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO, VALUE_TYPE_VALUE,
                                 base_style_info->m_iAttackRate, true);
    }
    // 블록 성공 감소
    if (base_style_info->m_iAvoidRate) {
        skill_calculator.DelAttr(eATTR_PHYSICAL_ATTACK_BLOCK_RATIO, VALUE_TYPE_VALUE,
                                 base_style_info->m_iAvoidRate, true);
    }
    // 공속 감소
    if (base_style_info->m_iAttackSpeed) {
        skill_calculator.DelAttr(eATTR_ATTACK_SPEED, VALUE_TYPE_VALUE,
                                 base_style_info->m_iAttackSpeed, true);
    }
    // 방어력 감소
    if (base_style_info->m_iBonusDefence) {
        skill_calculator.DelAttr(eATTR_OPTION_ALL_DEFENSE_POWER, VALUE_TYPE_VALUE,
                                 base_style_info->m_iBonusDefence, true);
    }
    // 크리티컬 확률 감소
    if (base_style_info->m_wCriticalBonus) {
        skill_calculator.DelAttr(eATTR_ADD_ALL_CRITICAL_RATIO, VALUE_TYPE_VALUE,
                                 base_style_info->m_wCriticalBonus, true);
    }
    //
    attributes.UpdateEx();
}

#endif //SKILL_SYSTEM_VERSION < 2

//==================================================================================================
//==================================================================================================
//==================================================================================================

namespace inner_impl {

// 스킬 직업 줄에 사용된 스킬 포인트를 얻는다.
int GetUsedSkillPointFromJobLine(
    const SkillJobTree* const job_tree, 
    const int line_index, 
    const SCSkillSlotContainer& skill_container)
{
    const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();
    int used_skill_point = 0;
    for (int col_index = 0; col_index < job_tree->GetColumnSize(); ++col_index)
    {
        const SkillJobTree::Desc* const job_desc = job_tree->GetDesc(line_index, col_index);
        if (job_desc == NULL)
        {
            return -1;
        }
        const SCSkillSlot* const skill_slot = 
            skill_container.GetSkillSlotByClass(job_desc->skill_class_code);
        if (skill_slot == NULL)
        {
            continue;
        }
        const SkillScriptInfo* const skill_info = skill_slot->GetSkillInfo();
        if (skill_info == NULL)
        {
            return -1;
        }
        SLOTCODE skill_code = skill_info->m_SkillCode;
        for (int level = skill_info->m_wSkillLv; level >= 0; --level, --skill_code)
        {
            const SkillScriptInfo* const temp_skill_info = skill_parser->GetSkillInfo(skill_code);
            if (temp_skill_info == NULL)
            {
                continue;
            }
            used_skill_point += temp_skill_info->m_byRequireSkillPoint;
        }
    }
    return used_skill_point;
}

// 스킬 줄 검사 방법
struct SkillLineTestMethod
{
    enum Value
    {
        kTotalLine = 1, // 줄 전체 단위 검사
        kJobLine = 2 // 직업 단위 검사
    };
#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
    enum { kTestMethod = kJobLine };
    enum { kRequirePoint = REQUIRE_USED_SKILL_POINT_FOR_NEXT_LINE };
#else
    enum { kTestMethod = kTotalLine };
    enum { kRequirePoint = REQUIRE_USED_SKILL_POINT_FOR_NEXT_LINE };
#endif   
};

template <int kTestMethod>
bool IsEnableSkillJobLine(
    const int char_type, 
    const int last_skill_line, 
    const STLX_VECTOR<int>& last_job_lines, 
    const SCSkillSlotContainer& skill_container)
{
    return false;
}

// 스킬 직업 줄이 선택 가능한지 검사한다. (줄 전체 단위 검사)
template <>
bool IsEnableSkillJobLine<SkillLineTestMethod::kTotalLine>(
    const int char_type, 
    const int last_skill_line, 
    const STLX_VECTOR<int>& last_job_lines, 
    const SCSkillSlotContainer& skill_container)
{
    if (last_skill_line < 2)
    {
        return true;
    }

    // 바로 아래 줄까지 사용된 스킬 포인트 합을 구한다.
    int used_skill_point = 0;
    const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();    
    for (int line_index = 1; line_index < last_skill_line; ++line_index)
    {
        for (int job_type = 0; job_type < skill_parser->GetJobSize(); ++job_type)
        {
            const SkillJobTree* const job_tree = skill_parser->GetJobTree(char_type, job_type);
            if (job_tree == NULL)
            {
                return false;
            }
            if (last_skill_line > job_tree->GetLineSize())
            {
                return false;
            }
            const int job_skill_point = 
                GetUsedSkillPointFromJobLine(job_tree, line_index - 1, skill_container);
            if (job_skill_point > 0)
            {
                used_skill_point += job_skill_point;
            }
        }        
    }

    const int require_point = (last_skill_line - 1) * SkillLineTestMethod::kRequirePoint;
    if (used_skill_point < require_point)
    {
        return false;
    }

    return true;
}

// 스킬 직업 줄이 선택 가능한지 검사한다. (직업 단위 검사)
template <>
bool IsEnableSkillJobLine<SkillLineTestMethod::kJobLine>(
    const int char_type, 
    const int last_skill_line, 
    const STLX_VECTOR<int>& last_job_lines, 
    const SCSkillSlotContainer& skill_container)
{
    __UNUSED(last_skill_line);

    const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();
    for (int job_type = 0; job_type < skill_parser->GetJobSize(); ++job_type)
    {
        const SkillJobTree* const job_tree = skill_parser->GetJobTree(char_type, job_type);
        if (job_tree == NULL)
        {
            return false;
        }
        const int last_job_line = last_job_lines[job_type];
        if (last_job_line > job_tree->GetLineSize())
        {
            return false;
        }
        for (int line_index = 1; line_index < last_job_line; ++line_index)
        {
            // 바로 아래 줄의 사용 포인트 합을 계산한다.
            const int used_skill_point = 
                GetUsedSkillPointFromJobLine(job_tree, line_index - 1, skill_container);
            if (used_skill_point < SkillLineTestMethod::kRequirePoint)
            {
                return false;
            }
        }
    }
    return true;
}

} // inner_impl

//_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
// 해당 스킬을 익히기 위한 상위 스킬의 스킬레벨이 만족되는지 검사.
RC::eSKILL_RESULT SkillSlotManager::check_enough_upper_skill_level(
    const SCSkillSlotContainer* skill_container, const SkillScriptInfo* skill_info) const
{
    // 해당 스킬을 익히기 위한 상위 스킬이 존재하는 경우.
    if (skill_info->m_UpperSkillClass != 0)
    {
        // 상위 스킬의 클래스 값이 존재하는지 찾는다.
        const SCSkillSlot* const upper_skill = skill_container->GetSkillSlotByClass(skill_info->m_UpperSkillClass);
        if (upper_skill == NULL)
        {
            return RC::RC_SKILL_CANT_FIND_UPPER_SKILL;
        }
            
        const SkillScriptInfo* const upper_skill_info = upper_skill->GetSkillInfo();
        if (upper_skill_info == NULL)
        {
            return RC::RC_SKILL_BASEINFO_NOTEXIST;
        }
            
        // 상위 스킬의 레벨이 부족하다.
        if (upper_skill_info->m_wSkillLv < skill_info->m_RequireUpperSkillLevel)
        {
            return RC::RC_SKILL_ISNOT_ENOUGH_UPPER_SKILL_LEVEL;
        }            
    }
    return RC::RC_SKILL_SUCCESS;
}

RC::eSKILL_RESULT SkillSlotManager::LevelUpSkill(
    SKILL_TOTAL_INFO& OUT total_stream,
    const SLOTCODE* skill_code_array, 
    const uint skill_code_count)
{
    if (ValidState() == false)
    {
        return RC::RC_SKILL_FAILED;
    }
        
    const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();

    // 원본 스킬 컨테이너를 보관해 놓는다.
    // 스킬의 레벨업시 임시 컨테이너에 적용시킨 후 모든 스킬 레벨업이 완료시 원본에 덮어쓴다.
    SCSkillSlotContainer temp_skill_container;
    skill_container_->Copy(&temp_skill_container);

    // 레벨업할 스킬들이 유효한지 검사한다.
    STLX_VECTOR<LevelUpData> levelup_data_array;
    levelup_data_array.clear();

    for (int i = 0; i < skill_code_count; ++i)
    {
        const SLOTCODE new_skill_code = skill_code_array[i];
        const SkillScriptInfo* const new_skill_info = skill_parser->GetSkillInfo(new_skill_code);
        if (new_skill_info == NULL)
        {
            return RC::RC_SKILL_BASEINFO_NOTEXIST;
        }

        LevelUpData levelup_data;
        const RC::eSKILL_RESULT test_result = 
            skill_levelup_test(&levelup_data, &temp_skill_container, new_skill_info);
        if (test_result != RC::RC_SKILL_SUCCESS)
        {
            return test_result;
        }
        
        levelup_data_array.push_back(levelup_data);
    }

    // 필요한 스킬 포인트와 마지막 줄번호를 얻는다.
    uint require_total_skill_point = 0;
    WORD last_skill_line = 0;
    STLX_VECTOR<int> last_job_lines;
    for (int job_type = 0; job_type < skill_parser->GetJobSize(); ++job_type)
    {
        last_job_lines.push_back(0);
    }
    for (size_t i = 0; i < levelup_data_array.size(); ++i)
    {
        const LevelUpData& levelup_data = levelup_data_array[i];
        const SkillScriptInfo* const skill_info = 
            skill_parser->GetSkillInfo(levelup_data.new_skill_code);
        if (skill_info == NULL)
        {
            return RC::RC_SKILL_FAILED;
        }

        const RC::eSKILL_RESULT upper_skill_check_result = 
            check_enough_upper_skill_level(&temp_skill_container, skill_info);
        if (upper_skill_check_result != RC::RC_SKILL_SUCCESS)
        {
            return upper_skill_check_result;
        }
            
        require_total_skill_point += levelup_data.require_skill_point;
        
        const int job_line = levelup_data.job_line_index + 1;
        if (job_line > last_job_lines[levelup_data.job_type])
        {
            last_job_lines[levelup_data.job_type] = job_line;
            if (job_line > last_skill_line)
            {
                last_skill_line = static_cast<WORD>(job_line);
            }
        }
    }

    // 남아있는 스킬 포인트가 충분한지 검사한다.
    const int remain_skill_point = player_->GetCharInfo()->m_iRemainSkill;
    if (remain_skill_point < require_total_skill_point)
    {
        return RC::RC_SKILL_REMAIN_SKILLPOINT_LACK;
    }

    // 사용 가능한 줄인지 검사한다.
    const int char_type = player_->GetCharInfo()->m_byClassCode;
    if (inner_impl::IsEnableSkillJobLine<inner_impl::SkillLineTestMethod::kTestMethod>(
        char_type, last_skill_line, last_job_lines, temp_skill_container) == false)
    {
        return RC::RC_SKILL_DISABLE_SKILL_LINE;
    }
    
    // 직렬화 한다.
    const uint serialize_count = serialize_skill_array(
        total_stream, &temp_skill_container, skill_code_array, skill_code_count);
    if (serialize_count != skill_code_count)
    {
        return RC::RC_SKILL_FAILED;
    }

    // 실제 스킬을 적용한다.
    temp_skill_container.Copy(skill_container_);
    for (size_t i = 0; i < levelup_data_array.size(); ++i)
    {
        const LevelUpData& levelup_data = levelup_data_array[i];
        apply_skill_levelup(&levelup_data);
    }

#ifdef _NA_001990_ACSYSTEM_ADD
    // AC를 처리한다.
    player_->GetACManager()->ProcessSkillStepUp(last_skill_line);
#endif //_NA_001990_ACSYSTEM_ADD

    return RC::RC_SKILL_SUCCESS;
}

uint SkillSlotManager::serialize_skill_array(
    SKILL_TOTAL_INFO& OUT total_stream,
    SCSkillSlotContainer* skill_container,
    const SLOTCODE* skill_code_array, 
    const uint skill_code_count) const
{
    BOOST_STATIC_ASSERT(MAX_SKILL_SLOT_NUM == _countof(total_stream.m_Slot));
    total_stream.m_Count = 0; // CHANGES: f110215.5L
    SKILLSLOT* it = total_stream.m_Slot;

    for (uint i = 0; i < skill_code_count; ++i)
    {
        const SLOTCODE skill_code = skill_code_array[i];
        if (skill_code == 0)
        {
            continue;
        }
            
        const SCSkillSlot* const skill_slot = skill_container->GetSkillSlotByCode(skill_code);
        if (skill_slot == NULL)
        {
            continue;
        }

        it->m_Pos = skill_slot->GetPos();
        skill_container->SerializeSkillStream(skill_slot->GetPos(), &it->m_Stream, SERIALIZE_LOAD);
        ++it;
    }

    total_stream.m_Count = static_cast<POSTYPE>(it - total_stream.m_Slot);
    return total_stream.m_Count;
}

void SkillSlotManager::apply_skill_levelup(const LevelUpData* levelup_data)
{
    if (levelup_data->new_skill_code == 0)
    {
        return;
    }
        
    // 패시브 스킬이라면 이전스킬을 제거하고 새로운 스킬을 추가한다.
    // 이전 스킬 쿨타임 제거 및 새스킬 쿨타임 등록
    // QuickSlot에 스킬이 등록되어 있다면 레벨업 시켜준다.
    if (levelup_data->old_skill_code)
    {
        nsSlot::QuickSlotOriginalCodeChanger do_change_event(
            quick_container_, levelup_data->old_skill_code, levelup_data->new_skill_code);

        if (levelup_data->is_passive_skill)
        {
            RemovePassiveSkill(levelup_data->old_skill_code);
        }
        else
        {
            player_->FreeSkillCoolTime(levelup_data->old_skill_code);
        }
            
        // 소환체 스킬이라면 이전스킬을 제거하고 새로운 스킬을 추가한다.
        if (levelup_data->is_summon_skill)
        {
            SummonManager* const summon_manager = SummonManager::Instance();
            const DWORD summoner_key = player_->GetObjectKey(); // ... player key?
            summon_manager->DelSummonedSkill(summoner_key, levelup_data->old_skill_code);
        }
    }

    if (levelup_data->is_passive_skill)
    {
        AddAttrForPassiveSkill(levelup_data->new_skill_code);
    }
    else
    {
        player_->SetSkillCoolTime(levelup_data->new_skill_code, SKILL_COOLTIME_MARGIN);
    }
        
    if (levelup_data->is_summon_skill)
    {
        SummonManager* const summon_manager = SummonManager::Instance();
        const DWORD summoner_key = player_->GetObjectKey(); // ... player key?
        summon_manager->AddSummonedSkill(summoner_key, levelup_data->new_skill_code);
    }

    player_->GetCharInfo()->m_iRemainSkill -= levelup_data->require_skill_point;

    const bool is_levelup = (levelup_data->old_skill_code != 0) ? true : false;
    GAMELOG->WriteSkillLevelUp(player_, levelup_data->new_skill_code, is_levelup);
}


RC::eSKILL_RESULT SkillSlotManager::skill_levelup_test(
    LevelUpData* OUT levelup_data, 
    SCSkillSlotContainer* INOUT skill_container, 
    const SkillScriptInfo* new_skill_info) const
{
    uint require_point = 0;
    SLOTCODE cur_skill_code = 0;
    const SLOTCODE new_skill_code = new_skill_info->m_SkillCode;
    const SLOTCODE new_skill_class_code = new_skill_info->m_SkillClassCode;

    // 새 스킬이 스킬트리에 등록되어 있는지 검사한다.
    const eCHAR_TYPE character_class = (eCHAR_TYPE)player_->GetCharInfo()->m_byClassCode;
    const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();
    const SkillJobTree::Desc* const job_desc = 
        skill_parser->FindAllJobDesc(character_class, new_skill_class_code);
    if (job_desc == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "[%s][%d] can not find skill desc", __FUNCTION__, __LINE__);
        return RC::RC_SKILL_FAILED;
    }

    // 같은 종류의 스킬이라면
    const SCSkillSlot* const cur_skill_slot = 
        skill_container->GetSkillSlotByClass(new_skill_class_code);
    if (cur_skill_slot != NULL)
    {
        // 최대 레벨인지 검사한다.
        const SkillScriptInfo* const cur_skill_info = cur_skill_slot->GetSkillInfo();
        if (cur_skill_info == NULL)
        {
            return RC::RC_SKILL_BASEINFO_NOTEXIST;
        }
        if (cur_skill_info->m_wSkillLv >= MAX_SKILL_LEVEL)
        {
            return RC::RC_SKILL_MAX_LEVEL_LIMIT;
        }
            
        if (cur_skill_info->m_SkillCode == new_skill_code)
        {
            return RC::RC_SKILL_FAILED;
        }
            
        cur_skill_code = cur_skill_info->m_SkillCode;

        // 레벨업할 스킬의 필요 포인트를 계산한다.
        for (SLOTCODE skill_code = cur_skill_code + 1; skill_code <= new_skill_code; ++skill_code)
        {
            const SkillScriptInfo* const skill_info = skill_parser->GetSkillInfo(skill_code);
            if (skill_info == NULL)
            {
                return RC::RC_SKILL_DOES_NOT_HAVE;
            }
                
            if (skill_info->m_SkillClassCode != new_skill_class_code)
            {
                return RC::RC_SKILL_DOES_NOT_HAVE;
            }
                
            require_point += skill_info->m_byRequireSkillPoint;
        }

        const POSTYPE skill_position = cur_skill_slot->GetPos();
        SCSkillSlot new_skill_slot;
        new_skill_slot.SetCode(new_skill_code);
        new_skill_slot.SetPos(skill_position);

        if (!skill_container->DeleteSlot(skill_position, NULL))
        {
            return RC::RC_SKILL_POSITION_INVALID;
        }
            
        if (skill_container->InsertSlot(skill_position, new_skill_slot) != RC::RC_INSERT_SLOT_SUCCESS)
        {
            return RC::RC_SKILL_FAILED;
        }
    }
    else
    {
        POSTYPE skill_position = 0;
        if (!skill_container->GetEmptyPos(skill_position)) 
        {
            return RC::RC_SKILL_POSITION_INVALID;
        }

        // 추가할 스킬의 필요 포인트를 계산한다.
        for (SLOTCODE skill_code = job_desc->first_skill_code; 
            skill_code <= new_skill_code; ++skill_code)
        {
            const SkillScriptInfo* const skill_info = skill_parser->GetSkillInfo(skill_code);
            if (skill_info == NULL)
            {
                return RC::RC_SKILL_DOES_NOT_HAVE;
            }
                
            if (skill_info->m_SkillClassCode != new_skill_class_code)
            {
                return RC::RC_SKILL_DOES_NOT_HAVE;
            }
                
            require_point += skill_info->m_byRequireSkillPoint;
        }

        SCSkillSlot new_skill_slot;
        new_skill_slot.SetCode(new_skill_code);
        new_skill_slot.SetPos(skill_position);

        if (skill_container->InsertSlot(skill_position, new_skill_slot) != RC::RC_INSERT_SLOT_SUCCESS)
        {
            return RC::RC_SKILL_FAILED;
        }
    }

    levelup_data->new_skill_code = new_skill_code;
    levelup_data->old_skill_code = cur_skill_code;
    levelup_data->require_skill_point = require_point;
    levelup_data->skill_line = job_desc->line_index + 1;
    levelup_data->is_summon_skill = (new_skill_info->m_bySkillUserType == eSKILL_USER_SUMMONED) ? true : false;
    levelup_data->is_passive_skill = (new_skill_info->m_bySkillType == SKILL_TYPE_PASSIVE) ? true : false;
    levelup_data->job_type = job_desc->job_type;
    levelup_data->job_line_index = job_desc->line_index;

    return RC::RC_SKILL_SUCCESS;
}


class FindSkillCount
{
    SkillSlotManager* skill_slot_manager_;
    uint        skill_count_;
public:
    FindSkillCount(SkillSlotManager* skill_slot_manager)
        : skill_slot_manager_(skill_slot_manager), skill_count_(0) {} 

    void operator()(SCSlot* slot)
    {
        if (skill_slot_manager_ == NULL)
            return;
        SCSkillSlot* pSkillSlot = static_cast<SCSkillSlot*>(slot);
        if (pSkillSlot->GetSkillInfo()) // 스킬일 경우만 값이 있다.
            ++skill_count_;
    }

    uint get_skill_count() const
    {
        return skill_count_;
    }
};

void SkillSlotManager::UnLockFirstSkillLine()
{
#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
    // 첫 번째 줄의 스킬도 기본으로 배우지 않는다.
    return;
#else
    FindSkillCount Opr(this);
    skill_container_->ForEachSlot(Opr);
    if (Opr.get_skill_count() > 0)
        return;

    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    eCHAR_TYPE character_class = (eCHAR_TYPE)player_->GetCharInfo()->m_byClassCode;
    const SkillTree* skill_tree = skill_parser->get_skill_tree(character_class);
    if (skill_tree == NULL)
    {
        ASSERT(0);
        return;
    }

    // 원본 스킬 컨테이너를 보관해 놓는다.
    // 스킬의 레벨업시 임시 컨테이너에 적용시킨 후 모든 스킬 레벨업이 완료시 원본에 덮어쓴다.
    SCSkillSlotContainer temp_skill_container;
    skill_container_->Copy(&temp_skill_container);

    STLX_VECTOR<LevelUpData> levelup_data_array;
    levelup_data_array.clear();
    LevelUpData levelup_data;


    // Line 번호는 1번 부터 시작한다.
    const SkillTree::SkillDescript* skill_descript_array = skill_tree->get_skill_descript(1);
    if (skill_descript_array == NULL)
    {
        ASSERT(0);
        return;
    }

    const uint max_column = skill_tree->get_column_count();
    for (uint column = 0 ; column < max_column ; ++column)
    {
        const SkillTree::SkillDescript* skill_descript = &(skill_descript_array[column]);
        if (skill_descript->skill_class == 0)
            continue;

        const SkillScriptInfo* script_info = skill_parser->GetSkillInfo(skill_descript->first_skill_code);
        if (script_info == NULL)
        {
            ASSERT(0);
            return;
        }

        if (temp_skill_container.GetSkillSlotByClass(skill_descript->skill_class))
        {
            continue;
        }

        if (skill_descript->upper_skill_class != 0)
        {
            // 상위 스킬의 레벨 체크
            if (RC::RC_SKILL_SUCCESS != check_enough_upper_skill_level(&temp_skill_container, script_info))
                continue;
        }

        // 스킬 레벨업.
        if (RC::RC_SKILL_SUCCESS != skill_levelup_test(&levelup_data, &temp_skill_container, script_info))
            continue;

        levelup_data_array.push_back(levelup_data);
    }

    // 실제 스킬을 적용한다.
    temp_skill_container.Copy(skill_container_);
    uint count = levelup_data_array.size();
    for (uint idx = 0 ; idx < count ; ++idx)
    {
        const LevelUpData& levelup_data = levelup_data_array[idx];
        apply_skill_levelup(&levelup_data);
    }
#endif // _NA_004530_20120315_1202_SKILL_RENEWAL
}

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM

namespace inner_impl {
class UsedSkillPoint
{
public:
    UsedSkillPoint(const SkillSlotManager* skill_slot_manager) : 
        skill_slot_manager_(skill_slot_manager), used_skill_point_(0) 
    {
    } 

    void operator()(const SCSlot* slot)
    {
        if (skill_slot_manager_ == NULL)
        {
            return;
        }            
        const SCSkillSlot* const skill_slot = static_cast<const SCSkillSlot*>(slot);
        const SkillScriptInfo* const skill_info = skill_slot->GetSkillInfo();
        if (skill_info)
        {
            // 스킬 레벨을 낮추면서 각 레벨에 사용된 스킬포인트를 반납한다.
            // 사용된 스킬 포인트를 반납한다.
            SLOTCODE skill_code = skill_info->m_SkillCode;
            for (int level = skill_info->m_wSkillLv; level >= 0; --level, --skill_code)
            {
                const SkillScriptInfo* const temp_skill_info = 
                    SkillInfoParser::Instance()->GetSkillInfo(skill_code);
                if (temp_skill_info == NULL)
                {
                    continue;
                }
                used_skill_point_ += temp_skill_info->m_byRequireSkillPoint;
            }
        }
    }

    uint get_used_skill_point() const
    {
        return used_skill_point_;
    }

private:
    const SkillSlotManager* skill_slot_manager_;  
    uint used_skill_point_;
};
} // inner_impl

uint SkillSlotManager::get_used_skill_point() const
{
    inner_impl::UsedSkillPoint oper(this);
    skill_container_->ForEachSlot(oper);
    return oper.get_used_skill_point();
}
#endif //_NA_0_20100901_SKILLTREE_SYSTEM

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
const SLOTCODE SkillSlotManager::FindSkill( const eSKILL skill_class ) const
{
    SCSkillSlotContainer* skill_container = GetSkillSlotContainer();
    SCSkillSlot* skill_slot = skill_container->GetSkillSlotByClass(skill_class);

    if (skill_slot != NULL)
    {
        return skill_slot->GetSkillInfo()->m_SkillCode;
    }
    return 0;
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL