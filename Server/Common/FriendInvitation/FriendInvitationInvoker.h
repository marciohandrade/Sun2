// declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
/*

작성자: 문상현
제작일: 2009-12-02

간단한설명: 프로토콜 핸들러를 호출하기 위한 Invoker

*/

#pragma once

#include "FriendInvitationCommand.h"

typedef STLX_MAP<DWORD, FriendInvitationCommand*>       CONCRETE_COMMAND_MAP;
typedef CONCRETE_COMMAND_MAP::iterator	    		    CONCRETE_COMMAND_MAP_ITER;
typedef std::pair< DWORD, FriendInvitationCommand* >    CONCRETE_COMMAND_MAP_PAIR;
typedef std::pair<CONCRETE_COMMAND_MAP::iterator, bool> CONCRETE_COMMAND_MAP_RESULT;


class FriendInvitationInvoker
{
public:
	FriendInvitationInvoker(void);
	~FriendInvitationInvoker(void);

    //커맨드 수행한다.
    void ProcessCommand(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    virtual void InsertConCreateCommads() = 0;
protected:
    
    //커맨드 등록, 삭제 한다.
    void InsertConCreateCommad(DWORD key, FriendInvitationCommand* value);
    void RemoveConCreateCommads();   
    
private:    
    CONCRETE_COMMAND_MAP m_ConcreteCommand;
	
};
