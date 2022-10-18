#pragma once


#include "Camera.h"

struct EffectInfo;
class SkillScriptInfo;
class Object;

#include "SkillEventEffectInfoParser.h"


namespace SkillEffectFunc
{
	// ��ų ����Ʈ ����
	HANDLE AddSkillEffectEfftct( Character* pTarget, WzID EffectID );

	// ��ų ����Ʈ�� �߻�ü ������� ����
	DWORD AddSkillEffectProjectile( Character* pCharacter, WzID EffectID );
	DWORD AddSkillEffectProjectile( Character* pCharacter, EffectInfo* pEffectInfo, BOOL bSyncAngle );


	// ��ų�� ���۵ɶ� ���Ǵ� ����Ʈó��
	DWORD CreateStartSkillEffect(DWORD dwSkillClassCode, Character* pCharacter );
	DWORD CreateDirtyStartSkillEffect( DWORD dwSkillClassCode, Character* pCharacter );

	// ��ų ��� ó�� �������� ó���Ǵ� ����Ʈ
	void CreateReceiverSkillEffect(DWORD dwSkillCode, int TargetIndex, Character* pTarget, Character* pChr);
	DWORD CreateDirtyReceiverSkillEffect(DWORD dwSkillClassCode, Character *pTarget );

	// ��ų�� ���� ĳ���� ����Ʈ 2��
	HANDLE CreateCastingSkillEffect(DWORD dwSkillClassCode, Character* pCharacter );
	HANDLE CreateCastingSkillBottomEffect(DWORD dwSkillClassCode, Character* pCharacter );

	// Custom �̺�Ʈ�� ǥ���Ǵ� �ܼ� ����Ʈ���� ó��
	BOOL ProcessSkillEffect( Character* pCharacter, DWORD dwSkillClassCode, int HitCount );
    void ProcessSkillHitEffect( Character* pCharacter, DWORD dwSkillClassCode, int HitCount );
    void ProcessSkillProjectile( Character* pCharacter, DWORD dwSkillClassCode, SkillScriptInfo* pSkillInfo, int HitCount );

    void CreateCameraViberation( const cSkillEventEffectInfoData* data, const WzVector& Position );
    void CreateFieldEffect( const cSkillEventEffectInfoData* data, const WzVector& Position );
    void CreateTargetEffect( const cSkillEventEffectInfoData* data, Character* pCharacter );

    void CreateSkillEventEffectByFieldSkill( DWORD dwSkillClassCode, const WzVector& Position );
    void CreateSkillEventEffectByAnimationEvent( DWORD dwSkillClassCode, Character* pCharacter, int HitCount );
    void CreateSkillEventEffect( DWORD dwSkillClassCode, Character* pCharacter, cSkillEventEffectInfoData::eEventType EventType );


	// ���ڱ� ����Ʈ (�ַ� ���ݷ��� ȿ��)
	void CreateFootEffect( DWORD dwObjectKey, const WzVector& vPosition, DWORD dwTerrainAttribute);

	// ������Ż�� ��ų�� ����Ҷ� �վտ��� ǥ�õǴ� ����Ʈ
	void CreateHandEffect(Object* pOwner, DWORD dwId, WzID effectID, const WzVector& vPos);

	// ī�޶� ����
	void AddCameraVibration( Character* pCharacter, float fTime, Camera::VIBE_TYPE VibeType = Camera::VIBE_TYPE_ALL, float fPower = 0.05f );

};
