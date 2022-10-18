#ifndef __PROGRAMCOMMON_PLAYER_ATTRIBUTES_H
#define __PROGRAMCOMMON_PLAYER_ATTRIBUTES_H
#pragma once

#include "Attribute.h"

//==================================================================================================
//  @history
//      - 100827.4L, waverix, a server object is not derived a SCPlayer class anymore
//                            so that the argument for a SetOwner need change to a linker
//                            to control a player information
//      - 100828.1L, waverix, 
//
//#ifdef _SERVER
    struct IPlayerAttributeLinker; // for Server
//#else
//    class SCPlayer;
//    typedef class SCPlayer  IPlayerAttributeLinker;
//#endif
//
struct BASE_PLAYERINFO;

//==================================================================================================

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
// 아이템이 가진 소켓 옵션에 의해 변화된 속성값 목록
class SocketOptionAttributes
{
public:
    // 속성값
    struct AttrValue
    {
        int point; // 소켓 옵션의 포인트값
        int calc_value; // 포인트에 의해 계산된 속성값
        AttrValue() : point(0), calc_value(0) { }
    private:
        __DISABLE_COPY(AttrValue);
    };
public:
    SocketOptionAttributes();
    // 속성값 목록을 초기화한다.
    void Clear();
    // 속성값을 읽기 전용으로 얻는다.
    const AttrValue& GetAttrValue(const eATTR_TYPE attr_type) const;
    // 속성값을 쓰기 가능으로 얻는다.
    AttrValue& GetAttrValueByMutable(const eATTR_TYPE attr_type);
    // CHANGES: f110217.2L, add clone interface to support stat configuration preview
    void Clone(SocketOptionAttributes* dest);
private:
    AttrValue attr_values_[eATTR_MAX];
    __DISABLE_COPY(SocketOptionAttributes);
};
#endif // _NA_000251_20100727_SOCKET_SYSTEM

//==================================================================================================

//#if !defined(_SERVER) && defined(_NA_002935_20110704_ITEM_SCORE)
// 셋트 아이템 옵션에 의해 변화된 속성값 목록
class SetOptionAttributes
{
public:
    // 속성값
    struct AttrValue
    {
        int fix_value; // 고정형 값
        int ratio_value; // 비율형 값
        AttrValue() : fix_value(0), ratio_value(0) {}
    private:
        __DISABLE_COPY(AttrValue);
    };
public:
    SetOptionAttributes() {}
    // 속성값 목록을 초기화한다.
    void Clear();
    // 속성값을 읽기 전용으로 얻는다.
    const AttrValue& GetAttrValue(const eATTR_TYPE attr_type) const;

    // 속성값을 더한다.
    void AddAttr(const eATTR_TYPE attr_type, const eATTR_KIND attr_kind, const int attr_value);

#ifdef _NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST
    void SetSetGroupCode(const WORD& set_code) {applied_setoption_list_.push_back(set_code);}
    bool IsAppliedThisSetOption(const WORD& set_group_code);    //해당 세트옵션이 적용되고 있는가
#endif //_NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST
    // CHANGES: f110217.2L, add clone interface to support stat configuration preview
    void Clone(SetOptionAttributes* dest);
private:
    AttrValue attr_values_[eATTR_MAX];
#ifdef _NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST
    std::vector<WORD> applied_setoption_list_;
#endif //_NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST

    __DISABLE_COPY(SetOptionAttributes);
};
//#endif

//==================================================================================================

class PlayerAttributes : public Attributes
{
public :
    PlayerAttributes();
    virtual ~PlayerAttributes();
    // Attributes interface
    virtual void Clear();
    // Attributes interface, 단순히 Update만 수행한다.
    virtual void Update();
    // Attributes interface, 공속, 이속, 최대 HP, MP 변화가 있을때 클라에 알려준다.
    virtual void UpdateEx();
    //
    virtual void SetOwner(IPlayerAttributeLinker* owner);
    virtual void Display();
    //
    virtual void SetCharInfo(const BASE_PLAYERINFO* player_info,
                     int recover_hp, int recover_mp, int recover_sd);

    // NOTE, f100828.1L, change a name of 'GetCharInfo' to 'SynchronizeBaseStatus'.
    // this method use to synchronizing
    // between a base status value of BASE_PLAYERINFO and it of Attributes.
    void SynchronizeBaseStatus(); //GetCharInfo(BASE_PLAYERINFO* OUT pInfo);
    //
#if !defined(_SERVER)
    // NOTE, f100829.1L, rollback to support a client logic
    void GetCharInfo(BASE_PLAYERINFO* OUT player_info);
#endif
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    SocketOptionAttributes& GetSocketOptionAttributes();
    const SocketOptionAttributes& GetSocketOptionAttributes() const;
#endif // _NA_000251_20100727_SOCKET_SYSTEM
    // CHANGES: f110217.2L, add clone interface to support stat configuration preview
    void Clone(PlayerAttributes* dest);
    void CheckMemoryTest(); // CHANGES: f110802.2L, _NA000000_SPEED_HACK_PREVENTION_SUPPORT
#if defined(_SERVER)
    // CHANGES: f110413.1L, added interface to support light recovery quantity calculation for server
    // NOTE: HP, MP and SD recovery have busy update processes.
    // if server is to avoid busy updating, detach the recovery parts from full update calling.
    // the usage of this method is calling on changed recoveries in run-time service
    // not to initialize like the 'SetCharInfo'
    void UpdateChangedRecoveries(const BASE_PLAYERINFO* player_info,
        bool hp_update, int hp_recovery_quantity,
        bool mp_update, int mp_recovery_quantity,
        bool sd_update, int sd_recovery_quantity);
#endif
#ifdef _NA_002935_20110704_ITEM_SCORE
    SetOptionAttributes& GetSetOptionAttributes();
    const SetOptionAttributes& GetSetOptionAttributes() const;
#endif
    //
private:
    void UpdateAttackPower();
    void UpdateDefense();
    void UpdateRecoveries(eCHAR_TYPE char_type); // CHANGES: f110413.1L
    //
    IPlayerAttributeLinker* owner_;
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    SocketOptionAttributes socket_option_attributes_; // 아이템이 가진 소켓 옵션에 의해 변화된 속성값 목록
#endif
#ifdef _NA_002935_20110704_ITEM_SCORE
    SetOptionAttributes set_option_attributes_; // 아이템이 가진 세트 옵션에 의해 변화된 속성값 목록
#endif
};

