#pragma once
//------------------------------------------------------------------------------
/**

	펫 클래스
	
	@author		광수

	@since		08.03.06

	@remarks	09.08.11(소스정리)
- 
*/

#include "Character.h"

struct sPet_Script_Info;
class uiPetGauge;

#define	C_PET_MAX_GAUGE		100

enum ePET_NAME_STATE
{
	NAME_DEFAULT		=0,	//펫이름(기본)
	NAME_DRAW,				//펫이름지정
	NAME_UNDRAW,			//펫이름안그림
};

typedef struct _PET_EFFECT_PARAM
{
	HANDLE		    hEffect;		//루프이펙트(포만감하락.친밀도맥스)핸들
	ePET_EFFECT     eEffectType;

}PET_EFFECT_PARAM;

//	PET DATA
struct PET_DATA
{
	DWORD					dwIndex;				//펫인덱스
	BOOL					bEffect;				//루프이펙트활성화
	ePET_NAME_STATE			ePetName;				//펫네임상태(활성.비활성)
	BOOL					bHeroPet;				//주인공펫이냐?
	BOOL					bFlyOnHead;				//머리위떠다니는펫
	BYTE					byFullness;				//펫포만감수치
	BYTE					byIntimacy;				//펫친밀도수치
	BOOL					bPetLock;				//펫강제멈춤상태
	BOOL					bCharacterScene;		//캐릭터씬이냐?
	HANDLE					hInstanceEffect;		//인스턴스이펙트(단발이펙트지만 이펙트가길어서 위치보정위해핸들보유)	

	//emoticon관련변수
	BOOL					bMaxIntimacyFlag;		//친밀도맥스이펙트발동flag
	BOOL					bLowFullnessFlag;		//포만감부족이펙트발동flag
	DWORD					dwMaxIntimacyTime;		//친밀도이펙랜덤발동타임1
	DWORD					dwMaxIntimacyOldTime;	//친밀도이펙랜덤발동타임2
	DWORD					dwLowFullnessTime;		//포만감이펙랜덤발동타임1
	DWORD					dwLowFullnessOldTime;	//포만감이펙랜덤발동타임2

	BOOL					bPetNameHide;			//이펙트안그리기flag
	ePET_EFFECT				eOldEffectType;			//-보정:이전인스턴스이펙트가 어떤타입이냐?(연속생성방지위해)
	DWORD					dwPetBoringCnt;			//펫5분대기타임

	WzVector				vOldPosition;			//펫이전위치
	PET_EFFECT_PARAM		PetEffectParam;			//펫루프이펙트구조체
	DWORD					dwIdlePetTime;			//주인공 대기타임
	DWORD					dwIdlePetOldTime;		//주인공 대기카운트
	DWORD					dwMasterObjKey;			//펫주인의 오브젝트키값
	BYTE					byMoveGaugeUI;			//고정petui
	BYTE					byHideState;				//펫하이드상태인지
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
	
	//순수가상합수재정의:pet에서는 안씀
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
				
	//위치관련	
	
	WzVector				GetPetBoundingVolume();						 	
	float					GetWarpDistance();
	float					GetFollowDistance();
	float					GetStopPositon();
	float					GetDistanceMaster(WzVector vPos);
	WzVector				GetBasePostion();
		
	//행동.게이지
	void					ProcessPetUnit(DWORD dwTick);				 	
	void					RenderPetGauge();
	
	//액션
	void					FollowAction();
	void					BaseAction(DWORD dwTick);
	void					WrapAction();

public:	
	//외부인터페이스
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
	case E_BORING:				return StrToWzID("R025");	//5분대기	
	case E_OWNER_LVUP:			return StrToWzID("R035");	//주인랩업
	case E_OWNER_DEATH:			return StrToWzID("R040");	//주인죽음
	case E_DECREASE_INTIMACY:	return StrToWzID("R045");	//친밀도하락
	case E_AUTO_PICK_ITEM:		return StrToWzID("R050");	//아이템자동줍기
	case E_EAT:					return StrToWzID("R015");	//먹이섭취
	case E_WARP:				return StrToWzID("k470");	//워프이펙트
	case E_DESTORY:				return StrToWzID("R006");	//소환해제
	case E_MIN_FULLNESS:		return StrToWzID("R000");	//포만감 min 
	case E_MAX_INTIMACY:		return StrToWzID("R002");	//친밀도 max
	default:return 0;
	}
}
