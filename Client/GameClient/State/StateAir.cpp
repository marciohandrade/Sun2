//------------------------------------------------------------------------------
//  StateAir.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "stateair.h"
#include "Character.h"

//------------------------------------------------------------------------------
/**
*/
StateAir::StateAir(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateAir::~StateAir(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateAir::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case 0:
		break;
	}
	
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateAir::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	// ���� ���۸��
    switch( pChr->GetWeaponAnimationCode() )
    {
    case '0':	pChr->SetAnimation( "HS01", FALSE); break;
    case '1':	pChr->SetAnimation( "HS11", FALSE); break;
    case '2':	pChr->SetAnimation( "HS21", FALSE); break;
    case '3':	pChr->SetAnimation( "HS31", FALSE); break;
    }
	pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_BLOW;
	pChr->m_fHeight = 0.001f;

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
	�߷� ���ӵ�
	friend ���������� ��� ������ ���ٰ��� ���� �ۼ��� �Լ��� ��ü
*/
STATEPROCESS::TYPE StateAir::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;
	pChr->ProcessInterpolation(dwTick);
    float fRet = pChr->Character::ProcessAnimation(dwTick);

#ifdef _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL
	if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_BLOW )
	{
		pChr->UpdateAir( dwTick );

        if( pChr->m_AirData.m_NoDown == TRUE )
        {
            // ����Ŀ�� ���� ����

            if( pChr->GetHeight() == 0.0f )
            {
                // �ٴڿ� �����ϴ� Ÿ�ֿ̹� ����

                pChr->SetAngleX( 0.0f );
                pChr->SetHeight( 0.0f );

                // ����ȿ��
                WzVector vUp;
                vUp.x = 0.0f;
                vUp.y = 0.0f;
                vUp.z = 1.0f;
                pChr->Vibrate(vUp,0.05f);			

                // ����Ƣ��
                pChr->CreateEffectAtHead(StrToWzID("A012"),0.0f);

                // �׾����� ����~
                if (pChr->GetHP() <= 0)
                {
                    pChr->SetNextState(STATE::DEATH,g_CurTime);
                    return STATEPROCESS::SUSPEND;
                }

                pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_SLASH;
                fRet = 0.0f;
            }
        }
        else
        {
            // �����ο�
            if( pChr->GetHeight() == 0.0f )
		    {
			    pChr->SetAngleX( 0.0f );
			    pChr->SetHeight( 0.0f );

			    // ����ȿ��
			    WzVector vUp;
			    vUp.x = 0.0f;
			    vUp.y = 0.0f;
			    vUp.z = 1.0f;
			    pChr->Vibrate(vUp,0.05f);			
    			
			    // ����Ƣ��
			    pChr->CreateEffectAtHead(StrToWzID("A012"),0.0f);


			    // �׾����� ����~
			    if (pChr->GetHP() <= 0)
			    {
				    pChr->SetNextState(STATE::DEATH,g_CurTime);
				    return STATEPROCESS::SUSPEND;
			    }

                // �ٴڿ� �д�������� ����
                switch( pChr->GetWeaponAnimationCode() )
                {
                case '0':	pChr->SetAnimation( "HS04", FALSE); break;
                case '1':	pChr->SetAnimation( "HS14", FALSE); break;
                case '2':	pChr->SetAnimation( "HS24", FALSE); break;
                case '3':	pChr->SetAnimation( "HS34", FALSE); break;
                }
                pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_DOWN;
                fRet = 0.0f;
		    }
	    }
    }

    // ����Ŀ�� ������� �ִϸ��̼��� ���⿡�� ������
    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_SLASH && fRet == 100.0f )
    {
        return STATEPROCESS::END;
    }



    // �����ο��� ������ �Ͼ�� �ִϸ��̼� ó��
    // �������� F002, F003 �ִϸ��̼��� �̿��� ������ ���������� ������ �����߾��µ�
    // F00X �ø���� �ٿ������ �ִϸ��̼ǿ����� ���ǰ� �־ ������� �ٲ��� ����..
    // 
    // �׷��� ���� �ִϸ��̼��� �߰��Ѱ��� HS07, HS08��..
    // ������ �� �� �ִϸ��̼��� �÷��̾ ������ �ִٴ°��ε�... 
    // �׷� ������ �÷��̾���� HS07, HS08 �ִϸ��̼��� ����ϱ���ϰ�, 
    // �׿��� ���͵��� ������ �ִϸ��̼��� F002, F003�� �״�� ����ϰԵȰ���
    //
    // �÷��̾��� ���Ͱ� �����ϱ� ������ �ܼ��� ������Ʈ Ÿ�Ը����δ� �ĺ��� �� ����
    // Class ������ �̿��� �ĺ���. SSQ�� �ƴ� �÷��̾��� ���ʹ� MONSTER_OBJECT�� �Ǿ��ֱ⶧����...


    // �ٿ� �ִϸ��̼� ������ �Ͼ�� ����
    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_DOWN && fRet == 100.0f )
    {
        bool is_monster = pChr->GetClass() == eCHAR_TYPE_MAX;

        // �ٿ������ ���� ���
        if( is_monster )
        {
            switch( pChr->GetWeaponAnimationCode() )
            {
            case '0':	pChr->SetAnimation( "F002", TRUE); break;
            case '1':	pChr->SetAnimation( "F102", TRUE); break;
            case '2':	pChr->SetAnimation( "F202", TRUE); break;
            case '3':	pChr->SetAnimation( "F302", TRUE); break;
            }
        }
        else
        {
            switch( pChr->GetWeaponAnimationCode() )
            {
            case '0':	pChr->SetAnimation( "HS07", TRUE); break;
            case '1':	pChr->SetAnimation( "HS17", TRUE); break;
            case '2':	pChr->SetAnimation( "HS27", TRUE); break;
            case '3':	pChr->SetAnimation( "HS37", TRUE); break;
            }
        }

        pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_DOWN_STUN;
    }

