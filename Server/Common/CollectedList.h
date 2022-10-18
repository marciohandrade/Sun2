#pragma once
//=======================================================================================================================
/// CollectedList class
/*
@author
Lee Su Tek
@since
2009. 2. 27
@remarks
@note
채집했던 물품들 리스트

서버단에서는 횟수 체크를 위해 필요한 클래스이나 클라쪽에서 구지 필요치 않음.

@history

*/
//=======================================================================================================================
#include "StructInPacket.h"

typedef STLX_HASH_MAP<WzID,WORD>	COLLECTED_HASH;
typedef COLLECTED_HASH::iterator	COLLECTED_HASH_IT;

struct CollectionInfo;

class CollectedList
{
public:
	CollectedList()	
		: m_IsLoaded(false)
		, m_ObjKeyCollecting(0)
        , m_pCollectionInfo(NULL)
        , m_ObjKeyCollected(0)
    {}  //__NA001283_20090221_COLLECTION_SYNC

	virtual ~CollectedList()	{ Clear(); }

public:
	inline void  BeginCollecting(DWORD ObjKeyCollecting) { m_ObjKeyCollecting = ObjKeyCollecting; }
	inline void  EndCollecting()						 { m_ObjKeyCollecting = 0; }


	inline bool  IsLoaded()				{ return m_IsLoaded; }
	inline void  SetIsLoad(bool IsLoad) { m_IsLoaded = IsLoad; }

	// 채집중인가?
	inline bool  IsCollecting()			{ return !!m_ObjKeyCollecting; }
	inline DWORD GetObjKeyCollecting()	{ return m_ObjKeyCollecting; }

    //__NA001283_20090221_COLLECTION_SYNC
    inline void SetSuccessCollectionInfo(CollectionInfo* pCollectionInfo)	
    { 
        m_pCollectionInfo = pCollectionInfo;
        m_ObjKeyCollected = m_ObjKeyCollecting;
    }

    inline CollectionInfo* GetSuccessCollectionInfo(OUT DWORD& obj_key_collected)	
    { 
        obj_key_collected = m_ObjKeyCollected;
        m_ObjKeyCollected = 0;

        CollectionInfo* pCollectionInfo = m_pCollectionInfo;
        m_pCollectionInfo = NULL;        
        return pCollectionInfo;
    }

public:
	void Clear() { m_Hash.clear(); m_ObjKeyCollecting=0; m_IsLoaded = false; m_ObjKeyCollected = 0;}
	WORD GetCount(WzID CollectionID);

	void Add(WzID CollectionID, WORD wCount = 1);
	void SerializeStream( COLLECTED_TOTAL_INFO & IN rTotalInfo, eSERIALIZE eType );
	
	void Delete(WzID CollectionID, WORD wCount = 1);

private:
	bool	m_IsLoaded;
	DWORD	m_ObjKeyCollecting;	// 현재 채집중인 채집물
    
    //__NA001283_20090221_COLLECTION_SYNC
    CollectionInfo* m_pCollectionInfo;    // 채집에 성공한 채집물 정보 (클라의 성공 패킷에 의해 상기 정보로 보상 지급)

    //m_pCollectionInfo 와 같이 셋팅한다.
    DWORD	m_ObjKeyCollected;	// 채집이 완료된 오브젝트

	COLLECTED_HASH	m_Hash;		// 채집했던 물품
};
