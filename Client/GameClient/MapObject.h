//------------------------------------------------------------------------------
/** MapObject

    @author
		���翵
    @since

    @remarks
        - 
*/
#pragma once
#include "object.h"

#define MAPOBJECT_CUSTOM_ATTRIBUTE 4


class MapObject
	: public Object
{
public:
	MapObject();
	virtual ~MapObject();

	virtual BOOL	Create( DWORD dwObjectKey, APPEARANCEINFO* pAppearInfo);
	virtual void	Destroy();

	virtual BOOL	Process( DWORD dwTick );

	virtual BOOL	Render();
	virtual BOOL	RenderOutline( const WzColor& color , float fThickness );
    virtual void    RenderInfo() {}
    virtual	void    RenderFocus() {}

	virtual void	UpdateOctree();

	void			RenderGuildMark();

	WzID			GetCurrentIdleAnimation();
	WzID			GetCurrentTriggerAnimation();

	void            SetTriggerAnimation(WzID AnimID,int iDelay = 0,BOOL bDone = FALSE, BOOL bLoop = FALSE);	
	void			SetTriggerObj(BOOL bFlag);
	void			SetTriggerOperating(BOOL bFlag);

	void            PlayTriggerAnimation();
		
	BOOL			IsEndTriggerAnimation();

	BOOL			IsTriggerOperating();
	BOOL			IsTriggerObj();
	BOOL			IsPickable();
	BOOL			IsSettingNPCObject();
	BOOL            IsWayPointObj();
	BOOL            IsTerrain();//�������� �� ������Ʈ �ΰ�
	BOOL            IsAlpha();  //�þ߸� ���� ��� ���� ó�� �Ұ��ΰ� 
	BOOL            IsTransparencyObj();//����Ʈ���� ����ó�� ������Ʈ �ΰ�
	BOOL            IsDominationObject();
    bool IsAirShipObject();

    bool IsBattleGroundStrongPointObject(); // ���� ���� ������Ʈ
    bool IsBattleGroundResurrectionObject(); // ���� ��Ȱ�ź� ������Ʈ
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    void ChangeBattleGroundObjectAppearance(battle_ground_object_type object_id); // ���� ������Ʈ ���� ����
    void ChangeBattleGroundObjectAnimation(battle_ground_waypoint_animation animation_type, bool is_visible_check = false);
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	WzID			GetCollectionID(){return m_wiCollectionID;}
	void			SetCollectionID(WzID wiCollectionID);
	void			RemoveCollectionIDandEffect();

	float			GetRotationAngle();
	WzVector		GetRotation();

	void            SetDelayStateChange(BOOL bShow,int iDelay = 0);	

	void            SetCustomAttribute(int index,DWORD dwCustom);
	DWORD           GetCustomAttribute(int index); 

	int             GetTriggerID();
	void            SetTriggerID(int iID);
	void			PushBackRealTriggerID(DWORD dwTriggerID){m_vecRealTriggerID.push_back(dwTriggerID);}
	BOOL			CheckTriggerActive();
	void            SetID(int iID);
	int             GetID();
	float			GetMeshHeight();
	int				GetBoneIndex(DWORD wiID);

public:
	BOOL			SetPosition( const WzVector& wvPos, int iTileIndex = -1 );	// Object ovrride 

	WzID            m_dwTriggerAnimation;
    BOOL            m_bTriggerAnimationLoop;
	BOOL			m_bTriggerObject;
	BOOL			m_bTriggerOperating;
	BOOL            m_bEndTriggerAnimation; // �̹� �÷��� �� ���´� Ʈ���� �ִϸ��̼��� �ٷ� ������.
    

	std::vector<DWORD>			m_vecRealTriggerID;

	WzID            m_dwDelayAnimation;
	int             m_iAnimationDelay;

	BOOL            m_bDelayStateChange;
	int             m_iStateChangeDelay;

	int              m_iTriggerID; //Ʈ���� ������Ʈ ID
	int              m_iID; //�������� ������ �ĺ� �ε��� 
	DWORD            m_dwCustomAttribute[MAPOBJECT_CUSTOM_ATTRIBUTE];
	float            m_fMeshHeight;
	WzBoundingVolume m_OldBv;
	WzID			 m_wiCollectionID;
	HANDLE			 m_hCollectionEftHdl;
protected:
	bool			m_bCanHit;			//������ �ִ� ��������(��ġ�� ���Ϳ� �ش��Ѵ�)
public:
	void				SetbCanHit(bool bVal){m_bCanHit = bVal;}
	bool				GetbCanHit(){return m_bCanHit;}

    bool shadow_object() const { return shadow_object_; }
    void set_shadow_object(bool val) { shadow_object_ = val; }
protected:
    bool is_billboard;
    bool shadow_object_; 
};
