#pragma once
//=============================================================================================================================
/// HuntingField 클래스
/**
@author	Cho Jae Sik
@since	2006. 4. 4
@remark
- 헌팅필드에 대한 존 정보 관리
@note
- 
@history 
- 
@usage
- 
*/
//=============================================================================================================================

#include "ZoneInterface.h"

class HuntingField : public ZoneInterface
{
    _DECLARE_POOL(HuntingField)
public:
    HuntingField(void);
    virtual ~HuntingField(void);

    VOID    Create(KEYTYPE key, MAPCODE MapCode, ServerSession* pServerSession);
    inline virtual eZONETYPE GetType() { return eZONETYPE_FIELD; }
    //virtual MAPCODE GetMapCode() 	{ return m_MapCode; }

protected:
    //inline VOID SetMapCode(MAPCODE code)	{ m_MapCode = code;	}
    virtual VOID On_create();
    virtual VOID On_destroy();
    virtual VOID On_readyuser(User* pUser);
    virtual VOID On_enteruser(User* pUser);
    virtual VOID On_leaveuser(User* pUser);
    virtual RC::eROOM_RESULT _CanJoinEach(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD);

private:
    //MAPCODE	m_MapCode;
};
