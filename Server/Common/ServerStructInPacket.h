#pragma once
#ifndef COMMON_SERVERSTRUCT_IN_PACKET_H
#define COMMON_SERVERSTRUCT_IN_PACKET_H

// @history
//  - (f100520.3L) (WAVERIX) critical changes : changes code about a 'pragma pack'
//  - (f100527.2L) (WAVERIX) code arrangement

#include <StructInPacket.h>

//==================================================================================================
#pragma pack(push,1)
//==================================================================================================
// 기본 슬롯 정보
//
/* (CHANGES) (f100520.1L) remove an unused section
struct _ITEMSLOT : public BASESLOT
{
    ITEMSTREAM m_Stream;
};

struct _ITEMSLOTEX : public BASESLOT
{
    ITEMSTREAMEX m_Stream;
};
*/
//==================================================================================================
// 슬롯 전체 정보
//
/*
#define TOTALINFO_START(name, num)    \
    struct name##_TOTAL_INFO    \
    {                           \
        enum { MAX_SLOT_NUM = num, };

#define TOTALINFO_COUNT1(slottype, type, name1)   \
        typedef slottype SLOT_TYPE;         \
        type        m_##name1;              \
        SLOT_TYPE   m_Slot[MAX_SLOT_NUM];   \
        int GetSize() const {               \
            return (sizeof(*this) - (MAX_SLOT_NUM - m_##name1) * sizeof(SLOT_TYPE)); \
        }
#define TOTALINFO_COUNT2(slottype, type, name1, name2)    \
        typedef slottype SLOT_TYPE;         \
        type        m_##name1;              \
        type        m_##name2;              \
        SLOT_TYPE   m_Slot[MAX_SLOT_NUM];   \
        int GetSize() const {               \
            return (sizeof(*this) - (MAX_SLOT_NUM - m_##name1 - m_##name2) * sizeof(SLOT_TYPE)); \
        }

#define TOTALINFO_COUNT3(slottype, type, name1, name2, name3) \
        typedef slottype SLOT_TYPE;         \
        type        m_##name1;              \
        type        m_##name2;              \
        type        m_##name3;              \
        SLOT_TYPE   m_Slot[MAX_SLOT_NUM];   \
        int GetSize() const {               \
            return (sizeof(*this) -         \
                    (MAX_SLOT_NUM - m_##name1 - m_##name2 - m_##name3) * sizeof(SLOT_TYPE)); \
        }
#define TOTALINFO_END   \
    };
*/
// (WARNING) the item stream structure format for serializing.
// the below code block should be follow a precedent

//--------------------------------------------------------------------------------------------------
// START
struct _EQUIP_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<_EQUIP_TOTAL_INFO,
                                                            ITEMSLOTEX_INSERVER,
                                                            MAX_EQUIPMENT_SLOT_NUM,
                                                            0,
                                                            0>
{
    //typedef ITEMSLOTEX_INSERVER SLOT_TYPE;
    POSTYPE m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(_EQUIP_TOTAL_INFO) == kDerivedClassSize);
        const int kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

struct _INVENTORY_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<_INVENTORY_TOTAL_INFO,
                                                                ITEMSLOTEX_INSERVER,
                                                                MAX_INVENTORY_SLOT_NUM,
                                                                MAX_TEMPINVENTORY_SLOT_NUM,
                                                                0>
{
    //typedef ITEMSLOTEX_INSERVER SLOT_TYPE;
    POSTYPE m_InvenCount;
    POSTYPE m_TmpInvenCount;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(_INVENTORY_TOTAL_INFO) == kDerivedClassSize);
        const int kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_InvenCount + m_TmpInvenCount));
    }
};

struct _ITEM_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<_ITEM_TOTAL_INFO,
                                                           ITEMSLOTEX_INSERVER,
                                                           MAX_EQUIPMENT_SLOT_NUM,
                                                           MAX_INVENTORY_SLOT_NUM,
                                                           MAX_TEMPINVENTORY_SLOT_NUM>
{
    //typedef ITEMSLOTEX_INSERVER SLOT_TYPE;
    POSTYPE m_EquipCount;
    POSTYPE m_InvenCount;
    POSTYPE m_TmpInvenCount;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(_ITEM_TOTAL_INFO) == kDerivedClassSize);
        const int kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize +
            sizeof(m_Slot[0]) * (m_EquipCount + m_InvenCount + m_TmpInvenCount));
    }
};

