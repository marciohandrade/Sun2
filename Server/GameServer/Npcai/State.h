#ifndef _FINITESTATE_H_
#define _FINITESTATE_H_

#include "AITypes.h"
#include "./Npcai/NpcHeartbeat.h"

#define _WAVERIX_DETECTION_PROBLEM_     0

struct AI_TYPE_INFO;
class NPC;

//==================================================================================================
// (CHANGES) 'ENUM_STATD_ID' moved to 'AITypes.h'
enum ENUM_STATD_ID;

// (CHANGES) (091222) (WAVERIX) 'SetNPC' moved to code-file section which is appended 'AI type info'
//==================================================================================================
// AI ���� Ŭ����
//  - Ư�� ���ǿ� ���� ���� ���̵��� �߰��ϰ� FSM�� ����Ѵ�.
//  - 3���� �����Լ� OnEnter(), OnLeave(), OnUpdate()�� �������Ѵ�.
//==================================================================================================
class State
{
public:
    State();
    virtual ~State();

    virtual void Init() = 0;
    virtual void OnEnter(LPARAM param1 = 0, LPARAM param2 = 0, LPARAM param3 = 0);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick) = 0;
    virtual void OnAIMsg(const AI_MSG* pMsg);
    virtual void SetNPC(NPC* pNPC);

    void SetStateID(ENUM_STATD_ID stateID);
    ENUM_STATD_ID GetStateID() const;

    void SetLinkStateID(ENUM_STATD_ID stateID);
    ENUM_STATD_ID GetLinkStateID() const;
    NPC* GetOwnerNpc() const;

    // (CHANGES) (091223) (WAVERIX) get incremental random number by all npc random call
    static DWORD GetStateRandomNo();

    // State�� ��ü Ŭ�������� �ʿ��ϸ� �޼��� ó�� �Լ����� �������̵� �ؼ� ����ϵ���...
protected:
    virtual void OnMsgForceAttack(const AI_MSG* pMsg);
    virtual void OnMsgAttacked(const AI_MSG* pMsg);
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg);
    virtual void OnMsgLeaveField(const AI_MSG* pMsg);
    virtual void OnMsgThrust(const AI_MSG* pMsg);
    virtual void OnMsgFlying(const AI_MSG* pMsg);
    virtual void OnMsgKnockDown(const AI_MSG* pMsg);
    virtual void OnMsgStun(const AI_MSG* pMsg);
    virtual void OnMsgLetsGo(const AI_MSG* pMsg);
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg);
    virtual void OnMsgRunAway(const AI_MSG* pMsg);
    virtual void OnMsgChaos(const AI_MSG* pMsg);
    virtual void OnMsgChangeToReturnState(const AI_MSG* pMsg);
    // AttackState �ܿ��� �ƹ� ������ ���� �ʴ´�.
    virtual void OnMsgChangeToRetreatState(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgCommandFollow(const AI_MSG* pMsg);
    virtual void OnMsgUseSkill(const AI_MSG* pMsg);
    virtual void OnMsgNONPVP(const AI_MSG* pMsg);
    virtual void OnMsgBlind(const AI_MSG* pMsg);
    // Group Command ����(���� ������ ����)
    virtual void OnMsgGroupCommand(const AI_MSG* pMsg);
    virtual void OnMsgGroupCommand_Attack(DWORD dwTargetKey);
    virtual void OnMsgGroupCommand_StopAttack();

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    BOOL SetTarget(const AI_MSG* pMsg, ENUM_STATD_ID next_state = STATE_ID_UNKNOWN);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    //----------------------------------------------------------------------------------------------
    static DWORD ms_increment_random;
    NPC* m_pNPC;
    const AI_TYPE_INFO* m_pAITypeInfo;
    BYTE m_StateID;
    BYTE m_LinkStateID;
};

//==================================================================================================

inline
State::State()
    : m_pNPC(NULL)
    , m_pAITypeInfo(NULL)
    , m_StateID(STATE_ID_UNKNOWN)
    , m_LinkStateID(STATE_ID_UNKNOWN)
{
}

inline
State::~State() {
}

inline void
State::SetStateID(ENUM_STATD_ID stateID) {
    m_StateID = (BYTE)stateID;
}

inline ENUM_STATD_ID
State::GetStateID() const {
    return (ENUM_STATD_ID)m_StateID;
}

inline void
State::SetLinkStateID(ENUM_STATD_ID stateID) {
    m_LinkStateID = (BYTE)stateID;
}

inline NPC*
State::GetOwnerNpc() const {
    return m_pNPC;
}

inline ENUM_STATD_ID
State::GetLinkStateID() const {
    return (ENUM_STATD_ID)m_LinkStateID;
}

inline DWORD
State::GetStateRandomNo() {
    return ms_increment_random = ms_increment_random + (13 + 100);
}

inline void
State::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    __UNUSED(param1);
    __UNUSED(param2);
    __UNUSED(param3);
}

inline void
State::OnExit()
{
}

#endif //_FINITESTATE_H_


