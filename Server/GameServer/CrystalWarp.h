#pragma once

class GameGuild;

class CrystalWarp : public NPC
{
public:
    CrystalWarp();
    virtual ~CrystalWarp();

public:
    void InitCrystalWarp(WORD NPCCode, DWORD dwGuildGuid, DWORD ExpireTime);
    VOID SetExpireTime(DWORD dwInterval);

    virtual VOID OnEnterObject(const Sector & IN rSector);
    virtual	VOID OnLeaveField();
    virtual VOID GetSummonedRenderInfo(SUMMONED_RENDER_INFO * OUT pRenderInfo);
    
    virtual BOOL Update(DWORD dwDeltaTick);
    virtual VOID Reuse() { Release(); }

    GUILDGUID GetSummonerGuildGuid() { return m_guildGuid; }

    Character*   SearchTarget() { return NULL; }
    virtual	VOID SetTargetChar(Character* pChar) { __UNUSED(pChar); return; }
    virtual BOOL CanResurrection(Character *pTarget) { __UNUSED(pTarget); return FALSE; }

    virtual eUSER_RELATION_TYPE	IsFriend(Character *pTarget);
    WzVector GetRandomPosInAround(INT iDist, int iLoop = 10);

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    virtual	VOID OnRecover( INT recoverHP, INT recoverMP, INT recoverSD = 0, eRECOVER_TYPE Type = eRECOVER_TYPE_NORMAL, Character* pAttacker = NULL ) {
        __UNUSED((recoverHP, recoverMP, recoverSD, Type, pAttacker));
    }

protected:
    virtual VOID Release();
    virtual	VOID OnDead();

private:
    util::ITimerBase m_pExpireTimer[1];		// 소환체 소멸타이머
    GUILDGUID    m_guildGuid;
};

