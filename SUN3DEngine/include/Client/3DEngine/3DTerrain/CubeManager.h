// cubemanager.h

#ifndef _3DTERRAIN_CUBEMANAGER_H_
#define _3DTERRAIN_CUBEMANAGER_H_

#include <programcommon/wzarchive.h>
#include <programcommon/wzhashtable.h>
#include <client/3dengine/common/boundingvolume.h>

class COctree;



#define DFLT_CUBE_SIZE		( 20.0f)


// 큐브 하나의 정보

typedef WORD WzCubeIndex;

typedef struct
{
	WzCubeIndex m_wciMyIndex;		// 자신의 큐브 번호
	int m_iNumNearCubes;					// 인근 큐브 개수
	WzCubeIndex m_wciNearCubes[26];
} St_CubeInfo;


class CCubeManager
{
public:
	CCubeManager(void);
	virtual ~CCubeManager(void);
	virtual void Clear();

public:
	BOOL Create( COctree *pOctree, float fCubeSize = DFLT_CUBE_SIZE);
	void Destroy();

	BOOL Serialize( CWzArchive *pArchive);

	// --- 영역 분할 정보
protected:
	BvAABB m_aabbRegion;
	float m_fCubeSize;
	WORD m_wDivisionCount[3];
	int m_iNumCube;
	St_CubeInfo *m_pCubeInfo;		// 큐브 정보를 담는 곳
	CWzHashTable <WORD, WzCubeIndex> m_htCubeInfo;

	// 인덱스 계산
	WzCubeIndex CubeIndex( int x, int y, int z);
	int CubeElement( WzCubeIndex wciIndex, int iElement);
	// 인덱스와 경계 간 계산
	WORD GetElementIndex( float fPos, int iElement);	// 큐브 인덱스 얻을때 좌표축 별 인덱스 얻기
	float GetElementPos( int iIndex, int iElement);	// 좌표축 별 인덱스를 통해 영역 경계 얻기
	// 기타
public:
	WzCubeIndex GetCubeIndex( WzVector wvPos);		// 해당 좌표가 포함될 큐브 인덱스 얻기
	BvAABB GetCubeRegion( WzCubeIndex wciIndex);	// 큐브가 차지하는 영역 얻기
	int GetTotalNumberOfCubeInfo()	{ return ( m_iNumCube); }	// 큐브 개수 얻기
	St_CubeInfo* GetCubeInfoByIndex( int iIndex)	{ return ( &m_pCubeInfo[iIndex]); }	// 인덱스에 의해 큐브 정보 얻기

	int GetNearCubeList( WzCubeIndex wciCube, WzCubeIndex *pwciResult, int iMaxCount);		// 인근 큐브 리스트 얻기 (개수 리턴)
	void AddNearCube( St_CubeInfo *pCubeInfo, WzCubeIndex wciNear);
};


#endif // _3DTERRAIN_CUBEMANAGER_H_
