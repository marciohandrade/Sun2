#ifndef __DATABAES_PROXY_QUERY_H__
#define __DATABAES_PROXY_QUERY_H__
#pragma once

#include "StructInQuery.h"
#include "QueryPoolFactoryDefine.h"

//==================================================================================================
/// 쿼리를 추가할 때 해야 할 일!
/// 1. 쿼리 클래스를 만든다.
/// 2. 쿼리 풀 클래스에다가 스크립트에 맞추어 등록한다.
/// 3. 해더파일을 만들었을 경우 -> Stdafx.h에 쿼리 클래스 파일 헤더를 등록한다.
//==================================================================================================

//==================================================================================================
/// select query의 prototype
//==================================================================================================
//class TestQuery : public QueryResult
//{
//	__DECL_SAFECUSTOMPOOL_PTR(TestQuery)
//	enum
//	{ 
//		ACCOUNT_ROW_NUM = 10,
//		ACCOUNT_COL_NUM = 9,
//	};
//	
//	struct tag_RETURN
//	{
//		INT		returnOut;
//		DWORD	paramOut;
//	} pReturn[1];
//
//	struct tag_SELECT
//	{
//		char	pName[50];
//	} pSelect[1];
//
//	ULONG uSerialLength[1];
//
//	_BEGIN_PARAM_DATA(tag_RETURN, pReturn, 2)
//	_BINDING_PARAM( 0, returnOut, SQL_PARAM_OUTPUT )
//	_BINDING_PARAM( 1, paramOut, SQL_PARAM_OUTPUT )
//	_END_PARAM_DATA()
//
//	_BEGIN_BINDING_DATA(tag_SELECT, pSelect, uSerialLength, 1, 1)
//	_BINDING_COLUMN_PTR(0, pName)
//	_END_BINDING_DATA()
//
//};

//class Query_Test_Insert : public QueryForUser
//{
//	__DECL_SAFECUSTOMPOOL_PTR(Query_Test_Insert)
//public:
//	enum
//	{
//		PARAM_ROW_NUM	= 1,
//		PARAM_COL_NUM	= 1,
//	};
//
//	struct sPARAM
//	{
//		INT		m_Result;
//	} pParam[PARAM_ROW_NUM];
//
//
//	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
//		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
//	_END_PARAM_DATA			()
//
//	_INIT_BINDING_DATA()
//
//	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
//};

//==================================================================================================
/// Insert query의 prototype
//==================================================================================================
//class AccountInsertQuery : public QueryResult
//{
//	__DECL_SAFECUSTOMPOOL_PTR(AccountInsertQuery)
//	/// 사용하는 사람이 작성해야 하는 코드
//	_INIT_BINDING_DATA()
//
//
//};


//==================================================================================================
// DBProxyServer가 부팅될때 쿼리
//==================================================================================================
class CreateItemSerialQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(CreateItemSerialQuery)
public:
	enum
	{ 
		ITEMSERIALQ_ROW_NUM = 1,
		ITEMSERIALQ_COL_NUM = 1,
	};
	struct sQUERY_ITEM_SERIAL
	{
		DBSERIAL	m_dwCurSerial;
	} pItemSerial[ITEMSERIALQ_ROW_NUM];

	ULONG uItemSerialLength[ITEMSERIALQ_COL_NUM];

	_INIT_PARAM_DATA()

	_BEGIN_BINDING_DATA(sQUERY_ITEM_SERIAL, pItemSerial, uItemSerialLength, ITEMSERIALQ_ROW_NUM, ITEMSERIALQ_COL_NUM)		
		_BINDING_COLUMN(0, m_dwCurSerial)
	_END_BINDING_DATA()

	DBSERIAL GetCurDBSerial() { return pItemSerial[0].m_dwCurSerial; }
};

