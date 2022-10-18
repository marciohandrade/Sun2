// cubemanager.h

#ifndef _3DTERRAIN_CUBEMANAGER_H_
#define _3DTERRAIN_CUBEMANAGER_H_

#include <programcommon/wzarchive.h>
#include <programcommon/wzhashtable.h>
#include <client/3dengine/common/boundingvolume.h>

class COctree;



#define DFLT_CUBE_SIZE		( 20.0f)


// ť�� �ϳ��� ����

typedef WORD WzCubeIndex;

typedef struct
{
	WzCubeIndex m_wciMyIndex;		// �ڽ��� ť�� ��ȣ
	int m_iNumNearCubes;					// �α� ť�� ����
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

	// --- ���� ���� ����
protected:
	BvAABB m_aabbRegion;
	float m_fCubeSize;
	WORD m_wDivisionCount[3];
	int m_iNumCube;
	St_CubeInfo *m_pCubeInfo;		// ť�� ������ ��� ��
	CWzHashTable <WORD, WzCubeIndex> m_htCubeInfo;

	// �ε��� ���
	WzCubeIndex CubeIndex( int x, int y, int z);
	int CubeElement( WzCubeIndex wciIndex, int iElement);
	// �ε����� ��� �� ���
	WORD GetElementIndex( float fPos, int iElement);	// ť�� �ε��� ������ ��ǥ�� �� �ε��� ���
	float GetElementPos( int iIndex, int iElement);	// ��ǥ�� �� �ε����� ���� ���� ��� ���
	// ��Ÿ
public:
	WzCubeIndex GetCubeIndex( WzVector wvPos);		// �ش� ��ǥ�� ���Ե� ť�� �ε��� ���
	BvAABB GetCubeRegion( WzCubeIndex wciIndex);	// ť�갡 �����ϴ� ���� ���
	int GetTotalNumberOfCubeInfo()	{ return ( m_iNumCube); }	// ť�� ���� ���
	St_CubeInfo* GetCubeInfoByIndex( int iIndex)	{ return ( &m_pCubeInfo[iIndex]); }	// �ε����� ���� ť�� ���� ���

	int GetNearCubeList( WzCubeIndex wciCube, WzCubeIndex *pwciResult, int iMaxCount);		// �α� ť�� ����Ʈ ��� (���� ����)
	void AddNearCube( St_CubeInfo *pCubeInfo, WzCubeIndex wciNear);
};


#endif // _3DTERRAIN_CUBEMANAGER_H_
