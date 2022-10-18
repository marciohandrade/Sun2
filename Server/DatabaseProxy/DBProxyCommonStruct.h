#pragma once
#ifndef DBPROXY_COMMON_STRUCT_H
#define DBPROXY_COMMON_STRUCT_H

namespace nsQuery {
;

//
struct ProxyTransaction
{
    typedef int value_type;
    // r100928.2L
    enum eTransaction {
        eTrans_None     = 0,
        //
        eTrans_ItemSlotInfo_Select, //S_ItemSlotInfo_Select
        eTrans_ItemSlotInfo_Update, //S_ItemSlotInfo_Update
        //
        eTrans_Perk_Select, //S_Perk_Select
        eTrans_Perk_Insert, //S_Perk_Insert
        eTrans_Perk_Delete, //S_Perk_Delete
        eTrans_Perk_Update, //S_Perk_Update
        //
        eTrans_SlotInfo_Select, //S_SlotInfo_Select
        eTrans_SlotInfo_Insert, //S_SlotInfo_Insert
        eTrans_SlotInfo_Update, //S_SlotInfo_Update
        //
        eTrans_Quest_Select, //S_Quest_Select
        eTrans_Quest_Update, //S_Quest_Update
        //
        eTrans_BattleRecords_Select, //S_BattleRecords_Select
        eTrans_BattleRecords_Update, //S_BattleRecords_Update
        //
        eTrans_Char_State_Select, //S_Char_State_Select
        eTrans_Char_State_Insert, //S_Char_State_Insert
        eTrans_Char_State_Delete, //S_Char_State_Delete
        eTrans_Char_State_Update, //S_Char_State_Update
        // C_NA_0_20100520_ACSYSTEM_CHANGED
		eTrans_Achievement_Select,
		eTrans_Achievement_Update,
        //
        eTrans_MissionInfo_Select, //S_MissionInfo_Select
        eTrans_MissionInfo_Insert, //S_MissionInfo_Insert
        eTrans_MissionInfo_Update, //S_MissionInfo_Update
        //_NA_006372_20130108_BADGE_ITEM
        eTrans_CharacterExtraInfo_Select,
        eTrans_CharacterExtraInfo_Update,
        //
        //!~ _NA_007086_20140318_MONSTERBOOK
        eTrans_CharacterMonsterBook_Select,
        eTrans_CharacterMonsterBook_Update,
        //~! _NA_007086_20140318_MONSTERBOOK
        //!~ _NA_007932_20150106_USER_EXTRA_INFO
        eTrans_UserExtraInfo_Select,
        eTrans_UserExtraInfo_Update,
        //~! _NA_007932_20150106_USER_EXTRA_INFO
        eTrans_Counts
    };
    value_type value_;
};

struct ProxyCommand
{
    typedef int value_type;
    // r100928.2L
    enum eCommand {
        eCmd_None = 0,
        eCmd_Equip_Load,
        eCmd_Equip_ChangedUpdate,
        eCmd_Inven_Load,
        eCmd_Inven_ChangedUpdate,
        eCmd_Perk_Load,
        eCmd_Perk_ChangedUpdate,
        eCmd_Skill_Load,
        eCmd_Skill_ChangedUpdate,
        eCmd_Quick_Load,
        eCmd_Quick_ChangedUpdate,
        eCmd_Quest_Load,
        eCmd_Quest_ChangedUpdate,
        eCmd_BattleRecords_Load,
        eCmd_BattleRecords_ChangedUpdate,
        eCmd_Char_State_Load,
        eCmd_Char_State_ChangedUpdate,
        // C_NA_0_20100520_ACSYSTEM_CHANGED
		eCmd_Achievement_Load,
		eCmd_Achievement_ChangedUpdate,
        //
        eCmd_MissionInfo_Load,
        eCmd_MissionInfo_ChangedUpdate,
        // _NA_006372_20130108_BADGE_ITEM
        eCmd_CharacterExtraInfo_Load,
        eCmd_CharacterExtraInfo_ChangedUpdate,
        //
        // _NA_007932_20150106_USER_EXTRA_INFO
        eCmd_UserExtraInfo_Load,
        eCmd_UserExtraInfo_ChangedUpdate,
        //
        eCmd_Counts
    };
    value_type value_;
};

}; //end of namespace

#endif //DBPROXY_COMMON_STRUCT_H