//==================================================================================================
// NOTE: Query_Char_Update, 유저(플레이어)의 정보를 주기적으로 저장하는 쿼리
//==================================================================================================
class Query_Char_Update : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Char_Update);
    //
    enum {
        _MAKE_REORDERING_PARAM_NO_START_(result_, 0),
        _MAKE_REORDERING_PARAM_NO_(m_UserGuid),
        _MAKE_REORDERING_PARAM_NO_(m_CharGuid),
        //_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM = {
        _MAKE_REORDERING_PARAM_NO_(m_byHeight),
        _MAKE_REORDERING_PARAM_NO_(m_byFace),
        _MAKE_REORDERING_PARAM_NO_(m_byHair),
        //}
        _MAKE_REORDERING_PARAM_NO_(m_LV),
        _MAKE_REORDERING_PARAM_NO_(m_sStrength),
        _MAKE_REORDERING_PARAM_NO_(m_sDexterity),
        _MAKE_REORDERING_PARAM_NO_(m_sVitality),
        _MAKE_REORDERING_PARAM_NO_(m_sInteligence),
        _MAKE_REORDERING_PARAM_NO_(m_sSpirit),
        _MAKE_REORDERING_PARAM_NO_(m_sSkillStat1),
        _MAKE_REORDERING_PARAM_NO_(m_sSkillStat2),
        _MAKE_REORDERING_PARAM_NO_(m_UserPoint),
        _MAKE_REORDERING_PARAM_NO_(m_i64Exp),
        _MAKE_REORDERING_PARAM_NO_(m_iMaxHP),
        _MAKE_REORDERING_PARAM_NO_(m_iHP),
        _MAKE_REORDERING_PARAM_NO_(m_iMaxMP),
        _MAKE_REORDERING_PARAM_NO_(m_iMP),
        _MAKE_REORDERING_PARAM_NO_(m_Money),
        _MAKE_REORDERING_PARAM_NO_(m_iRemainStat),
        _MAKE_REORDERING_PARAM_NO_(m_iRemainSkill),
        _MAKE_REORDERING_PARAM_NO_(m_iSelectStyleCode),
        _MAKE_REORDERING_PARAM_NO_(m_byPKState),
        _MAKE_REORDERING_PARAM_NO_(m_byCharState),
        _MAKE_REORDERING_PARAM_NO_(m_StateTime),
        _MAKE_REORDERING_PARAM_NO_(m_iRegion),
        _MAKE_REORDERING_PARAM_NO_(m_sX),
        _MAKE_REORDERING_PARAM_NO_(m_sY),
        _MAKE_REORDERING_PARAM_NO_(m_sZ),
        _MAKE_REORDERING_PARAM_NO_(m_iPreRegion),
        _MAKE_REORDERING_PARAM_NO_PTR_(param_info_title_id_),
        _MAKE_REORDERING_PARAM_NO_(m_TitleTime),
        _MAKE_REORDERING_PARAM_NO_(m_byInvisibleOpt),
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _MAKE_REORDERING_PARAM_NO_PTR_(m_pACStream),    
    #endif
        _MAKE_REORDERING_PARAM_NO_(m_byChaoState),
        _MAKE_REORDERING_PARAM_NO_(m_i64ChaoTime),
        _MAKE_REORDERING_PARAM_NO_(m_iPlayLimitedTime),
        _MAKE_REORDERING_PARAM_NO_(m_EventUseTime),
        _MAKE_REORDERING_PARAM_NO_(m_EventUseTimeNormal),

        _MAKE_REORDERING_PARAM_NO_(m_ExtraEquipCashSlotCount),
        _MAKE_REORDERING_PARAM_NO_(m_bTradeFlag),
        _MAKE_REORDERING_PARAM_NO_(m_bWhisperFlag),
        _MAKE_REORDERING_PARAM_NO_(m_byFreeInitCount),
        _MAKE_REORDERING_PARAM_NO_(m_ChangeOfClassStep),
        // Pet info.
        _MAKE_REORDERING_PARAM_NO_(m_bSummonedPet),
        _MAKE_REORDERING_PARAM_NO_(m_bMAXIntimacy),
        _MAKE_REORDERING_PARAM_NO_(m_PetItemPos),
        _MAKE_REORDERING_PARAM_NO_(m_bMAXOptionRemainTime),
        //
        _MAKE_REORDERING_PARAM_NO_(m_bActiveEther),
        _MAKE_REORDERING_PARAM_NO_(m_byEtherBulletPos),
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _MAKE_REORDERING_PARAM_NO_(m_iACVersion),
    #endif
        _MAKE_REORDERING_PARAM_NO_(m_iMaxSD),
        _MAKE_REORDERING_PARAM_NO_(m_iSD),
    #ifdef _NA_1419_20100121_ACCUMULATE_POINT
        _MAKE_REORDERING_PARAM_NO_(m_wAccumulateYear),
        _MAKE_REORDERING_PARAM_NO_(m_dwAccumulateTime),
        _MAKE_REORDERING_PARAM_NO_(m_wAccumulatePoint),
    #endif
    #ifdef _NA_002050_20110216_ADD_GENDER
        _MAKE_REORDERING_PARAM_NO_(gender), 
    #endif
    #ifdef _NA_003027_20111013_HONOR_SYSTEM
        _MAKE_REORDERING_PARAM_NO_(honor_title),
    #endif
        //_NA_004035_20120227_COSTUME_ITEM
        _MAKE_REORDERING_PARAM_NO_(is_equipped_costume),
        _MAKE_REORDERING_PARAM_NO_(costume_item_pos),
        _MAKE_REORDERING_PARAM_NO_(costume_item_code),

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        _MAKE_REORDERING_PARAM_NO_(is_equipped_deco_costume),
        _MAKE_REORDERING_PARAM_NO_(deco_costume_item_pos),
        _MAKE_REORDERING_PARAM_NO_(deco_costume_item_code),
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

        _MAKE_REORDERING_PARAM_NO_COUNTS_() // end of records. it equal to number of records
    };
    //
