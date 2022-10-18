#pragma once
//=============================================================================================================================
/// Village 클래스
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2005. 12. 6
	@remark
		- 마을에 대한 존 정보 관리
	@note
		- 
	@history 
		- 
	@usage
		- 
*/
//=============================================================================================================================

#include "ZoneInterface.h"

class Village : public ZoneInterface
{
    _DECLARE_POOL(Village)
public:
    Village();
    virtual ~Village();

    VOID    Create( KEYTYPE key, MAPCODE MapCode, ServerSession * pServerSession );
    inline virtual eZONETYPE GetType() { return eZONETYPE_VILLAGE; }
    //virtual MAPCODE GetMapCode() { return m_MapCode; }

protected:
    //inline VOID SetMapCode( MAPCODE code ) { m_MapCode = code;	}
    virtual VOID On_create();
    virtual VOID On_destroy();
    virtual VOID On_readyuser( User * pUser );
    virtual VOID On_enteruser( User * pUser );
    virtual VOID On_leaveuser( User * pUser );
private:
    //MAPCODE		m_MapCode;
};
