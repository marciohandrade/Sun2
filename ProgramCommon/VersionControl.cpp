#include "stdafx.h"
#include "VersionControl.h"
#include "Define.h"
#include <SCSlotStruct.h>
#include <SCItemSlot.h>
#include <ItemInfoParser.h>

#include "QuestDefine.h"

namespace nsSlot {
;

// (NOTE) (f100713.1L) this function has rules for the version content.
// the inputed item stream should be the latest version of an item specification
static bool ContainerItemConvertByContent(BYTE used_slot_num, ITEMSTREAMEX* to_stream_array INOUT)
{
    ItemInfoParser* const item_parser = ItemInfoParser::Instance();
    ITEMSTREAMEX* it = to_stream_array;
    ITEMSTREAMEX* const end = to_stream_array + used_slot_num;
    for (; it != end; ++it)
    {
        const SLOTCODE item_code = it->Part.wCode;
        // (CHANGES) (f100802.2L) remove data regard as planner's intend
        // if can't find item in item scripts
        const BASE_ITEMINFO* item_info = (item_code == 0)
                                       ?    NULL
                                       :    item_parser->GetItemInfo(item_code);
        if (item_info == NULL) {
            ZeroMemory(it, sizeof(*it));
            continue;
        }
#if defined(_GLOBAL) || defined(_RUSSIA)
        switch (item_code)
        { // convert the item code to solve too hard script versioning in the Global version
        //case 47005: // mega-mistone for event
        case 47060: // mega-mistone
            it->Part.wCode = 4204; // common mega-mistone
            break;
        }
#endif
    }

    return true;
}

// (CHANGES) (f100806.1L) change to a no-template version
static bool
ContainerItemConvert(SLOTIDX slot_index, const POSTYPE used_slot_num,
                     const int from_version IN, BYTE* const from_stream_array IN,
                     const int from_stream_node_size, //sizeof(ITEMSTREAMEX_VER1,2,...)
                     int& to_version OUT, ITEMSTREAMEX* const to_stream_array OUT)
{
    const int kLatestVersion = ITEM_STREAM_SIZE_LAST_VERSION;
    static const int kNumberOfMaxSlotsArray[SI_MAX] =
    {
        0, //SI_TEMPINVENTORY = 0,
        MAX_INVENTORY_SLOT_NUM, //SI_INVENTORY,
        MAX_EQUIPMENT_SLOT_NUM, //SI_EQUIPMENT,
        MAX_WAREHOUSE_SLOT_NUM, //SI_WAREHOUSE,
        0, //SI_SKILL, not item stream
        0, //SI_QUICK, not item stream
        0, //SI_STYLE, not item stream
        MAX_GUILD_WAREHOUSE_SLOT_NUM, //SI_GUILDWAREHOUSE,
    };
    BOOST_STATIC_ASSERT(MAX_INVENTORY_TAB_TOTAL_NUM * MAX_INVENTORY_SLOT_PER_TAB ==
                        MAX_INVENTORY_SLOT_NUM);
    //
    if (slot_index >= _countof(kNumberOfMaxSlotsArray)) {
        ASSERT(!"Unexpected status: slot index out of ranges");
        return false;
    }
    //
    const int number_of_max_slots = kNumberOfMaxSlotsArray[slot_index];
    if (number_of_max_slots == 0) {
        ASSERT(!"Unexpected status: a stream type of a slot index is not an item stream");
        return false;
    }
    if (number_of_max_slots < used_slot_num) {
        assert(number_of_max_slots >= used_slot_num);
        __debugbreak();
        return false;
    }
    //                          ITEMSTREAMEX
    const int used_size = sizeof(*to_stream_array) * used_slot_num;
    /*
    int number_of_max_slots = 0;
    int used_size = used_slot_num * sizeof(ITEMSTREAMEX);

    switch (slot_index)
    {
    case SI_INVENTORY:
        number_of_max_slots = MAX_INVENTORY_TAB_TOTAL_NUM * MAX_INVENTORY_SLOT_PER_TAB;
        break;
    case SI_EQUIPMENT:
        number_of_max_slots = MAX_EQUIPMENT_SLOT_NUM;
        break;
    case SI_TEMPINVENTORY:
        number_of_max_slots = MAX_TEMPINVENTORY_SLOT_NUM;
        break;
    case SI_WAREHOUSE:
        number_of_max_slots = MAX_WAREHOUSE_SLOT_NUM;
        break;
    case SI_GUILDWAREHOUSE:
        number_of_max_slots = MAX_GUILD_WAREHOUSE_SLOT_NUM;
        break;
    default:
        ASSERT(false);
        return false;
        break;
    }
    */
    // 사용중인 슬롯의 버전변환
    if (from_version == kLatestVersion)
    {
        CopyMemory(to_stream_array, from_stream_array, used_size);
    }
    else
    {
        // (CHANGES) (f100806.1L)
        BYTE* from_offset = from_stream_array;
        for (int i = 0; i < used_slot_num; ++i, (from_offset += from_stream_node_size))
        {
            SCItemSlot::ConvertStream(from_version, from_offset, to_version, &to_stream_array[i]);
            //SCItemSlot::ConvertStream(from_version, (BYTE*)&from_stream_array[i],
            //                          to_version, &to_stream_array[i]);
            if (to_version != kLatestVersion)
            {
                to_version = -1;
                return false;
            }
        }
    }
    // (CHANGES) (f100805.1L) clear an unused range
    ;{
        size_t clear_size = static_cast<size_t>(\
            reinterpret_cast<char*>(&to_stream_array[number_of_max_slots]) -
            reinterpret_cast<char*>(&to_stream_array[used_slot_num]));
        if (clear_size) {
            ZeroMemory(&to_stream_array[used_slot_num], clear_size);
        }
    #if SUN_CODE_BACKUP
        // 사용하지 않는 슬롯(캐쉬슬롯)의 초기화
        int init_size = (number_of_max_slots - used_slot_num) * sizeof(ITEMSTREAMEX);
        ZeroMemory((((BYTE*)to_stream_array) + used_size), init_size);
    #endif
    };
    ;{
        // (CHANGES) (f100805.5L) change process order
        // (NOTE) (f100713.1L) this function has rules for the version content.
        nsSlot::ContainerItemConvertByContent(used_slot_num, to_stream_array);
    };
    // last operation
    to_version = kLatestVersion;
    //
    return true;
}

static int GetFromVersionUsedSlotNum(SLOTIDX slot_index, int from_version, int used_slot_num)
{
    int used_slot_num_from_version = used_slot_num;

    if (slot_index != SI_EQUIPMENT) {
        return used_slot_num_from_version;
    }
    switch (from_version)
    {
    case ITEM_STREAM_SIZE_LAST_VERSION:
        used_slot_num_from_version = MAX_EQUIPMENT_SLOT_NUM;
        break;
//#ifdef _NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_
//    case x:  used_slot_num_from_version = MAX_EQUIPMENT_SLOT_NUM_Vx; break;
//#endif
#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    case 7:  used_slot_num_from_version = MAX_EQUIPMENT_SLOT_NUM_V7; break; // 1004
#endif
    case 6:  used_slot_num_from_version = MAX_EQUIPMENT_SLOT_NUM_V6; break; // 905
    case 5:  used_slot_num_from_version = MAX_EQUIPMENT_SLOT_NUM_V5; break; // 904
    case 4:  used_slot_num_from_version = MAX_EQUIPMENT_SLOT_NUM_V4; break;
    case 3:  used_slot_num_from_version = MAX_EQUIPMENT_SLOT_NUM_V3; break;
    case 2:  used_slot_num_from_version = MAX_EQUIPMENT_SLOT_NUM_V2; break;
    case 1:  used_slot_num_from_version = MAX_EQUIPMENT_SLOT_NUM_V1; break;
    }

    return used_slot_num_from_version;
}

// (CHANGES) change a ContainerItemConvert to a no-template version
static const uint8_t kItemStreamSizeArray[ITEM_STREAM_SIZE_LAST_VERSION + 1] =
{
    0,  // version 0 - not used, unexpected status
    sizeof(ITEMSTREAMEX_VER1),
    sizeof(ITEMSTREAMEX_VER2),
    sizeof(ITEMSTREAMEX_VER3),
    sizeof(ITEMSTREAMEX_VER4),
    sizeof(ITEMSTREAMEX_VER5),
    sizeof(ITEMSTREAMEX_VER6),
#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    sizeof(ITEMSTREAMEX_VER7),
#endif
    //
    sizeof(ITEMSTREAMEX) // end of record
};

}; //end of namespace 'nsSlot'

