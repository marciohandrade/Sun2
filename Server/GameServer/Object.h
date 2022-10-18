#ifndef _GAMESERVER_OBJECT_H
#define _GAMESERVER_OBJECT_H
#pragma once

//==================================================================================================
// @history
//  ~ 2010/08/27 : ...
//  - 2010/08/27 : waverix, CHANGES: remove multiple inheritance hierarchy to easy maintain object
//  - 2011/02/21 : waverix, CHANGES: f110221.4L, upgrade to using search node
//
#include <Const.h>
#include <SCObject.h>
#include "IGridObject.h"

#include "SkillSystem/SearchLinkNode.h"
#include "SkillSystem/TargetSearcher.h"

//==================================================================================================
//
class GameZone;
class GameField;
class Sector;
struct WzVector;
struct MSG_BASE_FORWARD;

//==================================================================================================
//
class Object : public IObjectDispose,
               public IGridObject
{
public:
    typedef DWORD Key;
    static const WzVector kNullVector;

    Object();
    virtual ~Object();
    //==============================================================================================
    // IGridObject interfaces
    //
    // IGridObject interface, interface for field enter event
    virtual void OnEnterField(GameField& IN game_field, const WzVector& IN pos, int angle = -1);
    // IGridObject interface, interface for field leave event
    virtual void OnLeaveField();
    // IGridObject interface, call this when an object is entered in a sector of a field
    virtual void OnEnterSector();
    // IGridObject interface, this -> others notification
    virtual void OnEnterObject(const Sector& IN sector);
    // IGridObject interface, this -> others notification
    virtual void OnLeaveObject(const Sector& IN sector);
    // IGridObject interface, this <- other notification
    virtual void OnEnterNeighbor(const Sector& IN sector);
    // IGridObject interface, this <- other notification
    virtual void OnLeaveNeighbor(const Sector& IN sector);
    //
    //==============================================================================================
    // Object inheritable interfaces
    //
    // Object interface.
    virtual void Reuse();
    // Object interface. 
    virtual BOOL Update(DWORD delta_tick);
    // Object interface. this was defined an interface derived from a SCObject
    virtual void SetPos(const WzVector* current_pos);
    // Object interface. use a path explorer in a character class
    virtual void SetField(GameField* game_field);
    // Object interface.
    virtual void SetGameZone(eZONE_STATE zone_state, GameZone* game_zone,
                             MAPCODE enter_field_code, WzID area_id = 0, WzVector* start_pos = NULL);
    //
    //==============================================================================================
    //
    // (WAVERIX) (090607) (ADDIN) Unify object type matching pattern
    bool IsEqualObjectKind(eOBJECT_TYPE object_type) const;
    //
    eOBJECT_TYPE GetObjectType() const;
    void SetObjectType(eOBJECT_TYPE object_type);
    //
    Key GetObjectKey() const
    {
        return object_node_->object_key;
    }
    void SetObjectKey(const Key object_key)
    {
        object_node_->object_key = object_key;
    }
    //
    SECTOR_IDX GetSectorIndex() const;
    void       SetSectorIndex(const SECTOR_IDX coord);
    SECTOR_IDX GetOldSectorIndex() const;
    void       SetOldSectorIndex(const SECTOR_IDX coord);
    //
    GameField* GetField() const;
    //
    void GetPos(WzVector* result_pos) const;
    const WzVector* GetPosPtr() const;

    //__NA000000_090310_GAMEFIELD_RECURSIVELY_CALL_PROBLEM_FIX__
    void RequestDelete();
    bool IsDeleteReserved() const;
    bool IsDeleted() const
    {
        return is_deleted_;
    }

protected:
    //==============================================================================================
    // Object inheritable interfaces
    // Object interface.
    virtual BOOL Init();
    // Object interface.
    virtual void Release();
    //
    //==============================================================================================
    //
    // NOTE: f110306.2L, added by _NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    void _RegisterSearchReference(const ns_object::SearchReferences& search_reference);
    void _SetPosVector(const WzVector* position);
    //
    //==============================================================================================
private:
    //
    ns_object::SearchNode::ObjectNode* const object_node_;
    ns_object::SearchNode::LinkNode link_node_; //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    bool is_deleted_;
    //
    friend struct ns_object::IFF;
    friend struct ns_object::SearchOption;
    friend class Sector;
    __DISABLE_COPY(Object);
};

