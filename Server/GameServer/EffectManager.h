#pragma once
#ifndef	GAMESERVER_EFFECT_MANAGER_H
#define GAMESERVER_EFFECT_MANAGER_H

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
class BaseEffect;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
class EffectManager
{
private:	typedef STLX_LIST< BaseEffect* >	EFFECT_LIST;

public:		EffectManager( GameField* const pField )
				: m_pField( pField )
			{}

			~EffectManager();

public:		VOID					Release();

			BaseEffect*				AllocEffect( const eFIELDEFFECT_TYPE effect_type );
			VOID					Update( const DWORD current_tick );

private:
	inline	VOID					ReleaseEffect( BaseEffect* const pEffect );

private:	GameField* const		m_pField;
			EFFECT_LIST				m_Effects;
    //
    __DISABLE_COPY(EffectManager);
};

#endif //GAMESERVER_EFFECT_MANAGER_H
