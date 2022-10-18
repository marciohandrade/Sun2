// ��� �پ�ٴϴ� ��ü�̸�, ������, ����Ʈ�� �ٷ� (�ٻ�ü�� �ƴ�)
// �̸��� ���� ����
#pragma once

#include "ProjectileObject.h"

class cProjectileChaseTarget 
    : public cProjectileTarget
    , public cProjectileObject
{
public:
    cProjectileChaseTarget();

private:
    // cProjectileObject
    const WzVector& GetObjectPosition() const { return cProjectileTarget::GetPosition(); }
    const WzVector& GetObjectRotate() const { return cProjectileTarget::GetRotate(); }


protected:
    void Destroy();
    BOOL Process( DWORD TickTime );
    void Render();
};


// �������� ��� ����
class cProjectilePortal : public cProjectileChaseTarget
{
    BOOL Process( DWORD TickTime ); // override

public:
    cProjectilePortal() {}

};

class cProjectileQuestPortal : public cProjectileChaseTarget
{
    DWORD m_OwnerKey;
    PORTALKEY m_PortalKey;
    MAPCODE m_MapCode;

    BOOL Process( DWORD TickTime ); // override

public:

    cProjectileQuestPortal( DWORD OwnerKey, PORTALKEY PortalKey, MAPCODE MapCode );
};


class cProjectileClone 
    : public cProjectileBase
    , public cProjectileCloneObject
{
public:
    cProjectileClone();

    WzVector m_CloneDirection;
    WzVector m_ClonePosition;

    virtual void SetActiveDelayTime( DWORD ActiveDelayTime );

    void SetCloneDirection(WzVector value) { m_CloneDirection = value; }
    void SetClonePosition(WzVector value) { m_ClonePosition = value; }



private:
    // cProjectile
    const WzVector& GetClonePosition() const { return m_ClonePosition; }
    const WzVector& GetCloneDirection() const { return m_CloneDirection; }
    float GetCloneLifeTimeRate() { return cProjectileBase::GetTotalLifeTimeRate(); }

    void Destroy();
    BOOL Process( DWORD dwTick );
    void Render();
};