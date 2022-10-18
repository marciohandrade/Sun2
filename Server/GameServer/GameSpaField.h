#pragma once

#include "GameZone.h"
#include "FieldPvP.h"

class MatchCheckCodeFilters;
class GameSpaField : public GameZone
{
public:
    GameSpaField();
    virtual ~GameSpaField();
public:
    virtual void OnCreate();
    virtual BOOL Join(Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos);
    virtual BOOL MoveField(Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos);
    virtual	void Leave(Player * pPlayer);
    virtual	void LeaveForWarp(Player* pPlayer);

    virtual BOOL CanPVP(Player* pAttacker, Player* pTarget, OUT RC::eSKILL_RESULT* pErrCode);
    virtual BOOL CanPVP(Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT* pErrCode);

    virtual eZONETYPE GetZoneType() { return eZONETYPE_SPA_FIELD; }
    virtual void SetZone(Object* pObject, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos);
    virtual void Ready(Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos);

private:
    SLOTCODE ban_item_type[1];  //check_filters를 위한 배열, 금지아이템타입 보관 
    MatchCheckCodeFilters* check_filters_;

    FieldPvPRuleManager field_pvp_rule_;

    __DECL_CUSTOMPOOL_PTR(GameSpaField)
};