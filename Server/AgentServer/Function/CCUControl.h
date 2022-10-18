#pragma once

#include <Packetstruct_AM.h>
#include <Usersessions/UserManager.h>

//////////////////////////////////////////////////////////////////////////
//
// 

//////////////////////////////////////////////////////////////////////////
//
class CCUControl
{
public:
    static const DWORD MASK_VILLAGE_GROUP = (1 << eZONETYPE_CHARSELECT)
                                          | (1 << eZONETYPE_VILLAGE)
                                          | (1 << eZONETYPE_LOBBY)
                                          | (1 << eZONETYPE_FIELD)
                                          | (1 << eZONETYPE_DOMINATION_FIELD)
                                          | (1 << eZONETYPE_SPA_FIELD)
                                          | (1 << eZONETYPE_MAX);	// ��~���ִ� ����
public:
    CCUControl();
    ~CCUControl() {}

public:
    VOID Calculation(User* const user);
    // Enter, Leave���� ����� �����
    VOID GetCurrentCCU(MSG_AM_CONNECTION_CCU_REPORT_NTF* OUT pInfo);

private:
    MSG_AM_CONNECTION_CCU_REPORT_NTF m_CCU_INFO;
};
