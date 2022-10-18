// light.h

#ifndef _3DTERRAIN_LIGHT_H_
#define _3DTERRAIN_LIGHT_H_

#include <client/3dengine/3drenderer/drawbase.h>
#include "octree.h"

//-----------------------------------------------------------------------------
// Desc: �������� ���Ǵ� ����Ʈ ���� Ŭ����
// 2005.05.02 KStaR79
//-----------------------------------------------------------------------------
#define DEFAULT_WORLD_LIGHT_RANGE			5.0f
#define DEFAULT_WORLD_LIGHT_INTENSITY		3.0f

typedef struct St_LightCache
{
	WzBoundingVolume m_bv;
	int m_iStateSerial;
	int m_iLight[MAX_NUM_ACTIVE_LIGHT];
} St_LightCache;


class CManagerLightTerrain
{
public:
	CManagerLightTerrain();
	virtual ~CManagerLightTerrain();

	// �ʱ�ȭ
	virtual void	Clear();
	virtual BOOL	Create( CDrawBase *pDraw );	
	virtual void	SetOctree( COctree *pOctree );
	virtual void	Destroy();	

	// ����Ʈ ���� �� ���� ����
	virtual BOOL	SetDirectionalLight( int iIndex, WzColor wcColor, WzVector wvDirection, ENUM_LIGHT_IRRA_TYPE eliType =0, float fRange = DEFAULT_WORLD_LIGHT_RANGE, float fIntensity = DEFAULT_WORLD_LIGHT_INTENSITY );
	virtual BOOL	SetPointLight( int iIndex, WzColor wcColor, WzVector wvPosition, ENUM_LIGHT_IRRA_TYPE eliType =0, float fRange = DEFAULT_WORLD_LIGHT_RANGE, float fIntensity = DEFAULT_WORLD_LIGHT_INTENSITY );
	virtual const	St_LightInformation* GetLightInfo( int iIndex) const;

	virtual	BOOL	SetLightIntensity( int iIndex, float fIntensity );
	virtual float	GetLightIntensity( int iIndex );

	// Active ����Ʈ ���� �� ���� ����	
	virtual int		GetIndexOfActiveLight( int iActive) const;	
	virtual const	St_LightInformation* GetActiveLightInfo( int iActive) const;
	virtual int		GetNumberOfActiveLight() const;	

	virtual void	ClearAllLight();
	virtual	void	ClearAllLight_MapEdit();
	virtual	void	DeleteLight( int iIndex);

	// ��������	
	virtual void	SetAreaLight( int iIndex, WzVector wvPos, float fRange, BOOL bValid );
	virtual	void	FindAreaLight( WzBoundingVolume bvTarget, int* iIndex );
	virtual	void	UseAreaLight( int* iIndex );	
	virtual void	GetAreaLightInfo( const int* iIndex, St_LightInformation* pLightInfo );
	
	virtual void	EnableAreaLight( BOOL bEnable) { m_bEnableAreaLight = bEnable; }
	virtual BOOL	IsAreaLight() const { return m_bEnableAreaLight; }

	virtual void	EnableIndoorLight( BOOL bEnable ) { m_bEnableIndoorLight = bEnable; }
	virtual BOOL	IsIndoorLight() const { return m_bEnableIndoorLight; }

    virtual void    RenewalLightToOctree();

protected:
	// ��������
	virtual void	ClearAllAreaLight();	
	virtual void	DeleteAreaLight( int iIndex );		

	// ����Ʈ�ѱ�-����
	virtual BOOL	TurnOnLight( int iIndex, BOOL bOn);	
	virtual BOOL	ChangeLight( int iActiveIndex, int iLightIndex );	
	
	// software shading �� ���� �� ���� ����
	virtual void	UpdateWzdLightInfo();

protected:
	BOOL			m_bEnableAreaLight;
	BOOL			m_bEnableIndoorLight;
	CDrawBase*		m_pDraw;
	COctree*		m_pOctree;

	St_LightInformation	 m_LightInfo[MAX_NUM_TERRAIN_LIGHT];	// �� ����	
	int	m_iIndexOfActiveLight[MAX_NUM_ACTIVE_LIGHT];			// ������� �� Index	
	int	m_iIndexOfDirLight[MAX_DIR_LIGHT];						// DirectionalLight Index

	// ĳ�� ó��
protected:
	BOOL GetResultFromCache( int iIndex, WzBoundingVolume *pbvTarget, int* iAreaIndex);
	void UpdateCache( int iIndex, WzBoundingVolume *pbvTarget, int* iAreaIndex);
	DWORD CalculateHashValue( WzBoundingVolume *pbvTarget);
	DWORD CalcAsDWORD( DWORD *pdwPtr, int iCount);
	int CalcIndexFromHash( DWORD dwHash);
	int m_iCacheSize;
	St_LightCache *m_pCache;
	int m_iStateSerial;
	void NotifyStateChanged()	{ if ( ++m_iStateSerial > 100000) { m_iStateSerial = 0; } }
};



// inline

inline const St_LightInformation* CManagerLightTerrain::GetLightInfo( int iIndex) const
{
	WzAssert( iIndex >= 0 && iIndex < MAX_NUM_TERRAIN_LIGHT );

    if( iIndex >= 0 && iIndex < MAX_NUM_TERRAIN_LIGHT )
    {
	    return &m_LightInfo[iIndex];
    }

    return NULL;
}

inline const St_LightInformation* CManagerLightTerrain::GetActiveLightInfo( int iActive) const
{
	int index = GetIndexOfActiveLight( iActive);
	if( index < 0) {
		return ( NULL);
	}
	return ( GetLightInfo( index));
}

inline int CManagerLightTerrain::GetIndexOfActiveLight( int iActive) const
{
	WzAssert( 0 <= iActive && iActive < MAX_NUM_ACTIVE_LIGHT);
	return ( m_iIndexOfActiveLight[iActive]);
}


// ���� �ΰ����� ���ϵǵ��� �ڵ�!
inline int CManagerLightTerrain::GetNumberOfActiveLight() const
{
	if( GetIndexOfActiveLight(1) >= 0 ) {
		return 2;
	}
	else if( GetIndexOfActiveLight(0) >= 0 ) {
		return 1;
	}
	return 0;
}

#endif // _3DTERRAIN_LIGHT_H_