//==================================================================================================
//==================================================================================================
//==================================================================================================

bool VersionControl::ConvertItem(int* converted_version, void* converted_stream,
                                 const int from_version, const void* from_stream)
{
    if (from_version == 0 || from_version >= _countof(nsSlot::kItemStreamSizeArray)) {
        ASSERT(!"Unexpected version");
        return false;
    };

    const int kLatestVersion = ITEM_STREAM_SIZE_LAST_VERSION;
    BOOST_STATIC_ASSERT(kLatestVersion + 1 == _countof(nsSlot::kItemStreamSizeArray));
    ITEMSTREAMEX* result_stream = reinterpret_cast<ITEMSTREAMEX*>(converted_stream);
    if (from_version == kLatestVersion)
    {
        *converted_version = from_version;
        CopyMemory(result_stream, from_stream, sizeof(*result_stream));
        return true;
    }
    else
    {
        int result_version = 0;
        uint8_t* from_stream_it = reinterpret_cast<uint8_t*>(const_cast<void*>(from_stream));
        SCItemSlot::ConvertStream(from_version, from_stream_it, result_version, result_stream);
        if (result_version != kLatestVersion)
        {
            *converted_version = -1;
            return false;
        }
        //
        *converted_version = result_version;
    }
    // (CHANGES) (f100805.5L) change process order
    // (NOTE) (f100713.1L) this function has rules for the version content.
    nsSlot::ContainerItemConvertByContent(1, result_stream);
    //assert 'result_stream == converted_stream'
    //
    return true;
}


