/**
    �߻�ü or �߰� ������ ��ü ó�� 
@author
    -����,����

@since

@remarks
    -2005.7.22 ȭ�� �̿��� ������Ʈ �����ϰ� ���� ���� 
	-2005.09.17 Todo:���� ����! ¯������ �ϴ�.
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
//        //PROJECTILEID_POWER_WAVE,    // ������� ���ÿ�!!!!! 
//		PROJECTILEID_SONY_VINE,
//
//        ////����� �߻�ü 
//        //PROJECTILEID_SWORD_WAVE,
//		//�Ϲ� �߻�ü 
//		PROJECTILEID_COMMON,
//		//��� �߻�ü 
//		PROJECTILEID_BEZIER,
//
//		//�����ο�
//		PROJECTILEID_AIRBLOW,
//		//Ÿ�ٿ� �پ� �ٴҰ͵� 
//		PROJECTILEID_CHASE_TARGET,
//		PROJECTILEID_SWORD_TRAIL,
//		PROJECTILEID_ATTACHMENT_WEAPON,
//		PROJECTILEID_PORTAL,
//		PROJECTILEID_QUEST_POTAL,
//
//		//����� �߻�ü
//		PROJECTILEID_BEZIER_TORCH,
//
//		//��������
//		PROJECTILEID_POISONDAGGER,
//		PROJECTILEID_VITAL_SUCTION,
//		PROJECTILEID_HPMP_SUCTION,
//
//    };
//};
//
//typedef struct _PROJECTILE_PARAM
//{
//	WzVector vStartPos;				//������
//	WzVector vTargetPos;			//��ǥ��
//	DWORD    dwLifeTime;			//�����ֱ� :��ǥ������ ���� �ϸ� ������ �ְ� �ð��� �ٵ������ ������� �͵� ��� 
//	DWORD    dwMoveTime;			//�̵��ð� :��ǥ ������ ������ ������°͵� ��� 
//    
//	DWORD    dwOwner;	
//	DWORD    dwTargetID;
//    float    fHeight;
//    DWORD    dwAttackSerial; 
//    DWORD    dwType;				//�߻�ü Ÿ�� 
//	DWORD    dwImageCode;			//���ҽ� �̹��� �ڵ�
//	WzID     effectID;
//    float    fSpeed;				//�ӵ�
//	float    fAccelate;				//���ӵ� 
//	bool     bAllDamage;			//����� �Ѳ����� �����ΰ�
//	bool     bGroupDamage;          //����ÿ� �׷����� ����� �����ΰ� 
//	bool     bNotMove;
//	DWORD    dwActiveDelayTime;		//�����̸� ������ �ִٰ� Ȱ�� �Ҷ� 
//	bool     bAddNextSerial;        //����� ó�� �Ҷ� �ø����� ���� ��ų���ΰ� 
//	bool     bCameraShake;          //ī�޶� �����ΰ�
//
//	DWORD    dwDamageTime;          //�������� ���� �ð�..event�� ��Ƴ� ����� ������...
//	bool     bDamageLoop;           //�ֱ⸶�� ��� �����ΰ�
//	bool     bFadeOut;              //���� ������ 
//	WzID     EndeffectID;			//������ ����� ����Ʈ �����̸� �ȵȴ�.
//	
//	WzVector vScale;
//	WzVector vDir;
//
//
//	BYTE     bR;					//���� ���� ����
//	BYTE     bG;
//	BYTE     bB;
//	BOOL     bMakeTrail;			//������ ������ΰ�
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
//			WzVector vStartPos;//�˱� ���κ�
//			WzVector vEndPos;  //�˱� �Ʒ��κ� 
//			WzVector vDir;     //������ ���� 
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
//			WzVector vPos;  //���� �߻�ü ��ġ
//			BOOL	 bMove; //�߻�ü �������� ����
//		}VITAL_SUCTION;
//
//		struct 
//		{
//			PORTALKEY   portalKey;          // ��ŻŰ
//			MAPCODE     enterableMapCode;   // ���ڵ�
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
//	//���� ȿ�� ���� 
//	BOOL            ProcessLighting(DWORD dwTick);
//    void            CreateEffect( const WzVector & vPos );
//#endif
//
//	//�˱�
//	BOOL            ProcessSwordWave(DWORD dwTick);
//
//	BOOL            ProcessSwordTrail(DWORD dwTick);	
//
//    //����� ���� 
//    BOOL            ProcessUpperBlowWave(DWORD dwTick);
//
//    void            SetMultiTarget(int iTargetNum,DWORD *pdwTargetList); // ��Ƽ Ÿ�� ����
//    void            SetTexture(HANDLE hTexture);                         // simpleMesh�� ���� �ý���    
//    //����ȿ���� 
//	void            GenerateWave(WzVector vPos);
//
//	void            CreateEffect( const PROJECTILE_PARAM * pParam );
//
//
//
//
//	//ȭ�� ���� 
//	void            CreateTrail(PROJECTILE_PARAM param);
//	BOOL            RenderTrail();
//
//	//���ֿ� ���� ����Ʈ 
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
//	// ��Ƽ Ÿ�� ó���� ����
//	BOOL                	m_bMultiTarget; 
//	int                 	m_iCurTargetIndex;						// ���� ��ǥ �ε���
//	int                 	m_iTargetNum;							// ��ǥ ��
//	DWORD               	m_pdwMultiTargetID[MAX_MULTITARGET];	// ��ǥ ID
//	// �ӽ�
//	DWORD               	m_dwTrailEffectCount;
//	BOOL                	m_bActive;                              // Ȱ��ȭ 
//	DWORD               	m_dwCurrentTime;						// �帥�ð� 
//
//#ifdef _KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//    // empty
//#else //_KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//	nArray<HANDLE>      	m_hEffectArray;                         // �������� ����Ʈ�� �ٷ���
//#endif
//
//	BOOL                	m_bStart;
//
//	WzColor					m_wzColor;								//���� ��ǻ��
//	// ���� ���ο� ����Ʈ
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