struct _WAREHOUSE_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<_WAREHOUSE_TOTAL_INFO,
                                                                ITEMSLOTEX_INSERVER,
                                                                MAX_WAREHOUSE_SLOT_NUM,
                                                                0,
                                                                0>
{
    //typedef ITEMSLOTEX_INSERVER SLOT_TYPE;
    POSTYPE m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(_WAREHOUSE_TOTAL_INFO) == kDerivedClassSize);
        const int kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

struct _GUILDWAREHOUSE_TOTAL_INFO : public _BASESPEC_SLOT_TOTAL_INFO<_GUILDWAREHOUSE_TOTAL_INFO,
                                                                     ITEMSLOTEX_INSERVER,
                                                                     MAX_GUILD_WAREHOUSE_SLOT_NUM,
                                                                     0,
                                                                     0>
{
    //typedef ITEMSLOTEX_INSERVER SLOT_TYPE;
    POSTYPE m_Count;
    SLOT_TYPE m_Slot[MAX_SLOT_NUM];
    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        BOOST_STATIC_ASSERT(sizeof(_GUILDWAREHOUSE_TOTAL_INFO) == kDerivedClassSize);
        const int kMinSize = sizeof(*this) - sizeof(m_Slot);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Slot[0]) * (m_Count));
    }
};

//==================================================================================================
//

//
//struct MISSION_RANK_MEMBER_INFO
//{
//	USERGUID	m_UserGuid;
//	TCHAR		m_tszCharName[MAX_CHARNAME_LENGTH+1];	//< 미리 NULL을 넣어둔다.
//	LEVELTYPE	m_LV;
//	BYTE		m_Class;
//};

struct MISSION_RECORD_INFO
{
	BYTE		m_byMissionNo;
	INT			m_iClearTime;
};

struct SPECIAL_ITEM_OWN_INFO
{
	BYTE		m_Apply;
	WORD		m_ItemType;
	WORD		m_Count;
};


//__NA000000_070523_REPORT_TO_SOLARAUTH
struct sCACHEITEM_REPORT_INFO
{
	DWORD		dwUserGuid;
	DWORD		dwCharGuid;
	TCHAR		szUserID[ID_MAX_LEN+1];
	DWORD		dwPriceGuid;
	WORD		wItemCode;
	BYTE		byCategoryID;
	DWORD		dwPrice;
};

// __NA000000_070523_REPORT_TO_SOLARAUTH
// 나중에는 위 자료형과 동일해질지도...
struct sCACHEITEM_REPORT_INFO_MINORVER
{
	INT			dwPriceGuid;
	WORD		wItemCode;
	BYTE		byCategoryID;
	DWORD		dwPrice;
};

// declared to support __NA_001359_20090619_HEIM_LOTTO_SYSTEM = { ~Lotto~ }
// 하임 로또 추첨 결과 건 레코드
struct ServerLottoResult
{
    WORD  wDrawNum;                     // 추첨 회차
    INT8  iNumWinners;                  // 1등 당첨자 수 (-1:진행, 0:이월)
    BYTE  bWinningNum[LOTTO_NUM_PICKS]; // 당첨 번호
    INT64 i64Jackpot;                   // 1등 당첨금
};

// 하임 로또 응모 건 레코드
struct ServerLottoEntry
{
    WORD wDrawNum;                      // 응모 회차
    BYTE bStatus;                       // 당첨 상태 (0:응모, 1~: 등위)
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // 응모 번호
};

// 하임 로또 당첨자 레코드
struct ServerLottoWinner
{
    INT64    i64Idx;                    // 응모 건 ID
    CHARGUID iCharGuid;                 // 캐릭터 ID
    BYTE     bStatus;                   // 당첨 상태
};

// (CHANGES) (f100623.1L) added by _NA000000_100623_CHECK_WMO_CHECKSUM_
struct EngineFileChecksum
{
    enum ePassResult {
        Passed = 0,
        Failed,
        Passed_But_NotMatchedWmo,
    };
    DWORD checksum_map;
    DWORD checksum_wmo;
    int pass_result;
};

//==================================================================================================
#pragma pack(pop)
//==================================================================================================

#endif //COMMON_SERVERSTRUCT_IN_PACKET_H
