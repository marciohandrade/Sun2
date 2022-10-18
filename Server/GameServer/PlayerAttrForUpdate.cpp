#include "stdafx.h"
#include "PlayerAttrForUpdate.h"
//
#include "Player.h"
#include "PacketStruct_AG.h"

#include "SkillSystem/CSSyncTuner.h"

void PlayerAttrForUpdate::SetOwner(IPlayerAttributeLinker* linker)
{
    ns_object::PlayerAttributeLinker* player_linker = \
        DOWNCASTER_SELECT<ns_object::PlayerAttributeLinker*>(linker);
    assert(player_linker);
    PlayerAttributes::SetOwner(player_linker);
    player_ = player_linker->GetOwner();
}

// 공속, 이속, 최대 HP, MP 변화시 클라에 알려주기 위해 필요한 부분이다.
void PlayerAttrForUpdate::UpdateEx()
{
    const int attack_speed_ratio = player_->GetAttSpeedRatio();
    const int move_speed_ratio = player_->GetMoveSpeedRatio();
    const int max_hp = player_->GetMaxHP();
    const int max_mp = player_->GetMaxMP();
    const int max_sd = player_->GetMaxSD();
    const int vitality = player_->GetVitality();
    const int spirit = player_->GetSpirit();
    //
    PlayerAttributes::Update();
    //
    const int changed_attack_speed_ratio = player_->GetAttSpeedRatio();
    const int changed_move_speed_ratio = player_->GetMoveSpeedRatio();
    if (attack_speed_ratio != changed_attack_speed_ratio) {
        player_->SendAttrChange(eATTR_ATTACK_SPEED, changed_attack_speed_ratio);
    };
    if (move_speed_ratio != changed_move_speed_ratio) {
        player_->SendAttrChange(eATTR_MOVE_SPEED, changed_move_speed_ratio);
    };

    // (CHANGES) (100107) change block
    // (BUGFIX) (100107) fix up invalid state that hp is grater than max hp
    const int changed_max_hp = player_->GetMaxHP();
    const int changed_max_mp = player_->GetMaxMP();
    const int changed_max_sd = player_->GetMaxSD();

    if (max_hp != changed_max_hp)
    {
        player_->SendAttrChange(eATTR_MAX_HP, changed_max_hp);
        player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedHP);
    }

    if (max_mp != changed_max_mp)
    {
        player_->SendAttrChange(eATTR_MAX_MP, changed_max_mp);
        player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedMP);
    }

    if (max_sd != changed_max_sd)
    {
        player_->SendAttrChange(eATTR_MAX_SD, changed_max_sd);
        player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedSD);
        player_->UpdateCalcRecover(false, false, true);
    }

    const int changed_vitality = player_->GetVitality();
    const int changed_spirit = player_->GetSpirit();
    if (vitality != changed_vitality) {
        player_->UpdateCalcRecover(true, false, false);
    };
    if (spirit != changed_spirit) {
        player_->UpdateCalcRecover(false, true, false);
    }
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    player_->GetSyncTuner()->OnChangeAttribute();
#endif
}


