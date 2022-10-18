#include "StdAfx.h"
#include "NewGameParty.h"
#include "Player.h"

NewGameParty::NewGameParty()
{
}

NewGameParty::~NewGameParty()
{
}

void
NewGameParty::Init(WORD partyKey, DWORD masterKey, ePARTY_TYPE party_type)
{
    BaseParty::Init(partyKey, masterKey, party_type);

    total_level_ = 0;
    update_timer_.SetTimer(UpdateTimerInterval);
    member_queue_.clear();
}

void
NewGameParty::Release()
{
    BaseParty::Release();
}

RC::ePARTY_RESULT
NewGameParty::Join(const _PARTY_MEMBER_INFO& member_info)
{
    RC::ePARTY_RESULT rcResult = BaseParty::Join(member_info);
    if (rcResult != RC::RC_PARTY_SUCCESS)
    {
        return rcResult;
    }

    total_level_ += member_info.m_wLevel;
    MemberNode node = { 0, };

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    node.player_key_ = member_info.m_dwMemberKey;
    member_queue_.push_back(node);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(member_info.m_dwMemberKey);
    if (player)
    {
        if (GetType() == ePARTY_ROOM)
        {
            NotifyMembersInfoToPlayer(player);
            NotifyPlayerInfoToPartyMember(player);
        }
    }

    return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
NewGameParty::Leave(DWORD memberKey)
{
    LEVELTYPE minusLevel = 0;
    const _PARTY_MEMBER_INFO* pMemberInfo = GetMemberInfo(memberKey);
    if(pMemberInfo)
        minusLevel = pMemberInfo->m_wLevel;

    RC::ePARTY_RESULT rcResult = BaseParty::Leave(memberKey);
    if(rcResult != RC::RC_PARTY_SUCCESS)
        return rcResult;

    if(total_level_ >= minusLevel)
        total_level_ -= minusLevel;
    else
        total_level_ = 0;

    PartyMemberQueue::iterator it = member_queue_.begin(),
                                 end = member_queue_.end();
    BOOLEAN isFinded = FALSE;
    while(it != end)
    {
        const MemberNode& node = *it;
        if(node.player_key_ == memberKey)
        {
            isFinded = TRUE;
            member_queue_.erase(it);
            break;
        }
        ++it;
    }

    if(isFinded == 0)
    {
        SUNLOG(eCRITICAL_LOG,
               "[NewGameParty::Leave] Can't Find memberKey[%u] At m_MemberQueue[Size:%u]",
               memberKey, member_queue_.size());
    }

	return RC::RC_PARTY_SUCCESS;
}

void
NewGameParty::MemberLevelUp(DWORD memberKey)
{
    const _PARTY_MEMBER_INFO* pMemberInfo = GetMemberInfo(memberKey);

    if(pMemberInfo)
        ++total_level_;
}

DWORD
NewGameParty::GetFIFOMemberKey()
{
    if(member_queue_.empty())
        return 0;

    const MemberNode& node = member_queue_.front();
    const DWORD memberKey = node.player_key_;
    member_queue_.splice(member_queue_.end(), member_queue_, member_queue_.begin());

    return memberKey;
}

// changed by __NA001336_090514_PARTY_NAVI_SUPPORT__ {
DWORD
NewGameParty::GetRandomMemberKey(eCHAR_TYPE classType)
{
    BOOLEAN isClassFilter = classType < eCHAR_TYPE_MAX;
    if(isClassFilter == 0)
    {
        PartyMemberQueue::size_type pos = member_queue_.size();
        pos = random(0, pos - 1);
        PartyMemberQueue::const_iterator where = member_queue_.begin();
        std::advance(where, pos);
        const MemberNode& node = *where;
        return node.player_key_;
    }
    else
    {
        BYTE number_of_same_class = 0;
        DWORD filtered_members[MAX_PARTYMEMBER_NUM];
        MEMBER_MAP::iterator it = party_member_map_.begin(),
                             end = party_member_map_.end();
        for( ; it != end; ++it)
        {
            const _PARTY_MEMBER_INFO& memberInfo = it->second;
            if(memberInfo.m_byClassType == classType)
                filtered_members[number_of_same_class++] = memberInfo.m_dwMemberKey;
            if(number_of_same_class == MAX_PARTYMEMBER_NUM)
                break; // overflow protector
        }

        if(number_of_same_class != 0)
        {
            int pos = 0;
            if(number_of_same_class != 1)
                pos = random(0, number_of_same_class - 1);
            return filtered_members[pos];
        }
        return GetRandomMemberKey(eCHAR_TYPE_MAX);
    }
}

void
NewGameParty::Update()
{
    if(update_timer_.IsExpired() == 0)
        return;

    int number_of_members = int(member_queue_.size());
    if(number_of_members == 0)
        return;

    MSG_AG_PARTY_NAVI_INFO_BRD msgNavi;
    msgNavi.m_CodeType = msgNavi.OPTION_PARTY_NAVI;
    msgNavi.m_NumberOfNodes = 0;

    MSG_AG_STATUS_CHAR_STAT_CHANGE_CMD msgHpMp;
    msgHpMp.m_NumberOfNodes = 0;
#ifdef _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
    msgHpMp.user_count_ = 0;
#endif //_NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE

    Player* pPolePlayer = NULL;
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    Player* brd_player = NULL;

    FOREACH_CONTAINER(MemberNode& member, member_queue_, PartyMemberQueue)
    {
        //_NA_20110630_BATTLE_ZONE_RENEWAL
        pPolePlayer = PlayerManager::Instance()->FindPlayerByObjKey(member.player_key_);
        if (pPolePlayer == NULL)
        {
            continue;
        }
        else
        {
            brd_player = pPolePlayer;
        }

        GameField* pGameField = pPolePlayer->GetField();
        if(pGameField == 0)
            continue;

        PlayerStatEvent::eFlag flag = pPolePlayer->CheckChangedStatus(PlayerStatEvent::eFlag_None);
        if ((flag & PlayerStatEvent::eFlag_ChangedPos))
        {
            MSG_AG_PARTY_NAVI_INFO_BRD::Node& node = msgNavi.m_Nodes[msgNavi.m_NumberOfNodes++];
            node.fieldCode = pGameField->GetFieldCode(); // assert(GetField() != 0)
            node.objectKey = WORD(member.player_key_);
            const WzVector* pPos = pPolePlayer->GetPosPtr();
            node.posX = WORD(pPos->x);
            node.posY = WORD(pPos->y);
            node.posZ = WORD(pPos->z);
        }

        typedef MSG_AG_STATUS_CHAR_STAT_CHANGE_CMD::uNode StatChangeNode;
        DWORD curHP = pPolePlayer->GetHP(),
              maxHP = pPolePlayer->GetMaxHP(),
              reserveHP = pPolePlayer->GetReserveHP(),
              curMP = pPolePlayer->GetMP(),
              maxMP = pPolePlayer->GetMaxMP(),
              curSD = pPolePlayer->GetSD(),
              maxSD = pPolePlayer->GetMaxSD();
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        PLAYERFP curFP = pPolePlayer->GetFP(),
                 maxFP = pPolePlayer->GetMaxFP();
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        // (CHANGES) (f100514.2L) (WAVERIX) fix up a loss which an original hp value over-value state
        // is possible if an old state is removed to replace a new state.
        // reference (BUGFIX) (100107) fix up invalid state that hp is grater than max hp.
        // changes to pending process related to HP, MP, SD events.
        // (WARNING) the contents of the Reserved HP has problems if a hp is grater than a max hp.
        if (curHP > maxHP) {
            curHP = maxHP;
            pPolePlayer->SetHP(curHP);
            flag = static_cast<PlayerStatEvent::eFlag>(flag | PlayerStatEvent::eFlag_ChangedHP);
        }
        if (curMP > maxMP) {
            curMP = maxMP;
            pPolePlayer->SetMP(curMP);
            flag = static_cast<PlayerStatEvent::eFlag>(flag | PlayerStatEvent::eFlag_ChangedMP);
        }
        if (curSD > maxSD) {
            curSD = maxSD;
            pPolePlayer->SetSD(curSD);
            flag = static_cast<PlayerStatEvent::eFlag>(flag | PlayerStatEvent::eFlag_ChangedSD);
        }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if (curFP > maxFP)
        {
            curFP = maxFP;
            pPolePlayer->SetFP(curFP);
            flag = static_cast<PlayerStatEvent::eFlag>(flag | PlayerStatEvent::eFlag_ChangedFP);
        }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        if((flag & PlayerStatEvent::eFlag_ChangedHP) ||
           (member.current_hp_ != curHP || member.max_hp_ != maxHP || member.reserve_hp_ != reserveHP))
        {
            StatChangeNode& node = msgHpMp.m_Nodes[msgHpMp.m_NumberOfNodes++];
            node.m_HP.event = msgHpMp.eEvent_ChangedHP;
            node.m_HP.objectKey = WORD(member.player_key_);
            node.m_HP.maxHP = maxHP;
            node.m_HP.currentHP = curHP;
            node.m_HP.reservedHP = reserveHP;

            member.current_hp_ = curHP; member.max_hp_ = maxHP; member.reserve_hp_ = reserveHP;
        }
        if((flag & PlayerStatEvent::eFlag_ChangedMP) ||
           (member.current_mp_ != curMP || member.max_mp_ != maxMP))
        {
            StatChangeNode& node = msgHpMp.m_Nodes[msgHpMp.m_NumberOfNodes++];
            node.m_MP.event = msgHpMp.eEvent_ChangedMP;
            node.m_MP.objectKey = WORD(member.player_key_);
            node.m_MP.maxMP = maxMP;
            node.m_MP.currentMP = curMP;

            member.current_mp_ = curMP; member.max_mp_ = maxMP;
        }
        if((flag & PlayerStatEvent::eFlag_ChangedSD) ||
            (member.current_sd_ != curSD || member.max_sd_ != maxSD))
        {
            StatChangeNode& node = msgHpMp.m_Nodes[msgHpMp.m_NumberOfNodes++];
            node.m_SD.event     = msgHpMp.eEvent_ChangedSD;
            node.m_SD.objectKey = WORD(member.player_key_);
            node.m_SD.maxSD     = maxSD;
            node.m_SD.currentSD = curSD;

            member.current_sd_ = curSD; 
            member.max_sd_ = maxSD;
        }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if ((flag & PlayerStatEvent::eFlag_ChangedFP) ||
            (member.current_fp_ != curFP))
        {
            StatChangeNode& node = msgHpMp.m_Nodes[msgHpMp.m_NumberOfNodes++];
            node.m_FP.event = msgHpMp.eEvent_ChangedFP;
            node.m_FP.objectKey = WORD(member.player_key_);
            node.m_FP.currentFP = curFP;

            member.current_fp_ = curFP;
            member.max_fp_ = maxFP;
        }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

#ifdef _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
        ++msgHpMp.user_count_;
#endif //_NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
        pPolePlayer->NotifyChangedStatus(PlayerStatEvent::eFlag_None);
    }

    // _NA_20110630_BATTLE_ZONE_RENEWAL
    if (brd_player == NULL)
    {
        return;
    }

    if(msgNavi.m_NumberOfNodes != 0)
    {
        brd_player->SendPacket(&msgNavi, msgNavi.GetSize());
    }

    if(msgHpMp.m_NumberOfNodes != 0)
    {
        brd_player->SendPacket(&msgHpMp, msgHpMp.GetSize());
    }
}

// _NA_20110630_BATTLE_ZONE_RENEWAL
void 
NewGameParty::SendPlayerRenderInfo(Player* request_player, Player* target_player)const
{
    VarMsg2* enter_msg = &(Player::m_VarMsg2[Player::SENDINFO_ENTER_OBJECT]);

    MSG_CG_PARTY_JOIN_ROOM_BRD* brd_msg = 
        enter_msg->StartTypePtr(Param2Class<MSG_CG_PARTY_JOIN_ROOM_BRD>());
    enter_msg->SetSize(brd_msg->GetSize());

    PLAYER_RENDER_INFO_EX* render_info = (PLAYER_RENDER_INFO_EX*)enter_msg->NextPtr();
    int packed_len = target_player->GetPlayerRenderInfo(false, render_info);
    enter_msg->SetSize((WORD)packed_len);

    ETC_STATE_TOTAL_INFO* state_info = (ETC_STATE_TOTAL_INFO*)enter_msg->NextPtr();
    target_player->GetEtcStateTotalInfo(state_info);
    enter_msg->SetSize(state_info->GetSize());

    const DWORD object_key = target_player->GetObjectKey();
    EQUIP_ITEM_BASEINFO* equip_info = (EQUIP_ITEM_BASEINFO*)enter_msg->NextPtr();
    equip_info->m_dwObjectKey = object_key;

    EQUIP_ITEM_BASEINFO::data_type* item_slot = 
        (EQUIP_ITEM_BASEINFO::data_type*)((BYTE*)equip_info + sizeof(EQUIP_ITEM_BASEINFO));
    equip_info->m_Count = target_player->SerializeRenderEquipItemBaseInfo(item_slot);
    enter_msg->SetSize(equip_info->GetSize());

    sMSG_RENDER_EFFECT_ITEM_LIST* const render_effect = 
        (sMSG_RENDER_EFFECT_ITEM_LIST*)enter_msg->NextPtr();
    render_effect->m_PlayerKey = object_key;
    DWORD render_item_count;
    render_item_count = target_player->RENDER_EFFECT.SerializeRenderInfo(
                                                render_effect->m_BitRender, 
                                                render_effect->m_ItemCodes, 
                                                sMSG_RENDER_EFFECT_ITEM_LIST::MAX_EFFECT_ITEMS);
    render_effect->m_ItemCount = static_cast<BYTE>(render_item_count);
    enter_msg->SetSize(render_effect->GetSize());

    request_player->SendPacket((MSG_BASE_FORWARD*)*enter_msg->GetMsgPPtr(), *enter_msg->GetMsgSizePtr(), TRUE);
}

void 
NewGameParty::NotifyPlayerInfoToPartyMember(Player* player)
{
    VarMsg2* enter_msg = &(Player::m_VarMsg2[Player::SENDINFO_ENTER_OBJECT]);
    
    MSG_CG_PARTY_JOIN_ROOM_BRD* brd_msg = 
        enter_msg->StartTypePtr(Param2Class<MSG_CG_PARTY_JOIN_ROOM_BRD>());
    enter_msg->SetSize(brd_msg->GetSize());

    PLAYER_RENDER_INFO_EX* render_info = (PLAYER_RENDER_INFO_EX*)enter_msg->NextPtr();
    int packed_len = player->GetPlayerRenderInfo(false, render_info);
    enter_msg->SetSize((WORD)packed_len);

    ETC_STATE_TOTAL_INFO* state_info = (ETC_STATE_TOTAL_INFO*)enter_msg->NextPtr();
    player->GetEtcStateTotalInfo(state_info);
    enter_msg->SetSize(state_info->GetSize());

    const DWORD object_key = player->GetObjectKey();
    EQUIP_ITEM_BASEINFO* equip_info = (EQUIP_ITEM_BASEINFO*)enter_msg->NextPtr();
    equip_info->m_dwObjectKey = object_key;

    EQUIP_ITEM_BASEINFO::data_type* item_slot = 
        (EQUIP_ITEM_BASEINFO::data_type*)((BYTE*)equip_info + sizeof(EQUIP_ITEM_BASEINFO));
    equip_info->m_Count = player->SerializeRenderEquipItemBaseInfo(item_slot);
    enter_msg->SetSize(equip_info->GetSize());

    sMSG_RENDER_EFFECT_ITEM_LIST* const render_effect = (sMSG_RENDER_EFFECT_ITEM_LIST*)enter_msg->NextPtr();
    render_effect->m_PlayerKey = object_key;
    DWORD render_item_count;
    render_item_count = player->RENDER_EFFECT.SerializeRenderInfo(
                                        render_effect->m_BitRender, 
                                        render_effect->m_ItemCodes, 
                                        sMSG_RENDER_EFFECT_ITEM_LIST::MAX_EFFECT_ITEMS);
    render_effect->m_ItemCount = static_cast<BYTE>(render_item_count);
    enter_msg->SetSize(render_effect->GetSize());

    PlayerManager* const player_manager = PlayerManager::Instance();
    MEMBER_MAP::const_iterator begin_iterator = party_member_map_.begin(),
                               end_iterator = party_member_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const _PARTY_MEMBER_INFO& member_info = begin_iterator->second;
        Player* const party_player = player_manager->FindPlayerByObjKey(member_info.m_dwMemberKey);
        if(party_player == NULL || player == party_player)
        {
            continue;
        }

        party_player->SendPacket((MSG_BASE_FORWARD*)*enter_msg->GetMsgPPtr(), *enter_msg->GetMsgSizePtr(), TRUE);
    }
}

