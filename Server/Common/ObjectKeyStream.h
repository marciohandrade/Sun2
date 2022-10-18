#pragma once

#include <Const.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
namespace nsKeyInfo
{
	//	(WAVERIX)(080708)
	//	(NOTE)
	//	-	xxx_START	: 해당 수치는 0으로 평가한다. 관례상 0은 입력 불허 키다.
	//	-	xxx_LAST	: 해당 수치까지 값을 제공한다.

	//	50000개의 키를 갖는다. - 플레이어는 훨씬 더 많은 지연 현상을 가질 수 있다. 따라서 정의된 대로 50000개 키를 보유하도록 한다.
	//	[1, 50000]
	const DWORD		PLAYER_OBJECT_KEY_START		= PLAYER_OBJECT_KEY;
	const DWORD		PLAYER_OBJECT_KEY_LAST		= MONSTER_OBJECT_KEY;
	//	10000개의 키를 갖는다. - 지연 현상을 감안해 기존대로 많은 키를 보유하도록 한다.
	//	[1, 10000]
	const DWORD		ZONE_KEY_START				=     0;
	const DWORD		ZONE_KEY_LAST				= 10000;

    const DWORD     PORTAL_KEY_START            = 10000;
    const DWORD     PORTAL_KEY_LAST             = 11000;

    // [11001, 15000]
    const DWORD     PARTY_KEY_START             = 11000;
    const DWORD     PARTY_KEY_LAST              = 15000;
    // CHANGES: f110831.2L, added to support '_NA_20110630_BATTLE_ZONE_RENEWAL'
    const DWORD     PARTY_ROOM_KEY_START        = 15000;
    const DWORD     PARTY_ROOM_KEY_LAST         = 18000;
    // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    const DWORD     BATTLE_GROUND_ROOM_KEY_START= 18000;
    const DWORD     BATTLE_GROUND_ROOM_KEY_LAST = 21000;
    //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    const DWORD     FREE_PVP_ROOM_KEY_START     = 21000;
    const DWORD     FREE_PVP_ROOM_KEY_LAST      = 24000;


};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct ObjectKeyStreamAddinEmpty
{
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<ObjectKeyStream>
//	-	Object Key 기반 인덱스 배열
//	-	SUN 정책에 의거 [0, 50000] 입력시 키 범위는 [1, 50001)까지의 50000개의 인덱스를 처리한다.
//	(WARNING)
//	-	입력되는 타입 _NODESTRUCTURE은 POD 정보만을 담는 struct여야 한다.
template< typename _OBJTYPE, DWORD _BEGIN, DWORD _END, typename _ADDIN_STRUCTURE=ObjectKeyStreamAddinEmpty >
class ObjectKeyStream
{
public:	typedef _OBJTYPE			object_type;
		typedef _ADDIN_STRUCTURE	addin_info;
		struct __declspec(align(4)) sNode
		{
			DWORD					ObjectKey;
			object_type*			pObject;
			addin_info				AddinInfo;

			sNode()
				: ObjectKey(0)
				, pObject(0)
			{}
			sNode( const sNode& rSrc )
				: ObjectKey( rSrc.ObjectKey )
				, pObject( rSrc.pObject )
				, AddinInfo( rSrc.AddinInfo )
			{}
		};
public:	typedef util::ARRAY_TYPEINFO< sNode, 0, _END-_BEGIN >			array_t;
private:typedef util::ARRAY_TYPEINFO< sNode, _BEGIN+1, _END+1 >		array_range_t;

private:
	typename array_t::array_type		m_Array;

public:		ObjectKeyStream()
			{
			}
public:
	inline	const typename array_t::array_type&		GetArrayList() { return m_Array; }

	inline	VOID			Clear()
	{
		new (&m_Array) typename array_t::array_type;
	}

	inline const sNode*		GetObjectInfo( const DWORD objectKey ) const
	{
		if( array_range_t::begin_<=objectKey && objectKey<array_range_t::end_ )
		{
			const DWORD index = objectKey-array_range_t::begin_;
			const sNode& rAlign = m_Array[index];
			return &rAlign;
		}
		return NULL;
	}

	inline	BOOLEAN			AddObjectInfo( const DWORD objectKey, object_type* const pObject, const addin_info& IN rAddinInfo )
	{
		if( array_range_t::begin_<=objectKey && objectKey<array_range_t::end_ )
		{
			const DWORD index = objectKey-array_range_t::begin_;
			sNode& rAlign = m_Array[index];
			if( rAlign.pObject )
				return FALSE;

			rAlign.ObjectKey	= objectKey;
			rAlign.pObject		= pObject;
			rAlign.AddinInfo	= rAddinInfo;
			return TRUE;
		}
		return FALSE;
	}

	inline	BOOLEAN			RemoveObjectInfo( const DWORD objectKey )
	{
		if( array_range_t::begin_<=objectKey && objectKey<array_range_t::end_ )
		{
			const DWORD index = objectKey-array_range_t::begin_;
			sNode& rAlign = m_Array[index];
			if( !rAlign.pObject )
				return FALSE;

			rAlign.ObjectKey	= 0;
			rAlign.pObject		= NULL;
			return TRUE;
		}
		return FALSE;
	}

	inline	BOOLEAN			UpdateObjectAddinInfo( const DWORD objectKey, const addin_info& rAddinInfo )
	{
		if( array_range_t::begin_<=objectKey && objectKey<array_range_t::end_ )
		{
			const DWORD index = objectKey-array_range_t::begin_;
			sNode& rAlign = m_Array[index];
			rAlign.AddinInfo = rAddinInfo;
			return TRUE;
		}
		return FALSE;
	}
};

