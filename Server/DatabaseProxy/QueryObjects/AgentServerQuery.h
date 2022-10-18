#pragma once
#ifndef __AGENT_SERVER_QUERY_H__
#define __AGENT_SERVER_QUERY_H__

#include "StructInQuery.h"
#include "QueryPoolFactoryDefine.h"

//==================================================================================================
/// 쿼리를 추가할 때 해야 할 일!
/// 1. 쿼리 클래스를 만든다.
/// 2. 쿼리 풀 클래스에다가 스크립트에 맞추어 등록한다.
//==================================================================================================
// @history
//  ~10.04.13|custom logic
//  10.04.13|waverix|delete macros 901 '__NA_1165_ETHER_DEVICE_SAVE_DB', 901
//                                 902 '__NA_1288_AC_RENEWAL_FOR_TIME_LINE' 902
//                                 903 '__NA000000_090506_EVENT_TYPE_MODIFICATION__'
//                                 903 '__NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM'
//  10.04.13|waverix|delete macro auto numbering logic related to deleted macros
//
//==================================================================================================

//==================================================================================================
/// 캐릭터 리스트 정보 읽어 오는 쿼리
//==================================================================================================
// 2005/12/28 (taiyo)
// 맴버추가 : height, face, hair, 길드관련
// 이름변경 : CharacterListQuery -> Query_User_Select
// 개별 POOL 구조로 변경
// r101007.3L
// CHANGES: f101005.3L, change a query structure making methodology
class Query_User_Select : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_User_Select);
    //
    enum {
        _MAKE_REORDERING_BINDING_NO_START_(m_UserGuid, 0),
        _MAKE_REORDERING_BINDING_NO_(m_bySlot),
        _MAKE_REORDERING_BINDING_NO_(m_byClass),
        _MAKE_REORDERING_BINDING_NO_(m_CharGuid),
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_char_name_),
        _MAKE_REORDERING_BINDING_NO_(m_byHeight),
        _MAKE_REORDERING_BINDING_NO_(m_byFace),
        _MAKE_REORDERING_BINDING_NO_(m_byHair),
        _MAKE_REORDERING_BINDING_NO_(m_LV),
        _MAKE_REORDERING_BINDING_NO_(m_sStrength),
        _MAKE_REORDERING_BINDING_NO_(m_sDexterity),
        _MAKE_REORDERING_BINDING_NO_(m_sVitality),
        _MAKE_REORDERING_BINDING_NO_(m_sInteligence),
        _MAKE_REORDERING_BINDING_NO_(m_sSpirit),
        _MAKE_REORDERING_BINDING_NO_(m_sSkillStat1),
        _MAKE_REORDERING_BINDING_NO_(m_sSkillStat2),
        _MAKE_REORDERING_BINDING_NO_(m_UserPoint),
        _MAKE_REORDERING_BINDING_NO_(m_i64Exp),
        _MAKE_REORDERING_BINDING_NO_(m_iMaxHP),
        _MAKE_REORDERING_BINDING_NO_(m_iHP),
        _MAKE_REORDERING_BINDING_NO_(m_iMaxMP),
        _MAKE_REORDERING_BINDING_NO_(m_iMP),

        _MAKE_REORDERING_BINDING_NO_(m_Money),
        _MAKE_REORDERING_BINDING_NO_(m_iRemainStat),
        _MAKE_REORDERING_BINDING_NO_(m_iRemainSkill),
        _MAKE_REORDERING_BINDING_NO_(m_iSelectStyleCode),
        _MAKE_REORDERING_BINDING_NO_(m_byPKState),
        _MAKE_REORDERING_BINDING_NO_(m_byCharState),
        _MAKE_REORDERING_BINDING_NO_(m_StateTime),
        _MAKE_REORDERING_BINDING_NO_(m_iRegion),
        _MAKE_REORDERING_BINDING_NO_(m_sX),
        _MAKE_REORDERING_BINDING_NO_(m_sY),
        _MAKE_REORDERING_BINDING_NO_(m_sZ),
        _MAKE_REORDERING_BINDING_NO_(m_iPreRegion),

        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_title_id_),
        _MAKE_REORDERING_BINDING_NO_(m_TitleTime),
        _MAKE_REORDERING_BINDING_NO_(m_byInvisibleOpt),
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _MAKE_REORDERING_BINDING_NO_PTR_(m_pACStream),    
    #endif
        _MAKE_REORDERING_BINDING_NO_(m_byChaoState),
        _MAKE_REORDERING_BINDING_NO_(m_i64ChaoTime),

        _MAKE_REORDERING_BINDING_NO_(m_iPlayLimitedTime),
        //-PVP---------------------------------
        //길드정보--------------------------------
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_guild_name_),
        _MAKE_REORDERING_BINDING_NO_(m_GuildGuid),
        _MAKE_REORDERING_BINDING_NO_(m_eGuildDuty),
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_guild_nick_name_),
        _MAKE_REORDERING_BINDING_NO_(m_EventUseTime),
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_create_time_),
        _MAKE_REORDERING_BINDING_NO_(m_byDelChk),
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_del_date_),
        _MAKE_REORDERING_BINDING_NO_(m_byExtraInventoryTabCount),
        _MAKE_REORDERING_BINDING_NO_(m_ExtraEquipCashSlotCount),
        _MAKE_REORDERING_BINDING_NO_(m_bTradeFlag),
        _MAKE_REORDERING_BINDING_NO_(m_bWhisperFlag),
        _MAKE_REORDERING_BINDING_NO_(m_byFreeInitCount),

        _MAKE_REORDERING_BINDING_NO_(m_ChangeOfClassStep),

        _MAKE_REORDERING_BINDING_NO_(m_bSummonedPet),
        _MAKE_REORDERING_BINDING_NO_(m_bMAXIntimacy),
        _MAKE_REORDERING_BINDING_NO_(m_PetItemPos),
        _MAKE_REORDERING_BINDING_NO_(m_bMAXOptionRemainTime),

        _MAKE_REORDERING_BINDING_NO_(m_bActiveEther),
        _MAKE_REORDERING_BINDING_NO_(m_byEtherBulletPos), 
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _MAKE_REORDERING_BINDING_NO_(m_iACVersion),
    #endif
        _MAKE_REORDERING_BINDING_NO_(m_EventUseTimeNormal),
        _MAKE_REORDERING_BINDING_NO_(m_iMaxSD),
        _MAKE_REORDERING_BINDING_NO_(m_iSD),
    #ifdef _NA_1419_20100121_ACCUMULATE_POINT
        _MAKE_REORDERING_BINDING_NO_(m_wAccumulateYear),
        _MAKE_REORDERING_BINDING_NO_(m_dwAccumulateTime),
        _MAKE_REORDERING_BINDING_NO_(m_wAccumulatePoint),
    #endif
    #ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
        _MAKE_REORDERING_BINDING_NO_(m_RenameStatus),
    #endif
    #ifdef _NA_002050_20110216_ADD_GENDER
        _MAKE_REORDERING_BINDING_NO_(gender), 
    #endif
	#ifdef _NA_003027_20111013_HONOR_SYSTEM
		_MAKE_REORDERING_BINDING_NO_(honor_title),
	#endif
        //_NA_004035_20120227_COSTUME_ITEM
        _MAKE_REORDERING_BINDING_NO_(is_equipped_costume),
        _MAKE_REORDERING_BINDING_NO_(costume_item_pos),
        _MAKE_REORDERING_BINDING_NO_(costume_item_code),

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        _MAKE_REORDERING_BINDING_NO_(is_equipped_deco_costume),
        _MAKE_REORDERING_BINDING_NO_(deco_costume_item_pos),
        _MAKE_REORDERING_BINDING_NO_(deco_costume_item_code),
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

        _MAKE_REORDERING_BINDING_NO_COUNTS_() // end of records. it equal to number of records
    };

