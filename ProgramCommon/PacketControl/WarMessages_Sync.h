#pragma once

#include <StructInPacket.h>
#include <PacketStruct_CG.h>
#include <PacketControl/WarMessages.h>

namespace nsSync {
;

enum eSYNC_OBJECT
{
    //SYNC_MONSTER_ENTER_BRD            = PROTOCOL_GENERATOR_SYNC_010,
    //SYNC_MONSTER_DIR_ENTER_BRD        = PROTOCOL_GENERATOR_SYNC_013,
    //SYNC_PLAYER_LEAVE_BRD             = PROTOCOL_GENERATOR_SYNC_014,
    //SYNC_MONSTER_LEAVE_BRD            = PROTOCOL_GENERATOR_SYNC_015,
    //SYNC_SUMMONED_ENTER_BRD           = PROTOCOL_GENERATOR_SYNC_017,
    //SYNC_MAPNPC_ENTER_BRD             = PROTOCOL_GENERATOR_SYNC_019,
    //SYNC_FIELDITEM_ENTER_BRD          = PROTOCOL_GENERATOR_SYNC_021,
    //SYNC_FIELDITEM_LEAVE_BRD          = PROTOCOL_GENERATOR_SYNC_023,

    //SYNC_BOSS_MONSTER_ENTER_BRD       = PROTOCOL_GENERATOR_SYNC_025,
    //SYNC_NPC_JUMPRESULT_BRD           = PROTOCOL_GENERATOR_SYNC_027,
    //SYNC_LUCKY_MONSTER_RUNAWY_BRD     = PROTOCOL_GENERATOR_SYNC_029,
    //SYNC_TOTEMNPC_ENTER_BRD           = PROTOCOL_GENERATOR_SYNC_031,
};

enum eSYNC_POS
{
    //SYNC_MOVE_NAK                     = PROTOCOL_GENERATOR_SYNC_055,
    SYNC_MOVE_BRD                       = PROTOCOL_GENERATOR_SYNC_057,
    SYNC_MOVE_THRUST_BRD                = PROTOCOL_GENERATOR_SYNC_059,
    SYNC_TARGET_MOVE_BRD                = PROTOCOL_GENERATOR_SYNC_061,
    SYNC_KBMOVE_BRD                     = PROTOCOL_GENERATOR_SYNC_063,
    SYNC_NPC_JUMP_BRD                   = PROTOCOL_GENERATOR_SYNC_065,
    SYNC_STOP_BRD                       = PROTOCOL_GENERATOR_SYNC_067,
    //SYNC_NPC_STOP_BRD                 = PROTOCOL_GENERATOR_SYNC_069,
    //SYNC_FORCED_WARP_BRD              = PROTOCOL_GENERATOR_SYNC_071,
    SYNC_FAST_KBMOVE_BRD                = PROTOCOL_GENERATOR_SYNC_073,
    //SYNC_JUMP_ACK                     = PROTOCOL_GENERATOR_SYNC_075,
    //SYNC_JUMP_NAK                     = PROTOCOL_GENERATOR_SYNC_077,
    SYNC_JUMP_BRD                       = PROTOCOL_GENERATOR_SYNC_079,
    //SYNC_JUMP_STOP_NAK                = PROTOCOL_GENERATOR_SYNC_081,
    // (WAVERIX) (090223) DEAD SYNC
    SYNC_CHARACTER_DEAD_BRD             = PROTOCOL_GENERATOR_SYNC_101,
    SYNC_CHARACTER_RESURRECTION_BRD     = PROTOCOL_GENERATOR_SYNC_102,
    SYNC_STATUS_ADD_BRD                 = PROTOCOL_GENERATOR_SYNC_103,
    SYNC_STATUS_REMOVE_BRD              = PROTOCOL_GENERATOR_SYNC_104,
    SYNC_REGENID_BRD                    = PROTOCOL_GENERATOR_SYNC_105,  //_NA_0_20100817_HELLON_LAST_TRIGGER
    //------------------------------------------------------------------------------------------
    // _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    SYNC_SERVER_TIMER_SYNCHRONIZE_BRD   = PROTOCOL_GENERATOR_SYNC_106,
    //_NA_007667_20141001_WITCHBLADE_SKILL
    SYNC_STATUS_RESET_BRD               = PROTOCOL_GENERATOR_SYNC_107,
    //------------------------------------------------------------------------------------------
    // NOTE: f110425.1L, collective object synchronization broadcast
    COLLECT_ACTION_BRD                  = PROTOCOL_GENERATOR_SYNC_190,
    COLLECT_CANCEL_BRD                  = PROTOCOL_GENERATOR_SYNC_191,
    //------------------------------------------------------------------------------------------
    // skill protocols based on region event (EP2) (201 ~
    BATTLE_MONSTER_ATTACK_BRD           = PROTOCOL_GENERATOR_SYNC_201,
    BATTLE_MONSTER_JUMP_ATTACK_BRD      = PROTOCOL_GENERATOR_SYNC_202,
    //
    BATTLE_PLAYER_ATTACK_BRD            = PROTOCOL_GENERATOR_SYNC_205,
    BATTLE_PLAYER_AREA_ATTACK_BRD       = PROTOCOL_GENERATOR_SYNC_206,
    //
    STYLE_PLAYER_ATTACK_BRD             = PROTOCOL_GENERATOR_SYNC_209,
    STYLE_PLAYER_ATTACK_RESULT_BRD      = PROTOCOL_GENERATOR_SYNC_210,
    //
    SKILL_ACTION_INSTANT_RESULT_BRD     = PROTOCOL_GENERATOR_SYNC_214,
    SKILL_ACTION_DELAY_START_BRD        = PROTOCOL_GENERATOR_SYNC_215,
    SKILL_ACTION_DELAY_RESULT_BRD       = PROTOCOL_GENERATOR_SYNC_216,
    //
    SKILL_CASTING_CANCEL_BRD            = PROTOCOL_GENERATOR_SYNC_218,

