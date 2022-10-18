#pragma once

#include <ITimeout.h>
class State;

//==================================================================================================
//  @history
//  ~09/12/22
//  09/12/22, waverix, code-arrangement
namespace nsAI {

//--------------------------------------------------------------------------------------------------
struct CmdAI
{
    enum eCmd {
        eCmd_None       = 0,
        eCmd_Escape     = 1<<20,
    };

    enum eArgOpt {
        //eArg_PathCmd_ListSize = 5,
    };

    const eCmd Cmd;

    CmdAI() : Cmd(eCmd_None) {}
    CmdAI(const eCmd _cmd) : Cmd(_cmd) {}

    struct DefaultArgs;
    struct Escape;
    __DISABLE_COPY(CmdAI);
};

//--------------------------------------------------------------------------------------------------
struct CmdAI::Escape : public CmdAI
{
    const DWORD TimeoutInterval;
    const WORD DestTileNo;
    const WzVector* pDestPos;

    Escape(DWORD timeoutInterval, WORD destTileNo, const WzVector* const _pDestPos = NULL)
        : CmdAI(CmdAI::eCmd_Escape)
        , TimeoutInterval(timeoutInterval)
        , DestTileNo(destTileNo)
        , pDestPos(_pDestPos)
    {}
    __DISABLE_COPY(Escape);
};

//--------------------------------------------------------------------------------------------------
class EscapeState;
}; //end of namespace 'nsAI'


//==================================================================================================
//  <NPCStateManager>
class NPCStateManager
{
public:
    NPCStateManager(NPC* pOwner);
    ~NPCStateManager();

    void Init(DWORD stateID, BYTE byMoveType, DWORD moveAreaID, LPARAM param1 = 0);
    void Release();
    BOOL Update(DWORD delta_tick);
    void AddStateObject(DWORD stateID, DWORD linkStateID);
    void ChangeState(DWORD stateID, LPARAM param1 = 0, LPARAM param2 = 0, LPARAM param3 = 0);

    State* GetCurState() const;

    void SetRequestHelp(BOOL bHelp);
    BOOLEAN IsRequestHelp() const;
    eNPC_MOVE_ATTITUDE  GetMoveType() const;
    DWORD GetMoveAreaID() const;
#ifdef _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
	void SetMoveAreaID(DWORD move_area_id);
#endif // _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER

    //_NA_0_20100222_UNIT_TRIGGERS_PATHLIST = {
    void SetMovePosIndex(WORD pos_index) { pos_index_ = pos_index; }
    BOOL IncreaseMovePosIndex();
    BOOL DecreaseMovePosIndex();
    const eCHAR_MOVE_STATE GetWalkState() const;
    bool IsArrivedLastPosition() const; // 위치 목록 중 마지막 위치에 도달 했는지 검사한다.
    //}
    const WzVector& GetStartPos() const;
    const WzVector& GetDestPos() const;

    void RequestCommand_(const nsAI::CmdAI& rCmd);
    void ResetStateObject();    //_NA_0_20100514_NPC_DEAD_STATE_TIME

private:
    struct StateTransaction
    {
        StateTransaction(BYTE& bTrans) : m_bTrans(bTrans) { m_bTrans = true; }
        ~StateTransaction() { m_bTrans = false; }
        BYTE&   m_bTrans;
        __DISABLE_COPY(StateTransaction);
    };

    void ClearFields();
private:
    typedef STLX_MAP<DWORD, State*> STATE_MAP;

    NPC* const m_pNPC; // 상태 주체
    State*    m_pCurState; // 현재 상태 (Delegator)
    STATE_MAP m_mapStates; // 상태 리스트

    eNPC_MOVE_ATTITUDE  m_eMoveType; // 이동 성향
    DWORD m_dwMoveAreaID; // 이동 영역

    WORD pos_index_;

    ITimerBase m_SpecialTimer; // 특수조건, 상태저항

    // for DEBUG
    ITimeout m_StateChangeCheckTimer;
    WORD m_StateChangeCount; // <주의!> 이 부분부터 순서 바꾸면 용량 늘어남 
    BYTE m_bStateTransaction;

    BOOLEAN m_bRequestHelp; // Help 요청을 한 상태인가?

    nsAI::EscapeState* m_pEscapeState;
    __DISABLE_COPY(NPCStateManager);
};


//==================================================================================================

inline State*
NPCStateManager::GetCurState() const {
    return m_pCurState;
}

inline void
NPCStateManager::SetRequestHelp(BOOL bHelp) {
    m_bRequestHelp = !!bHelp;
}

inline BOOLEAN
NPCStateManager::IsRequestHelp() const {
    return m_bRequestHelp;
}

inline eNPC_MOVE_ATTITUDE
NPCStateManager::GetMoveType() const {
    return m_eMoveType;
}

inline DWORD
NPCStateManager::GetMoveAreaID() const {
    return m_dwMoveAreaID;
}
#ifdef _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
inline void 
NPCStateManager::SetMoveAreaID(DWORD move_area_id) {
    m_dwMoveAreaID = move_area_id;
}
#endif // _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER

inline void
NPCStateManager::Release() {
    ClearFields();
}