#ifdef _NA_002050_20110216_ADD_GENDER
    struct Parameter
    {
        enum { kNumberOfRow = 1, kNumberOfColumn = 1 };
        int char_slot_size;
    } parameters[Parameter::kNumberOfRow];

    _BEGIN_PARAM_DATA(Parameter, parameters, Parameter::kNumberOfColumn)
        _BINDING_PARAM(0, char_slot_size, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA()
public:
    BYTE GetCharSlotSize() const
    {
        if (parameters[0].char_slot_size < MaxCharListConfig::kDefaultSlotSize)
        {
            return MaxCharListConfig::kDefaultSlotSize;
        }
        if (parameters[0].char_slot_size > MaxCharListConfig::kMaxSlotSize)
        {
            return MaxCharListConfig::kMaxSlotSize;
        }
        return static_cast<BYTE>(parameters[0].char_slot_size);
    }
#else
public:
    _INIT_PARAM_DATA();
#endif // _NA_002050_20110216_ADD_GENDER
    enum
    {
        RESULT_ROW_NUM = MAX_CHARACTER_SLOT_NUM,
        RESULT_COL_NUM = _MAKE_REORDERING_BINDING_NO_COUNTS_(),
    };
    //
    struct sQUERY_RESULT : public sQUERY_CHARACTER
    {
        #ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
            int m_RenameStatus;
        #endif
            int _dummy_field_;
    } pResult[RESULT_ROW_NUM];
    //
    ulong uLength[RESULT_COL_NUM];
    //
    //
    _BEGIN_BINDING_DATA(sQUERY_RESULT, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)
        _REORDERING_BINDING_COLUMN(m_UserGuid)
        _REORDERING_BINDING_COLUMN(m_bySlot)
        _REORDERING_BINDING_COLUMN(m_byClass)
        _REORDERING_BINDING_COLUMN(m_CharGuid)
        _REORDERING_BINDING_COLUMN_PTR(result_info_char_name_)
        _REORDERING_BINDING_COLUMN(m_byHeight)
        _REORDERING_BINDING_COLUMN(m_byFace)
        _REORDERING_BINDING_COLUMN(m_byHair)
        _REORDERING_BINDING_COLUMN(m_LV)
        _REORDERING_BINDING_COLUMN(m_sStrength)
        _REORDERING_BINDING_COLUMN(m_sDexterity)
        _REORDERING_BINDING_COLUMN(m_sVitality)
        _REORDERING_BINDING_COLUMN(m_sInteligence)
        _REORDERING_BINDING_COLUMN(m_sSpirit)
        _REORDERING_BINDING_COLUMN(m_sSkillStat1)
        _REORDERING_BINDING_COLUMN(m_sSkillStat2)
        _REORDERING_BINDING_COLUMN(m_UserPoint)
        _REORDERING_BINDING_COLUMN(m_i64Exp)
        _REORDERING_BINDING_COLUMN(m_iMaxHP)
        _REORDERING_BINDING_COLUMN(m_iHP)
        _REORDERING_BINDING_COLUMN(m_iMaxMP)
        _REORDERING_BINDING_COLUMN(m_iMP)

        _REORDERING_BINDING_COLUMN(m_Money)
        _REORDERING_BINDING_COLUMN(m_iRemainStat)
        _REORDERING_BINDING_COLUMN(m_iRemainSkill)
        _REORDERING_BINDING_COLUMN(m_iSelectStyleCode)
        _REORDERING_BINDING_COLUMN(m_byPKState)
        _REORDERING_BINDING_COLUMN(m_byCharState)
        _REORDERING_BINDING_COLUMN(m_StateTime)
        _REORDERING_BINDING_COLUMN(m_iRegion)
        _REORDERING_BINDING_COLUMN(m_sX)
        _REORDERING_BINDING_COLUMN(m_sY)
        _REORDERING_BINDING_COLUMN(m_sZ)
        _REORDERING_BINDING_COLUMN(m_iPreRegion)

        _REORDERING_BINDING_COLUMN_PTR(result_info_title_id_)
        _REORDERING_BINDING_COLUMN(m_TitleTime)
        _REORDERING_BINDING_COLUMN(m_byInvisibleOpt)
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _REORDERING_BINDING_COLUMN_PTR(m_pACStream)    
    #endif
        _REORDERING_BINDING_COLUMN(m_byChaoState)
        _REORDERING_BINDING_COLUMN(m_i64ChaoTime)

        _REORDERING_BINDING_COLUMN(m_iPlayLimitedTime)
        //-PVP---------------------------------
        //길드정보--------------------------------
        _REORDERING_BINDING_COLUMN_PTR(result_info_guild_name_)
        _REORDERING_BINDING_COLUMN(m_GuildGuid)
        _REORDERING_BINDING_COLUMN(m_eGuildDuty)
        _REORDERING_BINDING_COLUMN_PTR(result_info_guild_nick_name_)
        _REORDERING_BINDING_COLUMN(m_EventUseTime)
        _REORDERING_BINDING_COLUMN_PTR(result_info_create_time_)
        _REORDERING_BINDING_COLUMN(m_byDelChk)
        _REORDERING_BINDING_COLUMN_PTR(result_info_del_date_)
        _REORDERING_BINDING_COLUMN(m_byExtraInventoryTabCount)
        _REORDERING_BINDING_COLUMN(m_ExtraEquipCashSlotCount)
        _REORDERING_BINDING_COLUMN(m_bTradeFlag)
        _REORDERING_BINDING_COLUMN(m_bWhisperFlag)
        _REORDERING_BINDING_COLUMN(m_byFreeInitCount)

        _REORDERING_BINDING_COLUMN(m_ChangeOfClassStep)

        _REORDERING_BINDING_COLUMN(m_bSummonedPet)
        _REORDERING_BINDING_COLUMN(m_bMAXIntimacy)
        _REORDERING_BINDING_COLUMN(m_PetItemPos)
        _REORDERING_BINDING_COLUMN(m_bMAXOptionRemainTime)

        _REORDERING_BINDING_COLUMN(m_bActiveEther)
        _REORDERING_BINDING_COLUMN(m_byEtherBulletPos) 
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _REORDERING_BINDING_COLUMN(m_iACVersion)
    #endif
        _REORDERING_BINDING_COLUMN(m_EventUseTimeNormal)
        _REORDERING_BINDING_COLUMN(m_iMaxSD)
        _REORDERING_BINDING_COLUMN(m_iSD)
    #ifdef _NA_1419_20100121_ACCUMULATE_POINT
        _REORDERING_BINDING_COLUMN(m_wAccumulateYear)
        _REORDERING_BINDING_COLUMN(m_dwAccumulateTime)
        _REORDERING_BINDING_COLUMN(m_wAccumulatePoint)
    #endif
    #ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
        _REORDERING_BINDING_COLUMN(m_RenameStatus)
    #endif
    #ifdef _NA_002050_20110216_ADD_GENDER
        _REORDERING_BINDING_COLUMN(gender)
    #endif
	#ifdef _NA_003027_20111013_HONOR_SYSTEM
		_REORDERING_BINDING_COLUMN(honor_title)
	#endif
        //_NA_004035_20120227_COSTUME_ITEM
        _REORDERING_BINDING_COLUMN(is_equipped_costume)
        _REORDERING_BINDING_COLUMN(costume_item_pos)
        _REORDERING_BINDING_COLUMN(costume_item_code)

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        _REORDERING_BINDING_COLUMN(is_equipped_deco_costume)
        _REORDERING_BINDING_COLUMN(deco_costume_item_pos)
        _REORDERING_BINDING_COLUMN(deco_costume_item_code)
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
    _END_BINDING_DATA();
    //
}; //end of class 'Query_User_Select'

