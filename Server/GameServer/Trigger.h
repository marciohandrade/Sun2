#ifndef _GAMESERVER_TRIGGER_H
#define _GAMESERVER_TRIGGER_H
#pragma once

//==================================================================================================
#include <BaseTrigger.h>
#include <LinkedListCS.h>

class Player;
class TriggerManagerEx;
class IConditionTriggerEx;
class IActionTriggerEx;

typedef std::pair<DWORD, Player*>                SATISFIED_PLAYER_PAIR;
typedef STLX_VECTOR<SATISFIED_PLAYER_PAIR>  SATISFIED_PLAYER_LIST;

//==================================================================================================
//
class Trigger : public BaseTrigger
{
public:
    Trigger();
    virtual ~Trigger();

public:
    void OnRelease();
    BOOL Update();
    BOOL ActionUpdate();

    //----------------------------------------------------------------------------------------------
    //  SatisfiedPlayer Operations
    BOOL AddSatisfiedPlayer(Player* pPlayer);
    VOID RemoveUnsatisfiedPlayer();
    void RemoveAllSatisfiedPlayer();
    Player* GetFirstSatisfiedPlayer();

    VOID CheckUnsatisfiedPlayer(Object::Key player_key);

    template<typename _OPERATOR>
    void ForeachSatisfiedPlayer(_OPERATOR& op)
    {
        for (SATISFIED_PLAYER_LIST::iterator it = m_SatisfiedPlayers.begin(); 
            it != m_SatisfiedPlayers.end();)
        {
            Player* const player = it->second;
            if (obj_util::IsValid(player))
            {
                op(player);
                ++it;
            }
            else
            {
                it = m_SatisfiedPlayers.erase(it);
            }
        }
    }

    template<typename _FN, typename _OBJ>
    void ForeachSatisfiedPlayer(_FN _func, const _OBJ& _left)
    {
        for (SATISFIED_PLAYER_LIST::iterator it = m_SatisfiedPlayers.begin(); 
            it != m_SatisfiedPlayers.end();)
        {
            Player* const player = it->second;
            if (obj_util::IsValid(player))
            {
                boost::bind(_func, _left, _1)(*it);
                ++it;
            }
            else
            {
                it = m_SatisfiedPlayers.erase(it);
            }
        }

        RemoveUnsatisfiedPlayer();
    }

private:
    virtual void _createCondition(ConditionInfo* pTrigger);
    virtual void _createAction(ActionInfo* pTrigger);
    virtual void _destroyCondition(IConditionTrigger* pTrigger);
    virtual void _destroyAction(IActionTrigger* pTrigger);
    virtual void _OnAction();

private:
    // 조건을 만족하는 플레이어 리스트
    SATISFIED_PLAYER_LIST   m_SatisfiedPlayers;
    STLX_VECTOR<DWORD>      unsatisfied_players;
    __DECLARE_POOL(Trigger);
};

#endif //_GAMESERVER_TRIGGER_H

