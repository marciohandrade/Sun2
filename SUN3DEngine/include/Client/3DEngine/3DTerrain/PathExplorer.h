// pathexplorer.h

#ifndef _3DTERRAIN_PATHEXPLORER_H_
#define _3DTERRAIN_PATHEXPLORER_H_

#define MAX_PATH_ROUTE	(512)


// 이동 상태

enum ENUM_PEMT
{
	PEMT_STANDING =	0,
	PEMT_WALKING,
	PEMT_THRUSTING,

	// 다음 값 이하면 선택적 상태
	PEMT_CASE = 0x00FF,
	// 추가 상태 ( mask 연산 )
    PEMT_JUMPING			= 0x0100,	// 점프중
	PEMT_SKIP_HEIGHT_CALC	= 0x0200,	// 높이 계산 생략
};

class CPathExplorer  
{
public:
	CPathExplorer();
	virtual ~CPathExplorer();
	virtual void Clear();

	/// @name 현재 정보
protected:
	int					m_iTile;			///< 현재 위치한 타일 번호 (길찾기 관련)
	int					m_iTerrainMesh;		///< 현재 위치한 TerrainMesh 번호 (Normal 을 참조해서 z 값 얻는 관련)
	WzVector			m_wvPos;			///< 내부적으로 쓰이는 좌표 (이동은 이 좌표를 기준으로 하고 겉으로 보여지는 것은 m_wvVisiblePos 로 한다.
	WzVector			m_wvVisiblePos;		///< 겉으로 보여지는 3D 상의 좌표

public:
	void				SetTile( int iTile)	{ m_iTile = iTile; }
	virtual int			GetTile()	{ return ( m_iTile); }
	void				SetTerrainMesh( int iMesh);
	virtual int			GetTerrainMesh()	{ return ( m_iTerrainMesh); }
	void				SetPos( WzVector wvPos);
	void				SetPos( WzVector *pwvPos);
	WzVector			GetPos()	{ return ( m_wvPos); }
	const WzVector*		GetPosPtr()	{ return ( &m_wvPos); }
	WzVector			GetDir()	{ return ( m_wvDir); }
	const WzVector*		GetDirPtr()	{ return ( &m_wvDir); }
	void				SetVisiblePos( WzVector wvPos);
	void				SetVisiblePos( WzVector *pwvPos);
	virtual				WzVector GetVisiblePos()	{ return ( m_wvVisiblePos); }
	virtual const		WzVector* GetVisiblePosPtr()	{ return ( &m_wvVisiblePos); }
	void				Reset( int iTile);

	/// @name 찾아갈 정보
protected:
	int					m_iTargetTile;		///< 목표 점 타일 번호
	WzVector			m_wvTarget;			///< 목표 점 3D 상의 좌표
	DWORD				m_dwMovingStatus;	///< 이동 상태
	WORD				m_wAttribute;
	long				m_lnAgentID;		/// 길차지 Agent ID 번호.
	
public:
	WzVector			GetTargetPos()	{ return ( m_wvTarget); }					// IsMoving() 이 TRUE 인 경우만 유효, 이동하려는 대상점
	const				WzVector* GetTargetPosPtr()	{ return ( &m_wvTarget); }	// IsMoving() 이 TRUE 인 경우만 유효, 이동하려는 대상점
	void				SetAllowedAttribute( WORD wAttribute)	{ m_wAttribute = wAttribute; }
	WORD				GetAllowedAttribute()				{ return ( m_wAttribute); }

	/// @name 이동 경로
protected:
	int					m_iCurTileRoute;
	int					m_iNumTileRoute;
	int					m_iTileRoute[MAX_PATH_ROUTE];

	WzVector			m_wvCurTarget;		///< 현재 목표 점
	WzVector			m_wvDir;			///< 현재 이동 방향 ( m_wvTarget - m_wvPos)
	float				m_fMoveRate;		///< m_wvDir 의 방향으로 이동률 ( 기울기가 큰 경우 느려지는 단점 보완)
	
public:
	virtual WzVector	GetCurTargetPos()	{ return ( m_wvCurTarget); }
	void				SetReadyToThrust( int iTargetTile, WzVector *pwvTarget);
	void				SetReadyToGo( int iTargetTile, WzVector *pwvTarget, int **ppiNumRoute, int **ppiTileRoute, int iCurTile = -1);	///< 출발할때의 세팅, 길 찾기 정보는 포인터를 준다.
	void				SetMovingStatus( DWORD dwCheck);
	void				UnsetMovingStatus( DWORD dwCheck);
	virtual void		SetSHCStatus( BOOL bSkip)	{ SetMovingStatus( PEMT_SKIP_HEIGHT_CALC); }
	virtual BOOL		IsMoving()	{ return ( ( BOOL)( m_dwMovingStatus != PEMT_STANDING)); }
	virtual BOOL		CheckMovingStatus( DWORD dwCheck)	{ return ( ( m_dwMovingStatus & dwCheck) == dwCheck); }
	virtual void		Stop();
	BOOL				Progress( float fMove, float *pfRemain);
	virtual int			GetTileRoute( int *pTileRoute); /// 오버플로우가 가능한 함수이므로 쓰지말것.
    virtual int			GetTileRoute( int *pTileRoute, int buffer_size);
	int					GetNumRemainTile() { return (m_iNumTileRoute - m_iCurTileRoute - 1); }
	int					GetNextTile()
	{
		return ( ( m_iCurTileRoute + 1 < m_iNumTileRoute) ? m_iTileRoute[m_iCurTileRoute + 1] : -1);
	}
	int					GetFutureTile( int iStep = 2)	///< 몇 단계 뒤의 타일을 얻어오기 ( 1 이면 GetNextTile 과 같다)
	{
		return ( ( m_iCurTileRoute + iStep < m_iNumTileRoute) ? m_iTileRoute[m_iCurTileRoute + iStep] : -1);
	}
	int					GetLastTile()
	{
		return ( m_iTileRoute[m_iNumTileRoute - 1]);
	}
	int					GetNumTile(){return m_iNumTileRoute;}
	void				SetCurTarget( WzVector *pwvCurTarget = NULL);
};

#endif // _3DTERRAIN_PATHEXPLORER_H_