//==================================================================================================

//_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
class Query_User_Select_Inventory : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_User_Select_Inventory);
private:
    enum {
        _MAKE_REORDERING_BINDING_NO_START_(m_CharGuid, 0),
        _MAKE_REORDERING_BINDING_NO_(m_byExtraInventoryTabCount),
        _MAKE_REORDERING_BINDING_NO_(m_bInventoryLock),
        _MAKE_REORDERING_BINDING_NO_(result_info_password_),
        _MAKE_REORDERING_BINDING_NO_(config_version),
        _MAKE_REORDERING_BINDING_NO_(config01),
        _MAKE_REORDERING_BINDING_NO_(config02),
        _MAKE_REORDERING_BINDING_NO_(config03),
        _MAKE_REORDERING_BINDING_NO_(config04),
        _MAKE_REORDERING_BINDING_NO_(config05),
        _MAKE_REORDERING_BINDING_NO_(config06),
        _MAKE_REORDERING_BINDING_NO_(config07),
        _MAKE_REORDERING_BINDING_NO_(config08),
        _MAKE_REORDERING_BINDING_NO_(config09),
        _MAKE_REORDERING_BINDING_NO_(config10),
        //
        _MAKE_REORDERING_PARAM_NO_COUNTS_()
    };
public:
    enum
    {
        RESULT_ROW_NUM      = MAX_CHARACTER_SLOT_NUM,
        RESULT_COL_NUM      = _MAKE_REORDERING_PARAM_NO_COUNTS_(),
    };

    struct sQUERY_RESULT : public sQUERY_CHARACTER
    {
    } pResult[RESULT_ROW_NUM];

    ulong uLength[RESULT_COL_NUM];

    _INIT_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQUERY_RESULT, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)
        _REORDERING_BINDING_COLUMN(m_CharGuid)
        _REORDERING_BINDING_COLUMN(m_byExtraInventoryTabCount)
        _REORDERING_BINDING_COLUMN(m_bInventoryLock)
        _REORDERING_BINDING_COLUMN_PTR(result_info_password_)
        _REORDERING_BINDING_COLUMN(config_version)
        _REORDERING_BINDING_COLUMN(config01)
        _REORDERING_BINDING_COLUMN(config02)
        _REORDERING_BINDING_COLUMN(config03)
        _REORDERING_BINDING_COLUMN(config04)
        _REORDERING_BINDING_COLUMN(config05)
        _REORDERING_BINDING_COLUMN(config06)
        _REORDERING_BINDING_COLUMN(config07)
        _REORDERING_BINDING_COLUMN(config08)
        _REORDERING_BINDING_COLUMN(config09)
        _REORDERING_BINDING_COLUMN(config10)
    _END_BINDING_DATA();
}; //end of class 'Query_User_Select_Inventory'

