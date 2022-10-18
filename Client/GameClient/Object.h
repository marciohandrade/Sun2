#pragma once

#include "ObjectBase.h"
#include "ObjectData.h"
#include "StateController.h"
#include "ObjectChatData.h"
#include "ObjectEffectData.h"
#include "EventInfo.h"
#include "ObjectAniEventController.h"


class CWzUnitDraw;
class CPathExplorer;
struct APPEARANCEINFO;
class ObjectAniEventController;

class Object 
	: public ObjectBase
	, public ObjectChatData
	, public StateController
    , public ObjectAniEventController
{
private:
	void StateController::OnChangeState() { ApplyInterpolateAccesory(); }
	Object* StateController::GetObject() { return this; }


	ObjectData m_ObjectData;
    //ObjectEffectData m_EffectData;

private:
	ObjectData& GetObjectData() { return m_ObjectData; }
	const ObjectData& GetObjectData() const { return m_ObjectData; }


public:
	Object();
	virtual ~Object();

	virtual BOOL Create( DWORD dwObjectKey, DWORD dwID = 0 ); // dwID는 플레이어 또는 히어로의 클래스 코드이며, 나머지의 경우에는 미사용-_-
	virtual void Destroy();
	virtual BOOL Process( DWORD dwTick );

	virtual BOOL Render();
	virtual BOOL Render( St_RenderCommand* pCommand /* = NULL */ );
	virtual BOOL RenderOutline( const WzColor& color, float fThickness );


protected:
	void InitRender( size_t MaxPartNum, size_t BasePart, APPEARANCEINFO* pAppearInfo, size_t iAttachmentCount, BOOL bMakeBoneOnly );
	void RefreshRender( APPEARANCEINFO * pAppearInfo, BOOL bRefreshBone = FALSE);
	void ReleaseRender();

private:
	// 기본 
	BOOL 				m_bVisible;			// 표시 여부
	BOOL 				m_bEnableDisplay;	// 컬링후 표시하는지 여부 (직접 제어하지 않는다.)
	BOOL				m_CurShowFlag;		// 어딘가에 사용중인듯하나.. 의도를 모르겠음


	WzOctIndex			m_woiOctIndex;		// 옥트리의 인덱스

	BOOL				m_bAnimated;
	BOOL				m_bAniProcessed;
	//WzID				m_CurrentAnimID;

private:
	bool m_bUseDelayLoading;				// 지연 로딩을 사용할 것이냐
	bool m_bResourceForceRemove;			// 강제로 지우기 위한 플래그

protected:
	// 길찾기
	CPathExplorer*		m_pPathExplorer;			

public:
	virtual void        SetShow(BOOL bFlag);
	BOOL				GetShow() { return m_bVisible; }

	BOOL				IsEnableDisplay() const { return m_bEnableDisplay; }
	void				SetEnableDisplay(BOOL value) { m_bEnableDisplay = value; }

	void				SetCurShowFlag( BOOL value ) { m_CurShowFlag = value; }
	BOOL				GetCurShowFlag() const { return m_CurShowFlag; }


	// Position, Angle
	float				GetAngle() { return GetObjectData().GetAngle(); }
	float               GetAngleX() { return GetObjectData().GetAngleX(); }
	virtual void		SetAngleX(float fAngle ) { GetObjectData().SetAngleX( fAngle ); }
	virtual void		SetAngle(float fAngle, BOOL bSetVisibleAngle = FALSE ) { GetObjectData().SetAngle( fAngle, bSetVisibleAngle ); }
	virtual void		SetAngle(float fAngleX, float fAngleY, float fAngleZ) { GetObjectData().SetAngle( fAngleX, fAngleY, fAngleZ ); }
	virtual void		Rotate(float fAngle, BOOL bSetVisibleAngle = FALSE) { GetObjectData().Rotate( fAngle, bSetVisibleAngle ); }
	void				SetRandomAngle() { GetObjectData().SetRandomAngle(); }

	void				SetVisiblePos(const WzVector& vPos) { GetObjectData().SetVisiblePos( vPos ); }
	void				SetVisiblePos( float x, float y, float z ) { GetObjectData().SetVisiblePos( x, y, z ) ; }
	WzVector			GetVisiblePos() { return GetObjectData().GetVisiblePos(); }
	WzVector			GetPosition() { return GetObjectData().GetPosition(); }
	virtual BOOL		SetPosition( const WzVector& wvPos, int iTileIndex = -1) { return GetObjectData().SetPosition( wvPos, iTileIndex ); }
	virtual BOOL		SetPosition( const HONGPOS& pPos ) { return GetObjectData().SetPosition( pPos ); }

	void				SetDirection( const WzVector& TargetPosition, BOOL bVisibleAngle = FALSE ) { GetObjectData().SetDirection( TargetPosition, bVisibleAngle ); }
	void				SetDirection( const WzVector& TargetPosition, const WzVector& SrcPosition, BOOL bVisibleAngle = FALSE ) { GetObjectData().SetDirection( TargetPosition, SrcPosition, bVisibleAngle ); }
	WzVector			GetDirection() { return GetObjectData().GetDirection(); }


	// Parts, Attachment
	CWzUnitDraw*		GetBasePart() { return GetObjectData().GetBasePart(); }
	CWzUnitDraw**		GetParts() { return GetObjectData().GetRenderParts(); }
	CWzUnitDraw*		GetPart( size_t PartIndex ) { return GetObjectData().GetPart( PartIndex ); }
	size_t				GetPartCount() { return GetObjectData().GetPartCount(); }

	void				RefreshAniRefDraw() { GetObjectData().RefreshAniRefDraw(); }
	WzColor				GetColor() { return GetObjectData().GetColor(); }
	void				SetColor(int r,int g,int b,int a) { GetObjectData().SetColor( r, g, b, a ); }
	void				SetColor( WzColor Color ) { GetObjectData().SetColor( Color ); }
	void				HideAttachment(BOOL bFlag) { GetObjectData().SetHideAttachment( bFlag ); }

	void				EnableCustomLight(BOOL bFlag) { GetObjectData().EnableCustomLight( bFlag ); }
	void				SetShadowDraw(BOOL bShow) { GetObjectData().SetShadowDraw( bShow ); }
	void                SetSubTypeEffectShow(BOOL bShow) { GetObjectData().SetSubTypeEffectShow( bShow ); }
	void				DetachLightMap() { GetObjectData().DetachLightmap(); }
	void				SetRenderPartOption( size_t index, const RENDER_PART_OPTION& Option) { GetObjectData().SetRenderPartOption( index, Option ); }



	// Scale
	virtual void		SetScale(float fScale) { GetObjectData().SetScale( fScale ); }
	void				SetScale(const WzVector& vScale) { GetObjectData().SetScale( vScale ); }
	void				GetScale(WzVector* pScale) { GetObjectData().GetScale( pScale ); }
	WzVector            GetScale() { return GetObjectData().GetScale(); }

	CPathExplorer*		GetPathExplorer() { return GetObjectData().GetPathExplorer(); }

	//
	bool IsUseDelayLoading() const { return m_bUseDelayLoading; }
	void SetUseDelayLoading(bool value) { m_bUseDelayLoading = value; }
	bool IsForceRemove() { return m_bResourceForceRemove; }
	void SetForceRemove( bool bForce ) { m_bResourceForceRemove = bForce; }
	BOOL IsCompletelyLoaded() { return GetObjectData().IsCompletelyLoaded(); }
    BOOL IsCompletelyFullLoaded() { return GetObjectData().IsCompletelyFullLoaded(); }



protected:
	BOOL				IsAnimated() const { return m_bAnimated; }
	WzOctIndex			GetOctIndex() const { return m_woiOctIndex; }

	virtual BOOL IsSimpleRenderMode() { return FALSE; }	// 간략화한 랜더를 사용할 것인가. (로딩이 덜 된 플레이어)
	virtual WzID GetTrailEffectCode(); // 검기 관련 이펙트 코드를 가져옴
    virtual BOOL IsBodyOnlyRenderMode() { return FALSE; }


public:

	// Animation 관련
	WzAnimationInfo*	GetAnimationInfo(WzID idAnimation);
	WzID				GetCurrentAnimation();
	size_t				GetAnimationCount();
	//------------------------------------------------------------------------------
	//! Qualifier:
	//! Parameter: const char * pAnimation
	//! Parameter: BOOL bLoop : 반복 여부
	//! Parameter: BOOL bRestart : bLoop == TRUE 이면 무조건 bRestart = FALSE로 셋팅됨
	//------------------------------------------------------------------------------
	virtual void		SetAnimation(const char* pAnimation, BOOL bLoop = FALSE, BOOL bRestart = TRUE);
	virtual void		SetAnimation(WzID idAnimation, BOOL bLoop = FALSE, BOOL bRestart = TRUE);
	virtual void		SetCurrentBlendAnimation(char * pAnimation, 
		                                        int boneindex,
		                                        float fSpeed = 1.0f,
		                                        int iBlendFrame = -1,
		                                        BOOL bDelta = FALSE);
	virtual void		SetCurrentBlendAnimation(WzID idAnimation, 
	                                            int boneindex,
	                                            float fSpeed = 1.0f,
	                                            int iBlendFrame = -1,
	                                            BOOL bDelta = FALSE);
	virtual void		StopBlendAnimation();
	virtual void		SetCurrentFrame(float fFrame);
	virtual float		GetCurrentFrame();
	virtual float		GetTotalFrame();
	virtual float		ProcessAnimation(DWORD dwTick ,
		                                WzEvent *pEvent = NULL,
		                                BOOL bIgnoreEvent = FALSE);	


	// 바운딩볼룜
	WzBoundingVolume*	GetBoundingVolume();
	size_t              GetNumPickingBoundingVolume() { return GetObjectData().GetNumPickingBoundingVolume(); }
	WzBoundingVolume*	GetPickingBoundingVolume( size_t index) { return GetObjectData().GetPickingBoundingVolume( index ); }


	virtual void 		AnimateUnit();							// 애니메이션
	virtual float		GetHeightOffset() { return 0.f; }		// 높이를 보정해야 하는경우 오버라이딩
	virtual WzID		GetCurrentCamAnimation() { return 0; }

	// 단순 Part Access
	WzVector            GetSpecialBonePos(WzID id);
	WzVector            GetBonePos(int index);
	WzMatrix            GetBoneMatrix(int bone);
	float               GetCurrentProgressRate();				// 애니메이션 비율




	// Attachment 관련루틴
	BOOL				SetAttachment(size_t nIndex, const AttachmentInfo& AttachmentInfo, float fRequestRateResizeTexture = -1.0f);
	BOOL				DeleteAttachment(size_t nIndex);
	size_t				GetAttachmentCount() { return GetObjectData().GetAttachmentCount(); }
	const ObjectAttachment& GetAttachment(size_t Index) { return GetObjectData().GetAttachment(Index); }
	const AttachmentInfo& GetAttachmentInfo(size_t Index ) { return GetObjectData().GetAttachmentArray().at(Index).GetAttachmentInfo(); }

	void                SetAttachmentAnimation(size_t Index, WzID AniID, BOOL bLoop) { GetObjectData().SetAttachmentAnimation( Index, AniID, bLoop ); }
	void				ApplyInterpolateAccesory() { GetObjectData().ApplyInterpolateAccesory(); }
	BOOL				ProcessInterpolationAttachment( DWORD dwTick ) { return GetObjectData().ProcessInterpolationAttachment( dwTick ); }


    //int                 GetTrailEffectAttachmentIndex(int TrailIndex) { return m_EffectData.GetTrailAttachmentIndex(TrailIndex); }

	///////////////////////////////////////////////////////////////////////////////////////////////////

	//// 효과처리
	//void                InitializeEffect();
	//void                DestroyAllEffect();
	//void                DestroyObjectEffect(int index,int iBuffer);
	//void                MoveRemainEffect(int index);
	//void                DestroyAllLoopEffect();
	//void				UpdateEffectPos();

	//// 검기부분
	//void				StartTrail(int TrailIndex, const TRAIL_PARAM& TrailParam );
	//void				EndTrail(int TrailIndex);
	//void				UpdateTrail();
	virtual WzVector    GetTrailBonePos(int index, int start);

	// 발디딤 처리
    virtual void        OnFootStep(BOOL b2D = FALSE){}
    virtual void		OnWaterWave() {}
    virtual void		OnWaterEllipse() {}

	virtual DWORD		GetCurrentTileAtt( BOOL bForce = TRUE );

public:

	void                RenderInfo();
	virtual  float      GetMeshHeight();


	// 옥트리에 오브젝트 삽입
	void				AddObjectToOctree();			
	void				RemoveObjectToOctree();
	virtual void		UpdateOctree() { AddObjectToOctree(); }


    virtual void		RestoreObjectName() {}

	// 기타

#ifdef __NA_0_20080415_PROCESS_END_PROECSS
	virtual int				OnEndProcess( DWORD dwTick ){ return 0;}
#endif





private:

	void ProcessAttachmentAnimation(DWORD dwTick);

	//void ProcessSoundEvent(const SoundEventInfo& SoundEventInfo);
	//void ProcessEffectEvent(const EffectEventInfo& EffectEventInfo);
	//void ProcessCustomEvent(const CustomEventInfo& CustomEventInfo);
};

extern const DWORD   ciInterpolateTick;