void
NewGameParty::NotifyMembersInfoToPlayer(Player* player)const
{
    VarMsg2* const render_msg = &(Player::m_VarMsg2[Player::SENDINFO_PLAYER_RENDER]);
    VarMsg2* const equip_msg = &(Player::m_VarMsg2[Player::SENDINFO_PLAYER_EQUIP]);
    VarMsg2* const special_render_msg = &(Player::m_VarMsg2[Player::SENDINFO_PLAYER_SPECIAL_RENDER]);

    MSG_CG_PARTY_ALLPLAYERS_CMD* render_info = NULL;
    render_info = render_msg->StartTypePtr(Param2Class<MSG_CG_PARTY_ALLPLAYERS_CMD>());
    render_msg->SetSize(render_info->GetSize());

    MSG_CG_PARTY_ALLPLAYERS_EQUIPINFO_CMD* equip_info = NULL;
    equip_info = equip_msg->StartTypePtr(Param2Class<MSG_CG_PARTY_ALLPLAYERS_EQUIPINFO_CMD>());
    equip_msg->SetSize(equip_info->GetSize());

    MSG_CG_PARTY_RENDER_EFFECT_ITEM_LIST_CMD* special_render_info = NULL;
    special_render_info = special_render_msg->StartTypePtr(Param2Class<MSG_CG_PARTY_RENDER_EFFECT_ITEM_LIST_CMD>());
    special_render_msg->SetSize(special_render_info->GetSize());

    BYTE count = 0;
    const BYTE MaxCount = MSG_CG_PARTY_ALLPLAYERS_CMD::MAX_PLAYER_COUNT;
    PlayerManager* const player_manager = PlayerManager::Instance();

    MEMBER_MAP::const_iterator begin_iterator = party_member_map_.begin(),
                               end_iterator = party_member_map_.end();

    for (; begin_iterator != end_iterator && count < MaxCount; ++begin_iterator)
    {
        const _PARTY_MEMBER_INFO& member_info = begin_iterator->second;
        Player* party_player = player_manager->FindPlayerByObjKey(member_info.m_dwMemberKey);
        if (party_player == NULL || player == party_player)
        {
            continue;
        }

        const DWORD player_key = member_info.m_dwMemberKey;

        PLAYER_RENDER_INFO_EX* pack_info = (PLAYER_RENDER_INFO_EX*)render_msg->NextPtr();
        int packed_len = party_player->GetPlayerRenderInfo(false, pack_info);
        render_msg->SetSize((WORD)packed_len);

        ETC_STATE_TOTAL_INFO* state_total_info = (ETC_STATE_TOTAL_INFO*)render_msg->NextPtr();
        party_player->GetEtcStateTotalInfo(state_total_info);
        render_msg->SetSize(state_total_info->GetSize());

        EQUIP_ITEM_BASEINFO* equip_item_info = (EQUIP_ITEM_BASEINFO*)equip_msg->NextPtr();
        equip_item_info->m_dwObjectKey = player_key;

        EQUIP_ITEM_BASEINFO::data_type* item_slot;
        item_slot = (EQUIP_ITEM_BASEINFO::data_type*)((BYTE*)equip_item_info + sizeof(EQUIP_ITEM_BASEINFO));
        equip_item_info->m_Count = party_player->SerializeRenderEquipItemBaseInfo(item_slot);
        equip_msg->SetSize(equip_item_info->GetSize());

        sMSG_RENDER_EFFECT_ITEM_LIST* pEffectNode;
        pEffectNode = (sMSG_RENDER_EFFECT_ITEM_LIST*)special_render_msg->NextPtr();
        pEffectNode->m_PlayerKey = player_key;
        DWORD alignedCount;
        alignedCount = party_player->RENDER_EFFECT.SerializeRenderInfo(
                                               pEffectNode->m_BitRender, 
                                               pEffectNode->m_ItemCodes, 
                                               sMSG_RENDER_EFFECT_ITEM_LIST::MAX_EFFECT_ITEMS);
        pEffectNode->m_ItemCount = (BYTE)alignedCount;
        special_render_msg->SetSize(pEffectNode->GetSize());
        ++count;
    }

    if(count)
    {
        render_info->m_byCount = count;
        player->SendPacket((MSG_BASE_FORWARD*)*render_msg->GetMsgPPtr(), 
                           *render_msg->GetMsgSizePtr(), TRUE);
        
        equip_info->m_byCount = count;
        player->SendPacket((MSG_BASE_FORWARD*)*equip_msg->GetMsgPPtr(), 
                           *equip_msg->GetMsgSizePtr(), TRUE);

        special_render_info->m_Count = count;
        player->SendPacket((MSG_BASE_FORWARD*)*special_render_msg->GetMsgPPtr(), 
                           *special_render_msg->GetMsgSizePtr(), TRUE);
    }
}

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
DWORD NewGameParty::GetDicecastResultKey(Player* player, DicecastResultArray& result)
{
    _DICECAST_RESULT winner;
    PlayerManager* const player_manager = PlayerManager::Instance();
    GameField* pOwnerField = player->GetField();
    if (pOwnerField == NULL)
        return 0;

    MEMBER_MAP::const_iterator it = party_member_map_.begin(),
                               end = party_member_map_.end();
    for (int count_of_members = 0;
         count_of_members < _countof(result) && it != end;
         ++it)
    {
        const _PARTY_MEMBER_INFO& memberInfo = it->second;

        Player* const party_player = player_manager->FindPlayerByObjKey(memberInfo.m_dwMemberKey);
        if(party_player == NULL || party_player->IsDead()) {
            continue;
        }

        GameField* pMemberField	= party_player->GetField();
        if ((pMemberField == NULL) || (pOwnerField != pMemberField)) {
            continue;
        };
        
        _DICECAST_RESULT this_member;
        this_member.member_key = memberInfo.m_dwMemberKey;
        //
        do {
            this_member.random_num = random(1, 99);
        } while(this_member.random_num == winner.random_num); // 1등만 안겹치면 OK...
        //
       
        result[count_of_members++] = this_member;

        if (winner.random_num < this_member.random_num) {
            winner = this_member;
        };
    }

    return winner.member_key;
}
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING