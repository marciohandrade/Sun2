#ifndef _SOLAR_SLOT_SCSLOTSTRUCT_VERSION_H
#define _SOLAR_SLOT_SCSLOTSTRUCT_VERSION_H
#pragma once

//--------------------------------------------------------------------------------------------------
#define CREATE_ITEMSTREAMEX_VERSION(ver) \
    typedef union { \
        struct{ \
            ITEMPART_VER##ver   Part; \
            OPTIONPART_VER##ver OptionPart; \
        }; \
        BYTE pStream[sizeof(ITEMPART_VER##ver) + sizeof(OPTIONPART_VER##ver)]; \
    } ITEMSTREAMEX_VER##ver


#define TYPEDEF_ITEMSTREAMEX_VERSION(ver) \
    typedef ITEMPART_VER##ver ITEMPART; \
    typedef OPTIONPART_VER##ver OPTIONPART; \
    typedef ITEMSTREAMEX_VER##ver ITEMSTREAMEX
//--------------------------------------------------------------------------------------------------


#pragma pack(push,1)

template<typename _type = BYTE>
struct minislot_struct 
{
    _type Random   : 1; // �����̳�?
    _type Exist    : 1; // �����ϳ�?
    _type Position : 4; // �����Ѵٸ� Script�� ��Ʈ �ε����̴�. ����� (1~8)
    _type Reserve  : 2;
};

template<typename _type = UINT64>
struct LottoStruct 
{
    // �� 16 BYTE
    // 8 BYTE
    _type Index    : 16; // LotteryScript Index
    _type Identity :  8; // Identity State
    _type Count    :  8; // Item Count
    _type Reserve  : 32;

    // 8 BYTE
    minislot_struct<BYTE> minislot[MAX_PANDORA_SLOT_NUM];
};

//==================================================================================================

//-------------------------------------------------------------------------
// ������������ �ּ� ����
// ������ ���� ǥ�� �⺻ ����
//-------------------------------------------------------------------------
// 8byte
struct ITEMPART_VER1
{
    BYTE byType;
    DWORD dwSerial;
    WORD wCode;
    BYTE byDura;
    inline BOOL IsValid() { return (wCode != 0); } // (NOTE) unused, no change BOOL->bool and const
};

typedef ITEMPART_VER1   ITEMPART_VER2;

// 8 + 2 + 1 = 11Byte
struct ITEMPART_VER3
{
    union {   
        struct {
            DWORD64 dwSerial    : 40;
            DWORD64 byType      : 8; 
            DWORD64 byReserved2 : 8;
            DWORD64 byReserved1 : 8;
        };
        DWORD64 m_WorldSerial;
    };

    WORD wCode;
    BYTE byDura;
    inline bool IsValid() const { return (wCode != 0); }
};


typedef ITEMPART_VER3   ITEMPART_VER4;
typedef ITEMPART_VER3   ITEMPART_VER5;
typedef ITEMPART_VER5   ITEMPART_VER6; //_NA_20090925_ITEMSTREAM_INIT_RESERVED
typedef ITEMPART_VER6   ITEMPART_VER7; //__NA000000_090906_SOLAR_SLOT_CODE_ARRANGEMENT__
typedef ITEMPART_VER7   ITEMPART_VER8; //_NA002237_100802_EP2_SOCKET_SYSTEM_DB_
//typedef ITEMPART_VER7   ITEMPART_VERx; //_NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_
//==================================================================================================

//======================================= OPTIONPART ===============================================
// Version 1 : 11byte
struct OPTIONPART_VER1
{
    //------------------------------------------------------------------------------
    // 11Byte
    //------------------------------------------------------------------------------
    // 8byte == 64bit
    union {
        struct
        { // �Ϲ�
            UINT64 RankOption1     : 6;
            UINT64 RankOption2     : 6;
            UINT64 RankOption3     : 6;
            UINT64 RankOption4     : 6;
            UINT64 RankOption5     : 6;
            UINT64 RankOption6     : 6;
            UINT64 RankOption7     : 6;
            UINT64 RankOption8     : 6;
            UINT64 RankOption9     : 6; //54
            UINT64 Rank            : 4; //58
            UINT64 Enchant         : 4; //62
            UINT64 SocketNum       : 2; //64
        };
        struct
        { // �Ⱓ
            UINT64 bExpiredTime    :  1; // �ð� ���� ����
            UINT64 bProgressTime   :  1;
            UINT64 NotUsed         : 11; //13
            UINT64 DateTime        : 45; //58 : ������ [20051107246060:10��], max[35184372088832:35��], ������ 10�ð�25��35��
            UINT64 Enchant         :  4; //62
            UINT64 SocketNum       :  2; //64
        };
        struct
        { // ��ǥ
            UINT64 MapCode         : 16;
            UINT64 X               : 16; //32
            UINT64 Y               : 16; //48
            UINT64 Z               : 16; //64
            //UINT64 Enchant       : 4; //62
            //UINT64 SocketNum     : 2; //64
        };
    };

    // 2byte
    USHORT NOption         : 3;
    USHORT Block           : 1;
    USHORT SocketOption1   : 6;
    USHORT SocketOption2   : 6;

    // 1byte
    BYTE SocketOption3   : 6;
    BYTE Set             : 2;
};

// Version 2 : 12byte

struct OPTIONPART_VER2
{
    //--------------------------------------------------------------
    // 12Byte
    //-------------------------------------------------------------
    // 8byte == 64bit
    union
    {
        struct
        {// �Ϲ�
            UINT64 RankOption1     : 6;
            UINT64 RankOption2     : 6;
            UINT64 RankOption3     : 6;
            UINT64 RankOption4     : 6;
            UINT64 RankOption5     : 6;
            UINT64 RankOption6     : 6;
            UINT64 RankOption7     : 6;
            UINT64 RankOption8     : 6;
            UINT64 RankOption9     : 6; //54
            UINT64 Rank            : 4; //58
            UINT64 Enchant         : 4; //62
            UINT64 SocketNum       : 2; //64
        };
        struct
        {// �Ⱓ
            UINT64 bExpiredTime    :  1; // �ð� ���� ���� 
            UINT64 bProgressTime   :  1;
            UINT64 bExpiredWaitTime:  1;
            UINT64 NotUsed         : 10;
            UINT64 DateTime        : 45; //58 : ������ [20051107246060:10��], max[35184372088832:35��], ������ 10�ð�25��35��
            UINT64 Enchant         :  4; //62
            UINT64 SocketNum       :  2; //64
        };
        struct
        {// ��ǥ
            UINT64 MapCode         : 16;
            UINT64 X               : 16; //32
            UINT64 Y               : 16; //48
            UINT64 Z               : 16; //64
            //UINT64 Enchant         : 4; //62
            //UINT64 SocketNum       : 2; //64
        };
    };

    // 4byte
    UINT NOption         : 3;
    UINT Block           : 1;
    UINT SocketOption1   : 7;
    UINT SocketOption2   : 7;
    UINT SocketOption3   : 7;
    UINT Set             : 2;
    UINT UserBlock       : 1;
    UINT Reserved        : 4;
};

struct OPTIONPART_VER3      //< PC_ROOM_EXCLUSIVE_CONTENTS
{
    //-------------------------------------------------------------
    // 20Byte
    //-------------------------------------------------------------

    //16 byte
    union
    {
        struct
        {// �Ⱓ
            UINT64 RankOption1     : 7;
            UINT64 RankOption2     : 7;
            UINT64 RankOption3     : 7;
            UINT64 RankOption4     : 7;
            UINT64 RankOption5     : 7;
            UINT64 RankOption6     : 7;
            UINT64 RankOption7     : 7;
            UINT64 RankOption8     : 7;
            UINT64 RankOption9     : 7; //63
            UINT64 Rank            : 4; //4
            UINT64 bExpiredTime    : 1; // �ð� ���� ���� 
            UINT64 bProgressTime   : 1;
            UINT64 bExpiredWaitTime: 1;
            UINT64 NotUsed         : 6;
            UINT64 DateTime        : 45; //58 : ������ [20051107246060:10��], max[35184372088832:35��], ������ 10�ð�25��35��
            UINT64 Enchant         : 4; //62
            UINT64 SocketNum       : 2; //64

        };
        struct
        {// ��ǥ
            UINT64 MapCode         : 16;
            UINT64 X               : 16; //32
            UINT64 Y               : 16;
            UINT64 Z               : 16; //64
        };
        struct 
        {//����Ƽ��
            UINT64 LimitedIndex    : 16; //16    //����Ƽ�� �ε���
            UINT64 DuraMMax        : 9; //25    //�ִ� ������(�ִ� ������ ������) 
            UINT64 DuraMax         : 9; //34    //�ִ� ������(������ ����)
            UINT64 RepairCount     : 4; //38    //���� Ƚ��
            UINT64 DamageRatio     : 9; //47    //���ݷ� ����(���� ����) 
            UINT64 LimitValueRatio : 9; //56    //���� ��ġ ����
            UINT64 L_Enchant       : 4; //60
            UINT64 L_SocketNum     : 2; //62
            UINT64 Reserve1        : 2; //64

            UINT64 BasicOption1    : 7; //7     //�⺻�ɼ� 1
            UINT64 BasicOptValue1  : 6; //13    //�⺻�ɼ� 1 ��
            UINT64 BasicOption2    : 7; //20    //�⺻�ɼ� 2
            UINT64 BasicOptValue2  : 6; //26    //�⺻�ɼ� 2 ��
            UINT64 BasicOption3    : 7; //33    //�⺻�ɼ� 3
            UINT64 BasicOptValue3  : 6; //39    //�⺻�ɼ� 3 ��
            UINT64 Reserve2        : 25; //64    //�⺻�ɼ� 3 ��
        };
        struct
        {
            UINT64 FateInfoIndex       : 16; //16
            UINT64 F_DateTime          : 45; //61 : ������ [20051107246060:10��], max[35184372088832:35��], ������ 10�ð�25��35��            
            UINT64 F_bExpiredTime      : 1; //62 :  �ð� ���� ����
            UINT64 F_Identify          : 1; //63
            UINT64 F_bExpiredWaitTime  : 1; //64

            UINT64 F_AtkDefRatio       : 9; //9
            UINT64 F_DuraMax           : 9; //18
            UINT64 F_Enchant           : 4; //22
            UINT64 F_SocketNum         : 2; //24
            UINT64 F_Period            : 14; //38
            UINT64 F_Reserve3          : 26; //64
        };
    };

    // 4byte
    UINT NOption         : 3;
    UINT Block           : 1; //4 
    UINT SocketOption1   : 8; //12
    UINT SocketOption2   : 8; //20
    UINT SocketOption3   : 8; //28

    UINT Set             : 2; //30
    UINT UserBlock       : 1; //31
    UINT IsLimited       : 1; //32    // ����Ƽ�� �������ΰ�?  

};

typedef OPTIONPART_VER3 OPTIONPART_VER4;

struct OPTIONPART_VER5
{
    //-------------------------------------------------------------
    // 24Byte
    //-------------------------------------------------------------

    //16 byte
    union
    {
        struct
        {// �Ⱓ
            UINT64 RankOption1     : 7;
            UINT64 RankOption2     : 7;
            UINT64 RankOption3     : 7;
            UINT64 RankOption4     : 7;
            UINT64 RankOption5     : 7;
            UINT64 RankOption6     : 7;
            UINT64 RankOption7     : 7;
            UINT64 RankOption8     : 7;
            UINT64 RankOption9     : 7; //63
            UINT64 Rank            : 4; //4
            UINT64 bExpiredTime    : 1; // �ð� ���� ���� 
            UINT64 bProgressTime   : 1;
            UINT64 bExpiredWaitTime: 1;
            UINT64 NotUsed         : 6;
            UINT64 DateTime        : 45; //58 : ������ [20051107246060:10��], max[35184372088832:35��], ������ 10�ð�25��35��
            UINT64 Enchant         : 4; //62
            UINT64 SocketNum       : 2; //64
        };

        struct
        {// ��ǥ
            UINT64 MapCode         : 16;
            UINT64 X               : 16; //32
            UINT64 Y               : 16;
            UINT64 Z               : 16; //64
        };

        struct 
        {//����Ƽ��
            UINT64 LimitedIndex    : 16; //16    //����Ƽ�� �ε���
            UINT64 DuraMMax        : 9; //25    //�ִ� ������(�ִ� ������ ������) 
            UINT64 DuraMax         : 9; //34    //�ִ� ������(������ ����)
            UINT64 RepairCount     : 4; //38    //���� Ƚ��
            UINT64 DamageRatio     : 9; //47    //���ݷ� ����(���� ����) 
            UINT64 LimitValueRatio : 9; //56    //���� ��ġ ����
            UINT64 L_Enchant       : 4; //60
            UINT64 L_SocketNum     : 2; //62
            UINT64 Reserve1        : 2; //64

            UINT64 BasicOption1    : 7; //7     //�⺻�ɼ� 1
            UINT64 BasicOptValue1  : 6; //13    //�⺻�ɼ� 1 ��
            UINT64 BasicOption2    : 7; //20    //�⺻�ɼ� 2
            UINT64 BasicOptValue2  : 6; //26    //�⺻�ɼ� 2 ��
            UINT64 BasicOption3    : 7; //33    //�⺻�ɼ� 3
            UINT64 BasicOptValue3  : 6; //39    //�⺻�ɼ� 3 ��
            UINT64 Reserve2        : 25; //64    //�⺻�ɼ� 3 ��
        };

        struct
        {
            UINT64 FateInfoIndex       : 16; //16
            UINT64 F_DateTime          : 45; //61 : ������ [20051107246060:10��], max[35184372088832:35��], ������ 10�ð�25��35��            
            UINT64 F_bExpiredTime      : 1; //62 :  �ð� ���� ����
            UINT64 F_Identify          : 1; //63
            UINT64 F_bExpiredWaitTime  : 1; //64

            UINT64 F_AtkDefRatio       : 9; //9
            UINT64 F_DuraMax           : 9; //18
            UINT64 F_Enchant           : 4; //22
            UINT64 F_SocketNum         : 2; //24
            UINT64 F_Period            : 14; //38
            UINT64 F_Reserve3          : 26; //64
        };

        LottoStruct<UINT64> Lotto;

        struct
        {// ��ǥ����Ʈ ��ũ�� ������ ��ɵ��� Ȱ��, ��Ȱ�� �ٰŸ� ������.
            UINT64 WP_bExpiredTime     : 1; // �ð� ���� ���� 
            UINT64 WP_bProgressTime    : 1;
            UINT64 WP_DateTime         : 45; // 47 
            UINT64 WP_Reserve4         : 17; // 64

            UINT64 WP_bDelExpiredTime  : 1; // ���� ���� �ð� ���� ���� 
            UINT64 WP_DelDateTime      : 45; // 46
            UINT64 WP_Reserve5         : 18; // 64
        };
    };

    // 4byte
    UINT NOption         : 3;
    UINT Block           : 1; //4 
    UINT SocketOption1   : 8; //12
    UINT SocketOption2   : 8; //20
    UINT SocketOption3   : 8; //28

    UINT Set             : 2; //30
    UINT UserBlock       : 1; //31
    UINT IsLimited       : 1; //32    // ����Ƽ�� �������ΰ�?  

    // 4byte
    UINT bIsEquipEtherDevice : 1; //1
    UINT EtherBulletIndex    : 4; //5
    UINT PetName1            : 8; //13
    UINT PetName2            : 8; //21
    UINT bIdentifyPossession : 1;
    UINT bDestroy            : 1;
    UINT Reserved            : 9; //32
};

//_NA_20090925_ITEMSTREAM_INIT_RESERVED
typedef OPTIONPART_VER5  OPTIONPART_VER6;

//==================================================================================================
//
struct ItemParseType
{
    enum Value 
    {
        kEmpty = 0, // �� ����

        kDefault = 5, // �Ϲ� �۵�
        kDefaultTime = 6, // �Ϲ� �۵�(�ð� ���� �ִ� �͵�)
        //_NA_0_20100727_SOKET_SYSTEM_CONVERT
        kExtendData = 7, // �Ϲ� ���� ������ ���� ������ �ʿ��� �͵�(zard)
        kEquip = 10, // ���� ������ (Weapon | Armor)
        kEquipTime = 11, // �ð� ���� �����ϴ� ���� ������ (Weapon | Armor)
        kEquipEtcs = 16, // ���� ������ (Accessory | Special Accessory)
        kEquipEtcsTime = 17, // �ð� ���� �����ϴ� ���� ������ (Accessory | Special Accessory)
        kLimited = 20, // ����Ƽ�� ������ (��)
        kFate = 22, // ����Ʈ (Lucky) ������
        kCoord = 30, // ��ǥ ���� ��� ������ �ɼ�
        kWayPoint = 31, //
        kPet = 40, //
        //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
        kRider = 42, // Ż �� ������
        kRiderParts = 43, // Ż �� ���� ������
        kLottery = 45, //
        
        kCanNotParse = 254, // �ӽ� ��Ʈ���� ������ ���� (�ڵ尡 �������ų� ���... ���� ����)
        kCanNotParseOld = 255, // �ӽ� ��Ʈ���� ������ ���� (��ȯ ����, ���� ����)
        kUpperbound = 256, // ���� ������ ���� ū ��ġ + 1 (Max:255)
    };
    typedef uint8_t ValueType;
};

//==================================================================================================
//
struct OPTIONPART_VER7
{
#ifdef _NA_003740_20111122_RANDOM_ITEM
    enum { kBufferSize = 33 };
#else
    enum { kBufferSize = 23 };
#endif // _NA_003740_20111122_RANDOM_ITEM
    
    //-------------------------------------------------------------
    // 24Byte - Ver5�� ũ�� ����, Ÿ�Կ� ���� ��ġ �޸��ϴ� ����
    //-------------------------------------------------------------
    ItemParseType::ValueType ParseType; // 1 Byte
    BYTE Variable[kBufferSize];

    BOOST_STATIC_ASSERT(sizeof(ItemParseType::ValueType) == 1);
};
BOOST_STATIC_ASSERT(sizeof(OPTIONPART_VER7) == (OPTIONPART_VER7::kBufferSize + 1));
#ifndef _NA_003740_20111122_RANDOM_ITEM
BOOST_STATIC_ASSERT(sizeof(OPTIONPART_VER7) == sizeof(OPTIONPART_VER6));
#endif // _NA_003740_20111122_RANDOM_ITEM

//==================================================================================================
// _NA000109_090906_SOLAR_SLOT_UPDATE_VER8_
/*
struct OPTIONPART_VERx
{
    //-------------------------------------------------------------
    // 26Byte - an extended version based on version 7
    //-------------------------------------------------------------
    // referenced by the SerializeItemOption
    ItemParseType ParseType; // 1B
    uint8_t Variable[23 + 2]; // option block for the version 8
    BOOST_STATIC_ASSERT(sizeof(SLOTCODE) == 2 && sizeof(ItemParseType) == 1);
};
BOOST_STATIC_ASSERT(sizeof(OPTIONPART_VER8) == 26 &&
                    sizeof(OPTIONPART_VER8) == sizeof(OPTIONPART_VER7) + 2);
*/
//==================================================================================================
typedef OPTIONPART_VER7  OPTIONPART_VER8; //_NA002237_100802_EP2_SOCKET_SYSTEM_DB_
typedef OPTIONPART_VER8 OptionPartCurVer;
//==================================================================================================

//========================================== VERSION ===============================================

CREATE_ITEMSTREAMEX_VERSION(1);
CREATE_ITEMSTREAMEX_VERSION(2);
CREATE_ITEMSTREAMEX_VERSION(3);
CREATE_ITEMSTREAMEX_VERSION(4);
CREATE_ITEMSTREAMEX_VERSION(5); 
CREATE_ITEMSTREAMEX_VERSION(6); //_NA_20090925_ITEMSTREAM_INIT_RESERVED
CREATE_ITEMSTREAMEX_VERSION(7); //__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
CREATE_ITEMSTREAMEX_VERSION(8); //_NA002237_100802_EP2_SOCKET_SYSTEM_DB_
//CREATE_ITEMSTREAMEX_VERSION(x); //_NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_

//#if defined(_NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_)
//    TYPEDEF_ITEMSTREAMEX_VERSION(8);
#if   defined(_NA002237_100802_EP2_SOCKET_SYSTEM_DB_)
    TYPEDEF_ITEMSTREAMEX_VERSION(8);
#else
    TYPEDEF_ITEMSTREAMEX_VERSION(7); //__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
#endif

#pragma pack(pop)

#endif //_SOLAR_SLOT_SCSLOTSTRUCT_VERSION_H
