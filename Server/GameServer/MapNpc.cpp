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
    // ������������ MAPNPC�� AI�� ������� �ʴ´ٴ� ��Ȳ...
    // ����� ����ϰ� ���ϰ� �ƴ�, AI �˰����� ���� �߾����.
    // AI �˰����� �̵�����:�ൿ���±��� ���ݼ���:Ÿ������ 
    // �ΰ��� �˰��򱺿� �űԻ��� �߰�. Ÿ���� ���� ������, ���±���� ��ȭ�� ����...

    // "mapnpc�� ������� �ʴ´�." ��Ģ�� �̵�����(AI �˰���)���� ���±�� ���� �Ѵ�.
    // (eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK)
    
    // ���� mapnpc�� ������ ��� ����(����� � ���������� ��Ȯ�� �ľ� �ȵ�)�� 
    // ���ӿ��� ���� ������, ��������� Ī������ �κа� ����������, 
    // ���� ���� ���� ���� ����� �� �ʵ� ���꿡�� ���ȴ�. [5/5/2010 lst1024]
	return NPC::Update( dwDeltaTick );
}

VOID MapNpc::OnDead()
{
	// ����ġ�� �й��Ѵ�.
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
    // �̷��� �߰����� ������ ���� �Ʊ��̶� ������ �߸� ó���ȴ�.
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