public:
    enum
    { 
        UPDATE_CHARACTER_PARAM_ROW_NUM  = 1,
        UPDATE_CHARACTER_PARAM_COL_NUM  = _MAKE_REORDERING_PARAM_NO_COUNTS_(),
    };
    //
    struct sQueryParam : public sQUERY_CHARACTER
    {
        int result_;
        int _dummy_field_;
    } parameter_[UPDATE_CHARACTER_PARAM_ROW_NUM];
    //
    _BEGIN_PARAM_DATA(sQueryParam, parameter_, UPDATE_CHARACTER_PARAM_COL_NUM)
        _REORDERING_PARAM(result_, SQL_PARAM_OUTPUT)
        _REORDERING_PARAM(m_UserGuid, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_CharGuid, SQL_PARAM_INPUT)
        //_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM = {
        _REORDERING_PARAM(m_byHeight, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_byFace, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_byHair, SQL_PARAM_INPUT)
        //}
        _REORDERING_PARAM(m_LV, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_sStrength, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_sDexterity, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_sVitality, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_sInteligence, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_sSpirit, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_sSkillStat1, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_sSkillStat2, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_UserPoint, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_i64Exp, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_iMaxHP, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_iHP, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_iMaxMP, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_iMP, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_Money, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_iRemainStat, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_iRemainSkill, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_iSelectStyleCode, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_byPKState, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_byCharState, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_StateTime, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_iRegion, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_sX, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_sY, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_sZ, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_iPreRegion, SQL_PARAM_INPUT)
        _REORDERING_PARAM_PTR(param_info_title_id_, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_TitleTime, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_byInvisibleOpt, SQL_PARAM_INPUT)
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _REORDERING_PARAM_PTR(m_pACStream, SQL_PARAM_INPUT)
    #endif
        _REORDERING_PARAM(m_byChaoState, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_i64ChaoTime, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_iPlayLimitedTime, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_EventUseTime, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_EventUseTimeNormal, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_ExtraEquipCashSlotCount, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_bTradeFlag, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_bWhisperFlag, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_byFreeInitCount, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_ChangeOfClassStep, SQL_PARAM_INPUT)
        // Pet info.
        _REORDERING_PARAM(m_bSummonedPet, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_bMAXIntimacy, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_PetItemPos, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_bMAXOptionRemainTime, SQL_PARAM_INPUT)
        //
        _REORDERING_PARAM(m_bActiveEther, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_byEtherBulletPos, SQL_PARAM_INPUT)
    #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        _REORDERING_PARAM(m_iACVersion, SQL_PARAM_INPUT)
    #endif
        _REORDERING_PARAM(m_iMaxSD, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_iSD, SQL_PARAM_INPUT)
    #ifdef _NA_1419_20100121_ACCUMULATE_POINT
        _REORDERING_PARAM(m_wAccumulateYear, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_dwAccumulateTime, SQL_PARAM_INPUT)
        _REORDERING_PARAM(m_wAccumulatePoint, SQL_PARAM_INPUT)
    #endif
    #ifdef _NA_002050_20110216_ADD_GENDER
        _REORDERING_PARAM(gender, SQL_PARAM_INPUT)
    #endif
    #ifdef _NA_003027_20111013_HONOR_SYSTEM
        _REORDERING_PARAM(honor_title, SQL_PARAM_INPUT)
    #endif
        //_NA_004035_20120227_COSTUME_ITEM
        _REORDERING_PARAM(is_equipped_costume, SQL_PARAM_INPUT)
        _REORDERING_PARAM(costume_item_pos, SQL_PARAM_INPUT)
        _REORDERING_PARAM(costume_item_code, SQL_PARAM_INPUT)

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        _REORDERING_PARAM(is_equipped_deco_costume, SQL_PARAM_INPUT)
        _REORDERING_PARAM(deco_costume_item_pos, SQL_PARAM_INPUT)
        _REORDERING_PARAM(deco_costume_item_code, SQL_PARAM_INPUT)
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
    //
