#include "stdafx.h"
#include "CrystalWarp.h"
#include "StatusManager.h"
#include "NPCStateManager.h"
#include "GameDominationField.h"

CrystalWarp::CrystalWarp()
: m_guildGuid(0)
{
    SetObjectType(CRYSTALWARP_OBJECT);
}

CrystalWarp::~CrystalWarp()
{
}


void CrystalWarp::InitCrystalWarp(WORD NPCCode, DWORD dwGuildGuid, DWORD ExpireTime)
{
    NPC::Init();
    NPC::SetBaseInfo(NPCCode, eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK, 0);
    Character::SetSummonerKey(0);

    m_guildGuid = dwGuildGuid;
    SetExpireTime(ExpireTime);
}

VOID CrystalWarp::Release()
{
    m_guildGuid = 0;
    m_pExpireTimer->Clear();

    Character::SetSummonerKey(0);
    NPC::Release();
}

BOOL CrystalWarp::Update(DWORD dwDeltaTick)
{
    if (IsAlive() && 
        m_pExpireTimer->IsExpired())
    {
        SetHP(0);       
        OnDead();       // 시간이 다되면 소멸된다.

        return FALSE;   // 삭제한다.
    }

    return NPC::Update(dwDeltaTick);
}

VOID CrystalWarp::SetExpireTime( DWORD dwInterval )
{
    m_pExpireTimer->SetTimer(dwInterval);

    if (dwInterval == 0) {
        m_pExpireTimer->Stop();
    };
}

VOID CrystalWarp::GetSummonedRenderInfo(SUMMONED_RENDER_INFO * OUT pRenderInfo)
{
    pRenderInfo->m_dwSummonerObjKey = m_guildGuid;
    pRenderInfo->m_byCurCommand     = eSUMMON_COMMAND_ATTACK;
    pRenderInfo->m_dwObjectKey		= GetObjectKey();
    pRenderInfo->m_dwMonsterCode	= GetBaseInfo()->m_MonsterCode;
    pRenderInfo->m_dwMaxHP			= GetMaxHP();
    pRenderInfo->m_dwHP				= GetHP();
    pRenderInfo->m_wMoveSpeedRatio	= (WORD)m_Attr.GetMoveSpeedRatio();
    pRenderInfo->m_wAttSpeedRatio	= m_Attr.GetAttSpeedRatio();
    pRenderInfo->m_bySummonType     = eSUMMON_CRYSTALWARP;

    GetPos(&pRenderInfo->m_wvPos);

    // STATE_INFO를 채운다.
    GetStatusManager()->SerialzieStatusInfo(pRenderInfo->m_StateTotalInfo);
}

VOID CrystalWarp::OnDead()
{
    GameDominationField* pGameDominationField = (GameDominationField*)GetGameZonePtr();
    pGameDominationField->OnDeadCrystalWarp(GetObjectKey());
    
    NPC::OnDead();

    Character* const pChr = GetField()->FindCharacter(m_dwKillerObjectKey);
    if (NULL != pChr)
    {
        Player* const pAttacker = pChr->GetPlayerOwner();
        GAMELOG->LogDomination(ACT_DOMINATION_CRYSTALWARP_DEAD, pAttacker, this);
    }
}


eUSER_RELATION_TYPE	CrystalWarp::IsFriend(Character* pTarget)
{
    if (FALSE == pTarget->IsEqualObjectKind(PLAYER_OBJECT)) 
        return USER_RELATION_NEUTRALIST;
    
    Player* pPlayer = (Player*)pTarget;

    if (pPlayer->GetGuildGuid() != m_guildGuid)
        return USER_RELATION_ENEMY;

    return USER_RELATION_NEUTRALIST;
}

WzVector CrystalWarp::GetRandomPosInAround(INT iDist, int iLoop)
{
    WzVector vCurPos;
    GetPos(&vCurPos);

    if (!GetField())
    {
        SUNLOG( eCRITICAL_LOG, "[CrystalWarp::GetRandomPosInAround] Field is NULL!" );
        return vCurPos;
    }

    WzVector vDestPos;
    if (!GetField()->GetRandomPosInAroundCheckPath(this, MAX_TILE_NODE_CNT_IN_DOMINATION,
        &vCurPos, iDist, vDestPos, iLoop))
        return vCurPos;

    return vDestPos;
}
