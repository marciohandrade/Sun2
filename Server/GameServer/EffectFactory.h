#pragma once
#ifndef GAMESERVER_EFFECT_FACTORY_H
#define GAMESERVER_EFFECT_FACTORY_H

/*
#include <Singleton.h>
#include <MemoryPoolFactory.h>

class BaseEffect;
class PeriodicDamageEffect;
class BombEffect;


class EffectFactory : public Singleton<EffectFactory>
{
public:
	EffectFactory();
	virtual ~EffectFactory();

	VOID			Init( DWORD dwMaxPoolSize );
	VOID			Release();
	BaseEffect*		AllocEffect( eFIELDEFFECT_TYPE eEffectType );
	VOID			FreeEffect( BaseEffect* pEffect );

private:
	CMemoryPoolFactory<PeriodicDamageEffect> *	m_pPeriodicDamagePool;
	CMemoryPoolFactory<BombEffect> *			m_pBombPool;
};
*/
#endif //GAMESERVER_EFFECT_FACTORY_H