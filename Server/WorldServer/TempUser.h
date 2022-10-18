#pragma once

#include "User.h"

class TempUser : public User
{
public:
	TempUser(void);
	~TempUser(void);

	virtual VOID		Init();
    virtual eUSER_TYPE GetUserType() const;
protected:
	virtual VOID		OnAccept( DWORD dwnetworkIndex );
	virtual VOID		OnRecv( BYTE *pMsg, WORD wSize );
	BOOL				SendPacket( BYTE* pMsg, WORD wSize );

private:
	BOOL				m_bFirstTime;
	DWORD				m_dwAuthID;
};

inline eUSER_TYPE TempUser::GetUserType() const
{
    return TEMP_USER;
}
