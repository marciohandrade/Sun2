#pragma once

#include "User.h"

class ActiveUser : public User
{
public:
	ActiveUser(void);
	~ActiveUser(void);

	virtual VOID			Init();
	virtual VOID			Release();
	virtual VOID			Update();
    virtual eUSER_TYPE GetUserType() const;
protected:
	virtual VOID			OnAccept( DWORD dwNetworkIndex );
	virtual VOID			OnRecv( BYTE *pMsg, WORD wSize );
};

inline eUSER_TYPE ActiveUser::GetUserType() const
{
    return ACTIVE_USER;
}
