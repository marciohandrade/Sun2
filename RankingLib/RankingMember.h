#pragma once
#ifndef RANKING_LIB_RANKING_MEMBER_H
#define RANKING_LIB_RANKING_MEMBER_H

//==================================================================================================
/// RankingMember class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2006. 8. 22
    @remark
        - 미션 하나당 랭킹 정보의 단위 클래스
    @note
        - 
    @history 
        - 2006. 8. 25 : 불필요한 charguid 제거
        - ~2011.05.12,
        - 2011.05.12, waverix, code rearrangements
*/
//==================================================================================================

class ArchiveOfStream;

class RankingManager;
class MissionRankingRecord;
//==================================================================================================
//
class RankingMember
{
    __DECL_CUSTOMPOOL_PTR(RankingMember);
public:
    RankingMember();
    ~RankingMember();

    void Clone(const RankingMember* ranking_member);
    void Release();
    //
    void BeginTransaction();
    void EndTransaction();
    // properties
    BYTE GetReward() const;
    BYTE GetClass() const;
    LEVELTYPE GetLV() const;
    DWORD GetCharNameHash() const;
    const TCHAR* GetCharName() const;
    const TCHAR* GetGuildName() const;

    void Serialize(ArchiveOfStream& ar, eRANKING_SERIALIZE se);
    void Serialize(_MISSION_RANK_MEMBER_INFO& info, eSERIALIZE se);

    bool DoReward();
#ifdef _SERVER
    void Display() const;
#endif
private:
    void SetReward(BYTE reward);
    void SetClass(BYTE class_type);
    void SetLV(LEVELTYPE level);
    void SetCharName(const TCHAR* name);
    void SetGuildName(const TCHAR* name);
    static DWORD MakeCharNameHashKey(const char* charname); // CHANGES: f110525.6L
    //
    // data fields...
    bool transaction_;
    BYTE rewarded_;
    BYTE class_type_;
    LEVELTYPE level_;
    DWORD char_name_hash_;
    //
    TCHAR char_name_[MAX_CHARNAME_LENGTH + 1];
    TCHAR guild_name_[MAX_GUILDNAME_LENGTH + 1];
    //
    friend class RankingManager;
    friend class MissionRankingRecord;
};

//==================================================================================================
// inline implements

inline RankingMember::RankingMember() {
    ZeroMemory(this, sizeof(*this)); // pod-style object
}

inline RankingMember::~RankingMember() {
}

inline void RankingMember::Release() {
    ZeroMemory(this, sizeof(*this)); // pod-style object
}

inline void RankingMember::Clone(const RankingMember* ranking_member) {
    *this = *ranking_member;
}

inline void RankingMember::BeginTransaction() {
    transaction_ = true;
}

inline void RankingMember::EndTransaction() {
    transaction_ = false;
}

inline DWORD RankingMember::GetCharNameHash() const {
    return char_name_hash_;
};

inline const TCHAR* RankingMember::GetCharName() const {
    return char_name_;
}

inline const TCHAR* RankingMember::GetGuildName() const {
    return guild_name_;
}

inline void RankingMember::SetGuildName(const TCHAR* name) {
    _tcsncpy(guild_name_, name, _countof(guild_name_));
    guild_name_[_countof(guild_name_) - 1] = _T('\0');
}

inline BYTE RankingMember::GetReward() const {
    return rewarded_;
};

inline BYTE RankingMember::GetClass() const {
    return class_type_;
};

inline LEVELTYPE RankingMember::GetLV() const {
    return level_;
};

inline void RankingMember::SetReward(BYTE reward) {
    rewarded_ = reward;
}

inline void RankingMember::SetClass(BYTE class_type) {
    class_type_ = class_type;
}

inline void RankingMember::SetLV(LEVELTYPE level) {
    level_ = level;
};


__inline void FREE_MEMBER(RankingMember* ranking_member)
{
    ranking_member->Release();
    RankingMember::FREE(ranking_member);
}

#endif //RANKING_LIB_RANKING_MEMBER_H