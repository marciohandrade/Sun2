#error "No more use this header file"

#ifndef _SOLAR_SLOT_SCSLOTSTRUCT_VERSION_H
#define _SOLAR_SLOT_SCSLOTSTRUCT_VERSION_H

#pragma once

#pragma pack(push,1)

template<typename _type = BYTE>
struct minislot_struct 
{
    _type   Random          :   1; // 랜덤이냐?
    _type   Exist           :   1; // 존재하냐?
    _type   Position        :   4; // 존재한다면 Script의 파트 인덱스이다. 현재는 (1~8)
    _type   Reserve         :   2;
};

template<typename _type = UINT64>
struct LottoStruct 
{
    _type   Index           :  16; // LotteryScript Index
    _type   Identity        :   8; // Identity State
    _type   Count           :   8; // Item Count
    _type   Reserve         :  32;
    minislot_struct<BYTE> minislot[MAX_PANDORA_SLOT_NUM];
};

//-------------------------------------------------------------------------
// 아이템파츠의 최소 단위
// 아이템 정보 표시 기본 단위
//-------------------------------------------------------------------------
struct ITEMPART_VER1
{
    BYTE byType;
    DWORD dwSerial;
    WORD wCode;
    BYTE byDura;
    inline BOOL IsValid() { return (wCode != 0); }
};

// 8 + 2 + 1 = 11Byte
typedef struct ITEMPART_VER2
{
    union
    {   
        struct 
        {
            DWORD64 dwSerial    : 40;
            DWORD64 byType      : 8; 
            DWORD64 byReserved2 : 8;
            DWORD64 byReserved1 : 8;
        };
        DWORD64 m_WorldSerial;
    };

    WORD wCode;
    BYTE byDura;
    inline BOOL IsValid() { return (wCode != 0); }

}ITEMPART_VER2;

typedef struct ITEMPART_VER3
{
    union
    {   
        struct 
        {
            DWORD64 dwSerial    : 40;
            DWORD64 byType      : 8; 
            DWORD64 byReserved2 : 8;
            DWORD64 byReserved1 : 8;
        };
        DWORD64 m_WorldSerial;
    };

    WORD wCode;
    BYTE byDura;
    inline BOOL IsValid() { return (wCode != 0); }
}ITEMPART, ITEMPART_VER3;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////// OPTIONPAR //////////////////////////////////////////////////

// Version 1 : 11byte
struct OPTIONPART_VER1
{
    //--------------------------------------------------------------
    // 11Byte
    //-------------------------------------------------------------
    // 8byte == 64bit
    union
    {
        struct
        {// 일반
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
        {// 기간
            UINT64 bExpiredTime    : 1; // 시간 만료 여부
            UINT64 bProgressTime   : 1;
            UINT64 NotUsed         : 11; //13
            UINT64 DateTime        : 45; //58 : 정액제 [20051107246060:10조], max[35184372088832:35조], 정량제 10시간25분35초
            UINT64 Enchant         : 4; //62
            UINT64 SocketNum       : 2; //64
        };
        struct
        {// 좌표
            UINT64 MapCode         : 16;
            UINT64 X               : 16; //32
            UINT64 Y               : 16; //48
            UINT64 Z               : 16; //64
            //UINT64 Enchant         : 4; //62
            //UINT64 SocketNum       : 2; //64
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
        {// 일반
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
        {// 기간
            UINT64 bExpiredTime    : 1; // 시간 만료 여부 
            UINT64 bProgressTime   : 1;
            UINT64 bExpiredWaitTime: 1;
            UINT64 NotUsed         : 10;
            UINT64 DateTime        : 45; //58 : 정액제 [20051107246060:10조], max[35184372088832:35조], 정량제 10시간25분35초
            UINT64 Enchant         : 4; //62
            UINT64 SocketNum       : 2; //64
        };
        struct
        {// 좌표
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

typedef struct OPTIONPART_VER3      //< PC_ROOM_EXCLUSIVE_CONTENTS 변경
{
    //-------------------------------------------------------------
    // 20Byte
    //-------------------------------------------------------------

    //16 byte
    union
    {
        struct
        {// 기간
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
            UINT64 bExpiredTime    : 1; // 시간 만료 여부 
            UINT64 bProgressTime   : 1;
            UINT64 bExpiredWaitTime: 1;
            UINT64 NotUsed         : 6;
            UINT64 DateTime        : 45; //58 : 정액제 [20051107246060:10조], max[35184372088832:35조], 정량제 10시간25분35초
            UINT64 Enchant         : 4; //62
            UINT64 SocketNum       : 2; //64

        };
        struct
        {// 좌표
            UINT64 MapCode         : 16;
            UINT64 X               : 16; //32
            UINT64 Y               : 16;
            UINT64 Z               : 16; //64
        };
        struct 
        {//리미티드
            UINT64 LimitedIndex    : 16; //16    //리미티드 인덱스
            UINT64 DuraMMax        : 9; //25    //최대 내구도(최대 내구도 복구용) 
            UINT64 DuraMax         : 9; //34    //최대 내구도(수리시 깍임)
            UINT64 RepairCount     : 4; //38    //수리 횟수
            UINT64 DamageRatio     : 9; //47    //공격력 비율(방어구는 방어력) 
            UINT64 LimitValueRatio : 9; //56    //제한 수치 비율
            UINT64 L_Enchant       : 4; //60
            UINT64 L_SocketNum     : 2; //62
            UINT64 Reserve1        : 2; //64

            UINT64 BasicOption1    : 7; //7     //기본옵션 1
            UINT64 BasicOptValue1  : 6; //13    //기본옵션 1 값
            UINT64 BasicOption2    : 7; //20    //기본옵션 2
            UINT64 BasicOptValue2  : 6; //26    //기본옵션 2 값
            UINT64 BasicOption3    : 7; //33    //기본옵션 3
            UINT64 BasicOptValue3  : 6; //39    //기본옵션 3 값
            UINT64 Reserve2        : 25; //64    //기본옵션 3 값
        };
        struct
        {
            UINT64 FateInfoIndex       : 16; //16
            UINT64 F_DateTime          : 45; //61 : 정액제 [20051107246060:10조], max[35184372088832:35조], 정량제 10시간25분35초            
            UINT64 F_bExpiredTime      : 1; //62 :  시간 만료 여부
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
    UINT IsLimited       : 1; //32    // 리미티드 아이템인가?  
#ifdef __NA_000968_ETHER_EMISSION_DEVICE
}OPTIONPART_VER3; //< PC_ROOM_EXCLUSIVE_CONTENTS 변경
#else
}OPTIONPART, OPTIONPART_VER3; //< PC_ROOM_EXCLUSIVE_CONTENTS 변경
#endif //__NA_000968_ETHER_EMISSION_DEVICE

#ifdef __NA_000968_ETHER_EMISSION_DEVICE
typedef struct OPTIONPART
{
    //-------------------------------------------------------------
    // 24Byte
    //-------------------------------------------------------------

    //16 byte
    union
    {
        struct
        {// 기간
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
            UINT64 bExpiredTime    : 1; // 시간 만료 여부 
            UINT64 bProgressTime   : 1;
            UINT64 bExpiredWaitTime: 1;
            UINT64 NotUsed         : 6;
            UINT64 DateTime        : 45; //58 : 정액제 [20051107246060:10조], max[35184372088832:35조], 정량제 10시간25분35초
            UINT64 Enchant         : 4; //62
            UINT64 SocketNum       : 2; //64

        };
        struct
        {// 좌표
            UINT64 MapCode         : 16;
            UINT64 X               : 16; //32
            UINT64 Y               : 16;
            UINT64 Z               : 16; //64
        };
        struct 
        {//리미티드
            UINT64 LimitedIndex    : 16; //16    //리미티드 인덱스
            UINT64 DuraMMax        : 9; //25    //최대 내구도(최대 내구도 복구용) 
            UINT64 DuraMax         : 9; //34    //최대 내구도(수리시 깍임)
            UINT64 RepairCount     : 4; //38    //수리 횟수
            UINT64 DamageRatio     : 9; //47    //공격력 비율(방어구는 방어력) 
            UINT64 LimitValueRatio : 9; //56    //제한 수치 비율
            UINT64 L_Enchant       : 4; //60
            UINT64 L_SocketNum     : 2; //62
            UINT64 Reserve1        : 2; //64

            UINT64 BasicOption1    : 7; //7     //기본옵션 1
            UINT64 BasicOptValue1  : 6; //13    //기본옵션 1 값
            UINT64 BasicOption2    : 7; //20    //기본옵션 2
            UINT64 BasicOptValue2  : 6; //26    //기본옵션 2 값
            UINT64 BasicOption3    : 7; //33    //기본옵션 3
            UINT64 BasicOptValue3  : 6; //39    //기본옵션 3 값
            UINT64 Reserve2        : 25; //64    //기본옵션 3 값
        };
        struct
        {
            UINT64 FateInfoIndex       : 16; //16
            UINT64 F_DateTime          : 45; //61 : 정액제 [20051107246060:10조], max[35184372088832:35조], 정량제 10시간25분35초            
            UINT64 F_bExpiredTime      : 1; //62 :  시간 만료 여부
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
#ifdef __NA001164_20081024_CHAR_WAY_POINT_ITEM
        struct
        {// 좌표리스트 링크된 아이템 기능들의 활성, 비활성 근거를 가진다.
            UINT64 WP_bExpiredTime     : 1; // 시간 만료 여부 
            UINT64 WP_bProgressTime    : 1;
            UINT64 WP_DateTime         : 45; // 
            UINT64 WP_Reserve4         : 17; // 64

            UINT64 WP_bDelExpiredTime  : 1; // 삭제 제약 시간 만료 여부 
            UINT64 WP_DelDateTime      : 45; // 93
            UINT64 WP_Reserve5         : 18; // 64
        };
#endif //__NA001164_20081024_CHAR_WAY_POINT_ITEM
    };

    // 4byte
    UINT NOption         : 3;
    UINT Block           : 1; //4 
    UINT SocketOption1   : 8; //12
    UINT SocketOption2   : 8; //20
    UINT SocketOption3   : 8; //28

    UINT Set             : 2; //30
    UINT UserBlock       : 1; //31
    UINT IsLimited       : 1; //32    // 리미티드 아이템인가?  

    // 4byte
    UINT bIsEquipEtherDevice : 1; //1
    UINT EtherBulletIndex    : 4; //5
    UINT Reserved            : 27; //32
}OPTIONPART, OPTIONPART_VER4;
#endif //__NA_000968_ETHER_EMISSION_DEVICE

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef union {
    struct{
        ITEMPART Part;
        OPTIONPART OptionPart;
    };
    BYTE pStream[sizeof(ITEMPART)+sizeof(OPTIONPART)];
} ITEMSTREAMEX ;

#ifdef __NA_000968_ETHER_EMISSION_DEVICE
typedef union { 
    struct{
        ITEMPART_VER2       Part;
        OPTIONPART_VER4     OptionPart;
    };
    BYTE pStream[sizeof(ITEMPART_VER2)+sizeof(OPTIONPART_VER4)];
} ITEMSTREAMEX_VER4;
#endif

typedef union { 
    struct{
        ITEMPART_VER2       Part;
        OPTIONPART_VER3     OptionPart;
    };
    BYTE pStream[sizeof(ITEMPART_VER2)+sizeof(OPTIONPART_VER3)];
} ITEMSTREAMEX_VER3;

typedef union {
    struct{
        ITEMPART_VER1       Part;
        OPTIONPART_VER2     OptionPart;
    };
    BYTE pStream[sizeof(ITEMPART_VER1)+sizeof(OPTIONPART_VER2)];
} ITEMSTREAMEX_VER2 ;

typedef union {
    struct{
        ITEMPART_VER1       Part;
        OPTIONPART_VER1     OptionPart;
    };
    BYTE pStream[sizeof(ITEMPART_VER1)+sizeof(OPTIONPART_VER1)];
} ITEMSTREAMEX_VER1 ;

#pragma pack(pop)

#endif //_SOLAR_SLOT_SCSLOTSTRUCT_VERSION_H