BOOL VersionControl::ItemConverter(SLOTIDX slot_index, int used_slot_num IN,
                                   int from_version IN, BYTE* from_stream IN,
                                   int& to_version OUT, BYTE* to_stream OUT)
{
    // NOTE: latest_version == ITEM_STREAM_SIZE_LAST_VERSION
    if (used_slot_num < 0) {
        return false;
    }
    //
    const int used_slot_num_from_version = \
        nsSlot::GetFromVersionUsedSlotNum(slot_index, from_version, used_slot_num);
    //
    if (from_version == 0 || from_version >= _countof(nsSlot::kItemStreamSizeArray)) {
        ASSERT(!"Unexpected version");
        return false;
    };
    //
    const int from_stream_node_size = nsSlot::kItemStreamSizeArray[from_version];
    bool success = nsSlot::ContainerItemConvert(\
        slot_index, used_slot_num_from_version,
        from_version, from_stream, from_stream_node_size,
        to_version, reinterpret_cast<ITEMSTREAMEX*>(to_stream));
    //
    return success;

/*
    switch (from_version)
    {
    case ITEM_STREAM_SIZE_LAST_VERSION:
        if (!ContainerItemConvert(slot_index, used_slot_num_from_version,
                                  from_version, (ITEMSTREAMEX*)from_stream,
                                  to_version, (ITEMSTREAMEX*)to_stream))
        {
            return false;
        }
        break;
//#ifdef _NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_
//    case x:
//        if (!ContainerItemConvert(slot_index, used_slot_num_from_version,
//                                  from_version, reinterpret_cast<ITEMSTREAMEX_VER7*>(from_stream),
//                                  to_version, reinterpret_cast<ITEMSTREAMEXx*>(to_stream)))
//        {
//            return false;
//        }
//        break;
//#endif
#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    case 7:
        if (!ContainerItemConvert(slot_index, used_slot_num_from_version,
                                  from_version, (ITEMSTREAMEX_VER7*)from_stream,
                                  to_version, (ITEMSTREAMEX*)to_stream))
        {
            return false;
        }
        break;
#endif
    // convert to v7 from v6
    case 6:
        if (!ContainerItemConvert(slot_index, used_slot_num_from_version,
                                  from_version, (ITEMSTREAMEX_VER6*)from_stream,
                                  to_version, (ITEMSTREAMEX*)to_stream))
        {
            return false;
        }
        break;
    case 5:
        if (!ContainerItemConvert(slot_index, used_slot_num_from_version,
                                  from_version, (ITEMSTREAMEX_VER5*)from_stream,
                                  to_version, (ITEMSTREAMEX*)to_stream))
        {
            return false;
        }
        break;
    case 4:
        if (!ContainerItemConvert(slot_index, used_slot_num_from_version,
                                  from_version, (ITEMSTREAMEX_VER4*)from_stream,
                                  to_version, (ITEMSTREAMEX*)to_stream))
        {
            return false;
        }
        break;
    case 3:
        if (!ContainerItemConvert(slot_index, used_slot_num_from_version,
                                  from_version, (ITEMSTREAMEX_VER3*)from_stream,
                                  to_version, (ITEMSTREAMEX*)to_stream))
        {
            return false;
        }
        break;
    case 2:
        if (!ContainerItemConvert(slot_index, used_slot_num_from_version,
                                  from_version, (ITEMSTREAMEX_VER2*)from_stream,
                                  to_version, (ITEMSTREAMEX*)to_stream))
        {
            return false;
        }
        break;
    case 1:
        if (!ContainerItemConvert(slot_index, used_slot_num_from_version,
                                  from_version, (ITEMSTREAMEX_VER1*)from_stream,
                                  to_version, (ITEMSTREAMEX*)to_stream))
        {
            return false;
        }
        break;
    default:
        ASSERT(false);
        return false;
    }
    return true;
*/
}


