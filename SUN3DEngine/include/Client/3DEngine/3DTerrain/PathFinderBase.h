// pathfinderbase.h

#ifndef _3DTERRAIN_PATHFINDERBASE_H_
#define _3DTERRAIN_PATHFINDERBASE_H_

#include "octnode.h"
#include "pathtile.h"

class CDiscAccess;
class COctree;
class CWzdBase;
class CDrawBase;
class CPathEdge;
class CWzArchive;
class CPathExplorer;
class CRoomManager;


enum ENUM_MD_RESULT	// MoveByDirection �Լ� ���
{
	MDR_UNKNOWN_ERROR = -2,	// �� �� ���� ����
	MDR_INVALID_TILE = -1,		// ����
	MDR_MOVED_WELL = 0,		// ��ǥ�� ����
	MDR_BLOCKED,		// ���� �ε���
	MDR_ON_EDGE,		// �𼭸� ���� ����
};

enum ENUM_COLLISION_OBJECT
{
	ECO_OBJECT,
	ECO_SPEEDTREE,
};
enum ENUM_COLLISION_TYPE
{
	ECT_STATIC,
	ECT_DYNAMIC,
};
enum ENUM_CHECK_COLLISION
{
	ECC_YES,
	ECC_NO,
};
class CPathFinderBase  
{
public:
	CPathFinderBase();
	virtual ~CPathFinderBase();	

	virtual BOOL					Create( COctree *pOctree, CRoomManager *pRoomManager);

	virtual void					Clear();
	virtual void					Destroy();
	virtual BOOL					Serialize( CWzArchive *pArchive) = 0;

protected:
	COctree *						m_pOctree;
	CRoomManager *					m_pRoomManager;

public:
	/// @name ���� ����
	virtual BOOL					GenerateFrom( CWzdBase *pWzd, int iMeshGroup) = 0;
	virtual BOOL					Refresh() = 0;					///< ��ã�� ���� �ٽ� ����
	virtual BOOL					RenewalCurrentTileInfoToOcxtree() = 0; //. ���� �ִ� Ÿ�� ������ ��Ʈ���� �ٽ� ��Ͻ�Ų��.
	
	virtual BOOL					EntryCollisionTerritory(ENUM_COLLISION_OBJECT eCO, long lnID, WzVector wzPos, WzBoundingVolume* pwzBv) {return TRUE;} //. �ٿ�� ���� aabb ����Ҷ�
	virtual BOOL					EntryCollisionTerritoryUnderLine(ENUM_COLLISION_OBJECT eCO, long lnID, WzVector wzPos, int iVectexNum, WzVector* pwzVector) {return TRUE;}//. �� �� �浹 ������ ����������� ����Ҷ�
	virtual BOOL					RemoveCollisionTerritory(ENUM_COLLISION_OBJECT eCO, long lnID){return TRUE;};
	
	virtual BOOL					RenewalDynamicCollisionAttribute(){ return TRUE; }	//. ActiveRoom ���� �Ӽ� ���̺��� �ٲ��� Attribute Collision�� ���� ����� �Ѵ�.
	
	virtual int						DuplicateDynamicCollision(int iSourceIndex = 0 ){return -1;}
	virtual BOOL					DestroyDynamicCollision( int iRoomIndex ){return FALSE;}

	virtual BOOL					LoadStaticCollision( CDiscAccess* pStream, const char* pszFileName) { return TRUE; }
	virtual BOOL					SaveStaticCollision( CDiscAccess* pStream, const char* pszFileName) { return TRUE; }

	/// @name �� ã��
	virtual BOOL					FindPath_Requst_NewAgent(CPathExplorer *ppe) { return TRUE; }
	virtual BOOL					FindPath_Requst_DestroyAgent(CPathExplorer *ppe) { return TRUE; }
	virtual BOOL					FindPath_Agent_Reset(CPathExplorer *ppe, WzVector *pwvPos, int iTile) { return TRUE; };
	virtual BOOL					FindPath_Progress(CPathExplorer *ppe,float fMove, ENUM_CHECK_COLLISION eCC = ECC_NO ){ return TRUE; }
	virtual ENUM_FINDPATH_RESULT	FindPath( int iTileStart, int iTileEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND) = 0;	///< Convex �� �̿��ؼ� �ʿ��� ���� Tile �� ��´�.
	virtual ENUM_FINDPATH_RESULT	FindPath_FullTile( int iTileStart, int iTileEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND) = 0;	///< Convex �� �Ⱦ��� ��� Tile �� ��´�.
	virtual ENUM_FINDPATH_RESULT	FindPath_AsConvex( int iConvexStart, int iConvexEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK)) = 0;	///< Convex ������ ��ã�⸸ �ؼ� ��� ���
	virtual BOOL					FindPath_CheckCollision(CPathExplorer *ppe) = 0;
	virtual BOOL					SetPath( int *pTileRouteDst, int *piNumTileRouteDst, int *pTileRoute, int iNumTileRoute) = 0;	///< ��� ����

	/// @name --- ������/Picking
