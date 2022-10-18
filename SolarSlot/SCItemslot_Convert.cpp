#include "StdAfx.h"
#include ".\scitemslot.h"
#include <ItemInfoParser.h>

//==================================================================================================

//#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_

// (f100803.4L) change the type of a result stream to ITEMSTREAMEX
BOOL 
SCItemSlot::ConvertStream(int IN from_version, BYTE* IN from_stream,
                          int& OUT result_to_version, ITEMSTREAMEX* OUT result_to_stream)
{
    const int kLatestVersion = ITEM_STREAM_SIZE_LAST_VERSION;
    const int kLeastBinderVersion = 7;
    BOOST_STATIC_ASSERT(kLeastBinderVersion <= ITEM_STREAM_SIZE_LAST_VERSION);

    if (from_version > kLatestVersion || from_version < 1) {
        return false;
    };
    if (from_version == kLatestVersion)
    {
        result_to_version = from_version;
        *result_to_stream = *reinterpret_cast<ITEMSTREAMEX*>(from_stream);
        return true;
    }
    // (f100803.3L) enable multi version routing control block
    ITEMSTREAMEX working_space[2];
    int working_space_selector = 0;
    BYTE* src_it = from_stream;
    BYTE* dest_it = reinterpret_cast<BYTE*>(&working_space[0]);
    //
    int next_version = from_version;
    if (next_version < kLeastBinderVersion)
    {
        for (next_version = from_version; next_version < kLatestVersion; )
        {
            if (next_version >= kLeastBinderVersion) {
                break;
            }
            ZeroMemory(dest_it, sizeof(working_space[0]));
            if (!_ConvertStream(kLatestVersion, next_version,
                                src_it, result_to_version, dest_it))
            {
                return false;
            }
            assert(result_to_version <= kLeastBinderVersion);
            src_it = dest_it;
            dest_it = reinterpret_cast<BYTE*>(&working_space[(++working_space_selector) & 1]);
            next_version = result_to_version;
        }
    };
    assert(next_version == kLeastBinderVersion);
    if (next_version != kLeastBinderVersion) {
        __debugbreak(); // assert
        return false;
    }
    // current state: the value of a str_it has
    // the value of a from_stream or the value of a last converted stream
    return _ConvertStreamWithBinder(next_version, reinterpret_cast<ITEMSTREAMEX*>(src_it),
                                    &result_to_version, result_to_stream);

    /*
    int k = 0;
    static ITEMSTREAMEX TempStream[2];
    BYTE *pFromStreamIter = from_stream;
    BYTE *pToStreamIter = (BYTE*)&TempStream[0];

    for(int NextVersion = from_version; NextVersion < kLatestVersion;)
    {
        if (!_ConvertStream(kLatestVersion, NextVersion, pFromStreamIter, result_to_version, pToStreamIter))
            return false;

        if (result_to_version == kLatestVersion)
        {
            memcpy(result_to_stream, pToStreamIter, sizeof(ITEMSTREAMEX));
            return true;
        }

        pFromStreamIter = pToStreamIter;
        pToStreamIter = (BYTE*)&TempStream[ (++k) % 2 ];

        NextVersion = result_to_version;
    }

    return true;
    */
}
/*
#else //if !defined(_NA002237_100802_EP2_SOCKET_SYSTEM_DB_)

//==================================================================================================
// int IN iFromVersion : ITEMSTREAMEX
// BYTE* IN pFromStream , 
// int& OUT iToVersion, 
// BYTE* OUT pToStream

// (f100803.4L) change the type of a result stream to ITEMSTREAMEX
BOOL 
SCItemSlot::ConvertStream(int IN iFromVersion, BYTE* IN pFromStream,
                          int& OUT iToVersion, ITEMSTREAMEX* OUT pToStream)
{
    const int iLastestVersion = ITEM_STREAM_SIZE_LAST_VERSION;
    if(iFromVersion > iLastestVersion || iFromVersion < 1) return FALSE;

    if(iFromVersion == iLastestVersion)
    {
        iToVersion = iFromVersion;
        memcpy(pToStream, pFromStream, sizeof(ITEMSTREAMEX));
        return TRUE;
    }

    int k = 0;
    static ITEMSTREAMEX TempStream[2];
    BYTE *pFromStreamIter = pFromStream;
    BYTE *pToStreamIter = (BYTE*)&TempStream[0];

    for(int NextVersion = iFromVersion; NextVersion < iLastestVersion;)
    {
        if(!_ConvertStream(iLastestVersion, NextVersion, pFromStreamIter, iToVersion, pToStreamIter))
            return FALSE;

        if(iToVersion == iLastestVersion)
        {
            memcpy(pToStream, pToStreamIter, sizeof(ITEMSTREAMEX));
            return TRUE;
        }

        pFromStreamIter = pToStreamIter;
        pToStreamIter = (BYTE*)&TempStream[ (++k) % 2 ];

        NextVersion = iToVersion;
    }

    return TRUE;
}
#endif //_NA002237_100802_EP2_SOCKET_SYSTEM_DB_
*/
BOOL
SCItemSlot::_ConvertStream(int iLastestVersion,
                           int IN iFromVersion, BYTE* IN pFromStream,
                           int& OUT iToVersion, BYTE* OUT pToStream)
{
    // Version 1 : 기본
    //  { ITEMPART_VER1,   OPTIONPART_VER1 }
    // Version 2 : 옵션 사이즈 변경 및 유료화 관련 아이템 처리
    //  { ITEMPART_VER1=2, OPTIONPART_VER2 }
    // Version 3 : (리미티드 아이템) & (유료화와 랭크 분리)
    //  { ITEMPART_VER3,   OPTIONPART_VER3 }
    // Version 4 : 스트림은 변경 없음
    //  { ITEMPART_VER3,   OPTIONPART_VER4=3 }
    // Version 5 : 에테르 탄환
    //  { ITEMPART_VER3,   OPTIONPART_VER5 }
    // Version 6 : 스트림은 변경 없음    reseved 관련영영초기화     스트림 버전5
    //  { ITEMPART_VER3,   OPTIONPART_VER6=5 } - clear event
    // Version 7 : OPTIONPART의 타입별 파싱 방법 구분 처리 (비트 쪼개는 방법 더 이상 사용하지 않음)
    //  { ITEMPART_VER3,   OPTIONPART_VER7 <- "OPTIONPART_VER5와 크기는 같음" }
    // Version 8 : reordering stream spec. to support EP2 socket system
    //  (WARNING) no more enter this point
    const int kLeastBinderVersion = 7;
    if (iFromVersion >= kLeastBinderVersion) {
        assert(!"can't enter this version");
        return false;
    };

    switch(iFromVersion)
    {
    case ITEM_STREAM_SIZE_LAST_VERSION: //< 최신버전
        {
            iToVersion = iLastestVersion;
            memcpy(pToStream, pFromStream, sizeof(ITEMSTREAMEX));
        }
        break; 
/*
#ifdef _NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_
    case x:
        {
            iToVersion = x;
            if (!VersionXStreamConvertToX(pFromStream, iToVersion, pToStream)) {
                return false;
            }
            return true;
        }
        break;
#endif
*/
    case 6:
        {
            iToVersion = 6;
            if(!Version6StreamConvertTo7(pFromStream, iToVersion, pToStream))
                return false;
        }
        break;
    case 5:
        {
            iToVersion = 5;
            if(!Version5StreamConvertTo6(pFromStream, iToVersion, pToStream))
                return FALSE;
        }
        break;
    case 4:
        {
            iToVersion = 4;
            if(!Version4StreamConvertTo5(pFromStream, iToVersion, pToStream))
                return FALSE;
        }
        break;
    case 3:
        {
            iToVersion = 3;
            if(!Version3StreamConvertTo4(pFromStream, iToVersion, pToStream))
                return FALSE;
            //if(!Version4StreamConvertTo5(pFromStream, iToVersion, pToStream))
            //  return FALSE;
        }break;
    case 2:
        {
            iToVersion = 2;
            if(!UnderVersion3StreamConvertTo3(iFromVersion, pFromStream, iToVersion, pToStream))
                return FALSE;
            //if(!Version3StreamConvertTo4(pFromStream, iToVersion, pToStream))
            //  return FALSE;
            //if(!Version4StreamConvertTo5(pFromStream, iToVersion, pToStream))
            //  return FALSE;
        }break;

    case 1:
        {
            iToVersion = 1;
            if(!UnderVersion3StreamConvertTo3(iFromVersion, pFromStream, iToVersion, pToStream))
                return FALSE;
            //if(!Version3StreamConvertTo4(pFromStream, iToVersion, pToStream))
            //  return FALSE;
            //if(!Version4StreamConvertTo5(pFromStream, iToVersion, pToStream))
            //  return FALSE;
        }break; 
    default:
        ASSERT(FALSE);
        return FALSE;
    }

    return TRUE;
}

