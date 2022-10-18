#include "StdAfx.h"
#include ".\gamezone.h"
#include ".\Map.h"
#include ".\MapAllocator.h"
#include ".\GameField.h"
#include ".\Player.h"
#include <MapInfoParser.h>
#include ".\ResrcManager.h"
#include "MovementChecker.h"
#include "GameServerEx.h"
#include "Player.h"
#include "TriggerManagerEx.h"
#include "GameBattleResult.h" //_NA_0_20091118_GAMEROOM_BATTLE_RESULT

GameZone::GameZone(void)
    : m_MapCode(0)
    , m_Key(0)
    , m_NumberOfExpectedPlayers(0) //__NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__
    , m_NumberOfExpectedPlayersPrev(0)
    , m_bCreateTrigger(FALSE)
    , m_pBattleResult(NULL) //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
{
}

GameZone::~GameZone(void)
{
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    SAFE_DELETE(m_pBattleResult);
}

VOID
GameZone::Release()
{
    // 자식클래스에서 먼저 자료를 처리하기 위해
    OnDestroy();
    m_NumberOfExpectedPlayers = 0;
    m_NumberOfExpectedPlayersPrev = 0;

    Player* pPlayer = NULL;
    _PLAYER_HASH::iterator it = m_PlayerHash.begin(), end = m_PlayerHash.end();
    while(it != end)
    {
        pPlayer = it->second;
        if (obj_util::IsValid(pPlayer, __FUNCTION__, __LINE__))
        {
            pPlayer->SetField(NULL);
            pPlayer->SetGameZone(ePRS_NOT_AT_ZONE, NULL, 0);
        }
        ++it;
    }
    m_PlayerHash.clear();

    _GAMEFIELD_HASH::iterator it2 = m_GameFieldHash.begin(), end2 = m_GameFieldHash.end();
    while(it2 != end2)
    {
        (it2->second)->Release();
        GameField::FREE(it2->second);
        ++it2;
    }
    m_GameFieldHash.clear();
}

void
GameZone::Init(ZONEKEY zoneKey, MAPCODE mapCode, eZONE_RULESET::eRules rules)
{
    if(m_PlayerHash.empty() == false)
        SUNLOG(eCRITICAL_LOG, "[Map:%u],초기화시PlayerNum[%u] != 0", mapCode, m_PlayerHash.size());

    // 룸키 셋팅
    SetKey(zoneKey);
    SetMapCode(mapCode);

    m_pMAPINFO = const_cast<sMAPINFO*>(MapInfoParser::Instance()->FindMapInfo(mapCode));
    ASSERT(m_pMAPINFO);

    const eZONE_RULESET& rRulesetSrc = m_pMAPINFO->Ruleset;
    eZONE_RULESET& rRuleset = const_cast<eZONE_RULESET&>(Ruleset);
    if(rules == rRuleset.ZONE_RULE_EMPTY)
        rRuleset.rules = rRulesetSrc.rules;
    else
        rRuleset.rules = rules;
    m_NumberOfExpectedPlayers = 0;
    m_NumberOfExpectedPlayersPrev = 0;
}