//==================================================================================================
// implements
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
inline SocketOptionAttributes::SocketOptionAttributes()
{
}

// 속성값 목록을 초기화한다.
inline void SocketOptionAttributes::Clear()
{
    ::ZeroMemory(attr_values_, sizeof(attr_values_));
}

// 속성값을 읽기 전용으로 얻는다.
inline const SocketOptionAttributes::AttrValue& SocketOptionAttributes::GetAttrValue(
    const eATTR_TYPE attr_type) const
{
    assert(attr_type < eATTR_MAX);
    return attr_values_[attr_type];
}
// 속성값을 쓰기 가능으로 얻는다.
inline SocketOptionAttributes::AttrValue& SocketOptionAttributes::GetAttrValueByMutable(
    const eATTR_TYPE attr_type)
{
    assert(attr_type < eATTR_MAX);
    return attr_values_[attr_type];
}

// CHANGES: f110217.2L, add clone interface to support stat configuration preview
inline void SocketOptionAttributes::Clone(SocketOptionAttributes* dest)
{
    CopyMemory(dest->attr_values_, attr_values_, sizeof(dest->attr_values_));
}
#endif


inline void PlayerAttributes::Clear()
{
    RegisterAll();
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    socket_option_attributes_.Clear();
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#ifdef _NA_002935_20110704_ITEM_SCORE
    set_option_attributes_.Clear();
#endif
}

inline void PlayerAttributes::UpdateEx()
{
}

#if !defined(_SERVER)
inline void PlayerAttributes::SynchronizeBaseStatus()
{
    // Nothing to do
}

inline void PlayerAttributes::UpdateRecoveries(eCHAR_TYPE char_type)
{
    // Nothing to do
    __UNUSED(char_type);
}
#endif

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
inline SocketOptionAttributes& PlayerAttributes::GetSocketOptionAttributes()
{
    return socket_option_attributes_;
}
inline const SocketOptionAttributes& PlayerAttributes::GetSocketOptionAttributes() const
{
    return socket_option_attributes_;
}
#endif

inline void SetOptionAttributes::Clear()
{
#ifdef _NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST
    applied_setoption_list_.clear();
#endif //_NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST
    ::ZeroMemory(attr_values_, sizeof(attr_values_));
}

inline const SetOptionAttributes::AttrValue& SetOptionAttributes::GetAttrValue(
    const eATTR_TYPE attr_type) const
{
    assert(attr_type < _countof(attr_values_));
    return attr_values_[attr_type];
}

#ifdef _NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST

inline bool SetOptionAttributes::IsAppliedThisSetOption(const WORD& set_group_code)
{
    if (std::find(applied_setoption_list_.begin(), applied_setoption_list_.end(), set_group_code) 
        != applied_setoption_list_.end())
    {
        return true;
    }
    
    return false;
}
#endif //_NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST

inline void SetOptionAttributes::AddAttr(
    const eATTR_TYPE attr_type, const eATTR_KIND attr_kind, const int attr_value)
{
    assert(attr_type < _countof(attr_values_));
    AttrValue& attr = attr_values_[attr_type];
    if (attr_kind == eATTR_KIND_ITEM)
    {
        attr.fix_value += attr_value;
    }
    else if (attr_kind == eATTR_KIND_ITEM_RATIO)
    {
        attr.ratio_value += attr_value;
    }
    else
    {
        // 여기로 들어왔다면 ItemAttrCalculator쪽에서 사전 검토가 필요하다.
        assert(false);
    }
}

inline void SetOptionAttributes::Clone(SetOptionAttributes* dest)
{
    CopyMemory(dest->attr_values_, attr_values_, sizeof(dest->attr_values_));
}

#ifdef _NA_002935_20110704_ITEM_SCORE
inline SetOptionAttributes& PlayerAttributes::GetSetOptionAttributes()
{
    return set_option_attributes_;
}

inline const SetOptionAttributes& PlayerAttributes::GetSetOptionAttributes() const
{
    return set_option_attributes_;
}
#endif // _NA_002935_20110704_ITEM_SCORE

#endif //__PROGRAMCOMMON_PLAYER_ATTRIBUTES_H
