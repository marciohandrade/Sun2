#pragma once


#include "Camera.h"

struct EffectInfo;
class SkillScriptInfo;
class Object;

#include "SkillEventEffectInfoParser.h"


namespace SkillEffectFunc
{
	// 스킬 이펙트 생성
	HANDLE AddSkillEffectEfftct( Character* pTarget, WzID EffectID );

	// 스킬 이펙트를 발사체 방식으로 생성
	DWORD AddSkillEffectProjectile( Character* pCharacter, WzID EffectID );
	DWORD AddSkillEffectProjectile( Character* pCharacter, EffectInfo* pEffectInfo, BOOL bSyncAngle );


	// 스킬이 시작될때 사용되는 이펙트처리
	DWORD CreateStartSkillEffect(DWORD dwSkillClassCode, Character* pCharacter );
	DWORD CreateDirtyStartSkillEffect( DWORD dwSkillClassCode, Character* pCharacter );

	// 스킬 결과 처리 시점에서 처리되는 이펙트
	void CreateReceiverSkillEffect(DWORD dwSkillCode, int TargetIndex, Character* pTarget, Character* pChr);
	DWORD CreateDirtyReceiverSkillEffect(DWORD dwSkillClassCode, Character *pTarget );

	// 스킬을 사용시 캐스팅 이펙트 2종
	HANDLE CreateCastingSkillEffect(DWORD dwSkillClassCode, Character* pCharacter );
	HANDLE CreateCastingSkillBottomEffect(DWORD dwSkillClassCode, Character* pCharacter );

	// Custom 이벤트시 표현되는 단순 이펙트들의 처리
	BOOL ProcessSkillEffect( Character* pCharacter, DWORD dwSkillClassCode, int HitCount );
    void ProcessSkillHitEffect( Character* pCharacter, DWORD dwSkillClassCode, int HitCount );
    void ProcessSkillProjectile( Character* pCharacter, DWORD dwSkillClassCode, SkillScriptInfo* pSkillInfo, int HitCount );

    void CreateCameraViberation( const cSkillEventEffectInfoData* data, const WzVector& Position );
    void CreateFieldEffect( const cSkillEventEffectInfoData* data, const WzVector& Position );
    void CreateTargetEffect( const cSkillEventEffectInfoData* data, Character* pCharacter );

    void CreateSkillEventEffectByFieldSkill( DWORD dwSkillClassCode, const WzVector& Position );
    void CreateSkillEventEffectByAnimationEvent( DWORD dwSkillClassCode, Character* pCharacter, int HitCount );
    void CreateSkillEventEffect( DWORD dwSkillClassCode, Character* pCharacter, cSkillEventEffectInfoData::eEventType EventType );


	// 발자국 이펙트 (주로 돌격류의 효과)
	void CreateFootEffect( DWORD dwObjectKey, const WzVector& vPosition, DWORD dwTerrainAttribute);

	// 엘리멘탈이 스킬을 사용할때 손앞에서 표시되는 이펙트
	void CreateHandEffect(Object* pOwner, DWORD dwId, WzID effectID, const WzVector& vPos);

	// 카메라 흔들기
	void AddCameraVibration( Character* pCharacter, float fTime, Camera::VIBE_TYPE VibeType = Camera::VIBE_TYPE_ALL, float fPower = 0.05f );

};