//{__NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM

BOOL VersionControl::QuestConverter( OUT int& iDstVersion, OUT BYTE* pDstStreamProgr, OUT BYTE* pDstStreamCompl,
									const int iSrcVersion, const BYTE* pSrcStreamProgr, const BYTE* pSrcStreamCompl )
{
	switch( iSrcVersion )
	{
    //////////////////////////////////////////////////////////////////////////
    // 가장 최신 버전. (현재:2)
    case QUESTPART_PROGR::QUESTPART_PROGR_VERSION:
        memcpy( pDstStreamProgr, pSrcStreamProgr, MAX_PROGR_QUESTSTREAM_SIZE );
        memcpy( pDstStreamCompl, pSrcStreamCompl, QUESTPART_COMPL::MAX_QUEST_COMPL_NUM );

		iDstVersion = QUESTPART_PROGR::QUESTPART_PROGR_VERSION;

		return TRUE;

    //////////////////////////////////////////////////////////////////////////
    //
    case QUESTPART_PROGR_V1::QUESTPART_PROGR_VERSION:
        if( !_QuestProgrConvert( (QUESTPART_PROGR_V2*)pDstStreamProgr, (QUESTPART_PROGR_V1*)pSrcStreamProgr) )
            return FALSE;

        if( !_QuestComplConvert( (QUESTPART_COMPL_V2*)pDstStreamCompl, (QUESTPART_COMPL_V1*)pSrcStreamCompl) )
            return FALSE;

        iDstVersion = QUESTPART_PROGR_V2::QUESTPART_PROGR_VERSION;

        break;

    //////////////////////////////////////////////////////////////////////////
    //
	case QUESTPART_PROGR_V0::QUESTPART_PROGR_VERSION:
		if( !_QuestProgrConvert( (QUESTPART_PROGR_V1*)pDstStreamProgr, (QUESTPART_PROGR_V0*)pSrcStreamProgr) )
			return FALSE;
		if( !_QuestComplConvert( (QUESTPART_COMPL_V1*)pDstStreamCompl, (QUESTPART_COMPL_V0*)pSrcStreamCompl) )
			return FALSE;

		iDstVersion = QUESTPART_PROGR_V1::QUESTPART_PROGR_VERSION;
        
		break;
    //////////////////////////////////////////////////////////////////////////
    //
	default:
		ASSERT(FALSE);
		return FALSE;
	}

    //////////////////////////////////////////////////////////////////////////
    //
	//오랜만에 들어온 유저의 경우 한단계씩 버전업 한다.
	if( QUESTPART_PROGR::QUESTPART_PROGR_VERSION != iDstVersion )
	{
		static BYTE _SrcStreamProgr[MAX_PROGR_QUESTSTREAM_SIZE] = {0,};
		static BYTE _SrcStreamCompl[MAX_COMPL_QUESTSTREAM_SIZE] = {0,};

		memcpy( _SrcStreamProgr, pDstStreamProgr, MAX_PROGR_QUESTSTREAM_SIZE );
		memcpy( _SrcStreamCompl, pDstStreamCompl, MAX_COMPL_QUESTSTREAM_SIZE );

		QuestConverter( iDstVersion, pDstStreamProgr, pDstStreamCompl,
				iDstVersion, _SrcStreamProgr, _SrcStreamCompl);
	}
	return TRUE;
}

