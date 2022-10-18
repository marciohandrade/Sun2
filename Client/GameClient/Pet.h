#pragma once
//------------------------------------------------------------------------------
/**

	�� Ŭ����
	
	@author		����

	@since		08.03.06

	@remarks	09.08.11(�ҽ�����)
- 
*/

#include "Character.h"

struct sPet_Script_Info;
class uiPetGauge;

#define	C_PET_MAX_GAUGE		100

enum ePET_NAME_STATE
{
	NAME_DEFAULT		=0,	//���̸�(�⺻)
	NAME_DRAW,				//���̸�����
	NAME_UNDRAW,			//���̸��ȱ׸�
};

typedef struct _PET_EFFECT_PARAM
{
	HANDLE		    hEffect;		//��������Ʈ(�������϶�.ģ�е��ƽ�)�ڵ�
	ePET_EFFECT     eEffectType;

}PET_EFFECT_PARAM;

//	PET DATA
struct PET_DATA
{
	DWORD					dwIndex;				//���ε���
	BOOL					bEffect;				//��������ƮȰ��ȭ
	ePET_NAME_STATE			ePetName;				//����ӻ���(Ȱ��.��Ȱ��)
	BOOL					bHeroPet;				//���ΰ����̳�?
	BOOL					bFlyOnHead;				//�Ӹ������ٴϴ���
	BYTE					byFullness;				//����������ġ
	BYTE					byIntimacy;				//��ģ�е���ġ
	BOOL					bPetLock;				//�갭���������
	BOOL					bCharacterScene;		//ĳ���;��̳�?
	HANDLE					hInstanceEffect;		//�ν��Ͻ�����Ʈ(�ܹ�����Ʈ���� ����Ʈ���� ��ġ���������ڵ麸��)	

	//emoticon���ú���
	BOOL					bMaxIntimacyFlag;		//ģ�е��ƽ�����Ʈ�ߵ�flag
	BOOL					bLowFullnessFlag;		//��������������Ʈ�ߵ�flag
	DWORD					dwMaxIntimacyTime;		//ģ�е����巣���ߵ�Ÿ��1
	DWORD					dwMaxIntimacyOldTime;	//ģ�е����巣���ߵ�Ÿ��2
	DWORD					dwLowFullnessTime;		//���������巣���ߵ�Ÿ��1
	DWORD					dwLowFullnessOldTime;	//���������巣���ߵ�Ÿ��2

	BOOL					bPetNameHide;			//����Ʈ�ȱ׸���flag
	ePET_EFFECT				eOldEffectType;			//-����:�����ν��Ͻ�����Ʈ�� �Ÿ���̳�?(���ӻ�����������)
	DWORD					dwPetBoringCnt;			//��5�д��Ÿ��

	WzVector				vOldPosition;			//��������ġ
	PET_EFFECT_PARAM		PetEffectParam;			//���������Ʈ����ü
	DWORD					dwIdlePetTime;			//���ΰ� ���Ÿ��
	DWORD					dwIdlePetOldTime;		//���ΰ� ���ī��Ʈ
	DWORD					dwMasterObjKey;			//�������� ������ƮŰ��
	BYTE					byMoveGaugeUI;			//����petui
	BYTE					byHideState;				//�����̵��������
	BASE_NPCINFO			m_NPCInfo;
    sPet_Script_Info* pet_script_ptr_;
};


class Pet : public Character
{
public:
	Pet( void );
	virtual ~Pet( void );

	virtual BOOL			Create( DWORD dwObjectKey, DWORD dwID = 0 );
	virtual void			Destroy( void );
	virtual BOOL			Process( DWORD dwTick );
	void					RenderInfo();
    virtual BOOL            Render();
	virtual void 			AnimateUnit();
	void					SetFullness(BYTE byValue){m_Pet.byFullness=byValue;}
	void					SetIntimacy(BYTE byValue){m_Pet.byIntimacy=byValue;}
 	void					RefreshPetName();
	void					RenderPetName();
	virtual void			StartExtraEffect();
	void					SetStatePetHide(BYTE byHide);
	HANDLE					CreateInstanceEffect(ePET_EFFECT eType,WzVector vPos);
	void					CreateInstanceEffect(ePET_EFFECT eType);
	BASE_NPCINFO *			GetMonsterInfo(){return &m_Pet.m_NPCInfo;}
	virtual WzID 			GetCurrentMoveAnimation();
	virtual WzID 			GetCurrentIdleAnimation();
    void UpdateMeshHeight();
	