public:
    bool ResultSuccess() const { return (parameter_[0].result_ == 0); }

    CHARGUID char_guid() const { return char_guid_; }
    void set_char_guid(CHARGUID char_guid) { char_guid_ = char_guid; }
private:
    CHARGUID char_guid_;
};

//_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
//==================================================================================================
// 유저(플레이어)의 정보를 주기적으로 저장하는 쿼리
//==================================================================================================
class Query_Char_Update_Inventory : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Char_Update_Inventory);
    //
    enum {
        _MAKE_REORDERING_PARAM_NO_START_(result_, 0),
        _MAKE_REORDERING_PARAM_NO_(char_guid_),
        _MAKE_REORDERING_PARAM_NO_(extra_inven_tab_count_),
        _MAKE_REORDERING_PARAM_NO_(inventory_lock_),
        _MAKE_REORDERING_PARAM_NO_PTR_(password_),
        _MAKE_REORDERING_PARAM_NO_(config_version),
        _MAKE_REORDERING_PARAM_NO_(config01),
        _MAKE_REORDERING_PARAM_NO_(config02),
        _MAKE_REORDERING_PARAM_NO_(config03),
        _MAKE_REORDERING_PARAM_NO_(config04),
        _MAKE_REORDERING_PARAM_NO_(config05),
        _MAKE_REORDERING_PARAM_NO_(config06),
        _MAKE_REORDERING_PARAM_NO_(config07),
        _MAKE_REORDERING_PARAM_NO_(config08),
        _MAKE_REORDERING_PARAM_NO_(config09),
        _MAKE_REORDERING_PARAM_NO_(config10),
        //
        _MAKE_REORDERING_PARAM_NO_COUNTS_()
    };
    //