//==================================================================================================
// r101007.3L
// 2005/12/28 (taiyo)
// 개별 POOL 구조로 변경
// CHANGES: f101005.3L, change a query structure making methodology
class Query_Char_Create : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Char_Create);
    //
    enum {
        _MAKE_REORDERING_PARAM_NO_START_(m_Result, 0),
        _MAKE_REORDERING_PARAM_NO_COUNTS_() // end of records. it equal to number of records
    };
    //
    enum {
        _MAKE_REORDERING_BINDING_NO_START_(m_UserGuid, 0),
        _MAKE_REORDERING_BINDING_NO_(m_bySlot),
        _MAKE_REORDERING_BINDING_NO_(m_byClass),
        _MAKE_REORDERING_BINDING_NO_(m_CharGuid),
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_char_name_),
        _MAKE_REORDERING_BINDING_NO_(m_byHeight),
        _MAKE_REORDERING_BINDING_NO_(m_byFace),

        _MAKE_REORDERING_BINDING_NO_(m_byHair),
        _MAKE_REORDERING_BINDING_NO_(m_LV),
        _MAKE_REORDERING_BINDING_NO_(m_sStrength),
        _MAKE_REORDERING_BINDING_NO_(m_sDexterity),
        _MAKE_REORDERING_BINDING_NO_(m_sVitality),
        _MAKE_REORDERING_BINDING_NO_(m_sInteligence),
        _MAKE_REORDERING_BINDING_NO_(m_sSpirit),
        _MAKE_REORDERING_BINDING_NO_(m_sSkillStat1),
        _MAKE_REORDERING_BINDING_NO_(m_sSkillStat2),
        _MAKE_REORDERING_BINDING_NO_(m_UserPoint),
        _MAKE_REORDERING_BINDING_NO_(m_i64Exp),
        _MAKE_REORDERING_BINDING_NO_(m_iMaxHP),
        _MAKE_REORDERING_BINDING_NO_(m_iHP),
        _MAKE_REORDERING_BINDING_NO_(m_iMaxMP),
        _MAKE_REORDERING_BINDING_NO_(m_iMP),

        _MAKE_REORDERING_BINDING_NO_(m_Money),
        _MAKE_REORDERING_BINDING_NO_(m_iRemainStat),
        _MAKE_REORDERING_BINDING_NO_(m_iRemainSkill),
        _MAKE_REORDERING_BINDING_NO_(m_iSelectStyleCode),
        _MAKE_REORDERING_BINDING_NO_(m_byPKState),
        _MAKE_REORDERING_BINDING_NO_(m_byCharState),
        _MAKE_REORDERING_BINDING_NO_(m_StateTime),
        _MAKE_REORDERING_BINDING_NO_(m_iRegion),
        _MAKE_REORDERING_BINDING_NO_(m_sX),
        _MAKE_REORDERING_BINDING_NO_(m_sY),
        _MAKE_REORDERING_BINDING_NO_(m_sZ),
        _MAKE_REORDERING_BINDING_NO_(m_iPreRegion),

        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_title_id_),
        _MAKE_REORDERING_BINDING_NO_(m_TitleTime),
        _MAKE_REORDERING_BINDING_NO_(m_byInvisibleOpt),
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _MAKE_REORDERING_BINDING_NO_PTR_(m_pACStream),    
    #endif
        _MAKE_REORDERING_BINDING_NO_(m_byChaoState),
        _MAKE_REORDERING_BINDING_NO_(m_i64ChaoTime),

        _MAKE_REORDERING_BINDING_NO_(m_iPlayLimitedTime),
        //길드정보--------------------------------
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_guild_name_),
        _MAKE_REORDERING_BINDING_NO_(m_GuildGuid),
        _MAKE_REORDERING_BINDING_NO_(m_eGuildDuty),
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_guild_nick_name_),
        _MAKE_REORDERING_BINDING_NO_(m_EventUseTime),
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_create_time_),
        _MAKE_REORDERING_BINDING_NO_(m_byDelChk),
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_del_date_),
        _MAKE_REORDERING_BINDING_NO_(m_byExtraInventoryTabCount),
        _MAKE_REORDERING_BINDING_NO_(m_ExtraEquipCashSlotCount),
        _MAKE_REORDERING_BINDING_NO_(m_bTradeFlag),
        _MAKE_REORDERING_BINDING_NO_(m_bWhisperFlag),
        _MAKE_REORDERING_BINDING_NO_(m_byFreeInitCount),
        _MAKE_REORDERING_BINDING_NO_(m_ChangeOfClassStep),

        _MAKE_REORDERING_BINDING_NO_(m_bSummonedPet),
        _MAKE_REORDERING_BINDING_NO_(m_bMAXIntimacy),
        _MAKE_REORDERING_BINDING_NO_(m_PetItemPos),
        _MAKE_REORDERING_BINDING_NO_(m_bMAXOptionRemainTime),

        _MAKE_REORDERING_BINDING_NO_(m_bActiveEther),
        _MAKE_REORDERING_BINDING_NO_(m_byEtherBulletPos), 
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _MAKE_REORDERING_BINDING_NO_(m_iACVersion),
    #endif
        _MAKE_REORDERING_BINDING_NO_(m_EventUseTimeNormal),
        _MAKE_REORDERING_BINDING_NO_(m_iMaxSD),
        _MAKE_REORDERING_BINDING_NO_(m_iSD),
    #ifdef _NA_1419_20100121_ACCUMULATE_POINT
        _MAKE_REORDERING_BINDING_NO_(m_wAccumulateYear),
        _MAKE_REORDERING_BINDING_NO_(m_dwAccumulateTime),
        _MAKE_REORDERING_BINDING_NO_(m_wAccumulatePoint),
    #endif
    #ifdef _NA_002050_20110216_ADD_GENDER
        _MAKE_REORDERING_BINDING_NO_(gender), 
    #endif
	#ifdef _NA_003027_20111013_HONOR_SYSTEM
		_MAKE_REORDERING_BINDING_NO_(honor_title),
	#endif
        //_NA_004035_20120227_COSTUME_ITEM
        _MAKE_REORDERING_BINDING_NO_(is_equipped_costume),
        _MAKE_REORDERING_BINDING_NO_(costume_item_pos),
        _MAKE_REORDERING_BINDING_NO_(costume_item_code),

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        _MAKE_REORDERING_BINDING_NO_(is_equipped_deco_costume),
        _MAKE_REORDERING_BINDING_NO_(deco_costume_item_pos),
        _MAKE_REORDERING_BINDING_NO_(deco_costume_item_code),
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

        _MAKE_REORDERING_BINDING_NO_COUNTS_() // end of records. it equal to number of records
    };

