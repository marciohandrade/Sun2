#ifndef GAME_OPTION_ACCESSOR_H_
#define GAME_OPTION_ACCESSOR_H_

#if _MSC_VER > 1000
#pragma once
#endif


#ifdef _NA_0_20110329_GAMEOPTION_SETTING

#include <MemoryPoolFactory.h>

//===============================================================================================
//
// class GameOptionAccessor
//
//===============================================================================================
struct GameOptionEx : public GameOption
{
    enum
    {
        data_none = 0,
        data_loaded = (1 << 0),
        data_wait = (1 << 1),
    };

    uint user_guid;
    uint character_guid;
    uint accessor_guid;
    uint state;
};

// NOTE: f110609.9L, we need to look to ways of improving about interface usage
// for example, struct IGameOptionAccessor { virtual void init_gameoptionex(...) = 0; };
// class GameOptionAccessor : public IGameOptionAccessor
// class AutoHuntingAccessor : public IGameOptionAccessor
// and... it's possible that use composition object also.
// 

//===============================================================================================
//
// class GameOptionAccessor
//
//===============================================================================================
class GameOptionAccessor
{
public:
    GameOptionAccessor();
    virtual ~GameOptionAccessor();

    static void Initialize();
    static void Release();

    GameOptionEx* Find(User* user);
    GameOptionEx* Create(User* user);
    void Delete(User* user);

protected:
    virtual uint get_accessor_key(User* user) const = 0;
    virtual void init_gameoptionex(User* user, GameOptionEx* OUT gameoptionex) const = 0;

private:
    typedef util::CMemoryPoolFactory<GameOptionEx> GameOptionExPool;
    static GameOptionExPool  gameoptionex_pool_;
    STLX_HASH_MAP<uint, GameOptionEx*> hash_map_;
};



//===============================================================================================
//
// class 
//
//===============================================================================================

class AutoHuntingAccessor : public GameOptionAccessor
{
public:
    AutoHuntingAccessor();
    virtual ~AutoHuntingAccessor();

protected:
    virtual uint get_accessor_key(User* user) const;
    virtual void init_gameoptionex(User* user, GameOptionEx* OUT gameoptionex) const;
};

class KeyCustomizingAccessor : public GameOptionAccessor
{
public:
    KeyCustomizingAccessor();
    virtual ~KeyCustomizingAccessor();

protected:
    virtual uint get_accessor_key(User* user) const;
    virtual void init_gameoptionex(User* user, GameOptionEx* OUT gameoptionex) const;
};

// 계정 단위 사용자 가이드
class GuidePerUserAccessor : public GameOptionAccessor
{
public:
    GuidePerUserAccessor() {}
    virtual ~GuidePerUserAccessor() {}
protected:
    virtual uint get_accessor_key(User* user) const;
    virtual void init_gameoptionex(User* user, GameOptionEx* OUT option) const;
};

// 캐릭터 단위 사용자 가이드
class GuidePerCharAccessor : public GameOptionAccessor
{
public:
    GuidePerCharAccessor() {}
    virtual ~GuidePerCharAccessor() {}
protected:
    virtual uint get_accessor_key(User* user) const;
    virtual void init_gameoptionex(User* user, GameOptionEx* OUT option) const;
};

// 캐릭터 단위 알림창
class NoticePerCharAccessor : public GameOptionAccessor
{
public:
    NoticePerCharAccessor() {}
    virtual ~NoticePerCharAccessor() {}
protected:
    virtual uint get_accessor_key(User* user) const;
    virtual void init_gameoptionex(User* user, GameOptionEx* OUT option) const;
};

// 캐릭터 단위 단축키세팅
class CharacterKeyCustomizing : public GameOptionAccessor
{
public:
    CharacterKeyCustomizing() {}
    virtual ~CharacterKeyCustomizing() {}
protected:
    virtual uint get_accessor_key(User* user) const;
    virtual void init_gameoptionex(User* user, GameOptionEx* OUT option) const;
};

// 마지막으로 선택했던 캐릭터
class LastSelectedCharAccessor : public GameOptionAccessor
{
public:
    LastSelectedCharAccessor() {}
    virtual ~LastSelectedCharAccessor() {}
protected:
    virtual uint get_accessor_key(User* user) const;
    virtual void init_gameoptionex(User* user, GameOptionEx* OUT option) const;
};

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
class PerkListAccessor : public GameOptionAccessor
{
public:
    PerkListAccessor() {}
    virtual ~PerkListAccessor() {}
protected:
    virtual uint get_accessor_key(User* user) const;
    virtual void init_gameoptionex(User* user, GameOptionEx* OUT option) const;
};
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

#endif // _NA_0_20110329_GAMEOPTION_SETTING

#endif // GAME_OPTION_ACCESSOR_H_