    SKILL_PERIODIC_DAMAGE_BRD           = PROTOCOL_GENERATOR_SYNC_220,
    SKILL_REFLECT_DAMAGE_BRD            = PROTOCOL_GENERATOR_SYNC_221,
    SKILL_EFFECT_BOMB_BRD               = PROTOCOL_GENERATOR_SYNC_222,
    SKILL_SELF_DESTRUCTION_BRD          = PROTOCOL_GENERATOR_SYNC_223,
    SKILL_TRANSPARENT_BRD               = PROTOCOL_GENERATOR_SYNC_224,
    SKILL_SUCTION_BRD                   = PROTOCOL_GENERATOR_SYNC_225,
    SKILL_VITAL_SUCTION_BRD             = PROTOCOL_GENERATOR_SYNC_226,
    //SKILL_EXECUTE_STATUS_CMD
        //------------------------------------------------------------------------------------------
};

}; //end of namespace 'nsSync'

namespace nsSync {
#pragma pack(push, 1)
;
//////////////////////////////////////////////////////////////////////////
//  eSYNC_OBJECT

//WAR_MSG_DECL(SYNC_MONSTER_ENTER_BRD)
//{
//};

//WAR_MSG_DECL(SYNC_MONSTER_DIR_ENTER_BRD)
//{
//};

//WAR_MSG_DECL(SYNC_PLAYER_LEAVE_BRD)
//{
//  enum { _MAX_SIZE = 50 };
//  BYTE                        m_byCount;
//  WORD                        m_PlayerKey[_MAX_SIZE];
//  inline WORD                 GetSize() const
//  {
//      return (WORD)(sizeof(*this) - (sizeof(WORD)*(_MAX_SIZE - m_byCount)));
//  }
//};

//WAR_MSG_DECL(SYNC_MONSTER_LEAVE_BRD)
//{
//  enum { _MAX_SIZE = 50 };
//  BYTE                        m_byCount;
//  DWORD                       m_dwObjectKey[_MAX_SIZE];
//  inline WORD                 GetSize() const
//  {
//      return (WORD)(sizeof(*this) - (sizeof(DWORD)*(_MAX_SIZE - m_byCount)));
//  }
//};

//WAR_MSG_DECL(SYNC_SUMMONED_ENTER_BRD)
//{
//};

//WAR_MSG_DECL(SYNC_MAPNPC_ENTER_BRD)
//{
//};

//WAR_MSG_DECL(SYNC_FIELDITEM_ENTER_BRD)
//{
//};

//WAR_MSG_DECL(SYNC_FIELDITEM_LEAVE_BRD)
//{
//  enum { _MAX_SIZE = 50 };
//  BYTE                        m_byCount;
//  DWORD                       m_dwObjectKey[_MAX_SIZE];
//  inline WORD                 GetSize() const
//  {
//      return (WORD)(sizeof(*this) - (sizeof(DWORD)*(_MAX_SIZE - m_byCount)));
//  }
//};

//WAR_MSG_DECL(SYNC_BOSS_MONSTER_ENTER_BRD)
//{
//};

//WAR_MSG_DECL(SYNC_NPC_JUMPRESULT_BRD)
//{
//};

//WAR_MSG_DECL(SYNC_LUCKY_MONSTER_RUNAWY_BRD)
//{
//};

//WAR_MSG_DECL(SYNC_TOTEMNPC_ENTER_BRD)
//{
//};


//////////////////////////////////////////////////////////////////////////
//  eSYNC_POS

//WAR_MSG_DECL(SYNC_MOVE_NAK)
//{
//  WzVector        m_wvCurPos;
//  BYTE            m_byErrorCode;              // eSYNC_RESULT
//};

WAR_MSG_DECL(SYNC_MOVE_BRD)
{
    DWORD           m_dwObjectKey;
    BYTE            m_byState;
    BYTE            m_ForcedMove;               // 피어에 의한 강제 이동과 같은 사례에 (1) 일반적으로 (0)
    WzVector        m_wvCurPos;
    WzVector        m_wvDestPos;
};

WAR_MSG_DECL(SYNC_MOVE_THRUST_BRD)
{
    DWORD           m_dwObjectKey;
    BYTE            m_byState;                  // 상태(걷기, 뛰기)
    WzVector        m_wvCurPos;
    WzVector        m_wvDestPos;
    WORD            m_wTileIndex;
};

WAR_MSG_DECL(SYNC_TARGET_MOVE_BRD)
{
    DWORD           m_dwTargetKey;
    WORD            m_wPlayerKey;
    WzVector        m_wvCurPos;
    WzVector        m_wvDestPos;
};

WAR_MSG_DECL(SYNC_KBMOVE_BRD)
{
    WORD            m_wPlayerKey;
    WzVector        m_wvCurPos;
    TILEINDEX       m_wTileIndex;
    WORD            m_wAngle;
    BYTE            m_byMoveState;
};

WAR_MSG_DECL(SYNC_NPC_JUMP_BRD)
{
    bool            m_bJumpAttack;
    DWORD           m_dwObjectKey;
    WzVector        m_wvEndPos;
};

WAR_MSG_DECL(SYNC_STOP_BRD)
{
    DWORD           m_dwObjectKey;
    WzVector        m_wvCurPos;
};

//WAR_MSG_DECL(SYNC_NPC_STOP_BRD)
//{
//  DWORD           dwObjectKey;
//  WzVector        wvCurPos;
//};

//WAR_MSG_DECL(SYNC_FORCED_WARP_BRD)
//{
//  DWORD           dwObjectKey;
//  WzVector        wvWarpPos;
//};

WAR_MSG_DECL(SYNC_FAST_KBMOVE_BRD)
{
    WORD            m_wPlayerKey;
    WzVector        m_wvCurPos;
    WORD            m_wAngle;
    TILEINDEX       m_wTileIndex;
    BYTE            m_byCmsMoveState;
};

//WAR_MSG_DECL(SYNC_JUMP_ACK)
//{
//  //
//};

//WAR_MSG_DECL(SYNC_JUMP_NAK)
//{
//  WzVector        m_wvCurPos;
//  BYTE            m_byErrorCode;      
//};

WAR_MSG_DECL(SYNC_JUMP_BRD)
{
    PLAYERKEY       m_PlayerKey;
    WzVector        m_wvDestPos;
    int             m_iDir;
};

//WAR_MSG_DECL(SYNC_JUMP_STOP_NAK)
//{
//  DWORD           m_byErrorCode;          //< eSYNC_RESULT
//};

// (WAVERIX) (090223) DEAD SYNC - __NA000000_090223_VARIABLE_SIZE_GRID_SYNC_CORRECTION__
WAR_MSG_DECL(SYNC_CHARACTER_DEAD_BRD)
{
    DWORD           m_ObjectKey;
};

WAR_MSG_DECL(SYNC_CHARACTER_RESURRECTION_BRD)
{
    DWORD       m_dwObjectKey;  // 누구
    DWORD       m_dwHP;         // 최종값
    DWORD       m_dwMP;         // 최종값
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    DWORD       m_dwSD;
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    WzVector    m_wvCurPos;     // 부활 위치
    DWORD       m_dwReserveHP;
};

//  __NA_001290_20090525_SHIELD_SYSTEM
WAR_MSG_DECL(SYNC_STATUS_ADD_BRD)
{
    DWORD       m_dwTargetKey;  // 상태 추가 대상
    WORD        m_wStatusCode;  // 상태 코드
};

WAR_MSG_DECL(SYNC_STATUS_REMOVE_BRD)
{
    DWORD       m_dwTargetKey;  // 상태 해제 대상
    WORD        m_wStatusCode;  
};

//_NA_0_20100817_HELLON_LAST_TRIGGER
WAR_MSG_DECL(SYNC_REGENID_BRD)  //  regen 정보를 가지는 몬스터의 정보 동기화
{
    DWORD       m_dwObjectKey;
    DWORD       m_dwRegenID;
    DWORD       m_dwAnimationCode;
};

// _NA000000_SPEED_HACK_PREVENTION_SUPPORT
WAR_MSG_DECL(SYNC_SERVER_TIMER_SYNCHRONIZE_BRD)
{
    static const DWORD kMilestoneInterval = 10 * 1000;
    DWORD milestone_counter;
};

//==================================================================================================
// NOTE: f110425.1L, collective object synchronization broadcast
WAR_MSG_DECL(COLLECT_ACTION_BRD)
{
    C_COLLECTKEY object_key; // 채집물품
    WzID  collection_id; // 채집물품ID
    DWORD collector;     // 채집자
};

WAR_MSG_DECL(COLLECT_CANCEL_BRD)
{
    C_COLLECTKEY object_key; // 채집물품
    WzID  collection_id; // 채집물품ID
    DWORD collector;     // 채집자
};
//==================================================================================================


//==================================================================================================
#pragma pack(pop)
}; //end of namespace 'nsSync'
//==================================================================================================
//==================================================================================================
//==================================================================================================


//==================================================================================================

#define PROGRAMCOMMON_WARMESSAGES_INTER_INL
#include "./WarMessages_Skill.inl"
#undef PROGRAMCOMMON_WARMESSAGES_INTER_INL

//==================================================================================================

using namespace nsSync;	// 구조만 나누고 일반 접근 가능하게 하도록 한다.

//==================================================================================================
