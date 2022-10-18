#pragma once
#ifndef RANKING_LIB_STRUCT_H
#define RANKING_LIB_STRUCT_H

//==================================================================================================
#pragma pack(push,1)
//==================================================================================================

// Mission Clear Time
struct _MISSION_RANK_MEMBER_INFO
{
    USERGUID    m_UserGuid;
    //CHARGUID  m_CharGuid;
    TCHAR       m_tszCharName[MAX_CHARNAME_LENGTH + 1];
    TCHAR       m_tszGuildName[MAX_GUILDNAME_LENGTH + 1];
    LEVELTYPE   m_LV;
    BYTE        m_Class;
};

struct _MISSION_RECORD_INFO
{
    RANKINGNO   m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    DWORD       m_ClearTime;
    INT64       m_RegisterTime;
};

struct _MISSION_RANKING_TOTAL_INFO
{
    typedef _MISSION_RANK_MEMBER_INFO SLOT_TYPE;
    enum { MAX_SLOT_NUM = MAX_PARTYMEMBER_NUM, };
    _MISSION_RECORD_INFO m_Info;
    BYTE m_Count;
    SLOT_TYPE m_Member[MAX_SLOT_NUM];
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_Member);
        return static_cast<uint16>(kMinSize + sizeof(m_Member[0]) * m_Count);
    }
};

struct MISSION_RANK_MEMBER_INFO
{
    LEVELTYPE m_LV;
    BYTE      m_Class;
};

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
struct REWARDABLE_MISSION_LIST {
    REWARDABLE_MISSION_LIST()
    {
        ranking_id = 0;
        ranking_key = 0;
    }
    RANKINGID       ranking_id;
    RANKINGKEY      ranking_key;
};
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
//==================================================================================================
#pragma pack(pop)
//==================================================================================================

#endif //RANKING_LIB_STRUCT_H