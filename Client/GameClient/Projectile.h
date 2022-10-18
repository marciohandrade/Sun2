/**
    발사체 or 추가 데미지 객체 처리 
@author
    -준태,지완

@since

@remarks
    -2005.7.22 화살 이외의 오브젝트 가능하게 정리 시작 
	-2005.09.17 Todo:교통 정리! 짱지저분 하다.
*/

#pragma once
//
//#include "../Utility/Narray.h"
//#define MAX_MULTITARGET           10
//#include "ObjectEffectData.h"
//#include "character.h"
//namespace PROJECTILE
//{
//    enum PROJECTILEID
//    {
//        PROJECTILEID_ARROW = 0,
//        PROJECTILEID_POISON_THORN,
//        PROJECTILEID_POISON_RAIN,
//        //PROJECTILEID_POWER_WAVE,    // 사용하지 마시오!!!!! 
//		PROJECTILEID_SONY_VINE,
//
//        ////연출용 발사체 
//        //PROJECTILEID_SWORD_WAVE,
//		//일반 발사체 
//		PROJECTILEID_COMMON,
//		//곡면 발사체 
//		PROJECTILEID_BEZIER,
//
//		//에어블로우
//		PROJECTILEID_AIRBLOW,
//		//타겟에 붙어 다닐것들 
//		PROJECTILEID_CHASE_TARGET,
//		PROJECTILEID_SWORD_TRAIL,
//		PROJECTILEID_ATTACHMENT_WEAPON,
//		PROJECTILEID_PORTAL,
//		PROJECTILEID_QUEST_POTAL,
//
//		//연출용 발사체
//		PROJECTILEID_BEZIER_TORCH,
//
//		//포이즌대거
//		PROJECTILEID_POISONDAGGER,
//		PROJECTILEID_VITAL_SUCTION,
//		PROJECTILEID_HPMP_SUCTION,
//
//    };
//};
//
//typedef struct _PROJECTILE_PARAM
//{
//	WzVector vStartPos;				//시작점
//	WzVector vTargetPos;			//목표점
//	DWORD    dwLifeTime;			//생명주기 :목표지점에 도달 하면 가만히 있고 시간이 다됐을경우 사라지는 것들 사용 
//	DWORD    dwMoveTime;			//이동시간 :목표 지점에 맞으면 사라지는것들 사용 
//    
//	DWORD    dwOwner;	
//	DWORD    dwTargetID;
//    float    fHeight;
//    DWORD    dwAttackSerial; 
//    DWORD    dwType;				//발사체 타입 
//	DWORD    dwImageCode;			//리소스 이미지 코드
//	WzID     effectID;
//    float    fSpeed;				//속도
//	float    fAccelate;				//가속도 
//	bool     bAllDamage;			//결과를 한꺼번에 뺄것인가
//	bool     bGroupDamage;          //종료시에 그룹으로 결과를 뺄것인가 
//	bool     bNotMove;
//	DWORD    dwActiveDelayTime;		//딜레이를 가지고 있다가 활동 할때 
//	bool     bAddNextSerial;        //결과를 처리 할때 시리얼을 증가 시킬것인가 
//	bool     bCameraShake;          //카메라를 흔들것인가
//
//	DWORD    dwDamageTime;          //데미지를 먹일 시간..event를 잡아낼 방법이 없을까...
//	bool     bDamageLoop;           //주기마다 계속 뺄것인가
//	bool     bFadeOut;              //유닛 스르륵 
//	WzID     EndeffectID;			//끝날때 출력할 이펙트 루핑이면 안된다.
//	
//	WzVector vScale;
//	WzVector vDir;
//
//
//	BYTE     bR;					//궤적 관련 색상
//	BYTE     bG;
//	BYTE     bB;
//	BOOL     bMakeTrail;			//궤적을 만들것인가
//	float    fTrailLength;
//	DWORD    dwTrailEffectDelay;
//	ENUM_ALPHABLEND AlphaMode;
//
//	struct 
//	{
//		DWORD     dwTargetObjectKey;
//		TOAIR_INFO  TOAIR;
//	} AIR_BLOW;
//
//
//    union
//    {
//        struct 
//        {
//            DWORD    dwTrailEffectDelay;
//        } UPPERWAVE;
//
//		struct 
//		{
//		   DWORD                dwTargetObjectKey;
//		   WzVector             vPostion;
//		   eSKILLEFFECT_POS     dwPosType;
//		   DWORD                dwBoneIndex;
//		   BOOL                 bRot;
//		} CHASE_TARGET;
//
//        struct 
//        {
//			WzID  aniIDStart;
//			WzID  aniIDEnd;
//			float fDirZ;
//			BOOL  bAniChange;
//
//        } POISON_RAIN;
//
//        struct 
//        {
//			DWORD    dwEffectDelay;
//        } POWER_WAVE;
//
//		struct 
//		{
//			WzID aniID;
//			float fDirZ;
//			BOOL  bDeBuff;
//		} SONYVINE;
//		
//		struct 
//		{
//			WzID aniID;
//		} POISON_THORN;
//
//
//		struct 
//		{
//			WzVector vPos0;
//			WzVector vPos1;
//			DWORD    dwPos0Delay;
//		}BEZIER;
//
//		struct 
//		{
//			WzVector vStartPos;//검기 윗부분
//			WzVector vEndPos;  //검기 아랫부분 
//			WzVector vDir;     //진행할 방향 
//			DWORD    dwTrailEffectDelay;
//
//			WzVector vAdditionalStartPos;
//			WzVector vAdditionalEndPos;  
//
//		} SWORD_TRAIL;
//
//		struct 
//		{
//			WzVector vCtrl0;
//			WzVector vCtrl1;
//			DWORD	 dwSecondTime;
//			BOOL     bReturn;
//		}POISON_DAGGER;
//
//		struct 
//		{
//			WzVector vPos;  //현재 발사체 위치
//			BOOL	 bMove; //발사체 움직일지 여부
//		}VITAL_SUCTION;
//
//		struct 
//		{
//			PORTALKEY   portalKey;          // 포탈키
//			MAPCODE     enterableMapCode;   // 맵코드
//		}QUEST_POTAL;
//    };
//
//} PROJECTILE_PARAM;
//
//
//
//class Projectile 
//{
//public:
//	Projectile(void);
//	virtual ~Projectile(void);
//
//    //Common
//	virtual void    Create( const PROJECTILE_PARAM & param );
//	virtual void    Destroy();
//
//    virtual BOOL    Process(DWORD dwTick);
//    virtual BOOL    Render();
//
//    virtual BOOL    ProcessDamage();
//	virtual BOOL    ProcessDamageGroup();
//	
//	virtual BOOL    ProcessDamage(DWORD dwTarget);
//
//	void            ProcessEffect(DWORD dwTick);
//	void            SetKey(DWORD dwKey);
//	DWORD           GetKey();
//
//    void            SetUnitDraw(CWzUnitDraw  *pDraw);
//    CWzUnitDraw *   GetUnitDraw();
//
//    //ARROW
//    BOOL            ProcessArrow(DWORD dwTick);
//
//	//Sony
//	BOOL            ProcessSonyVine(DWORD dwTick);
//	
//	//PoisonThorn
//	BOOL            ProcessPoisonThorn(DWORD dwTick);
//
//	//Poison
//	BOOL            ProcessPoisonRain(DWORD dwTick);
//	
//	//common
//	BOOL            ProcessCommon(DWORD dwTick);
//
//	//Weapon
//	BOOL            ProcessAttachmentWeapon(DWORD dwTick);
//
//	//bezier
//	BOOL            ProcessBezier(DWORD dwTick);
//
//	BOOL			ProcessTroch(DWORD dwTick);
//	
//	//PoisonDagger
//	BOOL			ProcessPoisonDagger(DWORD dwTick);
//
//	BOOL            ProcessAirBlow(DWORD dwTick);
//
//	BOOL            ProcessChaseTarget(DWORD dwTick);
//
//	BOOL            ProcessPortal(DWORD dwTick);
//	
//	BOOL			ProcessQuestPortal(DWORD dwTick);
//
//	BOOL			ProcessVitalSuction(DWORD dwTick);
//	void			SetVitalSuctionMove(BOOL bVal){m_Param.VITAL_SUCTION.bMove = bVal;}
//	DWORD			GetProjectileType(){return m_Param.dwType;}
//	DWORD			GetOwnerKey(){return m_Param.dwOwner;}
//	DWORD			GetTargetKey(){return m_Param.dwTargetID;}
//	DWORD			GetPortalKey(){ return ((m_Param.dwType!=PROJECTILE::PROJECTILEID_QUEST_POTAL) ? 0:m_Param.QUEST_POTAL.portalKey);}
//
//	BOOL			ProcessHPMPSuction(DWORD dwTick);
//
//#ifdef _KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//    // empty
//#else
//	//번개 효과 관련 
//	BOOL            ProcessLighting(DWORD dwTick);
//    void            CreateEffect( const WzVector & vPos );
//#endif
//
//	//검기
//	BOOL            ProcessSwordWave(DWORD dwTick);
//
//	BOOL            ProcessSwordTrail(DWORD dwTick);	
//
//    //연출용 물결 
//    BOOL            ProcessUpperBlowWave(DWORD dwTick);
//
//    void            SetMultiTarget(int iTargetNum,DWORD *pdwTargetList); // 멀티 타겟 설정
//    void            SetTexture(HANDLE hTexture);                         // simpleMesh에 입힐 택스쳐    
//    //물결효과용 
//	void            GenerateWave(WzVector vPos);
//
//	void            CreateEffect( const PROJECTILE_PARAM * pParam );
//
//
//
//
//	//화살 꼬리 
//	void            CreateTrail(PROJECTILE_PARAM param);
//	BOOL            RenderTrail();
//
//	//유닛용 내부 이펙트 
//	void                InitializeEffect();
//	void                DestroyAllEffect();
//	void                DestroyObjectEffect(int index);
//	void                UpdateEffectPos();
//
//	void                SetAni(WzID aniId, BOOL bLoop = FALSE);
//
//protected:
//	BYTE                	m_bR,m_bG,m_bB;
//	ENUM_ALPHABLEND     	m_AlphaMode;
//	HANDLE              	m_hEffect;
//	DWORD               	m_dwDelay;
//	PROJECTILE_PARAM    	m_Param;
//    WzSimpleMesh *      	m_pMesh;  
//	CWzUnitDraw  *      	m_pUnitDraw;
//	float               	m_fRate;
//	BOOL                	m_bUpdated;
//	BOOL                	m_bDamageProcessed;
//	
//	
//	// 멀티 타겟 처리용 변수
//	BOOL                	m_bMultiTarget; 
//	int                 	m_iCurTargetIndex;						// 현재 목표 인덱스
//	int                 	m_iTargetNum;							// 목표 수
//	DWORD               	m_pdwMultiTargetID[MAX_MULTITARGET];	// 목표 ID
//	// 임시
//	DWORD               	m_dwTrailEffectCount;
//	BOOL                	m_bActive;                              // 활성화 
//	DWORD               	m_dwCurrentTime;						// 흐른시간 
//
//#ifdef _KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//    // empty
//#else //_KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//	nArray<HANDLE>      	m_hEffectArray;                         // 여러개의 이펙트를 다룰경우
//#endif
//
//	BOOL                	m_bStart;
//
//	WzColor					m_wzColor;								//유닛 디퓨즈
//	// 유닛 내부용 이펙트
//	BOOL                    m_bTrailActivate[MAX_TRAIL];
//	HANDLE					m_hTrailEffect[MAX_TRAIL];
//	TRAIL_PARAM				m_arTrailParam[MAX_TRAIL];
//	EFFECT_PARAM            m_EffectParam[MAX_OBJECT_EFFECT];
//	float					m_fAngleZ;
//
//	BOOL                    m_bUseExtraMatrix;
//	WzMatrix                m_ExtraMatrix;
//	DWORD					dwProjectileKey;
//};
//
////------------------------------------------------------------------------------
///**
//*/
//inline void Projectile::SetKey(DWORD dwKey)
//{
//	dwProjectileKey=dwKey;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//inline DWORD Projectile::GetKey()
//{
//	return dwProjectileKey;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//inline   CWzUnitDraw  * Projectile::GetUnitDraw()
//{
//    return m_pUnitDraw; 
//}
