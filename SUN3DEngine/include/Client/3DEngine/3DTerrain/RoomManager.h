// roommanager.h

#ifndef _3DTERRAIN_ROOMMANAGER_H_
#define _3DTERRAIN_ROOMMANAGER_H_

#include <programcommon/wzarray.h>
#include <programcommon/wzlist.h>

class CWzArchive;
class CPathTile;



typedef struct
{
	// 타일 정보
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

	// RoomInfo 리스트
protected:
	int								m_iMaxIndex;
	CWzArray<St_RoomInfo*>			m_arRoomInfoArray;
	CWzList<int>					m_lstFreeIndex;

	// RoomInfo 할당/해제
public:
	int								CreateRoomInfo( St_RoomInfo *pNewRoom);		// 비어있는 Room 생성
	void							DestroyAllRoomInfo();					// 모든 Room 정보 제거	

	int								GetAttCnt(int iRoom = 0);
	St_RoomInfo*					GetRoomInfo(int idx);
	void							ReSetRoomInfo(WORD* pwRoom, int iCnt, int iOldCnt);

public:
	virtual int						DuplicateRoomInfo( int iSourceIndex = 0);	// Room 정보 복사
	virtual BOOL					DestroyRoomInfo( int iRoomIndex);			// Room 정보 제거

	// Data 참조
protected:
	int								m_iActiveRoom;

public:
	virtual void					SetActiveRoom( int iRoomIndex);				// 해당 Room 활성화
	WORD*							GetTileAttributeDimPtr();
	int								GetMaxIndex();
	int								GetActiveRoom();
};

#endif // _3DTERRAIN_ROOMMANAGER_H_