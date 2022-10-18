#ifndef _AbutoState_H_
#define _AbutoState_H_
//------------------------------------------------------------------------
//
//  Name:   AutobotState.h
//
//  Desc:   abstract base class to define an interface for a state
//          http://www.ai-junkie.com/architecture/state_driven/tut_state1.html
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

struct MSG_BASE;

template <class entity_type>
class AutobotState
{
public:
  virtual ~AutobotState(){}

  virtual char* Name(){ return ""; }

  //this will execute when the state is entered
  virtual void Enter(entity_type*) = 0;

  //this is the states normal update function
  virtual bool Execute(entity_type*) = 0;

  //this will execute when the state is exited.
  virtual void Exit(entity_type*) = 0;

  //! return : fase : 이후 프로세스에 패킷 넘기지 않음 패킷 삼킴 (application에서도 처리 안함)
  //!          true : 이후 프로세스에서도 계속 처리
  virtual bool NetworkProc(entity_type*, MSG_BASE*){ return true; }
};

#endif