public:
    enum
    { 
        UPDATE_PARAM_ROW_NUM    = 1,
        UPDATE_PARAM_COL_NUM    = _MAKE_REORDERING_PARAM_NO_COUNTS_(),

        SELECT_ROW_NUM          = 1,
        SELECT_COL_NUM          = _MAKE_REORDERING_BINDING_NO_COUNTS_(),
    };
    //
    struct tag_PARAM
    {
        int  m_Result;
    } pParam[UPDATE_PARAM_ROW_NUM];
    //
    struct sQUERY_RESULT : public sQUERY_CHARACTER
    {
    } pResult[SELECT_ROW_NUM];
    //
    ulong uLength[SELECT_COL_NUM];
    // r100928.3L
    //
    //
    _BEGIN_PARAM_DATA(tag_PARAM, pParam, UPDATE_PARAM_COL_NUM)
        _REORDERING_PARAM(m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQUERY_RESULT, pResult, uLength, SELECT_ROW_NUM, SELECT_COL_NUM)
        _REORDERING_BINDING_COLUMN(m_UserGuid)
        _REORDERING_BINDING_COLUMN(m_bySlot)
        _REORDERING_BINDING_COLUMN(m_byClass)
        _REORDERING_BINDING_COLUMN(m_CharGuid)
        _REORDERING_BINDING_COLUMN_PTR(result_info_char_name_)
        _REORDERING_BINDING_COLUMN(m_byHeight)
        _REORDERING_BINDING_COLUMN(m_byFace)

        _REORDERING_BINDING_COLUMN(m_byHair)
        _REORDERING_BINDING_COLUMN(m_LV)
        _REORDERING_BINDING_COLUMN(m_sStrength)
        _REORDERING_BINDING_COLUMN(m_sDexterity)
        _REORDERING_BINDING_COLUMN(m_sVitality)
        _REORDERING_BINDING_COLUMN(m_sInteligence)
        _REORDERING_BINDING_COLUMN(m_sSpirit)
        _REORDERING_BINDING_COLUMN(m_sSkillStat1)
        _REORDERING_BINDING_COLUMN(m_sSkillStat2)
        _REORDERING_BINDING_COLUMN(m_UserPoint)
        _REORDERING_BINDING_COLUMN(m_i64Exp)
        _REORDERING_BINDING_COLUMN(m_iMaxHP)
        _REORDERING_BINDING_COLUMN(m_iHP)
        _REORDERING_BINDING_COLUMN(m_iMaxMP)
        _REORDERING_BINDING_COLUMN(m_iMP)

        _REORDERING_BINDING_COLUMN(m_Money)
        _REORDERING_BINDING_COLUMN(m_iRemainStat)
        _REORDERING_BINDING_COLUMN(m_iRemainSkill)
        _REORDERING_BINDING_COLUMN(m_iSelectStyleCode)
        _REORDERING_BINDING_COLUMN(m_byPKState)
        _REORDERING_BINDING_COLUMN(m_byCharState)
        _REORDERING_BINDING_COLUMN(m_StateTime)
        _REORDERING_BINDING_COLUMN(m_iRegion)
        _REORDERING_BINDING_COLUMN(m_sX)
        _REORDERING_BINDING_COLUMN(m_sY)
        _REORDERING_BINDING_COLUMN(m_sZ)
        _REORDERING_BINDING_COLUMN(m_iPreRegion)

        _REORDERING_BINDING_COLUMN_PTR(result_info_title_id_)
        _REORDERING_BINDING_COLUMN(m_TitleTime)
        _REORDERING_BINDING_COLUMN(m_byInvisibleOpt)
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _REORDERING_BINDING_COLUMN_PTR(m_pACStream)
    #endif
        _REORDERING_BINDING_COLUMN(m_byChaoState)
        _REORDERING_BINDING_COLUMN(m_i64ChaoTime)

        _REORDERING_BINDING_COLUMN(m_iPlayLimitedTime)
        //길드정보--------------------------------
        _REORDERING_BINDING_COLUMN_PTR(result_info_guild_name_)
        _REORDERING_BINDING_COLUMN(m_GuildGuid)
        _REORDERING_BINDING_COLUMN(m_eGuildDuty)
        _REORDERING_BINDING_COLUMN_PTR(result_info_guild_nick_name_)
        _REORDERING_BINDING_COLUMN(m_EventUseTime)
        _REORDERING_BINDING_COLUMN_PTR(result_info_create_time_)
        _REORDERING_BINDING_COLUMN(m_byDelChk)
        _REORDERING_BINDING_COLUMN_PTR(result_info_del_date_)
        _REORDERING_BINDING_COLUMN(m_byExtraInventoryTabCount)
        _REORDERING_BINDING_COLUMN(m_ExtraEquipCashSlotCount)
        _REORDERING_BINDING_COLUMN(m_bTradeFlag)
        _REORDERING_BINDING_COLUMN(m_bWhisperFlag)
        _REORDERING_BINDING_COLUMN(m_byFreeInitCount)
        _REORDERING_BINDING_COLUMN(m_ChangeOfClassStep)

        _REORDERING_BINDING_COLUMN(m_bSummonedPet)
        _REORDERING_BINDING_COLUMN(m_bMAXIntimacy)
        _REORDERING_BINDING_COLUMN(m_PetItemPos)
        _REORDERING_BINDING_COLUMN(m_bMAXOptionRemainTime)

        _REORDERING_BINDING_COLUMN(m_bActiveEther)
        _REORDERING_BINDING_COLUMN(m_byEtherBulletPos) 
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _REORDERING_BINDING_COLUMN(m_iACVersion)
    #endif
        _REORDERING_BINDING_COLUMN(m_EventUseTimeNormal)
        _REORDERING_BINDING_COLUMN(m_iMaxSD)
        _REORDERING_BINDING_COLUMN(m_iSD)
    #ifdef _NA_1419_20100121_ACCUMULATE_POINT
        _REORDERING_BINDING_COLUMN(m_wAccumulateYear)
        _REORDERING_BINDING_COLUMN(m_dwAccumulateTime)
        _REORDERING_BINDING_COLUMN(m_wAccumulatePoint)
    #endif
    #ifdef _NA_002050_20110216_ADD_GENDER
        _REORDERING_BINDING_COLUMN(gender)
    #endif
	#ifdef _NA_003027_20111013_HONOR_SYSTEM
        _REORDERING_BINDING_COLUMN(honor_title)
	#endif
        //_NA_004035_20120227_COSTUME_ITEM
        _REORDERING_BINDING_COLUMN(is_equipped_costume)
        _REORDERING_BINDING_COLUMN(costume_item_pos)
        _REORDERING_BINDING_COLUMN(costume_item_code)

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        _REORDERING_BINDING_COLUMN(is_equipped_deco_costume)
        _REORDERING_BINDING_COLUMN(deco_costume_item_pos)
        _REORDERING_BINDING_COLUMN(deco_costume_item_code)
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

    _END_BINDING_DATA();
    //
    inline int GetResult() {
        return pParam[0].m_Result;
    }
    //
}; //end of class 'Query_Char_Create'

