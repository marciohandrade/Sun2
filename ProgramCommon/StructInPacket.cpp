#include "stdafx.h"
#include "./StructInPacket.h"
#include <ArchiveOfStream.h>

// (WAVERIX) (090920) (CHANGES) no-define work Output&Input -> moved to code-file

void
CLIENT_CHARACTER_PART::Output(ArchiveOfStream& archive)
{
    //write
    m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
    BYTE len = (BYTE)strlen(m_tszCharName);

    archive << m_bySlot;
    archive << len;
    archive.Write(m_tszCharName, len);
    archive << m_byHeight;
    archive << m_byFace;
    archive << m_byHair;
    archive << m_byClass;
    archive << m_LV;
    archive << m_iRegion;
    archive << m_sX;
    archive << m_sY;
    archive << m_sZ;
    archive << m_ChangeOfClassStep;
    archive << m_EquipItemInfo.m_Count;
    for(BYTE i = 0 ; i < m_EquipItemInfo.m_Count ; ++i)
    {
        archive << m_EquipItemInfo.m_Slot[i];
    }

    archive << m_DelChk;
    if(m_DelChk != 0)
    {
        m_tszDelDate[MAX_SMALLDATETIME_SIZE] = '\0';
        len = (BYTE)strlen(m_tszDelDate);
        archive << len;
        archive.Write(m_tszDelDate, len);
    }

    archive << m_PetSlotStream;
    archive << m_bSummoned;
#ifdef _NA_002050_20110216_ADD_GENDER
    archive << gender;
#endif 
    //_NA_004035_20120227_COSTUME_ITEM
    archive << is_equipped_costume;
    archive << costume_item_pos;
    archive << costume_item_code;
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    archive << is_equipped_decorate_costume;
    archive << decorate_costume_item_pos;
    archive << decorate_costume_item_code;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
}

void
CLIENT_CHARACTER_PART::Input(ArchiveOfStream& archive)
{
    //read
    archive >> m_bySlot;
    BYTE len = 0;
    archive >> len;
    archive.Read(m_tszCharName, len);
    m_tszCharName[len] = '\0';

    archive >> m_byHeight;
    archive >> m_byFace;
    archive >> m_byHair;
    archive >> m_byClass;
    archive >> m_LV;
    archive >> m_iRegion;
    archive >> m_sX;
    archive >> m_sY;
    archive >> m_sZ;
    archive >> m_ChangeOfClassStep;
    archive >> m_EquipItemInfo.m_Count;
    for(BYTE i = 0 ; i < m_EquipItemInfo.m_Count ; ++i)
    {
        archive >> m_EquipItemInfo.m_Slot[i];
    }
    archive >> m_DelChk;
    if(m_DelChk != 0)
    {
        archive >> len;
        archive.Read(m_tszDelDate, len);
        m_tszDelDate[len] = '\0';

    }

    archive >> m_PetSlotStream;
    archive >> m_bSummoned;
#ifdef _NA_002050_20110216_ADD_GENDER
    archive >> gender;
#endif 
    //_NA_004035_20120227_COSTUME_ITEM
    archive >> is_equipped_costume;
    archive >> costume_item_pos;
    archive >> costume_item_code;
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    archive >> is_equipped_decorate_costume;
    archive >> decorate_costume_item_pos;
    archive >> decorate_costume_item_code;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
}