public:
	virtual void					RenderPathTile( CDrawBase *pDraw, WORD wAttribute = 0, int *piTiles = NULL, int iNumTile = 0, BOOL bViewCollision = FALSE) = 0;
	virtual int						PickPathTile( WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE) = 0;
    virtual int						PickPathTileForDebug( WzRay *pwr, WzVector* pResultVertexArray, int MaxVertexArrayCount, int* MeshCount, COctNode** pNode, float *pfT, BOOL bCulling) = 0;

	virtual BOOL					CheckPickablePathTile(WzVector* pwvCollision)=0;
	virtual BOOL					IsOnTile( int iTile, WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE) = 0;	// Ư�� Ÿ�Ͽ� ���ؼ��� Picking �ϱ�
	virtual WzVector				GetRandomPosInTile( int iTile) = 0;
	virtual BOOL					CorrectHeightByTile( int iTile, WzVector *pwvPos) = 0;
	virtual void					RenderConvex( CDrawBase *pDraw, int *piConvexes, int iNumConvex, WzColor wcColor) = 0;

	
	/// @name --- Ÿ�� ���� ����/���
public:
	virtual int						GetNumPathTile() = 0;
	/// @name �Ӽ�
	virtual void					SetAttribute( int iTile, WORD wAttribute ) = 0;
	virtual void					UnsetAttribute( int iTile, WORD wAttribute ) = 0;
	virtual BOOL					CheckAttribute( int iTile, WORD wAttribute) = 0;
	virtual WORD					GetAttribute( int iTile) = 0;
	/// Octree ����
	virtual WzOctIndex				GetTileOctIndex( int iTile) = 0;
	/// Convex
	virtual int						GetConvexOfTile( int iTile) = 0;
	/// �ֺ� Ÿ�� ���
	virtual int						GetNearNodes( int iTile, int *piTile, int iMaxCount) = 0;
    /// Ÿ�Ͽ� ���� 3������ ��´�.
    /// @param : pvOut : Ÿ�Ͽ� ���� ���� �迭�� ��� ���� ����. WzVector[3]
    /// @param : iTile : ���ϴ� ��޽� Ÿ���� �ε���
    virtual void                    GetTileVertexInfo( WzVector* pvOut, const int iTile) = 0;

	/// @name --- ��ã�� ����� �̿��ؼ� ã�ư� ��
public:
	virtual int						GetTileToStand( WzVector wvPos, float *pfT = NULL, float fJumpLimit = -1.0f, float fAdjustLimit = -1.0f, WORD *pwAttribute = NULL) = 0;	///< 3D ��ǥ �� �� ������ �� ���� Ÿ�� ��� (���ϰ��� -1�� ��� �� �� ���� ��)

	virtual int						GetAllowTileToStand( WzVector wvPos, float *pfT = NULL, float fJumpLimit = -1.0f, float fAdjustLimit = -1.0f, WORD *pwAttribute = NULL,WORD wNotAllowAttribute=PTA_NO_WALK)=0;
	virtual CPathEdge*				GetNextPathEdge( int iTileFrom, int iTileTo) = 0;	///< iTileFrom ���� iTileTo �� ������ ��ǥ CPathEdge
	virtual int						GetGatewayIndexToConvex( int iConvexFrom, int iConvexTo) = 0;
	virtual CPathEdge*				GetGatewayToConvex( int iConvexFrom, int iConvexTo) = 0;	///< Convex ���� Convex �� �Ѿ�� ������
	virtual WzVector				GetGatewayInfoBetweenConvex( int iConvexFrom, int iConvexTo, WzVector *pwvEdge = NULL, Wz2DLine *pw2lLine = NULL) = 0;
	virtual WzVector				GetVisiblePointToNextEdge( WzVector wvCurrentPos, int iEdge, WzVector wvTargetNext) = 0;
	virtual WzVector				GetNearestPointToTarget( int iTile, WzVector wvTarget) = 0;	///< iTile ���� ���� �߿� wvTarget �� ���� ����� ���� ��´�.
	virtual WzVector				GetEdgeVertex( int iEdge, int iIndex ) = 0;
	virtual ENUM_MD_RESULT			MoveByDirection( int iCurrentTile, WzVector wvCurrentPos, WzVector wvDistance, int *piResultTile, WzVector *pwvResultPos, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), WzVector *pwvSlide = NULL) = 0;
	virtual void					HeadToNextTile( CPathExplorer *ppe) = 0;	// ���� Ÿ�Ϸ� ���ϵ��� �ϱ�
	virtual float					GetPathTotalLength(CPathExplorer *ppe) = 0;	
	/// @name --- ��� ���
public:
	virtual int						GetNumConvex() = 0;

	/// @name --- ��ã�� �Ž� ����/����� ���� �Լ�.
public:
	virtual BOOL					SavePathTiles( CWzArchive *pArchive)=0;
	
	/// �־��� BoundingVolume�ȿ� ��ġ�� Tile�� ����� ��ȯ�Ѵ�.
	virtual void					GetTilesInBoundingVolume( CDrawBase *pDraw, WzBoundingVolume *pBV, CWzArray<DWORD> *arResult) = 0;
	virtual	void					GetTilesInterSectBoundingVolume( CDrawBase *pDraw, WzBoundingVolume *pBV, CWzArray<DWORD> *arResult) = 0;

	// �˻�
public:
	virtual BOOL					CheckTileConnection( WzVector *pwvError) = 0;	// �߸��� ���� ������ FALSE
};


#endif // !defined(AFX_PATHFINDERBASE_H__62B6EF9C_2EF9_49EF_9679_FBA93BD8FFA1__INCLUDED_)
