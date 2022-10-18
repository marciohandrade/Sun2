// declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
/*

�ۼ���: ������
������: 2009-12-02

�����Ѽ���: �������� �ڵ鷯�� ȣ���ϱ� ���� Invoker

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

    //Ŀ�ǵ� �����Ѵ�.
    void ProcessCommand(LPVOID pServerSession, MSG_BASE * pMsg, WORD wSize);
    virtual void InsertConCreateCommads() = 0;
protected:
    
    //Ŀ�ǵ� ���, ���� �Ѵ�.
    void InsertConCreateCommad(DWORD key, FriendInvitationCommand* value);
    void RemoveConCreateCommads();   
    
private:    
    CONCRETE_COMMAND_MAP m_ConcreteCommand;
	
};