public:
    enum ePostAction {
        ePostAction_None = 0,
        ePostAction_Default,    // query notification
        ePostAction_Returns,    // query & reply
        ePostAction_Diconnect   // query & disconnect
    };
    //
    enum
    { 
        UPDATE_CHARACTER_PARAM_ROW_NUM  = 1,
        UPDATE_CHARACTER_PARAM_COL_NUM  = _MAKE_REORDERING_PARAM_NO_COUNTS_(),
    };
    //
    struct sQueryParam
    {
        int result_;
        CHARGUID char_guid_;
        uint8_t inventory_lock_;
        uint8_t extra_inven_tab_count_;
    #pragma pack(push, 1)
        char password_[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH];
        char password_null_terminated_[1];
    #pragma pack(pop)
        //
        uint8_t config_version; // tiny int
        short config01; // small int
        short config02; // small int
        short config03; // small int
        short config04; // small int
        short config05; // small int
        short config06; // small int
        short config07; // small int
        short config08; // small int
        short config09; // small int
        short config10; // small int
        //
    } parameters_[UPDATE_CHARACTER_PARAM_ROW_NUM];
    //
    _BEGIN_PARAM_DATA(sQueryParam, parameters_, UPDATE_CHARACTER_PARAM_COL_NUM)
        _REORDERING_PARAM(result_, SQL_PARAM_OUTPUT)
        _REORDERING_PARAM(char_guid_, SQL_PARAM_INPUT)
        _REORDERING_PARAM(extra_inven_tab_count_, SQL_PARAM_INPUT)
        _REORDERING_PARAM(inventory_lock_, SQL_PARAM_INPUT)
        _REORDERING_PARAM_PTR(password_, SQL_PARAM_INPUT)
        _REORDERING_PARAM(config_version, SQL_PARAM_INPUT)
        _REORDERING_PARAM(config01, SQL_PARAM_INPUT)
        _REORDERING_PARAM(config02, SQL_PARAM_INPUT)
        _REORDERING_PARAM(config03, SQL_PARAM_INPUT)
        _REORDERING_PARAM(config04, SQL_PARAM_INPUT)
        _REORDERING_PARAM(config05, SQL_PARAM_INPUT)
        _REORDERING_PARAM(config06, SQL_PARAM_INPUT)
        _REORDERING_PARAM(config07, SQL_PARAM_INPUT)
        _REORDERING_PARAM(config08, SQL_PARAM_INPUT)
        _REORDERING_PARAM(config09, SQL_PARAM_INPUT)
        _REORDERING_PARAM(config10, SQL_PARAM_INPUT)
        //
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();

public:
    inline bool ResultSuccess() const { return parameters_[0].result_ == 0; }

    CHARGUID char_guid() const { return char_guid_; }
    void set_char_guid(CHARGUID char_guid) { char_guid_ = char_guid; }
    void SetPostAction(ePostAction post_action) { post_action_ = post_action; }
    ePostAction post_action() const { return post_action_; };
private:
    CHARGUID char_guid_;
    ePostAction post_action_;
};

// (CHANGES) (f100414.1L) (WAVERIX) remove unused query section
//==================================================================================================
// 유저(플레이어)의 정보를 종료시에 저장하는 쿼리
//==================================================================================================
//class Query_Char_Logout : public QueryForUser

//==================================================================================================
// 유저(플레이어)의 창고정보를 주기적 혹은 종료시에 저장하는 쿼리
//==================================================================================================
class Query_Warehouse_Update : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Warehouse_Update);
    enum {
        _MAKE_REORDERING_PARAM_NO_START_(result_, 0),
        _MAKE_REORDERING_PARAM_NO_(user_guid_),
        _MAKE_REORDERING_PARAM_NO_(money_),
        //
        _MAKE_REORDERING_PARAM_NO_COUNTS_() // end of records. it equal to number of records
    };
    enum {
    };
public:
    enum
    { 
        QUERY_RESULT_SUCCEEDED  = 0,
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = _MAKE_REORDERING_PARAM_NO_COUNTS_(),
    };
    struct sQueryParam
    {
        int result_;
        USERGUID user_guid_;
        int64 money_;
    } parameters_[PARAM_ROW_NUM];
    //
    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _REORDERING_PARAM(result_, SQL_PARAM_OUTPUT)
        _REORDERING_PARAM(user_guid_, SQL_PARAM_INPUT)
        _REORDERING_PARAM(money_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
    //
    bool ResultSuccess() const { return (parameters_[0].result_ == QUERY_RESULT_SUCCEEDED); }
    const TCHAR* GetCharName() const { return char_name_; }
    void SetCharName(const TCHAR* p) {
        _tcsncpy(char_name_, p, _countof(char_name_));
        char_name_[_countof(char_name_) - 1] = _T('\0');
    }
private:
    TCHAR char_name_[MAX_CHARNAME_LENGTH + 1];
};