void
GameZone::OnCreate()
{
    if(FALSE == FlowControl::FCAssert(m_pMAPINFO))
        return;

    // (WAVERIX) 재 작성 이유? 별 의미는 없다. 생각 정리 차원에서 무심코... 이건 병이다.

    for(INT idx=0 ; idx<sMAPINFO::MAX_FIELD_NUM ; ++idx)
    {
        const MAPCODE mcMapCode = m_pMAPINFO->FCode[idx];
        if(mcMapCode == 0)
            continue;

        const FIELDCODE fcWMOCode = m_pMAPINFO->FWMOCode[idx];
        const FIELDCODE fcWUTCode = m_pMAPINFO->FWUTCode[idx];
        ASSERT(0 != fcWMOCode);

        FieldInfo* const pFieldInfo = g_ResrcMgr.FindFieldInfo(mcMapCode, fcWMOCode);
        UnitRegenInfoManager* const pRegenInfo = g_ResrcMgr.FindRegenInfoMgr(mcMapCode, fcWUTCode);
        UnitGroupInfoManager* const pGroupInfo = g_ResrcMgr.FindGroupInfoMgr(mcMapCode, fcWUTCode);

        GameField* const pGameField = GameField::ALLOC();

        bool bPreMapLoading = true, bPreRegen = true, bViewport = false;
        const eZONETYPE zoneType = GetZoneType();
        switch(zoneType)
        {
        case eZONETYPE_VILLAGE: case eZONETYPE_FIELD:
        case eZONETYPE_DOMINATION_FIELD:
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        case eZONETYPE_SPA_FIELD:
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
            bViewport = false;
            break;
        case eZONETYPE_MISSION: case eZONETYPE_CHUNTING: 
            bPreRegen = false; // 리젠은 유저가 섹터로 들어왔을 때 한다.
            bPreMapLoading = false; // 최초의 유저가 입장한 후에 맵을 로딩한다.
        case eZONETYPE_INSTANCE:
            bPreRegen = false; // 리젠은 유저가 섹터로 들어왔을 때 한다.
            bPreMapLoading = false; // 최초의 유저가 입장한 후에 맵을 로딩한다.
            break;
        }

        if(!pFieldInfo)
        {
            SUNLOG(eCRITICAL_LOG,
                   "[GameZone::OnCreate()] pFieldInfo is NULL, FCode = %d, WMOCode = %d",
                   m_pMAPINFO->FCode[idx], m_pMAPINFO->FWMOCode[idx]);
        }

        pGameField->Init(this, pFieldInfo, m_pMAPINFO->FGCode[idx],
                         pRegenInfo, pGroupInfo,
                         bPreMapLoading, bViewport, bPreRegen);
        m_GameFieldHash.insert(_GAMEFIELD_HASH::value_type(pGameField->GetFieldCode(), pGameField));

    }
}


//__NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__
BOOLEAN
GameZone::UpdateNumberOfExpectedPlayers(WORD* number_of_active_players)
{
    // UpdateAllMonster발생시 사용되는... Get & Replace
    if(m_NumberOfExpectedPlayers != m_NumberOfExpectedPlayersPrev)
    {
        m_NumberOfExpectedPlayersPrev = m_NumberOfExpectedPlayers;
        *number_of_active_players = m_NumberOfExpectedPlayers;
        return true;
    }

    *number_of_active_players = m_NumberOfExpectedPlayers;
    return false;
}

VOID
GameZone::Update(DWORD dwDeltaTick)
{
    GameField* pGameField = NULL;
    _GAMEFIELD_HASH::iterator it = m_GameFieldHash.begin(), end = m_GameFieldHash.end();
    while(it != end)
    {
        (it->second)->Update(dwDeltaTick);
        ++it;
    }
}

VOID
GameZone::Leave(Player* pPlayer)
{
    if(!pPlayer->GetField())
    {
        SUNLOG(eCRITICAL_LOG, "[GameZone::Leave] Field is NULL!");
        return;
    }

    pPlayer->GetField()->LeaveField(pPlayer);
    // TODO(WAVERIX) (081218) (WARNING) 메모리 누수가 발생할 가능성 있다.
    // 하지만 플레이어는 이것 이후로도 많은 처리를 수행한다. 그곳에서 해제가 제대로 이뤄질 거라 생각되지만
    // 꼭 확인할 것!

    pPlayer->SetGameZone(ePRS_NOT_AT_ZONE, NULL, 0);

    _PLAYER_HASH::iterator it = m_PlayerHash.find(pPlayer->GetObjectKey());
    if(it == m_PlayerHash.end())
    {
        SUNLOG(eCRITICAL_LOG, "[Map:%u][%s]LeaveZone에러", GetMapCode(), pPlayer->GetCharName());
        return; //__NA000000_080324_GAMEZONE_IRREGULAR_CODE_FIX__ 매크로 제거
    }
    m_PlayerHash.erase(it);
}


