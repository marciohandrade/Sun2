//------------------------------------------------------------------------------
/**

    오브 젝트를 copy 주로 연출용으로 쓰임 
    @author

    @since

    @remarks
        -2005.06.07 소스 정리(지완)
        - 

*/
#pragma once
#include "object.h"

class Clone 
	: public Object
{
public:
	Clone();
	~Clone();

	void	Destroy();
    void	CopyAppearance(Object *pObject);
	void	CopyAppearanceExceptAni(Object *pObject);


	void	CopyAttachment(Object *pObject);

	void	Init();
	BOOL	Process(DWORD dwTick);
	BOOL	Render();
	BOOL    Render(St_RenderCommand *pCommand);
	void	SetColor(WzColor color);
	void	ResetCurDelay();
	void	SetLifeTime( DWORD time ) { m_dwLifeTime = time; }
	BOOL	SetPosition(const WzVector& wvPos, int iTileIndex = -1);
	BOOL	SetRotation(float fAngle);
	BOOL	Move(WzVector vCurPos,WzVector vDestPos);
	void	SetMoveSpeed(float fMoveSpeed);
	void	SetAlphaBlend(ENUM_ALPHABLEND alpha);
	void	SetDisappear(DWORD dwTime);
	void	SetAppear(DWORD dwTime,BYTE byStartAlpha = 0);
	void    SetScaleAnimation(DWORD dwTime,float fDestScale);
	void    SetScale(float fScale);
    void    SetPlayAni(BOOL isAni);
    void	SetRefraction(DWORD dwTime);
	void	Vibrate(WzVector vDirection,float fForce);
	void    AttachToObject(DWORD dwObjectKey);
//	void    SetPermanant(BOOL bTrue);
	void    SetCurPos(WzVector vPos);
	void    SetRestraintAniTick(BOOL bFlag);



protected:
	WzVector		m_vStart;
	WzVector		m_vEnd;
	WzVector		m_vCurPos;
	WzVector		m_vDir;
	WzVector        m_vVibStartPos;
	BOOL			m_bMove;
    BOOL            m_IsAni;
	BOOL			m_bDisappear;
	DWORD           m_dwAttachObject;
	BOOL            m_bRestraintAniTick;
	// 떨기
	WzVector		m_vVibrateDirection;
	float			m_fVibrateForce;

    BOOL            m_bRefraction;
    DWORD			m_dwRefractionCount;
    DWORD			m_dwRefractionTime;
    BYTE			m_byRefractionAlpha;

	BOOL			m_bAppear;
	DWORD			m_dwAppearCount;
	DWORD			m_dwAppearTime;
	BYTE			m_byAppearAlpha;
	BYTE			m_byAppearStartAlpha;
	float			m_fMoveSpeed;
	float			m_fDistance;
	float			m_fMoved;
	float			m_fRefractionRate;
	float			m_fRefractionRateAlpha;
	WzColor			m_Color;
	DWORD			m_dwLifeTime;
	DWORD			m_dwCurDelay;
	DWORD			m_dwDelayDisappear;
	BOOL            m_bScaleAnim;
	DWORD           m_dwScaleTime;
	DWORD           m_dwScaleCount;
	float           m_fDestScale;
	float           m_fCurScale;
	float           m_fStartScale;
	ENUM_ALPHABLEND m_AlphaMode;

//	BOOL            m_bPermanant;
};

inline void Clone::SetPlayAni(BOOL isAni)
{
    m_IsAni=isAni;
}