BOOL
SCItemSlot::UnderVersion3StreamConvertTo3(int IN iFromVersion, BYTE* IN pFromStream,
                                          int& OUT iToVersion, BYTE* OUT pToStream)
{   
    switch(iFromVersion)
    {
    case 2:
        {
            ITEMSTREAMEX_VER2* pOldItemStream = reinterpret_cast<ITEMSTREAMEX_VER2 *>(pFromStream);
            ITEMSTREAMEX_VER3* pNewItemStream = reinterpret_cast<ITEMSTREAMEX_VER3 *>(pToStream);
            // (f100803.5L) skip a process related to an empty item code stream
            const SLOTCODE item_code = pOldItemStream->Part.wCode;
            // (f100806.2L) remove data regard as planner's intend
            // if can't find item in item scripts
            const BASE_ITEMINFO* item_info = (item_code == 0)
                                           ?    NULL
                                           :    ItemInfoParser::Instance()->GetItemInfo(item_code);
            if (item_info == NULL) {
                ZeroMemory(pNewItemStream, sizeof(*pNewItemStream));
                iToVersion = 3;
                return true;
            }
            //ITEMPART 부분 복사
            pNewItemStream->Part.dwSerial   = pOldItemStream->Part.dwSerial;
            pNewItemStream->Part.byType     = pOldItemStream->Part.byType;
            pNewItemStream->Part.wCode      = pOldItemStream->Part.wCode;
            pNewItemStream->Part.byDura     = pOldItemStream->Part.byDura;
            //memcpy(&pNewItemStream->Part, &pOldItemStream->Part, sizeof(ITEMPART));

            //OPT파트 초기화
            memset(&pNewItemStream->OptionPart, 0, sizeof(OPTIONPART_VER3));

            //아이템 코드 없으면 일단 일반 아이템으로 컨버팅 한다.
            BYTE TempSubType = item_info->m_ChargeSubType;

            //OPT파트 UNION
            switch(TempSubType)
            {
            case eCHARGE_SUB_GENERAL:               
                pNewItemStream->OptionPart.RankOption1  = pOldItemStream->OptionPart.RankOption1;
                pNewItemStream->OptionPart.RankOption2  = pOldItemStream->OptionPart.RankOption2;
                pNewItemStream->OptionPart.RankOption3  = pOldItemStream->OptionPart.RankOption3;
                pNewItemStream->OptionPart.RankOption4  = pOldItemStream->OptionPart.RankOption4;
                pNewItemStream->OptionPart.RankOption5  = pOldItemStream->OptionPart.RankOption5;
                pNewItemStream->OptionPart.RankOption6  = pOldItemStream->OptionPart.RankOption6;
                pNewItemStream->OptionPart.RankOption7  = pOldItemStream->OptionPart.RankOption7;
                pNewItemStream->OptionPart.RankOption8  = pOldItemStream->OptionPart.RankOption8;
                pNewItemStream->OptionPart.RankOption9  = pOldItemStream->OptionPart.RankOption9;
                pNewItemStream->OptionPart.Rank         = pOldItemStream->OptionPart.Rank;
                pNewItemStream->OptionPart.Enchant      = pOldItemStream->OptionPart.Enchant;
                pNewItemStream->OptionPart.SocketNum    = pOldItemStream->OptionPart.SocketNum;
                break;
            case eCHARGE_SUB_FIXED_AMOUNT:
            case eCHARGE_SUB_FIXED_AMOUNT_EX:
            case eCHARGE_SUB_FIXED_QUANTITY:
            case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
            case eCHARGE_SUB_FIXED_AMOUNT_EX2:
                pNewItemStream->OptionPart.bExpiredTime     = pOldItemStream->OptionPart.bExpiredTime;
                pNewItemStream->OptionPart.bProgressTime    = pOldItemStream->OptionPart.bProgressTime;
                pNewItemStream->OptionPart.bExpiredWaitTime = pOldItemStream->OptionPart.bExpiredWaitTime;
                pNewItemStream->OptionPart.DateTime = pOldItemStream->OptionPart.DateTime;
                pNewItemStream->OptionPart.Enchant  = pOldItemStream->OptionPart.Enchant;
                pNewItemStream->OptionPart.SocketNum    = pOldItemStream->OptionPart.SocketNum;
                break;
            case eCHARGE_SUB_COORDINATE:                
                pNewItemStream->OptionPart.MapCode  = pOldItemStream->OptionPart.MapCode;
                pNewItemStream->OptionPart.X        = pOldItemStream->OptionPart.X;
                pNewItemStream->OptionPart.Y        = pOldItemStream->OptionPart.Y;
                pNewItemStream->OptionPart.Z        = pOldItemStream->OptionPart.Z;
                break;
            default:
                ASSERT(FALSE); 
                break;
            }
            // OPT파트 공통
            pNewItemStream->OptionPart.NOption      = pOldItemStream->OptionPart.NOption;
            pNewItemStream->OptionPart.Block        = pOldItemStream->OptionPart.Block;
            pNewItemStream->OptionPart.Set          = pOldItemStream->OptionPart.Set;
            pNewItemStream->OptionPart.UserBlock    = pOldItemStream->OptionPart.UserBlock;
            pNewItemStream->OptionPart.IsLimited    = 0;

            if(pOldItemStream->OptionPart.SocketOption1 <= 63)
                pNewItemStream->OptionPart.SocketOption1 = pOldItemStream->OptionPart.SocketOption1;
            else
                pNewItemStream->OptionPart.SocketOption1 = pOldItemStream->OptionPart.SocketOption1 + 64;

            if(pOldItemStream->OptionPart.SocketOption2 <= 63)
                pNewItemStream->OptionPart.SocketOption2 = pOldItemStream->OptionPart.SocketOption2;
            else
                pNewItemStream->OptionPart.SocketOption2 = pOldItemStream->OptionPart.SocketOption2 + 64;

            if(pOldItemStream->OptionPart.SocketOption3 <= 63)
                pNewItemStream->OptionPart.SocketOption3 = pOldItemStream->OptionPart.SocketOption3;
            else
                pNewItemStream->OptionPart.SocketOption3 = pOldItemStream->OptionPart.SocketOption3 + 64;

        }break;
    case 1:
        {
            ITEMSTREAMEX_VER1* pOldItemStream = reinterpret_cast<ITEMSTREAMEX_VER1 *>(pFromStream);
            ITEMSTREAMEX_VER3* pNewItemStream = reinterpret_cast<ITEMSTREAMEX_VER3 *>(pToStream);
            // (f100803.5L) skip a process related to an empty item code stream
            const SLOTCODE item_code = pOldItemStream->Part.wCode;
            // (f100806.2L) remove data regard as planner's intend
            // if can't find item in item scripts
            const BASE_ITEMINFO* item_info = (item_code == 0)
                                           ?    NULL
                                           :    ItemInfoParser::Instance()->GetItemInfo(item_code);
            if (item_info == NULL) {
                ZeroMemory(pNewItemStream, sizeof(*pNewItemStream));
                iToVersion = 3;
                return true;
            }
            //ITEMPART 부분 복사
            pNewItemStream->Part.dwSerial   = pOldItemStream->Part.dwSerial;
            pNewItemStream->Part.byType     = pOldItemStream->Part.byType;
            pNewItemStream->Part.wCode      = pOldItemStream->Part.wCode;
            pNewItemStream->Part.byDura     = pOldItemStream->Part.byDura;
            //memcpy(&pNewItemStream->Part, &pOldItemStream->Part, sizeof(ITEMPART));

            //OPT파트 초기화
            memset(&pNewItemStream->OptionPart, 0, sizeof(OPTIONPART_VER3));

            //아이템 코드 없으면 일단 일반 아이템으로 컨버팅 한다.
            BYTE TempSubType = item_info->m_ChargeSubType;

            //OPT파트 UNION
            switch(TempSubType)
            {
            case eCHARGE_SUB_GENERAL:               
                pNewItemStream->OptionPart.RankOption1  = pOldItemStream->OptionPart.RankOption1;
                pNewItemStream->OptionPart.RankOption2  = pOldItemStream->OptionPart.RankOption2;
                pNewItemStream->OptionPart.RankOption3  = pOldItemStream->OptionPart.RankOption3;
                pNewItemStream->OptionPart.RankOption4  = pOldItemStream->OptionPart.RankOption4;
                pNewItemStream->OptionPart.RankOption5  = pOldItemStream->OptionPart.RankOption5;
                pNewItemStream->OptionPart.RankOption6  = pOldItemStream->OptionPart.RankOption6;
                pNewItemStream->OptionPart.RankOption7  = pOldItemStream->OptionPart.RankOption7;
                pNewItemStream->OptionPart.RankOption8  = pOldItemStream->OptionPart.RankOption8;
                pNewItemStream->OptionPart.RankOption9  = pOldItemStream->OptionPart.RankOption9;
                pNewItemStream->OptionPart.Rank         = pOldItemStream->OptionPart.Rank;
                pNewItemStream->OptionPart.Enchant      = pOldItemStream->OptionPart.Enchant;
                pNewItemStream->OptionPart.SocketNum    = pOldItemStream->OptionPart.SocketNum;
                break;
            case eCHARGE_SUB_FIXED_AMOUNT:
            case eCHARGE_SUB_FIXED_AMOUNT_EX:
            case eCHARGE_SUB_FIXED_QUANTITY:
            case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
            case eCHARGE_SUB_FIXED_AMOUNT_EX2:
                pNewItemStream->OptionPart.bExpiredTime     = pOldItemStream->OptionPart.bExpiredTime;
                pNewItemStream->OptionPart.bProgressTime    = pOldItemStream->OptionPart.bProgressTime;
                pNewItemStream->OptionPart.DateTime = pOldItemStream->OptionPart.DateTime;
                pNewItemStream->OptionPart.Enchant  = pOldItemStream->OptionPart.Enchant;
                pNewItemStream->OptionPart.SocketNum    = pOldItemStream->OptionPart.SocketNum;
                break;
            case eCHARGE_SUB_COORDINATE:                
                pNewItemStream->OptionPart.MapCode  = pOldItemStream->OptionPart.MapCode;
                pNewItemStream->OptionPart.X        = pOldItemStream->OptionPart.X;
                pNewItemStream->OptionPart.Y        = pOldItemStream->OptionPart.Y;
                pNewItemStream->OptionPart.Z        = pOldItemStream->OptionPart.Z;
                break;
            default:
                ASSERT(FALSE); 
                break;
            }
            // OPT파트 공통
            pNewItemStream->OptionPart.NOption      = pOldItemStream->OptionPart.NOption;
            pNewItemStream->OptionPart.Block        = pOldItemStream->OptionPart.Block;
            pNewItemStream->OptionPart.Set          = pOldItemStream->OptionPart.Set;
            pNewItemStream->OptionPart.UserBlock    = 0; //< 버전 2 에서 추가됨
            pNewItemStream->OptionPart.IsLimited    = 0;

            if(pOldItemStream->OptionPart.SocketOption1 <= 31)
                pNewItemStream->OptionPart.SocketOption1 = pOldItemStream->OptionPart.SocketOption1;
            else
                pNewItemStream->OptionPart.SocketOption1 = pOldItemStream->OptionPart.SocketOption1 + 96;

            if(pOldItemStream->OptionPart.SocketOption2 <= 31)
                pNewItemStream->OptionPart.SocketOption2 = pOldItemStream->OptionPart.SocketOption2;
            else
                pNewItemStream->OptionPart.SocketOption2 = pOldItemStream->OptionPart.SocketOption2 + 96;

            if(pOldItemStream->OptionPart.SocketOption3 <= 31)
                pNewItemStream->OptionPart.SocketOption3 = pOldItemStream->OptionPart.SocketOption3;
            else
                pNewItemStream->OptionPart.SocketOption3 = pOldItemStream->OptionPart.SocketOption3 + 96;

        }break;
    default:
        ASSERT(FALSE);
        return FALSE;
    }

    iToVersion = 3;

    return TRUE;
}

