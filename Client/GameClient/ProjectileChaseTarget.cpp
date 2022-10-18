#include "SUNClientPrecompiledHeader.h"
#include "ProjectileChaseTarget.h"
#include "ObjectManager.h"
#include "GlobalFunc.h"
#include "GameFunc.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cProjectileChaseTarget::cProjectileChaseTarget()
{
}


void cProjectileChaseTarget::Destroy()
{
    cProjectileTarget::Destroy();
    cProjectileObject::DestroyProjectileObject();
}

void cProjectileChaseTarget::Render()
{
    cProjectileObject::RenderProjectileObject();
}

BOOL cProjectileChaseTarget::Process( DWORD TickTime )
{
    cProjectileTarget::Process();

    if( cProjectileObject::ProcessProjectileObject( TickTime ) == TRUE )
        return TRUE;

    return FALSE;
}

#include "Hero.h"
#include "HeroData.h"
#include "LoadScene.h"
#include "GameFramework.h"

BOOL cProjectilePortal::Process(DWORD TickTime)
{
    if( cProjectileChaseTarget::Process( TickTime ) == TRUE )
        return TRUE;

    if (GENERALPARAM->IsNet() == FALSE)
    {
        WzVector HeroPosition = g_pHero->GetVisiblePos();
        HeroPosition.z += 1.0f;

        WzVector vDistance = HeroPosition - GetPosition();
        float fDistance = VectorLength(&vDistance);

        if (fDistance <= 1.0f)
        {
            //��Ŷ ���� �� 
            xGetHeroData()->SetObjectKey( INVALID_OBJECT_KEY );
            g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_SSQ, 12001, 61001);
            GameFramework::ChangeScene(SCENE_TYPE_LOAD);
            return TRUE;
        }
    }

    return FALSE;
}



cProjectileQuestPortal::cProjectileQuestPortal( DWORD OwnerKey, PORTALKEY PortalKey, MAPCODE MapCode )
: m_OwnerKey( OwnerKey)
, m_PortalKey( PortalKey )
, m_MapCode( MapCode )
{

}

BOOL cProjectileQuestPortal::Process(DWORD TickTime)
{
    if( cProjectileChaseTarget::Process( TickTime ) == TRUE )
        return TRUE;

    if( g_pHero == NULL ) 
        return TRUE;

    //�ڽŰ� ��Ż���� �Ÿ�üũ
    WzVector vDistance =  g_pHero->GetVisiblePos() - GetPosition();
    float fDistance = VectorLength(&vDistance);

    //�Ÿ��� 0.8���� �̳��ϰ��
    if (fDistance <= 0.8f)
    {
        //Client->Server: (����Ʈor�̼�)��Ż�� �����ϰڴٴ�SYN
        MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN SendPacket;
        SendPacket.syncType	= MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN::eSYNC_TEMP_PORTAL;			
        SendPacket.portalKey = m_PortalKey;
        SendPacket.mapEnterancePortalIndex = m_MapCode;

        GlobalFunc::SendPacket( CI_GAMESERVERIDX, (BYTE *)&SendPacket, sizeof(SendPacket));
        return TRUE;
    }

    return FALSE;
}


cProjectileClone::cProjectileClone()
{

}

void cProjectileClone::SetActiveDelayTime( DWORD ActiveDelayTime )
{
    cProjectileBase::SetActiveDelayTime( ActiveDelayTime );
    cProjectileCloneObject::SetActiveCloneDelayTime( ActiveDelayTime );
}

void cProjectileClone::Destroy()
{
    cProjectileCloneObject::Destroy();
    cProjectileBase::Destroy();
}

BOOL cProjectileClone::Process( DWORD dwTick )
{
    cProjectileCloneObject::Process( dwTick );

    if( cProjectileBase::Process( dwTick ) == TRUE )
        return TRUE;

    return FALSE;
}
void cProjectileClone::Render()
{
    cProjectileCloneObject::Render();
    cProjectileBase::Render();
}

