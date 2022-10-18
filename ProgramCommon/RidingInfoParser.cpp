#include "stdafx.h"
#include "./RidingInfoParser.h"

//==================================================================================================

RidingInfoParser::RidingInfoParser()
{
}

RidingInfoParser::~RidingInfoParser()
{
}

void RidingInfoParser::Init(const char* pack_file_name) // null이면 pack file없는 거당
{
    __UNUSED(pack_file_name);
}

void RidingInfoParser::Release()
{
}

BOOL RidingInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch (script_code)
    {
    case SCRIPT_RIDING_INFO:
        return _Load(reloaded != false);
    }
    return false;
}

bool RidingInfoParser::_Load(bool reloaded)
{
    const int max_rows = GetRowSize();

    for (int row = 0; row < max_rows; ++row)
    {
        DWORD index = GetDataDWORD("Index", row);
        FASSERT(index < USHRT_MAX);

        RiderTable::iterator it = rider_table_.find(index);
        if (it != rider_table_.end())
        {
            if (reloaded == false) {
                SUNLOG(eCRITICAL_LOG, "Script File Data Error, RidingInfoParser, Index = %d", index);
                FASSERT(!"데이터에 오류가 있습니다.");
            }
        }
        else
        {
            std::pair<RiderTable::iterator, bool> ret =
                rider_table_.insert(std::make_pair(index, sRiderInfo()));
            assert(ret.second);
            it = ret.first;
        }
        //
        sRiderInfo& dest = it->second;
        ZeroMemory(&dest, sizeof(dest));
        //
        dest.index_ = static_cast<SLOTCODE>(index);
        ;{
            int check_boundary = GetDataInt("SetSpeed", row);
            //FASSERT(0 <= check_boundary && check_boundary < 256);
            //dest.set_speed_ = (BYTE)check_boundary;
            dest.set_speed_ = check_boundary;
        };
        ;{
            bool spawn_area = !!GetDataBYTE("SpawnArea", row);
            dest.spawn_area_ = (spawn_area == false) ?
                (dest.eSpawn_Village | dest.eSpawn_Field | dest.eSpawn_Battle) :
                (dest.eSpawn_Village | dest.eSpawn_Field);
        };
    #ifdef _NA001955_110210_WING_COSTUME
        dest.flying_ = (GetDataInt("Flying", row) != 0);
    #endif //_NA001955_110210_WING_COSTUME
        dest.spawn_cancel_count_ = GetDataWORD("SpawnCancelCount", row);
        dest.cancel_count_timer_ = GetDataWORD("CountTimer", row);
        dest.cancel_count_timer_ *= 1000;
        dest.spawn_timer_ = GetDataWORD("SpawnTime", row);
        dest.spawn_timer_ *= 1000;
        //
        //------------------------------------------------------------------------------------------
        dest.monster_code_ = GetDataWORD("MonsterCode", row);
        strncpy(dest.anicode_, GetDataString( "AniMaping", row ), _countof(dest.anicode_));

        dest.base_skin[0] = GetDataDWORD("SetParts1", row);
        dest.base_skin[1] = GetDataDWORD("SetParts2", row);
        dest.base_skin[2] = GetDataDWORD("SetParts3", row);

        //dest.size_control_ = GetDataWORD("SizeControl", row);
        dest.size_control_ = GetDataFloat("SizeControl", row);
        dest.set_jump_ = GetDataDWORD("SetJump", row);
        dest.set_hangtime_  = GetDataDWORD("SetHangTime", row);
        //
        char effect_code_string[4];
        strncpy(effect_code_string, GetDataString("SpawnEffectCode", row),
                _countof(effect_code_string));
        dest.spawn_effect_code_= StrToWzID(effect_code_string);
        //
    }; //end read record 'for'
    //
    return true;
}