// DB에 직접적인 변화는 없으나, 구조체 변화에 따라 버전 변경한다.
// 완료 스트림도 버전값을 같이 사용함으로 버전 변경한다. 
BOOL VersionControl::_QuestProgrConvert( OUT QUESTPART_PROGR_V2* pDstStreamProgr, const QUESTPART_PROGR_V1* pSrcStreamProgr ) 
{
    for( int i = 0; i < QUESTPART_PROGR_V1::MAX_QUEST_PROGR_NUM; ++i)
        pDstStreamProgr[i] = pSrcStreamProgr[i];

    return TRUE;
}

BOOL VersionControl::_QuestComplConvert( OUT QUESTPART_COMPL_V2* pDstStreamCompl, const QUESTPART_COMPL_V1* pSrcStreamCompl ) 
{
    memcpy( pDstStreamCompl, pSrcStreamCompl, QUESTPART_COMPL_V1::MAX_QUEST_COMPL_NUM );

    return TRUE;
}


// 사용되지 않는 퀘스트 대역 초기화
// 1번과 2번의 경우 기존 DB에서 케릭생성시 추가되는 퀘스트이나 현재 사용하지 않으므로 초기화 한다.
// 이후 필요시 여기에서 초기화 하지 않는 대역의 퀘스트를 사용하여야한다.(1500번 이후가 안전)
BOOL VersionControl::_QuestProgrConvert( OUT QUESTPART_PROGR_V1* pDstStreamProgr, const QUESTPART_PROGR_V0* pSrcStreamProgr ) 
{
	for( int i = 0; i < QUESTPART_PROGR_V0::MAX_QUEST_PROGR_NUM; ++i)
	{   
        const QCODE QCode = pSrcStreamProgr[i].m_QCode;

        if( Between<QCODE>(QCode, 1,    92)   ||
            Between<QCODE>(QCode, 800,  930)  ||
            Between<QCODE>(QCode, 935,  986)  ||
            Between<QCODE>(QCode, 1014, 1099) ||
            Between<QCODE>(QCode, 1200, 1219) )
        {
			pDstStreamProgr[i].init();
		}
		else
			pDstStreamProgr[i] = pSrcStreamProgr[i];
	}
	return TRUE;
}

BOOL VersionControl::_QuestComplConvert( OUT QUESTPART_COMPL_V1* pDstStreamCompl, const QUESTPART_COMPL_V0* pSrcStreamCompl ) 
{
	// 0~1499번(1500) 까지의 기록은 일단 저장한다.
	memcpy( pDstStreamCompl, pSrcStreamCompl, QUESTPART_COMPL_V0::MAX_QUEST_COMPL_NUM );

	// 1500~ 3000(1501) 은 초기화
	ZeroMemory( &pDstStreamCompl[QUESTPART_COMPL_V0::MAX_QUEST_COMPL_NUM], 
		QUESTPART_COMPL_V1::MAX_QUEST_COMPL_NUM-QUESTPART_COMPL_V0::MAX_QUEST_COMPL_NUM );

	//사용되지 않는 퀘스트 대역 초기화
	ZeroMemory( &pDstStreamCompl[1], 93 );
	ZeroMemory( &pDstStreamCompl[800], 131 );
	ZeroMemory( &pDstStreamCompl[935], 52 );
	ZeroMemory( &pDstStreamCompl[1014], 86 );
	ZeroMemory( &pDstStreamCompl[1200], 20 );

	return TRUE;
}
//}__NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM