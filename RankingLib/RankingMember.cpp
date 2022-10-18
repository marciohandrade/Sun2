#include "stdafx.h"
#include "./RankingMember.h"

#include <ArchiveOfStream.h>

#include "./MissionRankingRecord.h"

//==================================================================================================

__IMPL_CUSTOMPOOL_PTR(RankingMember);

//==================================================================================================

bool RankingMember::DoReward()
{
    if (transaction_) {
        return false;
    };
    BeginTransaction();

    return true;
}

void RankingMember::SetCharName(const TCHAR* name)
{
    _tcsncpy(char_name_, name, _countof(char_name_));
    char_name_[_countof(char_name_) - 1] = _T('\0');

    if (char_name_[0] == _T('\0')) {
        char_name_hash_ = 0;
        return;
    };
    char_name_hash_ = MakeCharNameHashKey(char_name_);
}

// CHANGES: f110525.6L
DWORD RankingMember::MakeCharNameHashKey(const char* char_name)
{
    if (char_name == NULL || char_name[0] == '\0') {
        return 0;
    };
    TCHAR lower_char_name[_countof(((RankingMember*)0)->char_name_)];
    CopyMemory(lower_char_name, char_name, sizeof(lower_char_name));
    _tcslwr(lower_char_name);
    DWORD char_name_hash = util::StreamHashKey::GetStringHashKey(lower_char_name);
    return char_name_hash;
}

void RankingMember::Serialize(_MISSION_RANK_MEMBER_INFO& member_info, eSERIALIZE se)
{
    if (se == SERIALIZE_STORE)
    {
        this->Release();
        class_type_ = member_info.m_Class;
        level_      = member_info.m_LV;

        _tcsncpy(char_name_, member_info.m_tszCharName, _countof(char_name_));
        char_name_[_countof(char_name_) - 1] = '\0';
        _tcsncpy(guild_name_, member_info.m_tszGuildName, _countof(guild_name_));
        guild_name_[_countof(guild_name_) - 1] = '\0';
        //
        char_name_hash_ = MakeCharNameHashKey(char_name_);
    }
    else if (se == SERIALIZE_LOAD)
    {
        member_info.m_Class = class_type_;
        member_info.m_LV    = level_;
        _tcsncpy(member_info.m_tszCharName, char_name_, _countof(member_info.m_tszCharName));
        member_info.m_tszCharName[_countof(member_info.m_tszCharName) - 1] = '\0';
        _tcsncpy(member_info.m_tszGuildName, guild_name_, _countof(member_info.m_tszGuildName));
        member_info.m_tszGuildName[_countof(member_info.m_tszGuildName) - 1] = '\0';
    }
}

void RankingMember::Serialize(ArchiveOfStream& ar, eRANKING_SERIALIZE se)
{
    switch (se)
    {
    case RANKING_SERIALIZE_SAVE:
        // for server
        this->Release();
        //
        ar >> rewarded_;
        ar >> class_type_;
        ar >> level_;
        assert((level_ <= UCHAR_MAX) && "Check 'RANKING_SERIALIZE_CLIENT_~' level part");

        ;{
            BYTE len = 0;
            ar >> len;
            ar.Read(char_name_, len);
            char_name_[len] = '\0';
        };
        ;{
            BYTE len = 0;
            ar >> len;
            ar.Read(guild_name_, len);
            guild_name_[len] = '\0';
        };
        char_name_hash_ = MakeCharNameHashKey(char_name_);
        break;
    case RANKING_SERIALIZE_LOAD:
        // for server
        ar << rewarded_;
        ar << class_type_;
        ar << level_;
        assert((level_ <= UCHAR_MAX) && "Check 'RANKING_SERIALIZE_CLIENT_~' level part");

        ;{
            BYTE len = (BYTE)_tcslen(char_name_);
            ar << len;
            ar.Write(char_name_, len);
        };
        ;{
            BYTE len = (BYTE)_tcslen(guild_name_);
            ar << len;
            ar.Write(guild_name_, len);
        };
        break;
        //------------------------------------------------------------------------------------------
    case RANKING_SERIALIZE_CLIENT_SAVE:
        // for client
        // NOTE: f110512.2L, warns, level_type is resized to 1byte
        this->Release();
        // in client, the next fields is not control. fields = { rewarded_, class_type_ };
        ;{
            BYTE level = 0;
            ar >> level; // f110512.2L
            level_ = level;
        };
        ;{
            BYTE len = 0;
            ar >> len;
            ar.Read(char_name_, len);
            char_name_[len] = _T('\0');
        };
        ;{
            BYTE len=0;
            ar >> len;
            ar.Read(guild_name_, len);
            guild_name_[len] = _T('\0');
        };
        char_name_hash_ = MakeCharNameHashKey(char_name_);
        break;
    case RANKING_SERIALIZE_CLIENT_LOAD:
        // for client
        ar << BYTE(level_);
        ;{
            BYTE len = BYTE(_tcslen(char_name_));
            ar << len;
            ar.Write(char_name_, len);
        };
        ;{
            BYTE len = BYTE(_tcslen(guild_name_));
            ar << len;
            ar.Write(guild_name_, len);
        };
        break;
    }; //end 'switch'
}

#ifdef _SERVER
void RankingMember::Display() const
{
    CONSOLELOG(eCRITICAL_LOG,
        _T("  Class=%d, Reward=%d, Level=%d, CharName='%s', GuildName='%s'"),
        class_type_, rewarded_, level_, char_name_, guild_name_);
}
#endif //_SERVER