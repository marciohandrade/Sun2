#ifndef _AutobotStateMachine_H_
#define _AutobotStateMachine_H_

#if _MSC_VER > 1000
#pragma once
#endif

//------------------------------------------------------------------------
//
//  Name:   AutobotStateMachine.h
//
//  Desc:   AutobotState machine class. Inherit from this class and create some 
//          states to give your agents FSM functionality
//          http://www.ai-junkie.com/architecture/state_driven/tut_state1.html
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <cassert>
#include <string>
#include <map>

#include "AutobotState.h"

//------------------------------------------------------------------------------
template <class entity_type>
class AutobotStateMachine
{
private:
    typedef std::map<std::string, AutobotState<entity_type>*>    AutobotStateMap;
    typedef std::vector<AutobotState<entity_type>*>              AutobotStateVector;

    //a pointer to the agent that owns this instance
    entity_type* m_pOwner;

    AutobotState<entity_type>* m_pCurrentState;

    //a record of the last state the agent was in
    AutobotState<entity_type>* m_pPreviousState;

    AutobotStateMap m_mapState;
    AutobotStateVector m_vecGolbalState;

public:
    AutobotStateMachine(entity_type* owner) : m_pOwner(owner),
                                        m_pCurrentState(NULL),
                                        m_pPreviousState(NULL)
    {
        m_mapState.clear();
        m_vecGolbalState.clear();
    }

    virtual ~AutobotStateMachine() { m_mapState.clear(); m_vecGolbalState.clear(); }

    //use these methods to initialize the FSM
    AutobotState<entity_type>*  CurrentState()  const{return m_pCurrentState;}
    AutobotState<entity_type>*  PreviousState() const{return m_pPreviousState;}

    //------------------------------------------------------------------------------
    bool PushGlobalState(AutobotState<entity_type>* state)
    {
        AutobotStateVector::iterator begin = m_vecGolbalState.begin();
        AutobotStateVector::iterator end = m_vecGolbalState.end();

        for (; begin != end; ++begin)
        {
            if (*begin == state)
            {
                return false;
            }
        }

        state->Enter(m_pOwner);
        m_vecGolbalState.push_back(state);
        return true;
    }
    //------------------------------------------------------------------------------
    bool PopGlobalState(AutobotState<entity_type>* state)
    {
        AutobotStateVector::iterator begin = m_vecGolbalState.begin();
        AutobotStateVector::iterator end = m_vecGolbalState.end();

        for (; begin != end; ++begin)
        {
            if (*begin == state)
            {
                (*begin)->Exit(m_pOwner);
                m_vecGolbalState.erase(begin);
                return true;
            }
        }
        return false;
    }
    //------------------------------------------------------------------------------
    void RegisterState(std::string state_name, AutobotState<entity_type>* state)
    {
        AutobotStateMap::iterator itor = m_mapState.find(state_name);
        if (itor != m_mapState.end())
        {
            ASSERT(state && "<AutobotStateMachine::RegisterState>: same state");
            return;
        }

        m_mapState.insert(std::make_pair(state_name, state));
    }

    //------------------------------------------------------------------------------
    //call this to update the FSM
    bool Update()
    {
        bool result = false;

        AutobotStateVector::iterator begin = m_vecGolbalState.begin();
        AutobotStateVector::iterator end = m_vecGolbalState.end();

        for (; begin != end; ++begin)
        {
            result = (*begin)->Execute(m_pOwner);
        }

        //same for the current state
        if (m_pCurrentState) 
        {
            result = m_pCurrentState->Execute(m_pOwner);
        }

        return result;
    }
    //------------------------------------------------------------------------------
    bool NetworkProc(MSG_BASE* packet)
    {
        AutobotStateVector::iterator begin = m_vecGolbalState.begin();
        AutobotStateVector::iterator end = m_vecGolbalState.end();

        bool result = true;
        for (; begin != end; ++begin, packet)
        {
            if (result)
            {
                result = (*begin)->NetworkProc(m_pOwner, packet);
            }
        }

        //same for the current state
        if (m_pCurrentState) 
        {
            if (result)
            {
                result = m_pCurrentState->NetworkProc(m_pOwner, packet);
            }
        }

        return result;
    }

    //------------------------------------------------------------------------------
    //change to a new state
    void ChangeState(std::string state_name)
    {
        AutobotStateMap::iterator itor = m_mapState.find(state_name);
        if (itor != m_mapState.end())
        {
            OutputDebugStr("[AutobotState Change] : ");
            OutputDebugStr(state_name.c_str());
            OutputDebugStr("\n");

            ChangeState(itor->second);
        }
    }
    //------------------------------------------------------------------------------
    void ChangeState(AutobotState<entity_type>* pNewState)
    {
        //keep a record of the previous state
        m_pPreviousState = m_pCurrentState;

        //call the exit method of the existing state
        if (m_pCurrentState)
        {
            m_pCurrentState->Exit(m_pOwner);
        }

        //change state to the new state
        m_pCurrentState = pNewState;

        //call the entry method of the new state
        if (m_pCurrentState)
        {
            m_pCurrentState->Enter(m_pOwner);
        }
    }

    //------------------------------------------------------------------------------
    //change state back to the previous state
    void  RevertToPreviousState()
    {
        ChangeState(m_pPreviousState);
    }
};

#endif