//==================================================================================================

class Query_Char_Destroy : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Char_Destroy);
    enum
    { 
        UPDATE_DELETE_PARAM_ROW_NUM = 1,
        UPDATE_DELETE_PARAM_COL_NUM = 1,
    };
public:
    struct sQUERY_DELETE_PARAM
    {
        int m_Result;
    } pParam[UPDATE_DELETE_PARAM_ROW_NUM];
public:
    _BEGIN_PARAM_DATA(sQUERY_DELETE_PARAM, pParam, UPDATE_DELETE_PARAM_COL_NUM)
        _BINDING_PARAM(0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

    inline int GetResult() { return pParam[0].m_Result; }
    //inline VOID SetSlotIndex(BYTE v)  { m_bySlotIndex = v; }
    //inline BYTE GetSlotIndex() { return m_bySlotIndex; }
    inline VOID SetCharGuid(CHARGUID g) { m_CharGuid = g; }
    inline CHARGUID GetCharGuid() const { return m_CharGuid;    }
    inline const TCHAR* GetCharName() const { return m_tszCharName; }
    inline void SetCharName(const TCHAR* p) {
        _tcsncpy(m_tszCharName, p, _countof(m_tszCharName));
        m_tszCharName[_countof(m_tszCharName) - 1] = _T('\0');
    }

private:
    //BYTE m_bySlotIndex;
    CHARGUID m_CharGuid;
    TCHAR m_tszCharName[MAX_CHARNAME_LENGTH + 1];
}; //end of class 'Query_Char_Destroy'

