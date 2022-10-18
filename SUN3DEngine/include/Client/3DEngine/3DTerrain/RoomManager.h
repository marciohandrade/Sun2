// roommanager.h

#ifndef _3DTERRAIN_ROOMMANAGER_H_
#define _3DTERRAIN_ROOMMANAGER_H_

#include <programcommon/wzarray.h>
#include <programcommon/wzlist.h>

class CWzArchive;
class CPathTile;



typedef struct
{
	// Ÿ�� ����
	int m_iNumTile;
	WORD *m_pwTileAttribute;
} St_RoomInfo;



class CRoomManager
{
public:
	CRoomManager(void);
	~CRoomManager(void);

	void Clear();
	virtual BOOL					Serialize( CWzArchive *pArchive, int iNumTile, CPathTile *pPathTiles);

	// RoomInfo ����Ʈ
protected:
	int								m_iMaxIndex;
	CWzArray<St_RoomInfo*>			m_arRoomInfoArray;
	CWzList<int>					m_lstFreeIndex;

	// RoomInfo �Ҵ�/����
public:
	int								CreateRoomInfo( St_RoomInfo *pNewRoom);		// ����ִ� Room ����
	void							DestroyAllRoomInfo();					// ��� Room ���� ����	

	int								GetAttCnt(int iRoom = 0);
	St_RoomInfo*					GetRoomInfo(int idx);
	void							ReSetRoomInfo(WORD* pwRoom, int iCnt, int iOldCnt);

public:
	virtual int						DuplicateRoomInfo( int iSourceIndex = 0);	// Room ���� ����
	virtual BOOL					DestroyRoomInfo( int iRoomIndex);			// Room ���� ����

	// Data ����
protected:
	int								m_iActiveRoom;

public:
	virtual void					SetActiveRoom( int iRoomIndex);				// �ش� Room Ȱ��ȭ
	WORD*							GetTileAttributeDimPtr();
	int								GetMaxIndex();
	int								GetActiveRoom();
};

#endif // _3DTERRAIN_ROOMMANAGER_H_