#pragma once

//=============================================================================================================================
/// Ʈ���� �޼��� ����
/**
    @author	Kim Min Wook < taiyo@webzen.com >
    @since	2004. 12. 29
    @remark
    - Ŭ���̾�Ʈ�� ���� �޴� Ʈ���� �޼��� ����
    @note
    - 
    @history 
    - 
*/
//=============================================================================================================================

#include <Const.h>
#include <LinkedListCS.h>
#include "./TriggerCommon.h"

enum eTRIGGER_MSG
{
    eTRIGGER_PICK_ITEM,     // ������ ȹ��
    eTRIGGER_KILL_MONSTER,  // ���� ����

    eTRIGGER_CLICK_OBJECT,
    eTRIGGER_ENTER_AREA,
    eTRIGGER_LEAVE_AREA,
    eTRIGGER_DAMAGE_OBJECT, 

    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    eTRIGGER_NPC_ENTER_AREA, // NPC�� Ư�� ������ ����
    eTRIGGER_NPC_LEAVE_AREA,

    //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    eTRIGGER_COMPLETE_COLLECTION,   //!< ä�� �Ϸ�

    // _NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER
    eTRIGGER_SERVER_EVENT,   //���� �̺�Ʈ

};

//==================================================================================================
//__NA000000_090613_TRIGGER_LOGIC_REARRANGE__
// NOTE: f110607.4L, prevent virtual function table address and inheritance list node mismatching

//==================================================================================================

struct TriggerLink : public util::__LList<TriggerLink>
{
    TriggerLink();
    ~TriggerLink(); // WARNING: non-virtual object. this object is only used in link management
    //
    DWORD object_key_;
    int area_id_;
};

// NOTE: f110311.3L, the trigger link node for player
struct TriggerPlayerLink : public TriggerLink
{
    TriggerPlayerLink();
    virtual ~TriggerPlayerLink();
    TriggerPlayerLink* Delete();
    util::__LListBase* base_node();
    virtual bool IsValidState() const;
};

// NOTE: f110311.4L, the trigger link node for npc
// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
struct TriggerNpcLink : public TriggerLink
{
    TriggerNpcLink();
    virtual ~TriggerNpcLink();
    TriggerNpcLink* Delete();
    util::__LListBase* base_node();
    virtual bool IsValidState() const;
};

//==================================================================================================

inline TriggerLink::TriggerLink()
    : object_key_(0)
    , area_id_(0)
{
    util::LList::Init(this);
}

inline TriggerLink::~TriggerLink()
{
    area_id_ = 0;
    util::LList::Delete(this);
}

inline TriggerPlayerLink::TriggerPlayerLink()
{
}

inline TriggerPlayerLink::~TriggerPlayerLink()
{
}

inline TriggerPlayerLink* TriggerPlayerLink::Delete()
{
    area_id_ = 0;
    return util::LList::Delete(this);
}

inline util::__LListBase* TriggerPlayerLink::base_node() {
    return this;
};

inline bool TriggerPlayerLink::IsValidState() const
{
    return false;
};


inline TriggerNpcLink::TriggerNpcLink()
{
}

inline TriggerNpcLink::~TriggerNpcLink()
{
}

inline TriggerNpcLink* TriggerNpcLink::Delete()
{
    area_id_ = 0;
    return util::LList::Delete(this);
}

inline util::__LListBase* TriggerNpcLink::base_node() {
    return this;
};

inline bool TriggerNpcLink::IsValidState() const
{
    return false;
};

//==================================================================================================

// (WAVERIX) (090831) (BUG-FIX) ���� ������ ���� Condition Trigger�� ���� �� ���� �� �ִ�.
// TriggerArea��� ���� ������ ��� �ؼ��� �ȵȴ�.
struct TriggerAreaLocalStorage : public util::__LList<TriggerAreaLocalStorage>
{
    WORD last_updated_number_of_chars_;

    TriggerAreaLocalStorage()
        : last_updated_number_of_chars_(0)
    {
        util::LList::Init(this);
    }

    ~TriggerAreaLocalStorage() {
        util::LList::Delete(this);
    }

    inline TriggerAreaLocalStorage* Delete() {
        return util::LList::Delete(this);
    }
};