//==================================================================================================
// implements
//
// IGridObject interface, call this when an object is entered in a sector of a field
//virtual
inline void Object::OnEnterSector() {}
// IGridObject interface, this -> others notification
//virtual
inline void Object::OnEnterObject(const Sector& IN) {}
// IGridObject interface, this -> others notification
//virtual
inline void Object::OnLeaveObject(const Sector& IN) {}
// IGridObject interface, this <- other notification
//virtual
inline void Object::OnEnterNeighbor(const Sector& IN) {}
// IGridObject interface, this <- other notification
//virtual
inline void Object::OnLeaveNeighbor(const Sector& IN) {}
//
//==============================================================================================
//
// Object interface.
//virtual
inline void Object::Reuse()
{
    Release();
}

// Object interface. 
//virtual
inline BOOL Object::Update(DWORD delta_tick)
{
    __UNUSED(delta_tick);
    return object_node_->delete_reserved == false;
}

// Object interface. this was defined an interface derived from a SCObject
//virtual
inline void Object::SetPos(const WzVector* current_pos)
{
    __UNUSED(current_pos);
}

// Object interface.
//virtual
inline void Object::SetField(GameField* game_field)
{
    object_node_->game_field = game_field;
}  ///< Character의 SetField는 길찾기를 위한 코드가 더 들어간다. -> virtual

// Object interface.
//virtual
inline void Object::SetGameZone(eZONE_STATE zone_state, GameZone* game_zone,
                                MAPCODE enter_field_code, WzID area_id, WzVector* start_pos)
{
    __UNUSED((zone_state, game_zone, enter_field_code, area_id, start_pos));
}

//
//==============================================================================================
//

// (WAVERIX) (090607) (ADDIN) Unify object type matching pattern
inline bool Object::IsEqualObjectKind(eOBJECT_TYPE object_type) const
{
    return (object_node_->object_type & object_type) == object_type;
}

inline eOBJECT_TYPE Object::GetObjectType() const
{
    return object_node_->object_type;
}

inline void Object::SetObjectType(eOBJECT_TYPE object_type)
{
    object_node_->object_type = object_type;
}
//
inline SECTOR_IDX Object::GetSectorIndex() const
{
    return object_node_->sector_index_current;
}

inline void Object::SetSectorIndex(const SECTOR_IDX coord)
{
    object_node_->sector_index_current = coord;
}

inline SECTOR_IDX Object::GetOldSectorIndex() const
{
    return object_node_->sector_index_previous;
}

inline void Object::SetOldSectorIndex(const SECTOR_IDX coord)
{
    object_node_->sector_index_previous = coord;
}
//
inline GameField* Object::GetField() const
{
    return object_node_->game_field;
}
//
inline void Object::GetPos(WzVector* result_pos) const
{
    *result_pos = *object_node_->object_position;
}

inline const WzVector* Object::GetPosPtr() const
{
    return object_node_->object_position;
}

//virtual void SetPos(const WzVector* pwzVec){}

//__NA000000_090310_GAMEFIELD_RECURSIVELY_CALL_PROBLEM_FIX__
inline void Object::RequestDelete()
{
    object_node_->delete_reserved = true;
}

inline bool Object::IsDeleteReserved() const
{
    return object_node_->delete_reserved;
}

//==============================================================================================
// Object inheritable interfaces
// Object interface.
//virtual
inline BOOL Object::Init()
{
    SetField(NULL); // 객체 주변에 패킷 처리 후에 NULL 세팅!!

    object_node_->sector_index_current = INVALID_SECTOR_INDEX;

    object_node_->delete_reserved = false;
    return true;
}

// Object interface.
//virtual
inline void Object::Release()
{
    SetField(NULL);

    object_node_->sector_index_current = INVALID_SECTOR_INDEX;
    object_node_->sector_index_previous = INVALID_SECTOR_INDEX;

    SetPos(&kNullVector);
    object_node_->delete_reserved = true;
}
//
//==============================================================================================
//
inline void Object::_SetPosVector(const WzVector* position)
{
    object_node_->object_position = position;
}

//
//==============================================================================================
//

namespace obj_util {
    
bool IsValid(const Object* const obj, const char* function_str = NULL, const int line_number = 0);

} // obj_util

#endif //_GAMESERVER_OBJECT_H