#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
    // �Ͼ�� ���� �� ���� ����
    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_DOWN_STUN && fRet == 100.0f )
    {
        return STATEPROCESS::END;
    }
#else
    // �ٿ� �ִϸ��̼� �Ͼ�� ����
	if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_DOWN_STUN && CurTime >= pChr->m_AirData.m_AirTime)
	{
        bool is_monster = pChr->GetClass() == eCHAR_TYPE_MAX;

        // �ٿ���¿��� �Ͼ�� ���
        if( is_monster )
        {
            switch( pChr->GetWeaponAnimationCode() )
            {
            case '0':	pChr->SetAnimation( "F003", FALSE); break;
            case '1':	pChr->SetAnimation( "F103", FALSE); break;
            case '2':	pChr->SetAnimation( "F203", FALSE); break;
            case '3':	pChr->SetAnimation( "F303", FALSE); break;
            }
        }
        else
        {
		    switch( pChr->GetWeaponAnimationCode() )
		    {
		    case '0':	pChr->SetAnimation( "HS08", FALSE); break;
		    case '1':	pChr->SetAnimation( "HS18", FALSE); break;
		    case '2':	pChr->SetAnimation( "HS28", FALSE); break;
		    case '3':	pChr->SetAnimation( "HS38", FALSE); break;
		    }
        }

		pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_GETUP;
        fRet = 0.0f;
	}

    // �Ͼ�� ���� �� ���� ����
	if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_GETUP && fRet == 100.0f )
	{
		return STATEPROCESS::END;
	}
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN

#else // _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL

    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_BLOW )
    {
        pChr->UpdateAir( dwTick );
        if( pChr->GetHeight() == 0.0f )
        {
            pChr->SetAngleX( 0.0f );
            pChr->SetHeight( 0.0f );

            // ����ȿ��
            WzVector vUp;
            vUp.x = 0.0f;
            vUp.y = 0.0f;
            vUp.z = 1.0f;
            pChr->Vibrate(vUp,0.05f);			

            // ����Ƣ��
            pChr->CreateEffectAtHead(StrToWzID("A012"),0.0f);


            // �׾����� ����~
            if (pChr->GetHP() <= 0)
            {
                pChr->SetNextState(STATE::DEATH,g_CurTime);
                return STATEPROCESS::SUSPEND;
            }

            // �ٿ� �������� ����
            switch( pChr->GetWeaponAnimationCode() )
            {
            case '0':	pChr->SetAnimation( "F002", TRUE); break;
            case '1':	pChr->SetAnimation( "F102", TRUE); break;
            case '2':	pChr->SetAnimation( "F202", TRUE); break;
            case '3':	pChr->SetAnimation( "F302", TRUE); break;
            }


            if( pChr->m_AirData.m_NoDown == TRUE )
            {
                // �ٿ���� �ʰ� �ٷ� ��������
                return STATEPROCESS::END;
            }
            else
            {
                pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_DOWN;
            }
        }
    }

    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_DOWN && CurTime >= pChr->m_AirData.m_AirTime)
    {
        switch( pChr->GetWeaponAnimationCode() )
        {
        case '0':	pChr->SetAnimation( "F003", FALSE); break;
        case '1':	pChr->SetAnimation( "F103", FALSE); break;
        case '2':	pChr->SetAnimation( "F203", FALSE); break;
        case '3':	pChr->SetAnimation( "F303", FALSE); break;
        }
        pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_GETUP;
    }

    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_GETUP && fRet == 100.0f )
    {
        return STATEPROCESS::END;
    }

#endif // _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL


	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateAir::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character* pChr = (Character*)pOwner;
	pChr->SetAngleX( 0.0f );
	pChr->SetHeight( 0.0f );

	return STATECHANGE::SUCCESS;
}
