#ifndef _MERCHANT_H_
#define _MERCHANT_H_

#include "NPC.h"

class Merchant : public NPC
{
public:
	Merchant();
	virtual ~Merchant();

public:
	virtual BOOL			Init();
	virtual VOID			Release();
	virtual BOOL			Update( DWORD dwDeltaTick );
	virtual VOID			Reuse()		{ Release(); }

protected:
    virtual void _Damaged(DamageArgs* damage_arg);

public:
	virtual VOID			OnDead();
};

#endif // _MERCHANT_H_