//==================================================================================================

#ifdef __NA_001044_ADD_CUSTOM_UI
class Query_Char_NameCheck : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Char_NameCheck);
    enum
    {
        UPDATE_PARAM_ROW_NUM    = 1,
        UPDATE_PARAM_COL_NUM    = 1,
    };
public:
    struct sQUERY_PARAM
    {
        int m_Exist;
    } pParam[UPDATE_PARAM_ROW_NUM];

public:
    _BEGIN_PARAM_DATA(sQUERY_PARAM, pParam, UPDATE_PARAM_COL_NUM)
        _BINDING_PARAM(0, m_Exist, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

    inline int GetResult() { return pParam[0].m_Exist; }
    inline VOID SetSearchID(char* szName) {
        strncpy(m_pszCharacterID, szName, MAX_CHARNAME_LENGTH+1);
        m_pszCharacterID[MAX_CHARNAME_LENGTH] = '\0';
    }
    inline char* GetSearchID() { return m_pszCharacterID;   }
private:
    char m_pszCharacterID[MAX_CHARNAME_LENGTH+1];
}; //end of class 'Query_Char_NameCheck'
#endif

//==================================================================================================

class Query_Char_Recover : public QueryForUser
{
    enum
    { 
        UPDATE_PARAM_ROW_NUM    = 1,
        UPDATE_PARAM_COL_NUM    = 1,
    };
    __DECL_SAFECUSTOMPOOL_PTR(Query_Char_Recover);
public:
    struct sQUERY_PARAM
    {
        int     m_Result;
    } pParam[UPDATE_PARAM_ROW_NUM];
public:
    _BEGIN_PARAM_DATA(sQUERY_PARAM, pParam, UPDATE_PARAM_COL_NUM)
        _BINDING_PARAM(0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

    inline int GetResult() { return pParam[0].m_Result; }
    inline VOID SetCharGuid(CHARGUID g) { m_CharGuid = g; }
    inline CHARGUID GetCharGuid() { return m_CharGuid;    }
private:
    CHARGUID m_CharGuid;
}; //end of class 'Query_Char_Recover'


class Query_User_Select_PetList : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_User_Select_PetList);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 2, // @RC, @UserGuid

        RESULT_ROW_NUM  = MAX_CHARACTER_SLOT_NUM, // max number of characters in a character scene
        RESULT_COL_NUM	= 8,
    };
    struct sQueryParam {
        int result_;
        CHARGUID user_guid_;
    } parameters_[PARAM_ROW_NUM];

    struct sQueryResult : public nsQuery::ItemSlotResultRecord
    {
        int char_guid_;
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, user_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, char_guid_)
        _BINDING_COLUMN(1, slot_type_)
        _BINDING_COLUMN(2, pos_)
        _BINDING_COLUMN(3, serial_)
        _BINDING_COLUMN(4, code_)
        _BINDING_COLUMN(5, dura_or_num_)
        _BINDING_COLUMN(6, version_)
        _BINDING_COLUMN_PTR(7, option_stream_)
    _END_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
};


#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE

class Query_CharName_Change : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_CharName_Change);
    enum
    { 
        UPDATE_PARAM_ROW_NUM    = 1,
        UPDATE_PARAM_COL_NUM    = 1,
    };
