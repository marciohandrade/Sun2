#pragma once
#ifndef UTILITY_COMMON_STRUCT_H
#define UTILITY_COMMON_STRUCT_H

#include <ArchiveOfStream.h>

//==================================================================================================

#pragma pack(push)
#pragma pack()

namespace util {
;
// (f100609.2L) the time info node to support a time synchronization of a C/S protocol
struct TimeSyncNode
{
    DWORD time_stamp_client_1st;
    DWORD time_stamp_server_1st;
    DWORD time_stamp_server_2nd;
    SYSTEMTIME server_time;
    LONG daylight_bias; // CHANGES, f100830.2L, apply a daylight of a server time zone
};

}; //end of namespace

#pragma pack(pop)

//==================================================================================================
//
#pragma pack(push,1)
// (f100527.4L) rearrangment about stream structures
template<class _DerivedType, class _SlotType,
         int _MaxSlotNumTab1, int _MaxSlotNumTab2, int _MaxSlotNumTab3>
struct _BASESPEC_SLOT_TOTAL_INFO
{
    // 255 == INVALID_POSTYPE_VALUE...
    enum { MAX_SLOT_NUM = _MaxSlotNumTab1 + _MaxSlotNumTab2 + _MaxSlotNumTab3 };
    BOOST_STATIC_ASSERT((0 <= _MaxSlotNumTab1 && _MaxSlotNumTab1 < 256) &&
                        (0 <= _MaxSlotNumTab2 && _MaxSlotNumTab2 < 256) &&
                        (0 <= _MaxSlotNumTab3 && _MaxSlotNumTab3 < 256) &&
                        (0 <  MAX_SLOT_NUM && MAX_SLOT_NUM < 256));
    static const int kNumberOfValueSlots = (_MaxSlotNumTab1 != 0) +
                                           (_MaxSlotNumTab2 != 0) +
                                           (_MaxSlotNumTab3 != 0);
    typedef _SlotType SLOT_TYPE;
    // number type : POSTYPE

    static const int kDerivedClassSize = (sizeof(POSTYPE) * kNumberOfValueSlots) +
                                         (sizeof(SLOT_TYPE) * MAX_SLOT_NUM);
    void Clear() {
        BOOST_STATIC_ASSERT(kDerivedClassSize == sizeof(_DerivedType));
        ZeroMemory(this, kDerivedClassSize);
        struct You_should_be_implement_clear_method { static const bool value = false; };
        BOOST_STATIC_ASSERT(You_should_be_implement_clear_method::value);
    }
};

template < typename SLOTETYPE >
struct sTEMPLATE_TOTAL_INFO
{
	explicit sTEMPLATE_TOTAL_INFO() : m_Count(0) {}
	typedef SLOTETYPE SLOT_TYPE;
	enum { MAX_SLOT_NUM = 0xFF, };
	BYTE		m_Count;
	SLOT_TYPE	m_Slot[MAX_SLOT_NUM];
	void Clear() { m_Count = 0; }
    uint16_t GetSize() const
    {
        // (f100527.3L)
        const size_t kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + (sizeof(m_Slot[0]) * m_Count));
    }

	void Output( ArchiveOfStream & archive )
	{
		archive << m_Count;
		for( BYTE i = 0 ; i < m_Count ; ++i )
		{
			archive << m_Slot[i];
		}
	}
	void Input( ArchiveOfStream & archive )
	{
		archive >> m_Count;
		for( BYTE i = 0 ; i < m_Count ; ++i )
		{
			archive >> m_Slot[i];
		}
	}
};

#pragma pack(pop)
//==================================================================================================

#endif //UTILITY_COMMON_STRUCT_H