// 미션에서 사용
VOID
GameZone::Warp(Player* pPlayer, MAPCODE fieldCode, WzID areaID, WzVector* pwvStartPos)
{
    GameZone* pEnterZone = NULL;
    MAPCODE EnterFieldCode = 0;
    eZONE_STATE eRoomState = pPlayer->GetGameZone(pEnterZone, EnterFieldCode);
    ASSERT((eRoomState & ePRS_AT_ZONE));

    if(pEnterZone)
        pEnterZone->LeaveForWarp(pPlayer);
    else
    {
        SUNLOG(eCRITICAL_LOG, "Warp()존이없다.[%u][%u]", eRoomState, EnterFieldCode);
    }

    pPlayer->SetGameZone(ePRS_BEFORE_ENTER_ZONE, this, fieldCode, areaID, pwvStartPos);

    pPlayer->GetMovementChecker()->Reset();
}

VOID
GameZone::GetArea(GameField* pField, Player* pPlayer, MAPCODE fieldCode, WzID& areaID, WzVector& rwvStartPos)
{
    pField->GetAreaRandomStartingVector(areaID, rwvStartPos);
}

BOOL
GameZone::Join(Player* pPlayer, MAPCODE fieldCode, WzID areaID, WzVector* pwvStartPos)
{
    FlowControl flow;
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        GameField* pField = FindGameField(fieldCode);
        IfFailThenBreak(pField, 10);

        BOOL bValdPos = FALSE;
        WzVector wzVec = { 0, 0, 0 };

        if(pwvStartPos && (pwvStartPos->x || pwvStartPos->y || pwvStartPos->z))
        {
            if(pField->IsMovableCoordinates(pwvStartPos))
                bValdPos = TRUE;
        }

        if(bValdPos)
        {
            wzVec = *pwvStartPos;
        }
        else if(areaID != 0)
        {
            GetArea(pField, pPlayer, fieldCode, areaID, wzVec);
        }
        else
        {
            const BASE_PLAYERINFO* const pINFO = pPlayer->GetCharInfo();
            if(pINFO->m_iRegion == GetMapCode())
            {
                wzVec.x = pINFO->m_sLocationX;
                wzVec.y = pINFO->m_sLocationY;
                wzVec.z = pINFO->m_sLocationZ;
            }
        }

        // 좌표가 선택되지 않았으면 랜덤하게 구한다.
        if(wzVec.x == 0 && wzVec.y == 0 && wzVec.z == 0)
        {
            wzVec = pField->GetRandomStartingVector();
        }
        else if(!pField->IsMovableCoordinates(&wzVec))
        {
            wzVec = pField->GetRandomStartingVector();
        }

        // 선택된 좌표의 유효성을 체크한다.
        IfFailThenBreak(pField->IsMovableCoordinates(&wzVec), 30);

        if(FALSE == flow.FCAssert(pField->EnterField(pPlayer, &wzVec)))
        {
            SetFailFc(pField->EnterField(pPlayer, &wzVec), 50);
            break;
        }

        _PLAYER_HASH::const_iterator itFIND = m_PlayerHash.find(pPlayer->GetObjectKey());
        if(itFIND != m_PlayerHash.end())
        {
            Player* pSyncFault = itFIND->second;
            SUNLOG(eCRITICAL_LOG, "[Map:%u][%s]=[%s]EnterZone Error Occured",
                GetMapCode(), pPlayer->GetCharName(), pSyncFault->GetCharName());
        }
        m_PlayerHash.insert(std::make_pair(pPlayer->GetObjectKey(), pPlayer));

        // 필드를 들어올 때 섀도우면 소환체가 있는지 체크하고 있으면 해당 몬스터도 Enter시킨다.
        // 이게 Player의 OnEnterField에 들어가지 못하고 여기 있는 이유는, 
        // 섀도우가 들어갈때 소환수도 소환되는데, 1. 섀도우 Enter ==> 소환수 생성 ==> 소환수 정보 클라 전달 ==> 섀도우 정보 클라 전달
        // 이다. 그런데 클라 입장에서는 섀도우가 들어오기 전에 소환수가 먼저 들어왔으니 
        // 주인없는 소환수라 하여 처리 하지 않는다. 그래서 부득이 Enter 처리 완료 시점에서 처리한다.
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        // 온천은 소환수 재소환하지 않는다
        if (GetZoneType() != eZONETYPE_SPA_FIELD) {
            pPlayer->CallSummonMonster();
        }
#else
        pPlayer->CallSummonMonster();
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

        return TRUE;
    }
    FLOWCONTROL_END;

    switch(flow.GetErrorCode())
    {
    case 10:
        SUNLOG(eCRITICAL_LOG, "[GameZone::Join] Can't Find Field (%u) OnEnterUser.", fieldCode);
        break;
    case 30:
        SUNLOG(eCRITICAL_LOG, "[GameZone::Join] IsMovableCoordinates Failed!!");
        break;
    case 50: // assert
        break;
    default:
        SUNLOG(eCRITICAL_LOG, _T("Unexpected Status %u"), flow.GetErrorCode());
        break;
    }
    return FALSE;
}