int
PLAYER_RENDER_INFO_EX::ConvertForSend(BOOLEAN is_player_render,
                                      const PLAYER_RENDER_INFO_EX& input,
                                      PLAYER_RENDER_INFO_EX* packed_output)
{
    //ZeroMemory(encoded_output, sizeof(*this));
    ArchiveOfStream serializer((BYTE*)packed_output, sizeof(input));

    is_player_render = !!is_player_render;

    serializer << is_player_render;
    serializer << input.player_key;
    // (CHANGES) (WAVERIX) unify village render and player render
    serializer << input.hp;
    serializer << input.max_hp;
    serializer << input.level;


    BYTE len = (BYTE)strlen(input.char_name); //...
    serializer << len;
    assert(len != 0);
    serializer.Write(input.char_name, len);

    serializer << input.position;
    serializer << input.selected_style_code;
    serializer << input.move_speed_ratio;
    // (CHANGES) (WAVERIX) unify village render and player render
    serializer << input.attack_speed_ratio;

#define RENDER_BITS_MASK(used_bits) \
    ((1 << (used_bits)) - 1)

#define RENDER_BITS_PUSH(field, used_bits) { \
    bits <<= (used_bits); \
    bits |= (field) & RENDER_BITS_MASK(used_bits); \
    number_of_used_bits += (used_bits);}

    //------------------------------------------------
    ;{
        DWORD bits = 0, number_of_used_bits = 0;
    #if !defined(_NA_002050_20110216_ADD_GENDER)
        RENDER_BITS_PUSH(input.hair_style, 6);
    #endif
        RENDER_BITS_PUSH(input.transparent, 2);
    #if !defined(_NA_002050_20110216_ADD_GENDER)
        RENDER_BITS_PUSH(input.height, 3);
        RENDER_BITS_PUSH(input.face, 4);
    #endif
        RENDER_BITS_PUSH(input.helmet, 1);
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        RENDER_BITS_PUSH(input.wing, 1);
#endif //_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
        RENDER_BITS_PUSH(input.costume, 1);
#endif // _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
        RENDER_BITS_PUSH(input.gm_grade, 3);
        RENDER_BITS_PUSH(input.pcroom, 1);
        RENDER_BITS_PUSH(input.char_condition, 1);
        RENDER_BITS_PUSH(input.char_class, 4);
        RENDER_BITS_PUSH(input.boradori, 1);
        RENDER_BITS_PUSH(input.chao_state, 2);
        RENDER_BITS_PUSH(input.behave_state, 5);
    #if defined(_NA_002050_20110216_ADD_GENDER) && defined(_NA001955_110210_WING_COSTUME)
        RENDER_BITS_PUSH(input.spread_wings, 1);
    #endif
        assert(number_of_used_bits <= 32);
        serializer << bits;
    };
    //------------------------------------------------
#ifdef _NA_002050_20110216_ADD_GENDER
    ;{
        DWORD bits = 0, number_of_used_bits = 0;
        RENDER_BITS_PUSH(input.gender, 2);
        RENDER_BITS_PUSH(input.hair_style, 6);
        RENDER_BITS_PUSH(input.height, 3);
        RENDER_BITS_PUSH(input.face, 4);

        assert(number_of_used_bits <= 32);
        serializer << bits;
    };
#elif defined(_NA001955_110210_WING_COSTUME)
    serializer << input.spread_wings;
#endif
    //------------------------------------------------

    serializer << input.special_effects;

    BOOLEAN checked = !!input.is_summoned_pet;
    serializer << checked;
    if(checked) {
        serializer << input.pet_index;
        serializer << input.pet_fullness;
        serializer << input.pet_intimacy;
    }

    len = (BYTE)strlen(input.pet_name);
    serializer << len;
    if(len) {
        serializer.Write(input.pet_name, len);
    }

    serializer << input.ether_device_index;

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    serializer << input.honor_title;
#endif //_NA_003027_20111013_HONOR_SYSTEM
    
    //_NA_004035_20120227_COSTUME_ITEM
    serializer << input.is_equipped_costume;
    serializer << input.costume_item_code;

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    serializer << input.is_equipped_decorate_costume;
    serializer << input.decorate_costume_item_code;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    serializer << input.spa_ticket_code;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
    serializer << input.total_item_score_;
