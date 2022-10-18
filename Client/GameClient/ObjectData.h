#pragma once


#include "ObjectAttachment.h"
#include "EventInfo.h"


struct RENDER_PART_OPTION
{
	BOOL    bSpecularVariation;
	BOOL    bCustomDiffuse;
	WzColor CustomDiffuseColor;

	void    Initialize()
	{
		bSpecularVariation = FALSE;
		bCustomDiffuse = FALSE;
	}
};

struct OBJECT_ATTACHMENT
{
	int				iBoneIndex;   // 붙은 본번호
	RESOURCECODE	codeResource; // 리소스 아이디
	int				iSubType;
	BOOL			bSpecularVariation;
	CWzUnitDraw *	pUnitDraw;
	WzID			animID;   
	BOOL			bAnimate;
	BOOL            bUseOffsetMatrix;
	WzMatrix		offsetMatrix;
	eATTACHTYPE		eAttachType;
	WzVector		wvInterpolateOffsetAttachment;
	int				iInterpolateTick;
	WzVector        vOldPos;
	float			fAccelerate;
};


struct APPEARANCEINFO;

class ObjectData
{
public:
	ObjectData();

	void Initialize();
	void Release();

	void InitializeRender( size_t MaxPartNum, size_t BasePart,  APPEARANCEINFO* pAppearInfo, size_t iAttachmentCount);
	//void RefreshRender( APPEARANCEINFO* pAppearInfo, BOOL bRefreshBone);
	void ReleaseRender();


	void StartRenderType( ENUM_ALTERNATIVE_RENDER eRenderType);		// 각 부착물에 효과를 적용 한다.
	void EndRenderType();											// 각 부착물에 예전 효과를 복원한다.


	// 랜더 설정
	void EnableCustomLight(BOOL bFlag);
	void SetShadowDraw(BOOL bShow);
	void SetRefractionRate(float fRefractionRate);
	void SetRefractionAlpha(float fRefractionAlpha);
	void SetInflationRange(float fRange);
	void SetRenderPartOption(size_t index, const RENDER_PART_OPTION& Option);

	void DetachLightmap();

	BOOL RenderBasicParts( St_RenderCommand* pCommand /* = NULL */ );
	BOOL RenderParts( St_RenderCommand* pCommand /* = NULL */ );
    BOOL RenderParts( St_RenderCommand* pCommand, int PartIndex );
	BOOL RenderOutline( const WzColor& Color, float fThickness );
    BOOL RenderPartsForCostume();
    BOOL RenderOutlineForCostume( const WzColor& Color, float fThickness );

	void RenderAttachment();

	void RefreshAniRefDraw();	// 각 파트의 에니메이션을 공유 한다.

	BOOL IsCompletelyLoaded();

    BOOL IsCompletelyFullLoaded();

	void SetSubTypeEffectShow(BOOL bShow);

private:

	//
	size_t					m_BasePart;
	size_t					m_MaxPartNum;
	//int					m_iAttachmentCount;	
	CWzUnitDraw**			m_ppRenderPart;
	RENDER_PART_OPTION*		m_pRenderPartVariation;

	// 부착물
	std::vector<ObjectAttachment> m_AttachmentArray;


	// 랜더 관련
	BOOL                    m_bCustomLight;
	BOOL                    m_bShadowDraw;
	BOOL                    m_bHideAttachment;

	ENUM_ALTERNATIVE_RENDER m_renderType;
	ENUM_ALTERNATIVE_RENDER m_oldRenderType;

	WzColor m_wzColor;					// 랜더러에 설정할 칼라

	bool m_bResourceForceRemove;

public:

	std::vector<ObjectAttachment>& GetAttachmentArray() { return m_AttachmentArray; }
	const std::vector<ObjectAttachment>& GetAttachmentArray() const { return m_AttachmentArray; }
	void SetAttachmentArray(const std::vector<ObjectAttachment>& value) { m_AttachmentArray = value; }

	const ObjectAttachment& GetAttachment( size_t Index ) { return m_AttachmentArray[Index]; }
	size_t GetAttachmentCount() {  return m_AttachmentArray.size(); }


	CWzUnitDraw* GetBasePart() { if( m_ppRenderPart ) return m_ppRenderPart[m_BasePart]; else return NULL;  }
	CWzUnitDraw* GetPart( size_t Index ) { if(m_ppRenderPart && (Index < m_MaxPartNum)) return m_ppRenderPart[Index]; else return NULL;  }

	CWzUnitDraw** GetRenderParts() const { return m_ppRenderPart; }
	size_t GetBasePartIndex() const { return m_BasePart; }
	void SetBasePartIndex(size_t value) { m_BasePart = value; }


	size_t GetPartCount() const { return m_MaxPartNum; }
	//void SetPartCount(size_t value) { m_MaxPartNum = value; }

	BOOL IsCustomLight() const { return m_bCustomLight; }
	void SetCustomLight(BOOL value) { m_bCustomLight = value; }

	BOOL IsShadowDraw() const { return m_bShadowDraw; }

	BOOL IsHideAttachment() const { return m_bHideAttachment; }
	void SetHideAttachment(BOOL value) { m_bHideAttachment = value; }


	void SetColor(WzColor wzColor);
	void SetColor(int r, int g, int b, int a);
	WzColor GetColor() {return m_wzColor;}


	WzBoundingVolume* GetBoundingVolume();
	size_t GetNumPickingBoundingVolume();
	WzBoundingVolume* GetPickingBoundingVolume( size_t index);


	void SetAttachmentAnimation(size_t nIndex, WzID animID,BOOL bLoop);
	void ApplyInterpolateAccesory();
	BOOL ProcessInterpolationAttachment( DWORD dwTick );



private:
	// 길찾기
	CPathExplorer*		m_pPathExplorer;			

	WzVector			m_wvVisiblePos;		// 출력할 위치
	float				m_fAngleZ;			// 오브젝트의 현재 Z 각도
	float				m_fAngleX;

public:
	CPathExplorer*		GetPathExplorer() { return m_pPathExplorer; }

	void		        SetScale(float fScale);
	void		        SetScale(const WzVector& vScale);
	void		        GetScale(WzVector* pScale);
	WzVector            GetScale();

	float				GetAngle() { return m_fAngleZ; };
	float               GetAngleX(){ return m_fAngleX; };
	virtual void		SetAngleX(float fAngle ) { m_fAngleX = fAngle; }
	virtual void		SetAngle(float fAngle, BOOL bSetVisibleAngle = FALSE);	
	virtual void		SetAngle(float fAngleX, float fAngleY, float fAngleZ);
	virtual void		Rotate(float fAngle, BOOL bSetVisibleAngle = FALSE);
	void				SetRandomAngle();

	void				SetVisiblePos(const WzVector& vPos) { m_wvVisiblePos = vPos; }
	void				SetVisiblePos( float x, float y, float z ) { m_wvVisiblePos.x = x; m_wvVisiblePos.y = y; m_wvVisiblePos.z = z; }
	WzVector			GetVisiblePos();
	WzVector			GetPosition();
	virtual BOOL		SetPosition( const WzVector& wvPos, int iTileIndex = -1);
	virtual BOOL		SetPosition( const HONGPOS& pPos );

	void				SetDirection( const WzVector& TargetPosition, BOOL bVisibleAngle = FALSE );
	void				SetDirection( const WzVector& TargetPosition, const WzVector& SrcPosition, BOOL bVisibleAngle = FALSE );
	WzVector			GetDirection();


private:
	void _ClampAngle(float* pAngle);
};