BOOL
SCItemSlot::Version3StreamConvertTo4(BYTE* IN pFromStream , int& OUT iToVersion, BYTE* OUT pToStream)
{
    if(3 != iToVersion)
        return FALSE;

    // 변경 사항이 없다. 그냥 버전 바꿔주자 ;;
    memcpy(pToStream, pFromStream, sizeof(ITEMSTREAMEX_VER4));

    iToVersion = 4;

    return TRUE;
}

BOOL
SCItemSlot::Version4StreamConvertTo5(BYTE* IN pFromStream , int& OUT iToVersion, BYTE* OUT pToStream)
{
    if(4 != iToVersion)
        return FALSE;

    // 에테르 추가됨 ;;
    ITEMSTREAMEX_VER4* pOldItemStream = reinterpret_cast<ITEMSTREAMEX_VER4 *>(pFromStream);
    ITEMSTREAMEX_VER5* pNewItemStream = reinterpret_cast<ITEMSTREAMEX_VER5 *>(pToStream);
    // (f100803.5L) skip a process related to an empty item code stream
    const SLOTCODE item_code = pOldItemStream->Part.wCode;
    // (f100806.2L) remove data regard as planner's intend
    // if can't find item in item scripts
    const BASE_ITEMINFO* item_info = (item_code == 0)
                                    ?    NULL
                                    :    ItemInfoParser::Instance()->GetItemInfo(item_code);
    if (item_info == NULL) {
        ZeroMemory(pNewItemStream, sizeof(*pNewItemStream));
        iToVersion = 5;
        return true;
    }

    //ITEMPART 부분 복사
    pNewItemStream->Part.dwSerial   = pOldItemStream->Part.dwSerial;
    pNewItemStream->Part.byType     = pOldItemStream->Part.byType;
    pNewItemStream->Part.wCode      = pOldItemStream->Part.wCode;
    pNewItemStream->Part.byDura     = pOldItemStream->Part.byDura;
    //memcpy(&pNewItemStream->Part, &pOldItemStream->Part, sizeof(ITEMPART));

    //OPT파트 초기화
    memset(&pNewItemStream->OptionPart, 0, sizeof(OPTIONPART_VER5));

    //OPT파트 UNION
    //리미티드  
    if(pOldItemStream->OptionPart.IsLimited)
    {
        pNewItemStream->OptionPart.LimitedIndex     = pOldItemStream->OptionPart.LimitedIndex;
        pNewItemStream->OptionPart.DuraMMax         = pOldItemStream->OptionPart.DuraMMax;
        pNewItemStream->OptionPart.DuraMax          = pOldItemStream->OptionPart.DuraMax;
        pNewItemStream->OptionPart.RepairCount      = pOldItemStream->OptionPart.RepairCount;
        pNewItemStream->OptionPart.DamageRatio      = pOldItemStream->OptionPart.DamageRatio;
        pNewItemStream->OptionPart.LimitValueRatio  = pOldItemStream->OptionPart.LimitValueRatio;
        pNewItemStream->OptionPart.L_Enchant        = pOldItemStream->OptionPart.L_Enchant;
        pNewItemStream->OptionPart.L_SocketNum      = pOldItemStream->OptionPart.L_SocketNum;
        pNewItemStream->OptionPart.Reserve1         = pOldItemStream->OptionPart.Reserve1;

        pNewItemStream->OptionPart.BasicOption1     = pOldItemStream->OptionPart.BasicOption1;
        pNewItemStream->OptionPart.BasicOptValue1   = pOldItemStream->OptionPart.BasicOptValue1;
        pNewItemStream->OptionPart.BasicOption2     = pOldItemStream->OptionPart.BasicOption2;
        pNewItemStream->OptionPart.BasicOptValue2   = pOldItemStream->OptionPart.BasicOptValue2;
        pNewItemStream->OptionPart.BasicOption3     = pOldItemStream->OptionPart.BasicOption3;
        pNewItemStream->OptionPart.BasicOptValue3   = pOldItemStream->OptionPart.BasicOptValue3;
        pNewItemStream->OptionPart.Reserve2         = pOldItemStream->OptionPart.Reserve2;
    }
    else
    {
        //아이템 코드 없으면 일단 일반 아이템으로 컨버팅 한다.
        BYTE TempSubType = item_info->m_ChargeSubType;

        switch(TempSubType)
        {
        case eCHARGE_SUB_GENERAL:
        case eCHARGE_PC_ROOM:
            pNewItemStream->OptionPart.RankOption1  = pOldItemStream->OptionPart.RankOption1;
            pNewItemStream->OptionPart.RankOption2  = pOldItemStream->OptionPart.RankOption2;
            pNewItemStream->OptionPart.RankOption3  = pOldItemStream->OptionPart.RankOption3;
            pNewItemStream->OptionPart.RankOption4  = pOldItemStream->OptionPart.RankOption4;
            pNewItemStream->OptionPart.RankOption5  = pOldItemStream->OptionPart.RankOption5;
            pNewItemStream->OptionPart.RankOption6  = pOldItemStream->OptionPart.RankOption6;
            pNewItemStream->OptionPart.RankOption7  = pOldItemStream->OptionPart.RankOption7;
            pNewItemStream->OptionPart.RankOption8  = pOldItemStream->OptionPart.RankOption8;
            pNewItemStream->OptionPart.RankOption9  = pOldItemStream->OptionPart.RankOption9;
            pNewItemStream->OptionPart.Rank         = pOldItemStream->OptionPart.Rank;
            pNewItemStream->OptionPart.Enchant      = pOldItemStream->OptionPart.Enchant;
            pNewItemStream->OptionPart.SocketNum    = pOldItemStream->OptionPart.SocketNum;
            break;
        case eCHARGE_SUB_FIXED_AMOUNT:
        case eCHARGE_SUB_FIXED_AMOUNT_EX:
        case eCHARGE_SUB_FIXED_QUANTITY:
        case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
        case eCHARGE_SUB_FIXED_AMOUNT_EX2:
            pNewItemStream->OptionPart.bExpiredTime     = pOldItemStream->OptionPart.bExpiredTime;
            pNewItemStream->OptionPart.bProgressTime    = pOldItemStream->OptionPart.bProgressTime;
            pNewItemStream->OptionPart.bExpiredWaitTime = pOldItemStream->OptionPart.bExpiredWaitTime;
            pNewItemStream->OptionPart.DateTime = pOldItemStream->OptionPart.DateTime;
            pNewItemStream->OptionPart.Enchant  = pOldItemStream->OptionPart.Enchant;
            pNewItemStream->OptionPart.SocketNum    = pOldItemStream->OptionPart.SocketNum;
            break;
        case eCHARGE_SUB_COORDINATE:                
            pNewItemStream->OptionPart.MapCode  = pOldItemStream->OptionPart.MapCode;
            pNewItemStream->OptionPart.X        = pOldItemStream->OptionPart.X;
            pNewItemStream->OptionPart.Y        = pOldItemStream->OptionPart.Y;
            pNewItemStream->OptionPart.Z        = pOldItemStream->OptionPart.Z;
            break;
        case eCHARGE_SUB_FATE:
            pNewItemStream->OptionPart.FateInfoIndex        = pOldItemStream->OptionPart.FateInfoIndex;
            pNewItemStream->OptionPart.F_DateTime           = pOldItemStream->OptionPart.F_DateTime;
            pNewItemStream->OptionPart.F_bExpiredTime       = pOldItemStream->OptionPart.F_bExpiredTime;
            pNewItemStream->OptionPart.F_Identify           = pOldItemStream->OptionPart.F_Identify;
            pNewItemStream->OptionPart.F_bExpiredWaitTime   = pOldItemStream->OptionPart.F_bExpiredWaitTime;

            pNewItemStream->OptionPart.F_AtkDefRatio        = pOldItemStream->OptionPart.F_AtkDefRatio;
            pNewItemStream->OptionPart.F_DuraMax            = pOldItemStream->OptionPart.F_DuraMax;
            pNewItemStream->OptionPart.F_Enchant            = pOldItemStream->OptionPart.F_Enchant;
            pNewItemStream->OptionPart.F_SocketNum          = pOldItemStream->OptionPart.F_SocketNum;
            pNewItemStream->OptionPart.F_Period             = pOldItemStream->OptionPart.F_Period;
            pNewItemStream->OptionPart.F_Reserve3           = pOldItemStream->OptionPart.F_Reserve3;
            break;
        default:
            ASSERT(FALSE); 
            break;
        }
    }

    // OPT파트 공통
    pNewItemStream->OptionPart.NOption      = pOldItemStream->OptionPart.NOption;
    pNewItemStream->OptionPart.Block        = pOldItemStream->OptionPart.Block;
    pNewItemStream->OptionPart.Set          = pOldItemStream->OptionPart.Set;
    pNewItemStream->OptionPart.UserBlock    = pOldItemStream->OptionPart.UserBlock;
    pNewItemStream->OptionPart.IsLimited    = pOldItemStream->OptionPart.IsLimited;

    pNewItemStream->OptionPart.SocketOption1 = pOldItemStream->OptionPart.SocketOption1;
    pNewItemStream->OptionPart.SocketOption2 = pOldItemStream->OptionPart.SocketOption2; 
    pNewItemStream->OptionPart.SocketOption3 = pOldItemStream->OptionPart.SocketOption3;

    //추가된 값
    pNewItemStream->OptionPart.bIsEquipEtherDevice  = 0;
    pNewItemStream->OptionPart.EtherBulletIndex     = 0;
    pNewItemStream->OptionPart.Reserved             = 0;

    iToVersion = 5;

    return TRUE;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

//{_NA_20090925_ITEMSTREAM_INIT_RESERVED
// 중국 귀속아이템 영역에 쓰래기 값이 들어 있는 문제로 해당 부분만 초기화

BOOL
SCItemSlot::Version5StreamConvertTo6(BYTE* IN pFromStream , int& OUT iToVersion, BYTE* OUT pToStream)
{
    if(5 != iToVersion)
        return FALSE;

    memcpy(pToStream, pFromStream, sizeof(ITEMSTREAMEX_VER5));

    ITEMSTREAMEX_VER6* pNewItemStream = reinterpret_cast<ITEMSTREAMEX_VER6 *>(pToStream);

    pNewItemStream->OptionPart.bIdentifyPossession = 0;
    pNewItemStream->OptionPart.bDestroy            = 0;
    pNewItemStream->OptionPart.Reserved            = 0;

    iToVersion = 6;

    return TRUE;
}
//}

//==================================================================================================
//==================================================================================================
//==================================================================================================

namespace nsSlot {
;

static void
Convert_From_V6_To_V7(nsSlot::SerializeItemOption::sLimited* OUT dest, const OPTIONPART_VER5& src,
                      const BASE_ITEMINFO* item_info, const ItemRuleInfo& type_info)
{
    __UNUSED((item_info, type_info));
    assert(type_info.is_armor);
    // Shared Option Part
    dest->ParseType = ItemParseType::kLimited;
    dest->Block = src.Block;
    dest->UserBlock = src.UserBlock;

    // Limited Option Part
    dest->IsIdentifyPossession = src.bIdentifyPossession;
    dest->IsDestroyed  = src.bDestroy;
    dest->NOption      = src.NOption;
    dest->EnchantGrade = src.L_Enchant;
    dest->IsDivine     = src.Set;

    dest->NumberOfSockets  = src.L_SocketNum;
    dest->SocketOption[SOCKET_1] = src.SocketOption1;
    dest->SocketOption[SOCKET_2] = src.SocketOption2;
    dest->SocketOption[SOCKET_3] = src.SocketOption3;

    dest->Index = src.LimitedIndex;
    dest->DurationForRestore = src.DuraMMax;
    dest->Duration         = src.DuraMax;
    dest->NumberOfRepairs  = src.RepairCount;
    dest->DefenceRatio     = src.DamageRatio;
    dest->LimitValueRatio  = src.LimitValueRatio;

    // (WAVERIX) (090912) (NOTE) 현재 사용중이 아닌 것 같다.
    // (CHANGES) (UNUSED SECTION) (WAVERIX) - 기획 확인
/*
    dest->LimitedOption[LIMITED_1].Option = src.BasicOption1;
    dest->LimitedOption[LIMITED_1].Value  = src.BasicOptValue1;
    dest->LimitedOption[LIMITED_2].Option = src.BasicOption2;
    dest->LimitedOption[LIMITED_2].Value  = src.BasicOptValue2;
    dest->LimitedOption[LIMITED_3].Option = src.BasicOption3;
    dest->LimitedOption[LIMITED_3].Value  = src.BasicOptValue3;
*/
}

static void
Convert_From_V6_To_V7(nsSlot::SerializeItemOption::sLottery* OUT dest, const OPTIONPART_VER5& src,
                      const BASE_ITEMINFO* item_info, const ItemRuleInfo& type_info)
{
    __UNUSED((item_info, type_info));
    assert(type_info.is_lottery);
    // Shared Option Part
    dest->ParseType = ItemParseType::kLottery;
    dest->Block = src.Block;
    dest->UserBlock = src.UserBlock;

    // Lottery Option Part
    dest->Index    = src.Lotto.Index;
    dest->Identity = src.Lotto.Identity;
    dest->Count    = src.Lotto.Count;
    for(int i = 0; i < MAX_PANDORA_SLOT_NUM; ++i) {
        const minislot_struct<BYTE>& minislot = src.Lotto.minislot[i];
        dest->Slots[i].Random   = minislot.Random;
        dest->Slots[i].Exist    = minislot.Exist;
        dest->Slots[i].Position = minislot.Position;
    }
}

static void
Convert_From_V6_To_V7(nsSlot::SerializeItemOption::sPet* OUT dest, const OPTIONPART_VER5& src,
                      const BASE_ITEMINFO* item_info, const ItemRuleInfo& type_info)
{
    __UNUSED((item_info, type_info));
    // Shared Option Part
    dest->ParseType = ItemParseType::kPet;
    dest->Block = src.Block;
    dest->UserBlock = src.UserBlock;

    // Pet Option Part
    dest->Fullness = src.SocketOption1;
    dest->Intimacy = src.SocketOption2;
    if(type_info.charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY) {
        dest->IsExpired = src.bExpiredTime;
        dest->DateTime = src.DateTime;
    }

    BYTE exist_name = src.Enchant;
    dest->IsSetPetName = !!exist_name;
    if(exist_name) {
        CopyMemory(dest->Name, &src, 8 /*bytes*/);
        dest->Name[8] = src.PetName1;
        dest->Name[9] = src.PetName2;
    }
}

static void
Convert_From_V6_To_V7(nsSlot::SerializeItemOption::sWayPoint* OUT dest, const OPTIONPART_VER5& src,
                      const BASE_ITEMINFO* item_info, const ItemRuleInfo& type_info)
{
    __UNUSED((item_info, type_info));
    // Shared Option Part
    dest->ParseType = ItemParseType::kWayPoint;
    dest->Block = src.Block;
    dest->UserBlock = src.UserBlock;

    // Waypoint Option Part
    dest->IsExpired = src.WP_bExpiredTime;
    dest->IsProgress = src.WP_bProgressTime;
    dest->DateTime = src.WP_DateTime;

    //
    dest->WayPointDeleteEnabled = src.WP_bDelExpiredTime;
    dest->WayPointDeleteDateTime = src.WP_DelDateTime;
}

static void
Convert_From_V6_To_V7(nsSlot::SerializeItemOption::sCoord* OUT dest, const OPTIONPART_VER5& src,
                      const BASE_ITEMINFO* item_info, const ItemRuleInfo& type_info)
{
    __UNUSED((item_info, type_info));
    // Shared Option Part
    dest->ParseType = ItemParseType::kCoord;
    dest->Block = src.Block;
    dest->UserBlock = src.UserBlock;

    // Coordinate Option Part
    dest->MapCode = src.MapCode;
    dest->X = src.X;
    dest->Y = src.Y;
    dest->Z = src.Z;
}

static void
Convert_From_V6_To_V7(nsSlot::SerializeItemOption::sEquip* OUT dest, const OPTIONPART_VER5& src,
                      const BASE_ITEMINFO* item_info, const ItemRuleInfo& type_info)
{
    __UNUSED((item_info, type_info));
    assert(type_info.is_weapon || type_info.is_armor);

    // Shared Option Part
    dest->ParseType = ItemParseType::kEquip;
    dest->Block = src.Block;
    dest->UserBlock = src.UserBlock;

    // Equipment Option Part
    dest->IsIdentifyPossession = src.bIdentifyPossession;
    dest->IsDestroyed = src.bDestroy;

    dest->NOption = src.NOption;
    dest->Rank = src.Rank;
    dest->RankOption[RANK_D ] = src.RankOption1;
    dest->RankOption[RANK_C ] = src.RankOption2;
    dest->RankOption[RANK_B ] = src.RankOption3;
    dest->RankOption[RANK_MA] = src.RankOption4;
    dest->RankOption[RANK_A ] = src.RankOption5;
    dest->RankOption[RANK_PA] = src.RankOption6;
    dest->RankOption[RANK_MS] = src.RankOption7;
    dest->RankOption[RANK_S ] = src.RankOption8;
    dest->RankOption[RANK_PS] = src.RankOption9;
    dest->EnchantGrade = src.Enchant;
    dest->IsDivine = src.Set;

    //----
    dest->NumberOfSockets = src.SocketNum;
    dest->SocketOption[SOCKET_1] = src.SocketOption1;
    dest->SocketOption[SOCKET_2] = src.SocketOption2;
    dest->SocketOption[SOCKET_3] = src.SocketOption3;

    if(type_info.is_weapon) {
        dest->IsEquipEtherDevice = src.bIsEquipEtherDevice;
        dest->EtherBulletIndex = src.EtherBulletIndex;
    }
}

static void
Convert_From_V6_To_V7(nsSlot::SerializeItemOption::sEquipEtc* OUT dest, const OPTIONPART_VER5& src,
                      const BASE_ITEMINFO* item_info, const ItemRuleInfo& type_info)
{
    __UNUSED((item_info, type_info));
    assert(type_info.is_accessory || type_info.is_accessory_special);

    // Shared Option Part
    dest->ParseType = ItemParseType::kEquipEtcs;
    dest->Block = src.Block;
    dest->UserBlock = src.UserBlock;

    dest->IsIdentifyPossession = src.bIdentifyPossession;
    dest->IsDestroyed = src.bDestroy;

    dest->NOption = src.NOption;
    dest->Rank = src.Rank;
    dest->RankOption[RANK_D ] = src.RankOption1;
    dest->RankOption[RANK_C ] = src.RankOption2;
    dest->RankOption[RANK_B ] = src.RankOption3;
    dest->RankOption[RANK_MA] = src.RankOption4;
    dest->RankOption[RANK_A ] = src.RankOption5;
    dest->RankOption[RANK_PA] = src.RankOption6;
    dest->RankOption[RANK_MS] = src.RankOption7;
    dest->RankOption[RANK_S ] = src.RankOption8;
    dest->RankOption[RANK_PS] = src.RankOption9;
    //dest->EnchantGrade = src.Enchant;
    //dest->IsDivine = src.Set; // (NOTE) 확인 사항: divine없다.
}

static void
Convert_From_V6_To_V7(nsSlot::SerializeItemOption::sEquipTime* OUT dest, const OPTIONPART_VER5& src,
                      const BASE_ITEMINFO* item_info, const ItemRuleInfo& type_info)
{
    __UNUSED((item_info));
    assert(type_info.is_weapon || type_info.is_armor);

    // Shared Option Part
    dest->ParseType = ItemParseType::kEquipTime;
    dest->Block = src.Block;
    dest->UserBlock = src.UserBlock;

    // Equipment Option Part
    dest->IsIdentifyPossession = src.bIdentifyPossession;
    dest->IsDestroyed = src.bDestroy;

    dest->NOption = src.NOption;

    dest->EnchantGrade = src.Enchant;
    dest->IsDivine = src.Set;

    //----
    dest->NumberOfSockets = src.SocketNum;
    dest->SocketOption[SOCKET_1] = src.SocketOption1;
    dest->SocketOption[SOCKET_2] = src.SocketOption2;
    dest->SocketOption[SOCKET_3] = src.SocketOption3;

    if(type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_WAIT) {
        dest->Rank = src.Rank;
        dest->RankOption[RANK_D ] = src.RankOption1;
        dest->RankOption[RANK_C ] = src.RankOption2;
        dest->RankOption[RANK_B ] = src.RankOption3;
        dest->RankOption[RANK_MA] = src.RankOption4;
        dest->RankOption[RANK_A ] = src.RankOption5;
        dest->RankOption[RANK_PA] = src.RankOption6;
        dest->RankOption[RANK_MS] = src.RankOption7;
        dest->RankOption[RANK_S ] = src.RankOption8;
        dest->RankOption[RANK_PS] = src.RankOption9;
    }

    if(type_info.is_weapon) {
        dest->IsEquipEtherDevice = src.bIsEquipEtherDevice;
        dest->EtherBulletIndex = src.EtherBulletIndex;
    }

    //--- time -------------------------------------------------------------------------------------
    dest->IsExpired = src.bExpiredTime;
    dest->DateTime = src.DateTime;
    if(type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX) {
        dest->IsProgressTime = src.bProgressTime;

        //----------------------------------------------------------------------
        // 주의!!!!!!임시 
        // 유료화 유효기간 오류 때문에 유효 만료시키기 위한 꽁수-_-;
        // 아이템샵 오픈 날짜(20061219000000)이후의 값을 가지고 있고, m_bProgressTime==false인 아이템을 진행중으로 바꿈
        if(dest->IsExpired == false &&
            dest->IsProgressTime == false &&
            dest->DateTime > 20061219000000ll)
        {
            dest->IsProgressTime = true;
        }
    }
    if(type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_WAIT) {
        dest->IsExpiredWaitTime = src.bExpiredWaitTime;
    }
}

static void
Convert_From_V6_To_V7(nsSlot::SerializeItemOption::sEquipEtcTime* OUT dest, const OPTIONPART_VER5& src,
                      const BASE_ITEMINFO* item_info, const ItemRuleInfo& type_info)
{
    __UNUSED((item_info));
    assert(type_info.is_accessory || type_info.is_accessory_special);

    // Shared Option Part
    dest->ParseType = ItemParseType::kEquipEtcsTime;
    dest->Block = src.Block;
    dest->UserBlock = src.UserBlock;

    dest->IsIdentifyPossession = src.bIdentifyPossession;
    dest->IsDestroyed = src.bDestroy;

    dest->NOption = src.NOption;
    dest->Rank = src.Rank;
    dest->RankOption[RANK_D ] = src.RankOption1;
    dest->RankOption[RANK_C ] = src.RankOption2;
    dest->RankOption[RANK_B ] = src.RankOption3;
    dest->RankOption[RANK_MA] = src.RankOption4;
    dest->RankOption[RANK_A ] = src.RankOption5;
    dest->RankOption[RANK_PA] = src.RankOption6;
    dest->RankOption[RANK_MS] = src.RankOption7;
    dest->RankOption[RANK_S ] = src.RankOption8;
    dest->RankOption[RANK_PS] = src.RankOption9;
    //dest->EnchantGrade = src.Enchant;
    //dest->IsDivine = src.Set; // (NOTE) 확인 사항: divine없다.

    //--- time -------------------------------------------------------------------------------------
    dest->IsExpired = src.bExpiredTime;
    dest->DateTime = src.DateTime;
    if(type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX) {
        dest->IsProgressTime = src.bProgressTime;

        //----------------------------------------------------------------------
        // 주의!!!!!!임시 
        // 유료화 유효기간 오류 때문에 유효 만료시키기 위한 꽁수-_-;
        // 아이템샵 오픈 날짜(20061219000000)이후의 값을 가지고 있고, m_bProgressTime==false인 아이템을 진행중으로 바꿈
        if(dest->IsExpired == false &&
            dest->IsProgressTime == false &&
            dest->DateTime > 20061219000000ll)
        {
            dest->IsProgressTime = true;
        }
    }
    if(type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_WAIT) {
        dest->IsExpiredWaitTime = src.bExpiredWaitTime;
    }
}

static void
Convert_From_V6_To_V7(nsSlot::SerializeItemOption::sTime* OUT dest, const OPTIONPART_VER5& src,
                      const BASE_ITEMINFO* item_info, const ItemRuleInfo& type_info)
{
    __UNUSED(item_info);
    // Shared Option Part
    dest->ParseType = ItemParseType::kDefaultTime;
    dest->Block = src.Block;
    dest->UserBlock = src.UserBlock;

    dest->IsExpired = src.bExpiredTime;
    dest->DateTime = src.DateTime;
    if(type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX) {
        dest->IsProgressTime = src.bProgressTime;

        //----------------------------------------------------------------------
        // 주의!!!!!!임시 
        // 유료화 유효기간 오류 때문에 유효 만료시키기 위한 꽁수-_-;
        // 아이템샵 오픈 날짜(20061219000000)이후의 값을 가지고 있고, m_bProgressTime==false인 아이템을 진행중으로 바꿈
        if(dest->IsExpired == false &&
            dest->IsProgressTime == false &&
            dest->DateTime > 20061219000000ll)
        {
            dest->IsProgressTime = true;
        }
    }
    if(type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_WAIT) {
        dest->IsExpiredWaitTime = src.bExpiredWaitTime;
    }
}

static void
Convert_From_V6_To_V7(nsSlot::SerializeItemOption::sFate* OUT dest, const OPTIONPART_VER5& src,
                      const BASE_ITEMINFO* item_info, const ItemRuleInfo& type_info)
{
    __UNUSED((item_info));
    assert(type_info.is_weapon || type_info.is_armor);

    // Shared Option Part
    dest->ParseType = ItemParseType::kFate;
    dest->Block = src.Block;
    dest->UserBlock = src.UserBlock;

    // Fate(Lucky) Equipment Option Part
    dest->IsIdentifyPossession = src.bIdentifyPossession;
    dest->IsDestroyed = src.bDestroy;

    dest->NOption = src.NOption;
    dest->EnchantGrade = src.F_Enchant;
    //dest->IsDivine = src.Set;

    dest->Index = src.FateInfoIndex; //(unused) ?
    dest->AtkDefRatio   = src.F_AtkDefRatio;
    dest->Duration      = src.F_DuraMax;
    dest->IsIdentify    = src.F_Identify;
    dest->Period        = src.F_Period;
    //----
    dest->NumberOfSockets = src.F_SocketNum;
    dest->SocketOption[SOCKET_1] = src.SocketOption1;
    dest->SocketOption[SOCKET_2] = src.SocketOption2;
    dest->SocketOption[SOCKET_3] = src.SocketOption3;

    if(type_info.is_weapon) {
        dest->IsEquipEtherDevice = src.bIsEquipEtherDevice;
        dest->EtherBulletIndex = src.EtherBulletIndex;
    }

    dest->IsExpired         = src.F_bExpiredTime;
    dest->IsExpiredWaitTime = src.F_bExpiredWaitTime;
    dest->DateTime          = src.F_DateTime;
}

static void
Convert_From_V6_To_V7_FailBackup(nsSlot::SerializeItemOption::sBackupStream* OUT dest,
                                 const OPTIONPART_VER5& src)
{
    dest->ParseType = ItemParseType::kCanNotParseOld;
    CopyMemory(dest->stream, &src, sizeof(dest->stream));
}

}; //end of namespace

BOOL
SCItemSlot::Version6StreamConvertTo7(BYTE* IN from_stream , int& OUT to_version, BYTE* OUT to_stream)
{
    if (6 != to_version) {
        return false;
    };
    const int kConvertedVersion = 7;
    ITEMSTREAMEX_VER6* item_stream_old = reinterpret_cast<ITEMSTREAMEX_VER6 *>(from_stream);
    ITEMSTREAMEX_VER7* item_stream_new = reinterpret_cast<ITEMSTREAMEX_VER7 *>(to_stream);
    ZeroMemory(item_stream_new, sizeof(*item_stream_new));

    //SCItemSlot work_slot;
    //work_slot.CopyPart(item_stream_old->Part);
    //work_slot.CopyPart(item_stream_old->OptionPart);

    //----------------------------------------------------------------------------------------------
    // ITEMPART
    const WORD item_code = item_stream_old->Part.wCode;
    // (f100803.5L) skip a process related to an empty item code stream
    if (item_code == 0) {
        ZeroMemory(item_stream_new, sizeof(*item_stream_new));
        to_version = kConvertedVersion;
        return true;
    }
    item_stream_new->Part.dwSerial   = item_stream_old->Part.dwSerial;
    item_stream_new->Part.byType     = item_stream_old->Part.byType;
    item_stream_new->Part.wCode      = item_code;
    item_stream_new->Part.byDura     = item_stream_old->Part.byDura;

    //----------------------------------------------------------------------------------------------
    // OPTIONPART
    // VER6 -> VER7 의 처리는 기존과 크게 다르다.
    const OPTIONPART_VER6& option_part_old = item_stream_old->OptionPart;
    ItemInfoParser* const pItemInfoParser = ItemInfoParser::Instance();

    const BASE_ITEMINFO* item_info = 0;
    if(item_code != 0)
        item_info = pItemInfoParser->GetItemInfo(item_code);

    BOOLEAN is_valid = (item_info != 0);

    nsSlot::SerializeItemOption& option_part_new = *SCItemSlot::_GetBindingBlock();
    ZeroMemory(&option_part_new, sizeof(option_part_new));

    if(is_valid == 0 && item_code != 0) {
        //SUNLOG(eCRITICAL_LOG, "Invalid ItemCode %d", item_code);
        nsSlot::Convert_From_V6_To_V7_FailBackup(&option_part_new.BackupStream, option_part_old);

        // 통과 시키고, 다음 턴을 노린다.
        to_version = kConvertedVersion;
        is_valid = _SerializeToItemStream(&item_stream_new->OptionPart, kConvertedVersion);
        return is_valid;
    }

    if(item_code == 0) {
        option_part_new.Default.ParseType = ItemParseType::kEmpty;

        to_version = kConvertedVersion;
        is_valid = _SerializeToItemStream(&item_stream_new->OptionPart, kConvertedVersion);
        return is_valid;
    }

    assert(is_valid);

    nsSlot::ItemRuleInfo type_info;
    ZeroMemory(&type_info, sizeof(type_info));
    if(option_part_new.Default.ParseType != ItemParseType::kCanNotParseOld) {
        bool is_limited = option_part_old.IsLimited != false;
        type_info.GetRuleInfo(&type_info, item_info, is_limited);
    }

    if(is_valid == 0)
    {
        nsSlot::Convert_From_V6_To_V7_FailBackup(&option_part_new.BackupStream, option_part_old);

        // 통과 시키고, 다음 턴을 노린다.
        to_version = kConvertedVersion;
        is_valid = _SerializeToItemStream(&item_stream_new->OptionPart, kConvertedVersion);
        return is_valid;
    }
    else if(type_info.is_limited)
    {
        if(type_info.is_armor) {
            nsSlot::Convert_From_V6_To_V7(&option_part_new.Limited, option_part_old,
                item_info, type_info);
        }
        else {
            ASSERT(!"Unsupported Option: Limited is support a armor only");
            nsSlot::Convert_From_V6_To_V7_FailBackup(&option_part_new.BackupStream, option_part_old);

            // 통과 시키고, 다음 턴을 노린다.
            to_version = kConvertedVersion;
            is_valid = _SerializeToItemStream(&item_stream_new->OptionPart, kConvertedVersion);
            return is_valid;
        }
    }
    else if(type_info.is_lottery)
    {
        nsSlot::Convert_From_V6_To_V7(&option_part_new.Lottery, option_part_old, item_info, type_info);
    }
    else if(type_info.is_pet)
    {
        nsSlot::Convert_From_V6_To_V7(&option_part_new.Pet, option_part_old, item_info, type_info);
    }
    else if(type_info.is_waypoint)
    {
        nsSlot::Convert_From_V6_To_V7(&option_part_new.WayPoint, option_part_old, item_info, type_info);
    }
    else if(type_info.charge_sub_type == eCHARGE_SUB_COORDINATE)
    {
        nsSlot::Convert_From_V6_To_V7(&option_part_new.Coord, option_part_old, item_info, type_info);
    }
    else if(type_info.charge_sub_type == eCHARGE_SUB_GENERAL ||
            type_info.charge_sub_type == eCHARGE_PC_ROOM)
    {
        if(type_info.is_weapon || type_info.is_armor) {
            // Equipment Option Part
            nsSlot::Convert_From_V6_To_V7(&option_part_new.Equip, option_part_old, item_info, type_info);
        }
        else if(type_info.is_accessory || type_info.is_accessory_special) {
            nsSlot::Convert_From_V6_To_V7(&option_part_new.EquipEtc, option_part_old, item_info, type_info);
        }
        else {
            //-- 일반 아이템들...
            //-- 특수 아이템들... 스크립트에 흔히 속성이 가미되는 경우, 즉, 고정값이기에
            //스크립트 제어가 가능한 것들은 여기까지만으로도 충분하다.
            option_part_new.Default.ParseType = ItemParseType::kDefault;
            option_part_new.Default.Block = option_part_old.Block;
            option_part_new.Default.UserBlock = option_part_old.UserBlock;
        }
    }
    else if(type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT ||
            type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX ||
            type_info.charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY ||
            type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_WAIT ||
            type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX2)
    {
        if(type_info.is_weapon || type_info.is_armor) {
            // Equipment Option Part
            nsSlot::Convert_From_V6_To_V7(&option_part_new.EquipTime, option_part_old,
                item_info, type_info);
        }
        else if(type_info.is_accessory || type_info.is_accessory_special) {
            nsSlot::Convert_From_V6_To_V7(&option_part_new.EquipEtcTime, option_part_old,
                item_info, type_info);
        }
        else {
            //-- 일반 아이템들...
            //-- 특수 아이템들... 스크립트에 흔히 속성이 가미되는 경우, 즉, 고정값이기에
            //스크립트 제어가 가능한 것들은 여기까지만으로도 충분하다.
            // + 시간 제한 설정은 필요하다.
            nsSlot::Convert_From_V6_To_V7(&option_part_new.Time, option_part_old,
                item_info, type_info);
        }
    }
    else if(type_info.charge_sub_type == eCHARGE_SUB_FATE)
    {
        if(type_info.is_weapon || type_info.is_armor) {
            // Equipment Option Part
            nsSlot::Convert_From_V6_To_V7(&option_part_new.Fate, option_part_old,
                item_info, type_info);
        }
        else {
            ASSERT(!"Unsupported Option: in a LuckyItem");
            to_version = 7;
            is_valid = _SerializeToItemStream(&item_stream_new->OptionPart, kConvertedVersion);
            return is_valid;
        }
    }
    else
    {
    #ifdef _DEBUG
        // { rider, rider parts } temporary setup
        if(item_info->IsRiderItem()) {
            option_part_new.Default.ParseType = ItemParseType::kRider;
            option_part_new.Default.Block = option_part_old.Block;
            option_part_new.Default.UserBlock = option_part_old.UserBlock;
            to_version = kConvertedVersion;
            is_valid = _SerializeToItemStream(&item_stream_new->OptionPart, kConvertedVersion);
            return is_valid;
        }
        if(item_info->IsRiderPartsItem()) {
            option_part_new.Default.ParseType = ItemParseType::kRiderParts;
            option_part_new.Default.Block = option_part_old.Block;
            option_part_new.Default.UserBlock = option_part_old.UserBlock;
            to_version = kConvertedVersion;
            is_valid = _SerializeToItemStream(&item_stream_new->OptionPart, kConvertedVersion);
            return is_valid;
        }
    #endif //_DEBUG temporary setup
        ASSERT(!"Unsupported Option: unkown format by info+data");
        nsSlot::Convert_From_V6_To_V7_FailBackup(&option_part_new.BackupStream, option_part_old);

        // 통과 시키고, 다음 턴을 노린다.
        to_version = kConvertedVersion;
        is_valid = _SerializeToItemStream(&item_stream_new->OptionPart, kConvertedVersion);
        return is_valid;
    }

    to_version = kConvertedVersion;
    is_valid = _SerializeToItemStream(&item_stream_new->OptionPart, kConvertedVersion);
    return is_valid;
}

/*
#ifdef _NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_
BOOL
SCItemSlot::Version7StreamConvertTo8(BYTE* IN from_stream, int& OUT to_version, BYTE* OUT to_stream)
{
    if (7 != to_version) {
        return false;
    }
    ITEMSTREAMEX_VER7* item_stream_old = reinterpret_cast<ITEMSTREAMEX_VER7 *>(from_stream);
    ITEMSTREAMEX_VER8* item_stream_new = reinterpret_cast<ITEMSTREAMEX_VER8 *>(to_stream);
    ZeroMemory(item_stream_new, sizeof(*item_stream_new));

    //----------------------------------------------------------------------------------------------
    // ITEMPART (VER3 <- VER3)
    // (NOTE) the ITEMPART of the ver7 is cleared of an unused section
    // OPTIONPART
    // (NOTE) default items, except the etheria item, are equal to the version 7 specification
    // without 2bytes padding section
    //       11B      26B = { 24B + 2B }
    //  ITEMPART_VER3|OPTIONPART_VER7+2B| - default item
    //  ITEMPART_VER3|OPTIONPART_VER8| - the etheria item
    // (NOTE) current version not supported to the etheria item,
    // however change to support mismatch problem
    // so that make to the empty etheria item
    BOOST_STATIC_ASSERT(sizeof(*item_stream_old) + 2 == sizeof(*item_stream_new));
    //             11B+26B          11B+24B               11B+24B
    CopyMemory(item_stream_new, item_stream_old, sizeof(*item_stream_old));

    to_version = 8;
    return true;
}
#endif
*/
