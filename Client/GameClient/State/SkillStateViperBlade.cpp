//------------------------------------------------------------------------------
//  SkillStateViperBlade.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateViperBlade.h"
#include "Character.h"
#include "Hero.h"
#include "GlobalFunc.h"

//------------------------------------------------------------------------------
/**
	�θ� Onstart ȣ����Ѵ�.
*/
STATECHANGE::TYPE SkillStateViperBlade::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character*)pOwner;

	// ����Ʈ ����
	HANDLE hHandle = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("H520"),pCharacter->GetVisiblePos(),pCharacter->GetScale(),pCharacter->GetObjectKey());//!loop ����Ʈ ���� ����    
	if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(hHandle) )
	{
		WzVector vRot;

		vRot.y = 0.0f;
		vRot.z=pCharacter->GetAngle(); 
		vRot.x=pCharacter->GetAngleX();

		WzVector vScale;

		vScale.x = 1.0f;
		vScale.y = 1.0f;
		vScale.z = 1.0f;

		pBody->SetResultMatrix(pCharacter->GetVisiblePos(),vRot,vScale);
	}

	return StateSkill::OnStart( pOwner, CurTime );
}