#endif // _NA_000000_20130206_BROADCAST_ITEM_SCORE

    checked = !!input.is_riding;
    serializer << checked;
    if(checked) {
        serializer << input.rider_item_slot;
    }

    BOOST_STATIC_ASSERT(sizeof(BOOLEAN) == sizeof(input.collection_render_info.m_bUseCollectRanderInfo));
    checked = !!input.collection_render_info.m_bUseCollectRanderInfo;
    if(checked == 0) {
        serializer << checked;
    }
    else {
        serializer << input.collection_render_info;
    }

    serializer.Write(&input.state_total_info, input.state_total_info.GetSize());

    return serializer.GetSize();
}

int
PLAYER_RENDER_INFO_EX::ConvertForRecv(const PLAYER_RENDER_INFO_EX* packed_input,
                                      PLAYER_RENDER_INFO_EX* output)
{
    const int SmallClearRange = static_cast<int>((char*)(&output->state_total_info) - (char*)output);
    ZeroMemory(output, SmallClearRange);
    ArchiveOfStream serializer((BYTE*)packed_input, sizeof(*packed_input));

    BOOLEAN is_player_render = 0;
    serializer >> is_player_render;
    serializer >> output->player_key;
    // (CHANGES) (WAVERIX) unify village render and player render
    serializer >> output->hp;
    serializer >> output->max_hp;
    serializer >> output->level;

    BYTE len = 0; //(BYTE)strlen(output->char_name); //...
    serializer >> len;
    assert(len != 0);
    serializer.Read(output->char_name, len);

    serializer >> output->position;
    serializer >> output->selected_style_code;
    serializer >> output->move_speed_ratio;
    // (CHANGES) (WAVERIX) unify village render and player render
    serializer >> output->attack_speed_ratio;

    //---- defined in 'ConvertForSend' ----
    //#define RENDER_BITS_MASK(used_bits) \
    //    ((1 << (used_bits)) - 1)
    //-------------------------------------
#define RENDER_BITS_POP(field, used_bits) { \
    (field) = (BYTE)((bits) & RENDER_BITS_MASK(used_bits)); \
    bits >>= (used_bits); } \

    //------------------------------------------------
    ;{
        DWORD bits = 0;
        // (WARNING) PUSH와 역순으로 풀어야 한다.
        serializer >> bits;
    #if defined(_NA_002050_20110216_ADD_GENDER) && defined(_NA001955_110210_WING_COSTUME)
        RENDER_BITS_POP(output->spread_wings, 1);
    #endif
        RENDER_BITS_POP(output->behave_state, 5);
        RENDER_BITS_POP(output->chao_state, 2);
        RENDER_BITS_POP(output->boradori, 1);
        RENDER_BITS_POP(output->char_class, 4);
        RENDER_BITS_POP(output->char_condition, 1);
        RENDER_BITS_POP(output->pcroom, 1);
        RENDER_BITS_POP(output->gm_grade, 3);
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
        RENDER_BITS_POP(output->costume, 1);
#endif // _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        RENDER_BITS_POP(output->wing, 1);
#endif
        RENDER_BITS_POP(output->helmet, 1);

    #if !defined(_NA_002050_20110216_ADD_GENDER)
        RENDER_BITS_POP(output->face, 4);
        RENDER_BITS_POP(output->height, 3);
    #endif
        RENDER_BITS_POP(output->transparent, 2);
    #if !defined(_NA_002050_20110216_ADD_GENDER)
        RENDER_BITS_POP(output->hair_style, 6);
    #endif
    };
    //------------------------------------------------
    //------------------------------------------------
#ifdef _NA_002050_20110216_ADD_GENDER
    ;{
        DWORD bits = 0;
        // (WARNING) PUSH와 역순으로 풀어야 한다.
        serializer >> bits;

        RENDER_BITS_POP(output->face, 4);
        RENDER_BITS_POP(output->height, 3);
        RENDER_BITS_POP(output->hair_style, 6);
        RENDER_BITS_POP(output->gender, 2);
    };
#elif defined(_NA001955_110210_WING_COSTUME)
    serializer >> output->spread_wings;
#endif

    serializer >> output->special_effects;

    serializer >> output->is_summoned_pet;
    if(output->is_summoned_pet) {
        serializer >> output->pet_index;
        serializer >> output->pet_fullness;
        serializer >> output->pet_intimacy;
    }
    len = 0;
    serializer >> len;
    if(len) {
        serializer.Read(output->pet_name, len);
    }

    serializer >> output->ether_device_index;

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    serializer >> output->honor_title;
#endif //_NA_003027_20111013_HONOR_SYSTEM
    
    //_NA_004035_20120227_COSTUME_ITEM
    serializer >> output->is_equipped_costume;
    serializer >> output->costume_item_code;
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    serializer >> output->is_equipped_decorate_costume;
    serializer >> output->decorate_costume_item_code;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    serializer >> output->spa_ticket_code;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
    serializer >> output->total_item_score_;
#endif // _NA_000000_20130206_BROADCAST_ITEM_SCORE

    serializer >> output->is_riding;
    if(output->is_riding) {
        serializer >> output->rider_item_slot;
    }

    BOOST_STATIC_ASSERT(sizeof(BOOLEAN) == \
                        sizeof(output->collection_render_info.m_bUseCollectRanderInfo));

    serializer >> output->collection_render_info.m_bUseCollectRanderInfo;
    if(output->collection_render_info.m_bUseCollectRanderInfo) {
        serializer.Seek(-(int)(sizeof(output->collection_render_info.m_bUseCollectRanderInfo)));
        serializer >> output->collection_render_info;
    }

    // (WARNING) 좀 더 좋은 방안을 강구하자.
    BOOST_STATIC_ASSERT(sizeof(BYTE) == sizeof(output->state_total_info.m_Count));
    serializer >> len;
    output->state_total_info.m_Count = len;
    if(len != 0) {
        serializer.Seek(-(int)(sizeof(output->state_total_info.m_Count)));
        serializer.Read(&output->state_total_info, output->state_total_info.GetSize());
    }

    return serializer.GetSize();
}

// (f100527.5L) moved a code block from a header file
void VENDOR_LIST_INFO::Output(ArchiveOfStream& archive) const
{
    //write
    BYTE len;
    len = (BYTE)strlen(m_pszTitle);
    archive << len;        
    archive.Write(m_pszTitle, len);
    len = (BYTE)strlen(m_pszID);
    archive << len;
    archive.Write(m_pszID, len);
    archive << m_dwPlayerKey;
}

void VENDOR_LIST_INFO::Input(const ArchiveOfStream& archive)
{
    //read
    BYTE len;
    archive >> len;
    archive.Read(m_pszTitle, len);
    m_pszTitle[MAX_VENDOR_TITLE_LENGTH] = '\0';
    archive >> len;
    archive.Read(m_pszID, len);
    m_pszID[ID_MAX_LEN] = '\0';
    archive >> m_dwPlayerKey;
}

void VENDOR_LIST_TOTAL_INFO::Output(ArchiveOfStream& archive) const
{
    //write
    archive << m_AllPage;
    archive << m_Count;
    for (BYTE i = 0 ; i < m_Count ; i++)
    {
        m_List[i].Output(archive);
    }
}

void VENDOR_LIST_TOTAL_INFO::Input(const ArchiveOfStream& archive)
{
    archive >> m_AllPage;
    archive >> m_Count;
    for (BYTE i = 0 ; i < m_Count ; i++)
    {
        m_List[i].Input(archive);
    }
}

//==================================================================================================
// in server
void nsSync::KBMoveSerialNode::Create()
{
    ZeroMemory(this, sizeof(*this));
    serial_guid = static_cast<uint16_t>(RandomNumberGenerator::GetRandomNumber());
    serial_key_begin = RandomNumberGenerator::GetRandomNumber();
    stop_serial_key_begin = RandomNumberGenerator::GetRandomNumber();
}

//==================================================================================================

