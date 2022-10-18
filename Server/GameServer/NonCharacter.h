#pragma once
#ifndef GAMESERVER_NONCHARACTOR_H
#define GAMESERVER_NONCHARACTOR_H
// NonCharacter.h: interface for the NonCharacter class.
//

#include "Object.h"

//==================================================================================================

class NonCharacter : public Object
{
public:
    NonCharacter();
    virtual ~NonCharacter();

    //==============================================================================================
    // IGridObject interfaces
    //
    // IGridObject interface, interface for field enter event
    virtual void OnEnterField(GameField& IN game_field, const WzVector& IN pos, int angle = -1);
    // IGridObject interface, interface for field leave event
    virtual void OnLeaveField();
    //// IGridObject interface, call this when an object is entered in a sector of a field
    //virtual void OnEnterSector();
    //// IGridObject interface, this -> others notification
    //virtual void OnEnterObject(const Sector& IN sector);
    //// IGridObject interface, this -> others notification
    //virtual void OnLeaveObject(const Sector& IN sector);
    //// IGridObject interface, this <- other notification
    //virtual void OnEnterNeighbor(const Sector& IN sector);
    //// IGridObject interface, this <- other notification
    //virtual void OnLeaveNeighbor(const Sector& IN sector);
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
    //// Object interface. use a path explorer in a character class
    //virtual void SetField(GameField* game_field);
    //// Object interface.
    //virtual void SetGameZone(eZONE_STATE zone_state, GameZone* game_zone,
    //                         MAPCODE enter_field_code, WzID area_id = 0, WzVector* start_pos = NULL);
    //
    //==============================================================================================
    // Object inheritable interfaces
    // Object interface.
    virtual BOOL Init();
protected:
    // Object interface.
    virtual void Release();
    // redefinition, inheritable virtual method, to use a call protector
    virtual void _SetPosVector(const WzVector* const pos_vector);

    WzVector m_wzVec; // 현재 위치
};

//==================================================================================================
//

inline NonCharacter::~NonCharacter()
{
}

//virtual
inline void NonCharacter::OnEnterField(GameField& IN game_field, const WzVector& IN pos, int angle)
{
    Object::OnEnterField(game_field, pos, angle);
}

//virtual
inline void NonCharacter::OnLeaveField()
{
    Object::OnLeaveField();
    Release();
}

//virtual
inline void NonCharacter::Reuse()
{
    Release();
}

//virtual
inline BOOL NonCharacter::Update(DWORD dwDeltaTick)
{
    return Object::Update(dwDeltaTick);
}

//virtual
inline void NonCharacter::SetPos(const WzVector* position) {
    m_wzVec = *position;
}

//virtual
inline BOOL NonCharacter::Init()
{
    return Object::Init();
}

//virtual
inline void NonCharacter::Release()
{
    Object::Release();
}

//virtual
inline void NonCharacter::_SetPosVector(const WzVector* const pos_vector)
{
    __UNUSED(pos_vector);
    __debugbreak();
}  // protector

#endif // !defined(GAMESERVER_NONCHARACTOR_H)
