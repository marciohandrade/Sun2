// pathexplorer.h

#ifndef _3DTERRAIN_PATHEXPLORER_H_
#define _3DTERRAIN_PATHEXPLORER_H_

#define MAX_PATH_ROUTE	(512)


// �̵� ����

enum ENUM_PEMT
{
	PEMT_STANDING =	0,
	PEMT_WALKING,
	PEMT_THRUSTING,

	// ���� �� ���ϸ� ������ ����
	PEMT_CASE = 0x00FF,
	// �߰� ���� ( mask ���� )
    PEMT_JUMPING			= 0x0100,	// ������
	PEMT_SKIP_HEIGHT_CALC	= 0x0200,	// ���� ��� ����
};

class CPathExplorer  
{
public:
	CPathExplorer();
	virtual ~CPathExplorer();
	virtual void Clear();

	/// @name ���� ����
protected:
	int					m_iTile;			///< ���� ��ġ�� Ÿ�� ��ȣ (��ã�� ����)
	int					m_iTerrainMesh;		///< ���� ��ġ�� TerrainMesh ��ȣ (Normal �� �����ؼ� z �� ��� ����)
	WzVector			m_wvPos;			///< ���������� ���̴� ��ǥ (�̵��� �� ��ǥ�� �������� �ϰ� ������ �������� ���� m_wvVisiblePos �� �Ѵ�.
	WzVector			m_wvVisiblePos;		///< ������ �������� 3D ���� ��ǥ

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

	/// @name ã�ư� ����
protected:
	int					m_iTargetTile;		///< ��ǥ �� Ÿ�� ��ȣ
	WzVector			m_wvTarget;			///< ��ǥ �� 3D ���� ��ǥ
	DWORD				m_dwMovingStatus;	///< �̵� ����
	WORD				m_wAttribute;
	long				m_lnAgentID;		/// ������ Agent ID ��ȣ.
	
public:
	WzVector			GetTargetPos()	{ return ( m_wvTarget); }					// IsMoving() �� TRUE �� ��츸 ��ȿ, �̵��Ϸ��� �����
	const				WzVector* GetTargetPosPtr()	{ return ( &m_wvTarget); }	// IsMoving() �� TRUE �� ��츸 ��ȿ, �̵��Ϸ��� �����
	void				SetAllowedAttribute( WORD wAttribute)	{ m_wAttribute = wAttribute; }
	WORD				GetAllowedAttribute()				{ return ( m_wAttribute); }

	/// @name �̵� ���
protected:
	int					m_iCurTileRoute;
	int					m_iNumTileRoute;
	int					m_iTileRoute[MAX_PATH_ROUTE];

	WzVector			m_wvCurTarget;		///< ���� ��ǥ ��
	WzVector			m_wvDir;			///< ���� �̵� ���� ( m_wvTarget - m_wvPos)
	float				m_fMoveRate;		///< m_wvDir �� �������� �̵��� ( ���Ⱑ ū ��� �������� ���� ����)
	
public:
	virtual WzVector	GetCurTargetPos()	{ return ( m_wvCurTarget); }
	void				SetReadyToThrust( int iTargetTile, WzVector *pwvTarget);
	void				SetReadyToGo( int iTargetTile, WzVector *pwvTarget, int **ppiNumRoute, int **ppiTileRoute, int iCurTile = -1);	///< ����Ҷ��� ����, �� ã�� ������ �����͸� �ش�.
	void				SetMovingStatus( DWORD dwCheck);
	void				UnsetMovingStatus( DWORD dwCheck);
	virtual void		SetSHCStatus( BOOL bSkip)	{ SetMovingStatus( PEMT_SKIP_HEIGHT_CALC); }
	virtual BOOL		IsMoving()	{ return ( ( BOOL)( m_dwMovingStatus != PEMT_STANDING)); }
	virtual BOOL		CheckMovingStatus( DWORD dwCheck)	{ return ( ( m_dwMovingStatus & dwCheck) == dwCheck); }
	virtual void		Stop();
	BOOL				Progress( float fMove, float *pfRemain);
	virtual int			GetTileRoute( int *pTileRoute); /// �����÷ο찡 ������ �Լ��̹Ƿ� ��������.
    virtual int			GetTileRoute( int *pTileRoute, int buffer_size);
	int					GetNumRemainTile() { return (m_iNumTileRoute - m_iCurTileRoute - 1); }
	int					GetNextTile()
	{
		return ( ( m_iCurTileRoute + 1 < m_iNumTileRoute) ? m_iTileRoute[m_iCurTileRoute + 1] : -1);
	}
	int					GetFutureTile( int iStep = 2)	///< �� �ܰ� ���� Ÿ���� ������ ( 1 �̸� GetNextTile �� ����)
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
