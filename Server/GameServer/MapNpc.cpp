#include "stdafx.h"
#include "MapNpc.h"
#include "GameField.h"
#include "Map.h"
#include "GameZone.h"
#include "GameRoom.h"
#include "AIParamParser.h"
#include "NPCStateManager.h"
#include "TargetManager.h"
#include "..\\.\\SolarTrigger\\TriggerMsg.h"
#include "TriggerManagerEx.h"
#include "GameDominationField.h"
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#include "GameGuildManager.h"
#include "GameGuild.h"
#include "GameGuildRelation.h"
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

MapNpc::MapNpc()
{
	SetObjectType( MAPNPC_OBJECT );
}

MapNpc::~MapNpc()
{

}

BOOL MapNpc::Init()
{
	return NPC::Init();
}

void MapNpc::InitMapNPC( WORD NPCCode, MAPOBJECT_INFO *pMapInfo )
{
	this->Init();
	this->SetMapObjectInfo( pMapInfo );
	m_LinkedAction = AppliedNpcDependencyChain::Instance()->GetNpcLinkAction( NPCCode, MAPNPC_OBJECT );
	NPC::SetBaseInfo( NPCCode, 0, 0 );
}

VOID MapNpc::Release()
{
	NPC::Release();
	m_LinkedAction = NULL;
}

BOOL MapNpc::Update( DWORD dwDeltaTick )
{
    // 지역점령전의 MAPNPC는 AI를 사용하지 않는다는 상황...
    // 모듈을 사용하고 안하고가 아닌, AI 알고리즘을 제어 했어야함.
    // AI 알고리즘중 이동성향:행동상태기계와 공격성향:타겟제어 
    // 두가지 알고리즘군에 신규상태 추가. 타겟을 잡지 않으며, 상태기계의 변화가 없는...

    // "mapnpc는 사라지지 않는다." 규칙은 이동성향(AI 알고리즘)에서 상태기계 제거 한다.
    // (eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK)
    
    // 현재 mapnpc는 기존에 몇가지 문제(현재는 어떤 문제였는지 정확히 파악 안됨)로 
    // 게임에서 빠져 있으며, 돌무더기로 칭해지는 부분과 지역점령전, 
    // 현재 적용 되지 않은 헬론의 성 필드 서브에서 사용된다. [5/5/2010 lst1024]
	return NPC::Update( dwDeltaTick );
}

VOID MapNpc::OnDead()
{
	// 경험치를 분배한다.
	// DistributeExp();

	NPC::OnDead();

	if( !GetField() )
        return;

	m_LinkedAction.OnDead( GetField(), this, NULL, m_dwKillerObjectKey );

    if (eNPC_DOMINATION_MAPOBJECT_NPC == m_pBaseInfo->m_byGrade)
    {
        Character* const pChr = GetField()->FindCharacter(m_dwKillerObjectKey);
        if (NULL != pChr)
        {
            Player* const pAttacker = pChr->GetPlayerOwner();
            GAMELOG->LogDomination(ACT_DOMINATION_DESTROY_OBJECT, pAttacker, this);
        }

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        GameDominationManager::Instance()->OnDeadDominationMapNPC(GetGameZonePtr()->GetMapCode(), 
                                                                  m_pBaseInfo->m_MonsterCode);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    }
}


void
MapNpc::_Damaged(DamageArgs* damage_arg)
{
    return NPC::_Damaged(damage_arg);
}

eUSER_RELATION_TYPE	MapNpc::IsFriend(Character* pTarget)
{
    if (obj_util::IsValid(pTarget, __FUNCTION__, __LINE__) == false) {
        return USER_RELATION_NEUTRALIST;
    }

    switch (m_pBaseInfo->m_byGrade)
    {
    case eNPC_DOMINATION_MAPOBJECT_NPC:
        return IsFriend_DominationMapNpc(pTarget);

    case eNPC_FRIEND_MAPOBJECT_NPC:
        return IsFriend_FriendMapNpc(pTarget);

    default:
        return NPC::IsFriend(pTarget);
    }

    return USER_RELATION_NEUTRALIST;
}

eUSER_RELATION_TYPE	MapNpc::IsFriend_FriendMapNpc(Character* pTarget)
{
    if (pTarget->IsEqualObjectKind(NPC_OBJECT))
    {
        NPC* const npc = static_cast<NPC*>(pTarget);
        const BYTE grade = npc->GetGrade();

#ifdef _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
        if (grade == eNPC_FRIEND_MONSTER || 
            grade == eNPC_FRIEND_MAPOBJECT_NPC)
#else
        if (grade == eNPC_FRIEND_MONSTER && 
            grade == eNPC_FRIEND_MAPOBJECT_NPC)
#endif // _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
        {
            return USER_RELATION_FRIEND;
        }
#ifdef _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
        else
        {
            return USER_RELATION_ENEMY;
        }
#endif // _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
    }
    else if (pTarget->IsEqualObjectKind(PLAYER_OBJECT))
    {
        return USER_RELATION_FRIEND;
    }
    else
    {
        return USER_RELATION_ENEMY;
    }

    return USER_RELATION_NEUTRALIST;    
}


eUSER_RELATION_TYPE	MapNpc::IsFriend_DominationMapNpc(Character* pTarget)
{
    if (pTarget->IsEqualObjectKind(PLAYER_OBJECT) == FALSE)
        return USER_RELATION_NEUTRALIST;

    Player* const player = static_cast<Player*>(pTarget);
    const GUILDGUID guild_guid = player->GetGuildGuid();

    if (guild_guid == INVALID_GUILDGUID || player->GetGameZoneState() != ePRS_AT_DOMINATION_FIELD) 
    {
        return USER_RELATION_NEUTRALIST;
    }

    GameDominationField* const zone = static_cast<GameDominationField*>(player->GetGameZonePtr());

    // added by _NA_000587_20100928_DOMINATION_BUFF_ITEM
    // 이렇게 추가되지 않으면 실제 아군이라도 언제나 중립 처리된다.
    if (guild_guid == zone->GetDefenseGuild())
    {
        return USER_RELATION_FRIEND;
    }
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL    
    GameGuild* const defense_game_guild = g_GameGuildManager.FindGuild(zone->GetDefenseGuild());
    if (defense_game_guild)
    {
        GuildRelationSystem::GuildRelationEntry* defense_guild_entry = \
            defense_game_guild->GetRelationEntry();
        if (defense_guild_entry == NULL)
        {
            return USER_RELATION_NEUTRALIST;
        }

        eGUILD_RELATION relation = defense_guild_entry->GetRelationType(guild_guid);
        if (relation == eGUILD_RELATION_ALLIENCE) {
            return USER_RELATION_FRIEND;
        }
    }
#else
    if (guild_guid != zone->GetOffenseGuild())
        return USER_RELATION_NEUTRALIST;
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
    return USER_RELATION_ENEMY;
}

