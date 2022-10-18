#pragma once
#ifndef GAMESERVER_DROP_MANAGER_H
#define GAMESERVER_DROP_MANAGER_H

//==================================================================================================
/// Drop처리하는 메니져 클래스 
/**
    @author
        Kim Min Wook < taiyo@webzen.co.kr > 
    @since
        2005. 11. 20
    @remarks
        - Drop관련 툴을 제작하기 편리하도록 실제 코어부분과 메니져 부분을 분리하는 구조로 제작
        다시말하면, 기존의 DropManager에서 DropFormulaCore, DropItemRatioCalc을 분리함
    @note
        - 
    @todo
        - 
    @history
    - 2005/11/21 : 돈 아이템 드랍 딜레이 처리
    - 2006/1/16 : 랭크 아이템을 떨어 뜨릴때 옵션과 구멍을 뚫어서 떨어뜨림 처리
    - 2006/8/10 : 딜레이 처리 리팩토링
*/
//==================================================================================================

#include <ProgramCommon/WzDataType.h>
#include <DropCore.h>
#include <list>


class Item;
class GameField;
class SCItemSlot;

//==================================================================================================

class DropManager : public IParser
{
public:
    DropManager();
    virtual ~DropManager();

    //void Reload();
    void Init(DWORD pool_size);
    void Release();
    void Process();
    void OnFieldRelease(ZONEKEY zone_key);

    void DelayedDrop_(GameField* game_field, DWORD sector_index, const WzVector* origin_pos,
        Player* owner, DWORD monster_key, const BASE_NPCINFO* monster_info,
        eNPC_GRADE npc_grade, LEVELTYPE monster_display_level);
    void InstantDrop_(GameField* game_field, DWORD sector_index, const WzVector* origin_pos,
        Player* owner, DWORD monster_key, const BASE_NPCINFO* monster_info,
        eNPC_GRADE npc_grade, LEVELTYPE monster_display_level);
    bool DropMoneyToField(GameField* game_field, const WzVector* creating_pos,
        PLAYERKEY player_key, DWORD from_monster_key,
        const MONEY& money, DWORD delaying_interval = 0);
    bool DropItemToField(GameField* game_field, const WzVector* creating_pos,
        PLAYERKEY player_key, DWORD from_monster_key,
        SCItemSlot& drop_item_slot, DWORD delaying_interval = 0);

    virtual BOOL Load(eSCRIPT_CODE script_code, const TCHAR* pszFileName, BOOL bBigData, eSEPERATOR_MODE mode, const TCHAR* pszPackFileName, BOOL bReload);
    virtual BOOL SolarLoad(eSCRIPT_CODE script_code, BYTE* pSTREAM, DWORD dwSize, BOOL bBigData);
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL bReload) {  __UNUSED((script_code, bReload)); return false; }

private:
    enum { DONT_DROP_LEVEL_INTERVAL = 10, };
    //
    struct DelayDropInfo;
    struct DelayedFieldObject;
    //{__NA000000_20090320_DELAY_ITEM_DROP_TIME
    typedef STLX_MULTIMAP<ZONEKEY, DelayDropInfo*> DELAY_DROPINFO_LIST;
    typedef STLX_LIST<DelayedFieldObject*> DELAYEDFOBJ_LIST;
    //----------------------------------------------------------------------------------------------
    void UpdateDelayedItemDrop();

    void DropQuest(GameField* game_field, const BASE_NPCINFO* monster_info, Player* owner,
        eNPC_GRADE npc_grade, float added_bonus_percentage,
        GameZone* pZone, const WzVector* origin_pos,
        DWORD monster_key, DWORD unused__owner_object_key);

    void DropMoney(bool jackpot, GameField* game_field, const WzVector* origin_pos, 
        DWORD monster_key, DWORD dwPlayerKey, 
        MONEY& DroppedMoney);
    void DropItem(GameField* game_field, const WzVector* origin_pos, 
        DWORD monster_key, DWORD player_key, const BASE_NPCINFO* pNPCInfo,
        SCItemSlot& dropped_item_slot);
    DWORD GetSecondItemOwnershipUserKey(Player* pre_owner, SCItemSlot& DroppedItemSlot);
    DWORD GetSecondHeimOwnershipUserKey(Player* pre_owner);
    void GMDrop(GameField* game_field, const WzVector* origin_pos, Player* owner,
        DWORD monster_key, const BASE_NPCINFO* monster_info, 
        eZONETYPE zone_type, DWORD drop_index, eNPC_GRADE npc_grade,
        float bonus_percentage, DWORD temp_player_key);
    //----------------------------------------------------------------------------------------------
    DropCore drop_core_;
    DELAY_DROPINFO_LIST delay_drop_info_list_;
    DELAYEDFOBJ_LIST delayed_field_object_list_;
};

extern DropManager g_DropManager;

#endif //GAMESERVER_DROP_MANAGER_H