//==================================================================================================
// 매칭 DB Query 들
//==================================================================================================
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
class Query_MatchChar_Update : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_MatchChar_Update)
public:
	_INIT_PARAM_DATA()
	_INIT_BINDING_DATA()

};
class Query_MatchChar_Delete : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_MatchChar_Delete)
public:
	_INIT_PARAM_DATA()
	_INIT_BINDING_DATA()
};


class Query_MatchFatigue_Update : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_MatchFatigue_Update)
public:

	enum
	{
		FATIGUE_PARAM_ROW_NUM	= 1,
		FATIGUE_PARAM_COL_NUM	= 2,
	};

	struct sQUERY_FATIGUE_PARAM
	{
		INT			m_nResult;
	}pParam[FATIGUE_PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA(sQUERY_FATIGUE_PARAM, pParam, FATIGUE_PARAM_COL_NUM )
		_BINDING_PARAM(0, m_nResult, SQL_PARAM_OUTPUT)
		_END_PARAM_DATA()

		_INIT_BINDING_DATA()

		inline BOOL ResultSuccess() { return (pParam[0].m_nResult==0); }
};

class Query_MatchFatigue_Select : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_MatchFatigue_Select)
public:

	enum
	{
		FATIGUE_PARAM_ROW_NUM	= 1,
		FATIGUE_PARAM_COL_NUM	= 2,

		FATIGUE_RESULT_ROW_NUM	= 1,
		FATIGUE_RESULT_COL_NUM	= 2,
	};

	struct sQUERY_FATIGUE_PARAM
	{
		INT			m_nResult;
	}pParam[FATIGUE_PARAM_ROW_NUM];

	struct sQUERY_FATIGUE_SELECT
	{
		INT			m_nPlayTime;
		INT			m_nOfflineTime;
	}pSelect[FATIGUE_RESULT_ROW_NUM];

	ULONG uFatigueLength[FATIGUE_RESULT_COL_NUM];

	_BEGIN_PARAM_DATA(sQUERY_FATIGUE_PARAM, pParam, FATIGUE_PARAM_COL_NUM )
		_BINDING_PARAM(0, m_nResult, SQL_PARAM_OUTPUT)
		_END_PARAM_DATA()

		_BEGIN_BINDING_DATA(sQUERY_FATIGUE_SELECT, pSelect, uFatigueLength, FATIGUE_RESULT_ROW_NUM, FATIGUE_RESULT_COL_NUM)
		_BINDING_COLUMN(0, m_nPlayTime)
		_BINDING_COLUMN(1, m_nOfflineTime)
		_END_BINDING_DATA()

		inline BOOL ResultSuccess() { return (pParam[0].m_nResult==0); }

};
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

class GuildWarehouseUpdateQuery : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(GuildWarehouseUpdateQuery);
    enum {
        _MAKE_REORDERING_PARAM_NO_START_(result_, 0),
        _MAKE_REORDERING_PARAM_NO_(guild_guid_),
        _MAKE_REORDERING_PARAM_NO_(money_),
        //
        _MAKE_REORDERING_PARAM_NO_COUNTS_()
    };

