#pragma once
#ifndef GAMESERVER_IGRID_OBJECT_H
#define GAMESERVER_IGRID_OBJECT_H

//==================================================================================================
/// IGridObject class
/**
    @author
        Kim Min Wook < taiyo@webzen.com >
    @since
        2006. 2. 22 ~
    @purpose
        각 오브젝트의 뷰포트 처리 함수의 인터페이스 처리를 위해 클래스 제작
    @history
        - ~100826
        - 100826, waverix, change to an interface type.
                           fix up a polymorphism problem about an OnEnterField method arguments.
*/
//==================================================================================================

class GameField;
class Sector;
class Object;

//==================================================================================================

struct IGridObject
{
    // IGridObject interface, interface for field enter event
    virtual void OnEnterField(GameField& IN game_field, const WzVector& IN pos, int angle) = 0;
    // IGridObject interface, interface for field leave event
    virtual void OnLeaveField() = 0;
    // IGridObject interface, call this when an object is entered in a sector of a field
    virtual void OnEnterSector() = 0;
    // IGridObject interface, this -> others notification
    virtual void OnEnterObject(const Sector& IN sector) = 0;
    // IGridObject interface, this -> others notification
    virtual void OnLeaveObject(const Sector& IN sector) = 0;
    // IGridObject interface, this <- other notification
    virtual void OnEnterNeighbor(const Sector& IN sector) = 0;
    // IGridObject interface, this <- other notification
    virtual void OnLeaveNeighbor(const Sector& IN sector) = 0;
};

#endif //GAMESERVER_IGRID_OBJECT_H