// (f100623.1L) change an interface to support a multi-checksum checker
bool GameZone::CheckMapVersion(FIELDCODE field_code,
                               EngineFileChecksum* stored_checksums,
                               const EngineFileChecksum& inputed_checksums) const
{
    GameField* game_field = const_cast<GameZone*>(this)->FindGameField(field_code);
    if (game_field == NULL) {
        return false;
    }
    return game_field->CheckMapVersion(stored_checksums, inputed_checksums);
}
/*
BOOL
GameZone::CheckMapVersion(FIELDCODE fieldCode, DWORD checksum, DWORD* OUT pCurCheckSum)
{
    GameField* pField = FindGameField(fieldCode);
    if(NULL == pField)
        return FALSE;

    return pField->CheckMapVersion(checksum, pCurCheckSum);
}
*/

GameField*
GameZone::GetEntryField() const
{
    //  GameField 자료구조 정리하자.
    const FIELDCODE fieldCode = m_pMAPINFO->FCode[0];
    assert(fieldCode);
    _GAMEFIELD_HASH::const_iterator it = m_GameFieldHash.find(fieldCode);
    return it != m_GameFieldHash.end() ? it->second : 0 ;
}

MAPCODE
GameZone::GetEntryFieldCode() const
{
    return GetMAPINFO()->FCode[0];
}

DWORD
GameZone::GetEntryAreaID()
{
    return GetMAPINFO()->wStartAreaID;
}

DWORD
GameZone::GetTotalMonsterNum(DWORD & OUT dwDeadNPCNum)
{
    DWORD dwNPCNum = 0;
    dwDeadNPCNum = 0;

    GameField* pGameField = NULL;
    _GAMEFIELD_HASH::iterator it = m_GameFieldHash.begin(), end = m_GameFieldHash.end();
    while(it != end)
    {
        pGameField = it->second;
        dwNPCNum += pGameField->GetNPCNum();
#ifdef _DEV_TAIYO_ADD_REGEN_TIME
        dwDeadNPCNum += pGameField->GetDeadNPCNum();
#endif
        ++it;
    }

    return dwNPCNum;
}



VOID
GameZone::ChangeHandler(const eZONETYPE kindOfZone, const eINSTANCE_DUNGEON_KIND kindOfIndun)
{
    switch(kindOfZone)
    {
    case eZONETYPE_VILLAGE:
        Policyment.SetField
            ( Policyment.ENABLE_GROUP_VILLAGE
            );
        break;
    case eZONETYPE_FIELD:
    case eZONETYPE_TUTORIAL_FIELD:
        Policyment.SetField
            ( Policyment.ENABLE_GROUP_HUNTING_FIELD
            | Policyment.ENABLE_SAVE_LOCATION
            );
        break;
    case eZONETYPE_DOMINATION_FIELD:
        Policyment.SetField
            ( Policyment.ENABLE_GROUP_HUNTING_FIELD
            | Policyment.ENABLE_DOMINATION_CONTROL // (WAVERIX) (100122) add rule
            );
        break;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    case eZONETYPE_SPA_FIELD:
        Policyment.SetField(Policyment.ENABLE_GROUP_SPAFIELD);
        break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        //  하단의 타입들은 GameRoom에서 평가된다.
    case eZONETYPE_LOBBY:
    case eZONETYPE_CHUNTING:
    case eZONETYPE_PVP:
    case eZONETYPE_MISSION:
    case eZONETYPE_INSTANCE:
    default:
        ASSERT(!"ChangeHandler Unexpected ZoneType");
        Policyment.SetField(Policyment.DISABLE_ALL);
        break;
    }
}

#ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
//현재 게임존에 있는 모든 유저가 같은 길드원인가?
bool
GameZone::IsAllSameGuildMember()
{
    Player* pPlayer = NULL;
    GUILDGUID firstGuildGuid = 0;
    GUILDGUID curGuildGuid = 0;
    bool bFirst = true;
    _PLAYER_HASH::iterator it = m_PlayerHash.begin(), end = m_PlayerHash.end();
    for(; it != end ; ++it)
    {
        pPlayer = it->second;
        if (obj_util::IsValid(pPlayer, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }

        //길드에 가입되어 있고 
        curGuildGuid = pPlayer->GetGuildGuid();
        if(curGuildGuid == 0)
            return false;

        if(bFirst)
        {
            firstGuildGuid = curGuildGuid;
            bFirst = false;
        }

        // 서로 다른 길드원이면 
        if(firstGuildGuid != curGuildGuid)
            return false;
    }

    return true;
}
#endif

void GameZone::ResetTrigger()
{
    DestroyTrigger();
    CreateTrigger();
}

void GameZone::CreateTrigger()
{
    if( FALSE == m_bCreateTrigger )
        m_bCreateTrigger = TRUE;
    else
        return;

    const _GAMEFIELD_HASH& rFields = GetFieldHash();
    FOREACH_CONTAINER(const _GAMEFIELD_HASH::value_type& rField, rFields, _GAMEFIELD_HASH)
    {
        GameField* const pField = rField.second;
        TriggerManagerEx* const pTriggerManager = pField->GetTriggerManager();
        pTriggerManager->MAP_STATE = TriggerLastedState::Create();
        pTriggerManager->MAP_STATE.Init();
    }
}

void GameZone::DestroyTrigger()
{
    if( FALSE != m_bCreateTrigger )
        m_bCreateTrigger = FALSE;
    else
        return;

    const _GAMEFIELD_HASH& rFields = GetFieldHash();
    FOREACH_CONTAINER(const _GAMEFIELD_HASH::value_type& rField, rFields, _GAMEFIELD_HASH)
    {
        GameField* const pField = rField.second;
        TriggerLastedStateDelegator& rDelegator = pField->GetTriggerManager()->MAP_STATE;
        TriggerLastedState* pINFO = rDelegator.REFERENCE();
        TriggerLastedState::Destroy(pINFO);
        rDelegator = TriggerLastedStateDelegator();
    }
}

BOOL GameZone::TriggerActive(WzID triggerID)
{
    BOOL bSatisfied = TRUE;
    const _GAMEFIELD_HASH& rFields = GetFieldHash();
    FOREACH_CONTAINER(const _GAMEFIELD_HASH::value_type& rField, rFields, _GAMEFIELD_HASH)
    {
        GameField* const pField = rField.second;
        TriggerManagerEx* const pTriggerManager = pField->GetTriggerManager();
        if (FALSE == pTriggerManager->InsertActiveTrigger(triggerID) )
            bSatisfied = FALSE;
    }

    return bSatisfied;
}

void GameZone::TriggerInactive(WzID triggerID)
{
    const _GAMEFIELD_HASH& rFields = GetFieldHash();
    FOREACH_CONTAINER(const _GAMEFIELD_HASH::value_type& rField, rFields, _GAMEFIELD_HASH)
    {
        GameField* const pField = rField.second;
        TriggerManagerEx* const pTriggerManager = pField->GetTriggerManager();
        pTriggerManager->RequestRemoveActiveTrigger(triggerID);
    }
}

void GameZone::AllTriggerInactive()
{
    const _GAMEFIELD_HASH& rFields = GetFieldHash();
    FOREACH_CONTAINER(const _GAMEFIELD_HASH::value_type& rField, rFields, _GAMEFIELD_HASH)
    {
        GameField* const pField = rField.second;
        TriggerManagerEx* const pTriggerManager = pField->GetTriggerManager();
        pTriggerManager->RemoveAllActiveTrigger();
    }
}

