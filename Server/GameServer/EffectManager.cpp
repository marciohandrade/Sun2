#include "stdafx.h"
#include "BaseEffect.h"
#include "EffectManager.h"

#include "PeriodicDamageEffect.h"
#include "BombEffect.h"


EffectManager::~EffectManager()
{
	Release();
}


#define EFFECT_TYPE_LIST()	\
	EFFECT_TYPE_NODE( EFFECT_TYPE_PERIODIC_DAMAGE,	PeriodicDamageEffect		)	\
	EFFECT_TYPE_NODE( EFFECT_TYPE_BOMB,				BombEffect					)	\
	EFFECT_TYPE_NODE( EFFECT_TYPE_SELF_DESTRUCTION,	SelfDestructionDamageEffect	)	

//	<Sample>
#define EFFECT_TYPE_NODE( _typeid, _type )						\
	struct _type##_Info : public sun_type_info< _type >			\
	{ static const eFIELDEFFECT_TYPE		value = _typeid; };


inline VOID
EffectManager::ReleaseEffect( BaseEffect* const pEffect )
{
	const eFIELDEFFECT_TYPE effect_type = (eFIELDEFFECT_TYPE)pEffect->GetStateID();
	switch( effect_type )
	{
#undef EFFECT_TYPE_NODE
#define EFFECT_TYPE_NODE(_typeid, _type)  \
    case _typeid:   TAllocDelete(_type, pEffect); break;

	EFFECT_TYPE_LIST();

	default:
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] Invalid eEffectType[%d]! \n", effect_type );
		break;
	}
}


VOID
EffectManager::Release()
{
	FOREACH_CONTAINER( EFFECT_LIST::value_type& rEffect, m_Effects, EFFECT_LIST )
	{
		BaseEffect* const pEffect = rEffect;
		ReleaseEffect( pEffect );
	}
	m_Effects.clear();
}


BaseEffect*
EffectManager::AllocEffect( const eFIELDEFFECT_TYPE effect_type )
{
	// Effect를 새로 할당해서 추가한다.
	BaseEffect* pEffect = NULL;

	switch( effect_type )
	{
#undef EFFECT_TYPE_NODE
#define EFFECT_TYPE_NODE(_typeid, _type) \
    case _typeid: pEffect = TAllocNew(_type); break;

	EFFECT_TYPE_LIST();

	default:
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] Invalid eEffectType[%d]! \n", effect_type );
		break;
	}

	if( !pEffect )
		return NULL;	// 로그는 앞에서 해결될 것이다.

	//	먼저 등록이라... 비슷한 구조가 스킬에 있었다가 내부에서 검사후 등록하는 방식으로 바꿨었는데... 여기는... 음...
	pEffect->SetStateID( (WORD)effect_type );

	m_Effects.push_back( pEffect );

	return pEffect;
}


VOID
EffectManager::Update(const DWORD current_tick)
{
	const BOOL	NEED_DELETE = FALSE;
    // (CHANGES) fix up invalid iterator operation which has increase end position
    EFFECT_LIST::iterator it = m_Effects.begin(),
                          end = m_Effects.end();
    while (it != end)
    {
        BaseEffect* effect = *it;
        if (NEED_DELETE == effect->Update(current_tick))
        {
            ReleaseEffect(effect);
            it = m_Effects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}



