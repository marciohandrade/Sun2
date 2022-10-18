/**

    스킬 아이디에 따라 스킬 생성  
    @author
        -박준태,지완 

    @since

    @remarks
        -2005.06.07 소스 정리(지완)
*/
#pragma once

#include "SkillDef.h"

namespace SKILL
{
	enum SKILLID
	{
		SKILLID_DEATHDIVIDE = 1,
		SKILLID_ASSAULT,
		SKILLID_TRIPLEPIERCE,

		SKILLID_TACKLE,
		SKILLID_BASH,
		SKILLID_SPINNINGBLOW,

		SKILLID_BLOODRAIN,
		SKILLID_HEAVYSHOT,
		SKILLID_SPINNINGSWORD,
	};
};

class Character;
class StateBase;

class SkillStateFactory
{
	typedef stdext::hash_map<DWORD, StateBase * >	STATE_BASE_HASH_MAP_TYPE;
	typedef	STATE_BASE_HASH_MAP_TYPE::iterator		STATE_BASE_HASH_MAP_ITR_TYPE;

public:
    enum ResultProcessingType
    {
        kProcessing_PacketRecvImmediate = 0, 
        kProcessing_Projectile, 
        kProcessing_Max
    };

public:
	SkillStateFactory();
	~SkillStateFactory();

	StateBase * CreateState(int iSkillID,DWORD dwObjectType);
	BOOL		CanUseSkill(Character *pChr,int iSkillID);
	BOOL		CreateNonstopSkillEffect(Character *pChr,Character *pTarget,int iSkillID);

	BOOL		InitState();
	void		DeleteState();

    SkillStateFactory::ResultProcessingType GetResultProcessingType(SLOTCODE skill_code);

private:
	STATE_BASE_HASH_MAP_TYPE	m_StateBaseMap;

};

extern SkillStateFactory g_SkillStateFactory;
