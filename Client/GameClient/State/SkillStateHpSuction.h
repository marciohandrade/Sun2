#pragma once


#include "State/SkillState.h"

class SkillStateHpSuction : public StateSkill
{
public:

#ifdef _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
    struct Projectile_Info
    {
        Character* target;
        DWORD projectile_key;
    };

    typedef STLX_VECTOR<Projectile_Info> Projectile_Info_List;

    void AddHpSuctionProjectile(Character* owner_, Character* target);

#endif //_NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL

	SkillStateHpSuction();
	~SkillStateHpSuction( void );

	STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
    STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);


private:

#ifdef _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
    Projectile_Info_List target_projectile_key_list;
#else
    DWORD                       m_TargetProjectileKey;
#endif //_NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
    
    DWORD                       m_LastTime;
	BYTE						m_btAniState; //0->1->2
	DWORD						m_dwTime;

#ifdef _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
    void UpdateEffectRotate(Character* pCharacter);
    void UpdateEffectRotate(Character* pCharacter, Character* pTarget, DWORD projectile_key);
#else
    void UpdateEffectRotate( Character* pCharacter, Character* pTarget );
#endif //_NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
};
