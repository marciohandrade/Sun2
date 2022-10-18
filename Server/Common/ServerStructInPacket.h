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
// �⺻ ���� ����
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
// ���� ��ü ����
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
//	TCHAR		m_tszCharName[MAX_CHARNAME_LENGTH+1];	//< �̸� NULL�� �־�д�.
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
// ���߿��� �� �ڷ����� ������������...
struct sCACHEITEM_REPORT_INFO_MINORVER
{
	INT			dwPriceGuid;
	WORD		wItemCode;
	BYTE		byCategoryID;
	DWORD		dwPrice;
};

// declared to support __NA_001359_20090619_HEIM_LOTTO_SYSTEM = { ~Lotto~ }
// ���� �ζ� ��÷ ��� �� ���ڵ�
struct ServerLottoResult
{
    WORD  wDrawNum;                     // ��÷ ȸ��
    INT8  iNumWinners;                  // 1�� ��÷�� �� (-1:����, 0:�̿�)
    BYTE  bWinningNum[LOTTO_NUM_PICKS]; // ��÷ ��ȣ
    INT64 i64Jackpot;                   // 1�� ��÷��
};

// ���� �ζ� ���� �� ���ڵ�
struct ServerLottoEntry
{
    WORD wDrawNum;                      // ���� ȸ��
    BYTE bStatus;                       // ��÷ ���� (0:����, 1~: ����)
    BYTE bPickedNum[LOTTO_NUM_PICKS];   // ���� ��ȣ
};

// ���� �ζ� ��÷�� ���ڵ�
struct ServerLottoWinner
{
    INT64    i64Idx;                    // ���� �� ID
    CHARGUID iCharGuid;                 // ĳ���� ID
    BYTE     bStatus;                   // ��÷ ����
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