public:
    struct sQUERY_PARAM
    {
        int  return_value_;        
    } pParam[UPDATE_PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQUERY_PARAM, pParam, UPDATE_PARAM_COL_NUM)
        _BINDING_PARAM(0, return_value_,     SQL_PARAM_OUTPUT)        
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

    int         GetReturn() const { return pParam[0].return_value_; }    
    
    char*       GetCharName() { return char_name_; }
    void        SetCharName(const char* char_name) 
    {
        strncpy(char_name_, char_name, MAX_CHARNAME_LENGTH+1);
        char_name_[MAX_CHARNAME_LENGTH] = '\0';
    }

    SLOTIDX     GetSlotIndex() {return slot_index_;}
    void        SetSlotIndex(SLOTIDX slot_index) {slot_index_ = slot_index;}

    CHARGUID    GetCharGuid() {return char_guid_;}
    void        SetCharGuid(CHARGUID char_guid) {char_guid_ = char_guid;}
    
    bool ResultSuccess() const { return (pParam[0].return_value_ == 0); }
private:    
    char        char_name_[MAX_CHARNAME_LENGTH+1];
    SLOTIDX     slot_index_;
    CHARGUID    char_guid_;
}; 

#endif //_JP_20110222_INGAME_CHARNAME_CHANGE


#ifdef _NA_0_20110329_GAMEOPTION_SETTING
class Query_GameOption_Select : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GameOption_Select);

    enum 
    {
        _MAKE_REORDERING_PARAM_NO_START_(result_, 0),
        _MAKE_REORDERING_PARAM_NO_(user_guid_),
        _MAKE_REORDERING_PARAM_NO_(character_guid_),
        _MAKE_REORDERING_PARAM_NO_(gameoption_type_),
        _MAKE_REORDERING_PARAM_NO_COUNTS_()
    };

    enum 
    {
        _MAKE_REORDERING_BINDING_NO_START_(gameoption_size_, 0),
        _MAKE_REORDERING_BINDING_NO_PTR_(gameoption_data_),
        _MAKE_REORDERING_BINDING_NO_COUNTS_()
    };


public:
    enum
    { 
        PARAM_ROW_NUM = 1,
        PARAM_COL_NUM = _MAKE_REORDERING_PARAM_NO_COUNTS_(),
        RESULT_ROW_NUM = 1,
        RESULT_COL_NUM = _MAKE_REORDERING_BINDING_NO_COUNTS_(),
    };

    struct sQueryParam
    {
        int      result_;
        USERGUID user_guid_;
        CHARGUID character_guid_;
        int      gameoption_type_;
    } parameter_[PARAM_ROW_NUM];

    struct sQueryResult
    {
        int gameoption_size_;
        uint8_t gameoption_data_[MAX_GAMEOPTIONSTREAM_SIZE];
    } result_[RESULT_ROW_NUM];

    ulong length_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameter_, PARAM_COL_NUM)
        _REORDERING_PARAM(result_, SQL_PARAM_OUTPUT)
        _REORDERING_PARAM(user_guid_, SQL_PARAM_INPUT)
        _REORDERING_PARAM(character_guid_, SQL_PARAM_INPUT)
        _REORDERING_PARAM(gameoption_type_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_, length_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _REORDERING_BINDING_COLUMN(gameoption_size_)
        _REORDERING_BINDING_COLUMN_PTR(gameoption_data_)
    _END_BINDING_DATA();
};

class Query_GameOption_Update : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GameOption_Update);

    enum 
    {
        _MAKE_REORDERING_PARAM_NO_START_(result_, 0),
        _MAKE_REORDERING_PARAM_NO_(user_guid_),
        _MAKE_REORDERING_PARAM_NO_(character_guid_),
        _MAKE_REORDERING_PARAM_NO_(gameoption_type_),
        _MAKE_REORDERING_PARAM_NO_(stream_size_), 
        _MAKE_REORDERING_PARAM_NO_PTR_(gameoption_stream_),
        _MAKE_REORDERING_PARAM_NO_COUNTS_()
    };

public:
    enum
    { 
        QUERY_RESULT_SUCCESS = 0,
        PARAM_ROW_NUM = 1,
        PARAM_COL_NUM = _MAKE_REORDERING_PARAM_NO_COUNTS_(),
    };

    struct sQueryParam
    {
        int  result_;
        USERGUID user_guid_;
        CHARGUID character_guid_;
        uint    gameoption_type_;
        uint    stream_size_;
        uint8_t gameoption_stream_[MAX_GAMEOPTIONSTREAM_SIZE];
    } parameter_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameter_, PARAM_COL_NUM)
        _REORDERING_PARAM(result_, SQL_PARAM_OUTPUT)
        _REORDERING_PARAM(user_guid_, SQL_PARAM_INPUT)
        _REORDERING_PARAM(character_guid_, SQL_PARAM_INPUT)
        _REORDERING_PARAM(gameoption_type_, SQL_PARAM_INPUT)
        _REORDERING_PARAM(stream_size_, SQL_PARAM_INPUT)
        _REORDERING_PARAM_PTR(gameoption_stream_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

    bool ResultSuccess() const { return (parameter_[0].result_ == QUERY_RESULT_SUCCESS); }
};
#endif //_NA_0_20110329_GAMEOPTION_SETTING


#endif // __AGENT_SERVER_QUERY_H__

//==================================================================================================