#pragma once

//#include "Projectile.h"
//#include "ProjectileBase.h"
//
//
//class cProjectileEx
//    : public Projectile
//{
//private:
//    nProjectile::eTraceType m_TraceType;
//
//    WzVector m_CurrentPosition;
//    WzVector m_CurrentRotate;
//
//    float m_Rate;
//    float m_EndRate;
//    float m_TailRate;
//
//
//protected:
//    cProjectileEx();
//
//    void UpdateTargetPosition();     // override
//    void UpdateProjectilePosition();
//
//    void ProcessMeshProjectile();
//    void ProcessUnitProjectile();
//    void ProcessEffectProjectile();
//
//    void ProcessHit();
//
//    void SetRate( float rate, float end_rate, float tail_rate ) { m_Rate = rate; m_EndRate = end_rate; m_TailRate = tail_rate; }
//    float GetRate() { return m_Rate; }
//    float GetEndRate() { return m_EndRate; }
//    float GetTailRate() { return m_TailRate; }
//
//    void SetTargetObjectKey( DWORD object_key ) { m_Param.dwTargetID = object_key; }
//    DWORD GetTargetObjectKey() { return m_Param.dwTargetID; }
//    void SetTargetPosition( const WzVector& position ) { m_Param.vTargetPos = position; }
//
//    const WzVector& GetStartPosition() { return m_Param.vStartPos; }
//    const WzVector& GetTargetPosition() { return m_Param.vTargetPos; }
//
//    const WzVector& GetProjectilePosition() { return m_CurrentPosition; }
//    const WzVector& GetProjectileRotate() { return m_CurrentRotate; }
//    void SetProjectilePosition( const WzVector& position ) { m_CurrentPosition = position; }
//    void SetProjectileRotate(const WzVector& rotate ) { m_CurrentRotate = rotate; }
//
//
//    nProjectile::eTraceType GetTraceType() { return m_TraceType; }
//    void SetTraceType( nProjectile::eTraceType type ) { m_TraceType = type; }
//};
//
//
//
//
//class cProjectileArrow : public cProjectileEx
//{
//public:
//    cProjectileArrow();
//
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//
//class cProjectileSonyVine : public cProjectileEx
//{
//public:
//    cProjectileSonyVine();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//
//class cProjectilePoisonThorn: public cProjectileEx
//{
//public:
//    cProjectilePoisonThorn();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//
//class cProjectilePoisonRain : public cProjectileEx
//{
//public:
//    cProjectilePoisonRain();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//
//class cProjectileAttachmentWeapon : public cProjectileEx
//{
//public:
//    cProjectileAttachmentWeapon();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//
//class cProjectileBezier : public cProjectileEx
//{
//public:
//    cProjectileBezier();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//
//class cProjectileTroch : public cProjectileEx
//{
//public:
//    cProjectileTroch();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//
//
//class cProjectilePoisonDagger: public cProjectileEx
//{
//public:
//    cProjectilePoisonDagger();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//
//
//class cProjectileAirBlow: public cProjectileEx
//{
//public:
//    cProjectileAirBlow();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//
//class cProjectileQuestPortal: public cProjectileEx
//{
//public:
//    cProjectileQuestPortal();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//
//class cProjectileVitalSuction: public cProjectileEx
//{
//public:
//    cProjectileVitalSuction();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//class cProjectileHPMPSuction: public cProjectileEx
//{
//public:
//    cProjectileHPMPSuction();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//class cProjectileSwordWave: public cProjectileEx
//{
//public:
//    cProjectileSwordWave();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//class cProjectileSwordTrail: public cProjectileEx
//{
//public:
//    cProjectileSwordTrail();
//
//private:
//    BOOL OnProcessProjectile( DWORD dwTick );   // override
//
//};
//
////
////namespace nProjectileProcess
////{
////    BOOL            ProcessArrow(DWORD dwTick);
////    BOOL            ProcessSonyVine(DWORD dwTick);
////    BOOL            ProcessPoisonThorn(DWORD dwTick);
////    BOOL            ProcessPoisonRain(DWORD dwTick);
////    BOOL            ProcessCommon(DWORD dwTick);
////    BOOL            ProcessAttachmentWeapon(DWORD dwTick);
////    BOOL            ProcessBezier(DWORD dwTick);
////    BOOL			ProcessTroch(DWORD dwTick);
////    BOOL			ProcessPoisonDagger(DWORD dwTick);
////    BOOL            ProcessAirBlow(DWORD dwTick);
////    BOOL            ProcessChaseTarget(DWORD dwTick);
////    BOOL            ProcessPortal(DWORD dwTick);
////    BOOL			ProcessQuestPortal(DWORD dwTick);
////    BOOL			ProcessVitalSuction(DWORD dwTick);
////    BOOL			ProcessHPMPSuction(DWORD dwTick);
////    BOOL            ProcessSwordWave(DWORD dwTick);
////    BOOL            ProcessSwordTrail(DWORD dwTick);	
////    BOOL            ProcessUpperBlowWave(DWORD dwTick);
////};
//
