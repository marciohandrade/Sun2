//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateDoubleSpinkick.h"
#include "Character.h"
#include "GlobalFunc.h"
//------------------------------------------------------------------------------
/**
*/
SkillStateDoubleSpinkick::SkillStateDoubleSpinkick()
{

}

//------------------------------------------------------------------------------
/**
*/
SkillStateDoubleSpinkick::~SkillStateDoubleSpinkick()
{
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateDoubleSpinkick::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;


	if (g_pEffectManager)
	{

		m_hTrailEffect = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("E004"),pChr->GetBonePos(87),pChr->GetObjectKey());


		// ������ �������� �ϴ� ������� �������
		// !! ��������� �������̽��� �׸��� ����. !!
		CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_hTrailEffect);
		if( pBody)
		{

			pBody->SetRenderType(AR_REFRACTION_GLASS);
			pBody->SetRefractionRate(0.5f);		// ������.(0�� ����� ������ ����)
			pBody->SetRefractionAlpha(1.0f);	// 1.0 �� ����������� �� �̹����� �������

			WzVector pos=pChr->GetBonePos(90);
			WzVector dir;
			WzMatrix mat=pChr->GetBoneMatrix(90);
			dir.x=mat._11*1.3f;
			dir.y=mat._12*1.3f;
			dir.z=mat._13*1.3f;//������ ������.

			pos+=dir;

			CEElementFlatChain* pFChain = pBody->GetFlatChainPtr();
            assert( pFChain );

            if( pFChain )
            {
			    pFChain->SetLifeInfo(0, 30000);
			    pFChain->SetTailNum(40);							
			    pFChain->SetSplineLength(0.2f);

			    pFChain->SetPosition(pChr->GetBonePos(90),pos);				
            }

		}
		else
		{
			//assert(!"�˱� ����Ʈ�� �������� ���߽��ϴ�");
		}


	}

	return StateSkill::OnStart(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateDoubleSpinkick::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

    EnterProcess(pOwner,CurTime,dwTick);

	// Ÿ���� üũ
	CheckEvent();

	// 2��Ÿ
	ProcessHit(pChr,1);
	ProcessHit(pChr,2);

	
	if(m_hTrailEffect!=INVALID_HANDLE_VALUE)
	{
		CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_hTrailEffect);
		if(pBody)
		{
			WzVector pos=pChr->GetBonePos(90);
			WzMatrix mat=pChr->GetBoneMatrix(90);
			WzVector dir;
			dir.x=mat._11*1.3f;
			dir.y=mat._12*1.3f;
			dir.z=mat._13*1.3f;

			pos+=dir;

			CEElementFlatChain* pFChain = pBody->GetFlatChainPtr();
            if( pFChain )
            {
			    pFChain->SetPosition(pChr->GetBonePos(90),pos);
            }

		}
	}

	if( GetAnimationRate() > 70)
	{
		if (g_pEffectManager&&m_hTrailEffect!=INVALID_HANDLE_VALUE)
		{
			g_pEffectManager->RemoveEffectPtr(m_hTrailEffect);
			m_hTrailEffect=INVALID_HANDLE_VALUE;
		}

	}

  return LeaveProcess(pOwner,CurTime,dwTick);
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateDoubleSpinkick::OnEnd(Object* pOwner, DWORD CurTime)
{
    //Todo:Ưȭ�� ó���� �Ѵ�.
	if (g_pEffectManager&&m_hTrailEffect!=INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_hTrailEffect);
	}

	return StateSkill::OnEnd(pOwner,CurTime);

}