public:
    enum
    { 
        PARAM_ROW_NUM = 1,
        PARAM_COL_NUM = _MAKE_REORDERING_PARAM_NO_COUNTS_(),

        QUERY_RESULT_SUCCEEDED = 0,
    };
    //
    struct sQueryParam : public nsQuery::GuildWarehouseResultRecord
    {
        int result_;
        GUILDGUID guild_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _REORDERING_PARAM(result_, SQL_PARAM_OUTPUT)
        _REORDERING_PARAM(guild_guid_, SQL_PARAM_INPUT)
        _REORDERING_PARAM(money_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

    bool ResultSuccess() const { return (parameters_[0].result_ == QUERY_RESULT_SUCCEEDED); }
    GUILDGUID GetGuildGuid() const { return parameters_[0].guild_guid_; }
    CHARGUID char_guid() const { return char_guid_; }
    void set_char_guid(CHARGUID actor_char_guid) { char_guid_ = actor_char_guid; };
    //
private:
    CHARGUID char_guid_;
};


//!~ _NA_006372_20130108_BADGE_ITEM
enum CharacterExtraType
{
    kExtraType_ToggleItemInfo = 1, //_NA_006372_20130108_BADGE_ITEM
    kExtraType_BattleGroundRecordInfo = 2, // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    kExtraType_DailyQuestResetTime = 3, //_NA_000000_20130521_DAILY_QUEST_RESET_ITEM
    kExtraType_GoldRushRecordInfo = 4,  //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    kExtraType_MiniGameBingoInfo = 5,   //_NA_008404_20150712_MINIGAME_SYSTEM
    kExtraType_Max,
};
struct CharacterExtraInfoData
{
    int extra_info_type_;
    CHAR extra_info_data_[1024];
};
class Query_CharacterExtraInfo_Select : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_CharacterExtraInfo_Select);

    enum { 
        RESULT_COL_NUM = 2,
        RESULT_ROW_NUM = (kExtraType_Max - 1),
    };

    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
    } parameters_;

    struct sQueryResult {
        CharacterExtraInfoData extra_info_data_;
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters_, 2/*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, extra_info_data_.extra_info_type_)
        _BINDING_COLUMN(1, extra_info_data_.extra_info_data_)
    _END_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_.result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_CharacterExtraInfo_Select;
    }
};

#ifdef _NA_007932_20150106_USER_EXTRA_INFO
enum UserExtraType
{
    // UserExtraInfo 추가시 수정
    kUserExtraType_AttendanceInfo = 1,      // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    kUserExtraType_Max,
};
struct UserExtraInfoData
{
    int extra_info_type_;
    CHAR extra_info_data_[1024];
};
class Query_UserExtraInfo_Select : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_UserExtraInfo_Select);

    enum { 
        RESULT_COL_NUM = 2,
        RESULT_ROW_NUM = (kUserExtraType_Max - 1),
    };

    struct sQueryParam {
        int result_;
        USERGUID user_guid_;
    } parameters_;

    struct sQueryResult {
        UserExtraInfoData extra_info_data_;
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters_, 2/*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, user_guid_, SQL_PARAM_INPUT)
        _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, extra_info_data_.extra_info_type_)
        _BINDING_COLUMN(1, extra_info_data_.extra_info_data_)
        _END_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_.result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_UserExtraInfo_Select;
    }
};
class Query_UserExtraInfo_Update : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_UserExtraInfo_Update);

    struct sQueryParam {
        int result_;
        USERGUID user_guid_;
        UserExtraInfoData extra_info_data_;
    } parameters_;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters_, 4/*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, user_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, extra_info_data_.extra_info_type_, SQL_PARAM_INPUT)
        _BINDING_PARAM_PTR(3, extra_info_data_.extra_info_data_, SQL_PARAM_INPUT)
        _END_PARAM_DATA();

    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_.result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_UserExtraInfo_Update;
    }
};
#endif // _NA_007932_20150106_USER_EXTRA_INFO
class Query_CharacterExtraInfo_Update : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_CharacterExtraInfo_Update);

    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
        CharacterExtraInfoData extra_info_data_;
    } parameters_;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters_, 4/*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, extra_info_data_.extra_info_type_, SQL_PARAM_INPUT)
        _BINDING_PARAM_PTR(3, extra_info_data_.extra_info_data_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_.result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_CharacterExtraInfo_Update;
    }
};
//~! _NA_006372_20130108_BADGE_ITEM

#ifdef _NA_007086_20140318_MONSTERBOOK
class Query_CharacterMonsterBook_Select : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_CharacterMonsterBook_Select);

    enum { 
        RESULT_COL_NUM = 1,
        RESULT_ROW_NUM = 1000,
    };

    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
    } parameters_;

    struct sQueryResult {
        DWORD monster_code;
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters_, 2/*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, monster_code)
    _END_BINDING_DATA();

public:
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_CharacterMonsterBook_Select;
    }
};

class Query_CharacterMonsterBook_Update : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_CharacterMonsterBook_Update);

    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
        DWORD monster_code_;
    } parameters_;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters_, 3/*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, monster_code_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();
public:
    
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_CharacterMonsterBook_Update;
    }
};
#endif //_NA_007086_20140318_MONSTERBOOK

#endif // __DATABAES_PROXY_QUERY_H__