// NOTE: f110311.4L, _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
struct TriggerAreaLocalStorageForNpc : public util::__LList<TriggerAreaLocalStorageForNpc>
{
    uint16_t last_updated_number_of_chars_;

    TriggerAreaLocalStorageForNpc()
        : last_updated_number_of_chars_(0)
    {
        util::LList::Init(this);
    }

    ~TriggerAreaLocalStorageForNpc() {
        util::LList::Delete(this);
    }

    inline TriggerAreaLocalStorageForNpc* Delete() {
        return util::LList::Delete(this);
    }
};
//
//==================================================================================================


struct TRIGGER_MSG
{
    DWORD               m_dwMsgID;
};

struct TRIGGER_PICK_ITEM : public TRIGGER_MSG
{
    TRIGGER_PICK_ITEM() { m_dwMsgID = eTRIGGER_PICK_ITEM; }
    DWORD m_dwObjectKey;
    DWORD m_dwItemCode;
};

struct TRIGGER_KILL_MONSTER : public TRIGGER_MSG
{
    TRIGGER_KILL_MONSTER() { m_dwMsgID = eTRIGGER_KILL_MONSTER; }
    DWORD m_dwObjectKey;    //< � �÷��̾ ���͸� �׿���?
    DWORD m_dwMonsterCode;
    DWORD m_MonsterUnitID;
};

struct TRIGGER_CLICK_OBJECT : public TRIGGER_MSG
{
    TRIGGER_CLICK_OBJECT() { m_dwMsgID = eTRIGGER_CLICK_OBJECT; }
    eOBJECT_TYPE m_eObjectType;
    DWORD m_dwObjectKey; //< �� ������Ʈ
    DWORD m_PlayerKey;   //(WAVERIX) (090621) (CHANGES)
};

// �÷��̾  � ������ ��� �Դ�.
struct TRIGGER_ENTER_AREA : public TRIGGER_MSG
{
    TRIGGER_ENTER_AREA() { m_dwMsgID = eTRIGGER_ENTER_AREA; }
    int   m_AreaID;
    DWORD m_PlayerKey;
    TriggerPlayerLink* m_pLink;
};
// �÷��̾  � ������ ������.
struct TRIGGER_LEAVE_AREA : public TRIGGER_MSG
{
    TRIGGER_LEAVE_AREA() { m_dwMsgID = eTRIGGER_LEAVE_AREA; }
    int   m_AreaID;
    DWORD m_PlayerKey;
    TriggerPlayerLink*  m_pLink;
};

struct TRIGGER_DAMAGE_OBJECT : public TRIGGER_MSG
{
    TRIGGER_DAMAGE_OBJECT() { m_dwMsgID = eTRIGGER_DAMAGE_OBJECT; }
    DWORD m_dwObjectKey;
};

// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
// NPC�� � ������ ��� �Դ�.
struct TRIGGER_NPC_ENTER_AREA : public TRIGGER_MSG
{
    TRIGGER_NPC_ENTER_AREA() : area_id(0), object_key(0) 
    {
        m_dwMsgID = eTRIGGER_NPC_ENTER_AREA;
    }
    CODETYPE area_id;
    CODETYPE object_key;
    TriggerNpcLink* link;
};

struct TRIGGER_NPC_LEAVE_AREA : public TRIGGER_MSG
{
    TRIGGER_NPC_LEAVE_AREA() : area_id(0), object_key(0) 
    {
        m_dwMsgID = eTRIGGER_NPC_LEAVE_AREA;
    }
    CODETYPE area_id;
    CODETYPE object_key;
    TriggerNpcLink* link;
};

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
struct TRIGGER_COMPLETE_COLLECTION : public TRIGGER_MSG
{
    TRIGGER_COMPLETE_COLLECTION() : object_key(0)
    {
        m_dwMsgID = eTRIGGER_COMPLETE_COLLECTION;
    }
    CODETYPE object_key;
};

//_NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER
struct TRIGGER_SERVER_EVENT : public TRIGGER_MSG
{
    TRIGGER_SERVER_EVENT() : event_id(-1)
    {
        m_dwMsgID = eTRIGGER_SERVER_EVENT;
    }
    int event_id;
};