	//���������ռ�������:pet������ �Ⱦ�
	virtual WzID			GetCurrentAttackAnimation(){return 0;}
	virtual WzID			GetCurrentDeathAnimation(){return 0;}
	virtual WzID			GetCurrentDamageAnimation(BOOL bRight,BOOL bCritical){return 0;}
	virtual void			SetExp(EXPTYPE dwExp){}
	virtual EXPTYPE			GetExp(){return 0;}
	virtual EXPTYPE			GetNextExp(){return 0;}
	virtual void			SetLevel(LEVELTYPE LV){}
	virtual LEVELTYPE		GetLevel(){return 0;}
	virtual DWORD			GetMaxHP(){return 0;}
	virtual DWORD			GetMaxMP(){return 0;}

private:	
	//process
	void					ProcessEmoticon(DWORD dwTick);
	WzID					GetPetEffectCode(ePET_EFFECT eType);
	void					AddMotionEffect(ePET_EFFECT eType,float fScaleSize=0.7f);
	void					UpdatePetEffectPos();
	void					DestroyEffect();
				
	//��ġ����	
	
	WzVector				GetPetBoundingVolume();						 	
	float					GetWarpDistance();
	float					GetFollowDistance();
	float					GetStopPositon();
	float					GetDistanceMaster(WzVector vPos);
	WzVector				GetBasePostion();
		
	//�ൿ.������
	void					ProcessPetUnit(DWORD dwTick);				 	
	void					RenderPetGauge();
	
	//�׼�
	void					FollowAction();
	void					BaseAction(DWORD dwTick);
	void					WrapAction();

public:	
	//�ܺ��������̽�
	Player*					GetMasterObject();
	void 					UpdatePetAttributes(BOOL bHero = TRUE);
	inline void				DestroyPetEffect(){CreateInstanceEffect(E_DESTORY,GetPosition());}
	void					PetEatAction();								 
	void					InitPetMonster(DWORD dwMaserKey,WORD wPetIndex);
	void					RefreshEffect();
	void					SetModifyPosition(float fAngle,float fDistance);
	void					SetPetLock(BOOL bLock);
    void					SetSubType(BYTE subtype);
    bool RefreshApperPet(Player* master_ptr);
    PET_DATA* GetPet() { return &m_Pet; }

private:
	PET_DATA				m_Pet;	

    uiPetGauge* pet_dialog() { return pet_dialog_; }

    uiPetGauge* pet_dialog_;
	
};
//------------------------------------------------------------------------------
/**
*/
inline float Pet::GetWarpDistance()
{
	return 15.f;
}
//------------------------------------------------------------------------------
/**
*/
inline float Pet::GetFollowDistance()
{
	return  ((m_Pet.bFlyOnHead) ? (1.3f):(2.5f));
}
//------------------------------------------------------------------------------
/**
*/
inline float Pet::GetStopPositon()
{
	return (m_Pet.bFlyOnHead ? 0.0f:1.3f);
}
//------------------------------------------------------------------------------
/**
*/
inline WzID Pet::GetCurrentIdleAnimation()
{
	WzID animationID = StrToWzID("N001");	
	return animationID;
}
//------------------------------------------------------------------------------
/**
*/
inline WzID Pet::GetCurrentMoveAnimation()
{
	WzID animationID = StrToWzID("N004");	
	return animationID;
}
//------------------------------------------------------------------------------
/**
*/
inline WzID Pet::GetPetEffectCode(ePET_EFFECT eType)
{
	switch(eType)
	{	
	case E_BORING:				return StrToWzID("R025");	//5�д��	
	case E_OWNER_LVUP:			return StrToWzID("R035");	//���η���
	case E_OWNER_DEATH:			return StrToWzID("R040");	//��������
	case E_DECREASE_INTIMACY:	return StrToWzID("R045");	//ģ�е��϶�
	case E_AUTO_PICK_ITEM:		return StrToWzID("R050");	//�������ڵ��ݱ�
	case E_EAT:					return StrToWzID("R015");	//���̼���
	case E_WARP:				return StrToWzID("k470");	//��������Ʈ
	case E_DESTORY:				return StrToWzID("R006");	//��ȯ����
	case E_MIN_FULLNESS:		return StrToWzID("R000");	//������ min 
	case E_MAX_INTIMACY:		return StrToWzID("R002");	//ģ�е� max
	default:return 0;
	}
}
