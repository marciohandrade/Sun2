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
ä���ߴ� ��ǰ�� ����Ʈ

�����ܿ����� Ƚ�� üũ�� ���� �ʿ��� Ŭ�����̳� Ŭ���ʿ��� ���� �ʿ�ġ ����.

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

	// ä�����ΰ�?
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
	DWORD	m_ObjKeyCollecting;	// ���� ä������ ä����
    
    //__NA001283_20090221_COLLECTION_SYNC
    CollectionInfo* m_pCollectionInfo;    // ä���� ������ ä���� ���� (Ŭ���� ���� ��Ŷ�� ���� ��� ������ ���� ����)

    //m_pCollectionInfo �� ���� �����Ѵ�.
    DWORD	m_ObjKeyCollected;	// ä���� �Ϸ�� ������Ʈ

	COLLECTED_HASH	m_Hash;		// ä���ߴ� ��ǰ
};
